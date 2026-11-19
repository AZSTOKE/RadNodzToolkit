// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).


#include "WG_MarkerRow.h"
#include "WG_RadNodzToolkit.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Styling/SlateTypes.h"

#include "ReaperFont.h"
#include "ReaperLang.h"

namespace
{
	// メディア行(MediaRowUI)と同じ配色に合わせる。
	static const FLinearColor MrRowBg    = FLinearColor(0.020f, 0.040f, 0.048f, 1.f);   // 通常の行背景
	static const FLinearColor MrActiveBg = FLinearColor(0.060f, 0.140f, 0.110f, 1.f);   // 選択（カーソル位置）行
	static const FLinearColor MrEdge     = FLinearColor(0.14f, 0.40f, 0.42f, 1.f);      // 行/フィールドのふちどり
	static const FLinearColor MrTeal     = FLinearColor(0.16f, 0.78f, 0.72f, 1.f);      // 再生ボタン(停止中)
	static const FLinearColor MrPlaying  = FLinearColor(0.16f, 0.22f, 0.25f, 1.f);      // 再生中ボタン(グレー)
	static const FLinearColor MrFieldBg  = FLinearColor(0.040f, 0.072f, 0.085f, 1.f);   // 入力欄の背景（濃色）
	static const FLinearColor MrFieldText= FLinearColor(0.86f, 0.95f, 0.95f, 1.f);      // 入力欄の文字（明色）
	static const FLinearColor MrText     = FLinearColor(0.90f, 0.96f, 0.97f, 1.f);
	static const FLinearColor MrDim      = FLinearColor(0.55f, 0.72f, 0.74f, 1.f);
	static const FLinearColor MrDark     = FLinearColor(0.03f, 0.06f, 0.06f, 1.f);
}

void UWG_MarkerRow::Setup(UWG_RadNodzToolkit* InOwner, int32 InRowIndex, int32 InMarkerId,
	const FString& InName, double InStartTime, const FString& InTimeLabel)
{
	Owner = InOwner;
	RowIndex = InRowIndex;
	MarkerId = InMarkerId;
	MarkerName = InName;
	StartTime = InStartTime;
	TimeLabel = InTimeLabel;

	if (NameText) { NameText->SetText(FText::FromString(MarkerName)); }
	if (TimeText) { TimeText->SetText(FText::FromString(TimeLabel)); }
	RefreshVisual();
}

void UWG_MarkerRow::SetName(const FString& NewName)
{
	MarkerName = NewName;
	if (NameText) { NameText->SetText(FText::FromString(MarkerName)); }
}

TSharedRef<SWidget> UWG_MarkerRow::RebuildWidget()
{
	if (!bUICreated && WidgetTree)
	{
		bUICreated = true;

		RootBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("MarkerRootBorder"));
		RootBorder->SetBrush(FSlateRoundedBoxBrush(MrRowBg, 10.f, MrEdge, 1.1f));
		RootBorder->SetPadding(FMargin(18.f, 6.f, 12.f, 6.f));   // 左（時刻の頭）に余白を持たせる
		WidgetTree->RootWidget = RootBorder;

		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		RootBorder->SetContent(Row);

		// 先頭：時刻（秒数）。幅を固定して各行の再生ボタン位置を揃える。
		TimeText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		TimeText->SetText(FText::FromString(TimeLabel));
		TimeText->SetFont(ReaperFont::Get(16, true));
		TimeText->SetColorAndOpacity(FSlateColor(MrDim));
		TimeText->SetJustification(ETextJustify::Center);
		TimeText->SetAutoWrapText(false);
		{
			USizeBox* TB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			TB->SetMinDesiredWidth(78.f);
			TB->AddChild(TimeText);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(TB);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 18.f, 0.f));
		}

		// 時刻の次：再生/停止トグル
		PlayButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("MarkerPlayBtn"));
		{
			FButtonStyle Style = PlayButton->GetStyle();
			Style.Normal   = FSlateRoundedBoxBrush(MrTeal, 9.f);
			Style.Hovered  = FSlateRoundedBoxBrush(FLinearColor(0.20f, 0.90f, 0.83f, 1.f), 9.f);
			Style.Pressed  = FSlateRoundedBoxBrush(FLinearColor(0.13f, 0.66f, 0.61f, 1.f), 9.f);
			Style.NormalPadding = FMargin(0.f);
			Style.PressedPadding = FMargin(0.f);
			PlayButton->SetStyle(Style);

			PlayGlyph = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			PlayGlyph->SetText(FText::FromString(TEXT("▶")));
			PlayGlyph->SetFont(ReaperFont::Get(24, true));
			PlayGlyph->SetColorAndOpacity(FSlateColor(MrDark));
			PlayGlyph->SetJustification(ETextJustify::Center);
			PlayButton->AddChild(PlayGlyph);

			// 押しやすいよう大きめの当たり判定にする。
			USizeBox* PB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			PB->SetWidthOverride(76.f);
			PB->SetHeightOverride(60.f);
			PB->AddChild(PlayButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(PB);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 18.f, 0.f));
		}

		// 再生の隣：編集ボタン（タップで名前編集ダイアログを開く）。
		EditButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("MarkerEditBtn"));
		{
			FButtonStyle Style = EditButton->GetStyle();
			Style.Normal   = FSlateRoundedBoxBrush(MrFieldBg, 9.f, MrEdge, 1.1f);
			Style.Hovered  = FSlateRoundedBoxBrush(MrFieldBg, 9.f, MrTeal, 1.4f);
			Style.Pressed  = FSlateRoundedBoxBrush(FLinearColor(MrFieldBg.R * 1.2f, MrFieldBg.G * 1.2f, MrFieldBg.B * 1.2f, 1.f), 9.f, MrTeal, 1.4f);
			Style.NormalPadding = FMargin(0.f);
			Style.PressedPadding = FMargin(0.f);
			EditButton->SetStyle(Style);

			EditGlyph = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			EditGlyph->SetText(FText::FromString(ReaperLang::S(TEXT("編集"), TEXT("Edit"))));
			EditGlyph->SetFont(ReaperFont::Get(16, true));
			EditGlyph->SetColorAndOpacity(FSlateColor(MrFieldText));
			EditGlyph->SetJustification(ETextJustify::Center);
			EditGlyph->SetAutoWrapText(false);
			EditButton->AddChild(EditGlyph);

			USizeBox* EB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			EB->SetWidthOverride(84.f);
			EB->SetHeightOverride(60.f);
			EB->AddChild(EditButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(EB);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 18.f, 0.f));
		}

		// 中央：マーカー名（表示のみ。タップ不可なのでスワイプはそのまま一覧スクロールに通る）。
		NameText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		NameText->SetText(FText::FromString(MarkerName));
		NameText->SetFont(ReaperFont::Get(18, false));
		NameText->SetColorAndOpacity(FSlateColor(MrText));
		NameText->SetJustification(ETextJustify::Left);
		NameText->SetAutoWrapText(false);
		{
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(NameText);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 4.f, 0.f));
		}
	}
	return Super::RebuildWidget();
}

