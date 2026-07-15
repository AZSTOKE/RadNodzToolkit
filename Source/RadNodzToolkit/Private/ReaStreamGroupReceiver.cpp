// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#include "ReaStreamGroupReceiver.h"

#include "Common/UdpSocketBuilder.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Interfaces/IPv4/IPv4Address.h"

#include "Sound/SoundWaveProcedural.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Async/Async.h"

bool UReaStreamGroupReceiver::Start(UObject* WorldContextObject, int32 InPort)
{
	if (bActive) return true;

	WorldContext = WorldContextObject;
	Port = InPort;

	FIPv4Endpoint Endpoint(FIPv4Address::Any, Port);
	Socket = FUdpSocketBuilder(TEXT("ReaStreamGroupSocket"))
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(1 << 21);   // 2MB

	if (!Socket)
	{
		return false;
	}

	Receiver = new FUdpSocketReceiver(Socket, FTimespan::FromMilliseconds(100), TEXT("ReaStreamGroupReceiverThread"));
	Receiver->OnDataReceived().BindUObject(this, &UReaStreamGroupReceiver::HandleData);
	Receiver->Start();

	bActive = true;
	return true;
}

void UReaStreamGroupReceiver::Stop()
{
	bActive = false;

	if (Receiver)
	{
		delete Receiver;
		Receiver = nullptr;
	}
	if (Socket)
	{
		Socket->Close();
		if (ISocketSubsystem* SS = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM))
		{
			SS->DestroySocket(Socket);
		}
		Socket = nullptr;
	}

	// 全参加者の再生を止めて解放する。
	TArray<uint32> Keys;
	{
		FScopeLock Lock(&PeersLock);
		Peers.GetKeys(Keys);
	}
	for (uint32 Addr : Keys)
	{
		TeardownPeer(Addr);
	}

	PeerSoundWaves.Reset();
	PeerAudioComponents.Reset();
	FreeSlots.Reset();

	{
		FScopeLock Lock(&AllowedMembersLock);
		AllowedMemberAddrs.Reset();
	}
}

void UReaStreamGroupReceiver::BeginDestroy()
{
	Stop();
	Super::BeginDestroy();
}

void UReaStreamGroupReceiver::SetAllowedMembers(const TArray<FString>& InMemberIPs)
{
	TSet<uint32> NewSet;
	NewSet.Reserve(InMemberIPs.Num());
	for (const FString& IP : InMemberIPs)
	{
		FIPv4Address Addr;
		if (!IP.IsEmpty() && FIPv4Address::Parse(IP, Addr))
		{
			NewSet.Add(Addr.Value);
		}
	}

	FScopeLock Lock(&AllowedMembersLock);
	AllowedMemberAddrs = MoveTemp(NewSet);
}

