// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_TrackCard.h"
#include "WG_RadNodzToolkit.h"
#include "WG_MediaItemRow.h"
#include "ReaperFont.h"
#include "ReaperLang.h"
#include "ReaperChannelUtil.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Texture2D.h"
#include "Styling/CoreStyle.h"
#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Brushes/SlateColorBrush.h"

namespace ReaperUI
{
	// 共通カラーパレット（ダーク×ティール ─ iZotope調）
	static const FLinearColor CardBg        = FLinearColor(0.034f, 0.072f, 0.085f, 1.f);
	static const FLinearColor CardArmedBg   = FLinearColor(0.28f, 0.07f, 0.13f, 1.f);
	static const FLinearColor RecRed        = FLinearColor(0.93f, 0.27f, 0.38f, 1.f);
	// 未アーム時のRECボタン：周囲のカードに溶け込む「色のついていない」状態
	static const FLinearColor IdleBtn       = FLinearColor(0.070f, 0.130f, 0.150f, 1.f);
	// 親フォルダ行・親RECボタン用（子をまとめてREC＝ティール系で「個別REC」と区別）
	static const FLinearColor ParentRowBg   = FLinearColor(0.036f, 0.095f, 0.110f, 1.f);
	static const FLinearColor ParentBtn     = FLinearColor(0.10f, 0.50f, 0.52f, 1.f);
	static const FLinearColor TextPrimary   = FLinearColor(0.85f, 0.93f, 0.94f, 1.f);
	static const FLinearColor TextDim       = FLinearColor(0.40f, 0.58f, 0.61f, 1.f);

	static const FLinearColor FieldBg       = FLinearColor(0.046f, 0.100f, 0.118f, 1.f);
	static const FLinearColor FieldText     = FLinearColor(0.85f, 0.95f, 0.95f, 1.f);
	static const FLinearColor FieldOutline  = FLinearColor(0.14f, 0.40f, 0.42f, 1.f);
	static const FLinearColor NeonEdge      = FLinearColor(0.20f, 0.78f, 0.73f, 1.f);
	// M/S/R 識別カラー（OFF=暗背景に色文字 / ON=色背景）
	static const FLinearColor MuteCol       = FLinearColor(0.95f, 0.55f, 0.12f, 1.f);  // M = オレンジ
	static const FLinearColor SoloCol       = FLinearColor(0.97f, 0.82f, 0.15f, 1.f);  // S = イエロー
	// R = RecRed を使用
	static const FLinearColor BtnDarkTxt    = FLinearColor(0.05f, 0.04f, 0.00f, 1.f);  // 明色背景上の濃い文字

	// 入力ボックスを「暗い背景＋明るい文字＋角丸＋ティール枠線」に整える
	static void StyleDarkInput(UEditableTextBox* Box)
	{
		if (!Box) return;

		const float Radius = 10.f;
		FSlateRoundedBoxBrush Normal(FieldBg, Radius, FieldOutline, 1.2f);
		FSlateRoundedBoxBrush Focused(FieldBg, Radius, NeonEdge, 1.6f);
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
		Box->WidgetStyle.TextStyle.Font = ReaperFont::Get(20, false);
	}

	// ボタンを角丸（白ブラシ＋Tint）にする
	static void StyleRoundedButton(UButton* Btn, float Radius = 10.f)
	{
		if (!Btn) return;
		FButtonStyle Style = Btn->GetStyle();
		Style.Normal   = FSlateRoundedBoxBrush(FLinearColor::White, Radius);
		Style.Hovered  = FSlateRoundedBoxBrush(FLinearColor(1.12f, 1.12f, 1.12f, 1.f), Radius);
		Style.Pressed  = FSlateRoundedBoxBrush(FLinearColor(0.85f, 0.85f, 0.85f, 1.f), Radius);
		Btn->SetStyle(Style);
	}

	// プルダウン（暗い背景＋ティール枠）を作る
	static UComboBoxString* MakeCombo(UWidgetTree* Tree)
	{
		UComboBoxString* C = Tree->ConstructWidget<UComboBoxString>(UComboBoxString::StaticClass());
		// Font は構築直後のみ直接代入可（非推奨警告を抑制）
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		C->Font = ReaperFont::Get(16, false);
		FComboBoxStyle Style = C->WidgetStyle;
		const FSlateRoundedBoxBrush NormalBox(FieldBg, 8.f, FieldOutline, 1.2f);
		const FSlateRoundedBoxBrush HoverBox (FieldBg, 8.f, NeonEdge,    1.6f);
		Style.ComboButtonStyle.ButtonStyle.Normal   = NormalBox;
		Style.ComboButtonStyle.ButtonStyle.Hovered  = HoverBox;
		Style.ComboButtonStyle.ButtonStyle.Pressed  = HoverBox;
		Style.ComboButtonStyle.ButtonStyle.Disabled = NormalBox;
		C->WidgetStyle = Style;
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
		return C;
	}
}

TSharedRef<SWidget> UWG_TrackCard::RebuildWidget()
{
	if (WidgetTree && !WidgetTree->RootWidget)
	{
		BuildUI();
	}
	return Super::RebuildWidget();
}

