// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ReaStreamGroupReceiver.h"

#include "Common/UdpSocketBuilder.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Interfaces/IPv4/IPv4Address.h"

#include "Sound/SoundWaveProcedural.h"
#include "ParaknotPullSoundWave.h"   // 再生側の要求に応じて取り出す SoundWave（QueueAudio を使わない）
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Async/Async.h"
#include "ReaStreamSender.h"   // KeepAliveIdentifier（無音キープアライブの識別子）
#include "ReaperBackgroundKeepAlive.h"   // IsInBackground（バックグラウンド中はバッファを広げる）

bool UReaStreamGroupReceiver::Start(UObject* WorldContextObject, int32 InPort)
{
	if (bActive) return true;

	WorldContext = WorldContextObject;
	Port = InPort;

	FIPv4Endpoint Endpoint(FIPv4Address::Any, Port);
	// 受信バッファは小さめ（128KB）にする。ここを大きく取ると、受信スレッドが処理落ちした
	// ぶんがカーネル内に溜まり続け、アプリ側の遅延上限（BufferSeconds）では捕まえられない
	// 遅延になる（2MBだとモノラル48kHz float32で約10秒ぶん滞留できてしまう）。
	// 小さくしておけば、処理落ちは「遅延」ではなく「パケットロス」として現れて把握しやすい。
	Socket = FUdpSocketBuilder(TEXT("ReaStreamGroupSocket"))
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(1 << 17);   // 128KB（モノラル48kHz float32で約0.6秒ぶん）

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

void UReaStreamGroupReceiver::SetPlaybackGain(float InGain)
{
	// ゲームスレッドから呼ばれる想定（設定変更時）。再生中の全参加者へ即反映する。
	PlaybackGain = FMath::Clamp(InGain, 0.f, 16.f);
	for (UAudioComponent* AC : PeerAudioComponents)
	{
		if (AC) { AC->SetVolumeMultiplier(PlaybackGain); }
	}
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

	// 送信側がDTX（無音時は送信しない）で送ってくる生存通知かどうか。
	// これは「まだ居る」ことを伝えるためだけの極短い無音なので、音声としては再生せず、
	// 参加者の生存時刻の更新と再生リソースの先行準備にだけ使う。
	const int32 KeepAliveIdLen = FCStringAnsi::Strlen(UReaStreamSender::KeepAliveIdentifier) + 1;   // 終端0まで比較
	const bool bKeepAlive = (FMemory::Memcmp(Bytes + 8, UReaStreamSender::KeepAliveIdentifier, KeepAliveIdLen) == 0);

	// 受信インジケータ用：許可済みメンバーからの有効な音声パケットを受け取った時刻を記録する。
	// キープアライブは「声が流れている」わけではないので、インジケータには反映しない。
	const double NowSeconds = FPlatformTime::Seconds();
	if (!bKeepAlive)
	{
		LastRecvTime.Store(NowSeconds);
		if (NowSeconds - LastRecvLogTime > 1.0)
		{
			LastRecvLogTime = NowSeconds;
			UE_LOG(LogTemp, Verbose, TEXT("[RIGDRED GroupRecv] from=%s ch=%d sr=%d"), *Endpoint.Address.ToString(), NumChannels, SampleRate);
		}
	}

	// この送信元の状態を取得（無ければ作る。バッファもここで 1 度だけ作る）。
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
			Peer->Jitter = MakeShared<FReaStreamGroupJitterBuffer, ESPMode::ThreadSafe>();
			Peers.Add(SrcAddr, Peer);
		}
	}

	Peer->LastPacketTime.Store(NowSeconds);

	// 上限・目標。バックグラウンド中は Wi-Fi の省電力でパケットがまとめて届くので広げる
	// （BackgroundTargetSeconds 参照）。切り替えはバッファの中身を残したまま反映される。
	const bool bBackground = FReaperKeepAlive::IsInBackground();
	const float MaxSeconds = bBackground ? FMath::Max(BufferSeconds.Load(), BackgroundMaxSeconds) : BufferSeconds.Load();
	const float TargetSeconds = bBackground ? BackgroundTargetSeconds : GroupTalkTargetSeconds;

	// 形式が決まった / 変わった（送信側の入力経路変更など）→ バッファを作り直し、
	// 再生もその ch 数で作り直す（UObject の生成はゲームスレッド専用なので投げる）。
	// 再生の準備ができるまでに届いた音はバッファに溜まり、上限を超えた分は古い方から捨てる。
	if (Peer->Jitter->Configure(NumChannels, SampleRate, MaxSeconds, TargetSeconds))
	{
		TWeakObjectPtr<UReaStreamGroupReceiver> WeakThis(this);
		AsyncTask(ENamedThreads::GameThread, [WeakThis, SrcAddr, NumChannels, SampleRate]()
		{
			if (UReaStreamGroupReceiver* Self = WeakThis.Get())
			{
				Self->SetupPeerPlayback(SrcAddr, NumChannels, SampleRate);
			}
		});
	}

	// キープアライブは再生せずここで終わり
	// （目的は上の LastPacketTime 更新＝PruneStalePeersで解放されないようにすることと、再生の先行準備）。
	if (bKeepAlive) return;

	Peer->Jitter->Push(Bytes + 47, PerChannel, NumChannels);

	// 途切れ・捨てた量をときどきログへ出す（音が荒れたときの切り分け用。何も無ければ出さない）。
	if (NowSeconds - Peer->LastStatLogTime > 5.0)
	{
		Peer->LastStatLogTime = NowSeconds;

		int32 Underruns = 0, DroppedFrames = 0, CountFrames = 0;
		Peer->Jitter->TakeStats(Underruns, DroppedFrames, CountFrames);

		if (Underruns > 0 || DroppedFrames > 0)
		{
			UE_LOG(LogTemp, Verbose, TEXT("[RIGDRED GroupRecv] from=%s 直近5秒: 途切れ=%d 破棄=%.0fms 滞留=%.0fms 目標=%.0fms（%s）"),
				*Endpoint.Address.ToString(), Underruns,
				DroppedFrames * 1000.0 / SampleRate, CountFrames * 1000.0 / SampleRate,
				TargetSeconds * 1000.0, bBackground ? TEXT("バックグラウンド") : TEXT("前面"));
		}
	}
}

