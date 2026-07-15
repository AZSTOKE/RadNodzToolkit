// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_DeleteConfirmDialog.h"
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
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/SizeBox.h"
#include "Brushes/SlateRoundedBoxBrush.h"

namespace DelDlgUI
{
	static const FLinearColor DimBg       = FLinearColor(0.0f, 0.0f, 0.0f, 0.82f);   // 画面を暗くする
	static const FLinearColor PanelBg     = FLinearColor(0.036f, 0.080f, 0.094f, 1.f);
	static const FLinearColor HeaderBg    = FLinearColor(0.050f, 0.105f, 0.122f, 1.f);
	static const FLinearColor ListBg      = FLinearColor(0.020f, 0.045f, 0.052f, 1.f);
	static const FLinearColor RecRed      = FLinearColor(0.85f, 0.16f, 0.22f, 1.f);   // 削除＝赤
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

void UWG_DeleteConfirmDialog::Setup(UWG_RadNodzToolkit* InOwner, const TArray<FString>& InTrackNames)
{
	Owner = InOwner;
	TrackNames = InTrackNames;

	// Setup が RebuildWidget 後に呼ばれた場合に備えて、一覧を埋め直す
	if (ListBox)
	{
		using namespace DelDlgUI;
		ListBox->ClearChildren();
		for (const FString& Name : TrackNames)
		{
			UTextBlock* Row = MakeText(WidgetTree, Name, 20, TextPrimary, false);
			UVerticalBoxSlot* S = ListBox->AddChildToVerticalBox(Row);
			S->SetPadding(FMargin(6.f, 6.f, 6.f, 6.f));
		}
	}
}

TSharedRef<SWidget> UWG_DeleteConfirmDialog::RebuildWidget()
{
	if (WidgetTree && !bUICreated)
	{
		BuildUI();
		bUICreated = true;
	}
	return Super::RebuildWidget();
}

void UWG_DeleteConfirmDialog::NativeConstruct()
{
	Super::NativeConstruct();

	if (YesButton && !YesButton->OnClicked.IsAlreadyBound(this, &UWG_DeleteConfirmDialog::HandleYes))
		YesButton->OnClicked.AddDynamic(this, &UWG_DeleteConfirmDialog::HandleYes);
	if (NoButton && !NoButton->OnClicked.IsAlreadyBound(this, &UWG_DeleteConfirmDialog::HandleNo))
		NoButton->OnClicked.AddDynamic(this, &UWG_DeleteConfirmDialog::HandleNo);
}

void UWG_DeleteConfirmDialog::BuildUI()
{
	using namespace DelDlgUI;

	// 画面全体を覆う暗幕（背面のクリックを通さない）
	UBorder* Dim = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Dim"));
	Dim->SetBrushColor(DimBg);
	Dim->SetHorizontalAlignment(HAlign_Center);
	Dim->SetVerticalAlignment(VAlign_Center);
	WidgetTree->RootWidget = Dim;

	// 中央パネル
	USizeBox* PanelSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("PanelSize"));
	PanelSize->SetWidthOverride(720.f);
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
			ReaperLang::S(TEXT("トラックを削除"), TEXT("Delete Tracks")), 26, TextPrimary, true));
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
			ReaperLang::S(TEXT("以下のトラックを削除します。"), TEXT("The following tracks will be deleted.")),
			18, TextDim, true);
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(L);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 10.f));
	}

	// 削除対象トラック名の一覧（多い場合はスクロール。高さを上限で固定）
	{
		UBorder* ListPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
		ListPanel->SetBrushColor(ListBg);
		ListPanel->SetPadding(FMargin(10.f, 8.f));

		USizeBox* ScrollSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		ScrollSize->SetMaxDesiredHeight(420.f);   // これを超えるとスクロール
		ListPanel->SetContent(ScrollSize);

		UScrollBox* Scroll = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("DeleteListScroll"));
		ScrollSize->AddChild(Scroll);

		ListBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("DeleteListBox"));
		{
			UScrollBoxSlot* S = Cast<UScrollBoxSlot>(Scroll->AddChild(ListBox));
			if (S) { S->SetHorizontalAlignment(HAlign_Fill); }
		}

		// Setup 済みなら名前を反映
		for (const FString& Name : TrackNames)
		{
			UTextBlock* Row = MakeText(WidgetTree, Name, 20, TextPrimary, false);
			UVerticalBoxSlot* S = ListBox->AddChildToVerticalBox(Row);
			S->SetPadding(FMargin(6.f, 6.f, 6.f, 6.f));
		}

		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(ListPanel);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
	}

	// 確認文
	{
		UTextBlock* L = MakeText(WidgetTree,
			ReaperLang::S(TEXT("本当に削除していいですか？"), TEXT("Are you sure you want to delete them?")),
			22, TextPrimary, true);
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(L);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 16.f));
	}

	// はい / いいえ
	{
		UHorizontalBox* BtnRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		NoButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("いいえ"), TEXT("No")), StopGray, TEXT("NoButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(72.f);
			B->AddChild(NoButton);
			UHorizontalBoxSlot* S = BtnRow->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		}

		YesButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("はい（削除）"), TEXT("Yes (Delete)")), RecRed, TEXT("YesButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(72.f);
			B->AddChild(YesButton);
			UHorizontalBoxSlot* S = BtnRow->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(8.f, 0.f, 0.f, 0.f));
		}

		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(BtnRow);
		S->SetHorizontalAlignment(HAlign_Fill);
	}
}

void UWG_DeleteConfirmDialog::HandleYes()
{
	if (Owner)
	{
		Owner->ConfirmDeleteSelectedTracks();
	}
	RemoveFromParent();
}

void UWG_DeleteConfirmDialog::HandleNo()
{
	// 削除せずに閉じる（選択モードは維持。コントローラ側で戻す）
	if (Owner)
	{
		Owner->CancelDeleteConfirm();
	}
	RemoveFromParent();
}
