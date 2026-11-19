// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AudioCaptureCore.h"
#include "HAL/ThreadSafeBool.h"
#include "Templates/Atomic.h"
#include "ReaperMicInputMode.h"
#include "ParaknotSender.generated.h"

namespace audiolink { class NetworkEngine; }

/**
 * 端末のマイク入力をキャプチャし、PARAKNOT（スタジオ側の VST）へ **押し込み送信** する。
 *
 * ReaStream（UReaStreamSender）との違いは相手と経路:
 *   ReaStream : REAPER のトラックに挿した ReaStream へ平文 UDP
 *   PARAKNOT  : AudioLink の独自プロトコル（暗号化つき）
 *
 * **プロトコル・暗号・ソケット・送信スレッドは AudioLink が持つ。** ここは
 * マイクを掴んで PCM を渡すだけの殻で、将来 PARAKNOT_UE プラグインへ移す。
 *
 * 使う手順:
 *   1. ParaknotRpc::SetRole(instanceId, 2) で相手を受信（待受）にする
 *   2. ParaknotRpc::OpenInputSession(instanceId) でポートと鍵を得る
 *   3. Start(スタジオのIP, ポート, 鍵)
 *
 * **鍵は保存しない。** 接続のたびに 2. で取り直す。
 */
UCLASS()
class RADNODZTOOLKIT_API UParaknotSender : public UObject
{
	GENERATED_BODY()

public:

	UParaknotSender();

	// **デストラクタは .cpp 側で定義する。** TUniquePtr の中身
	// (audiolink::NetworkEngine) をここでは前方宣言しか知らないため、
	// ヘッダで破棄コードを作らせるとコンパイルが通らない。
	~UParaknotSender();

	/**
	 * 押し込み送信を開始する。
	 *
	 * @param TargetIP       PARAKNOT が動いているマシンの IP
	 * @param InPort         OpenInputSession が返した待受ポート
	 * @param InSessionKey   同じく返ってきたセッション鍵（Base64 の 32byte）
	 * @param InChannelName  相手のチャンネル名欄に出す表示名
	 * @param InMode         使用する入力経路（本体マイク / Bluetoothマイク）
	 */
	bool Start(const FString& TargetIP, int32 InPort, const FString& InSessionKey,
		const FString& InChannelName = TEXT("Mic"),
		EMicInputMode InMode = EMicInputMode::Builtin);

	/** 送信を停止する。 */
	void Stop();

	bool IsActive() const { return bActive; }

	/**
	 * 相手が押し込みを受理したか。
	 * **受理されるまで AudioLink は音声を送らない**（捨てられるだけなので）。
	 */
	bool IsAccepted() const;

	/** 直近にマイクへ声（しきい値以上の入力）が入った時刻（FPlatformTime::Seconds）。0=まだ無し。 */
	double GetLastVoiceTime() const { return LastVoiceTime.Load(); }

	/** 現在設定されている入力経路を返す。 */
	EMicInputMode GetInputMode() const { return InputMode; }

	/** 送信ゲイン（送る前にマイク信号へ掛ける倍率。1.0=素通し）。 */
	void SetSendGain(float InGain);

	virtual void BeginDestroy() override;

private:

	// オーディオキャプチャのコールバック（キャプチャスレッド）
	void OnAudioCapture(const float* InAudio, int32 NumFrames, int32 NumChannels, int32 SampleRate);

	void Teardown();

	Audio::FAudioCapture AudioCapture;

	/**
	 * 送受信の実体。**ソケットもスレッドも暗号もこちらが持つ。**
	 *
	 * **生ポインタで持つ（TUniquePtr にしない）。** UHT が生成するコードは
	 * 配置 new でこのクラスを作るため、構築が失敗したときの後始末として
	 * メンバの破棄コードがその場に作られる。スマートポインタだと、そこで
	 * audiolink::NetworkEngine の全体が見える必要が出てヘッダを公開することに
	 * なる。確保と解放は .cpp 側の ctor / dtor で行う。
	 */
	audiolink::NetworkEngine* Engine = nullptr;

	/** 送信ゲイン（キャプチャスレッドで読む・ゲームスレッドで書く）。1.0=素通し。 */
	TAtomic<float> SendGain { 1.0f };

	EMicInputMode InputMode = EMicInputMode::Builtin;

	FThreadSafeBool bActive = false;

	/** 直近に声が入った時刻（キャプチャスレッドで書き込み・ゲームスレッドで読み取り）。 */
	TAtomic<double> LastVoiceTime { 0.0 };

	/**
	 * キャプチャをプレーナへ並べ替える作業バッファ（キャプチャスレッド専用）。
	 * AudioLink はプレーナ（ch ごとに連続）で受け取るが、キャプチャは
	 * インターリーブで届くため。毎回確保し直さないよう使い回す。
	 */
	TArray<float> PlanarBuffer;
	TArray<const float*> PlanarPointers;
};