// ===== FReaStreamGroupJitterBuffer =====

bool FReaStreamGroupJitterBuffer::Configure(int32 InChannels, int32 InSampleRate, float InMaxSeconds, float InTargetSeconds)
{
	FScopeLock Lock(&Mutex);

	const int32 NewMaxFrames = FMath::Max(1, (int32)(InSampleRate * InMaxSeconds));
	const int32 NewTargetFrames = FMath::Clamp((int32)(InSampleRate * InTargetSeconds), 1, FMath::Max(1, NewMaxFrames / 2));

	if (InChannels == Channels && InSampleRate == SampleRate)
	{
		// 形式はそのまま。上限・目標だけ変わった（前面 / バックグラウンドの切り替え）なら中身を残して反映する。
		// 広げたときは溜め直さない（途切れれば次の再開で新しい目標まで溜まる）。
		// 狭めたときは次の取り出しで上限超過として目標まで古い方から捨てる。
		if (NewMaxFrames != MaxFrames || NewTargetFrames != TargetFrames)
		{
			MaxFrames = NewMaxFrames;
			TargetFrames = NewTargetFrames;

			if (MaxFrames * 2 > CapacityFrames)
			{
				GrowCapacity(MaxFrames * 2);
			}
		}

		return false;
	}

	Channels = InChannels;
	SampleRate = InSampleRate;

	MaxFrames = NewMaxFrames;
	TargetFrames = NewTargetFrames;

	// 容量は上限の 2 倍。取り出しが始まる前（再生の準備中）や、1 回の取り出しの間に
	// まとめて届いた分も受け止め、上限の判定は取り出し時に行う。
	CapacityFrames = MaxFrames * 2;
	Ring.SetNumZeroed(CapacityFrames * Channels);

	ReadFrame = 0;
	CountFrames = 0;
	bPrimed = false;

	Plc.Prepare(Channels, SampleRate);

	return true;
}

