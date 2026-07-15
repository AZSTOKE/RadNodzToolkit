// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_MicSettingsDialog.h"
#include "WG_RadNodzToolkit.h"
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
#include "Styling/CoreStyle.h"
#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateRoundedBoxBrush.h"

namespace MicDlgUI
{
	// ダーク×ティール（iZotope調）
	static const FLinearColor DimBg       = FLinearColor(0.0f, 0.01f, 0.015f, 0.80f);
	static const FLinearColor PanelBg     = FLinearColor(0.036f, 0.080f, 0.094f, 1.f);
	static const FLinearColor HeaderBg    = FLinearColor(0.050f, 0.105f, 0.122f, 1.f);

	static const FLinearColor AccentGreen = FLinearColor(0.16f, 0.74f, 0.58f, 1.f);
	static const FLinearColor AccentBlue  = FLinearColor(0.20f, 0.78f, 0.73f, 1.f);
	static const FLinearColor StopGray    = FLinearColor(0.085f, 0.150f, 0.170f, 1.f);

	static const FLinearColor TextPrimary = FLinearColor(0.85f, 0.93f, 0.94f, 1.f);
	static const FLinearColor TextDim     = FLinearColor(0.40f, 0.58f, 0.61f, 1.f);

	static const FLinearColor FieldBg     = FLinearColor(0.046f, 0.100f, 0.118f, 1.f);
	static const FLinearColor FieldText   = FLinearColor(0.85f, 0.95f, 0.95f, 1.f);
	static const FLinearColor FieldOutline= FLinearColor(0.14f, 0.40f, 0.42f, 1.f);
	static const FLinearColor NeonEdge    = FLinearColor(0.20f, 0.78f, 0.73f, 1.f);

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
		UTextBlock* Txt = MakeText(Tree, Label, 18, FLinearColor::White, true);
		Txt->SetJustification(ETextJustify::Center);
		Txt->SetAutoWrapText(!ReaperLang::IsJapanese());   // 日本語は折り返し不要。長い翻訳文字列だけボタン幅をはみ出さず2行に折り返す
		Btn->SetContent(Txt);
		return Btn;
	}

	static UComboBoxString* MakeCombo(UWidgetTree* Tree)
	{
		UComboBoxString* C = Tree->ConstructWidget<UComboBoxString>(UComboBoxString::StaticClass());
		// Font は構築時のみ設定可（直接代入は非推奨だが構築直後なので可）。警告を抑制。
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		C->Font = ReaperFont::Get(16, false);   // 日本語ラベル用にM PLUSフォント
		PRAGMA_ENABLE_DEPRECATION_WARNINGS

		// 閉じた状態のボックスを濃い背景＋ティール枠に（既定の明るい灰色は暗テーマで浮くため）
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
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
}

void UWG_MicSettingsDialog::Setup(UWG_RadNodzToolkit* InOwner)
{
	Owner = InOwner;
	if (Owner)
	{
		InitialMics = Owner->GetMics();
		InputChannelNames = Owner->GetInputChannelNames();   // 接続中のみ中身あり
		Owner->GetMicCatalog(CatalogNames, CatalogChannels); // リストの「マイク」シート由来（未読込なら空）
	}
}

TSharedRef<SWidget> UWG_MicSettingsDialog::RebuildWidget()
{
	if (WidgetTree && !bUICreated)
	{
		BuildUI();
		bUICreated = true;
	}
	return Super::RebuildWidget();
}

void UWG_MicSettingsDialog::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlusButton && !PlusButton->OnClicked.IsAlreadyBound(this, &UWG_MicSettingsDialog::HandlePlus))
		PlusButton->OnClicked.AddDynamic(this, &UWG_MicSettingsDialog::HandlePlus);
	if (MinusButton && !MinusButton->OnClicked.IsAlreadyBound(this, &UWG_MicSettingsDialog::HandleMinus))
		MinusButton->OnClicked.AddDynamic(this, &UWG_MicSettingsDialog::HandleMinus);
	if (SaveButton && !SaveButton->OnClicked.IsAlreadyBound(this, &UWG_MicSettingsDialog::HandleSave))
		SaveButton->OnClicked.AddDynamic(this, &UWG_MicSettingsDialog::HandleSave);
	if (CancelButton && !CancelButton->OnClicked.IsAlreadyBound(this, &UWG_MicSettingsDialog::HandleCancel))
		CancelButton->OnClicked.AddDynamic(this, &UWG_MicSettingsDialog::HandleCancel);
	if (AutoAssignButton && !AutoAssignButton->OnClicked.IsAlreadyBound(this, &UWG_MicSettingsDialog::HandleAutoAssignInputCh))
		AutoAssignButton->OnClicked.AddDynamic(this, &UWG_MicSettingsDialog::HandleAutoAssignInputCh);

	// 既存の登録マイクを入力欄へ反映（初回のみ）
	if (!bSeeded)
	{
		MicCount = FMath::Clamp(InitialMics.Num(), 1, 32);
		RebuildMicList();
		for (int32 i = 0; i < MicInputs.Num() && i < InitialMics.Num(); ++i)
		{
			const FMicSetting& M = InitialMics[i];
			if (MicInputs[i])
			{
				MicInputs[i]->SetText(FText::FromString(M.Name));
			}
			if (MicChCountCombos.IsValidIndex(i) && MicChCountCombos[i])
			{
				MicChCountCombos[i]->SetSelectedIndex(NumChannelsToIndex(M.NumChannels));
			}
			if (MicInputChCombos.IsValidIndex(i) && MicInputChCombos[i])
			{
				const int32 Cnt = MicInputChCombos[i]->GetOptionCount();
				MicInputChCombos[i]->SetSelectedIndex(FMath::Clamp(M.FirstChannel, 0, FMath::Max(0, Cnt - 1)));
			}
		}
		bSeeded = true;
	}
}

