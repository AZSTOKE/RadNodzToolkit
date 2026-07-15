// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"

class UComboBoxString;

/**
 * マイク／トラックの入力チャンネル設定で共通して使う小物。
 * 「構成（モノ/ステレオ/5.1）コンボの index ⇔ 実ch数」変換と、
 * 入力chコンボの選択肢生成をまとめ、各UI（マイク登録ダイアログ／トラックカード）から共用する。
 */
namespace ReaperChannelUtil
{
	// 構成コンボの index ⇔ 実ch数（0=モノ:1ch / 1=ステレオ:2ch / 2=5.1:6ch）
	inline int32 NumChannelsToIndex(int32 NumCh)
	{
		switch (NumCh) { case 2: return 1; case 6: return 2; default: return 0; }
	}
	inline int32 IndexToNumChannels(int32 Index)
	{
		switch (Index) { case 1: return 2; case 2: return 6; default: return 1; }
	}

	/**
	 * 入力ch（開始）コンボの選択肢を埋める。
	 * Names があれば入力チャンネル名、無ければ番号 0〜31 を入れ、SelectIndex を範囲内に収めて選択する。
	 */
	RADNODZTOOLKIT_API void FillInputChannelOptions(UComboBoxString* Combo, const TArray<FString>& Names, int32 SelectIndex);
}
