// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "ReaperFont.h"
#include "ReaperLang.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Brushes/SlateRoundedBoxBrush.h"

/**
 * ダイアログ共通の配色と部品（ダーク×ティールの「サイバー」系）。
 *
 * 同じ配色・同じ組み方のコピーが各ダイアログの .cpp に散らばっていたので、
 * ここに1つだけ置いて共有する。見た目を変えるときはここを直せば全部に効く。
 * （WG_AddTracksDialog / WG_DeleteConfirmDialog / WG_MarkerNameDialog などの
 *   先行実装と同じ値。新しいダイアログはこのヘッダを使うこと）
 */
namespace ReaperDialogStyle
{
	// --- 配色 -------------------------------------------------------
	inline const FLinearColor DimBg       = FLinearColor(0.0f, 0.01f, 0.015f, 0.82f);   // 画面全体を覆う暗幕
	inline const FLinearColor PanelBg     = FLinearColor(0.036f, 0.080f, 0.094f, 1.f);
	inline const FLinearColor HeaderBg    = FLinearColor(0.050f, 0.105f, 0.122f, 1.f);
	inline const FLinearColor SunkenBg    = FLinearColor(0.020f, 0.045f, 0.052f, 1.f);  // 一覧・本文など一段沈んだ面

	inline const FLinearColor AccentGreen = FLinearColor(0.16f, 0.74f, 0.58f, 1.f);     // 肯定（OK / 登録）
	inline const FLinearColor AccentBlue  = FLinearColor(0.16f, 0.80f, 0.76f, 1.f);     // 補助操作（QR撮影など）
	inline const FLinearColor StopGray    = FLinearColor(0.085f, 0.150f, 0.170f, 1.f);  // 否定（キャンセル / いいえ）
	inline const FLinearColor RecRed      = FLinearColor(0.85f, 0.16f, 0.22f, 1.f);     // 破壊的操作
	inline const FLinearColor ErrorRed    = FLinearColor(0.96f, 0.30f, 0.32f, 1.f);     // エラー文言

	inline const FLinearColor TextPrimary = FLinearColor(0.85f, 0.93f, 0.94f, 1.f);
	inline const FLinearColor TextDim     = FLinearColor(0.40f, 0.58f, 0.61f, 1.f);

	inline const FLinearColor FieldBg     = FLinearColor(0.046f, 0.100f, 0.118f, 1.f);
	inline const FLinearColor FieldText   = FLinearColor(0.85f, 0.95f, 0.95f, 1.f);
	inline const FLinearColor FieldOutline= FLinearColor(0.14f, 0.40f, 0.42f, 1.f);
	inline const FLinearColor NeonEdge    = FLinearColor(0.20f, 0.78f, 0.73f, 1.f);     // フォーカス時の発光ふち

	// --- 部品 -------------------------------------------------------

