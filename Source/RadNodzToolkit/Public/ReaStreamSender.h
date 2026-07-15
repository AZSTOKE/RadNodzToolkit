// Fill out your copyright notice in the Description page of Project Settings.

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
	 * 接続中のBluetoothヘッドセット（通信用入力デバイス）の名前を返す。
	 * Androidのみ。未接続/取得不可/他プラットフォームでは空文字。
	 */
	static FString GetBluetoothInputDeviceName();

	virtual void BeginDestroy() override;

private:

	// オーディオキャプチャのコールバック（キャプチャスレッド）
	void OnAudioCapture(const float* InAudio, int32 NumFrames, int32 NumChannels, int32 SampleRate);
	// 1パケット分を組み立てて送信
	void SendChunk(const float* Interleaved, int32 FrameOffset, int32 Frames, int32 NumChannels, int32 SampleRate);

	// 入力経路に応じてOS側のオーディオルーティングを切り替える（Android: Bluetooth SCO）
	void ApplyAudioRouting(EMicInputMode Mode);
	// ルーティングを既定（本体マイク側）へ戻す
	void RestoreAudioRouting();

	Audio::FAudioCapture AudioCapture;

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
};
