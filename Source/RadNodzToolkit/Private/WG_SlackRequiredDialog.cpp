// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_SlackRequiredDialog.h"
#include "WG_RadNodzToolkit.h"
#include "ReaperFont.h"
#include "ReaperLang.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Brushes/SlateRoundedBoxBrush.h"

namespace SlackReqDlgUI
{
	static const FLinearColor DimBg       = FLinearColor(0.0f, 0.0f, 0.0f, 0.82f);   // 画面を暗くする
	static const FLinearColor PanelBg     = FLinearColor(0.036f, 0.080f, 0.094f, 1.f);
	static const FLinearColor HeaderBg    = FLinearColor(0.050f, 0.105f, 0.122f, 1.f);
	static const FLinearColor AccentBlue  = FLinearColor(0.16f, 0.55f, 0.85f, 1.f);
	static const FLinearColor StopGray    = FLinearColor(0.085f, 0.150f, 0.170f, 1.f);
	static const FLinearColor TextPrimary = FLinearColor(0.85f, 0.93f, 0.94f, 1.f);
	static const FLinearColor TextDim     = FLinearColor(0.40f, 0.58f, 0.61f, 1.f);

	static UTextBlock* MakeText(UWidgetTree* Tree, const FString& Str, int32 Size, FLinearColor Color, bool bBold = false)
	{
		UTextBlock* T = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(Str));
		T->SetFont(ReaperFont::Get(Size, bBold));
		T->SetColorAndOpacity(FSlateColor(Color));
		return T;
	}

	static UButton* MakeButton(UWidgetTree* Tree, const FString& Label, FLinearColor BgColor, FName Name)
	{
		UButton* Btn = Tree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
		FButtonStyle Style = Btn->GetStyle();
		Style.Normal  = FSlateRoundedBoxBrush(FLinearColor::White, 10.f);
		Style.Hovered = FSlateRoundedBoxBrush(FLinearColor(1.12f, 1.12f, 1.12f, 1.f), 10.f);
		Style.Pressed = FSlateRoundedBoxBrush(FLinearColor(0.85f, 0.85f, 0.85f, 1.f), 10.f);
		Btn->SetStyle(Style);
		Btn->SetBackgroundColor(BgColor);
		UTextBlock* Txt = MakeText(Tree, Label, 20, FLinearColor::White, true);
		Txt->SetJustification(ETextJustify::Center);
		Txt->SetAutoWrapText(!ReaperLang::IsJapanese());   // 日本語は折り返し不要。長い翻訳文字列だけボタン幅をはみ出さず2行に折り返す
		Btn->SetContent(Txt);
		return Btn;
	}
}

void UWG_SlackRequiredDialog::Setup(UWG_RadNodzToolkit* InOwner)
{
	Owner = InOwner;
}

TSharedRef<SWidget> UWG_SlackRequiredDialog::RebuildWidget()
{
	if (WidgetTree && !bUICreated)
	{
		BuildUI();
		bUICreated = true;
	}
	return Super::RebuildWidget();
}

void UWG_SlackRequiredDialog::NativeConstruct()
{
	Super::NativeConstruct();

	if (OkButton && !OkButton->OnClicked.IsAlreadyBound(this, &UWG_SlackRequiredDialog::HandleOk))
		OkButton->OnClicked.AddDynamic(this, &UWG_SlackRequiredDialog::HandleOk);
	if (GoToSettingsButton && !GoToSettingsButton->OnClicked.IsAlreadyBound(this, &UWG_SlackRequiredDialog::HandleGoToSettings))
		GoToSettingsButton->OnClicked.AddDynamic(this, &UWG_SlackRequiredDialog::HandleGoToSettings);
}

void UWG_SlackRequiredDialog::BuildUI()
{
	using namespace SlackReqDlgUI;

	// 画面全体を覆う暗幕（背面のクリックを通さない）
	UBorder* Dim = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Dim"));
	Dim->SetBrushColor(DimBg);
	Dim->SetHorizontalAlignment(HAlign_Center);
	Dim->SetVerticalAlignment(VAlign_Center);
	WidgetTree->RootWidget = Dim;

	// 中央パネル
	USizeBox* PanelSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("PanelSize"));
	PanelSize->SetWidthOverride(560.f);
	Dim->SetContent(PanelSize);

	UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Panel"));
	Panel->SetBrushColor(PanelBg);
	Panel->SetPadding(FMargin(0.f));
	PanelSize->AddChild(Panel);

	UVerticalBox* Col = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Col"));
	Panel->SetContent(Col);

	// タイトル
	{
		UBorder* Head = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
		Head->SetBrushColor(HeaderBg);
		Head->SetPadding(FMargin(28.f, 20.f));
		Head->SetContent(MakeText(WidgetTree,
			ReaperLang::S(TEXT("Slackの設定が必要です"), TEXT("Slack setup required")), 24, TextPrimary, true));
		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(Head);
		S->SetHorizontalAlignment(HAlign_Fill);
	}

	// 本体
	UBorder* Body = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
	Body->SetBrushColor(PanelBg);
	Body->SetPadding(FMargin(28.f, 22.f));
	{
		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(Body);
		S->SetHorizontalAlignment(HAlign_Fill);
	}
	UVerticalBox* BodyCol = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	Body->SetContent(BodyCol);

	// 説明
	{
		UTextBlock* L = MakeText(WidgetTree,
			ReaperLang::S(TEXT("Slackの設定をしてください。"), TEXT("Please set up Slack first.")),
			20, TextPrimary, true);
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(L);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 20.f));
	}

	// OK / 設定へ
	{
		UHorizontalBox* BtnRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		OkButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("OK"), TEXT("OK")), StopGray, TEXT("OkButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(64.f);
			B->AddChild(OkButton);
			UHorizontalBoxSlot* S = BtnRow->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		}

		GoToSettingsButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("設定へ"), TEXT("Go to Settings")), AccentBlue, TEXT("GoToSettingsButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(64.f);
			B->AddChild(GoToSettingsButton);
			UHorizontalBoxSlot* S = BtnRow->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(8.f, 0.f, 0.f, 0.f));
		}

		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(BtnRow);
		S->SetHorizontalAlignment(HAlign_Fill);
	}
}

void UWG_SlackRequiredDialog::HandleOk()
{
	RemoveFromParent();
}

void UWG_SlackRequiredDialog::HandleGoToSettings()
{
	if (Owner)
	{
		Owner->SetActiveTab(2);
	}
	RemoveFromParent();
}
