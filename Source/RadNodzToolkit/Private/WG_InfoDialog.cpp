// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).


#include "WG_InfoDialog.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/SizeBox.h"

#include "ReaperDialogStyle.h"
#include "ReaperLang.h"

void UWG_InfoDialog::Setup(const FString& InTitle, const FString& InMessage)
{
	Title = InTitle;
	Message = InMessage;

	if (TitleText)
	{
		TitleText->SetText(FText::FromString(Title));
	}
	if (HeaderBorder)
	{
		HeaderBorder->SetVisibility(Title.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
	if (MessageText)
	{
		MessageText->SetText(FText::FromString(Message));
	}
}

TSharedRef<SWidget> UWG_InfoDialog::RebuildWidget()
{
	if (!bUICreated && WidgetTree)
	{
		bUICreated = true;
		BuildUI();
	}

	return Super::RebuildWidget();
}

void UWG_InfoDialog::BuildUI()
{
	using namespace ReaperDialogStyle;

	// 見出し帯は Setup が後から呼ばれても差し替えられるよう、参照を受け取っておく。
	UTextBlock* HeadText = nullptr;
	UBorder* Head = nullptr;
	UVerticalBox* BodyCol = BuildFrame(this, Title, 720.f, &HeadText, &Head);
	TitleText = HeadText;
	HeaderBorder = Head;

	// 本文（FP のような長い1行も、複数行のメッセージも読めるように一段沈めて折り返す）
	{
		UBorder* MsgPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
		MsgPanel->SetBrushColor(SunkenBg);
		MsgPanel->SetPadding(FMargin(18.f, 16.f));

		USizeBox* ScrollSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		ScrollSize->SetMaxDesiredHeight(560.f);   // これを超える長文はスクロール
		MsgPanel->SetContent(ScrollSize);

		UScrollBox* Scroll = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("InfoScroll"));
		ScrollSize->AddChild(Scroll);

		MessageText = MakeText(WidgetTree, Message, 20, TextPrimary, false);
		MessageText->SetAutoWrapText(true);
		if (UScrollBoxSlot* S = Cast<UScrollBoxSlot>(Scroll->AddChild(MessageText)))
		{
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(MsgPanel);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 20.f));
	}

	// OK
	{
		OkButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("OK"), TEXT("OK")), AccentGreen, TEXT("OkButton"));
		OkButton->OnClicked.AddDynamic(this, &UWG_InfoDialog::HandleOk);

		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredHeight(72.f);
		B->AddChild(OkButton);

		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(B);
		S->SetHorizontalAlignment(HAlign_Fill);
	}
}

void UWG_InfoDialog::HandleOk()
{
	RemoveFromParent();
}