void UWG_TrackCard::NativeConstruct()
{
	Super::NativeConstruct();

	if (ArmButton && !ArmButton->OnClicked.IsAlreadyBound(this, &UWG_TrackCard::HandleArmClicked))
	{
		ArmButton->OnClicked.AddDynamic(this, &UWG_TrackCard::HandleArmClicked);
	}
	if (CollapseButton && !CollapseButton->OnClicked.IsAlreadyBound(this, &UWG_TrackCard::HandleCollapseClicked))
	{
		CollapseButton->OnClicked.AddDynamic(this, &UWG_TrackCard::HandleCollapseClicked);
	}
	if (SoloButton && !SoloButton->OnClicked.IsAlreadyBound(this, &UWG_TrackCard::HandleSoloClicked))
	{
		SoloButton->OnClicked.AddDynamic(this, &UWG_TrackCard::HandleSoloClicked);
	}
	if (MuteButton && !MuteButton->OnClicked.IsAlreadyBound(this, &UWG_TrackCard::HandleMuteClicked))
	{
		MuteButton->OnClicked.AddDynamic(this, &UWG_TrackCard::HandleMuteClicked);
	}
	if (VolMinusButton && !VolMinusButton->OnClicked.IsAlreadyBound(this, &UWG_TrackCard::HandleVolMinusClicked))
	{
		VolMinusButton->OnClicked.AddDynamic(this, &UWG_TrackCard::HandleVolMinusClicked);
	}
	if (VolPlusButton && !VolPlusButton->OnClicked.IsAlreadyBound(this, &UWG_TrackCard::HandleVolPlusClicked))
	{
		VolPlusButton->OnClicked.AddDynamic(this, &UWG_TrackCard::HandleVolPlusClicked);
	}
	if (MediaToggleButton && !MediaToggleButton->OnClicked.IsAlreadyBound(this, &UWG_TrackCard::HandleMediaToggleClicked))
	{
		MediaToggleButton->OnClicked.AddDynamic(this, &UWG_TrackCard::HandleMediaToggleClicked);
	}
	if (NameInput && !NameInput->OnTextCommitted.IsAlreadyBound(this, &UWG_TrackCard::HandleNameCommitted))
	{
		NameInput->OnTextCommitted.AddDynamic(this, &UWG_TrackCard::HandleNameCommitted);
	}
	if (InputFormatCombo && !InputFormatCombo->OnSelectionChanged.IsAlreadyBound(this, &UWG_TrackCard::HandleInputSelectionChanged))
	{
		InputFormatCombo->OnSelectionChanged.AddDynamic(this, &UWG_TrackCard::HandleInputSelectionChanged);
	}
	if (InputChCombo && !InputChCombo->OnSelectionChanged.IsAlreadyBound(this, &UWG_TrackCard::HandleInputSelectionChanged))
	{
		InputChCombo->OnSelectionChanged.AddDynamic(this, &UWG_TrackCard::HandleInputSelectionChanged);
	}
	if (SelectCheckButton && !SelectCheckButton->OnClicked.IsAlreadyBound(this, &UWG_TrackCard::HandleSelectCheckClicked))
	{
		SelectCheckButton->OnClicked.AddDynamic(this, &UWG_TrackCard::HandleSelectCheckClicked);
	}
	RefreshVisuals();
	RefreshHierarchyVisuals();
	RefreshSoloVisual();
	RefreshMuteVisual();
	RefreshVolumeText();
	// 入力プルダウンの中身は展開時（HandleMediaToggleClicked）に取得する。
	// 接続直後に全カードで同期取得すると起動が重く、かつ入力名がまだ揃っていないため。
}

