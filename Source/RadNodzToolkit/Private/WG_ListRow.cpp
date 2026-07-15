// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_ListRow.h"
#include "WG_RadNodzToolkit.h"
#include "ReaperFont.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Brushes/SlateRoundedBoxBrush.h"

namespace ListRowUI
{
	static const FLinearColor RowBg       = FLinearColor(0.034f, 0.072f, 0.085f, 1.f);
	static const FLinearColor RowDoneBg   = FLinearColor(0.045f, 0.050f, 0.055f, 1.f);   // 収録済み＝グレー寄り
	static const FLinearColor DoneGreen   = FLinearColor(0.13f, 0.62f, 0.40f, 1.f);
	static const FLinearColor TextPrimary = FLinearColor(0.88f, 0.95f, 0.96f, 1.f);
	static const FLinearColor TextDimmed  = FLinearColor(0.42f, 0.46f, 0.48f, 1.f);      // グレーアウト時
	static const FLinearColor HeaderText  = FLinearColor(0.55f, 0.68f, 0.72f, 1.f);      // 見出し行（列文字/行番号）
	static const FLinearColor HeaderBg    = FLinearColor(0.055f, 0.110f, 0.128f, 1.f);   // 見出し行の背景
	static const FLinearColor HighlightColor = FLinearColor(0.20f, 0.90f, 0.80f, 1.f);   // 台本一致でチェック対象に選んだ列

	// チェックボックス（左端）の見た目
	static const FLinearColor BoxFillIdle = FLinearColor(0.08f, 0.14f, 0.16f, 1.f);      // 未チェック時の中身
	static const FLinearColor BoxOutline  = FLinearColor(0.32f, 0.56f, 0.62f, 1.f);      // 未チェック時の枠

	// 1列ぶんの最小幅（横スクロールで全文が見えるよう、内容が長ければさらに自動拡張）
	static const float CellMinWidth = 220.f;
	// 先頭列（行番号/#）は内容が短いため、他列と同じ最小幅を強制すると隙間が空きすぎる。専用の小さい最小幅を使う。
	static const float RowNumberColMinWidth = 40.f;
}

void UWG_ListRow::InitRow(int32 InRowIndex, const TArray<FString>& InCells, bool bChecked, bool bShowCheckbox, UWG_RadNodzToolkit* InOwner,
	const TArray<float>& InColumnWidths, bool bInHeaderRow, int32 InHighlightColIndex)
{
	RowIndex          = InRowIndex;
	Cells             = InCells;
	ColumnWidths      = InColumnWidths;
	bIsChecked        = bChecked;
	bShowCheck        = bShowCheckbox;
	bIsHeaderRow      = bInHeaderRow;
	HighlightColIndex = InHighlightColIndex;
	Owner             = InOwner;

	if (bUICreated && CellsBox)
	{
		BuildUI();
	}
}

TSharedRef<SWidget> UWG_ListRow::RebuildWidget()
{
	if (WidgetTree && !bUICreated)
	{
		BuildUI();
		bUICreated = true;
	}
	return Super::RebuildWidget();
}

void UWG_ListRow::NativeConstruct()
{
	Super::NativeConstruct();
	if (CheckButton && !CheckButton->OnClicked.IsAlreadyBound(this, &UWG_ListRow::HandleCheckClicked))
	{
		CheckButton->OnClicked.AddDynamic(this, &UWG_ListRow::HandleCheckClicked);
	}
}

