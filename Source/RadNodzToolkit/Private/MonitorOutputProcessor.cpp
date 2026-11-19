// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "MonitorOutputProcessor.h"

#include "HRTF/HRTFRegistry.h"

// フロントL/R=等倍、C/サラウンド/リア=-3dB、LFE=除外。
const float FMonitorOutputProcessor::DefaultDownmixGains[FMonitorOutputProcessor::MaxDownmixChannels] =
	{ 1.0f, 1.0f, 0.7071f, 0.0f, 0.7071f, 0.7071f, 0.7071f, 0.7071f };

int32 FMonitorOutputProcessor::Prepare(int32 InChannels, int32 InSampleRate)
{
	Reset();

	if (InChannels <= 0 || InSampleRate <= 0)
	{
		return 0;
	}

	InputChannels = InChannels;
	EffectiveMode = Settings.Mode;

	// **「そのまま」でも UE が扱えない ch 数はステレオへ落とす。**
	// そのまま渡すと UE のミキサーがゲイン 0 で鳴らし、無音になる。
	if (EffectiveMode == EMonitorOutputMode::Passthrough && InChannels > MaxPlaybackChannels)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[Monitor] 受信 %dch は UE の上限(%dch)を超えるため、「そのまま」ではなくステレオへダウンミックスします"),
			InChannels, MaxPlaybackChannels);

		EffectiveMode = EMonitorOutputMode::Stereo;
	}

	switch (EffectiveMode)
	{
	case EMonitorOutputMode::Spatial:
		OutputChannels = 2;
		// HRTF未使用時のフォールバックとして簡易バイノーラルの係数も必ず作る。
		BuildBinauralCoeffs(InChannels, InSampleRate);
		BuildHrtfCoeffs(InChannels);
		break;

	case EMonitorOutputMode::Passthrough:
		OutputChannels = InChannels;
		break;

	default: // Stereo
		OutputChannels = 2;
		BuildDownmixGains(InChannels);
		break;
	}

	return OutputChannels;
}

void FMonitorOutputProcessor::Reset()
{
	InputChannels = 0;
	OutputChannels = 0;
	EffectiveMode = Settings.Mode;

	DmL.Reset();
	DmR.Reset();

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

	bHrtfActive = false;
	HrtfTaps = 0;
	HrtfL.Reset();
	HrtfR.Reset();
	HrtfInBuf.Reset();
	HrtfInWrite.Reset();
}

void FMonitorOutputProcessor::Process(const float* Plane, int32 Frames, float* OutInterleaved)
{
	if (!IsPrepared() || Plane == nullptr || OutInterleaved == nullptr || Frames <= 0)
	{
		return;
	}

	switch (EffectiveMode)
	{
	case EMonitorOutputMode::Spatial:
		if (bHrtfActive) { HrtfMixToStereo(Plane, Frames, OutInterleaved); }
		else             { BinauralMixToStereo(Plane, Frames, OutInterleaved); }
		break;

	case EMonitorOutputMode::Passthrough:
		CopyInterleaved(Plane, Frames, OutInterleaved);
		break;

	default: // Stereo
		DownmixToStereo(Plane, Frames, OutInterleaved);
		break;
	}
}

//==============================================================================
// そのまま
//==============================================================================

void FMonitorOutputProcessor::CopyInterleaved(const float* Plane, int32 Frames, float* Out) const
{
	const int32 Ch = InputChannels;

	for (int32 s = 0; s < Frames; ++s)
	{
		for (int32 c = 0; c < Ch; ++c)
		{
			Out[s * Ch + c] = FMath::Clamp(Plane[c * Frames + s], -1.f, 1.f);
		}
	}
}

//==============================================================================
// ステレオダウンミックス
//==============================================================================