void UWG_TrackCard::BuildUI()
{
	using namespace ReaperUI;

	// ルート：カード背景ボーダー（角丸）
	RootBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("CardBorder"));
	RootBorder->SetBrush(FSlateRoundedBoxBrush(FLinearColor::White, 12.f)); // 角丸。色はSetBrushColorで付ける
	RootBorder->SetBrushColor(CardBg);
	RootBorder->SetPadding(FMargin(18.f, 13.f));
	WidgetTree->RootWidget = RootBorder;

	// カード内は縦並び：[メイン行] ＋ [メディア一覧（折りたたみ）]
	UVerticalBox* CardCol = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("CardCol"));
	RootBorder->SetContent(CardCol);

	UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("CardRow"));
	CardCol->AddChildToVerticalBox(Row);

	// 削除選択用チェックボックス（左端）。削除モード時のみ表示する。
	SelectCheckButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("SelectCheckButton"));
	StyleRoundedButton(SelectCheckButton);
	SelectCheckButton->SetBackgroundColor(IdleBtn);

	// ゴミ箱アイコンは常時表示し、選択時は背景色だけ変える
	SelectCheckIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("SelectCheckIcon"));
	if (UTexture2D* TrashTex = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/T_icon_trash_1.T_icon_trash_1")))
	{
		SelectCheckIcon->SetBrushFromTexture(TrashTex);
	}
	SelectCheckIcon->SetDesiredSizeOverride(FVector2D(34.f, 34.f));
	SelectCheckButton->SetContent(SelectCheckIcon);

	SelectCheckSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SelectCheckSize"));
	SelectCheckSize->SetMinDesiredWidth(64.f);
	SelectCheckSize->SetMinDesiredHeight(64.f);
	SelectCheckSize->AddChild(SelectCheckButton);
	SelectCheckSize->SetVisibility(ESlateVisibility::Collapsed);   // 既定は非表示（削除モードで表示）
	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(SelectCheckSize);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));
	}

	// 階層インデント用のスペーサー（レベルに応じて幅が変わる）
	IndentBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("IndentBox"));
	IndentBox->SetWidthOverride(0.f);
	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(IndentBox);
		S->SetVerticalAlignment(VAlign_Center);
	}

	// 親トラックの▽/▷（畳み込み）ボタン。子トラックでは非表示。
	CollapseButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("CollapseButton"));
	CollapseButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f)); // 透明（無着色）

	CollapseGlyph = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CollapseGlyph"));
	CollapseGlyph->SetText(FText::FromString(TEXT("▼")));
	CollapseGlyph->SetFont(ReaperFont::Get(20, true));
	CollapseGlyph->SetColorAndOpacity(FSlateColor(TextPrimary));
	CollapseGlyph->SetJustification(ETextJustify::Center);
	CollapseButton->SetContent(CollapseGlyph);
	{
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("CollapseBtnSize"));
		B->SetMinDesiredWidth(48.f);
		B->SetMinDesiredHeight(48.f);
		B->AddChild(CollapseButton);
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 6.f, 0.f));
	}

	// メディア展開トグル（▶/▼）。再生ボタンと紛らわしいので、階層展開（CollapseButton）と
	// 同じく左側のプレーンな矢印にする（角丸ボタン背景は付けない）。
	MediaToggleButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("MediaToggleButton"));
	MediaToggleButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f)); // 透明（無着色）

	MediaToggleGlyph = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("MediaToggleGlyph"));
	MediaToggleGlyph->SetText(FText::FromString(TEXT("▶")));
	MediaToggleGlyph->SetFont(ReaperFont::Get(20, true));
	MediaToggleGlyph->SetColorAndOpacity(FSlateColor(TextDim));
	MediaToggleGlyph->SetJustification(ETextJustify::Center);
	MediaToggleButton->SetContent(MediaToggleGlyph);
	{
		// タップ判定を矢印の見た目ぴったりより一回り広く取り、反応の悪さを解消する。
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("MediaToggleBtnSize"));
		B->SetMinDesiredWidth(56.f);
		B->SetMinDesiredHeight(56.f);
		B->AddChild(MediaToggleButton);
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 4.f, 0.f));
	}

	// トラック番号
	IndexText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("IndexText"));
	IndexText->SetFont(ReaperFont::Get(20, true));
	IndexText->SetColorAndOpacity(FSlateColor(TextDim));
	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(IndexText);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(4.f, 0.f, 16.f, 0.f));
	}

	// トラック名（編集可能）
	NameInput = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("NameInput"));
	NameInput->SetHintText(FText::FromString(ReaperLang::S(TEXT("トラック名…"), TEXT("Track name…"))));
	StyleDarkInput(NameInput);
	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(NameInput);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		S->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));
	}

	// ミュートボタン
	MuteButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("MuteButton"));
	StyleRoundedButton(MuteButton);
	MuteButton->SetBackgroundColor(IdleBtn);

	MuteButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("MuteButtonText"));
	MuteButtonText->SetText(FText::FromString(TEXT("M")));
	MuteButtonText->SetFont(ReaperFont::Get(20, true));
	MuteButtonText->SetColorAndOpacity(FSlateColor(TextDim));
	MuteButtonText->SetJustification(ETextJustify::Center);
	MuteButton->SetContent(MuteButtonText);
	{
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(78.f); B->SetMinDesiredHeight(64.f); B->AddChild(MuteButton);
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));
	}

	// ソロボタン
	SoloButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("SoloButton"));
	StyleRoundedButton(SoloButton);
	SoloButton->SetBackgroundColor(IdleBtn);

	SoloButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SoloButtonText"));
	SoloButtonText->SetText(FText::FromString(TEXT("S")));
	SoloButtonText->SetFont(ReaperFont::Get(20, true));
	SoloButtonText->SetColorAndOpacity(FSlateColor(TextDim));
	SoloButtonText->SetJustification(ETextJustify::Center);
	SoloButton->SetContent(SoloButtonText);
	{
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(78.f); B->SetMinDesiredHeight(64.f); B->AddChild(SoloButton);
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));
	}

	// （入力ch設定ボタンはメイン行から外し、展開パネルの音量行に移動）

	// アームボタン（角丸）
	ArmButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ArmButton"));
	StyleRoundedButton(ArmButton);
	ArmButton->SetBackgroundColor(IdleBtn);

	ArmButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ArmButtonText"));
	ArmButtonText->SetText(FText::FromString(TEXT("R")));
	ArmButtonText->SetFont(ReaperFont::Get(20, true));
	ArmButtonText->SetColorAndOpacity(FSlateColor(TextDim));
	ArmButtonText->SetJustification(ETextJustify::Center);
	ArmButton->SetContent(ArmButtonText);

	USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("ArmBtnSize"));
	BtnSize->SetMinDesiredWidth(78.f);
	BtnSize->SetMinDesiredHeight(64.f);
	BtnSize->AddChild(ArmButton);
	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(BtnSize);
		S->SetVerticalAlignment(VAlign_Center);
	}

	// 入力レベルメーター（アーム中のみ表示）。チャンネル数ぶんのメーターを縦に並べる入れ物だけ作る。
	// 実際の各chメーターは BuildMeter()（InitTrackCard でch数確定時）で生成する。
	MeterBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("MeterBox"));
	MeterBox->SetVisibility(ESlateVisibility::Collapsed);
	{
		UVerticalBoxSlot* S = CardCol->AddChildToVerticalBox(MeterBox);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(2.f, 8.f, 2.f, 0.f));
	}
	BuildMeter({ TEXT("1") }, false);   // 既定はモノ1本（Initで実際の入力ch・名前・モードに作り直す）

	// ▶展開で出る領域（既定は折りたたみ）：音量調整 ＋ 計測 ＋ メディア一覧
	ExpandPanel = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("ExpandPanel"));
	ExpandPanel->SetVisibility(ESlateVisibility::Collapsed);
	{
		UVerticalBoxSlot* S = CardCol->AddChildToVerticalBox(ExpandPanel);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(8.f, 10.f, 0.f, 2.f));
	}

	// --- 音量調整の行（計測は廃止し、ラウドネス計測＆音量調整はホームの一括ボタンで行う） ---
	UHorizontalBox* ToolRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ToolRow"));
	{
		UVerticalBoxSlot* S = ExpandPanel->AddChildToVerticalBox(ToolRow);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));
	}

	// 「音量」ラベル
	{
		UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(ReaperLang::S(TEXT("音量"), TEXT("Volume"))));
		T->SetFont(ReaperFont::Get(17, true));
		T->SetColorAndOpacity(FSlateColor(TextDim));
		UHorizontalBoxSlot* S = ToolRow->AddChildToHorizontalBox(T);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));
	}

	// 音量 −
	VolMinusButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("VolMinusButton"));
	StyleRoundedButton(VolMinusButton);
	VolMinusButton->SetBackgroundColor(IdleBtn);
	{
		UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(TEXT("－")));
		T->SetFont(ReaperFont::Get(22, true));
		T->SetColorAndOpacity(FSlateColor(NeonEdge));
		T->SetJustification(ETextJustify::Center);
		VolMinusButton->SetContent(T);
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(50.f); B->SetMinDesiredHeight(56.f); B->AddChild(VolMinusButton);
		UHorizontalBoxSlot* S = ToolRow->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 4.f, 0.f));
	}

	// 音量 dB表示
	VolDbText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("VolDbText"));
	VolDbText->SetText(FText::FromString(TEXT("0.0 dB")));
	VolDbText->SetFont(ReaperFont::Get(17, true));
	VolDbText->SetColorAndOpacity(FSlateColor(TextPrimary));
	VolDbText->SetJustification(ETextJustify::Center);
	{
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(72.f); B->AddChild(VolDbText);
		UHorizontalBoxSlot* S = ToolRow->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
	}

	// 音量 ＋
	VolPlusButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("VolPlusButton"));
	StyleRoundedButton(VolPlusButton);
	VolPlusButton->SetBackgroundColor(IdleBtn);
	{
		UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(TEXT("＋")));
		T->SetFont(ReaperFont::Get(22, true));
		T->SetColorAndOpacity(FSlateColor(NeonEdge));
		T->SetJustification(ETextJustify::Center);
		VolPlusButton->SetContent(T);
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(50.f); B->SetMinDesiredHeight(56.f); B->AddChild(VolPlusButton);
		UHorizontalBoxSlot* S = ToolRow->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 16.f, 0.f));
	}

	// 入力（構成ch数／入力ch開始）：音量の右隣・展開パネル内。
	// 旧「入力」ボタン＋ダイアログを廃止し、カード上で直接プルダウン選択→即Reaperへ反映する。
	{
		UTextBlock* L = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		L->SetText(FText::FromString(ReaperLang::S(TEXT("入力"), TEXT("Input"))));
		L->SetFont(ReaperFont::Get(17, true));
		L->SetColorAndOpacity(FSlateColor(TextDim));
		L->SetJustification(ETextJustify::Center);
		UHorizontalBoxSlot* S = ToolRow->AddChildToHorizontalBox(L);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
	}

	// 構成（モノ/ステレオ/5.1）
	InputFormatCombo = MakeCombo(WidgetTree);
	InputFormatCombo->OnGenerateWidgetEvent.BindDynamic(this, &UWG_TrackCard::MakeInputComboItem);
	{
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(120.f); B->SetMinDesiredHeight(56.f); B->AddChild(InputFormatCombo);
		UHorizontalBoxSlot* S = ToolRow->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
	}

	// 入力ch（開始）
	InputChCombo = MakeCombo(WidgetTree);
	InputChCombo->OnGenerateWidgetEvent.BindDynamic(this, &UWG_TrackCard::MakeInputComboItem);
	{
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(170.f); B->SetMinDesiredHeight(56.f); B->AddChild(InputChCombo);
		UHorizontalBoxSlot* S = ToolRow->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
	}

	// メディア一覧
	MediaListBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("MediaListBox"));
	{
		UVerticalBoxSlot* S = ExpandPanel->AddChildToVerticalBox(MediaListBox);
		S->SetHorizontalAlignment(HAlign_Fill);
	}

	ApplyName();
	RefreshVisuals();
	RefreshSoloVisual();
	RefreshMuteVisual();

	// 呼び出し元（ApplyTracksUI）がAddChildより前にSetMediaExpanded(true)を呼んでいた場合、
	// その時点ではExpandPanel/MediaListBoxがまだ存在せず何もできていない。
	// ここでウィジェット構築が完了したタイミングで、既にtrueになっているbMediaExpandedを反映し直す。
	if (bMediaExpanded)
	{
		SetMediaExpanded(true);
	}
}

