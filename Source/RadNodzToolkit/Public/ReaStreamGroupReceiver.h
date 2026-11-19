// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/CriticalSection.h"
#include "Templates/Atomic.h"
#include "Common/UdpSocketReceiver.h"   // FArrayReaderPtr / FUdpSocketReceiver / FIPv4Endpoint
#include "audiolink/Concealer.h"        // 途切れの補間（PARAKNOT_UE の AudioLink）
#include "ReaStreamGroupReceiver.generated.h"

class USoundWaveProcedural;
class UAudioComponent;
class FSocket;

/**
 * 参加者1人ぶんの受信音のバッファ（ch インターリーブの float リングバッファ）。
 *
 * UDP の受信スレッドが Push で書き、オーディオの描画スレッドが Pull で取り出す
 * （UParaknotPullSoundWave の生成器から呼ばれる）。両者は Mutex で排他する。
 *
 * 受信スレッドから USoundWaveProcedural::QueueAudio で押し込む方式は使わない。
 * - キューが空になるたびにエンジンが無音を挟み、遅れて届いた音がその分だけ後ろへずれる
 * - 送信側の録音クロックと受信側の再生クロックのずれでキューが育つ
 * - 上限超過時の ResetAudio は残量カウンタのリセットが描画スレッド側で遅れて走るため、
 *   直後に積んだ分がカウンタから消え、上限判定が効かなくなる
 * が重なり、通話を続けるほど遅延が伸びるため。
 *
 * ここでは残量を 1 つの Mutex の内側で正確に持ち、取り出し時に上限を超えていれば
 * 目標量まで古い音を捨てる。足りないときは補間音（Plc）を返すだけで、それをデータとして
 * 後ろに積まないので遅延は育たない。
 */
struct FReaStreamGroupJitterBuffer
{
	/**
	 * 形式（ch 数・サンプルレート）と上限・目標を設定する。受信スレッドから呼ぶ。
	 *
	 * 形式が**変わったときだけ**中身を捨てて作り直し、true を返す（再生の作り直しが要る）。
	 * 上限・目標だけが変わったとき（前面 / バックグラウンドの切り替え）は中身を残す。
	 * 広げたときは溜め直さず、狭めたときは次の取り出しで超過分を捨てる。
	 */
	bool Configure(int32 InChannels, int32 InSampleRate, float InMaxSeconds, float InTargetSeconds);

	/** 前回呼んでからの途切れ回数・捨てたフレーム数を返して 0 に戻す（ログ用）。 */
	void TakeStats(int32& OutUnderruns, int32& OutDroppedFrames, int32& OutCountFrames);

	/** プレーナ float（ch ごとに PerChannel サンプルずつ並ぶ）を追加する。受信スレッドから呼ぶ。 */
	void Push(const uint8* PlanarFloatBytes, int32 PerChannel, int32 InChannels);

	/**
	 * 描画スレッドから呼ばれる。要求分（全 ch 合計 NumSamples）を書く。足りない分は補間音で埋める。
	 * @return 書いたことにするサンプル数（補間音を含む）。ch 数が合わなければ 0。
	 */
	int32 Pull(float* OutInterleaved, int32 NumSamples, int32 InChannels);

private:
	FCriticalSection Mutex;

	int32 Channels = 0;
	int32 SampleRate = 0;

	// リングバッファ（ch インターリーブ）。容量・位置・残量はフレーム単位。
	TArray<float> Ring;
	int32 CapacityFrames = 0;
	int32 ReadFrame = 0;
	int32 CountFrames = 0;

	// 取り出し時にこれを超えていたら TargetFrames まで古い音を捨てる（= 遅延の上限）。
	int32 MaxFrames = 0;
	// 再生開始・途切れ後の再開時に溜めてから出す量（= 揺らぎの吸収分）。
	int32 TargetFrames = 0;

	// TargetFrames 溜まるまで補間音を返して待つ。途切れたら戻す。
	bool bPrimed = false;

	// 途切れの補間（PLC）。足りないときは無音ではなく直前の波形の続きを絞りながら出し、
	// 明けや古い音を捨てて飛んだ位置へはクロスフェードでつなぐ（AudioLink と同じもの）。
	audiolink::Concealer Plc;

	// ログ用（TakeStats で 0 に戻す）。途切れ回数と、溢れ・上限超過で捨てたフレーム数。
	int32 StatUnderruns = 0;
	int32 StatDroppedFrames = 0;