void FReaStreamGroupJitterBuffer::TakeStats(int32& OutUnderruns, int32& OutDroppedFrames, int32& OutCountFrames)
{
	FScopeLock Lock(&Mutex);

	OutUnderruns = StatUnderruns;
	OutDroppedFrames = StatDroppedFrames;
	OutCountFrames = CountFrames;

	StatUnderruns = 0;
	StatDroppedFrames = 0;
}

void FReaStreamGroupJitterBuffer::GrowCapacity(int32 NewCapacityFrames)
{
	// Mutex の内側から呼ぶこと。中身を先頭から並べ直して移す。
	TArray<float> NewRing;
	NewRing.SetNumZeroed(NewCapacityFrames * Channels);

	for (int32 f = 0; f < CountFrames; ++f)
	{
		const int32 Src = (ReadFrame + f) % CapacityFrames;
		FMemory::Memcpy(&NewRing[(int64)f * Channels], &Ring[(int64)Src * Channels], Channels * sizeof(float));
	}

	Ring = MoveTemp(NewRing);
	CapacityFrames = NewCapacityFrames;
	ReadFrame = 0;
}

void FReaStreamGroupJitterBuffer::DropOldest(int32 Frames)
{
	// Mutex の内側から呼ぶこと
	Frames = FMath::Min(Frames, CountFrames);
	if (Frames <= 0) return;

	ReadFrame = (ReadFrame + Frames) % CapacityFrames;
	CountFrames -= Frames;
}

void FReaStreamGroupJitterBuffer::Push(const uint8* PlanarFloatBytes, int32 PerChannel, int32 InChannels)
{
	FScopeLock Lock(&Mutex);

	if (InChannels != Channels || CapacityFrames <= 0 || PerChannel <= 0)
	{
		return;
	}

	// 1 パケットが容量を超える（異常に長い）なら末尾だけ使う
	const int32 Skip = FMath::Max(0, PerChannel - CapacityFrames);
	const int32 Frames = PerChannel - Skip;

	// 入りきらない分は古い方から捨てる
	const int32 Overflow = CountFrames + Frames - CapacityFrames;
	if (Overflow > 0)
	{
		StatDroppedFrames += FMath::Min(Overflow, CountFrames);
		DropOldest(Overflow);
	}

	int32 WriteFrame = (ReadFrame + CountFrames) % CapacityFrames;

	for (int32 f = 0; f < Frames; ++f)
	{
		float* Dst = &Ring[(int64)WriteFrame * Channels];

		for (int32 c = 0; c < Channels; ++c)
		{
			// パケット内の float は 4 バイト境界に揃っていない（ヘッダ 47 バイトの後ろ）ので memcpy で読む
			float V;
			FMemory::Memcpy(&V, PlanarFloatBytes + ((int64)c * PerChannel + Skip + f) * sizeof(float), sizeof(float));
			Dst[c] = FMath::Clamp(V, -1.f, 1.f);
		}

		if (++WriteFrame == CapacityFrames)
		{
			WriteFrame = 0;
		}
	}

	CountFrames += Frames;
}

