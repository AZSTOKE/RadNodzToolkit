// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"
#include "ReaperMonitorOutputMode.generated.h"

/**
 * モニター受信音声の出力方法。
 *
 *   Stereo      : 受信chをステレオ(2ch)へダウンミックスして出力（既定）。
 *   Spatial     : 各chを“仮想スピーカー”として頭の周りに配置し、HRTFでバイノーラル化（ヘッドホン向け立体音響）。
 *                 ※ Resonance Audio プラグインを各プラットフォームの Spatialization Plugin に指定しておく必要がある。
 *   Passthrough : 受信したch数のまま出力（従来動作）。
 */
UENUM(BlueprintType)
enum class EMonitorOutputMode : uint8
{
	Stereo       UMETA(DisplayName = "Stereo Downmix"),
	Spatial      UMETA(DisplayName = "3D Spatial (Binaural)"),
	Passthrough  UMETA(DisplayName = "As-is"),
};

/**
 * 立体音響(バイノーラル)で受信chをどの方位に割り当てるか（ch順マッピング）。
 *   ReaperITU : L R C LFE Ls Rs (/ +Lb Rb)  ※REAPER標準
 *   Film      : L R Ls Rs C LFE (/ 8ch: L R Ls Rs Lb Rb C LFE)
 *   Sequential: 円周上に等間隔
 */
UENUM(BlueprintType)
enum class EBinauralChannelOrder : uint8
{
	ReaperITU   UMETA(DisplayName = "REAPER/ITU"),
	Film        UMETA(DisplayName = "Film"),
	Sequential  UMETA(DisplayName = "Sequential"),
	Custom      UMETA(DisplayName = "Custom"),   // 各chの角度・距離を個別指定
};
