// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ReaStreamReceiver.h"

#include "Common/UdpSocketBuilder.h"
#include "Common/UdpSocketReceiver.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Interfaces/IPv4/IPv4Address.h"

#include "Sound/SoundWaveProcedural.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Async/Async.h"

bool UReaStreamReceiver::Start(UObject* WorldContextObject, int32 InPort)
{
	if (bActive) return true;

	WorldContext = WorldContextObject;
	Port = InPort;

	// 任意のアドレス(0.0.0.0)で指定ポートを待ち受ける
	FIPv4Endpoint Endpoint(FIPv4Address::Any, Port);
	// 受信バッファを大きく取りすぎると、受信スレッドが処理落ちしたぶんがカーネル内に溜まり、
	// 下のBufferSeconds（再生キューの上限）では捕まえられない遅延になる。2MBだとマルチchでも
	// 1秒以上、モノラルなら約10秒ぶん滞留できてしまうため、256KBに抑える。
	// （こちらはReaperからのマルチch受信もあるため、グループ通話用より少し余裕を持たせる。
	//   8ch/48kHz float32 でも約170ms、モノラルなら約1.3秒ぶん）
	Socket = FUdpSocketBuilder(TEXT("ReaStreamSocket"))
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(1 << 18);   // 256KB

	if (!Socket)
	{
		return false;
	}

	Receiver = new FUdpSocketReceiver(Socket, FTimespan::FromMilliseconds(100), TEXT("ReaStreamReceiverThread"));
	Receiver->OnDataReceived().BindUObject(this, &UReaStreamReceiver::HandleData);
	Receiver->Start();

	bActive = true;
	return true;
}

void UReaStreamReceiver::Stop()
{
	bActive = false;
	bReady = false;
	bSetupDispatched = false;
	LastAudioRecvTime.Store(0.0);   // 停止で受信時刻をクリア
	LockedSenderAddr.Store(0);      // 次回開始時は新たな最初の送信元にロックし直す
	LockedSenderPort.Store(0);

	if (Receiver)
	{
		delete Receiver;     // 受信スレッドを停止
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
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent = nullptr;
	}

	// 出力変換の係数・状態を解放（受信スレッドは上で止めてある）
	Output.Reset();

	SoundWave = nullptr;
	StreamChannels = 0;
	StreamSampleRate = 0;
	OutputChannels = 0;
}

void UReaStreamReceiver::BeginDestroy()
{
	Stop();
	Super::BeginDestroy();
}