int32 FReaStreamGroupJitterBuffer::Pull(float* OutInterleaved, int32 NumSamples, int32 InChannels)
{
	FScopeLock Lock(&Mutex);

	// 形式を変えて作り直している最中は、古い再生（ch 数が違う）に渡さない
	if (InChannels != Channels || Channels <= 0 || CapacityFrames <= 0)
	{
		return 0;
	}

	const int32 Frames = NumSamples / Channels;
	if (Frames <= 0)
	{
		return 0;
	}

	// 開始時・途切れた後は、目標量が溜まるまで待つ（揺らぎで小刻みに途切れないように）。
	// 待つ間は補間音（直前の波形の続きを 30ms で絞る。それ以降は無音）で埋める。
	if (!bPrimed)
	{
		if (CountFrames < TargetFrames)
		{
			for (int32 f = 0; f < Frames; ++f)
			{
				Plc.Gap(OutInterleaved + (int64)f * Channels);
			}
			return Frames * Channels;
		}
		bPrimed = true;
	}

	// 溜まりすぎ（クロックのずれ・まとめて届いた分）は目標量まで古い方から捨てる。
	// 残量はこの Mutex の内側で正確に分かるので、上限が必ず効く。
	if (CountFrames > MaxFrames)
	{
		StatDroppedFrames += CountFrames - TargetFrames;
		DropOldest(CountFrames - TargetFrames);

		// 読み出し位置が飛んだ。直前の音の続きから飛び先へクロスフェードでつなぐ。
		Plc.Splice();
	}

	const int32 ToCopy = FMath::Min(Frames, CountFrames);

	for (int32 f = 0; f < ToCopy; ++f)
	{
		const int32 Src = (ReadFrame + f) % CapacityFrames;
		float* Frame = OutInterleaved + (int64)f * Channels;
		FMemory::Memcpy(Frame, &Ring[(int64)Src * Channels], Channels * sizeof(float));

		// 途切れの明け・飛んだ直後なら補間音からクロスフェードする（それ以外は素通し）
		Plc.Real(Frame);
	}

	DropOldest(ToCopy);

	// 足りなかった（途切れた）。残りは補間音で埋め、溜まるまで待ってから再開する。
	// 補間音をデータとして後ろに積まないので、途切れても遅延は増えない。
	if (ToCopy < Frames)
	{
		for (int32 f = ToCopy; f < Frames; ++f)
		{
			Plc.Gap(OutInterleaved + (int64)f * Channels);
		}

		++StatUnderruns;
		bPrimed = false;
	}

	return Frames * Channels;
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

	// 再生側が要求した分だけバッファから取り出す（QueueAudio は使わない）。
	UParaknotPullSoundWave* SoundWave = NewObject<UParaknotPullSoundWave>(this);
	SoundWave->SetSampleRate(InSampleRate);
	SoundWave->NumChannels = InChannels;
	SoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	SoundWave->SoundGroup = SOUNDGROUP_Default;
	SoundWave->bLooping = false;

	// this ではなくバッファ（共有ポインタ）を掴ませる。描画スレッドは停止後にも
	// 呼んでくることがあり、そのとき受信側が破棄されていても安全にするため。
	// ch 数が変わって作り直した後は、古い再生には Pull が 0 を返す（無音）。
	TSharedPtr<FReaStreamGroupJitterBuffer, ESPMode::ThreadSafe> Jitter = Peer->Jitter;
	SoundWave->SetGenerator([Jitter, InChannels](float* Out, int32 NumSamples)
	{
		return Jitter.IsValid() ? Jitter->Pull(Out, NumSamples, InChannels) : 0;
	});

	PeerSoundWaves[Slot] = SoundWave;

	UObject* WC = WorldContext.Get();
	if (!WC) return;

	UAudioComponent* AudioComponent = UGameplayStatics::CreateSound2D(WC, SoundWave);
	if (AudioComponent)
	{
		AudioComponent->bAutoDestroy = false;
		// グループトークの受信音が小さいため、再生時にメイクアップゲインを掛ける（設定で変更可）。
		AudioComponent->SetVolumeMultiplier(PlaybackGain);
		AudioComponent->Play();
	}
	PeerAudioComponents[Slot] = AudioComponent;
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
