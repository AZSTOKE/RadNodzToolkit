// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#include "WG_AddTracksDialog.h"
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
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"
#include "Components/SizeBox.h"
#include "Styling/CoreStyle.h"
#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateRoundedBoxBrush.h"

namespace AddDlgUI
{
	// ダーク×ティール（iZotope調）
	static const FLinearColor DimBg       = FLinearColor(0.0f, 0.01f, 0.015f, 0.80f);
	static const FLinearColor PanelBg     = FLinearColor(0.036f, 0.080f, 0.094f, 1.f);
	static const FLinearColor HeaderBg    = FLinearColor(0.050f, 0.105f, 0.122f, 1.f);

	static const FLinearColor AccentGreen = FLinearColor(0.16f, 0.74f, 0.58f, 1.f);
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

	static UComboBoxString* MakeDarkCombo(UWidgetTree* Tree)
	{
		UComboBoxString* C = Tree->ConstructWidget<UComboBoxString>(UComboBoxString::StaticClass());
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		C->Font = ReaperFont::Get(18, false);
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

void UWG_AddTracksDialog::Setup(UWG_RadNodzToolkit* InOwner)
{
	Owner = InOwner;
}

TSharedRef<SWidget> UWG_AddTracksDialog::RebuildWidget()
{
	if (WidgetTree && !bUICreated)
	{
		BuildUI();
		bUICreated = true;
	}
	return Super::RebuildWidget();
}

void UWG_AddTracksDialog::NativeConstruct()
{
	Super::NativeConstruct();

	if (AddButton && !AddButton->OnClicked.IsAlreadyBound(this, &UWG_AddTracksDialog::HandleAdd))
		AddButton->OnClicked.AddDynamic(this, &UWG_AddTracksDialog::HandleAdd);
	if (CancelButton && !CancelButton->OnClicked.IsAlreadyBound(this, &UWG_AddTracksDialog::HandleCancel))
		CancelButton->OnClicked.AddDynamic(this, &UWG_AddTracksDialog::HandleCancel);

	// Setup() で渡された Owner の登録マイクをプレビューに反映
	RebuildMicPreview();
}

void UWG_AddTracksDialog::BuildUI()
{
	using namespace AddDlgUI;

	// 画面全体を覆う暗幕（クリックを背面に通さない）
	UBorder* Dim = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Dim"));
	Dim->SetBrushColor(DimBg);
	Dim->SetHorizontalAlignment(HAlign_Center);
	Dim->SetVerticalAlignment(VAlign_Center);
	WidgetTree->RootWidget = Dim;

	// 中央のパネル
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
		Head->SetContent(MakeText(WidgetTree, ReaperLang::S(TEXT("新規トラック追加"), TEXT("Add New Track")), 26, TextPrimary, true));
		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(Head);
		S->SetHorizontalAlignment(HAlign_Fill);
	}

	// 本体（余白付き）
	UBorder* Body = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
	Body->SetBrushColor(PanelBg);
	Body->SetPadding(FMargin(28.f, 22.f));
	{
		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(Body);
		S->SetHorizontalAlignment(HAlign_Fill);
	}
	UVerticalBox* BodyCol = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	Body->SetContent(BodyCol);

	// 収録素材名
	{
		UTextBlock* L = MakeText(WidgetTree, ReaperLang::S(TEXT("収録素材名"), TEXT("Material name")), 18, TextDim, true);
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(L);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 4.f));
	}
	MaterialInput = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("MaterialInput"));
	MaterialInput->SetHintText(FText::FromString(ReaperLang::S(TEXT("例: 鉄骨"), TEXT("e.g. Steel frame"))));
	StyleDarkInput(MaterialInput);
	{
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(MaterialInput);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 14.f));
	}

	// 追加先の親トラック（先頭 "Master" ＝最上位／親なし。赤字で区別）
	{
		UTextBlock* L = MakeText(WidgetTree, ReaperLang::S(TEXT("追加先（親トラック）"), TEXT("Add under (parent track)")), 18, TextDim, true);
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(L);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 4.f));
	}
	ParentCombo = MakeDarkCombo(WidgetTree);
	ParentCombo->OnGenerateWidgetEvent.BindDynamic(this, &UWG_AddTracksDialog::MakeParentItem);
	PopulateParentCombo();
	{
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(ParentCombo);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 14.f));
	}

	// 登録マイク（設定で登録した本数分のトラックが追加される旨を表示）
	{
		UTextBlock* L = MakeText(WidgetTree, ReaperLang::S(TEXT("登録マイク（このマイク分のトラックを追加します）"), TEXT("Registered mics (tracks added for each)")), 18, TextDim, true);
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(L);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 4.f));
	}
	MicPreviewBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("MicPreviewBox"));
	{
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(MicPreviewBox);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
	}

	// ボタン行
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

		AddButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("追加する"), TEXT("Add")), AccentGreen, TEXT("AddButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(64.f); B->AddChild(AddButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}

		BodyCol->AddChildToVerticalBox(Row);
	}

	RebuildMicPreview();
}

