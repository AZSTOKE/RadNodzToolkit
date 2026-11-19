// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ParaknotReceiver.h"

#include "Async/Async.h"
#include "Components/AudioComponent.h"
#include "HAL/CriticalSection.h"
#include "HAL/RunnableThread.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/ScopeLock.h"

#include "ParaknotPullSoundWave.h"
#include "ReaperBackgroundKeepAlive.h"

#include "audiolink/NetworkEngine.h"

namespace
{
	// 再生のサンプルレート。
	//
	// **送信側が何 Hz でもこれで出る。** AudioLink のジッターバッファが公称
	// レートの比で読み出し速度を合わせるため（端末マイクの 16k / 44.1k 対策と
	// 同じ仕組み）。
	constexpr double PlaybackSampleRate = 48000.0;

	// NetworkEngine::Prepare に渡すブロック長。**目安でしかない**（AudioLink は
	// 使っていない）。実際に 1 回で取り出す量は再生側が決める。
	constexpr int32 PrepareBlockFrames = 512;

	// 受信 ch 数を見張る間隔。**音声の取り出しとは無関係**なので粗くてよい。
	constexpr float WatchIntervalSeconds = 0.050f;

	// メーターの落ち方の時定数（秒）。**大きいほどゆっくり落ちる。**
	// 1 回の取り出し量は再生側しだいで一定でないので、係数は都度その長さから求める。
	constexpr double MeterDecaySeconds = 0.3;

	// AudioLink のジッターバッファの目標遅延（秒）。
	//
	// **バックグラウンド中（画面オフ・他アプリ表示）は広げる。** Android 14 以降は
	// アプリから Wi-Fi の省電力を止められず（WifiLock は前面・画面オンでしか効かない）、
	// パケットが 100〜300ms ごとにまとめて届く。前面の 40ms のままだと途切れと
	// 再同期を繰り返して音がブツブツになる。前面に戻ったら低遅延へ戻す。
	constexpr double ForegroundLatencySeconds = 0.040;
	constexpr double BackgroundLatencySeconds = 0.25;

	// 途切れ・再同期の回数をログへ出す間隔（秒）。増えていなければ出さない。
	constexpr double StatLogIntervalSeconds = 5.0;
}

//==============================================================================
/**
 * 再生側（オーディオの描画スレッド）と共有する状態。
 *
 * **Engine と Output を触るのは Mutex の内側だけ。** 停止するときはまず Engine を
 * nullptr にして切り離し、その後で AudioLink を閉じる。描画スレッドが停止後に
 * 呼んできても、切り離し済みなら何もしない（無音になる）。
 */
struct FParaknotPullState
{
	FCriticalSection Mutex;

	// nullptr = 切り離し済み。
	audiolink::NetworkEngine* Engine = nullptr;

	// 出力の変換（ステレオ / 立体音響 / そのまま）。SetupPlayback が作り直す。
	FMonitorOutputProcessor Output;

	// 今の再生の世代。**これと違う世代の古い再生には渡さない**（ch 数や出力設定が
	// 変わって作り直している最中、止めた側が最後に 1〜2 回呼んでくる。渡すと
	// ジッターバッファを 2 者で取り合って音が飛ぶ）。0 = 再生なし。
	uint32 Generation = 0;

	// メーターの現在値（線形ピーク）。描画スレッドが書き、UI が読む。
	TAtomic<float> MeterLevel { 0.0f };

	// 取り出し用の作業バッファ（描画スレッド専用。Mutex の内側で触る）。
	TArray<float> Planar;
	TArray<float*> Pointers;