void FMonitorOutputProcessor::BuildDownmixGains(int32 InChannels)
{
	// 受信ch（標準レイアウト L R C LFE Ls Rs Lb Rb）ごとの係数。足りない分は既定値。
	float G[MaxDownmixChannels];
	for (int32 i = 0; i < MaxDownmixChannels; ++i)
	{
		G[i] = Settings.DownmixGains.IsValidIndex(i)
			? FMath::Clamp(Settings.DownmixGains[i], 0.0f, 1.5f)
			: DefaultDownmixGains[i];
	}

	DmL.Init(0.f, InChannels);
	DmR.Init(0.f, InChannels);

	switch (InChannels)
	{
	case 1:
		// モノ → 左右複製
		DmL[0] = DmR[0] = 1.f;
		break;

	case 2:
		// ステレオ（L R）。**係数は掛けずに素通し**（従来どおり。係数は
		// マルチchをまとめるためのもので、2ch の受信には効かせない）。
		DmL[0] = 1.f;
		DmR[1] = 1.f;
		break;

	case 6:
	case 8:
		// 5.1（L R C LFE Ls Rs）/ 7.1（+ Lb Rb）
		DmL[0] = G[0];
		DmR[1] = G[1];
		DmL[2] = DmR[2] = G[2];
		DmL[3] = DmR[3] = G[3];
		DmL[4] = G[4];
		DmR[5] = G[5];
		if (InChannels == 8)
		{
			DmL[6] = G[6];
			DmR[7] = G[7];
		}
		break;

	default:
		if (InChannels > MaxDownmixChannels)
		{
			// 9ch 以上（7.1.4 など）。先頭 8ch を 7.1 とみなし、9ch 目以降
			// （ハイトなど）は 2ch ずつ左右の組として -3dB で混ぜる。
			// 組にならない最後の 1ch は中央へ。
			DmL[0] = G[0];
			DmR[1] = G[1];
			DmL[2] = DmR[2] = G[2];
			DmL[3] = DmR[3] = G[3];
			DmL[4] = G[4];
			DmR[5] = G[5];
			DmL[6] = G[6];
			DmR[7] = G[7];

			const bool bOddTail = ((InChannels - MaxDownmixChannels) % 2) == 1;

			for (int32 c = MaxDownmixChannels; c < InChannels; ++c)
			{
				if (bOddTail && c == InChannels - 1)
				{
					DmL[c] = DmR[c] = 0.7071f;
				}
				else if ((c % 2) == 0)
				{
					DmL[c] = 0.7071f;
				}
				else
				{
					DmR[c] = 0.7071f;
				}
			}
		}
		else
		{
			// その他のch数（3/4/5/7ch）：偶数chを左、奇数chを右に振り分けて平均
			const int32 NL = (InChannels + 1) / 2;
			const int32 NR = InChannels / 2;
			for (int32 c = 0; c < InChannels; ++c)
			{
				if ((c & 1) == 0) { DmL[c] = 1.f / (float)NL; }
				else              { DmR[c] = NR > 0 ? 1.f / (float)NR : 0.f; }
			}
		}
		break;
	}
}

void FMonitorOutputProcessor::DownmixToStereo(const float* Plane, int32 Frames, float* Out) const
{
	const int32 Ch = InputChannels;

	for (int32 s = 0; s < Frames; ++s)
	{
		float L = 0.f;
		float R = 0.f;

		for (int32 c = 0; c < Ch; ++c)
		{
			const float X = Plane[c * Frames + s];
			L += DmL[c] * X;
			R += DmR[c] * X;
		}

		Out[s * 2 + 0] = FMath::Clamp(L, -1.f, 1.f);
		Out[s * 2 + 1] = FMath::Clamp(R, -1.f, 1.f);
	}
}

//==============================================================================
// 立体音響（簡易バイノーラル / HRTF）
//==============================================================================

void FMonitorOutputProcessor::GetChannelAzimuths(int32 InChannels, EBinauralChannelOrder Order, TArray<float>& OutAzimuthsDeg)
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

void FMonitorOutputProcessor::BuildBinauralCoeffs(int32 InChannels, int32 InSampleRate)
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
	GetChannelAzimuths(InChannels, Settings.ChannelOrder, Azimuths);

	const bool bCustom = (Settings.ChannelOrder == EBinauralChannelOrder::Custom);
	const float ShadowAlpha = Settings.BinShadowAlpha;

	// 最大ITD（設定のms値から算出。0ならITDなし）
	const int32 MaxDelay = FMath::Clamp(FMath::RoundToInt((Settings.BinItdMs / 1000.f) * InSampleRate), 0, 256);

	for (int32 c = 0; c < InChannels; ++c)
	{
		// 方位角：Customなら個別指定、それ以外はプリセット
		float Deg;
		if (bCustom && Settings.CustomAzimuths.IsValidIndex(c))
		{
			Deg = Settings.CustomAzimuths[c];
		}
		else
		{
			Deg = Azimuths.IsValidIndex(c) ? Azimuths[c] : 0.f;
		}
		const float Rad = FMath::DegreesToRadians(Deg);
		const float Pan = FMath::Clamp(FMath::Sin(Rad), -1.f, 1.f); // -1=左, +1=右

		// 距離（Customのみ。近いほど大きく、遠いほど小さく）
		float Dist = 1.f;
		if (bCustom && Settings.CustomDistances.IsValidIndex(c))
		{
			Dist = FMath::Clamp(Settings.CustomDistances[c], 0.2f, 2.0f);
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
		SpLpfAlpha[c] = FMath::Lerp(1.0f, ShadowAlpha, FMath::Abs(Pan));

		// 前後キュー：後方(|Deg|>90°)ほど音色を暗くして前後を聞き分けやすくする。
		// 効きの強さは頭部減衰設定(BinShadowAlpha)に連動（オフなら無効）。
		const float Rearness = FMath::Clamp((FMath::Abs(Deg) - 90.f) / 90.f, 0.f, 1.f);
		SpToneAlpha[c] = FMath::Lerp(1.0f, ShadowAlpha, Rearness);
		SpToneState[c] = 0.f;

		// 遅延リングバッファ確保＆状態クリア
		SpDelayBuf[c].Init(0.f, FMath::Max(1, MaxDelay) + 1);
		SpDelayWrite[c] = 0;
		SpLpfPrev[c] = 0.f;
	}
}

