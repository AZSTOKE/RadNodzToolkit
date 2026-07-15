// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.


//
// 内蔵の合成HRTFプロファイル。
// 実測データ（外部データセットのSOFA/MAT等）が無くてもHRTF経路が動作・選択できるよう、
// 簡易モデル（ITD＋頭部減衰LPF＋等パワーILD）から各方位のHRIRを生成して登録する。
// 実測HRTFを使う場合は Tools/sofa_to_hrtf_cpp.py で生成した HRTFData_*.cpp を追加すること。

#include "HRTF/HRTFRegistry.h"
#include "Math/UnrealMathUtility.h"

namespace
{
	struct FBuiltinHRTF
	{
		TArray<float> Az;
		TArray<float> L;
		TArray<float> R;
		int32 SampleRate = 48000;
		int32 Taps = 128;
		int32 Count = 0;

		FBuiltinHRTF()
		{
			Build();

			FReaperHRTFProfile P;
			// 言語に依存しない固定の内部名（保存データ・検索キー）。表示名はUI側で言語ごとに翻訳する
			// （UWG_RadNodzToolkit::HrtfDisplayNameForInternal）。
			P.Name        = TEXT("Generic");
			P.Attribution = FString();   // 自前生成のため第三者ライセンス表記なし
			P.SampleRate  = SampleRate;
			P.Taps        = Taps;
			P.Count       = Count;
			P.AzDeg       = Az.GetData();
			P.L           = L.GetData();
			P.R           = R.GetData();
			FReaperHRTFRegistry::Get().Register(P);
		}

		void Build()
		{
			const int32 NumAz = 36;                 // 10度刻み
			Count = NumAz;
			Az.SetNum(NumAz);
			L.SetNumZeroed(NumAz * Taps);
			R.SetNumZeroed(NumAz * Taps);

			const float MaxItdMs = 0.7f;
			const int32 MaxDelay = FMath::Clamp(FMath::RoundToInt((MaxItdMs / 1000.f) * SampleRate), 0, Taps - 1);

			for (int32 i = 0; i < NumAz; ++i)
			{
				const float Deg = -180.f + (360.f / NumAz) * i;
				Az[i] = Deg;

				const float Rad = FMath::DegreesToRadians(Deg);
				const float Pan = FMath::Clamp(FMath::Sin(Rad), -1.f, 1.f);  // -1=左 / +1=右

				// 等パワーパン（ILD）
				const float A = (Pan + 1.f) * 0.5f * (PI * 0.5f);
				const float GLeft  = FMath::Cos(A);
				const float GRight = FMath::Sin(A);

				const bool  bNearLeft = (Pan <= 0.f);
				const int32 DelayFar  = FMath::RoundToInt(MaxDelay * FMath::Abs(Pan));
				// 頭部減衰の一次LPF係数（|pan|が大きいほど低く＝こもる）
				const float Shadow = FMath::Lerp(1.0f, 0.35f, FMath::Abs(Pan));

				float* HL = &L[i * Taps];
				float* HR = &R[i * Taps];

				// 近い耳：遅延なしのデルタ（gain）。遠い耳：遅延＋一次LPFのインパルス応答。
				auto WriteEar = [&](float* H, float Gain, bool bFar)
				{
					if (!bFar)
					{
						H[0] = Gain;
					}
					else
					{
						for (int32 n = 0; n < Taps; ++n)
						{
							const int32 k = n - DelayFar;
							if (k < 0) continue;
							const float Ir = Shadow * FMath::Pow(1.f - Shadow, (float)k);   // h[k]=α(1-α)^k
							H[n] = Gain * Ir;
						}
					}
				};

				WriteEar(HL, GLeft,  !bNearLeft);   // 左耳：近側でないなら遠い耳
				WriteEar(HR, GRight,  bNearLeft);   // 右耳：近側が左なら右が遠い耳
			}
		}
	};

	// モジュール読み込み時に生成・登録（静的寿命なので配列ポインタは有効）
	static const FBuiltinHRTF GBuiltinHRTF;
}
