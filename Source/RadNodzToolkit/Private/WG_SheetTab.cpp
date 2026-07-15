// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#include "WG_SheetTab.h"
#include "WG_RadNodzToolkit.h"
#include "ReaperFont.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Styling/SlateColor.h"
#include "Brushes/SlateColorBrush.h"

namespace SheetTabUI
{
	static const FLinearColor AccentBlue = FLinearColor(0.16f, 0.80f, 0.74f, 1.f);   // 下部ナビと同じティール
	static const FLinearColor TextDim    = FLinearColor(0.45f, 0.62f, 0.65f, 1.f);
}

void UWG_SheetTab::InitTab(int32 InIndex, const FString& InLabel, bool bInActive, UWG_RadNodzToolkit* InOwner)
{
	TabIndex = InIndex;
	Label    = InLabel;
	bActive  = bInActive;
	Owner    = InOwner;

	if (bUICreated && TabText)
	{
		TabText->SetText(FText::FromString(Label));
		SetActiveVisual(bActive);
	}
}

TSharedRef<SWidget> UWG_SheetTab::RebuildWidget()
{
	if (WidgetTree && !bUICreated)
	{
		BuildUI();
		bUICreated = true;
	}
	return Super::RebuildWidget();
}

void UWG_SheetTab::NativeConstruct()
{
	Super::NativeConstruct();
	if (TabButton && !TabButton->OnClicked.IsAlreadyBound(this, &UWG_SheetTab::HandleClicked))
	{
		TabButton->OnClicked.AddDynamic(this, &UWG_SheetTab::HandleClicked);
	}
}

void UWG_SheetTab::BuildUI()
{
	using namespace SheetTabUI;

	UVerticalBox* Cell = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("SheetTabRoot"));
	WidgetTree->RootWidget = Cell;

	// アクティブ表示の細線（上端）
	Indicator = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
	Indicator->SetBrushColor(AccentBlue);
	Indicator->SetPadding(FMargin(0.f));
	{
		USizeBox* IndH = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		IndH->SetHeightOverride(3.f);
		IndH->AddChild(Indicator);
		UVerticalBoxSlot* S = Cell->AddChildToVerticalBox(IndH);
		S->SetHorizontalAlignment(HAlign_Fill);
	}

	// 透明背景のボタン＋中央ラベル
	TabButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("SheetTabButton"));
	{
		FButtonStyle Style = TabButton->GetStyle();
		const FSlateColorBrush Clear(FLinearColor(0.f, 0.f, 0.f, 0.f));
		const FSlateColorBrush Press(FLinearColor(1.f, 1.f, 1.f, 0.06f));
		Style.Normal   = Clear;
		Style.Hovered  = Press;
		Style.Pressed  = Press;
		Style.Disabled = Clear;
		TabButton->SetStyle(Style);
	}

	TabText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	TabText->SetText(FText::FromString(Label));
	TabText->SetFont(ReaperFont::Get(18, true));
	TabText->SetJustification(ETextJustify::Center);
	TabButton->SetContent(TabText);

	{
		// 押しやすい高さ＋左右に余白を確保（横並びでタブらしく）
		USizeBox* BtnH = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnH->SetMinDesiredWidth(96.f);
		BtnH->SetMinDesiredHeight(56.f);
		BtnH->AddChild(TabButton);
		UVerticalBoxSlot* S = Cell->AddChildToVerticalBox(BtnH);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(14.f, 4.f, 14.f, 4.f));
	}

	SetActiveVisual(bActive);
}

void UWG_SheetTab::SetActiveVisual(bool bInActive)
{
	using namespace SheetTabUI;
	bActive = bInActive;
	if (TabText)   { TabText->SetColorAndOpacity(FSlateColor(bActive ? AccentBlue : TextDim)); }
	if (Indicator) { Indicator->SetVisibility(bActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden); }
}

void UWG_SheetTab::HandleClicked()
{
	if (Owner)
	{
		Owner->SwitchListSheet(TabIndex);
	}
}