	void DropOldest(int32 Frames);

	/** 容量を広げる（中身は残す）。Mutex の内側から呼ぶ。 */
	void GrowCapacity(int32 NewCapacityFrames);
};

/** 参加者1人ぶんの再生状態のうち、UObjectを含まない部分（GC対象外でよいのでプレーンな構造体でよい）。 */
struct FReaStreamGroupPeerState
{
	uint32 SenderAddr = 0;      // 送信元IPv4アドレス（キー）
	int32 SlotIndex = INDEX_NONE;  // PeerSoundWaves/PeerAudioComponentsの添字（ゲームスレッド専用）

	// 受信音のバッファ。参加者を作ったときに 1 度だけ作り、以後差し替えない
	// （描画スレッドの生成器も同じものを掴むので、再生を作り直しても安全）。
	TSharedPtr<FReaStreamGroupJitterBuffer, ESPMode::ThreadSafe> Jitter;

	TAtomic<double> LastPacketTime { 0.0 };

	// 途切れ・捨てた量を前回ログへ出した時刻（受信スレッド専用）。
	double LastStatLogTime = 0.0;
};

/**
 * グループ通話用：複数の送信元から同時に届くReaStream形式のUDP音声を、
 * 送信元(IP)ごとに独立した UParaknotPullSoundWave / UAudioComponent で再生する。
 * 複数の音声を同時に鳴らすこと自体はUnrealのオーディオエンジンに任せる
 * （自前でPCMを合成/ミックスしない）ことで、実装をシンプルかつ堅牢にしている。
 *
 * 再生は「受信スレッドがリングバッファへ書き、描画スレッドが要求分だけ取り出す」
 * 方式（FReaStreamGroupJitterBuffer 参照）。
 *
 * パケット形式は ReaStreamReceiver と同じ（Reaper非経由、端末同士の直接送受信）。
 */
UCLASS()
class RADNODZTOOLKIT_API UReaStreamGroupReceiver : public UObject
{
	GENERATED_BODY()

public:

	/** 受信を開始する。WorldContextObject は再生用ワールドの取得に使う。 */
	bool Start(UObject* WorldContextObject, int32 InPort);

	/** 受信を停止し、全参加者ぶんの再生リソースを解放する。 */
	void Stop();

	/** 受信中かどうか。 */
	bool IsActive() const { return bActive; }

	/**
	 * 参加を許可する送信元IPの一覧を差し替える（グループ通話のメンバー一覧が変わるたびに呼ぶ）。
	 * 一覧に無いIPからのパケットは無視する。一覧から外れた参加者は、次回のPruneStalePeersで
	 * 音声再生が停止される（自然にタイムアウトするため即座の切断処理は不要）。
	 */
	void SetAllowedMembers(const TArray<FString>& InMemberIPs);

	/**
	 * 一定時間パケットが届いていない参加者の再生を止めて解放する（ゲームスレッドから定期的に呼ぶ）。
	 * @param StaleSeconds  この秒数だけ無音が続いたら退出とみなす。
	 */
	void PruneStalePeers(float StaleSeconds = 5.0f);

	/** 現在アクティブに再生中（＝直近にパケットを受けた）の参加者数。UI表示用。 */
	int32 GetActivePeerCount() const;

	/** 誰か（許可済みメンバーの誰か1人でも）から直近に音声パケットを受け取った時刻（FPlatformTime::Seconds）。0=まだ無し。受信インジケータ用。 */
	double GetLastRecvTime() const { return LastRecvTime.Load(); }

	/**
	 * 受信音の再生メイクアップゲイン（VolumeMultiplier）を設定する。設定画面から変更でき、
	 * 現在再生中の全参加者にも即反映する。1.0=素通し。マイク入力が低めなのを補う用途。
	 */
	void SetPlaybackGain(float InGain);

	/**
	 * 参加者ごとのバッファに溜めてよい上限秒数（＝許容する遅延の上限）を設定する。
	 *
	 * 送信側の録音クロックと受信側の再生クロックは別の水晶なので、必ず少しずつずれる
	 * （±100ppmなら1時間で最大0.36秒）。リサンプルによる補正はしていないので、
	 * 溜まった分は取り出し時に上限を超えたところで目標量（GroupTalkTargetSeconds）まで捨てる。
	 * 大きいほど音飛びに強いが、遅延も大きくなりうる。既定0.15秒。
	 * 次に届いたパケットから反映する（バッファの中身は残す）。
	 */
	void SetBufferSeconds(float InSeconds) { BufferSeconds.Store(FMath::Clamp(InSeconds, 0.05f, 2.0f)); }

