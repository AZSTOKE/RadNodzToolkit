// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "Templates/Atomic.h"
#include "Templates/SharedPointer.h"
#include "MonitorOutputProcessor.h"
#include "ParaknotReceiver.generated.h"

namespace audiolink { class NetworkEngine; }

class FRunnableThread;
class UParaknotPullSoundWave;
class UAudioComponent;

/** 再生側 (オーディオの描画スレッド) と共有する状態。定義は .cpp。 */
struct FParaknotPullState;

/**
 * PARAKNOT（スタジオ側の VST）が配信している音声を購読して再生する。
 * **PARAKNOT のモニターの出力設定（ステレオ / 立体音響 / そのまま）もここが受け持つ。**
 *
 * ReaStream のモニター（UReaStreamReceiver）との違いは相手と経路で、
 * **プロトコル・暗号・ソケット・ジッターバッファは AudioLink が持つ**。
 * 受け取った後の変換（ダウンミックス・バイノーラル）は両者共通で、
 * FMonitorOutputProcessor が行う。
 *
 * **取り出しは再生側が駆動する**（UParaknotPullSoundWave）。供給と消費の時計の
 * ずれで再生キューに音が溜まり、遅延が伸びるのを防ぐため。
 *
 * 使う手順:
 *   1. ParaknotRpc::SetRole(instanceId, 0) で相手を送信（待受）にする
 *   2. ParaknotRpc::OpenOutputSession(instanceId) でポートと鍵を得る
 *   3. SetOutputSettings(...)（任意。既定はステレオダウンミックス）
 *   4. Start(WorldContextObject, スタジオのIP, ポート, 鍵)
 *
 * **複数の端末で同じ配信を同時に聞ける**（OpenOutputSession が既存の鍵を返す）。
 *
 * **鍵は保存しない。** 接続のたびに 2. で取り直す。復号に失敗し続けるときは
 * 鍵が入れ替わっているので、取り直して繋ぎ直すこと（GetDecryptFailureCount）。
 */
UCLASS()
class RADNODZTOOLKIT_API UParaknotReceiver : public UObject
{
	GENERATED_BODY()

public:

	UParaknotReceiver();

	// **デストラクタは .cpp 側で定義する。** audiolink::NetworkEngine を
	// ここでは前方宣言しか知らないため、ヘッダで破棄コードを作らせると
	// コンパイルが通らない。
	~UParaknotReceiver();

	/**
	 * 購読を開始する。
	 *
	 * @param WorldContextObject 再生用ワールドの取得に使う
	 * @param TargetIP           PARAKNOT が動いているマシンの IP
	 * @param InPort             OpenOutputSession が返した待受ポート
	 * @param InSessionKey       同じく返ってきたセッション鍵（Base64 の 32byte）
	 */
	bool Start(UObject* WorldContextObject, const FString& TargetIP, int32 InPort,
		const FString& InSessionKey);

	/** 購読を停止してリソースを解放する。 */
	void Stop();

	bool IsActive() const { return bActive; }

	/** 音声が届いているか（届き始めるまでは false）。 */
	bool IsReceiving() const;

	/** 受信しているチャンネル数（未受信は 0）。 */
	int32 GetReceivedChannels() const;

	/**
	 * 復号に失敗して捨てたパケットの累計。
	 *
	 * **増え続けるときは鍵が食い違っている。** 相手が別のセッションへ乗り換えた
	 * ことを意味するので、OpenOutputSession を呼び直して繋ぎ直す。
	 */
	int32 GetDecryptFailureCount() const;

	/** 再生音量（1.0=素通し）。再生中でも反映する。 */
	void SetPlaybackGain(float InGain);

	/**
	 * 出力設定（ステレオ / 立体音響 / そのまま と、その調整値）。**ゲームスレッドから。**
	 *
	 * **購読中に呼んでも張り直さない。** 再生（UE 側の音源）だけを新しい設定で
	 * 作り直す。購読（ハンドシェイク・鍵）はそのままなので、業務 TCP の往復は増えない。
	 */
	void SetOutputSettings(const FMonitorOutputSettings& InSettings);

