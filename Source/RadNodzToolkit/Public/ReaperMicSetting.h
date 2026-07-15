// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"
#include "ReaperMicSetting.generated.h"

/**
 * 登録マイク1本ぶんの設定。
 * 名前に加えて、トラック生成時に適用する入力チャンネル設定（開始ch・ch数）を持つ。
 * 種別はAudio固定（MIDIは扱わない）。ch数: 1=モノ / 2=ステレオ / 6=5.1 など。
 */
USTRUCT(BlueprintType)
struct FMicSetting
{
	GENERATED_BODY()

	/** マイク名（例: OH）。 */
	UPROPERTY()
	FString Name;

	/** 入力チャンネルの開始index（AZ_GetInputChannelList のindexに対応）。 */
	UPROPERTY()
	int32 FirstChannel = 0;

	/** 入力チャンネル数（1=モノ / 2=ステレオ / 6=5.1 …）。 */
	UPROPERTY()
	int32 NumChannels = 1;
};