void UReaStreamGroupReceiver::HandleData(const FArrayReaderPtr& Data, const FIPv4Endpoint& Endpoint)
{
	if (!bActive || !Data.IsValid()) return;

	const uint32 SrcAddr = Endpoint.Address.Value;

	// メンバー一覧に無い送信元は無視する（本人以外の割り込み・混線を防ぐ）。
	{
		FScopeLock Lock(&AllowedMembersLock);
		if (!AllowedMemberAddrs.Contains(SrcAddr))
		{
			return;
		}
	}

	const int32 Len = Data->Num();
	if (Len < 47) return;

	const uint8* Bytes = Data->GetData();

	// 音声パケットのマジック 'MRSR' のみ扱う（'mRSR' はMIDIなので無視）
	if (!(Bytes[0] == 'M' && Bytes[1] == 'R' && Bytes[2] == 'S' && Bytes[3] == 'R'))
	{
		return;
	}

	const int32 NumChannels = (int32)Bytes[40];
	int32 SampleRate = 0;
	FMemory::Memcpy(&SampleRate, Bytes + 41, sizeof(int32)); // little-endian

	if (NumChannels < 1 || NumChannels > 64 || SampleRate <= 0)
	{
		return;
	}

	const int32 PayloadBytes = Len - 47;
	const int32 NumFloats = PayloadBytes / 4;
	if (NumFloats <= 0) return;

	const int32 PerChannel = NumFloats / NumChannels;
	if (PerChannel <= 0) return;

	// 受信インジケータ用：許可済みメンバーからの有効な音声パケットを受け取った時刻を記録する。
	const double NowSeconds = FPlatformTime::Seconds();
	LastRecvTime.Store(NowSeconds);
	if (NowSeconds - LastRecvLogTime > 1.0)
	{
		LastRecvLogTime = NowSeconds;
		UE_LOG(LogTemp, Warning, TEXT("[RIGDRED GroupRecv] from=%s ch=%d sr=%d"), *Endpoint.Address.ToString(), NumChannels, SampleRate);
	}

	// この送信元の再生状態を取得（無ければ作成をゲームスレッドへ依頼）。
	TSharedPtr<FReaStreamGroupPeerState> Peer;
	{
		FScopeLock Lock(&PeersLock);
		TSharedPtr<FReaStreamGroupPeerState>* Found = Peers.Find(SrcAddr);
		if (Found)
		{
			Peer = *Found;
		}
		else
		{
			Peer = MakeShared<FReaStreamGroupPeerState>();
			Peer->SenderAddr = SrcAddr;
			Peers.Add(SrcAddr, Peer);
		}
	}

	Peer->LastPacketTime.Store(FPlatformTime::Seconds());

	if (!Peer->bReady)
	{
		if (!Peer->bSetupDispatched)
		{
			Peer->bSetupDispatched = true;
			TWeakObjectPtr<UReaStreamGroupReceiver> WeakThis(this);
			AsyncTask(ENamedThreads::GameThread, [WeakThis, SrcAddr, NumChannels, SampleRate]()
			{
				if (UReaStreamGroupReceiver* Self = WeakThis.Get())
				{
					Self->SetupPeerPlayback(SrcAddr, NumChannels, SampleRate);
				}
			});
		}
		return;   // 準備できるまでこの参加者の音は捨てる
	}

	// 開始時に決めたフォーマットと違う場合は、作り直して追従する（送信側の入力経路変更などに対応）。
	if (NumChannels != Peer->StreamChannels)
	{
		if (Peer->bReady)
		{
			Peer->bReady = false;
			TWeakObjectPtr<UReaStreamGroupReceiver> WeakThis(this);
			AsyncTask(ENamedThreads::GameThread, [WeakThis, SrcAddr, NumChannels, SampleRate]()
			{
				if (UReaStreamGroupReceiver* Self = WeakThis.Get())
				{
					Self->SetupPeerPlayback(SrcAddr, NumChannels, SampleRate);
				}
			});
		}
		return;
	}

	// プレーナfloat → インターリーブint16（グループ通話はシンプルに受信chそのまま再生する）
	TArray<float> Plane;
	Plane.SetNumUninitialized(PerChannel * NumChannels);
	FMemory::Memcpy(Plane.GetData(), Bytes + 47, PerChannel * NumChannels * sizeof(float));

	TArray<int16> Interleaved;
	Interleaved.SetNumUninitialized(PerChannel * NumChannels);
	for (int32 s = 0; s < PerChannel; ++s)
	{
		for (int32 c = 0; c < NumChannels; ++c)
		{
			float V = Plane[c * PerChannel + s];
			V = FMath::Clamp(V, -1.f, 1.f);
			Interleaved[s * NumChannels + c] = (int16)(V * 32767.f);
		}
	}

	// SlotIndexはゲームスレッド（SetupPeerPlayback）でのみ書き込まれる。ここでは読むだけ。
	const int32 Slot = Peer->SlotIndex;
	if (Slot != INDEX_NONE && PeerSoundWaves.IsValidIndex(Slot))
	{
		if (USoundWaveProcedural* SW = PeerSoundWaves[Slot])
		{
			// 遅延の上限（1秒）を設ける：溜まりすぎたら捨てて最新へ追従する。
			const int32 BytesPerSec = Peer->StreamSampleRate * Peer->StreamChannels * (int32)sizeof(int16);
			const int32 MaxQueuedBytes = FMath::Max(1, BytesPerSec);
			if (SW->GetAvailableAudioByteCount() > MaxQueuedBytes)
			{
				SW->ResetAudio();
			}
			SW->QueueAudio(reinterpret_cast<const uint8*>(Interleaved.GetData()), Interleaved.Num() * sizeof(int16));
		}
	}
}