void FMonitorOutputProcessor::BuildHrtfCoeffs(int32 InChannels)
{
	bHrtfActive = false;
	HrtfTaps = 0;
	HrtfL.Reset();
	HrtfR.Reset();
	HrtfInBuf.Reset();
	HrtfInWrite.Reset();

	if (Settings.HrtfProfileName.IsEmpty()) return;

	const FReaperHRTFProfile* Profile = FReaperHRTFRegistry::Get().FindByName(Settings.HrtfProfileName);
	if (!Profile || !Profile->IsValid()) return;   // 未登録 → 簡易バイノーラルにフォールバック

	// 各chの方位（簡易バイノーラルと同じマッピングを使用）
	TArray<float> Azimuths;
	GetChannelAzimuths(InChannels, Settings.ChannelOrder, Azimuths);
	const bool bCustom = (Settings.ChannelOrder == EBinauralChannelOrder::Custom);

	HrtfTaps = Profile->Taps;
	HrtfL.SetNum(InChannels);
	HrtfR.SetNum(InChannels);
	HrtfInBuf.SetNum(InChannels);
	HrtfInWrite.SetNum(InChannels);

	for (int32 c = 0; c < InChannels; ++c)
	{
		float Deg;
		if (bCustom && Settings.CustomAzimuths.IsValidIndex(c)) { Deg = Settings.CustomAzimuths[c]; }
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

void FMonitorOutputProcessor::BinauralMixToStereo(const float* Plane, int32 Frames, float* Out)
{
	const int32 Ch = InputChannels;

	// 係数が未構築/ch数不一致なら通常のステレオダウンミックスにフォールバック
	if (SpNearGain.Num() != Ch || SpDelayBuf.Num() != Ch)
	{
		if (DmL.Num() != Ch) { BuildDownmixGains(Ch); }
		DownmixToStereo(Plane, Frames, Out);
		return;
	}

	for (int32 s = 0; s < Frames; ++s)
	{
		float L = 0.f;
		float R = 0.f;

		for (int32 c = 0; c < Ch; ++c)
		{
			float X = Plane[c * Frames + s] * SpDistGain[c];

			// 前後キュー：後方chは両耳共通の一次LPFで音色を暗くする
			if (SpToneAlpha[c] < 0.999f)
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

		Out[s * 2 + 0] = FMath::Clamp(L, -1.f, 1.f);
		Out[s * 2 + 1] = FMath::Clamp(R, -1.f, 1.f);
	}
}

void FMonitorOutputProcessor::HrtfMixToStereo(const float* Plane, int32 Frames, float* Out)
{
	const int32 Ch = InputChannels;

	// 状態不整合なら簡易バイノーラルにフォールバック
	if (!bHrtfActive || HrtfL.Num() != Ch || HrtfTaps <= 0)
	{
		BinauralMixToStereo(Plane, Frames, Out);
		return;
	}

	const int32 Taps = HrtfTaps;
	for (int32 s = 0; s < Frames; ++s)
	{
		float L = 0.f;
		float R = 0.f;

		for (int32 c = 0; c < Ch; ++c)
		{
			TArray<float>& Buf = HrtfInBuf[c];
			int32& W = HrtfInWrite[c];
			Buf[W] = Plane[c * Frames + s];   // 最新サンプルを書き込み

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

		Out[s * 2 + 0] = FMath::Clamp(L, -1.f, 1.f);
		Out[s * 2 + 1] = FMath::Clamp(R, -1.f, 1.f);
	}
}