	/** 共通フォントの TextBlock。 */
	inline UTextBlock* MakeText(UWidgetTree* Tree, const FString& Str, int32 Size, FLinearColor Color, bool bBold = false)
	{
		UTextBlock* T = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(Str));
		T->SetFont(ReaperFont::Get(Size, bBold));
		T->SetColorAndOpacity(FSlateColor(Color));
		return T;
	}

	/**
	 * 角丸のベタ塗りボタン。
	 * ブラシは白で作り、色は SetBackgroundColor で乗算する（ホバー/押下の明暗を効かせるため）。
	 */
	inline UButton* MakeButton(UWidgetTree* Tree, const FString& Label, FLinearColor BgColor, FName Name, int32 FontSize = 20)
	{
		UButton* Btn = Tree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
		FButtonStyle Style = Btn->GetStyle();
		Style.Normal  = FSlateRoundedBoxBrush(FLinearColor::White, 10.f);
		Style.Hovered = FSlateRoundedBoxBrush(FLinearColor(1.12f, 1.12f, 1.12f, 1.f), 10.f);
		Style.Pressed = FSlateRoundedBoxBrush(FLinearColor(0.85f, 0.85f, 0.85f, 1.f), 10.f);
		// 無効時はエンジン既定の灰色ボックスにせず、同じ角丸のまま沈めて見せる
		Style.Disabled = FSlateRoundedBoxBrush(FLinearColor(0.30f, 0.30f, 0.30f, 1.f), 10.f);
		Btn->SetStyle(Style);
		Btn->SetBackgroundColor(BgColor);
		UTextBlock* Txt = MakeText(Tree, Label, FontSize, FLinearColor::White, true);
		Txt->SetJustification(ETextJustify::Center);
		Txt->SetAutoWrapText(!ReaperLang::IsJapanese());   // 日本語は折り返し不要。長い翻訳文だけ2行に折り返す
		Btn->SetContent(Txt);
		return Btn;
	}

	/** 入力欄を暗い面＋ティールのふちにする（フォーカス時だけ発光させる）。 */
	inline void StyleDarkInput(UEditableTextBox* Box, int32 FontSize = 20)
	{
		if (!Box) { return; }

		const float Radius = 10.f;
		const FSlateRoundedBoxBrush Normal(FieldBg, Radius, FieldOutline, 1.2f);
		const FSlateRoundedBoxBrush Focused(FieldBg, Radius, NeonEdge, 1.6f);
		Box->WidgetStyle.SetBackgroundImageNormal(Normal);
		Box->WidgetStyle.SetBackgroundImageHovered(Normal);
		Box->WidgetStyle.SetBackgroundImageFocused(Focused);
		Box->WidgetStyle.SetBackgroundImageReadOnly(Normal);
		Box->WidgetStyle.SetBackgroundColor(FSlateColor(FieldBg));
		Box->WidgetStyle.SetForegroundColor(FSlateColor(FieldText));
		Box->WidgetStyle.FocusedForegroundColor = FSlateColor(FieldText);
		Box->WidgetStyle.ReadOnlyForegroundColor = FSlateColor(FieldText);
		Box->WidgetStyle.Padding = FMargin(16.f, 13.f);
		Box->WidgetStyle.TextStyle.ColorAndOpacity = FSlateColor(FieldText);
		Box->WidgetStyle.TextStyle.Font = ReaperFont::Get(FontSize, false);
	}

	/**
	 * ダイアログの骨組み（暗幕 → 中央パネル → 見出し帯 → 本体）を組んで、
	 * 中身を積む本体の VerticalBox を返す。Widget->WidgetTree->RootWidget もここで設定する。
	 *
	 * 見出しを後から差し替えるダイアログは OutTitleText / OutHeader を受け取る。
	 * TitleStr が空の場合、受け取る相手がいなければ見出し帯を作らず、
	 * 受け取る相手がいれば畳んだ状態で作る（後から SetText + Visible で出せる）。
	 */
	inline UVerticalBox* BuildFrame(UUserWidget* Widget, const FString& TitleStr, float PanelWidth = 720.f,
		UTextBlock** OutTitleText = nullptr, UBorder** OutHeader = nullptr)
	{
		UWidgetTree* Tree = Widget->WidgetTree;

		// 画面全体を覆う暗幕（背面のクリックを通さない）
		UBorder* Dim = Tree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Dim"));
		Dim->SetBrushColor(DimBg);
		Dim->SetHorizontalAlignment(HAlign_Center);
		Dim->SetVerticalAlignment(VAlign_Center);
		Tree->RootWidget = Dim;

		// 中央のパネル
		USizeBox* PanelSize = Tree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("PanelSize"));
		PanelSize->SetWidthOverride(PanelWidth);
		Dim->SetContent(PanelSize);

		UBorder* Panel = Tree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Panel"));
		Panel->SetBrushColor(PanelBg);
		Panel->SetPadding(FMargin(0.f));
		PanelSize->AddChild(Panel);

		UVerticalBox* Col = Tree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Col"));
		Panel->SetContent(Col);

		// 見出し帯
		if (!TitleStr.IsEmpty() || OutTitleText || OutHeader)
		{
			UBorder* Head = Tree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Head"));
			Head->SetBrushColor(HeaderBg);
			Head->SetPadding(FMargin(28.f, 20.f));
			UTextBlock* HeadText = MakeText(Tree, TitleStr, 26, TextPrimary, true);
			Head->SetContent(HeadText);
			Head->SetVisibility(TitleStr.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

			UVerticalBoxSlot* S = Col->AddChildToVerticalBox(Head);
			S->SetHorizontalAlignment(HAlign_Fill);

			if (OutTitleText) { *OutTitleText = HeadText; }
			if (OutHeader)    { *OutHeader = Head; }
		}

		// 本体（余白付き）
		UBorder* Body = Tree->ConstructWidget<UBorder>(UBorder::StaticClass());
		Body->SetBrushColor(PanelBg);
		Body->SetPadding(FMargin(28.f, 22.f));
		{
			UVerticalBoxSlot* S = Col->AddChildToVerticalBox(Body);
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		UVerticalBox* BodyCol = Tree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("BodyCol"));
		Body->SetContent(BodyCol);
		return BodyCol;
	}
}
