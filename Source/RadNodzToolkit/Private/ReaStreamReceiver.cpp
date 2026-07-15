// Fill out your copyright notice in the Description page of Project Settings.

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
#include "HRTF/HRTFRegistry.h"

bool UReaStreamReceiver::Start(UObject* WorldContextObject, int32 InPort)
{
	if (bActive) return true;

	WorldContext = WorldContextObject;
	Port = InPort;

	// 任意のアドレス(0.0.0.0)で指定ポートを待ち受ける
	FIPv4Endpoint Endpoint(FIPv4Address::Any, Port);
	Socket = FUdpSocketBuilder(TEXT("ReaStreamSocket"))
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(1 << 21);   // 2MB

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
	// 立体音響(バイノーラル)の係数・状態を解放
	SpNearIsLeft.Reset();
	SpNearGain.Reset();
	SpFarGain.Reset();
	SpDelaySamples.Reset();
	SpLpfAlpha.Reset();
	SpDelayBuf.Reset();
	SpDelayWrite.Reset();
	SpLpfPrev.Reset();
	SpDistGain.Reset();
	SpToneAlpha.Reset();
	SpToneState.Reset();

	// HRTF状態の解放
	bHrtfActive = false;
	HrtfTaps = 0;
	HrtfL.Reset();
	HrtfR.Reset();
	HrtfInBuf.Reset();
	HrtfInWrite.Reset();

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

	// プレーナ float → インターリーブ int16（モードに応じて変換）
	TArray<int16> Interleaved;
	const EMonitorOutputMode Mode = OutputMode.Load();
	if (Mode == EMonitorOutputMode::Spatial)
	{
		// 立体音響：HRTFが有効ならHRIR畳み込み、無効なら簡易バイノーラルでステレオへ合成
		if (bHrtfActive)
		{
			HrtfMixToStereoInterleaved(Plane.GetData(), PerChannel, NumChannels, Interleaved);
		}
		else
		{
			BinauralMixToStereoInterleaved(Plane.GetData(), PerChannel, NumChannels, Interleaved);
		}
	}
	else if (OutputChannels == 2 && NumChannels != 2)
	{
		// ダウンミックス（1ch→複製 / マルチch→ステレオ）
		DownmixToStereoInterleaved(Plane.GetData(), PerChannel, NumChannels, Interleaved);
	}
	else
	{
		// そのまま（受信chをインターリーブ）。OutputChannels==NumChannels。
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

	// 立体音響モードは別経路でセットアップ
	if (OutputMode.Load() == EMonitorOutputMode::Spatial)
	{
		SetupSpatialPlayback(InChannels, InSampleRate);
		return;
	}

	StreamChannels = InChannels;
	StreamSampleRate = InSampleRate;
	// ステレオなら2ch出力。そのままなら受信chそのまま。
	OutputChannels = (OutputMode.Load() == EMonitorOutputMode::Stereo) ? 2 : InChannels;

	SoundWave = NewObject<USoundWaveProcedural>(this);
	SoundWave->SetSampleRate(InSampleRate);
	SoundWave->NumChannels = OutputChannels;
	SoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	SoundWave->SoundGroup = SOUNDGROUP_Default;
	SoundWave->bLooping = false;

	UObject* WC = WorldContext.Get();
	if (!WC) return;

	AudioComponent = UGameplayStatics::CreateSound2D(WC, SoundWave);
	if (AudioComponent)
	{
		AudioComponent->bAutoDestroy = false;
		AudioComponent->Play();
	}

	bReady = true;
}

void UReaStreamReceiver::GetChannelAzimuths(int32 InChannels, EBinauralChannelOrder Order, TArray<float>& OutAzimuthsDeg)
{
	// 前方=0°、右回り(右)が正。
	OutAzimuthsDeg.Reset();

	// 円周上に等間隔（Sequential / その他ch数のフォールバック）
	auto FillEven = [&]()
	{
		for (int32 c = 0; c < InChannels; ++c)
		{
			OutAzimuthsDeg.Add(-180.f + (360.f * (c + 0.5f) / (float)InChannels));
		}
	};

	if (Order == EBinauralChannelOrder::Sequential)
	{
		FillEven();
		return;
	}

	switch (InChannels)
	{
	case 1: // モノ
		OutAzimuthsDeg = { 0.f };
		break;
	case 2: // ステレオ L R
		OutAzimuthsDeg = { -30.f, +30.f };
		break;
	case 6:
		if (Order == EBinauralChannelOrder::Film)
			OutAzimuthsDeg = { -30.f, +30.f, -110.f, +110.f, 0.f, 0.f };   // L R Ls Rs C LFE
		else
			OutAzimuthsDeg = { -30.f, +30.f, 0.f, 0.f, -110.f, +110.f };   // REAPER/ITU: L R C LFE Ls Rs
		break;
	case 8:
		if (Order == EBinauralChannelOrder::Film)
			OutAzimuthsDeg = { -30.f, +30.f, -110.f, +110.f, -150.f, +150.f, 0.f, 0.f }; // L R Ls Rs Lb Rb C LFE
		else
			OutAzimuthsDeg = { -30.f, +30.f, 0.f, 0.f, -110.f, +110.f, -150.f, +150.f }; // REAPER/ITU
		break;
	default:
		FillEven();
		break;
	}
}

void UReaStreamReceiver::BuildBinauralCoeffs(int32 InChannels, int32 InSampleRate)
{
	SpNearIsLeft.SetNum(InChannels);
	SpNearGain.SetNum(InChannels);
	SpFarGain.SetNum(InChannels);
	SpDelaySamples.SetNum(InChannels);
	SpLpfAlpha.SetNum(InChannels);
	SpDelayBuf.SetNum(InChannels);
	SpDelayWrite.SetNum(InChannels);
	SpLpfPrev.SetNum(InChannels);
	SpDistGain.SetNum(InChannels);
	SpToneAlpha.SetNum(InChannels);
	SpToneState.SetNum(InChannels);

	TArray<float> Azimuths;
	GetChannelAzimuths(InChannels, BinChannelOrder, Azimuths);

	const bool bCustom = (BinChannelOrder == EBinauralChannelOrder::Custom);

	// 最大ITD（設定のms値から算出。0ならITDなし）
	const int32 MaxDelay = FMath::Clamp(FMath::RoundToInt((BinItdMs / 1000.f) * InSampleRate), 0, 256);

	for (int32 c = 0; c < InChannels; ++c)
	{
		// 方位角：Customなら個別指定、それ以外はプリセット
		float Deg;
		if (bCustom && BinCustomAzimuths.IsValidIndex(c))
		{
			Deg = BinCustomAzimuths[c];
		}
		else
		{
			Deg = Azimuths.IsValidIndex(c) ? Azimuths[c] : 0.f;
		}
		const float Rad = FMath::DegreesToRadians(Deg);
		const float Pan = FMath::Clamp(FMath::Sin(Rad), -1.f, 1.f); // -1=左, +1=右

		// 距離（Customのみ。近いほど大きく、遠いほど小さく）
		float Dist = 1.f;
		if (bCustom && BinCustomDistances.IsValidIndex(c))
		{
			Dist = FMath::Clamp(BinCustomDistances[c], 0.2f, 2.0f);
		}
		SpDistGain[c] = FMath::Clamp(1.0f / Dist, 0.3f, 2.0f);

		// 等パワーパン（ILD）
		const float A = (Pan + 1.f) * 0.5f * (PI * 0.5f);
		const float GLeft  = FMath::Cos(A);
		const float GRight = FMath::Sin(A);

		const bool bNearLeft = (Pan <= 0.f);
		SpNearIsLeft[c] = bNearLeft;
		SpNearGain[c]   = bNearLeft ? GLeft : GRight;
		SpFarGain[c]    = bNearLeft ? GRight : GLeft;

		// ITD：遠い耳を |Pan| に比例して遅延
		SpDelaySamples[c] = FMath::RoundToInt(MaxDelay * FMath::Abs(Pan));

		// 頭部減衰（遠い耳の高域を抑える）。|Pan|が大きいほど低いカットオフ。
		// alpha=1で素通り、小さいほどこもる。設定値(BinShadowAlpha)を|pan|=1時の値とする。
		SpLpfAlpha[c] = FMath::Lerp(1.0f, BinShadowAlpha, FMath::Abs(Pan));

		// 前後キュー：後方(|Deg|>90°)ほど音色を暗くして前後を聞き分けやすくする。
		// 効きの強さは頭部減衰設定(BinShadowAlpha)に連動（オフなら無効）。
		const float Rearness = FMath::Clamp((FMath::Abs(Deg) - 90.f) / 90.f, 0.f, 1.f);
		SpToneAlpha[c] = FMath::Lerp(1.0f, BinShadowAlpha, Rearness);
		SpToneState[c] = 0.f;

		// 遅延リングバッファ確保＆状態クリア
		SpDelayBuf[c].Init(0.f, FMath::Max(1, MaxDelay) + 1);
		SpDelayWrite[c] = 0;
		SpLpfPrev[c] = 0.f;
	}
}

void UReaStreamReceiver::SetupSpatialPlayback(int32 InChannels, int32 InSampleRate)
{
	if (!bActive) return;

	StreamChannels = InChannels;
	StreamSampleRate = InSampleRate;
	OutputChannels = 2;   // アプリ内バイノーラル → ステレオ出力

	// バイノーラル係数を計算（HRTF未使用時のフォールバック）
	BuildBinauralCoeffs(InChannels, InSampleRate);
	// HRTFプロファイルが選択されていれば畳み込み用データを用意（成功で bHrtfActive=true）
	BuildHrtfCoeffs(InChannels, InSampleRate);

	// 再生は「確実に鳴る」ステレオ経路（CreateSound2D）を使う
	SoundWave = NewObject<USoundWaveProcedural>(this);
	SoundWave->SetSampleRate(InSampleRate);
	SoundWave->NumChannels = OutputChannels;
	SoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	SoundWave->SoundGroup = SOUNDGROUP_Default;
	SoundWave->bLooping = false;

	UObject* WC = WorldContext.Get();
	if (!WC) return;

	AudioComponent = UGameplayStatics::CreateSound2D(WC, SoundWave);
	if (AudioComponent)
	{
		AudioComponent->bAutoDestroy = false;
		AudioComponent->Play();
	}

	bReady = true;
}

UReaStreamReceiver::UReaStreamReceiver()
{
	// ダウンミックス係数の既定値（標準レイアウト L R C LFE Ls Rs Lb Rb）。
	// フロントL/R=等倍、C/サラウンド/リア=-3dB、LFE=除外。
	const float Def[MaxDownmixChannels] = { 1.0f, 1.0f, 0.7071f, 0.0f, 0.7071f, 0.7071f, 0.7071f, 0.7071f };
	for (int32 i = 0; i < MaxDownmixChannels; ++i)
	{
		DmGains[i].Store(Def[i]);
	}
}

void UReaStreamReceiver::DownmixToStereoInterleaved(const float* Plane, int32 PerChannel, int32 InChannels,
	TArray<int16>& OutInterleaved) const
{
	OutInterleaved.SetNumUninitialized(PerChannel * 2);

	// プレーナ配列の ch=c, sample=s へのアクセス
	auto Smp = [Plane, PerChannel](int32 c, int32 s) -> float { return Plane[c * PerChannel + s]; };

	// 受信ch（標準レイアウト L R C LFE Ls Rs Lb Rb）ごとの係数を取り出す
	float G[MaxDownmixChannels];
	for (int32 i = 0; i < MaxDownmixChannels; ++i) { G[i] = DmGains[i].Load(); }

	for (int32 s = 0; s < PerChannel; ++s)
	{
		float L = 0.f;
		float R = 0.f;

		switch (InChannels)
		{
		case 1:
			// モノ → 左右複製
			L = R = Smp(0, s);
			break;

		case 2:
			// ステレオ（L R）
			L = G[0] * Smp(0, s);
			R = G[1] * Smp(1, s);
			break;

		case 6:
			// 5.1（L R C LFE Ls Rs）
			L = G[0] * Smp(0, s) + G[2] * Smp(2, s) + G[3] * Smp(3, s) + G[4] * Smp(4, s);
			R = G[1] * Smp(1, s) + G[2] * Smp(2, s) + G[3] * Smp(3, s) + G[5] * Smp(5, s);
			break;

		case 8:
			// 7.1（L R C LFE Ls Rs Lb Rb）
			L = G[0] * Smp(0, s) + G[2] * Smp(2, s) + G[3] * Smp(3, s) + G[4] * Smp(4, s) + G[6] * Smp(6, s);
			R = G[1] * Smp(1, s) + G[2] * Smp(2, s) + G[3] * Smp(3, s) + G[5] * Smp(5, s) + G[7] * Smp(7, s);
			break;

		default:
			// その他のch数：偶数chを左、奇数chを右に振り分けて平均
			{
				int32 NL = 0, NR = 0;
				for (int32 c = 0; c < InChannels; ++c)
				{
					if ((c & 1) == 0) { L += Smp(c, s); ++NL; }
					else              { R += Smp(c, s); ++NR; }
				}
				if (NL > 0) L /= (float)NL;
				if (NR > 0) R /= (float)NR;
			}
			break;
		}

		L = FMath::Clamp(L, -1.f, 1.f);
		R = FMath::Clamp(R, -1.f, 1.f);
		OutInterleaved[s * 2 + 0] = (int16)(L * 32767.f);
		OutInterleaved[s * 2 + 1] = (int16)(R * 32767.f);
	}
}

void UReaStreamReceiver::BinauralMixToStereoInterleaved(const float* Plane, int32 PerChannel, int32 InChannels,
	TArray<int16>& OutInterleaved)
{
	OutInterleaved.SetNumUninitialized(PerChannel * 2);

	// 係数が未構築/ch数不一致なら通常のステレオダウンミックスにフォールバック
	if (SpNearGain.Num() != InChannels || SpDelayBuf.Num() != InChannels)
	{
		DownmixToStereoInterleaved(Plane, PerChannel, InChannels, OutInterleaved);
		return;
	}

	for (int32 s = 0; s < PerChannel; ++s)
	{
		float L = 0.f;
		float R = 0.f;

		for (int32 c = 0; c < InChannels; ++c)
		{
			const float Dg = SpDistGain.IsValidIndex(c) ? SpDistGain[c] : 1.f;
			float X = Plane[c * PerChannel + s] * Dg;

			// 前後キュー：後方chは両耳共通の一次LPFで音色を暗くする
			if (SpToneAlpha.IsValidIndex(c) && SpToneAlpha[c] < 0.999f)
			{
				float& TonePrev = SpToneState[c];
				TonePrev = TonePrev + SpToneAlpha[c] * (X - TonePrev);
				X = TonePrev;
			}

			// 近い耳：遅延なし・素通り
			const float NearOut = SpNearGain[c] * X;

			// 遠い耳：ITD遅延（リングバッファ）
			TArray<float>& Buf = SpDelayBuf[c];
			const int32 BufLen = Buf.Num();
			int32& WIdx = SpDelayWrite[c];
			const int32 D = SpDelaySamples[c];
			float Delayed;
			if (D <= 0)
			{
				Delayed = X;
			}
			else
			{
				const int32 RIdx = ((WIdx - D) % BufLen + BufLen) % BufLen;
				Delayed = Buf[RIdx];
			}
			Buf[WIdx] = X;
			WIdx = (WIdx + 1) % BufLen;

			// 遠い耳：頭部減衰（一次LPF）
			float& Prev = SpLpfPrev[c];
			Prev = Prev + SpLpfAlpha[c] * (Delayed - Prev);
			const float FarOut = SpFarGain[c] * Prev;

			if (SpNearIsLeft[c]) { L += NearOut; R += FarOut; }
			else                 { R += NearOut; L += FarOut; }
		}

		L = FMath::Clamp(L, -1.f, 1.f);
		R = FMath::Clamp(R, -1.f, 1.f);
		OutInterleaved[s * 2 + 0] = (int16)(L * 32767.f);
		OutInterleaved[s * 2 + 1] = (int16)(R * 32767.f);
	}
}

void UReaStreamReceiver::BuildHrtfCoeffs(int32 InChannels, int32 InSampleRate)
{
	bHrtfActive = false;
	HrtfTaps = 0;
	HrtfL.Reset();
	HrtfR.Reset();
	HrtfInBuf.Reset();
	HrtfInWrite.Reset();

	if (HrtfProfileName.IsEmpty()) return;

	const FReaperHRTFProfile* Profile = FReaperHRTFRegistry::Get().FindByName(HrtfProfileName);
	if (!Profile || !Profile->IsValid()) return;   // 未登録 → 簡易バイノーラルにフォールバック

	// 各chの方位（簡易バイノーラルと同じマッピングを使用）
	TArray<float> Azimuths;
	GetChannelAzimuths(InChannels, BinChannelOrder, Azimuths);
	const bool bCustom = (BinChannelOrder == EBinauralChannelOrder::Custom);

	HrtfTaps = Profile->Taps;
	HrtfL.SetNum(InChannels);
	HrtfR.SetNum(InChannels);
	HrtfInBuf.SetNum(InChannels);
	HrtfInWrite.SetNum(InChannels);

	for (int32 c = 0; c < InChannels; ++c)
	{
		float Deg;
		if (bCustom && BinCustomAzimuths.IsValidIndex(c)) { Deg = BinCustomAzimuths[c]; }
		else { Deg = Azimuths.IsValidIndex(c) ? Azimuths[c] : 0.f; }

		const int32 Idx = Profile->NearestIndex(Deg);
		const int32 SafeIdx = (Idx >= 0) ? Idx : 0;
		HrtfL[c] = Profile->L + (int64)SafeIdx * Profile->Taps;
		HrtfR[c] = Profile->R + (int64)SafeIdx * Profile->Taps;

		HrtfInBuf[c].Init(0.f, FMath::Max(1, Profile->Taps));
		HrtfInWrite[c] = 0;
	}

	// 注意：HRIRのサンプルレート(Profile->SampleRate)と受信レートが違う場合は本来リサンプルが必要。
	// 初版では未対応（近い値なら実用上問題は小さい）。
	bHrtfActive = true;
}

void UReaStreamReceiver::HrtfMixToStereoInterleaved(const float* Plane, int32 PerChannel, int32 InChannels,
	TArray<int16>& OutInterleaved)
{
	OutInterleaved.SetNumUninitialized(PerChannel * 2);

	// 状態不整合なら簡易バイノーラルにフォールバック
	if (!bHrtfActive || HrtfL.Num() != InChannels || HrtfTaps <= 0)
	{
		BinauralMixToStereoInterleaved(Plane, PerChannel, InChannels, OutInterleaved);
		return;
	}

	const int32 Taps = HrtfTaps;
	for (int32 s = 0; s < PerChannel; ++s)
	{
		float L = 0.f;
		float R = 0.f;

		for (int32 c = 0; c < InChannels; ++c)
		{
			TArray<float>& Buf = HrtfInBuf[c];
			int32& W = HrtfInWrite[c];
			Buf[W] = Plane[c * PerChannel + s];   // 最新サンプルを書き込み

			const float* hl = HrtfL[c];
			const float* hr = HrtfR[c];
			float oL = 0.f;
			float oR = 0.f;
			int32 idx = W;
			for (int32 k = 0; k < Taps; ++k)
			{
				const float x = Buf[idx];
				oL += hl[k] * x;
				oR += hr[k] * x;
				idx = (idx == 0) ? (Taps - 1) : (idx - 1);
			}
			L += oL;
			R += oR;

			W = (W + 1) % Taps;
		}

		L = FMath::Clamp(L, -1.f, 1.f);
		R = FMath::Clamp(R, -1.f, 1.f);
		OutInterleaved[s * 2 + 0] = (int16)(L * 32767.f);
		OutInterleaved[s * 2 + 1] = (int16)(R * 32767.f);
	}
}
