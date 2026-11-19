// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HAL/ThreadSafeBool.h"
#include "Templates/Atomic.h"
#include "ParaknotAudioReceiver.generated.h"

/** 接続の状態。audiolink::ConnectionState をそのまま写したもの。 */
UENUM(BlueprintType)
enum class EParaknotConnectionState : uint8
{
	Disconnected = 0,
	Listening,
	Connecting,
	Connected,
	Error
};

/**
 * PARAKNOT の配信を受け取る（受信 + 接続）。
 *
 * REAPER 側の PARAKNOT を **配信**（role 0 = 送信 + 待受）にしておき、
 * こちらから購読を申し込む。音声は UDP + AEAD(XChaCha20-Poly1305) で、
 * 鍵が無い／噛み合わないパケットは捨てられる（フェイルクローズ）。
 *
 * **接続先はここでは解決しない。** host / port / セッション鍵は利用側が
 * 用意して Start() へ渡す。RadNodzToolkit では RADNODZ の業務 TCP 越しに
 * AZ_PARAKNOT_OpenOutputSession から受け取る。
 *
 * **音声はポンプスレッドから渡す。** AudioLink は「届いたら呼ぶ」ではなく
 * 「こちらから引く」作り（内部にジッターバッファがある）なので、一定間隔で
 * PullForReceive して Sink へ渡す。Sink は**ゲームスレッドではない**ので、
 * UObject を触らないこと。
 */
UCLASS()
class PARAKNOTAUDIO_API UParaknotAudioReceiver : public UObject
{
	GENERATED_BODY()

public:

	UParaknotAudioReceiver();

	// **デストラクタを明示する。** TUniquePtr の中身（FImpl / ポンプ）は
	// この .cpp にしか定義が無いので、暗黙のデストラクタをヘッダ側で
	// 実体化されると「delete する型の定義がない」(C4150) になる。
	virtual ~UParaknotAudioReceiver() override;

	// **VTable ヘルパのコンストラクタも自前で持つ。** 宣言しないと UHT が
	// .gen.cpp に DEFINE_VTABLE_PTR_HELPER_CTOR で定義を吐き、そちらでも
	// TUniquePtr のデストラクタ（= 不完全型の delete）が実体化されて
	// 同じ C4150 になる。宣言しておけば UHT はダミーに差し替える。
	UParaknotAudioReceiver(FVTableHelper& Helper);

	/**
	 * 届いた音声を受け取る関数。**ポンプスレッドから呼ばれる。**
	 *
	 * @param Planar      ch ごとの先頭ポインタ（NumChannels 本。非インターリーブ）
	 * @param NumChannels 届いている ch 数
	 * @param NumFrames   1 ch あたりのサンプル数
	 * @param SampleRate  こちらの出力レート（送信側のレートではない。変換済み）
	 */
	using FAudioSink = TFunction<void(const float* const* Planar, int32 NumChannels,
		int32 NumFrames, int32 SampleRate)>;

	/** 音声の渡し先を設定する。**Start() より前に呼ぶこと。** */
	void SetAudioSink(FAudioSink InSink);

	/**
	 * 受信を開始する。
	 *
	 * @param Host             配信側の IP（REAPER が動いている PC）
	 * @param Port             配信側の待受ポート
	 * @param SessionKeyBase64 セッション鍵（Base64 の 32byte）
	 * @return 設定して開始できたか（**相手と繋がったかどうかではない**。
	 *         成立は GetState() が Connected になるのを見る）
	 */
	bool Start(const FString& Host, int32 Port, const FString& SessionKeyBase64);

	/** 受信を停止してリソースを解放する。 */
	void Stop();

	/** 受信を走らせているか（＝ Start 済みで Stop していない）。 */
	bool IsActive() const { return bActive; }

	/** 相手と繋がっているか（復号できた音声が届いているか）。 */
	bool IsConnected() const { return GetState() == EParaknotConnectionState::Connected; }

	EParaknotConnectionState GetState() const;

	/** 届いている ch 数（0 = まだ届いていない）。 */
	int32 GetReceivedChannels() const;

	/** 届いている ch の名前（配信側が付けたもの）。 */
	TArray<FString> GetReceivedChannelNames() const;

	/**
	 * 復号に失敗したパケットの累計。**増え続けるなら鍵が食い違っている。**
	 * セッションを取り直す判断に使う。
	 */
	int32 GetDecryptFailureCount() const;

	/** ジッターバッファの溜まり具合（サンプル数）。 */
	int32 GetJitterFill() const;

	/**
	 * 最後に音声を受け取った時刻（FPlatformTime::Seconds 基準）。0 = 未受信。
	 * 受信インジケータ（点滅）に使う。
	 */
	double GetLastAudioRecvTime() const { return LastAudioRecvTime.Load(); }

	/** ポンプが引く 1 ブロックのサンプル数（既定 512）。**Start() より前に。** */
	void SetBlockSize(int32 InBlockSize);

	/** 出力のサンプリングレート（既定 48000）。**Start() より前に。** */
	void SetSampleRate(int32 InSampleRate);

	virtual void BeginDestroy() override;

private:

	// ポンプ本体（別スレッド）。
	void RunPump();

	// audiolink の型を公開ヘッダへ出さないための実体。
	struct FImpl;
	TUniquePtr<FImpl> Impl;

	TUniquePtr<class FRunnableThread> PumpThread;
	TUniquePtr<class FParaknotPumpRunnable> PumpRunnable;

	FAudioSink Sink;

	int32 BlockSize = 512;
	int32 SampleRate = 48000;

	FThreadSafeBool bActive = false;

	TAtomic<double> LastAudioRecvTime { 0.0 };

	friend class FParaknotPumpRunnable;
};
