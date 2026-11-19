// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ParaknotSender.h"

#include "ReaStreamSender.h"   // 入力経路(Android の Bluetooth SCO)の切替を借りる

#include "audiolink/NetworkEngine.h"

namespace
{
	// キャプチャ1回あたりのフレーム数（48kHzで約21ms）。ReaStream 送信と揃える。
	constexpr int32 CaptureFrames = 1024;

	// 「声あり」とみなすRMSのしきい値（約 -45dBFS）。送信インジケータ用。
	constexpr double VoiceRmsThreshold = 0.0056;

	// AudioLink へ Prepare で渡すサンプルレート。
	//
	// **送信では FIFO の容量(約0.5秒)を決めるのに使うだけ。** 実際のレートは
	// PushForSend の引数でパケットごとに伝わる（端末のマイクは 16k / 44.1k の
	// ことがあり、開始時点では分からない）。
	constexpr double NominalSampleRate = 48000.0;
}

//==============================================================================
UParaknotSender::UParaknotSender()
	: Engine(new audiolink::NetworkEngine())
{
}

// ここで初めて audiolink::NetworkEngine の全体が見える（ヘッダ側は前方宣言だけ）。
UParaknotSender::~UParaknotSender()
{
	delete Engine;
	Engine = nullptr;
}

bool UParaknotSender::Start(const FString& TargetIP, int32 InPort, const FString& InSessionKey,
	const FString& InChannelName, EMicInputMode InMode)
{
	if (bActive) return true;

	// **鍵が無ければ始めない。** AudioLink は鍵が入るまで送らない（素通しにしない）
	// ので、始めても無音が続くだけになる。
	if (InSessionKey.IsEmpty() || InPort <= 0 || TargetIP.IsEmpty())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[PARAKNOT Mic] 送信先か鍵が不正（OpenInputSession の戻りを渡すこと）"));
		return false;
	}

	InputMode = InMode;

	// マイクをキャプチャする前にOS側のルーティングを切り替える（Android: Bluetooth SCO）
	UReaStreamSender::SetInputRouting(InputMode);

#if PLATFORM_ANDROID
	// BluetoothはsetCommunicationDevice()が成功しても、SCO音声リンクの確立には
	// 数百ms〜1秒かかる。待たずに録音を開くと無音を拾い続けることがある。
	if (InMode == EMicInputMode::Bluetooth)
	{
		FPlatformProcess::Sleep(0.8f);
	}
#endif

	// 送受信の準備。**接続の向きは「接続」**で、こちらから相手の待受へ押し込む。
	Engine->Prepare(NominalSampleRate, CaptureFrames);

	audiolink::NetworkEngine::Config Config;
	Config.role = audiolink::Role::send;
	Config.transport = audiolink::Transport::connect;
	Config.host = TCHAR_TO_UTF8(*TargetIP);
	Config.port = InPort;
	// 音声は常に暗号化される (AudioLink に平文の経路は無い)。鍵が空なら送らない。
	Config.sessionKeyBase64 = TCHAR_TO_UTF8(*InSessionKey);
	Engine->SetConfig(Config);

	// 相手のチャンネル名欄に出す表示名（マイクはモノラル前提）。
	Engine->SetChannelLayout(1, 0, { std::string(TCHAR_TO_UTF8(*InChannelName)) });

	Engine->Start();

	// マイク入力をキャプチャ
	Audio::FAudioCaptureDeviceParams Params;   // 既定デバイス
	Audio::FOnAudioCaptureFunction OnCap =
		[this](const void* InAudio, int32 NumFrames, int32 NumChannels, int32 SampleRate,
			double /*StreamTime*/, bool /*bOverflow*/)
	{
		OnAudioCapture(static_cast<const float*>(InAudio), NumFrames, NumChannels, SampleRate);
	};

	if (!AudioCapture.OpenAudioCaptureStream(Params, MoveTemp(OnCap), CaptureFrames))
	{
		UE_LOG(LogTemp, Warning, TEXT("[PARAKNOT Mic] OpenAudioCaptureStream 失敗（マイク権限/デバイス確認）"));
		Teardown();
		return false;
	}

	if (!AudioCapture.StartStream())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PARAKNOT Mic] StartStream 失敗"));
		Teardown();
		return false;
	}

	bActive = true;

	UE_LOG(LogTemp, Verbose, TEXT("[PARAKNOT Mic] 押し込み開始 → %s:%d"), *TargetIP, InPort);

	return true;
}

void UParaknotSender::Stop()
{
	if (!bActive) return;

	bActive = false;

	AudioCapture.StopStream();
	AudioCapture.CloseStream();

	Teardown();

	if (InputMode == EMicInputMode::Bluetooth)
	{
		UReaStreamSender::ClearInputRouting();
	}

	UE_LOG(LogTemp, Warning, TEXT("[PARAKNOT Mic] 押し込み停止"));
}

void UParaknotSender::Teardown()
{
	// **Stop() が相手へ unpublish を送って閉じる。** 鍵もここで捨てられる。
	Engine->Stop();
	Engine->Release();
}

bool UParaknotSender::IsAccepted() const
{
	return Engine->State() == audiolink::ConnectionState::connected;
}

void UParaknotSender::SetSendGain(float InGain)
{
	SendGain = FMath::Clamp(InGain, 0.0f, 8.0f);
}

void UParaknotSender::BeginDestroy()
{
	Stop();

	Super::BeginDestroy();
}

//==============================================================================
void UParaknotSender::OnAudioCapture(const float* InAudio, int32 NumFrames, int32 NumChannels,
	int32 SampleRate)
{
	if (!bActive || InAudio == nullptr || NumFrames <= 0 || NumChannels <= 0) return;

	// 声が入っているか（送信インジケータ用）。ゲイン適用前の生レベルで見る。
	double SumSq = 0.0;
	for (int32 i = 0; i < NumFrames * NumChannels; ++i)
	{
		SumSq += (double)InAudio[i] * (double)InAudio[i];
	}

	const double Rms = FMath::Sqrt(SumSq / FMath::Max(1, NumFrames * NumChannels));

	if (Rms >= VoiceRmsThreshold)
	{
		LastVoiceTime = FPlatformTime::Seconds();
	}

	// インターリーブ → プレーナ。ゲインはここで掛け、クリップで止める
	// （AudioLink 側は ±1.0 を超える値を丸めるだけなので、歪ませない）。
	const float Gain = SendGain.Load();

	PlanarBuffer.SetNumUninitialized(NumFrames * NumChannels);
	PlanarPointers.SetNumUninitialized(NumChannels);

	for (int32 c = 0; c < NumChannels; ++c)
	{
		float* Dst = PlanarBuffer.GetData() + (int64)c * NumFrames;
		PlanarPointers[c] = Dst;

		for (int32 f = 0; f < NumFrames; ++f)
		{
			Dst[f] = FMath::Clamp(InAudio[f * NumChannels + c] * Gain, -1.0f, 1.0f);
		}
	}

	// **受理前でも渡してよい。** 宛先が決まるまで AudioLink 側が捨てる
	// （溜め込まないので遅延も増えない）。
	Engine->PushForSend(PlanarPointers.GetData(), NumChannels,
		NumChannels, 0, NumFrames, (double)SampleRate);
}