	/** 現在のバッファ上限秒数。 */
	float GetBufferSeconds() const { return BufferSeconds.Load(); }

	/** 再生開始・途切れ後の再開時に溜めてから出す秒数（揺らぎの吸収分）。上限の半分を超えない。 */
	static constexpr float GroupTalkTargetSeconds = 0.06f;

	/**
	 * バックグラウンド中（画面オフ・他アプリ表示）の目標と上限。
	 *
	 * Android 14 以降はアプリから Wi-Fi の省電力を止められず（WifiLock は前面・画面オンで
	 * しか効かない）、パケットが 100〜300ms ごとにまとめて届く。前面の値のままだと途切れと
	 * 古い音の破棄を細かく繰り返して音がブツブツになるので、その間だけ広げる。
	 * 上限は設定値（BufferSeconds）の方が大きければそちらを使う。
	 */
	static constexpr float BackgroundTargetSeconds = 0.25f;
	static constexpr float BackgroundMaxSeconds = 0.5f;

	virtual void BeginDestroy() override;

private:

	// グループトーク受信音のメイクアップゲイン（再生時のVolumeMultiplier）。設定で変更可。
	// 既定1.0（素通し）。持ち上げは主に送信側ゲインで行い、二重増幅を避ける。
	float PlaybackGain = 1.0f;

	// 参加者ごとのバッファ上限秒数（＝遅延の上限）。受信スレッドから読むためアトミック。
	// 通話は低遅延が最優先なので既定は短め（0.15秒）。
	TAtomic<float> BufferSeconds { 0.15f };

	// UDP受信スレッドから呼ばれる（ゲームスレッドではない）
	void HandleData(const FArrayReaderPtr& Data, const FIPv4Endpoint& Endpoint);

	// 受信インジケータ用（受信スレッドで書き込み・ゲームスレッドで読み取り）
	TAtomic<double> LastRecvTime { 0.0 };
	// デバッグログのスロットリング用（受信スレッドでのみ読み書きするのでプレーンなdoubleでよい）
	double LastRecvLogTime = 0.0;

	// 参加者用の再生セットアップ（ゲームスレッドで実行）。空きスロットが無ければ新規追加する。
	// 同じ参加者で再度呼ばれたら（ch 数が変わったとき）、前の再生を止めて作り直す。
	void SetupPeerPlayback(uint32 SenderAddr, int32 InChannels, int32 InSampleRate);

	// 参加者の再生を止めてスロットを解放する（ゲームスレッドで実行）
	void TeardownPeer(uint32 SenderAddr);

	FSocket* Socket = nullptr;
	FUdpSocketReceiver* Receiver = nullptr;
	TWeakObjectPtr<UObject> WorldContext;

	int32 Port = 0;

	// 参加を許可する送信元IPv4アドレス一覧。受信スレッドから読み、ゲームスレッドから書くためロックで保護する。
	FCriticalSection AllowedMembersLock;
	TSet<uint32> AllowedMemberAddrs;

	// 送信元(IPv4アドレス)ごとの再生状態（UObjectを含まない部分）。
	// 受信スレッド・ゲームスレッド双方から触るためロックで保護する。
	FCriticalSection PeersLock;
	TMap<uint32, TSharedPtr<FReaStreamGroupPeerState>> Peers;

	// 参加者ごとの再生用UObject。GC追跡のため必ずUPROPERTYの配列で保持する
	// （FReaStreamGroupPeerStateはプレーン構造体でGC非対応のため、UObjectはここに分離している）。
	// 添字はFReaStreamGroupPeerState::SlotIndexに対応。空きスロットはnullptrのまま次の参加者に再利用する。
	UPROPERTY(Transient) TArray<TObjectPtr<USoundWaveProcedural>> PeerSoundWaves;
	UPROPERTY(Transient) TArray<TObjectPtr<UAudioComponent>>      PeerAudioComponents;
	TArray<int32> FreeSlots;   // 再利用可能な空きスロットの添字

	FThreadSafeBool bActive = false;
};
