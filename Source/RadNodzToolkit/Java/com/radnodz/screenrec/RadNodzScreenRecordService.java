// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

// RadNodz screen recorder foreground service.
//
// 画面キャプチャ＋音声2系統（モニター再生音＋マイク送信音）のステレオミックスを
// MP4へ書き出し、ギャラリー(Movies/RIGDRED)へ保存する。
//
// 音声2系統をミックスするため MediaRecorder は使えない（音声入力1本のみ）。
// そのため以下の低レベル構成で実装する：
//   映像 : MediaProjection → VirtualDisplay → MediaCodec(H.264/AVC, Surface入力)
//   音声 : AudioPlaybackCapture(API29+, アプリ自身の再生音=モニター音) と
//          AudioRecord(マイク=送信音) を PCM で自前ミックス → MediaCodec(AAC, 2ch)
//   多重化: MediaMuxer（H.264 + AAC → MP4）
//   保存  : MediaStore（IS_PENDING で書き込み中フラグ管理、完了後にギャラリー公開）
//
// MediaProjection は API34+ で「type=mediaProjection の前面サービスが起動済み」で
// なければ getMediaProjection() が失敗するため、本サービスを起動→前面化してから
// プロジェクションを取得する順序で行う。マイクタイプも RECORD_AUDIO がある時だけ付与。
package com.radnodz.screenrec;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ServiceInfo;
import android.media.AudioAttributes;
import android.media.AudioFormat;
import android.media.AudioPlaybackCaptureConfiguration;
import android.media.AudioRecord;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
import android.media.MediaMuxer;
import android.media.MediaRecorder;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.ParcelFileDescriptor;
import android.provider.MediaStore;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Surface;
import android.view.WindowManager;

