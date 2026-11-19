// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ParaknotAudioReceiver.h"

#include "ParaknotAudio.h"

#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/PlatformProcess.h"

// **audiolink はここだけ。** 公開ヘッダには出さない（ParaknotAudio.Build.cs の
// コメント参照）。
#include "audiolink/NetworkEngine.h"
#include "audiolink/Types.h"

//==============================================================================
// 実体
//==============================================================================

struct UParaknotAudioReceiver::FImpl
{
	audiolink::NetworkEngine Engine;

	// PullForReceive へ渡す ch ごとのバッファと、その先頭ポインタの配列。
	// **最大 ch 数ぶん確保しておく。** 届く ch 数は相手次第で、
	// 走り始めてからでないと分からない。
	TArray<TArray<float>> ChannelBuffers;
	TArray<float*>        ChannelPointers;
};

//==============================================================================
// ポンプ
//==============================================================================

// 一定間隔で PullForReceive を呼ぶスレッド。
//
// **AudioLink は「引く」作り。** 内部のジッターバッファが到着のばらつきを
// 吸収するので、こちらは実時間どおりの速さで淡々と引けばよい。
class FParaknotPumpRunnable : public FRunnable
{
public:

	explicit FParaknotPumpRunnable(UParaknotAudioReceiver& InOwner)
		: Owner(InOwner)
	{
	}

	virtual bool Init() override { return true; }

	virtual uint32 Run() override
	{
		Owner.RunPump();
		return 0;
	}

	virtual void Stop() override { bStopRequested = true; }

	bool ShouldStop() const { return bStopRequested; }

private:

	UParaknotAudioReceiver& Owner;
	FThreadSafeBool         bStopRequested = false;
};

//==============================================================================

UParaknotAudioReceiver::UParaknotAudioReceiver()
{
	Impl = MakeUnique<FImpl>();
}

// ここまで来れば FImpl / FParaknotPumpRunnable / FRunnableThread の定義が
// 見えているので、TUniquePtr を安全にたためる。
UParaknotAudioReceiver::~UParaknotAudioReceiver() = default;

UParaknotAudioReceiver::UParaknotAudioReceiver(FVTableHelper& Helper)
	: Super(Helper)
{
}

void UParaknotAudioReceiver::SetAudioSink(FAudioSink InSink)
{
	Sink = MoveTemp(InSink);
}

void UParaknotAudioReceiver::SetBlockSize(int32 InBlockSize)
{
	BlockSize = FMath::Clamp(InBlockSize, 64, 4096);
}

void UParaknotAudioReceiver::SetSampleRate(int32 InSampleRate)
{
	SampleRate = FMath::Clamp(InSampleRate, 8000, 192000);
}

bool UParaknotAudioReceiver::Start(const FString& Host, int32 Port, const FString& SessionKeyBase64)
{
	if (bActive)
	{
		return true;
	}

	if (Host.IsEmpty() || Port <= 0 || Port > 65535)
	{
		UE_LOG(LogParaknotAudio, Warning,
			TEXT("Start: 接続先が不正です (Host='%s' Port=%d)"), *Host, Port);
		return false;
	}

	// **鍵が無いなら始めない。** AudioLink はフェイルクローズなので、
	// 鍵なしで走らせても一切復号できず「繋がっているのに無音」になる。
	if (SessionKeyBase64.IsEmpty())
	{
		UE_LOG(LogParaknotAudio, Warning, TEXT("Start: セッション鍵が空です"));
		return false;
	}

	// ch ごとのバッファを用意する。
	Impl->ChannelBuffers.SetNum(audiolink::kMaxChannels);
	Impl->ChannelPointers.SetNum(audiolink::kMaxChannels);

	for (int32 Channel = 0; Channel < audiolink::kMaxChannels; ++Channel)
	{
		Impl->ChannelBuffers[Channel].SetNumZeroed(BlockSize);
		Impl->ChannelPointers[Channel] = Impl->ChannelBuffers[Channel].GetData();
	}

	audiolink::NetworkEngine::Config Config;

	Config.role = audiolink::Role::receive;
	Config.transport = audiolink::Transport::connect;   // こちらから購読を申し込む
	Config.host = TCHAR_TO_UTF8(*Host);
	Config.port = Port;
	// 音声は常に暗号化される (AudioLink に平文の経路は無い)。鍵が空なら何も鳴らない。
	Config.sessionKeyBase64 = TCHAR_TO_UTF8(*SessionKeyBase64);

	Impl->Engine.Prepare(static_cast<double>(SampleRate), BlockSize);
	Impl->Engine.SetConfig(Config);
	Impl->Engine.Start();

	LastAudioRecvTime.Store(0.0);
	bActive = true;

	PumpRunnable = MakeUnique<FParaknotPumpRunnable>(*this);
	PumpThread.Reset(FRunnableThread::Create(PumpRunnable.Get(), TEXT("ParaknotAudioPump"),
		0, TPri_AboveNormal));

	if (!PumpThread)
	{
		UE_LOG(LogParaknotAudio, Error, TEXT("Start: ポンプスレッドを作れませんでした"));
		Stop();
		return false;
	}

	UE_LOG(LogParaknotAudio, Log, TEXT("Start: %s:%d へ購読を申し込みます (%d Hz / %d サンプル)"),
		*Host, Port, SampleRate, BlockSize);

	return true;
}