void UWG_MicSettingsDialog::BuildUI()
{
	using namespace MicDlgUI;

	// 画面全体を覆う暗幕
	UBorder* Dim = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Dim"));
	Dim->SetBrushColor(DimBg);
	Dim->SetHorizontalAlignment(HAlign_Center);
	Dim->SetVerticalAlignment(VAlign_Center);
	WidgetTree->RootWidget = Dim;

	USizeBox* PanelSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("PanelSize"));
	PanelSize->SetWidthOverride(880.f);
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
		Head->SetContent(MakeText(WidgetTree, ReaperLang::S(TEXT("マイク登録"), TEXT("Mic Registration")), 26, TextPrimary, true));
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

	// マイク本数（＋／－）
	{
		UTextBlock* L = MakeText(WidgetTree, ReaperLang::S(TEXT("マイク本数"), TEXT("Mic count")), 18, TextDim, true);
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(L);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 4.f));
	}
	{
		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		MinusButton = MakeButton(WidgetTree, TEXT("－"), StopGray, TEXT("MinusButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredWidth(64.f); B->SetMinDesiredHeight(56.f); B->AddChild(MinusButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetVerticalAlignment(VAlign_Center);
		}

		CountText = MakeText(WidgetTree, TEXT("1"), 26, TextPrimary, true);
		CountText->SetJustification(ETextJustify::Center);
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredWidth(90.f); B->AddChild(CountText);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetVerticalAlignment(VAlign_Center);
		}

		PlusButton = MakeButton(WidgetTree, TEXT("＋"), AccentGreen, TEXT("PlusButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredWidth(64.f); B->SetMinDesiredHeight(56.f); B->AddChild(PlusButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetVerticalAlignment(VAlign_Center);
		}

		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(Row);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 14.f));
	}

	// マイク名リスト（見出し ＋ 入力ch自動割り当てボタン）
	{
		UHorizontalBox* Head = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		UTextBlock* L = MakeText(WidgetTree, ReaperLang::S(TEXT("マイク名 ／ 構成 ／ 入力ch"), TEXT("Mic name / Format / Input ch")), 18, TextDim, true);
		{
			UHorizontalBoxSlot* S = Head->AddChildToHorizontalBox(L);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}

		// 入力chを上から順番に自動で割り当てるボタン
		AutoAssignButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("入力ch自動割り当て"), TEXT("Auto-assign input ch")), AccentGreen, TEXT("AutoAssignButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredWidth(240.f); B->SetMinDesiredHeight(48.f); B->AddChild(AutoAssignButton);
			UHorizontalBoxSlot* S = Head->AddChildToHorizontalBox(B);
			S->SetVerticalAlignment(VAlign_Center);
		}

		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(Head);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 4.f));
	}
	MicListBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("MicListBox"));
	{
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(MicListBox);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
	}

	// ボタン行（キャンセル / 保存）
	{
		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		CancelButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("キャンセル"), TEXT("Cancel")), StopGray, TEXT("CancelButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(64.f); B->AddChild(CancelButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
		}

		SaveButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("保存"), TEXT("Save")), AccentBlue, TEXT("SaveButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(64.f); B->AddChild(SaveButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}

		BodyCol->AddChildToVerticalBox(Row);
	}

	RebuildMicList();
}