import java.nio.ByteBuffer;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class RadNodzScreenRecordService extends Service
{
	private static final String TAG = "RadNodzScreenRec";
	private static final String CHANNEL_ID = "rigdred_screenrec";
	private static final int NOTIFICATION_ID = 0x52445352; // 'RDSR'

	private static final String EXTRA_RESULT_CODE = "resultCode";
	private static final String EXTRA_RESULT_DATA = "resultData";
	private static final String EXTRA_INCLUDE_MIC = "includeMic";
	private static final String ACTION_START = "com.radnodz.screenrec.START";
	private static final String ACTION_STOP  = "com.radnodz.screenrec.STOP";

	// 音声フォーマット（両系統・出力とも共通）
	private static final int SAMPLE_RATE = 48000;
	private static final int CHANNELS    = 2;              // ステレオ出力
	private static final int AUDIO_BITRATE = 128000;

	// 映像フォーマット
	private static final int VIDEO_FRAME_RATE = 30;
	private static final int VIDEO_IFRAME_INTERVAL = 1;    // 秒

	// 録画中かどうか（C++/GameActivity から参照）。
	private static volatile boolean sRecording = false;
	// 直近の停止理由・エラー（デバッグ用）
	private static volatile String sLastError = "";

	public static boolean isRecording() { return sRecording; }
	public static String  getLastError() { return sLastError; }

	/** 前面サービスを起動して録画を開始する（権限取得済みの resultCode/data を渡す）。 */
	public static void start(Context ctx, int resultCode, Intent resultData, boolean includeMic)
	{
		Intent it = new Intent(ctx, RadNodzScreenRecordService.class);
		it.setAction(ACTION_START);
		it.putExtra(EXTRA_RESULT_CODE, resultCode);
		it.putExtra(EXTRA_RESULT_DATA, resultData);
		it.putExtra(EXTRA_INCLUDE_MIC, includeMic);
		if (Build.VERSION.SDK_INT >= 26) { ctx.startForegroundService(it); }
		else { ctx.startService(it); }
	}

	/** 録画を停止してサービスを終了する。 */
	public static void stop(Context ctx)
	{
		try
		{
			Intent it = new Intent(ctx, RadNodzScreenRecordService.class);
			it.setAction(ACTION_STOP);
			ctx.startService(it);
		}
		catch (Exception e) { Log.e(TAG, "stop() failed: " + e); }
	}

	// --- インスタンス状態 ---
	private MediaProjection mProjection;
	private MediaProjection.Callback mProjectionCb;
	private VirtualDisplayHolder mVDisplay;

	private MediaCodec mVideoEnc;
	private Surface    mVideoInputSurface;
	private Thread     mVideoThread;
	// 映像エンコーダの入力Surface（=VirtualDisplayミラー）は各フレームに
	// CLOCK_MONOTONIC由来の巨大な絶対値タイムスタンプを付ける。これを生のまま
	// muxerへ書くと、0開始の音声トラックとの差でムービー長(mvhd duration)が桁溢れし、
	// ギャラリーに「59:39:08」等のデタラメな再生時間が出る。先頭フレームのPTSを
	// 引いて映像トラックを0起点に正規化する。
	private long mVideoPtsOffsetUs = -1;

	private MediaCodec  mAudioEnc;
	private Thread      mAudioThread;
	private AudioRecord mPlaybackRec;   // アプリ再生音（モニター音）
	private AudioRecord mMicRec;        // マイク（送信音）

	private MediaMuxer mMuxer;
	private final Object mMuxerLock = new Object();
	private int mVideoTrack = -1;
	private int mAudioTrack = -1;
	private boolean mMuxerStarted = false;

	private ContentResolver mResolver;
	private Uri mOutputUri;
	private ParcelFileDescriptor mPfd;

	private volatile boolean mRunning = false;
	private boolean mIncludeMic = false;   // 録画にマイク音を含めるか（設定で切替）

	private int mWidth, mHeight, mDpi;

	@Override
	public IBinder onBind(Intent intent) { return null; }

	@Override
	public int onStartCommand(Intent intent, int flags, int startId)
	{
		final String action = (intent != null) ? intent.getAction() : null;
		if (ACTION_STOP.equals(action))
		{
			stopRecordingInternal("user stop");
			stopSelf();
			return START_NOT_STICKY;
		}

		if (sRecording)
		{
			Log.w(TAG, "already recording; ignore start");
			return START_NOT_STICKY;
		}

		int resultCode = (intent != null) ? intent.getIntExtra(EXTRA_RESULT_CODE, 0) : 0;
		Intent resultData = (intent != null) ? (Intent) intent.getParcelableExtra(EXTRA_RESULT_DATA) : null;
		mIncludeMic = (intent != null) && intent.getBooleanExtra(EXTRA_INCLUDE_MIC, false);
		if (resultData == null)
		{
			sLastError = "no projection data";
			Log.e(TAG, sLastError);
			stopSelf();
			return START_NOT_STICKY;
		}

		try
		{
			startForegroundNotification();

			// API34+ は前面サービス(type=mediaProjection)起動後でないと取得不可。
			MediaProjectionManager mpm =
				(MediaProjectionManager) getSystemService(Context.MEDIA_PROJECTION_SERVICE);
			mProjection = mpm.getMediaProjection(resultCode, resultData);
			if (mProjection == null) { throw new IllegalStateException("getMediaProjection returned null"); }

			// API34+ は VirtualDisplay 作成前に Callback 登録が必須。
			mProjectionCb = new MediaProjection.Callback()
			{
				@Override public void onStop()
				{
					// ユーザーがシステムの「共有停止」を押した等
					stopRecordingInternal("projection onStop");
					stopSelf();
				}
			};
			mProjection.registerCallback(mProjectionCb, new Handler(Looper.getMainLooper()));

			resolveDisplayMetrics();
			openOutputFile();

			setupVideo();
			setupAudio();

			mRunning = true;
			sRecording = true;
			sLastError = "";

			mVideoThread.start();
			mAudioThread.start();
			Log.d(TAG, "recording started " + mWidth + "x" + mHeight + " @" + mDpi + "dpi");
		}
		catch (Exception e)
		{
			sLastError = "start failed: " + e;
			Log.e(TAG, sLastError, e);
			stopRecordingInternal("start exception");
			stopSelf();
		}
		return START_NOT_STICKY;
	}

	// ================= セットアップ =================

	private void resolveDisplayMetrics()
	{
		DisplayMetrics dm = new DisplayMetrics();
		WindowManager wm = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
		wm.getDefaultDisplay().getRealMetrics(dm);
		mDpi = dm.densityDpi;
		// エンコーダの安定のため偶数に丸める。過大な解像度は帯域が跳ねるため長辺を1600pxに制限。
		int w = dm.widthPixels;
		int h = dm.heightPixels;
		final int maxLong = 1600;
		int longSide = Math.max(w, h);
		if (longSide > maxLong)
		{
			float scale = (float) maxLong / (float) longSide;
			w = Math.round(w * scale);
			h = Math.round(h * scale);
		}
		mWidth  = (w % 2 == 0) ? w : w - 1;
		mHeight = (h % 2 == 0) ? h : h - 1;
	}

	private void openOutputFile() throws Exception
	{
		mResolver = getContentResolver();
		String name = "RIGDRED_" +
			new SimpleDateFormat("yyyyMMdd_HHmmss", Locale.US).format(new Date()) + ".mp4";

		ContentValues cv = new ContentValues();
		cv.put(MediaStore.Video.Media.DISPLAY_NAME, name);
		cv.put(MediaStore.Video.Media.MIME_TYPE, "video/mp4");
		if (Build.VERSION.SDK_INT >= 29)
		{
			cv.put(MediaStore.Video.Media.RELATIVE_PATH, Environment.DIRECTORY_MOVIES + "/RIGDRED");
			cv.put(MediaStore.Video.Media.IS_PENDING, 1);
		}
		mOutputUri = mResolver.insert(MediaStore.Video.Media.EXTERNAL_CONTENT_URI, cv);
		if (mOutputUri == null) { throw new IllegalStateException("MediaStore insert failed"); }

		mPfd = mResolver.openFileDescriptor(mOutputUri, "rw");
		if (mPfd == null) { throw new IllegalStateException("openFileDescriptor failed"); }

		mMuxer = new MediaMuxer(mPfd.getFileDescriptor(), MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4);
	}

	private void setupVideo() throws Exception
	{
		mVideoPtsOffsetUs = -1;   // 録画ごとに先頭PTS基準をリセット
		int bitrate = (int) (mWidth * mHeight * VIDEO_FRAME_RATE * 0.12f); // おおよそ画素×fps×係数
		bitrate = Math.max(3_000_000, Math.min(bitrate, 12_000_000));

		MediaFormat fmt = MediaFormat.createVideoFormat(MediaFormat.MIMETYPE_VIDEO_AVC, mWidth, mHeight);
		fmt.setInteger(MediaFormat.KEY_COLOR_FORMAT,
			MediaCodecInfo.CodecCapabilities.COLOR_FormatSurface);
		fmt.setInteger(MediaFormat.KEY_BIT_RATE, bitrate);
		fmt.setInteger(MediaFormat.KEY_FRAME_RATE, VIDEO_FRAME_RATE);
		fmt.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, VIDEO_IFRAME_INTERVAL);

		mVideoEnc = MediaCodec.createEncoderByType(MediaFormat.MIMETYPE_VIDEO_AVC);
		mVideoEnc.configure(fmt, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
		mVideoInputSurface = mVideoEnc.createInputSurface();
		mVideoEnc.start();

		mVDisplay = new VirtualDisplayHolder(mProjection.createVirtualDisplay(
			"RadNodzScreenRec", mWidth, mHeight, mDpi,
			android.hardware.display.DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
			mVideoInputSurface, null, null));

		mVideoThread = new Thread(this::videoLoop, "RadNodzVideoEnc");
	}

	private void setupAudio() throws Exception
	{
		// --- モニター再生音：アプリ自身の再生をキャプチャ ---
		AudioPlaybackCaptureConfiguration cfg =
			new AudioPlaybackCaptureConfiguration.Builder(mProjection)
				.addMatchingUsage(AudioAttributes.USAGE_MEDIA)
				.addMatchingUsage(AudioAttributes.USAGE_GAME)
				.addMatchingUsage(AudioAttributes.USAGE_UNKNOWN)
				.build();

		AudioFormat stereoFmt = new AudioFormat.Builder()
			.setEncoding(AudioFormat.ENCODING_PCM_16BIT)
			.setSampleRate(SAMPLE_RATE)
			.setChannelMask(AudioFormat.CHANNEL_IN_STEREO)
			.build();

		int pbMin = AudioRecord.getMinBufferSize(SAMPLE_RATE,
			AudioFormat.CHANNEL_IN_STEREO, AudioFormat.ENCODING_PCM_16BIT);
		mPlaybackRec = new AudioRecord.Builder()
			.setAudioFormat(stereoFmt)
			.setBufferSizeInBytes(Math.max(pbMin, 4 * 4096))
			.setAudioPlaybackCaptureConfig(cfg)
			.build();
		if (mPlaybackRec.getState() != AudioRecord.STATE_INITIALIZED)
		{
			throw new IllegalStateException("playback capture AudioRecord not initialized");
		}

		// --- マイク送信音：マイク入力（UE側でもマイクを使用中の場合がある。
		//     同一アプリ内での同時キャプチャの可否は端末に依存する）---
		AudioFormat monoFmt = new AudioFormat.Builder()
			.setEncoding(AudioFormat.ENCODING_PCM_16BIT)
			.setSampleRate(SAMPLE_RATE)
			.setChannelMask(AudioFormat.CHANNEL_IN_MONO)
			.build();
		int micMin = AudioRecord.getMinBufferSize(SAMPLE_RATE,
			AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);
		mMicRec = null;
		if (mIncludeMic && checkSelfPermission(android.Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED)
		{
			try
			{
				mMicRec = new AudioRecord.Builder()
					.setAudioSource(MediaRecorder.AudioSource.VOICE_COMMUNICATION)
					.setAudioFormat(monoFmt)
					.setBufferSizeInBytes(Math.max(micMin, 4 * 4096))
					.build();
				if (mMicRec.getState() != AudioRecord.STATE_INITIALIZED)
				{
					Log.w(TAG, "mic AudioRecord not initialized; recording playback only");
					mMicRec.release();
					mMicRec = null;
				}
			}
			catch (Exception e)
			{
				Log.w(TAG, "mic AudioRecord failed; recording playback only: " + e);
				mMicRec = null;
			}
		}

		// --- AACエンコーダ（ステレオ）---
		MediaFormat afmt = MediaFormat.createAudioFormat(
			MediaFormat.MIMETYPE_AUDIO_AAC, SAMPLE_RATE, CHANNELS);
		afmt.setInteger(MediaFormat.KEY_AAC_PROFILE, MediaCodecInfo.CodecProfileLevel.AACObjectLC);
		afmt.setInteger(MediaFormat.KEY_BIT_RATE, AUDIO_BITRATE);
		afmt.setInteger(MediaFormat.KEY_MAX_INPUT_SIZE, 16 * 1024);
		mAudioEnc = MediaCodec.createEncoderByType(MediaFormat.MIMETYPE_AUDIO_AAC);
		mAudioEnc.configure(afmt, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
		mAudioEnc.start();

		mAudioThread = new Thread(this::audioLoop, "RadNodzAudioEnc");
	}

	// ================= 映像ループ =================

	private void videoLoop()
	{
		MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
		try
		{
			while (mRunning)
			{
				int idx = mVideoEnc.dequeueOutputBuffer(info, 10_000);
				if (idx == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED)
				{
					synchronized (mMuxerLock)
					{
						mVideoTrack = mMuxer.addTrack(mVideoEnc.getOutputFormat());
						maybeStartMuxer();
					}
				}
				else if (idx >= 0)
				{
					ByteBuffer buf = mVideoEnc.getOutputBuffer(idx);
					if (buf != null && info.size > 0 &&
						(info.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG) == 0)
					{
						// 先頭フレームのPTSを基準に0起点へ正規化（ムービー長の桁溢れ対策）。
						if (mVideoPtsOffsetUs < 0) { mVideoPtsOffsetUs = info.presentationTimeUs; }
						info.presentationTimeUs -= mVideoPtsOffsetUs;
						if (info.presentationTimeUs < 0) { info.presentationTimeUs = 0; }
						synchronized (mMuxerLock)
						{
							if (mMuxerStarted)
							{
								buf.position(info.offset);
								buf.limit(info.offset + info.size);
								mMuxer.writeSampleData(mVideoTrack, buf, info);
							}
						}
					}
					mVideoEnc.releaseOutputBuffer(idx, false);
				}
				if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) { break; }
			}
		}
		catch (Exception e)
		{
			sLastError = "video loop: " + e;
			Log.e(TAG, sLastError, e);
		}
	}

	// ================= 音声ループ（2系統ミックス→AAC）=================

	private void audioLoop()
	{
		final int framesPerChunk = 1024;
		final short[] pbBuf  = new short[framesPerChunk * 2];   // ステレオ
		final short[] micBuf = new short[framesPerChunk];       // モノ
		final byte[]  outBuf = new byte[framesPerChunk * 2 * 2]; // ステレオ16bit
		long totalFrames = 0;

		MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
		try
		{
			mPlaybackRec.startRecording();
			if (mMicRec != null) { mMicRec.startRecording(); }

			while (mRunning)
			{
				// モニター音（ステレオ）
				int pbShorts = readFully(mPlaybackRec, pbBuf, pbBuf.length);
				// マイク音（モノ）
				int micShorts = (mMicRec != null) ? readFully(mMicRec, micBuf, micBuf.length) : 0;

				// ミックスしてバイト列へ（L/R ともに モニター + マイク をクリップ付き加算）
				int outBytes = 0;
				for (int i = 0; i < framesPerChunk; i++)
				{
					int l = (i * 2     < pbShorts) ? pbBuf[i * 2]     : 0;
					int r = (i * 2 + 1 < pbShorts) ? pbBuf[i * 2 + 1] : 0;
					int m = (i < micShorts) ? micBuf[i] : 0;
					int outL = clip16(l + m);
					int outR = clip16(r + m);
					outBuf[outBytes++] = (byte) (outL & 0xff);
					outBuf[outBytes++] = (byte) ((outL >> 8) & 0xff);
					outBuf[outBytes++] = (byte) (outR & 0xff);
					outBuf[outBytes++] = (byte) ((outR >> 8) & 0xff);
				}

				// AACエンコーダへ投入
				int inIdx = mAudioEnc.dequeueInputBuffer(10_000);
				if (inIdx >= 0)
				{
					ByteBuffer in = mAudioEnc.getInputBuffer(inIdx);
					in.clear();
					in.put(outBuf, 0, outBytes);
					long ptsUs = totalFrames * 1_000_000L / SAMPLE_RATE;
					mAudioEnc.queueInputBuffer(inIdx, 0, outBytes, ptsUs, 0);
					totalFrames += framesPerChunk;
				}

				drainAudioEncoder(info, false);
			}

			// 終端フラグを流して残りを吐き出す
			int inIdx = mAudioEnc.dequeueInputBuffer(10_000);
			if (inIdx >= 0)
			{
				long ptsUs = totalFrames * 1_000_000L / SAMPLE_RATE;
				mAudioEnc.queueInputBuffer(inIdx, 0, 0, ptsUs, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
			}
			drainAudioEncoder(info, true);
		}
		catch (Exception e)
		{
			sLastError = "audio loop: " + e;
			Log.e(TAG, sLastError, e);
		}
		finally
		{
			try { mPlaybackRec.stop(); } catch (Exception ignored) {}
			if (mMicRec != null) { try { mMicRec.stop(); } catch (Exception ignored) {} }
		}
	}

	private void drainAudioEncoder(MediaCodec.BufferInfo info, boolean endOfStream)
	{
		while (true)
		{
			int idx = mAudioEnc.dequeueOutputBuffer(info, endOfStream ? 10_000 : 0);
			if (idx == MediaCodec.INFO_TRY_AGAIN_LATER)
			{
				if (!endOfStream) { break; }
				// EOS待ちは少しだけスピン
				continue;
			}
			else if (idx == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED)
			{
				synchronized (mMuxerLock)
				{
					mAudioTrack = mMuxer.addTrack(mAudioEnc.getOutputFormat());
					maybeStartMuxer();
				}
			}
			else if (idx >= 0)
			{
				ByteBuffer buf = mAudioEnc.getOutputBuffer(idx);
				if (buf != null && info.size > 0 &&
					(info.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG) == 0)
				{
					synchronized (mMuxerLock)
					{
						if (mMuxerStarted)
						{
							buf.position(info.offset);
							buf.limit(info.offset + info.size);
							mMuxer.writeSampleData(mAudioTrack, buf, info);
						}
					}
				}
				mAudioEnc.releaseOutputBuffer(idx, false);
				if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) { break; }
			}
		}
	}

	// muxerLock を保持した状態で呼ぶこと。
	private void maybeStartMuxer()
	{
		if (!mMuxerStarted && mVideoTrack >= 0 && mAudioTrack >= 0)
		{
			mMuxer.start();
			mMuxerStarted = true;
			// muxer開始直後に映像のキーフレームを要求し、再生開始位置にIDRを置く。
			try
			{
				android.os.Bundle b = new android.os.Bundle();
				b.putInt(MediaCodec.PARAMETER_KEY_REQUEST_SYNC_FRAME, 0);
				mVideoEnc.setParameters(b);
			}
			catch (Exception ignored) {}
		}
	}

	// AudioRecord から要求サイズぶん読む（ブロッキング）。読めた short 数を返す。
	private int readFully(AudioRecord rec, short[] buf, int size)
	{
		int read = 0;
		while (read < size && mRunning)
		{
			int n = (Build.VERSION.SDK_INT >= 23)
				? rec.read(buf, read, size - read, AudioRecord.READ_BLOCKING)
				: rec.read(buf, read, size - read);
			if (n <= 0) { break; }
			read += n;
		}
		return read;
	}

	private static int clip16(int v)
	{
		if (v > 32767) { return 32767; }
		if (v < -32768) { return -32768; }
		return v;
	}

	// ================= 停止・終了処理 =================

	private synchronized void stopRecordingInternal(String reason)
	{
		if (!mRunning && !sRecording && mMuxer == null) { return; }
		Log.d(TAG, "stopRecording: " + reason);
		mRunning = false;
		sRecording = false;

		// 映像エンコーダへEOSを通知（Surface入力）
		try { if (mVideoEnc != null) { mVideoEnc.signalEndOfInputStream(); } } catch (Exception ignored) {}

		joinQuietly(mVideoThread);
		joinQuietly(mAudioThread);
		mVideoThread = null;
		mAudioThread = null;

		// VirtualDisplay / Projection
		if (mVDisplay != null) { mVDisplay.release(); mVDisplay = null; }
		if (mProjection != null)
		{
			try { if (mProjectionCb != null) { mProjection.unregisterCallback(mProjectionCb); } } catch (Exception ignored) {}
			try { mProjection.stop(); } catch (Exception ignored) {}
			mProjection = null;
			mProjectionCb = null;
		}

		// エンコーダ解放
		releaseCodec(mVideoEnc); mVideoEnc = null;
		if (mVideoInputSurface != null) { try { mVideoInputSurface.release(); } catch (Exception ignored) {} mVideoInputSurface = null; }
		releaseCodec(mAudioEnc); mAudioEnc = null;

		releaseAudioRecord(mPlaybackRec); mPlaybackRec = null;
		releaseAudioRecord(mMicRec); mMicRec = null;

		// Muxer解放。有効な録画データが書けたか（muxer開始まで到達したか）を控える。
		boolean produced;
		synchronized (mMuxerLock)
		{
			produced = mMuxerStarted;
			if (mMuxer != null)
			{
				try { if (mMuxerStarted) { mMuxer.stop(); } } catch (Exception e) { Log.w(TAG, "muxer.stop: " + e); produced = false; }
				try { mMuxer.release(); } catch (Exception ignored) {}
				mMuxer = null;
			}
			mMuxerStarted = false;
			mVideoTrack = -1;
			mAudioTrack = -1;
		}

		// 出力ファイルの後始末（IS_PENDING解除でギャラリー公開／データ無しなら削除）
		finalizeOutput(produced);
	}

	private void finalizeOutput(boolean produced)
	{
		if (mPfd != null) { try { mPfd.close(); } catch (Exception ignored) {} mPfd = null; }
		if (mOutputUri != null && mResolver != null)
		{
			try
			{
				if (!produced)
				{
					// 映像/音声が1フレームも書けなかった（即停止・初期化失敗等）。
					// 0バイトMP4をギャラリーに残さないよう削除する。
					mResolver.delete(mOutputUri, null, null);
					Log.w(TAG, "no data produced; deleted pending entry");
				}
				else if (Build.VERSION.SDK_INT >= 29)
				{
					ContentValues cv = new ContentValues();
					cv.put(MediaStore.Video.Media.IS_PENDING, 0);
					mResolver.update(mOutputUri, cv, null, null);
					Log.d(TAG, "saved to gallery: " + mOutputUri);
				}
			}
			catch (Exception e) { Log.e(TAG, "finalize failed: " + e); }
		}
		mOutputUri = null;
		mResolver = null;
	}

	private static void joinQuietly(Thread t)
	{
		if (t == null) { return; }
		try { t.join(3000); } catch (InterruptedException ignored) {}
	}

	private static void releaseCodec(MediaCodec c)
	{
		if (c == null) { return; }
		try { c.stop(); } catch (Exception ignored) {}
		try { c.release(); } catch (Exception ignored) {}
	}

	private static void releaseAudioRecord(AudioRecord r)
	{
		if (r == null) { return; }
		try { if (r.getRecordingState() == AudioRecord.RECORDSTATE_RECORDING) { r.stop(); } } catch (Exception ignored) {}
		try { r.release(); } catch (Exception ignored) {}
	}

	// VirtualDisplay をAPIレベル差異を吸収して保持・解放する薄いホルダ。
	private static final class VirtualDisplayHolder
	{
		private final android.hardware.display.VirtualDisplay vd;
		VirtualDisplayHolder(android.hardware.display.VirtualDisplay v) { vd = v; }
		void release() { if (vd != null) { try { vd.release(); } catch (Exception ignored) {} } }
	}

	// ================= 前面サービス通知 =================

	private void startForegroundNotification()
	{
		NotificationManager nm = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
		if (Build.VERSION.SDK_INT >= 26)
		{
			NotificationChannel ch = new NotificationChannel(
				CHANNEL_ID, "Screen recording", NotificationManager.IMPORTANCE_LOW);
			ch.setShowBadge(false);
			nm.createNotificationChannel(ch);
		}
		Notification.Builder b = (Build.VERSION.SDK_INT >= 26)
			? new Notification.Builder(this, CHANNEL_ID)
			: new Notification.Builder(this);
		b.setContentTitle("RIGDRED");
		b.setContentText("画面録画中 (Recording)");
		b.setSmallIcon(getApplicationInfo().icon);
		b.setOngoing(true);
		Notification n = b.build();

		if (Build.VERSION.SDK_INT >= 29)
		{
			int types = ServiceInfo.FOREGROUND_SERVICE_TYPE_MEDIA_PROJECTION;
			if (mIncludeMic && checkSelfPermission(android.Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED)
			{
				types |= ServiceInfo.FOREGROUND_SERVICE_TYPE_MICROPHONE;
			}
			startForeground(NOTIFICATION_ID, n, types);
		}
		else
		{
			startForeground(NOTIFICATION_ID, n);
		}
	}

	@Override
	public void onTaskRemoved(Intent rootIntent)
	{
		stopRecordingInternal("task removed");
		stopSelf();
		super.onTaskRemoved(rootIntent);
	}

	@Override
	public void onDestroy()
	{
		stopRecordingInternal("onDestroy");
		super.onDestroy();
	}
}