void UReaStreamGroupReceiver::SetupPeerPlayback(uint32 SenderAddr, int32 InChannels, int32 InSampleRate)
{
	if (!bActive) return;

	TSharedPtr<FReaStreamGroupPeerState> Peer;
	{
		FScopeLock Lock(&PeersLock);
		TSharedPtr<FReaStreamGroupPeerState>* Found = Peers.Find(SenderAddr);
		if (!Found) return;   // Stop()やTeardownPeer()で既に消えている
		Peer = *Found;
	}

	// スロットを確保（空きがあれば再利用、無ければ配列を伸ばす）。
	int32 Slot = Peer->SlotIndex;
	if (Slot == INDEX_NONE)
	{
		if (FreeSlots.Num() > 0)
		{
			Slot = FreeSlots.Pop();
		}
		else
		{
			Slot = PeerSoundWaves.Add(nullptr);
			PeerAudioComponents.Add(nullptr);
		}
		Peer->SlotIndex = Slot;
	}
	else if (PeerAudioComponents.IsValidIndex(Slot) && PeerAudioComponents[Slot])
	{
		// フォーマット変更などによる作り直し：前の再生を止める。
		PeerAudioComponents[Slot]->Stop();
		PeerAudioComponents[Slot] = nullptr;
		PeerSoundWaves[Slot] = nullptr;
	}

	Peer->StreamChannels = InChannels;
	Peer->StreamSampleRate = InSampleRate;

	USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>(this);
	SoundWave->SetSampleRate(InSampleRate);
	SoundWave->NumChannels = InChannels;
	SoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	SoundWave->SoundGroup = SOUNDGROUP_Default;
	SoundWave->bLooping = false;
	PeerSoundWaves[Slot] = SoundWave;

	UObject* WC = WorldContext.Get();
	if (!WC) return;

	UAudioComponent* AudioComponent = UGameplayStatics::CreateSound2D(WC, SoundWave);
	if (AudioComponent)
	{
		AudioComponent->bAutoDestroy = false;
		AudioComponent->Play();
	}
	PeerAudioComponents[Slot] = AudioComponent;

	Peer->bReady = true;
}

void UReaStreamGroupReceiver::TeardownPeer(uint32 SenderAddr)
{
	TSharedPtr<FReaStreamGroupPeerState> Peer;
	{
		FScopeLock Lock(&PeersLock);
		Peers.RemoveAndCopyValue(SenderAddr, Peer);
	}
	if (!Peer.IsValid()) return;

	const int32 Slot = Peer->SlotIndex;
	if (Slot != INDEX_NONE && PeerAudioComponents.IsValidIndex(Slot))
	{
		if (PeerAudioComponents[Slot])
		{
			PeerAudioComponents[Slot]->Stop();
			PeerAudioComponents[Slot] = nullptr;
		}
		PeerSoundWaves[Slot] = nullptr;
		FreeSlots.Add(Slot);
	}
}

void UReaStreamGroupReceiver::PruneStalePeers(float StaleSeconds)
{
	const double Now = FPlatformTime::Seconds();

	TArray<uint32> StaleKeys;
	{
		FScopeLock Lock(&PeersLock);
		for (const TPair<uint32, TSharedPtr<FReaStreamGroupPeerState>>& Pair : Peers)
		{
			const double Last = Pair.Value->LastPacketTime.Load();
			if (Last <= 0.0 || (Now - Last) > (double)StaleSeconds)
			{
				StaleKeys.Add(Pair.Key);
			}
		}
	}

	for (uint32 Addr : StaleKeys)
	{
		TeardownPeer(Addr);
	}
}

int32 UReaStreamGroupReceiver::GetActivePeerCount() const
{
	FScopeLock Lock(&const_cast<FCriticalSection&>(PeersLock));
	return Peers.Num();
}
