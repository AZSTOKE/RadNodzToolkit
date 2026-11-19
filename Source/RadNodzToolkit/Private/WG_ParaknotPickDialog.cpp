// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).


#include "WG_ParaknotPickDialog.h"

#include "Blueprint/WidgetTree.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

#include "ReaperLang.h"

namespace PickDlgUI
{
	// 行ボタンの背景。
	//
	// **既定のボタン背景（リニア 0.495 の明るいグレー）のままにしない。**
	// 明るい地に白文字を載せると読めず、かといって文字を暗くすると
	// 今度は背景に溶ける。濃さで区別し、文字はどちらも白に揃える。
	//
	// 配信モードのほうをパネル(0.10/0.12/0.14)より暗くして沈ませる。
	static const FLinearColor ReadyBg   = FLinearColor(0.05f, 0.07f, 0.09f, 1.f);
	static const FLinearColor DimmedBg  = FLinearColor(0.22f, 0.25f, 0.28f, 1.f);
}

void UWG_ParaknotPickDialog::Setup(const FString& InTitle, const TArray<FString>& InItems,
	const TArray<bool>& InDimmed)
{
	Title = InTitle;
	Items = InItems;
	Dimmed = InDimmed;

	// 上限を超えた分は出さない（出しても押すハンドラが無いため）。
	if (Items.Num() > MaxItems)
	{
		Items.SetNum(MaxItems);
	}

	// **足りない分は「暗くしない」で埋める。** 添字で引くので、
	// 長さが違うと行ごとに範囲確認が要る。
	Dimmed.SetNum(Items.Num());

	if (TitleText)
	{
		TitleText->SetText(FText::FromString(Title));
	}
}

TSharedRef<SWidget> UWG_ParaknotPickDialog::RebuildWidget()
{
	if (!bUICreated && WidgetTree)
	{
		bUICreated = true;

		// 画面全体を覆う暗幕
		UBorder* Backdrop = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Backdrop"));
		Backdrop->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.6f));
		Backdrop->SetHorizontalAlignment(HAlign_Center);
		Backdrop->SetVerticalAlignment(VAlign_Center);
		WidgetTree->RootWidget = Backdrop;

		// 中央パネル
		UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Panel"));
		Panel->SetBrushColor(FLinearColor(0.10f, 0.12f, 0.14f, 1.f));
		Panel->SetPadding(FMargin(24.f));
		Backdrop->SetContent(Panel);

		UVerticalBox* Box = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
		Panel->SetContent(Box);

		TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		TitleText->SetText(FText::FromString(Title));
		TitleText->SetAutoWrapText(true);
		if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(TitleText)))
		{
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
			S->SetHorizontalAlignment(HAlign_Center);
		}

		ItemBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
		Box->AddChild(ItemBox);

		// 行ボタン。ハンドラは固定数なので、対応する番号のものを結ぶ。
		for (int32 i = 0; i < Items.Num(); ++i)
		{
			UButton* Btn = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(),
				*FString::Printf(TEXT("PickButton%d"), i));

			// すぐ使えるインスタンス（モニターなら配信モード）は背景を暗くして沈ませる。
			// **押せなくはしない。** 使えないほうを選べば、呼び出し側が切り替えを確認する。
			//
			// 区別を**文字色ではなく背景の濃淡で付ける**のは、文字を暗くすると
			// 読めなくなるため。文字はどちらも白。
			{
				const FLinearColor BgColor = Dimmed[i] ? PickDlgUI::DimmedBg : PickDlgUI::ReadyBg;

				FButtonStyle Style = Btn->GetStyle();
				Style.Normal  = FSlateRoundedBoxBrush(FLinearColor::White, 10.f);
				Style.Hovered = FSlateRoundedBoxBrush(FLinearColor(1.12f, 1.12f, 1.12f, 1.f), 10.f);
				Style.Pressed = FSlateRoundedBoxBrush(FLinearColor(0.85f, 0.85f, 0.85f, 1.f), 10.f);
				Btn->SetStyle(Style);
				Btn->SetBackgroundColor(BgColor);
			}

			UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			T->SetText(FText::FromString(Items[i]));
			T->SetJustification(ETextJustify::Center);
			T->SetColorAndOpacity(FSlateColor(FLinearColor::White));

			Btn->AddChild(T);

			if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(ItemBox->AddChild(Btn)))
			{
				S->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
				S->SetHorizontalAlignment(HAlign_Fill);
			}

			switch (i)
			{
				case 0: Btn->OnClicked.AddDynamic(this, &UWG_ParaknotPickDialog::HandlePick0); break;
				case 1: Btn->OnClicked.AddDynamic(this, &UWG_ParaknotPickDialog::HandlePick1); break;
				case 2: Btn->OnClicked.AddDynamic(this, &UWG_ParaknotPickDialog::HandlePick2); break;
				case 3: Btn->OnClicked.AddDynamic(this, &UWG_ParaknotPickDialog::HandlePick3); break;
				case 4: Btn->OnClicked.AddDynamic(this, &UWG_ParaknotPickDialog::HandlePick4); break;
				case 5: Btn->OnClicked.AddDynamic(this, &UWG_ParaknotPickDialog::HandlePick5); break;
				case 6: Btn->OnClicked.AddDynamic(this, &UWG_ParaknotPickDialog::HandlePick6); break;
				case 7: Btn->OnClicked.AddDynamic(this, &UWG_ParaknotPickDialog::HandlePick7); break;
				default: break;
			}
		}

		CancelButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("CancelButton"));
		{
			UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			T->SetText(FText::FromString(ReaperLang::S(TEXT("キャンセル"), TEXT("Cancel"))));
			T->SetJustification(ETextJustify::Center);
			CancelButton->AddChild(T);
		}
		if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(CancelButton)))
		{
			S->SetPadding(FMargin(0.f, 12.f, 0.f, 0.f));
			S->SetHorizontalAlignment(HAlign_Fill);
		}
		CancelButton->OnClicked.AddDynamic(this, &UWG_ParaknotPickDialog::HandleCancel);
	}

	return Super::RebuildWidget();
}

void UWG_ParaknotPickDialog::Pick(int32 Index)
{
	// **先に閉じる。** 選択後の処理でさらにダイアログを出すことがあるので、
	// こちらが残っていると重なって見えなくなる。
	RemoveFromParent();

	OnPicked.ExecuteIfBound(Index);
}

void UWG_ParaknotPickDialog::HandleCancel() { RemoveFromParent(); }

void UWG_ParaknotPickDialog::HandlePick0() { Pick(0); }
void UWG_ParaknotPickDialog::HandlePick1() { Pick(1); }
void UWG_ParaknotPickDialog::HandlePick2() { Pick(2); }
void UWG_ParaknotPickDialog::HandlePick3() { Pick(3); }
void UWG_ParaknotPickDialog::HandlePick4() { Pick(4); }
void UWG_ParaknotPickDialog::HandlePick5() { Pick(5); }
void UWG_ParaknotPickDialog::HandlePick6() { Pick(6); }
void UWG_ParaknotPickDialog::HandlePick7() { Pick(7); }
