// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

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
 * REAPER の再生音をどの経路で受け取るか。
 *
 *   Paraknot  : REAPER に挿した PARAKNOT の「配信」を購読する（既定）。
 *               UDP + AEAD で暗号化され、接続先と鍵は RADNODZ の業務 TCP 越しに
 *               受け取る（AZ_PARAKNOT_OpenOutputSession）。
 *   ReaStream : 従来方式。REAPER の ReaStream(JSFX) が投げる平文 UDP を
 *               固定ポート(58710)で待ち受ける。**PARAKNOT が使えないときの退避。**
 *
 * 出力方法(EMonitorOutputMode)や立体音響の設定は**どちらでも同じように効く**。
 * 違うのは「音をどこから貰うか」だけで、ダウンミックス以降の処理は共通。
 */
UENUM(BlueprintType)
enum class EMonitorSource : uint8
{
	Paraknot   UMETA(DisplayName = "PARAKNOT"),
	ReaStream  UMETA(DisplayName = "ReaStream"),
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
