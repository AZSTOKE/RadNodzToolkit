// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.




#include "WG_ConfirmDialog.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "ReaperLang.h"

void UWG_ConfirmDialog::Setup(const FString& InMessage)
{
	Message = InMessage;
	if (MessageText)
	{
		MessageText->SetText(FText::FromString(Message));
	}
}

TSharedRef<SWidget> UWG_ConfirmDialog::RebuildWidget()
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

		// メッセージ
		MessageText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		MessageText->SetText(FText::FromString(Message));
		MessageText->SetAutoWrapText(true);
		if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(MessageText)))
		{
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
			S->SetHorizontalAlignment(HAlign_Center);
		}

		// ボタン行（はい / いいえ）
		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(Row)))
		{
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		auto MakeBtn = [&](const FString& Label, FName Name) -> UButton*
		{
			UButton* Btn = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
			UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			T->SetText(FText::FromString(Label));
			T->SetJustification(ETextJustify::Center);
			Btn->AddChild(T);
			if (UHorizontalBoxSlot* HS = Cast<UHorizontalBoxSlot>(Row->AddChild(Btn)))
			{
				HS->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				HS->SetPadding(FMargin(4.f));
			}
			return Btn;
		};

		YesButton = MakeBtn(ReaperLang::S(TEXT("はい"), TEXT("Yes")), TEXT("YesButton"));
		NoButton = MakeBtn(ReaperLang::S(TEXT("いいえ"), TEXT("No")), TEXT("NoButton"));
		YesButton->OnClicked.AddDynamic(this, &UWG_ConfirmDialog::HandleYes);
		NoButton->OnClicked.AddDynamic(this, &UWG_ConfirmDialog::HandleNo);
	}

	return Super::RebuildWidget();
}

void UWG_ConfirmDialog::HandleYes()
{
	OnConfirmed.ExecuteIfBound();
	RemoveFromParent();
}

void UWG_ConfirmDialog::HandleNo()
{
	RemoveFromParent();
}