void UReaStreamReceiver::HandleData(const FArrayReaderPtr& Data, const FIPv4Endpoint& Endpoint)
{
	if (!bActive || !Data.IsValid()) return;

	// 送信元IP制限：許可IPが設定されていて一致しないパケットは破棄する（0=制限なし）。
	const uint32 Allowed = AllowedSenderAddr.Load();
	if (Allowed != 0 && Endpoint.Address.Value != Allowed)
	{
		return;
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

	// 送信元ロック：同じポートへ複数の送信元（例：Reaperの別トラックのReaStream Send）が
	// 同時に送ってくるとパケットが混ざって音が割れる（バリバリ言う）ため、最初に有効な音声を
	// 送ってきた送信元（IP+ポート）だけをこのストリームの相手として以後ロックし、
	// それ以外の送信元からのパケットは無視する（Stop()するまでロックは維持）。
	const uint32 SrcAddr = Endpoint.Address.Value;
	const int32  SrcPort = (int32)Endpoint.Port;
	const uint32 LockedAddr = LockedSenderAddr.Load();
	if (LockedAddr == 0)
	{
		LockedSenderAddr.Store(SrcAddr);
		LockedSenderPort.Store(SrcPort);
	}
	else if (LockedAddr != SrcAddr || LockedSenderPort.Load() != SrcPort)
	{
		return;
	}

	// 有効な音声パケットを受信した時刻を記録（受信中インジケータの点滅判定用）
	LastAudioRecvTime.Store(FPlatformTime::Seconds());

	const int32 PayloadBytes = Len - 47;
	const int32 NumFloats = PayloadBytes / 4;
	if (NumFloats <= 0) return;

	const int32 PerChannel = NumFloats / NumChannels;
	if (PerChannel <= 0) return;

	// 初回受信：ゲームスレッドで再生をセットアップ（準備できるまで音は捨てる）
	if (!bReady)
	{
		if (!bSetupDispatched)
		{
			bSetupDispatched = true;
			TWeakObjectPtr<UReaStreamReceiver> WeakThis(this);
			AsyncTask(ENamedThreads::GameThread, [WeakThis, NumChannels, SampleRate]()
			{
				if (UReaStreamReceiver* Self = WeakThis.Get())
				{
					Self->SetupPlayback(NumChannels, SampleRate);
				}
			});
		}
		return;
	}

	// 最初に決めたフォーマットと途中で変わった場合は、再生を作り直して追従する
	// （送信側のトラック構成変更や、開始直後の一瞬だけ違うch数のパケットが来て
	// 誤ってロックされてしまった場合でも、正しいch数に自動復帰できるようにするため）。
	if (NumChannels != StreamChannels)
	{
		if (bReady)   // 再初期化を多重ディスパッチしないよう、まだ依頼していない時だけ
		{
			bReady = false;
			TWeakObjectPtr<UReaStreamReceiver> WeakThis(this);
			AsyncTask(ENamedThreads::GameThread, [WeakThis, NumChannels, SampleRate]()
			{
				if (UReaStreamReceiver* Self = WeakThis.Get())
				{
					Self->SetupPlayback(NumChannels, SampleRate);
				}
			});
		}
		return;
	}

	// アライメント安全のためいったんコピー（プレーナfloat）
	TArray<float> Plane;
	Plane.SetNumUninitialized(PerChannel * NumChannels);
	FMemory::Memcpy(Plane.GetData(), Bytes + 47, PerChannel * NumChannels * sizeof(float));

	ProcessPlanarAudio(Plane.GetData(), PerChannel, NumChannels);
}

// プレーナ float を出力設定に従って変換し、再生キューへ積む（UDP受信スレッド）。
void UReaStreamReceiver::ProcessPlanarAudio(const float* Plane, int32 PerChannel, int32 NumChannels)
{
	if (!Output.IsPrepared() || Output.GetInputChannels() != NumChannels || OutputChannels <= 0)
	{
		return;
	}

	// 出力設定に従って変換（ステレオ / 立体音響 / そのまま）→ インターリーブ float
	OutputScratch.SetNumUninitialized(PerChannel * OutputChannels, EAllowShrinking::No);
	Output.Process(Plane, PerChannel, OutputScratch.GetData());

	// インターリーブ float → int16（USoundWaveProcedural のキューは int16）
	TArray<int16> Interleaved;
	Interleaved.SetNumUninitialized(OutputScratch.Num());
	for (int32 i = 0; i < OutputScratch.Num(); ++i)
	{
		Interleaved[i] = (int16)(OutputScratch[i] * 32767.f);
	}

	if (SoundWave)
	{
		// 遅延の上限を設ける：キューに溜まった音声が一定量を超えたら一度捨てて最新へ追従する。
		// （Bluetooth等で再生消費が遅れると音声が溜まり続け、10〜20秒の遅延になるのを防ぐ）
		// 上限(=遅延)は設定で可変。大きいほどジッタ吸収＝音飛びに強いが遅延が増える。
		const int32 BytesPerSec = StreamSampleRate * OutputChannels * (int32)sizeof(int16);
		const int32 MaxQueuedBytes = FMath::Max(1, (int32)(BytesPerSec * BufferSeconds.Load()));
		if (SoundWave->GetAvailableAudioByteCount() > MaxQueuedBytes)
		{
			SoundWave->ResetAudio();
		}

		SoundWave->QueueAudio(reinterpret_cast<const uint8*>(Interleaved.GetData()),
			Interleaved.Num() * sizeof(int16));
	}
}

void UReaStreamReceiver::SetupPlayback(int32 InChannels, int32 InSampleRate)
{
	if (!bActive) return;

	// 作り直す前に古い再生を止める（ch 数が変わったときに通る）。
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent = nullptr;
	}

	// 出力の変換を用意する。**受信スレッドは bReady が落ちている間 Output に触らない。**
	Output.SetSettings(OutputSettings);
	const int32 OutCh = Output.Prepare(InChannels, InSampleRate);

	StreamChannels = InChannels;
	StreamSampleRate = InSampleRate;
	OutputChannels = OutCh;

	if (OutCh <= 0) return;

	SoundWave = NewObject<USoundWaveProcedural>(this);
	SoundWave->SetSampleRate(InSampleRate);
	SoundWave->NumChannels = OutCh;
	SoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	SoundWave->SoundGroup = SOUNDGROUP_Default;
	SoundWave->bLooping = false;

	UObject* WC = WorldContext.Get();
	if (!WC) return;

	AudioComponent = UGameplayStatics::CreateSound2D(WC, SoundWave);
	if (AudioComponent)
	{
		AudioComponent->bAutoDestroy = false;
		AudioComponent->SetVolumeMultiplier(PlaybackGain);   // 端末側の受信音量（dB設定）を反映
		AudioComponent->Play();
	}

	bReady = true;
}

void UReaStreamReceiver::SetPlaybackGain(float InGain)
{
	// ゲームスレッドから呼ばれる想定（設定変更時）。再生中でも即反映する。
	PlaybackGain = FMath::Clamp(InGain, 0.f, 8.f);
	if (AudioComponent)
	{
		AudioComponent->SetVolumeMultiplier(PlaybackGain);
	}
}
