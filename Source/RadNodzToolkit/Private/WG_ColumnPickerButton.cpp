// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#include "WG_ColumnPickerButton.h"
#include "WG_RadNodzToolkit.h"
#include "ReaperFont.h"

#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Brushes/SlateRoundedBoxBrush.h"

namespace ColumnPickerUI
{
	static const FLinearColor BtnBg      = FLinearColor(0.098f, 0.243f, 0.278f, 1.f);
	static const FLinearColor BtnBgHover = FLinearColor(0.130f, 0.320f, 0.360f, 1.f);
	static const FLinearColor TextOn     = FLinearColor(0.92f, 0.97f, 0.98f, 1.f);
}

void UWG_ColumnPickerButton::InitButton(int32 InColIndex, const FString& InLabel, UWG_RadNodzToolkit* InOwner)
{
	ColIndex = InColIndex;
	Label    = InLabel;
	Owner    = InOwner;

	if (bUICreated && ColText)
	{
		ColText->SetText(FText::FromString(Label));
	}
}

TSharedRef<SWidget> UWG_ColumnPickerButton::RebuildWidget()
{
	if (WidgetTree && !bUICreated)
	{
		BuildUI();
		bUICreated = true;
	}
	return Super::RebuildWidget();
}

void UWG_ColumnPickerButton::NativeConstruct()
{
	Super::NativeConstruct();
	if (ColButton && !ColButton->OnClicked.IsAlreadyBound(this, &UWG_ColumnPickerButton::HandleClicked))
	{
		ColButton->OnClicked.AddDynamic(this, &UWG_ColumnPickerButton::HandleClicked);
	}
}

void UWG_ColumnPickerButton::BuildUI()
{
	using namespace ColumnPickerUI;

	ColButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ColumnPickerButton"));

	FButtonStyle Style = ColButton->GetStyle();
	Style.Normal  = FSlateRoundedBoxBrush(BtnBg, 8.f);
	Style.Hovered = FSlateRoundedBoxBrush(BtnBgHover, 8.f);
	Style.Pressed = FSlateRoundedBoxBrush(BtnBgHover * 0.85f, 8.f);
	ColButton->SetStyle(Style);

	ColText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	ColText->SetText(FText::FromString(Label));
	ColText->SetFont(ReaperFont::Get(18, true));
	ColText->SetColorAndOpacity(FSlateColor(TextOn));
	ColText->SetJustification(ETextJustify::Center);
	ColButton->SetContent(ColText);

	USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	B->SetMinDesiredWidth(64.f);
	B->SetMinDesiredHeight(48.f);
	B->AddChild(ColButton);
	WidgetTree->RootWidget = B;
}

void UWG_ColumnPickerButton::HandleClicked()
{
	if (Owner)
	{
		Owner->HandleScriptMatchColumnChosen(ColIndex);
	}
}
