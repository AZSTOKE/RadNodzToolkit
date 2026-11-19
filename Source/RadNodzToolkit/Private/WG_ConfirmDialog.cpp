// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).


#include "WG_ConfirmDialog.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

#include "ReaperDialogStyle.h"
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
		BuildUI();
	}

	return Super::RebuildWidget();
}

void UWG_ConfirmDialog::BuildUI()
{
	using namespace ReaperDialogStyle;

	UVerticalBox* BodyCol = BuildFrame(this, ReaperLang::S(TEXT("確認"), TEXT("Confirm")), 720.f);

	// メッセージ
	{
		MessageText = MakeText(WidgetTree, Message, 22, TextPrimary, true);
		MessageText->SetAutoWrapText(true);
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(MessageText);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(2.f, 4.f, 2.f, 22.f));
	}

	// いいえ / はい
	{
		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		NoButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("いいえ"), TEXT("No")), StopGray, TEXT("NoButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(72.f);
			B->AddChild(NoButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		}

		YesButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("はい"), TEXT("Yes")), AccentGreen, TEXT("YesButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(72.f);
			B->AddChild(YesButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(8.f, 0.f, 0.f, 0.f));
		}

		YesButton->OnClicked.AddDynamic(this, &UWG_ConfirmDialog::HandleYes);
		NoButton->OnClicked.AddDynamic(this, &UWG_ConfirmDialog::HandleNo);

		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(Row);
		S->SetHorizontalAlignment(HAlign_Fill);
	}
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