void UWG_TrackCard::ApplyName()
{
	// 編集中(フォーカス中)は上書きしない
	if (NameInput && !NameInput->HasKeyboardFocus())
	{
		NameInput->SetText(FText::FromString(TrackName));
	}
}

void UWG_TrackCard::RefreshVisuals()
{
	using namespace ReaperUI;

	if (IndexText)
	{
		IndexText->SetText(FText::FromString(FString::Printf(TEXT("%02d"), TrackIndex + 1)));
	}

	if (bIsParent)
	{
		// 親フォルダ：自身はRECされない。ボタンは「子をまとめてREC」（ALL表記）。
		if (RootBorder)
		{
			RootBorder->SetBrushColor(ParentRowBg);
		}
		if (ArmButton)
		{
			// 子が全REC中なら赤点灯、それ以外は親用のティール（個別RECと区別）
			ArmButton->SetBackgroundColor(bChildrenArmed ? RecRed : ParentBtn);
		}
		if (ArmButtonText)
		{
			ArmButtonText->SetText(FText::FromString(TEXT("ALL")));
			ArmButtonText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		}
	}
	else
	{
		// R = レッド。ON(アーム中):赤背景＋白文字 / OFF:暗背景＋赤文字
		if (RootBorder)
		{
			RootBorder->SetBrushColor(bIsArmed ? CardArmedBg : CardBg);
		}
		if (ArmButton)
		{
			ArmButton->SetBackgroundColor(bIsArmed ? RecRed : IdleBtn);
		}
		if (ArmButtonText)
		{
			ArmButtonText->SetText(FText::FromString(TEXT("R")));
			ArmButtonText->SetColorAndOpacity(FSlateColor(bIsArmed ? FLinearColor::White : RecRed));
		}
	}

	// 入力メーターの表示/非表示を更新（アーム切替時はリセットも兼ねる）
	RefreshMeterVisibility();

	RefreshLockedButtonOpacity();   // ロック中でもON/OFFの濃淡が最新状態を反映するように
}

