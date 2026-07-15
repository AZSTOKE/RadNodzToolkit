// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/CriticalSection.h"
#include "Templates/Atomic.h"
#include "Common/UdpSocketReceiver.h"   // FArrayReaderPtr / FUdpSocketReceiver / FIPv4Endpoint
#include "ReaStreamGroupReceiver.generated.h"

class USoundWaveProcedural;
class UAudioComponent;
class FSocket;

/** 参加者1人ぶんの再生状態のうち、UObjectを含まない部分（GC対象外でよいのでプレーンな構造体でよい）。 */
struct FReaStreamGroupPeerState
{
	uint32 SenderAddr = 0;      // 送信元IPv4アドレス（キー）
	int32 SlotIndex = INDEX_NONE;  // PeerSoundWaves/PeerAudioComponentsの添字
	int32 StreamChannels = 0;
	int32 StreamSampleRate = 0;
	FThreadSafeBool bReady = false;
	FThreadSafeBool bSetupDispatched = false;
	TAtomic<double> LastPacketTime { 0.0 };
};

/**
 * グループ通話用：複数の送信元から同時に届くReaStream形式のUDP音声を、
 * 送信元(IP)ごとに独立した USoundWaveProcedural / UAudioComponent で再生する。
 * 複数の音声を同時に鳴らすこと自体はUnrealのオーディオエンジンに任せる
 * （自前でPCMを合成/ミックスしない）ことで、実装をシンプルかつ堅牢にしている。
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

	virtual void BeginDestroy() override;

private:

	// UDP受信スレッドから呼ばれる（ゲームスレッドではない）
	void HandleData(const FArrayReaderPtr& Data, const FIPv4Endpoint& Endpoint);

	// 受信インジケータ用（受信スレッドで書き込み・ゲームスレッドで読み取り）
	TAtomic<double> LastRecvTime { 0.0 };
	// デバッグログのスロットリング用（受信スレッドでのみ読み書きするのでプレーンなdoubleでよい）
	double LastRecvLogTime = 0.0;

	// 参加者用の再生セットアップ（ゲームスレッドで実行）。空きスロットが無ければ新規追加する。
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
