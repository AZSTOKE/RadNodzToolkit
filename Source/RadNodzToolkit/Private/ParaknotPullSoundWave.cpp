// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ParaknotPullSoundWave.h"

int32 UParaknotPullSoundWave::OnGeneratePCMAudio(TArray<uint8>& OutAudio, int32 NumSamples)
{
	if (!Generator || NumSamples <= 0)
	{
		return 0;
	}

	// 書き込み先を 0 で用意してから渡す（USynthSound と同じ作法）。
	// 生成器が足りない分を書かなくても無音になる。
	OutAudio.Reset();
	OutAudio.AddZeroed(NumSamples * sizeof(float));

	return Generator(reinterpret_cast<float*>(OutAudio.GetData()), NumSamples);
}