void UWG_AddTracksDialog::RebuildMicPreview()
{
	using namespace AddDlgUI;
	if (!MicPreviewBox) return;

	MicPreviewBox->ClearChildren();

	TArray<FMicSetting> Mics;
	if (Owner)
	{
		Mics = Owner->GetMics();
	}

	if (Mics.Num() == 0)
	{
		// 未登録時の案内
		UTextBlock* Empty = MakeText(WidgetTree,
			ReaperLang::S(TEXT("マイクが登録されていません。「設定」からマイク名を登録してください。"), TEXT("No mics registered. Please register from Settings.")),
			16, TextDim);
		Empty->SetAutoWrapText(true);
		UVerticalBoxSlot* S = MicPreviewBox->AddChildToVerticalBox(Empty);
		S->SetPadding(FMargin(2.f, 2.f, 0.f, 2.f));

		// 登録が無いときは追加できない
		if (AddButton) AddButton->SetIsEnabled(false);
		return;
	}

	if (AddButton) AddButton->SetIsEnabled(true);

	for (int32 i = 0; i < Mics.Num(); ++i)
	{
		FString Fmt;
		switch (Mics[i].NumChannels)
		{
		case 2:  Fmt = ReaperLang::S(TEXT("ステレオ"), TEXT("Stereo")); break;
		case 6:  Fmt = TEXT("5.1"); break;
		case 1:  Fmt = ReaperLang::S(TEXT("モノ"), TEXT("Mono")); break;
		default: Fmt = FString::Printf(TEXT("%dch"), Mics[i].NumChannels); break;
		}
		const FString Line = FString::Printf(TEXT("%d.  %s   (%s / in %d)"),
			i + 1, *Mics[i].Name, *Fmt, Mics[i].FirstChannel);
		UTextBlock* T = MakeText(WidgetTree, Line, 18, TextPrimary, true);
		UVerticalBoxSlot* S = MicPreviewBox->AddChildToVerticalBox(T);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 6.f));
	}
}

UWidget* UWG_AddTracksDialog::MakeParentItem(FString Item)
{
	using namespace AddDlgUI;
	UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	T->SetText(FText::FromString(Item));
	T->SetFont(ReaperFont::Get(18, false));
	// 「Master」＝最上位（親なし）の特別項目は赤字で区別（同名の実トラックと混同しないように）
	const bool bMaster = (Item == TEXT("Master"));
	T->SetColorAndOpacity(FSlateColor(bMaster ? FLinearColor(0.96f, 0.30f, 0.32f, 1.f) : FieldText));
	return T;
}

void UWG_AddTracksDialog::PopulateParentCombo()
{
	if (!ParentCombo) return;

	ParentCombo->ClearOptions();
	ParentCombo->AddOption(TEXT("Master"));   // 先頭＝最上位（親なし）
	if (Owner)
	{
		for (const FTrackDetail& T : Owner->TrackList)
		{
			if (!T.Name.IsEmpty() && T.Name != TEXT("Master"))
			{
				ParentCombo->AddOption(T.Name);
			}
		}
	}
	ParentCombo->SetSelectedIndex(0);   // 既定は Master
}

void UWG_AddTracksDialog::HandleAdd()
{
	const FString Material = MaterialInput ? MaterialInput->GetText().ToString() : FString();
	FString ParentName = ParentCombo ? ParentCombo->GetSelectedOption() : TEXT("Master");
	if (ParentName.IsEmpty()) { ParentName = TEXT("Master"); }

	if (Owner)
	{
		// 登録済みマイク分のトラックを一括追加（生成ロジックはコントローラー側）
		Owner->AddTracksForMaterial(Material, ParentName);
	}

	RemoveFromParent();
}

void UWG_AddTracksDialog::HandleCancel()
{
	RemoveFromParent();
}