void UParaknotAudioReceiver::Stop()
{
	if (!bActive && !PumpThread)
	{
		return;
	}

	bActive = false;

	// **ポンプを先に止める。** Engine を先に落とすと、引いている最中の
	// バッファを解放することになる。
	if (PumpRunnable)
	{
		PumpRunnable->Stop();
	}

	if (PumpThread)
	{
		PumpThread->WaitForCompletion();
		PumpThread.Reset();
	}

	PumpRunnable.Reset();

	Impl->Engine.Stop();
	Impl->Engine.Release();

	Impl->ChannelPointers.Reset();
	Impl->ChannelBuffers.Reset();

	UE_LOG(LogParaknotAudio, Log, TEXT("Stop: 受信を停止しました"));
}

void UParaknotAudioReceiver::RunPump()
{
	const double BlockSeconds = static_cast<double>(BlockSize) / static_cast<double>(SampleRate);

	// **次に引くべき時刻を積み上げる。** 毎回 Sleep(BlockSeconds) にすると
	// Sleep の誤差が積もって少しずつ遅れ、ジッターバッファが溢れる。
	double NextDeadline = FPlatformTime::Seconds();

	while (PumpRunnable.IsValid() && !PumpRunnable->ShouldStop())
	{
		const int32 Channels = Impl->Engine.ReceivedChannels();

		if (Channels > 0)
		{
			const int32 Use = FMath::Min(Channels, audiolink::kMaxChannels);

			Impl->Engine.PullForReceive(Impl->ChannelPointers.GetData(), Use, BlockSize);

			if (Sink)
			{
				Sink(const_cast<const float* const*>(Impl->ChannelPointers.GetData()),
					Use, BlockSize, SampleRate);
			}

			// **「音が届いた」は復号できていることを指す。** 繋がっただけで
			// 復号できていない状態と区別したいので State も見る。
			if (Impl->Engine.State() == audiolink::ConnectionState::connected)
			{
				LastAudioRecvTime.Store(FPlatformTime::Seconds());
			}
		}

		NextDeadline += BlockSeconds;

		const double Now = FPlatformTime::Seconds();
		const double Wait = NextDeadline - Now;

		if (Wait > 0.0)
		{
			FPlatformProcess::SleepNoStats(static_cast<float>(Wait));
		}
		else if (Wait < -0.5)
		{
			// 大きく出遅れた（スリープ復帰・重い処理のあとなど）。
			// 取り戻そうとして一気に引くとジッターバッファを空にするので、
			// 基準を今に取り直す。
			NextDeadline = Now;
		}
	}
}

EParaknotConnectionState UParaknotAudioReceiver::GetState() const
{
	switch (Impl->Engine.State())
	{
		case audiolink::ConnectionState::listening:   return EParaknotConnectionState::Listening;
		case audiolink::ConnectionState::connecting:  return EParaknotConnectionState::Connecting;
		case audiolink::ConnectionState::connected:   return EParaknotConnectionState::Connected;
		case audiolink::ConnectionState::error:       return EParaknotConnectionState::Error;
		default:                                      return EParaknotConnectionState::Disconnected;
	}
}

int32 UParaknotAudioReceiver::GetReceivedChannels() const
{
	return Impl->Engine.ReceivedChannels();
}

TArray<FString> UParaknotAudioReceiver::GetReceivedChannelNames() const
{
	TArray<FString> Names;

	for (const std::string& Name : Impl->Engine.ReceivedNames())
	{
		Names.Add(UTF8_TO_TCHAR(Name.c_str()));
	}

	return Names;
}

int32 UParaknotAudioReceiver::GetDecryptFailureCount() const
{
	return Impl->Engine.DecryptFailureCount();
}

int32 UParaknotAudioReceiver::GetJitterFill() const
{
	return Impl->Engine.JitterFill();
}

void UParaknotAudioReceiver::BeginDestroy()
{
	Stop();

	Super::BeginDestroy();
}
