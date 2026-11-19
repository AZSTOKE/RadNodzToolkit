// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AudioCaptureCore.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/CriticalSection.h"
#include "Templates/Atomic.h"
#include "ReaperMicInputMode.h"
#include "ReaStreamSender.generated.h"

class FSocket;
class FInternetAddr;

/**
 * 端末のマイク入力をキャプチャし、ReaStream形式(UDP)で指定先(Reaper)へ送信する。
 * Reaper側はトラックに ReaStream を「Receive」モードで挿して受信する。
 *
 * 音声パケット(リトルエンディアン):
 *   0  'M','R','S','R' / 4 packetsize(int) / 8 identifier[32]
 *   40 nch(uint8) / 41 samplerate(int) / 45 sblocklen(short=サンプル総バイト)
 *   47- float サンプル（プレーナ＝非インターリーブ）
 */
UCLASS()
class RADNODZTOOLKIT_API UReaStreamSender : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * マイク送信を開始する。TargetIP=Reaperの居るPCのIP。
	 * @param InMode  使用する入力経路（本体マイク / Bluetoothマイク）。
	 */
	bool Start(const FString& TargetIP, int32 InPort = 58710, EMicInputMode InMode = EMicInputMode::Builtin);

	/**
	 * 送信先IPの一覧を差し替える（グループ通話用）。開始中でも呼び出し可能で、次に送るチャンクから反映される。
	 * 空/不正なIPは無視する。ポートは開始時に指定したものをそのまま使う。
	 */
	void SetGroupTargets(const TArray<FString>& InTargetIPs);

	/**
	 * 送信先IPの一覧と宛先ポートを同時に差し替える（グループトークのチャンネル排他切替用）。
	 * 開始中でも呼び出し可能で、次に送るチャンクから反映される。単一センダーを2チャンネルの
	 * どちらかへ排他的に振り向けるために使う（送信ソケットは固定ローカルポートに束縛していないため再作成不要）。
	 */
	void SetGroupTargets(const TArray<FString>& InTargetIPs, int32 InPort);

	/** 送信を停止する。 */
	void Stop();

	bool IsActive() const { return bActive; }

	/** 直近にマイクへ声（しきい値以上の入力）が入った時刻（FPlatformTime::Seconds）。0=まだ無し。 */
	double GetLastVoiceTime() const { return LastVoiceTime.Load(); }

	/** 現在設定されている入力経路を返す。 */
	EMicInputMode GetInputMode() const { return InputMode; }

	/**
	 * 送信ゲイン（送る前にマイク信号へ掛ける倍率。1.0=素通し）。
	 * グループトークの送信音量調整用。マイク取り込み(FAudioCapture)は生レベルで小さめなので持ち上げる。
	 * キャプチャスレッドが読むため atomic。クリップは送信時に±1.0でクランプする。
	 */
	void SetSendGain(float InGain);

	/**
	 * DTX（無音のあいだは送信しない）の有効・無効を切り替える。既定OFF。
	 *
	 * グループトークはフルメッシュ（参加者全員へ同じ音を配る）なので、無圧縮のまま全員が
	 * 常時送り続けると台数の2乗でWi-Fiのエアタイムを食い潰し、APのキュー詰まりで遅延が増える。
	 * 通常の会話では同時に喋るのは1人なので、無音の端末が送信を止めるだけでトラフィックが
	 * 大きく減る。あわせて、無音区間で受信側のキューが自然に空になるため、クロックずれで
	 * 溜まっていく遅延もリセットされる。
	 *
	 * 録音用途（Reaperへマイクを送る MicSender）でONにすると音に穴があくため、
	 * グループトークの送信機でのみONにすること。
	 */
	void SetDtxEnabled(bool bInEnabled) { bDtxEnabled = bInEnabled; }

	/** DTXが有効かどうか。 */
	bool IsDtxEnabled() const { return bDtxEnabled; }

	/**
	 * DTXで送信を止めているあいだ、相手に「まだ居る」ことを伝えるために低頻度で送る
	 * 無音パケットの識別子。受信側はこれを見て、音声として再生せず参加者の生存時刻だけ
	 * 更新する（無音が続いても参加者ごとの再生リソースが破棄されないようにするため）。
	 */
	static constexpr const ANSICHAR* KeepAliveIdentifier = "rnz-ka";

	/**
	 * 接続中のBluetoothヘッドセット（通信用入力デバイス）の名前を返す。
	 * Androidのみ。未接続/取得不可/他プラットフォームでは空文字。
	 */
	static FString GetBluetoothInputDeviceName();

	/**
	 * 入力経路（本体マイク / Bluetooth）へOS側のルーティングを切り替える。
	 * Androidのみ実効（他プラットフォームではログだけ）。
	 *
	 * マイクを掴む送信機はこのクラス以外にもある（UParaknotSender）ので、
	 * JNIの実装を1か所に置いたまま共有できるよう公開している。
	 */
	static void SetInputRouting(EMicInputMode Mode);

	/** Bluetoothへ切り替えていた場合に既定（本体マイク側）へ戻す。 */
	static void ClearInputRouting();

	virtual void BeginDestroy() override;

