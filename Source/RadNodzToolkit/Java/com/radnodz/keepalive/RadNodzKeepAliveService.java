// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

// RadNodz keep-alive foreground service.
//
// モニター再生・マイク送信・グループトーク中に画面を消したりアプリを
// バックグラウンドへ回しても音声処理を続けられるように、
//   - 前面サービス（microphone / mediaPlayback タイプ）でプロセスの凍結
//     （cached-app freezer）とマイク入力の遮断を防ぎ、
//   - PARTIAL WakeLock で画面OFF中もCPUを維持し、
//   - WifiLock でWiFiの省電力スリープによる音声途切れを防ぐ。
// サービス自体は音声処理を行わない（実体はUnreal側のスレッドが継続する）。
package com.radnodz.keepalive;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ServiceInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.IBinder;
import android.os.PowerManager;
import android.util.Log;

public class RadNodzKeepAliveService extends Service
{
	private static final String TAG = "RadNodzKeepAlive";
	private static final String CHANNEL_ID = "rigdred_keepalive";
	private static final int NOTIFICATION_ID = 0x52444E4B; // 'RDNK'

	private PowerManager.WakeLock WakeLock;
	private WifiManager.WifiLock WifiLock;

	@Override
	public IBinder onBind(Intent intent) { return null; }

	@Override
	public int onStartCommand(Intent intent, int flags, int startId)
	{
		try
		{
			startForegroundWithTypes();
			acquireLocks();
			Log.d(TAG, "keep-alive service started");
		}
		catch (Exception e)
		{
			Log.e(TAG, "failed to start foreground: " + e);
			stopSelf();
		}
		// 音声継続はアプリ操作で明示的に始まる機能なので、OSに殺されたときの自動再起動はしない
		return START_NOT_STICKY;
	}

	private void startForegroundWithTypes()
	{
		NotificationManager nm = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
		if (Build.VERSION.SDK_INT >= 26)
		{
			NotificationChannel ch = new NotificationChannel(
				CHANNEL_ID, "Audio keep-alive", NotificationManager.IMPORTANCE_LOW);
			ch.setShowBadge(false);
			nm.createNotificationChannel(ch);
		}

		Notification.Builder b = (Build.VERSION.SDK_INT >= 26)
			? new Notification.Builder(this, CHANNEL_ID)
			: new Notification.Builder(this);
		b.setContentTitle("RIGDRED");
		b.setContentText("モニター/通話を継続中 (Audio active)");
		b.setSmallIcon(getApplicationInfo().icon);
		b.setOngoing(true);
		Notification notification = b.build();

		if (Build.VERSION.SDK_INT >= 29)
		{
			// マイクタイプはRECORD_AUDIO許可が無いと開始できない(API34+)ため、許可がある時だけ付ける。
			int types = ServiceInfo.FOREGROUND_SERVICE_TYPE_MEDIA_PLAYBACK;
			if (checkSelfPermission(android.Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED)
			{
				types |= ServiceInfo.FOREGROUND_SERVICE_TYPE_MICROPHONE;
			}
			startForeground(NOTIFICATION_ID, notification, types);
		}
		else
		{
			startForeground(NOTIFICATION_ID, notification);
		}
	}

	private void acquireLocks()
	{
		if (WakeLock == null)
		{
			PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
			WakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "RIGDRED:KeepAlive");
			WakeLock.setReferenceCounted(false);
		}
		if (!WakeLock.isHeld()) { WakeLock.acquire(); }

		if (WifiLock == null)
		{
			WifiManager wm = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
			final int mode = (Build.VERSION.SDK_INT >= 29)
				? WifiManager.WIFI_MODE_FULL_LOW_LATENCY
				: WifiManager.WIFI_MODE_FULL_HIGH_PERF;
			WifiLock = wm.createWifiLock(mode, "RIGRED:KeepAliveWifi");
			WifiLock.setReferenceCounted(false);
		}
		if (!WifiLock.isHeld()) { WifiLock.acquire(); }
	}

	private void releaseLocks()
	{
		try { if (WakeLock != null && WakeLock.isHeld()) { WakeLock.release(); } } catch (Exception ignored) {}
		try { if (WifiLock != null && WifiLock.isHeld()) { WifiLock.release(); } } catch (Exception ignored) {}
	}

	@Override
	public void onTaskRemoved(Intent rootIntent)
	{
		// アプリをタスク一覧からスワイプで終了したらサービスも止める
		stopSelf();
		super.onTaskRemoved(rootIntent);
	}

	@Override
	public void onDestroy()
	{
		releaseLocks();
		Log.d(TAG, "keep-alive service destroyed");
		super.onDestroy();
	}
}