	/**
	 * 描画スレッドから呼ばれる。要求された分だけ AudioLink から取り出し、
	 * 出力設定に従って変換してインターリーブで書く。
	 * **足りない分は AudioLink が無音で埋める。**
	 *
	 * @param InGeneration この再生を作ったときの世代
	 * @return 書いたサンプル数（全 ch 合計）。0 なら無音のまま。
	 */
	int32 Generate(float* Out, int32 NumSamples, uint32 InGeneration)
	{
		FScopeLock Lock(&Mutex);

		if (Engine == nullptr || InGeneration != Generation || !Output.IsPrepared())
		{
			return 0;
		}

		const int32 SourceChannels = Output.GetInputChannels();
		const int32 OutChannels = Output.GetOutputChannels();

		if (SourceChannels <= 0 || OutChannels <= 0)
		{
			return 0;
		}

		const int32 Frames = NumSamples / OutChannels;

		if (Frames <= 0)
		{
			return 0;
		}

		// AudioLink からは受信した ch 数のまま取り出す（連続したプレーナ配置）。
		Planar.SetNumUninitialized(Frames * SourceChannels, EAllowShrinking::No);
		Pointers.SetNumUninitialized(SourceChannels, EAllowShrinking::No);

		for (int32 c = 0; c < SourceChannels; ++c)
		{
			Pointers[c] = Planar.GetData() + (int64)c * Frames;
		}

		Engine->PullForReceive(Pointers.GetData(), SourceChannels, Frames);

		// メーター用のピーク。**全 ch の最大**を 1 本で出す（ch ごとに出す画面ではない）。
		// 「相手から何が来ているか」を表すので、変換する前の受信音で採る。
		float BlockPeak = 0.0f;

		for (int32 i = 0; i < Frames * SourceChannels; ++i)
		{
			BlockPeak = FMath::Max(BlockPeak, FMath::Abs(Planar[i]));
		}

		BlockPeak = FMath::Min(BlockPeak, 1.0f);

		// 出力設定に従って変換（ステレオ / 立体音響 / そのまま）。
		// UE の上限（8ch）を超えるものはここでステレオに落ちている。
		Output.Process(Planar.GetData(), Frames, Out);

		// バリスティクス：**立ち上がりは即、落ちはゆっくり。**
		// 瞬間値をそのまま出すと波形の谷でメーターが落ちてチラつくので、
		// ピークを保持して時定数 MeterDecaySeconds で落とす。
		const float Decay = (float)FMath::Exp(-((double)Frames / PlaybackSampleRate) / MeterDecaySeconds);

		MeterLevel.Store(FMath::Max(BlockPeak, MeterLevel.Load() * Decay));

		return Frames * OutChannels;
	}
};

//==============================================================================
uint32 UParaknotReceiver::FWatchWorker::Run()
{
	while (!bStopping)
	{
		Owner.WatchOnce();

		FPlatformProcess::Sleep(WatchIntervalSeconds);
	}

	return 0;
}

//==============================================================================
UParaknotReceiver::UParaknotReceiver()
	: Engine(new audiolink::NetworkEngine())
{
}

// ここで初めて audiolink::NetworkEngine の全体が見える（ヘッダ側は前方宣言だけ）。
UParaknotReceiver::~UParaknotReceiver()
{
	delete Engine;
	Engine = nullptr;
}

bool UParaknotReceiver::Start(UObject* WorldContextObject, const FString& TargetIP, int32 InPort,
	const FString& InSessionKey)
{
	if (bActive) return true;

	if (InSessionKey.IsEmpty() || InPort <= 0 || TargetIP.IsEmpty())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[PARAKNOT Monitor] 接続先か鍵が不正（OpenOutputSession の戻りを渡すこと）"));
		return false;
	}

	WorldContext = WorldContextObject;

	// 目標遅延は見張りが状況に合わせて切り替える。開始時は前面の値から。
	bBackgroundLatency = false;
	Engine->SetReceiveLatency(ForegroundLatencySeconds);

	LastUnderrunCount = 0;
	LastResyncCount = 0;
	LastStatLogTime = FPlatformTime::Seconds();

	Engine->Prepare(PlaybackSampleRate, PrepareBlockFrames);

	// **接続の向きは「接続」**で、相手の待受へ購読を申し込む。
	audiolink::NetworkEngine::Config Config;
	Config.role = audiolink::Role::receive;
	Config.transport = audiolink::Transport::connect;
	Config.host = TCHAR_TO_UTF8(*TargetIP);
	Config.port = InPort;
	// 音声は常に暗号化される (AudioLink に平文の経路は無い)。鍵が空なら何も鳴らない。
	Config.sessionKeyBase64 = TCHAR_TO_UTF8(*InSessionKey);
	Engine->SetConfig(Config);

	Engine->Start();

	// **再生側と共有する状態は開始ごとに作り直す。** 前回の再生が停止後に
	// 呼んできても、古い状態（切り離し済み）を見るだけで済む。
	PullState = MakeShared<FParaknotPullState, ESPMode::ThreadSafe>();
	PullState->Engine = Engine;

	bActive = true;

	Worker = new FWatchWorker(*this);
	WorkerThread = FRunnableThread::Create(Worker, TEXT("ParaknotReceiverWatch"));

	UE_LOG(LogTemp, Verbose, TEXT("[PARAKNOT Monitor] 購読開始 → %s:%d"), *TargetIP, InPort);

	return true;
}