private:

	// キャプチャ1回分の入力をまるごと保持する（DTXのプリロール用）
	struct FPrerollChunk
	{
		TArray<float> Samples;    // インターリーブ（キャプチャから届いたまま）
		int32 Frames = 0;
		int32 Channels = 0;
		int32 SampleRate = 0;
	};

	// オーディオキャプチャのコールバック（キャプチャスレッド）
	void OnAudioCapture(const float* InAudio, int32 NumFrames, int32 NumChannels, int32 SampleRate);
	// キャプチャ1回分をパケットサイズに割って全部送る
	void SendFrames(const float* Interleaved, int32 NumFrames, int32 NumChannels, int32 SampleRate, bool bKeepAlive = false);
	// 1パケット分を組み立てて送信
	void SendChunk(const float* Interleaved, int32 FrameOffset, int32 Frames, int32 NumChannels, int32 SampleRate, bool bKeepAlive = false);

	// DTX用（いずれもキャプチャスレッドからのみ呼ぶ）
	void PushPreroll(const float* Interleaved, int32 NumFrames, int32 NumChannels, int32 SampleRate);   // 直近の入力を貯める
	void FlushPreroll();                                                                                // 貯めたぶんを古い順に送る
	void SendKeepAlive(int32 NumChannels, int32 SampleRate);                                            // 無音キープアライブ

	// 入力経路に応じてOS側のオーディオルーティングを切り替える（Android: Bluetooth SCO）
	void ApplyAudioRouting(EMicInputMode Mode);
	// ルーティングを既定（本体マイク側）へ戻す
	void RestoreAudioRouting();

	Audio::FAudioCapture AudioCapture;

	// 送信ゲイン（キャプチャスレッドで読む・ゲームスレッドで書く）。1.0=素通し。
	TAtomic<float> SendGain { 1.0f };

	// 使用中の入力経路
	EMicInputMode InputMode = EMicInputMode::Builtin;

	FSocket* Socket = nullptr;

	// 送信先アドレス一覧（通常は1件。グループ通話では複数件へ同時送信する）。
	// キャプチャスレッド（SendChunk）から読み、ゲームスレッド（SetGroupTargets）から書くためロックで保護する。
	FCriticalSection RemoteAddrsLock;
	TArray<TSharedPtr<FInternetAddr>> RemoteAddrs;

	int32 Port = 58710;

	// ReaStreamの識別子。Reaper側 Receive の Identifier と一致させる必要がある（既定 "default"）。
	FString Identifier = TEXT("default");

	FThreadSafeBool bActive = false;

	// 入力レベル確認用（スロットルログ）
	double LastLogTime = 0.0;

	// 直近に声が入った時刻（キャプチャスレッドで書き込み・ゲームスレッドで読み取り）
	TAtomic<double> LastVoiceTime { 0.0 };

	// ハウリング（フィードバックループ）簡易検知用（キャプチャスレッドでのみ使用）。
	// 通常の会話は音量が上下するが、ハウリングは大音量が途切れず続くという特徴を使う。
	double HowlingHighSince = 0.0;   // しきい値超えが連続している開始時刻（0=超えていない）
	double HowlingMuteUntil = 0.0;   // この時刻まで送信を止める（0=ミュート中でない）

	// --- DTX（無音時は送信しない）---
	// bDtxEnabled 以外はキャプチャスレッドでのみ読み書きするのでプレーンな変数でよい。
	FThreadSafeBool bDtxEnabled = false;   // ゲームスレッドから切替・キャプチャスレッドから参照
	double DtxLastVoiceTime = 0.0;         // 直近に「声あり」と判定した時刻（0=まだ無し）
	bool   bDtxSending = false;            // いま送信中か（false=無音ゲートで止めている）
	double DtxLastKeepAliveTime = 0.0;     // 直近に無音キープアライブを送った時刻

	// 語頭が切れないよう、無音中も直近の入力だけリングに残しておき、喋り始めに先行して送る。
	TArray<FPrerollChunk> DtxPreroll;
	int32 DtxPrerollWrite = 0;   // 次に書き込むリングの位置
	int32 DtxPrerollNum = 0;     // リングに溜まっている有効な要素数

	TArray<float> DtxKeepAliveBuf;   // 無音キープアライブ用のゼロ埋めバッファ（使い回す）
};
