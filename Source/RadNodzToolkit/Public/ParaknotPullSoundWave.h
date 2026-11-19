// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundWaveProcedural.h"

#include "ParaknotPullSoundWave.generated.h"

/**
 * 再生側の求めに応じて音声を引き出す USoundWaveProcedural。
 *
 * **キュー (QueueAudio) を使わない。** ミキサーがデータを要求したときに
 * オーディオの描画スレッドから OnGeneratePCMAudio が呼ばれるので、そこで
 * 要求された分だけ生成器から書き込む。消費の速さを決めるのは再生デバイス
 * だけになり、供給側の時計とのずれで音が溜まることがない。
 *
 * 別スレッドから一定間隔で QueueAudio する方式は、供給の間隔と 1 回分の長さが
 * 合わないとキューに音が溜まり続け（Android で顕著）、遅延が時間とともに伸びるため使わない。
 *
 * 出力は float (USynthSound と同じ)。int16 へ丸めない。
 */
UCLASS()
class RADNODZTOOLKIT_API UParaknotPullSoundWave : public USoundWaveProcedural
{
	GENERATED_BODY()

public:
	/**
	 * 生成器。**オーディオの描画スレッドから呼ばれる。**
	 *
	 * @param OutInterleaved 書き込み先（ch インターリーブ、0 で埋めてある）
	 * @param NumSamples     全 ch 合計のサンプル数（フレーム数 × ch 数）
	 * @return 書いたサンプル数。0 なら無音のまま。
	 */
	using FGenerator = TFunction<int32(float* OutInterleaved, int32 NumSamples)>;

	/** **Play する前に 1 度だけ設定する。** 再生中に差し替えない（描画スレッドが読んでいる）。 */
	void SetGenerator(FGenerator InGenerator) { Generator = MoveTemp(InGenerator); }

	virtual int32 OnGeneratePCMAudio(TArray<uint8>& OutAudio, int32 NumSamples) override;

	virtual Audio::EAudioMixerStreamDataFormat::Type GetGeneratedPCMDataFormat() const override
	{
		return Audio::EAudioMixerStreamDataFormat::Float;
	}

private:
	FGenerator Generator;
};