void UWG_MicSettingsDialog::RebuildMicList()
{
	using namespace MicDlgUI;
	if (!MicListBox) return;

	// 既存の入力内容（名前・構成・入力ch）を保持
	TArray<FString> PrevNames;
	TArray<int32>   PrevCountIdx;
	TArray<int32>   PrevChIdx;
	for (int32 i = 0; i < MicInputs.Num(); ++i)
	{
		PrevNames.Add(MicInputs[i] ? MicInputs[i]->GetText().ToString() : FString());
		PrevCountIdx.Add((MicChCountCombos.IsValidIndex(i) && MicChCountCombos[i]) ? MicChCountCombos[i]->GetSelectedIndex() : 0);
		PrevChIdx.Add((MicInputChCombos.IsValidIndex(i) && MicInputChCombos[i]) ? MicInputChCombos[i]->GetSelectedIndex() : 0);
	}

	MicListBox->ClearChildren();
	MicInputs.Reset();
	MicChCountCombos.Reset();
	MicInputChCombos.Reset();
	MicCatalogCombos.Reset();
	MicCatalogLastSel.Reset();

	for (int32 i = 0; i < MicCount; ++i)
	{
		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		// 番号
		UTextBlock* Num = MakeText(WidgetTree, FString::Printf(TEXT("%d"), i + 1), 18, TextDim, true);
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredWidth(28.f); B->AddChild(Num);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetVerticalAlignment(VAlign_Center);
		}

		// マイク名
		UEditableTextBox* In = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass());
		In->SetHintText(FText::FromString(ReaperLang::S(TEXT("例: OH"), TEXT("e.g. OH"))));
		StyleDarkInput(In);
		if (PrevNames.IsValidIndex(i))
		{
			In->SetText(FText::FromString(PrevNames[i]));
		}
		{
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(In);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		}
		MicInputs.Add(In);

		// 一覧から選択（マイクシートのマイク名）：選ぶと上のマイク名欄へ流し込む（欄は編集可のまま）。
		// マイクシート未読込（候補なし）のときは置かない。
		if (CatalogNames.Num() > 0)
		{
			UComboBoxString* Cat = MakeCombo(WidgetTree);
			Cat->OnGenerateWidgetEvent.BindDynamic(this, &UWG_MicSettingsDialog::MakeComboItem);
			Cat->AddOption(ReaperLang::S(TEXT("一覧から選択"), TEXT("Pick…")));   // index0 = プレースホルダ
			for (const FString& Nm : CatalogNames) { Cat->AddOption(Nm); }
			Cat->SetSelectedIndex(0);
			// SetSelectedIndex後にバインド（初期化で不要なコールバックが飛ばないように）
			Cat->OnSelectionChanged.AddDynamic(this, &UWG_MicSettingsDialog::HandleCatalogPick);
			{
				USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				B->SetMinDesiredWidth(150.f); B->AddChild(Cat);
				UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
			}
			MicCatalogCombos.Add(Cat);
			MicCatalogLastSel.Add(Cat->GetSelectedOption());
		}

		// 構成（モノ / ステレオ / 5.1）
		UComboBoxString* CountCombo = MakeCombo(WidgetTree);
		CountCombo->OnGenerateWidgetEvent.BindDynamic(this, &UWG_MicSettingsDialog::MakeComboItem);
		CountCombo->AddOption(ReaperLang::S(TEXT("モノ"), TEXT("Mono")));
		CountCombo->AddOption(ReaperLang::S(TEXT("ステレオ"), TEXT("Stereo")));
		CountCombo->AddOption(TEXT("5.1"));
		CountCombo->SetSelectedIndex(PrevCountIdx.IsValidIndex(i) ? FMath::Clamp(PrevCountIdx[i], 0, 2) : 0);
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredWidth(140.f); B->AddChild(CountCombo);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		}
		MicChCountCombos.Add(CountCombo);

		// 入力ch（開始）
		UComboBoxString* ChCombo = MakeCombo(WidgetTree);
		ChCombo->OnGenerateWidgetEvent.BindDynamic(this, &UWG_MicSettingsDialog::MakeComboItem);
		FillInputChannelOptions(ChCombo, PrevChIdx.IsValidIndex(i) ? PrevChIdx[i] : 0);
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredWidth(180.f); B->AddChild(ChCombo);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetVerticalAlignment(VAlign_Center);
		}
		MicInputChCombos.Add(ChCombo);

		UVerticalBoxSlot* VS = MicListBox->AddChildToVerticalBox(Row);
		VS->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));
	}

	if (CountText)
	{
		CountText->SetText(FText::FromString(FString::FromInt(MicCount)));
	}
}

void UWG_MicSettingsDialog::HandlePlus()
{
	MicCount = FMath::Clamp(MicCount + 1, 1, 32);
	RebuildMicList();
}

void UWG_MicSettingsDialog::HandleMinus()
{
	MicCount = FMath::Clamp(MicCount - 1, 1, 32);
	RebuildMicList();
}