void UParaknotReceiver::Stop()
{
	if (!bActive) return;

	bActive = false;

	Teardown();

	UE_LOG(LogTemp, Warning, TEXT("[PARAKNOT Monitor] 購読停止"));
}

void UParaknotReceiver::Teardown()
{
	if (Worker)
	{
		Worker->Stop();
	}

	if (WorkerThread)
	{
		WorkerThread->WaitForCompletion();
		delete WorkerThread;
		WorkerThread = nullptr;
	}

	if (Worker)
	{
		delete Worker;
		Worker = nullptr;
	}

	// **AudioLink を閉じる前に再生側から切り離す。** 描画スレッドが取り出している
	// 最中に閉じると落ちる。Mutex を取るので、取り出し中ならそれが終わるまで待つ。
	if (PullState.IsValid())
	{
		FScopeLock Lock(&PullState->Mutex);

		PullState->Engine = nullptr;
		PullState->Generation = 0;
		PullState->Output.Reset();

		// 止めたらメーターも落とす。**残っていると「まだ来ている」ように見える。**
		PullState->MeterLevel.Store(0.0f);
	}

	Engine->Stop();
	Engine->Release();

	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent = nullptr;
	}

	SoundWave = nullptr;
	PlaybackChannels = 0;
	RequestedChannels = 0;
}

bool UParaknotReceiver::IsReceiving() const
{
	return Engine->State() == audiolink::ConnectionState::connected;
}

int32 UParaknotReceiver::GetReceivedChannels() const
{
	return Engine->ReceivedChannels();
}

int32 UParaknotReceiver::GetDecryptFailureCount() const
{
	return Engine->DecryptFailureCount();
}

float UParaknotReceiver::GetMeterLevel() const
{
	return PullState.IsValid() ? PullState->MeterLevel.Load() : 0.0f;
}

void UParaknotReceiver::SetPlaybackGain(float InGain)
{
	PlaybackGain = FMath::Clamp(InGain, 0.0f, 8.0f);

	if (AudioComponent)
	{
		AudioComponent->SetVolumeMultiplier(PlaybackGain);
	}
}

void UParaknotReceiver::SetOutputSettings(const FMonitorOutputSettings& InSettings)
{
	OutputSettings = InSettings;

	// 再生中なら再生だけ作り直す（購読はそのまま）。
	const int32 Channels = PlaybackChannels.Load();

	if (bActive && Channels > 0)
	{
		SetupPlayback(Channels);
	}
}

void UParaknotReceiver::BeginDestroy()
{
	Stop();

	Super::BeginDestroy();
}

//==============================================================================
void UParaknotReceiver::WatchOnce()
{
	if (!bActive) return;

	UpdateReceiveLatency();
	LogStats();

	const int32 Channels = Engine->ReceivedChannels();

	// まだ 1 パケットも届いていない。
	if (Channels <= 0) return;

	// 用意済み、または作り直しを頼んである。
	if (PlaybackChannels.Load() == Channels || RequestedChannels.Load() == Channels) return;

	RequestedChannels = Channels;

	// ch 数が決まった / 変わった → 再生を作り直す（UObject の生成は
	// ゲームスレッド専用なので投げる）。
	TWeakObjectPtr<UParaknotReceiver> WeakThis(this);

	AsyncTask(ENamedThreads::GameThread, [WeakThis, Channels]()
	{
		if (UParaknotReceiver* Self = WeakThis.Get())
		{
			Self->SetupPlayback(Channels);
		}
	});
}

