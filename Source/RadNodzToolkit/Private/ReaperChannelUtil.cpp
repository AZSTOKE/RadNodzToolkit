// Fill out your copyright notice in the Description page of Project Settings.

#include "ReaperChannelUtil.h"
#include "Components/ComboBoxString.h"

namespace ReaperChannelUtil
{
	void FillInputChannelOptions(UComboBoxString* Combo, const TArray<FString>& Names, int32 SelectIndex)
	{
		if (!Combo) return;

		Combo->ClearOptions();
		if (Names.Num() > 0)
		{
			// 接続中：Reaperの入力チャンネル名一覧
			for (const FString& N : Names) { Combo->AddOption(N); }
		}
		else
		{
			// 未接続：番号(0〜31)で代替
			for (int32 c = 0; c < 32; ++c) { Combo->AddOption(FString::FromInt(c)); }
		}

		const int32 Count = Combo->GetOptionCount();
		Combo->SetSelectedIndex(FMath::Clamp(SelectIndex, 0, FMath::Max(0, Count - 1)));
	}
}