void UWG_MicSettingsDialog::HandleSave()
{
	// 現在の入力（名前＋構成＋入力ch）をすべて集めてコントローラーに保存（空欄は保存側で除外）
	TArray<FMicSetting> Settings;
	for (int32 i = 0; i < MicInputs.Num(); ++i)
	{
		FMicSetting M;
		M.Name = MicInputs[i] ? MicInputs[i]->GetText().ToString() : FString();
		M.NumChannels = IndexToNumChannels(
			(MicChCountCombos.IsValidIndex(i) && MicChCountCombos[i]) ? MicChCountCombos[i]->GetSelectedIndex() : 0);
		M.FirstChannel = (MicInputChCombos.IsValidIndex(i) && MicInputChCombos[i])
			? FMath::Max(0, MicInputChCombos[i]->GetSelectedIndex()) : 0;
		Settings.Add(M);
	}

	if (Owner)
	{
		Owner->SaveMics(Settings);
	}

	RemoveFromParent();
}

void UWG_MicSettingsDialog::HandleCancel()
{
	RemoveFromParent();
}

void UWG_MicSettingsDialog::HandleAutoAssignInputCh()
{
	// 各マイクの構成ch数ぶんずつ、入力chを 0 から上から順番に詰めて割り当てる。
	// 例: 1本目=ステレオ→0,1 / 2本目=モノ→2 / 3本目=5.1→3〜8 …
	int32 Offset = 0;
	for (int32 i = 0; i < MicInputChCombos.Num(); ++i)
	{
		UComboBoxString* Ch = MicInputChCombos[i];
		if (!Ch) continue;

		const int32 OptCount = Ch->GetOptionCount();
		if (OptCount > 0)
		{
			Ch->SetSelectedIndex(FMath::Clamp(Offset, 0, OptCount - 1));
		}

		// この行の構成（モノ=1/ステレオ=2/5.1=6）ぶんだけ次の開始chを進める
		const int32 NumCh = IndexToNumChannels(
			(MicChCountCombos.IsValidIndex(i) && MicChCountCombos[i]) ? MicChCountCombos[i]->GetSelectedIndex() : 0);
		Offset += FMath::Max(1, NumCh);
	}
}

void UWG_MicSettingsDialog::HandleCatalogPick(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// OnSelectionChanged は「どのコンボか」を渡してこないため、
	// 前回選択との差分で変わった行を特定する。
	for (int32 i = 0; i < MicCatalogCombos.Num(); ++i)
	{
		UComboBoxString* C = MicCatalogCombos[i];
		if (!C) continue;

		const FString Cur = C->GetSelectedOption();
		if (MicCatalogLastSel.IsValidIndex(i) && Cur == MicCatalogLastSel[i])
		{
			continue;   // この行は変わっていない
		}
		if (MicCatalogLastSel.IsValidIndex(i))
		{
			MicCatalogLastSel[i] = Cur;
		}

		const int32 Sel = C->GetSelectedIndex();
		if (Sel <= 0) continue;   // 先頭はプレースホルダ（「一覧から選択」）

		// マイク名をテキスト欄へ流し込む（欄はそのまま編集可）
		if (MicInputs.IsValidIndex(i) && MicInputs[i])
		{
			MicInputs[i]->SetText(FText::FromString(Cur));
		}

		// カタログのch数を構成（モノ/ステレオ/5.1）コンボへ反映
		const int32 CatIdx = Sel - 1;   // プレースホルダ分ずらす
		if (CatalogChannels.IsValidIndex(CatIdx) && MicChCountCombos.IsValidIndex(i) && MicChCountCombos[i])
		{
			MicChCountCombos[i]->SetSelectedIndex(NumChannelsToIndex(CatalogChannels[CatIdx]));
		}
	}
}

// 変換・選択肢生成は ReaperChannelUtil に集約。ここは既存の呼び出し元向けの薄いラッパ。
int32 UWG_MicSettingsDialog::NumChannelsToIndex(int32 NumCh)
{
	return ReaperChannelUtil::NumChannelsToIndex(NumCh);
}

int32 UWG_MicSettingsDialog::IndexToNumChannels(int32 Index)
{
	return ReaperChannelUtil::IndexToNumChannels(Index);
}

void UWG_MicSettingsDialog::FillInputChannelOptions(UComboBoxString* Combo, int32 SelectIndex)
{
	ReaperChannelUtil::FillInputChannelOptions(Combo, InputChannelNames, SelectIndex);
}

UWidget* UWG_MicSettingsDialog::MakeComboItem(FString Item)
{
	using namespace MicDlgUI;
	UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	T->SetText(FText::FromString(Item));
	T->SetFont(ReaperFont::Get(16, false));
	T->SetColorAndOpacity(FSlateColor(FieldText));   // 明るい文字色（暗い背景で読めるように）
	return T;
}
