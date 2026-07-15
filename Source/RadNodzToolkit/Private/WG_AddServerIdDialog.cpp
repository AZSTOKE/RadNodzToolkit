// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.




#include "WG_AddServerIdDialog.h"
#include "WG_RadNodzToolkit.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "ReaperLang.h"

void UWG_AddServerIdDialog::Setup(UWG_RadNodzToolkit* InOwner)
{
	Owner = InOwner;
}

TSharedRef<SWidget> UWG_AddServerIdDialog::RebuildWidget()
{
	if (!bUICreated && WidgetTree)
	{
		bUICreated = true;

		// 画面全体を覆う暗幕（クリックは奥に通さない）
		UBorder* Backdrop = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Backdrop"));
		Backdrop->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.6f));
		Backdrop->SetHorizontalAlignment(HAlign_Center);
		Backdrop->SetVerticalAlignment(VAlign_Center);
		WidgetTree->RootWidget = Backdrop;

		// 中央のパネル
		UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Panel"));
		Panel->SetBrushColor(FLinearColor(0.10f, 0.12f, 0.14f, 1.f));
		Panel->SetPadding(FMargin(24.f));
		Backdrop->SetContent(Panel);

		UVerticalBox* Box = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
		Panel->SetContent(Box);

		// タイトル
		UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Title->SetText(FText::FromString(ReaperLang::S(TEXT("新しいサーバーIDを登録"), TEXT("Register a new server ID"))));
		if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(Title)))
		{
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 12.f));
			S->SetHorizontalAlignment(HAlign_Center);
		}

		// 入力欄
		IdInput = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass());
		IdInput->SetHintText(FText::FromString(TEXT("MyReaper")));
		// 入力文字は黒（明るい入力欄背景で読めるように）
		IdInput->WidgetStyle.SetForegroundColor(FSlateColor(FLinearColor::Black));
		IdInput->WidgetStyle.FocusedForegroundColor = FSlateColor(FLinearColor::Black);
		IdInput->WidgetStyle.ReadOnlyForegroundColor = FSlateColor(FLinearColor::Black);
		IdInput->OnTextCommitted.AddDynamic(this, &UWG_AddServerIdDialog::HandleInputCommitted);
		if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(IdInput)))
		{
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		// ボタン行（登録 / キャンセル）
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

		RegisterButton = MakeBtn(ReaperLang::S(TEXT("登録"), TEXT("Register")), TEXT("RegisterButton"));
		CancelButton = MakeBtn(ReaperLang::S(TEXT("キャンセル"), TEXT("Cancel")), TEXT("CancelButton"));
		RegisterButton->OnClicked.AddDynamic(this, &UWG_AddServerIdDialog::HandleRegister);
		CancelButton->OnClicked.AddDynamic(this, &UWG_AddServerIdDialog::HandleCancel);
	}

	return Super::RebuildWidget();
}

void UWG_AddServerIdDialog::Commit()
{
	const FString NewId = IdInput ? IdInput->GetText().ToString().TrimStartAndEnd() : FString();
	if (Owner && !NewId.IsEmpty())
	{
		Owner->AddServerAuthEntry(NewId);
	}
	RemoveFromParent();
}

void UWG_AddServerIdDialog::HandleRegister()
{
	Commit();
}

void UWG_AddServerIdDialog::HandleCancel()
{
	RemoveFromParent();
}

void UWG_AddServerIdDialog::HandleInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		Commit();
	}
}