	/**
	 * 届いている音声のレベル（線形 0..1 のピーク）。メーター表示用。
	 *
	 * **再生音量を掛ける前、出力設定で変換する前の値**＝「相手から何が来ているか」を
	 * 表す。こちらの音量を絞ってもメーターは振れるので、「音が来ていない」のか
	 * 「来ているが音量が 0」なのかを切り分けられる。
	 *
	 * 立ち上がりは即、落ちはゆっくり（ピークホールド風）。停止で 0 に戻る。
	 */
	float GetMeterLevel() const;

	virtual void BeginDestroy() override;

private:

	/**
	 * 受信 ch 数を見張るワーカー。決まった / 変わったら再生を作り直す。
	 *
	 * **音声の取り出しはしない**（再生側が駆動する）。ch 数は最初のパケットが
	 * 届くまで分からず、届いた通知も無いので見張るしかない。
	 */
	class FWatchWorker : public FRunnable
	{
	public:
		explicit FWatchWorker(UParaknotReceiver& InOwner) : Owner(InOwner) {}

		virtual bool Init() override { return true; }
		virtual uint32 Run() override;
		virtual void Stop() override { bStopping = true; }

		FThreadSafeBool bStopping = false;

	private:
		UParaknotReceiver& Owner;
	};

	/** ワーカー 1 周ぶん（ch 数を見て、必要なら再生の作り直しを頼む）。 */
	void WatchOnce();

	/** 前面 / バックグラウンドに合わせて AudioLink の目標遅延を切り替える（見張りスレッド）。 */
	void UpdateReceiveLatency();

	/** 途切れ・再同期が増えていたらログへ出す（見張りスレッド）。 */
	void LogStats();

	/** 受信 ch 数と出力設定に合わせて再生を用意する（ゲームスレッド）。 */
	void SetupPlayback(int32 InChannels);

	void Teardown();

	/**
	 * 受信の実体。**生ポインタで持つ（TUniquePtr にしない）。**
	 * 理由は UParaknotSender と同じ（UHT の生成コードが配置 new を使うため、
	 * スマートポインタだとヘッダに audiolink の全体が要る）。
	 */
	audiolink::NetworkEngine* Engine = nullptr;

	FWatchWorker* Worker = nullptr;
	FRunnableThread* WorkerThread = nullptr;

	UPROPERTY(Transient) TObjectPtr<UParaknotPullSoundWave> SoundWave;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent> AudioComponent;

	TWeakObjectPtr<UObject> WorldContext;

	FThreadSafeBool bActive = false;

	/** 用意した再生の受信 ch 数。0 = 未設定。 */
	TAtomic<int32> PlaybackChannels { 0 };

	/**
	 * 作り直しを頼んだ ch 数。**同じ ch 数で頼み直さない**ため。
	 * ゲームスレッドが片付けるまでの間、見張りの周期ごとに投げ続けないようにする。
	 */
	TAtomic<int32> RequestedChannels { 0 };

	float PlaybackGain = 1.0f;

	/** 出力設定（ゲームスレッド専用。SetupPlayback で再生側へ渡す）。 */
	FMonitorOutputSettings OutputSettings;

	// 以下は見張りスレッド専用（Start で初期化してから見張りを起こす）。

	/** 今バックグラウンド用の目標遅延にしているか。 */
	bool bBackgroundLatency = false;

	/** 前回ログを出した時点の AudioLink の累計（差分を出すため）。 */
	int32 LastUnderrunCount = 0;
	int32 LastResyncCount = 0;
	double LastStatLogTime = 0.0;

	/**
	 * 再生側と共有する状態（AudioLink への参照・出力の変換・メーター・作業バッファ）。
	 *
	 * **UObject ではなく共有ポインタで持つ。** 描画スレッドは停止の後にも
	 * 1〜2 回呼んでくることがあり、そのときこのオブジェクトが先に破棄されて
	 * いても触れるようにする。停止時は Engine を外してから AudioLink を閉じる。
	 */
	TSharedPtr<FParaknotPullState, ESPMode::ThreadSafe> PullState;
};