void UParaknotReceiver::UpdateReceiveLatency()
{
	const bool bBackground = FReaperKeepAlive::IsInBackground();

	if (bBackground == bBackgroundLatency) return;

	bBackgroundLatency = bBackground;

	// 広げるときは溜め直さない（途切れた分だけ滞留が増えて追従する）。
	// 狭めるときは AudioLink が次の読み出しで超過分を捨てる（1 回だけ音が飛ぶ）。
	Engine->SetReceiveLatency(bBackground ? BackgroundLatencySeconds : ForegroundLatencySeconds);

	UE_LOG(LogTemp, Warning, TEXT("[PARAKNOT Monitor] 目標遅延 → %.0fms（%s）"),
		Engine->ReceiveLatency() * 1000.0, bBackground ? TEXT("バックグラウンド") : TEXT("前面"));
}

void UParaknotReceiver::LogStats()
{
	const double Now = FPlatformTime::Seconds();

	if (Now - LastStatLogTime < StatLogIntervalSeconds) return;

	LastStatLogTime = Now;

	const int32 Underruns = Engine->UnderrunCount();
	const int32 Resyncs = Engine->ResyncCount();

	// 累計は AudioLink 側で張り直しのたびに 0 へ戻るので、減っていたら差分の基準を取り直す。
	const int32 NewUnderruns = Underruns >= LastUnderrunCount ? Underruns - LastUnderrunCount : Underruns;
	const int32 NewResyncs = Resyncs >= LastResyncCount ? Resyncs - LastResyncCount : Resyncs;

	LastUnderrunCount = Underruns;
	LastResyncCount = Resyncs;

	if (NewUnderruns == 0 && NewResyncs == 0) return;

	UE_LOG(LogTemp, Verbose, TEXT("[PARAKNOT Monitor] 直近%.0f秒: 途切れ=%d 再同期=%d 滞留=%.0fms 目標=%.0fms"),
		StatLogIntervalSeconds, NewUnderruns, NewResyncs,
		Engine->JitterFill() * 1000.0 / PlaybackSampleRate, Engine->ReceiveLatency() * 1000.0);
}

void UParaknotReceiver::SetupPlayback(int32 InChannels)
{
	if (!bActive || InChannels <= 0 || !PullState.IsValid()) return;

	// 作り直す前に古い再生を止める（ch 数・出力設定が変わったときに通る）。
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent = nullptr;
	}

	UObject* WC = WorldContext.Get();
	if (!WC) return;

	// 出力の変換を作り直し、世代を進める。以降はこの世代の再生にだけ渡す
	// （止めた古い再生が呼んできても無音）。
	int32 OutChannels = 0;
	uint32 Generation = 0;
	{
		FScopeLock Lock(&PullState->Mutex);

		PullState->Output.SetSettings(OutputSettings);
		OutChannels = PullState->Output.Prepare(InChannels, (int32)PlaybackSampleRate);

		Generation = ++PullState->Generation;
		if (Generation == 0) { Generation = ++PullState->Generation; }   // 0 は「再生なし」に予約
	}

	if (OutChannels <= 0) return;

	UE_LOG(LogTemp, Log, TEXT("[PARAKNOT Monitor] 再生を用意: 受信 %dch → 出力 %dch（出力方法=%d）"),
		InChannels, OutChannels, (int32)PullState->Output.GetEffectiveMode());

	SoundWave = NewObject<UParaknotPullSoundWave>(this);
	SoundWave->SetSampleRate((int32)PlaybackSampleRate);
	SoundWave->NumChannels = OutChannels;
	SoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	SoundWave->SoundGroup = SOUNDGROUP_Default;
	SoundWave->bLooping = false;

	// **this ではなく共有状態を掴ませる。** 描画スレッドは停止後にも呼んでくる
	// ことがあり、そのときこのオブジェクトが破棄されていても安全にするため。
	TSharedPtr<FParaknotPullState, ESPMode::ThreadSafe> State = PullState;

	SoundWave->SetGenerator([State, Generation](float* Out, int32 NumSamples)
	{
		return State->Generate(Out, NumSamples, Generation);
	});

	AudioComponent = UGameplayStatics::CreateSound2D(WC, SoundWave);

	if (!AudioComponent) return;

	AudioComponent->bAutoDestroy = false;
	AudioComponent->SetVolumeMultiplier(PlaybackGain);
	AudioComponent->Play();

	PlaybackChannels = InChannels;
}