void UWG_TrackCard::RefreshMeterVisibility()
{
	if (!MeterBox) return;

	// 入力メーターはアーム中の葉トラック、かつ「メーター受信」がONのときだけ表示する。
	// OFF時は非表示にして、コントローラ側のポーリングも止める（処理を軽くする）。
	const bool bRecvOn = OwnerController && OwnerController->IsMeterReceiveEnabled();
	const bool bShowMeter = bIsArmed && !bIsParent && bRecvOn;
	MeterBox->SetVisibility(bShowMeter ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	ResetMeters();   // レベル・最大値線・クリップ・数値をリセット
}

void UWG_TrackCard::RefreshHierarchyVisuals()
{
	// インデント幅（レベル × 1段の幅）
	if (IndentBox)
	{
		IndentBox->SetWidthOverride(HierarchyLevel * 36.f);
	}

	// 親トラックのみ▽/▷ボタンを表示。子トラックでは場所だけ残して非表示。
	if (CollapseButton)
	{
		CollapseButton->SetVisibility(bIsParent ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (CollapseGlyph)
	{
		CollapseGlyph->SetText(FText::FromString(bIsCollapsed ? TEXT("▶") : TEXT("▼")));
	}
}

void UWG_TrackCard::SetHierarchy(int32 Level, bool bInParent, bool bCollapsed)
{
	HierarchyLevel = FMath::Max(0, Level);
	bIsParent = bInParent;
	bIsCollapsed = bCollapsed;
	RefreshHierarchyVisuals();
	// 親/子でRECボタンの見た目が変わるので更新
	RefreshVisuals();
}

void UWG_TrackCard::SetCollapsed(bool bCollapsed)
{
	bIsCollapsed = bCollapsed;
	RefreshHierarchyVisuals();
}

void UWG_TrackCard::SetParentChildrenArmed(bool bAllArmed)
{
	bChildrenArmed = bAllArmed;
	RefreshVisuals();
}

void UWG_TrackCard::SetDimmed(bool bDim)
{
	// カードを薄暗くするが、操作ボタン（M / S / R）は「押せる」と分かるよう常に明るく保つ。
	// （カード全体ではなく、ボタンを除いた各要素に個別に不透明度を当てる）
	const float Op = bDim ? 0.4f : 1.0f;

	if (CollapseButton)    CollapseButton->SetRenderOpacity(Op);
	if (IndexText)         IndexText->SetRenderOpacity(Op);
	if (NameInput)         NameInput->SetRenderOpacity(Op);
	if (MediaToggleButton) MediaToggleButton->SetRenderOpacity(Op);
	if (MediaListBox)      MediaListBox->SetRenderOpacity(Op);

	// M / S / R は通常は常に明るく（押下可能と分かるように）。ロック中は専用ロジックで管理する。
	RefreshLockedButtonOpacity();
}

void UWG_TrackCard::RefreshLockedButtonOpacity()
{
	if (!bIsLocked)
	{
		if (MuteButton) MuteButton->SetRenderOpacity(1.0f);
		if (SoloButton) SoloButton->SetRenderOpacity(1.0f);
		if (ArmButton)  ArmButton->SetRenderOpacity(1.0f);
		return;
	}

	// ロック中：押せないことは暗さで示しつつ、ON中のものは濃く／OFFは薄くして、
	// 今どの状態か（Reaper側で直接ON/OFFされた場合を含む）が一目で分かるようにする。
	const bool bArmOn = bIsParent ? bChildrenArmed : bIsArmed;
	if (MuteButton) MuteButton->SetRenderOpacity(bIsMuted ? 0.85f : 0.35f);
	if (SoloButton) SoloButton->SetRenderOpacity(bIsSoloed ? 0.85f : 0.35f);
	if (ArmButton)  ArmButton->SetRenderOpacity(bArmOn ? 0.85f : 0.35f);
}

void UWG_TrackCard::HandleCollapseClicked()
{
	if (!OwnerController || !bIsParent) return;
	OwnerController->ToggleCollapse(TrackIndex);
}

void UWG_TrackCard::RefreshInputControls()
{
	using namespace ReaperUI;
	if (!InputFormatCombo || !InputChCombo) return;

	// programmatic な選択で HandleInputSelectionChanged が走らないようにする
	bInitializingInput = true;

	// 現在の入力設定（開始ch・ch数）を取得（未接続時は 0 / 1）
	int32 First = 0;
	int32 Num   = 1;
	if (OwnerController && TrackObj)
	{
		OwnerController->GetTrackInputChannel(TrackObj, First, Num);
	}

	// 構成（モノ / ステレオ / 5.1）
	InputFormatCombo->ClearOptions();
	InputFormatCombo->AddOption(ReaperLang::S(TEXT("モノ"), TEXT("Mono")));
	InputFormatCombo->AddOption(ReaperLang::S(TEXT("ステレオ"), TEXT("Stereo")));
	InputFormatCombo->AddOption(TEXT("5.1"));
	InputFormatCombo->SetSelectedIndex(ReaperChannelUtil::NumChannelsToIndex(Num));

	// 入力ch（開始）：接続中は入力チャンネル名、未接続は番号(0〜31)
	const TArray<FString> Names = OwnerController ? OwnerController->GetInputChannelNames() : TArray<FString>();
	UE_LOG(LogTemp, Warning, TEXT("[ReaperInput] Card '%s' RefreshInputControls -> %d 件 (First=%d Num=%d)"),
		*TrackName, Names.Num(), First, Num);
	ReaperChannelUtil::FillInputChannelOptions(InputChCombo, Names, First);

	bInitializingInput = false;
}

void UWG_TrackCard::HandleInputSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	using namespace ReaperUI;
	if (bInitializingInput) return;             // 初期化中の選択は反映しない
	if (!OwnerController || !TrackObj) return;

	const int32 Num   = ReaperChannelUtil::IndexToNumChannels(InputFormatCombo ? InputFormatCombo->GetSelectedIndex() : 0);
	const int32 First = InputChCombo ? FMath::Max(0, InputChCombo->GetSelectedIndex()) : 0;

	// 選んだら即Reaperへ反映
	OwnerController->SetTrackInputChannel(TrackObj, First, Num);

	// 入力が変わったので、メーターのラベル/本数を作り直す
	SetMeterMode(bIndicatorMode);
}

UWidget* UWG_TrackCard::MakeInputComboItem(FString Item)
{
	using namespace ReaperUI;
	UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	T->SetText(FText::FromString(Item));
	T->SetFont(ReaperFont::Get(16, false));
	T->SetColorAndOpacity(FSlateColor(FieldText));   // 暗背景で読める明るい文字
	return T;
}

void UWG_TrackCard::HandleArmClicked()
{
	if (bIsLocked) return;

	if (bIsParent)
	{
		// 親フォルダ：自身はRECせず、配下の子トラックをまとめてREC ON/OFF
		if (OwnerController)
		{
			OwnerController->ToggleChildrenArmed(TrackIndex);
		}
		return;
	}
	ToggleArm();
}

void UWG_TrackCard::HandleNameCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// Enterキー、またはフォーカスを外した時に自動で確定する（更新ボタン不要）
	if (CommitMethod == ETextCommit::OnEnter || CommitMethod == ETextCommit::OnUserMovedFocus)
	{
		CommitNameEdit();
	}
}

void UWG_TrackCard::CommitNameEdit()
{
	if (!NameInput) return;

	const FString NewName = NameInput->GetText().ToString();
	if (NewName == TrackName)
	{
		return;
	}

	TrackName = NewName;
	if (OwnerController)
	{
		OwnerController->RenameTrack(TrackIndex, NewName);
	}
}

void UWG_TrackCard::InitTrackCard(const FTrackDetail& Detail, UWG_RadNodzToolkit* Controller)
{
	TrackIndex = Detail.Index;
	TrackName = Detail.Name;
	OwnerController = Controller;
	bIsArmed = Controller ? Controller->IsTrackArmed(TrackIndex) : false;
	bIsSoloed = Controller ? Controller->IsTrackSoloed(TrackIndex) : false;
	bIsMuted = Controller ? Controller->IsTrackMuted(TrackIndex) : false;
	TrackObj = Detail.Track;
	VolumeDb = Controller ? Controller->GetTrackVolume(TrackObj) : 0.0;

	// メーターを用意（横に入力名／信号〇）。本数・モードはコントローラ設定に従う。
	if (Controller)
	{
		BuildMeter(Controller->GetMeterLabelsForTrack(TrackObj), Controller->IsMeterIndicatorMode());
	}

	ApplyName();
	RefreshVisuals();
	RefreshSoloVisual();
	RefreshMuteVisual();
	RefreshVolumeText();
}

void UWG_TrackCard::RefreshSoloVisual()
{
	using namespace ReaperUI;
	// S = イエロー。ON:黄背景＋濃文字 / OFF:暗背景＋黄文字
	if (SoloButton)
	{
		SoloButton->SetBackgroundColor(bIsSoloed ? SoloCol : IdleBtn);
	}
	if (SoloButtonText)
	{
		SoloButtonText->SetColorAndOpacity(FSlateColor(bIsSoloed ? BtnDarkTxt : SoloCol));
	}
	RefreshLockedButtonOpacity();   // ロック中でもON/OFFの濃淡が最新状態を反映するように
}

void UWG_TrackCard::HandleSoloClicked()
{
	if (bIsLocked) return;
	if (!OwnerController) return;
	bIsSoloed = !bIsSoloed;
	OwnerController->SetTrackSolo(TrackIndex, bIsSoloed);
	RefreshSoloVisual();
}

void UWG_TrackCard::RefreshMuteVisual()
{
	using namespace ReaperUI;
	// M = オレンジ。ON:橙背景＋濃文字 / OFF:暗背景＋橙文字
	if (MuteButton)
	{
		MuteButton->SetBackgroundColor(bIsMuted ? MuteCol : IdleBtn);
	}
	if (MuteButtonText)
	{
		MuteButtonText->SetColorAndOpacity(FSlateColor(bIsMuted ? BtnDarkTxt : MuteCol));
	}
	RefreshLockedButtonOpacity();   // ロック中でもON/OFFの濃淡が最新状態を反映するように
}

void UWG_TrackCard::HandleMuteClicked()
{
	if (bIsLocked) return;
	if (!OwnerController) return;
	bIsMuted = !bIsMuted;
	OwnerController->SetTrackMute(TrackIndex, bIsMuted);
	RefreshMuteVisual();
}

void UWG_TrackCard::RefreshVolumeText()
{
	if (VolDbText)
	{
		VolDbText->SetText(FText::FromString(FString::Printf(TEXT("%.1f dB"), VolumeDb)));
	}
}

void UWG_TrackCard::HandleVolMinusClicked()
{
	if (!OwnerController) return;
	VolumeDb = OwnerController->AddTrackVolume(TrackObj, -1.0);
	RefreshVolumeText();
}

void UWG_TrackCard::HandleVolPlusClicked()
{
	if (!OwnerController) return;
	VolumeDb = OwnerController->AddTrackVolume(TrackObj, +1.0);
	RefreshVolumeText();
}

void UWG_TrackCard::HandleMediaToggleClicked()
{
	SetMediaExpanded(!bMediaExpanded);

	// トラック一覧の自動更新でカードが作り直されても展開状態を復元できるよう、親へ記録しておく
	if (OwnerController)
	{
		OwnerController->SetTrackMediaExpanded(TrackIndex, bMediaExpanded);
	}
}

void UWG_TrackCard::SetMediaExpanded(bool bExpand)
{
	bMediaExpanded = bExpand;

	if (MediaToggleGlyph)
	{
		MediaToggleGlyph->SetText(FText::FromString(bMediaExpanded ? TEXT("▼") : TEXT("▶")));
	}

	if (bMediaExpanded)
	{
		BuildMediaList();
		// 展開時に入力プルダウンを取り直す。接続直後は入力チャンネル名がまだ揃っておらず
		// 番号にフォールバックすることがあるため、開いた時点の最新の名前を反映する。
		RefreshInputControls();
		if (ExpandPanel) ExpandPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (ExpandPanel) ExpandPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWG_TrackCard::SetDeleteMode(bool bOn)
{
	// 削除モードに入る/出るたびに選択をリセットし、右端チェックボックスの表示を切り替える
	bSelectedForDelete = false;
	if (SelectCheckSize)
	{
		SelectCheckSize->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	RefreshSelectCheckVisual();
}

void UWG_TrackCard::SetSelectedForDelete(bool bSel)
{
	bSelectedForDelete = bSel;
	RefreshSelectCheckVisual();
}

void UWG_TrackCard::SetLocked(bool bLocked)
{
	bIsLocked = bLocked;

	// ロック中はミュート/ソロ/RECを無効化する。見た目（濃淡でON/OFFが分かる表現）は専用ロジックで管理する。
	if (MuteButton) MuteButton->SetIsEnabled(!bLocked);
	if (SoloButton) SoloButton->SetIsEnabled(!bLocked);
	if (ArmButton)  ArmButton->SetIsEnabled(!bLocked);
	RefreshLockedButtonOpacity();

	// 展開中のメディア一覧にも反映する（メディアの「再生」を無効化）
	if (MediaListBox)
	{
		for (UWidget* Child : MediaListBox->GetAllChildren())
		{
			if (UWG_MediaItemRow* Row = Cast<UWG_MediaItemRow>(Child))
			{
				Row->SetLocked(bLocked);
			}
		}
	}
}

void UWG_TrackCard::HandleSelectCheckClicked()
{
	bSelectedForDelete = !bSelectedForDelete;
	RefreshSelectCheckVisual();
}

void UWG_TrackCard::RefreshSelectCheckVisual()
{
	using namespace ReaperUI;
	// ゴミ箱アイコンは常時表示。選択中は背景を赤、未選択は通常色にして区別する。
	if (SelectCheckButton)
	{
		SelectCheckButton->SetBackgroundColor(bSelectedForDelete ? RecRed : IdleBtn);
	}
}

namespace
{
	// 信号「来てる」判定のしきい値（dB）。これより大きければ緑、以下なら赤。
	static const float MeterSignalThresholdDb = -50.f;
	static const FLinearColor DotGreen(0.16f, 0.85f, 0.45f, 1.f);
	static const FLinearColor DotRed  (0.92f, 0.26f, 0.28f, 1.f);
	static const FLinearColor DotOff  (0.20f, 0.22f, 0.24f, 1.f);
}

void UWG_TrackCard::BuildMeter(const TArray<FString>& ChannelLabels, bool bIndicator)
{
	if (!MeterBox) return;

	bIndicatorMode = bIndicator;
	const int32 N = FMath::Clamp(ChannelLabels.Num(), 1, 8);

	MeterBox->ClearChildren();
	ChFill.Reset();   ChHold.Reset();   ChClip.Reset();   ChDbText.Reset();   ChDot.Reset();
	ChLevel.Reset();  ChHoldLevel.Reset(); ChClipped.Reset();

	const int32 FontSz = (N >= 5) ? 12 : 15;

	for (int32 ch = 0; ch < N; ++ch)
	{
		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		// 入力名ラベル（両モード共通）
		{
			UTextBlock* Name = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Name->SetText(FText::FromString(ChannelLabels[ch]));
			Name->SetFont(ReaperFont::Get(FontSz, true));
			Name->SetColorAndOpacity(FSlateColor(ReaperUI::TextDim));
			Name->SetJustification(ETextJustify::Left);
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredWidth(150.f);
			B->SetMaxDesiredWidth(150.f);
			B->AddChild(Name);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		}

		if (bIndicator)
		{
			// 信号〇（緑=来てる / 赤=来てない）。低処理。
			const float DotSz = (N >= 5) ? 14.f : 18.f;
			UBorder* Dot = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
			Dot->SetBrush(FSlateRoundedBoxBrush(FLinearColor::White, DotSz * 0.5f, FVector2D(DotSz, DotSz)));
			Dot->SetBrushColor(DotRed);   // 初期＝赤（信号なし）
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetWidthOverride(DotSz);
			B->SetHeightOverride(DotSz);
			B->AddChild(Dot);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetVerticalAlignment(VAlign_Center);
			ChDot.Add(Dot);
		}
		else
		{
			// フルメーター：[ Canvas（背景/バー/最大値線/クリップ） ] [ 数値dB ]
			UCanvasPanel* Canvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());

			UBorder* Bg = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
			Bg->SetBrush(FSlateRoundedBoxBrush(FLinearColor::White, 3.f));
			Bg->SetBrushColor(FLinearColor(0.02f, 0.05f, 0.06f, 1.f));
			if (UCanvasPanelSlot* CS = Canvas->AddChildToCanvas(Bg)) { CS->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f)); CS->SetOffsets(FMargin(0.f)); }

			UBorder* Fill = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
			Fill->SetBrush(FSlateRoundedBoxBrush(FLinearColor::White, 3.f));
			Fill->SetBrushColor(FLinearColor(0.13f, 0.74f, 0.55f, 1.f));
			if (UCanvasPanelSlot* CS = Canvas->AddChildToCanvas(Fill)) { CS->SetAnchors(FAnchors(0.f, 0.f, 0.001f, 1.f)); CS->SetOffsets(FMargin(0.f)); }

			UBorder* Hold = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
			Hold->SetBrush(FSlateColorBrush(FLinearColor::White));
			Hold->SetBrushColor(FLinearColor(0.95f, 0.97f, 0.99f, 1.f));
			Hold->SetVisibility(ESlateVisibility::Collapsed);
			if (UCanvasPanelSlot* CS = Canvas->AddChildToCanvas(Hold)) { CS->SetAnchors(FAnchors(0.f, 0.f, 0.f, 1.f)); CS->SetOffsets(FMargin(0.f, 0.f, 3.f, 0.f)); }

			UBorder* Clip = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
			Clip->SetBrush(FSlateRoundedBoxBrush(FLinearColor::White, 2.f));
			Clip->SetBrushColor(FLinearColor(0.18f, 0.05f, 0.06f, 1.f));
			if (UCanvasPanelSlot* CS = Canvas->AddChildToCanvas(Clip)) { CS->SetAnchors(FAnchors(1.f, 0.f, 1.f, 1.f)); CS->SetOffsets(FMargin(-14.f, 0.f, 0.f, 0.f)); }

			const float BarH = (N >= 5) ? 9.f : 12.f;
			{
				USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				B->SetHeightOverride(BarH);
				B->AddChild(Canvas);
				UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
			}

			UTextBlock* Db = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Db->SetText(FText::FromString(TEXT("-99.0 dB")));
			Db->SetFont(ReaperFont::Get(FontSz, true));
			Db->SetColorAndOpacity(FSlateColor(ReaperUI::TextDim));
			Db->SetJustification(ETextJustify::Right);
			{
				USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				B->SetMinDesiredWidth(80.f);
				B->AddChild(Db);
				UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
				S->SetVerticalAlignment(VAlign_Center);
			}

			ChFill.Add(Fill); ChHold.Add(Hold); ChClip.Add(Clip); ChDbText.Add(Db);
			ChLevel.Add(0.f); ChHoldLevel.Add(0.f); ChClipped.Add(false);
		}

		UVerticalBoxSlot* RS = MeterBox->AddChildToVerticalBox(Row);
		RS->SetHorizontalAlignment(HAlign_Fill);
		RS->SetPadding(FMargin(0.f, (ch == 0) ? 0.f : 3.f, 0.f, 0.f));
	}
}

void UWG_TrackCard::SetMeterMode(bool bIndicator)
{
	// ラベル（入力名／信号）を再取得して、新しいモードでメーターを作り直す
	TArray<FString> Labels;
	if (OwnerController && TrackObj)
	{
		Labels = OwnerController->GetMeterLabelsForTrack(TrackObj);
	}
	if (Labels.Num() == 0) { Labels.Add(TEXT("1")); }
	BuildMeter(Labels, bIndicator);
}

void UWG_TrackCard::ResetMeters()
{
	if (bIndicatorMode)
	{
		for (UBorder* Dot : ChDot) { if (Dot) { Dot->SetBrushColor(DotRed); } }
		return;
	}
	for (int32 ch = 0; ch < ChFill.Num(); ++ch)
	{
		ChLevel[ch] = 0.f; ChHoldLevel[ch] = 0.f; ChClipped[ch] = false;
		if (ChFill[ch])
		{
			if (UCanvasPanelSlot* CS = Cast<UCanvasPanelSlot>(ChFill[ch]->Slot)) { CS->SetAnchors(FAnchors(0.f, 0.f, 0.001f, 1.f)); }
		}
		if (ChHold[ch])   { ChHold[ch]->SetVisibility(ESlateVisibility::Collapsed); }
		if (ChClip[ch])   { ChClip[ch]->SetBrushColor(FLinearColor(0.18f, 0.05f, 0.06f, 1.f)); }
		if (ChDbText[ch]) { ChDbText[ch]->SetText(FText::FromString(TEXT("-99.0 dB"))); }
	}
}

void UWG_TrackCard::SetMeterChannels(const TArray<float>& CurDbPerCh)
{
	// 信号〇モード：しきい値で緑/赤を切り替えるだけ（低処理）
	if (bIndicatorMode)
	{
		const int32 N = FMath::Min(ChDot.Num(), CurDbPerCh.Num());
		for (int32 ch = 0; ch < N; ++ch)
		{
			if (ChDot[ch])
			{
				ChDot[ch]->SetBrushColor(CurDbPerCh[ch] > MeterSignalThresholdDb ? DotGreen : DotRed);
			}
		}
		return;
	}

	auto Norm = [](float Db) { return FMath::Clamp((Db + 60.f) / 60.f, 0.f, 1.f); };   // -60..0dB → 0..1

	const int32 N = FMath::Min(ChFill.Num(), CurDbPerCh.Num());
	for (int32 ch = 0; ch < N; ++ch)
	{
		const float CurDb = CurDbPerCh[ch];

		// 現在レベル（アタック速め・リリース遅めでスムージング）
		const float Target = Norm(CurDb);
		const float Rate = (Target > ChLevel[ch]) ? 0.6f : 0.25f;
		ChLevel[ch] = FMath::Lerp(ChLevel[ch], Target, Rate);

		if (ChFill[ch])
		{
			if (UCanvasPanelSlot* CS = Cast<UCanvasPanelSlot>(ChFill[ch]->Slot))
			{
				CS->SetAnchors(FAnchors(0.f, 0.f, FMath::Max(0.001f, ChLevel[ch]), 1.f));
				CS->SetOffsets(FMargin(0.f));
			}
			FLinearColor Col(0.13f, 0.74f, 0.55f, 1.f);
			if (CurDb > -3.f)       Col = FLinearColor(0.93f, 0.27f, 0.30f, 1.f);
			else if (CurDb > -12.f) Col = FLinearColor(0.97f, 0.82f, 0.15f, 1.f);
			ChFill[ch]->SetBrushColor(Col);
		}

		// 最大値の線（走行最大）
		ChHoldLevel[ch] = FMath::Max(ChHoldLevel[ch] * 0.999f, Target);
		if (ChHold[ch])
		{
			if (ChHoldLevel[ch] > 0.005f)
			{
				ChHold[ch]->SetVisibility(ESlateVisibility::HitTestInvisible);
				if (UCanvasPanelSlot* CS = Cast<UCanvasPanelSlot>(ChHold[ch]->Slot))
				{
					CS->SetAnchors(FAnchors(ChHoldLevel[ch], 0.f, ChHoldLevel[ch], 1.f));
					CS->SetOffsets(FMargin(-1.5f, 0.f, 3.f, 0.f));
				}
			}
			else { ChHold[ch]->SetVisibility(ESlateVisibility::Collapsed); }
		}

		// クリップ：0dB近傍でラッチ
		if (!ChClipped[ch] && CurDb >= -0.1f) { ChClipped[ch] = true; }
		if (ChClip[ch])
		{
			ChClip[ch]->SetBrushColor(ChClipped[ch]
				? FLinearColor(0.96f, 0.20f, 0.22f, 1.f)
				: FLinearColor(0.18f, 0.05f, 0.06f, 1.f));
		}

		// 数値（下限 -99dB）
		if (ChDbText[ch])
		{
			const float Shown = FMath::Max(CurDb, -99.f);
			ChDbText[ch]->SetText(FText::FromString(FString::Printf(TEXT("%.1f dB"), Shown)));
			FLinearColor TxtCol(0.55f, 0.72f, 0.74f, 1.f);
			if (CurDb > -3.f)       TxtCol = FLinearColor(0.96f, 0.40f, 0.40f, 1.f);
			else if (CurDb > -12.f) TxtCol = FLinearColor(0.97f, 0.82f, 0.15f, 1.f);
			ChDbText[ch]->SetColorAndOpacity(FSlateColor(TxtCol));
		}
	}
}

void UWG_TrackCard::RefreshVisibleMediaLevels()
{
	if (!MediaListBox) return;
	for (UWidget* Child : MediaListBox->GetAllChildren())
	{
		if (UWG_MediaItemRow* Row = Cast<UWG_MediaItemRow>(Child))
		{
			Row->RefreshLevelsFromCache();
		}
	}
}

void UWG_TrackCard::RebuildMediaListIfExpanded()
{
	if (bMediaExpanded)
	{
		BuildMediaList();
	}
}

void UWG_TrackCard::BuildMediaList()
{
	using namespace ReaperUI;
	if (!MediaListBox || !OwnerController) return;

	MediaListBox->ClearChildren();

	TArray<UMediaItem*> Items;
	TArray<FString> Names;
	TArray<double> Starts;
	// キャッシュ経由で取得（接続/更新時に先読み済みなら通信しない。展開のたびの重い再取得を防ぐ）
	OwnerController->GetTrackMediaItemsCached(TrackIndex, Items, Names, Starts);

	if (Items.Num() == 0)
	{
		UTextBlock* Empty = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Empty->SetText(FText::FromString(ReaperLang::S(TEXT("メディアなし"), TEXT("No media"))));
		Empty->SetFont(ReaperFont::Get(16, false));
		Empty->SetColorAndOpacity(FSlateColor(TextDim));
		UVerticalBoxSlot* S = MediaListBox->AddChildToVerticalBox(Empty);
		S->SetPadding(FMargin(4.f, 2.f, 0.f, 2.f));
		return;
	}

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		UWG_MediaItemRow* RowWidget = CreateWidget<UWG_MediaItemRow>(this, UWG_MediaItemRow::StaticClass());
		if (!RowWidget) continue;

		RowWidget->InitRow(Items[i], Names[i], Starts[i], OwnerController);
		RowWidget->SetLocked(bIsLocked);   // 作り直したメディア行にも現在のロック状態を反映する

		UVerticalBoxSlot* S = MediaListBox->AddChildToVerticalBox(RowWidget);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
	}
}

void UWG_TrackCard::ToggleArm()
{
	if (!OwnerController) return;

	bIsArmed = !bIsArmed;
	OwnerController->SetTrackArmed(TrackIndex, bIsArmed);
	OnArmStateChanged(bIsArmed);
}

void UWG_TrackCard::RefreshArmState(bool bArmed)
{
	bIsArmed = bArmed;
	OnArmStateChanged(bArmed);
}

void UWG_TrackCard::OnArmStateChanged_Implementation(bool bArmed)
{
	bIsArmed = bArmed;
	RefreshVisuals();
}