void UWG_ListRow::BuildUI()
{
	using namespace ListRowUI;

	if (!WidgetTree->RootWidget)
	{
		RootBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ListRowRoot"));
		RootBorder->SetPadding(FMargin(14.f, 8.f));
		WidgetTree->RootWidget = RootBorder;
	}

	UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	RootBorder->SetContent(Row);

	// 左端：収録済みチェックボックス（チェックでアイコン表示）
	CheckButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ListRowCheck"));
	CheckIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	if (UTexture2D* CheckTex = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/T_icon_check_1.T_icon_check_1")))
	{
		CheckIcon->SetBrushFromTexture(CheckTex);
	}
	CheckIcon->SetDesiredSizeOverride(FVector2D(30.f, 30.f));
	CheckButton->SetContent(CheckIcon);

	CheckSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	CheckSize->SetMinDesiredWidth(54.f);
	CheckSize->SetMinDesiredHeight(54.f);
	CheckSize->AddChild(CheckButton);
	CheckSize->SetVisibility(bShowCheck ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (bIsHeaderRow)
	{
		// 見出し行のチェック位置は空欄にする（クリック不可・枠なし）。列位置だけ揃える。
		CheckButton->SetIsEnabled(false);
		FButtonStyle Style = CheckButton->GetStyle();
		const FSlateRoundedBoxBrush Clear(FLinearColor(0.f, 0.f, 0.f, 0.f), 0.f);
		Style.Normal = Clear; Style.Hovered = Clear; Style.Pressed = Clear; Style.Disabled = Clear;
		CheckButton->SetStyle(Style);
	}
	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(CheckSize);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 14.f, 0.f));
	}

	// 各列の値：折り返さず、列ごとに最小幅を確保（内容が長ければ自動拡張）。
	// 行全体が画面幅を超えたら、リスト側の横スクロールで全文が見える。
	CellsBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	CellTexts.Reset();
	for (int32 i = 0; i < Cells.Num(); ++i)
	{
		UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(Cells[i]));
		T->SetFont(ReaperFont::Get(20, bIsHeaderRow || i == 0));
		T->SetAutoWrapText(false);   // 折り返さない（横スクロールで全文表示）
		if (i == HighlightColIndex)
		{
			// 台本一致でチェック対象に選んだ列は常に強調色（見出し/収録済みグレーアウトより優先）
			T->SetColorAndOpacity(FSlateColor(HighlightColor));
		}
		else if (bIsHeaderRow)
		{
			T->SetColorAndOpacity(FSlateColor(HeaderText));
		}
		CellTexts.Add(T);

		// 実測した列幅（他行と揃えるため）があればそれを使い、無ければ最小幅にフォールバックする。
		// 先頭列（行番号/#）は他列より小さい最小幅にして、詰めて表示する。
		const float MinW = (i == 0) ? RowNumberColMinWidth : CellMinWidth;
		const float Width = ColumnWidths.IsValidIndex(i) ? FMath::Max(MinW, ColumnWidths[i]) : MinW;
		USizeBox* Cell = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		Cell->SetMinDesiredWidth(Width);
		Cell->AddChild(T);

		UHorizontalBoxSlot* S = CellsBox->AddChildToHorizontalBox(Cell);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));   // 内容＋最小幅で自動
		S->SetPadding(FMargin(0.f, 0.f, 18.f, 0.f));
	}
	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(CellsBox);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
	}

	RefreshChecked(bIsChecked);
}

void UWG_ListRow::HandleCheckClicked()
{
	bIsChecked = !bIsChecked;
	RefreshChecked(bIsChecked);
	if (Owner)
	{
		Owner->SetListRowChecked(RowIndex, bIsChecked);
	}
}

void UWG_ListRow::ApplyCheckStyle(bool bChecked)
{
	using namespace ListRowUI;
	if (!CheckButton) return;

	// 未チェック＝枠付きの空ボックス、チェック済み＝緑塗り。
	FButtonStyle Style = CheckButton->GetStyle();
	if (bChecked)
	{
		const FSlateRoundedBoxBrush On(DoneGreen, 7.f, DoneGreen, 2.f);
		Style.Normal  = On;
		Style.Hovered = FSlateRoundedBoxBrush(DoneGreen * 1.10f, 7.f, DoneGreen, 2.f);
		Style.Pressed = FSlateRoundedBoxBrush(DoneGreen * 0.90f, 7.f, DoneGreen, 2.f);
	}
	else
	{
		const FSlateRoundedBoxBrush Off(BoxFillIdle, 7.f, BoxOutline, 2.f);
		Style.Normal  = Off;
		Style.Hovered = FSlateRoundedBoxBrush(BoxFillIdle * 1.4f, 7.f, BoxOutline, 2.f);
		Style.Pressed = FSlateRoundedBoxBrush(BoxFillIdle * 1.4f, 7.f, BoxOutline, 2.f);
	}
	CheckButton->SetStyle(Style);
}

void UWG_ListRow::RefreshChecked(bool bChecked)
{
	using namespace ListRowUI;
	bIsChecked = bChecked;

	if (RootBorder)
	{
		RootBorder->SetBrushColor(bIsHeaderRow ? HeaderBg : (bChecked ? RowDoneBg : RowBg));
	}

	if (bIsHeaderRow)
	{
		// 見出し行はチェックボックス・グレーアウトの対象外。強調列（台本一致でチェック対象に選んだ列）だけ反映する。
		for (int32 i = 0; i < CellTexts.Num(); ++i)
		{
			if (CellTexts[i]) { CellTexts[i]->SetColorAndOpacity(FSlateColor(i == HighlightColIndex ? HighlightColor : HeaderText)); }
		}
		return;
	}

	ApplyCheckStyle(bChecked);

	// チェックアイコンはチェック時のみ表示（未チェックは空ボックス）
	if (CheckIcon)
	{
		CheckIcon->SetVisibility(bChecked ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	// 収録済みは文字をグレーアウト（強調列は優先して常に強調色を保つ）
	const FLinearColor Col = bChecked ? TextDimmed : TextPrimary;
	for (int32 i = 0; i < CellTexts.Num(); ++i)
	{
		if (CellTexts[i]) { CellTexts[i]->SetColorAndOpacity(FSlateColor(i == HighlightColIndex ? HighlightColor : Col)); }
	}
}