void UWG_MarkerRow::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayButton && !PlayButton->OnClicked.IsAlreadyBound(this, &UWG_MarkerRow::HandlePlayClicked))
	{
		PlayButton->OnClicked.AddDynamic(this, &UWG_MarkerRow::HandlePlayClicked);
	}
	if (EditButton && !EditButton->OnClicked.IsAlreadyBound(this, &UWG_MarkerRow::HandleEditClicked))
	{
		EditButton->OnClicked.AddDynamic(this, &UWG_MarkerRow::HandleEditClicked);
	}
}

void UWG_MarkerRow::RefreshVisual()
{
	if (RootBorder)
	{
		RootBorder->SetBrush(FSlateRoundedBoxBrush(bActive ? MrActiveBg : MrRowBg, 10.f, MrEdge, 1.1f));
	}
	if (PlayGlyph)
	{
		PlayGlyph->SetText(FText::FromString(bPlaying ? TEXT("■") : TEXT("▶")));
		PlayGlyph->SetColorAndOpacity(FSlateColor(bPlaying ? MrText : MrDark));
	}
	if (PlayButton)
	{
		FButtonStyle Style = PlayButton->GetStyle();
		const FLinearColor Bg = bPlaying ? MrPlaying : MrTeal;
		Style.Normal  = FSlateRoundedBoxBrush(Bg, 9.f);
		Style.Hovered = FSlateRoundedBoxBrush(FLinearColor(Bg.R * 1.15f + 0.02f, Bg.G * 1.15f + 0.02f, Bg.B * 1.15f + 0.02f, 1.f), 9.f);
		Style.Pressed = FSlateRoundedBoxBrush(FLinearColor(Bg.R * 0.85f, Bg.G * 0.85f, Bg.B * 0.85f, 1.f), 9.f);
		Style.NormalPadding = FMargin(0.f);
		Style.PressedPadding = FMargin(0.f);
		PlayButton->SetStyle(Style);
	}
}

void UWG_MarkerRow::SetPlaying(bool bInPlaying)
{
	bPlaying = bInPlaying;
	RefreshVisual();
}

void UWG_MarkerRow::SetActive(bool bInActive)
{
	bActive = bInActive;
	RefreshVisual();
}

void UWG_MarkerRow::HandlePlayClicked()
{
	if (!Owner) { return; }
	if (bPlaying)
	{
		Owner->StopMarkerPlayback();
	}
	else
	{
		Owner->PlayFromMarkerRow(this, StartTime);
	}
}

void UWG_MarkerRow::HandleEditClicked()
{
	// 「編集」ボタンで名前編集ダイアログを開く。
	if (Owner)
	{
		Owner->OpenMarkerRename(MarkerId, MarkerName);
	}
}
