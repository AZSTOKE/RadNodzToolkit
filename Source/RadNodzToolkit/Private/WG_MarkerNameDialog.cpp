// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).


#include "WG_MarkerNameDialog.h"
#include "WG_RadNodzToolkit.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Styling/SlateTypes.h"

#include "ReaperLang.h"
#include "ReaperFont.h"

namespace
{
	// マーカー名ダイアログの配色（本体UIのティール系テーマに合わせる）。
	static const FLinearColor MdPanelBg  = FLinearColor(0.045f, 0.070f, 0.080f, 1.f);   // パネル背景（少し明るめ）
	static const FLinearColor MdAccent   = FLinearColor(0.16f, 0.80f, 0.74f, 1.f);       // ティール（設定ボタン/タイトル）
	static const FLinearColor MdGray     = FLinearColor(0.11f, 0.17f, 0.19f, 1.f);       // キャンセル/履歴チップ背景
	static const FLinearColor MdText     = FLinearColor(0.88f, 0.95f, 0.96f, 1.f);       // 明るい文字
	static const FLinearColor MdTextDim  = FLinearColor(0.50f, 0.66f, 0.69f, 1.f);       // 補助文字
	static const FLinearColor MdField    = FLinearColor(0.92f, 0.95f, 0.96f, 1.f);       // 入力欄背景（明色）
	static const FLinearColor MdEdge     = FLinearColor(0.16f, 0.80f, 0.74f, 0.55f);     // ふちどり（ティール半透明）

	// 角丸ボタンにスタイルを当てる。bgColor が明るいときは黒文字、暗いときは明文字にする。
	static void StyleDialogButton(UButton* Btn, const FLinearColor& Bg, float Radius = 12.f)
	{
		if (!Btn) { return; }
		FButtonStyle Style = Btn->GetStyle();
		const FLinearColor Hover(Bg.R * 1.15f + 0.02f, Bg.G * 1.15f + 0.02f, Bg.B * 1.15f + 0.02f, 1.f);
		const FLinearColor Press(Bg.R * 0.85f, Bg.G * 0.85f, Bg.B * 0.85f, 1.f);
		Style.Normal   = FSlateRoundedBoxBrush(Bg,    Radius, MdEdge, 1.2f);
		Style.Hovered  = FSlateRoundedBoxBrush(Hover, Radius, MdAccent, 1.6f);
		Style.Pressed  = FSlateRoundedBoxBrush(Press, Radius, MdEdge, 1.2f);
		Style.Disabled = FSlateRoundedBoxBrush(FLinearColor(0.3f, 0.34f, 0.35f, 1.f), Radius, MdEdge, 1.f);
		Style.NormalPadding  = FMargin(0.f);
		Style.PressedPadding = FMargin(0.f);
		Btn->SetStyle(Style);
	}

	// ボタンに1行センタリングのラベルを付ける（折り返し無効ではみ出しを防ぐ）。
	static void SetButtonLabel(UWidgetTree* Tree, UButton* Btn, const FString& Label, int32 FontSize, const FLinearColor& Color)
	{
		if (!Btn) { return; }
		UTextBlock* T = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(Label));
		T->SetFont(ReaperFont::Get(FontSize, true));
		T->SetColorAndOpacity(FSlateColor(Color));
		T->SetJustification(ETextJustify::Center);
		T->SetAutoWrapText(false);   // ← 「キャンセル」がボタンからはみ出す不具合の対策
		Btn->AddChild(T);
	}
}

void UWG_MarkerNameDialog::Setup(UWG_RadNodzToolkit* InOwner, double InPositionSeconds)
{
	Owner = InOwner;
	PositionSeconds = InPositionSeconds;
	bRenameMode = false;
}

void UWG_MarkerNameDialog::SetupRename(UWG_RadNodzToolkit* InOwner, int32 InMarkerId, const FString& InCurrentName)
{
	Owner = InOwner;
	bRenameMode = true;
	RenameMarkerId = InMarkerId;
	InitialName = InCurrentName;
	if (NameInput) { NameInput->SetText(FText::FromString(InitialName)); }
}

TSharedRef<SWidget> UWG_MarkerNameDialog::RebuildWidget()
{
	if (!bUICreated && WidgetTree)
	{
		bUICreated = true;

		// 画面全体を覆う暗幕（クリックは奥に通さない）
		UBorder* Backdrop = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Backdrop"));
		Backdrop->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.62f));
		Backdrop->SetHorizontalAlignment(HAlign_Center);
		Backdrop->SetVerticalAlignment(VAlign_Center);
		WidgetTree->RootWidget = Backdrop;

		// パネル幅を固定して、横画面でも間延びしない中央ダイアログにする。
		USizeBox* PanelSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		PanelSize->SetWidthOverride(460.f);
		Backdrop->SetContent(PanelSize);

		// 中央パネル（角丸＋ティールのふちどり）
		UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Panel"));
		Panel->SetBrush(FSlateRoundedBoxBrush(MdPanelBg, 18.f, MdEdge, 1.5f));
		Panel->SetPadding(FMargin(28.f, 26.f, 28.f, 24.f));
		PanelSize->AddChild(Panel);

		UVerticalBox* Box = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
		Panel->SetContent(Box);

		// タイトル（新規作成 / 編集 で出し分け）
		UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Title->SetText(FText::FromString(bRenameMode
			? ReaperLang::S(TEXT("マーカー名を編集"), TEXT("Edit Marker Name"))
			: ReaperLang::S(TEXT("マーカー名を設定"), TEXT("Set Marker Name"))));
		Title->SetFont(ReaperFont::Get(24, true));
		Title->SetColorAndOpacity(FSlateColor(MdAccent));
		Title->SetJustification(ETextJustify::Center);
		if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(Title)))
		{
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		// 補助テキスト
		UTextBlock* Hint = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Hint->SetText(FText::FromString(ReaperLang::S(TEXT("マーカーに付ける名前を入力してください"), TEXT("Enter a name for this marker"))));
		Hint->SetFont(ReaperFont::Get(14, false));
		Hint->SetColorAndOpacity(FSlateColor(MdTextDim));
		Hint->SetJustification(ETextJustify::Center);
		Hint->SetAutoWrapText(true);
		if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(Hint)))
		{
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 18.f));
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		// 入力欄（角丸・明色背景・黒文字）
		NameInput = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass());
		NameInput->SetHintText(FText::FromString(ReaperLang::S(TEXT("マーカー名"), TEXT("Marker name"))));
		if (!InitialName.IsEmpty()) { NameInput->SetText(FText::FromString(InitialName)); }   // 編集モードは現在名を初期表示
		{
			FEditableTextBoxStyle TB = NameInput->WidgetStyle;
			TB.BackgroundImageNormal   = FSlateRoundedBoxBrush(MdField, 10.f, MdEdge, 1.2f);
			TB.BackgroundImageHovered  = FSlateRoundedBoxBrush(MdField, 10.f, MdAccent, 1.4f);
			TB.BackgroundImageFocused  = FSlateRoundedBoxBrush(MdField, 10.f, MdAccent, 1.8f);
			TB.BackgroundImageReadOnly = FSlateRoundedBoxBrush(MdField, 10.f, MdEdge, 1.2f);
			TB.Padding = FMargin(12.f, 10.f);
			TB.SetFont(ReaperFont::Get(20, false));
			TB.SetForegroundColor(FSlateColor(FLinearColor::Black));
			TB.FocusedForegroundColor = FSlateColor(FLinearColor::Black);
			TB.ReadOnlyForegroundColor = FSlateColor(FLinearColor::Black);
			NameInput->WidgetStyle = TB;
		}
		NameInput->OnTextCommitted.AddDynamic(this, &UWG_MarkerNameDialog::HandleInputCommitted);
		{
			USizeBox* FieldSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			FieldSize->SetMinDesiredHeight(54.f);
			FieldSize->AddChild(NameInput);
			if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(FieldSize)))
			{
				S->SetPadding(FMargin(0.f, 0.f, 0.f, 18.f));
				S->SetHorizontalAlignment(HAlign_Fill);
			}
		}

		// ボタン行（設定 / キャンセル）。各ボタンは高さ固定・1行センタリング。
		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(Row)))
		{
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		auto MakeActionBtn = [&](const FString& Label, FName Name, const FLinearColor& Bg, const FLinearColor& TextColor, float RightPad) -> UButton*
		{
			UButton* Btn = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
			StyleDialogButton(Btn, Bg);
			SetButtonLabel(WidgetTree, Btn, Label, 18, TextColor);

			USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BtnSize->SetMinDesiredHeight(58.f);
			BtnSize->AddChild(Btn);
			if (UHorizontalBoxSlot* HS = Cast<UHorizontalBoxSlot>(Row->AddChild(BtnSize)))
			{
				HS->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				HS->SetVerticalAlignment(VAlign_Fill);
				HS->SetPadding(FMargin(0.f, 0.f, RightPad, 0.f));
			}
			return Btn;
		};

		// 設定=ティール（黒文字）、キャンセル=グレー（明文字）。
		SetButton    = MakeActionBtn(ReaperLang::S(TEXT("設定"),       TEXT("Set")),    TEXT("SetButton"),    MdAccent, FLinearColor(0.02f, 0.09f, 0.09f, 1.f), 10.f);
		CancelButton = MakeActionBtn(ReaperLang::S(TEXT("キャンセル"), TEXT("Cancel")), TEXT("CancelButton"), MdGray,   MdText,                                 0.f);
		SetButton->OnClicked.AddDynamic(this, &UWG_MarkerNameDialog::HandleSet);
		CancelButton->OnClicked.AddDynamic(this, &UWG_MarkerNameDialog::HandleCancel);

		// --- 履歴セクション（以前入力した名前をチップとして並べ、押すと入力欄へ流し込む）---
		HistoryValues.Reset();
		if (Owner)
		{
			const TArray<FString>& Hist = Owner->GetMarkerNameHistory();
			const int32 Count = FMath::Min(Hist.Num(), MaxHistoryButtons);
			for (int32 i = 0; i < Count; ++i) { HistoryValues.Add(Hist[i]); }
		}

		if (HistoryValues.Num() > 0)
		{
			// 見出し
			UTextBlock* HistLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			HistLabel->SetText(FText::FromString(ReaperLang::S(TEXT("以前の入力"), TEXT("Previous entries"))));
			HistLabel->SetFont(ReaperFont::Get(13, true));
			HistLabel->SetColorAndOpacity(FSlateColor(MdTextDim));
			if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(HistLabel)))
			{
				S->SetPadding(FMargin(2.f, 20.f, 0.f, 8.f));
				S->SetHorizontalAlignment(HAlign_Left);
			}

			// チップを折り返し配置
			UWrapBox* Wrap = WidgetTree->ConstructWidget<UWrapBox>(UWrapBox::StaticClass());
			if (UVerticalBoxSlot* S = Cast<UVerticalBoxSlot>(Box->AddChild(Wrap)))
			{
				S->SetHorizontalAlignment(HAlign_Fill);
			}

			for (int32 i = 0; i < HistoryValues.Num(); ++i)
			{
				UButton* Chip = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(),
					FName(*FString::Printf(TEXT("HistChip_%d"), i)));
				StyleDialogButton(Chip, MdGray, 10.f);
				// チップは押しやすいよう内側にしっかり余白を持たせる（短い名前でも大きな当たり判定にする）。
				{
					FButtonStyle CS = Chip->GetStyle();
					CS.NormalPadding  = FMargin(22.f, 13.f);
					CS.PressedPadding = FMargin(22.f, 13.f);
					Chip->SetStyle(CS);
				}
				SetButtonLabel(WidgetTree, Chip, HistoryValues[i], 18, MdText);

				switch (i)
				{
				case 0: Chip->OnClicked.AddDynamic(this, &UWG_MarkerNameDialog::HandleHistory0); break;
				case 1: Chip->OnClicked.AddDynamic(this, &UWG_MarkerNameDialog::HandleHistory1); break;
				case 2: Chip->OnClicked.AddDynamic(this, &UWG_MarkerNameDialog::HandleHistory2); break;
				case 3: Chip->OnClicked.AddDynamic(this, &UWG_MarkerNameDialog::HandleHistory3); break;
				case 4: Chip->OnClicked.AddDynamic(this, &UWG_MarkerNameDialog::HandleHistory4); break;
				case 5: Chip->OnClicked.AddDynamic(this, &UWG_MarkerNameDialog::HandleHistory5); break;
				case 6: Chip->OnClicked.AddDynamic(this, &UWG_MarkerNameDialog::HandleHistory6); break;
				case 7: Chip->OnClicked.AddDynamic(this, &UWG_MarkerNameDialog::HandleHistory7); break;
				default: break;
				}

				USizeBox* ChipSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				ChipSize->SetMinDesiredHeight(56.f);
				ChipSize->SetMinDesiredWidth(96.f);
				ChipSize->AddChild(Chip);
				if (UWrapBoxSlot* WS = Cast<UWrapBoxSlot>(Wrap->AddChild(ChipSize)))
				{
					WS->SetPadding(FMargin(0.f, 0.f, 10.f, 10.f));
				}
			}
		}
	}

	return Super::RebuildWidget();
}

void UWG_MarkerNameDialog::ApplyHistory(int32 Index)
{
	if (NameInput && HistoryValues.IsValidIndex(Index))
	{
		NameInput->SetText(FText::FromString(HistoryValues[Index]));
	}
}

void UWG_MarkerNameDialog::HandleHistory0() { ApplyHistory(0); }
void UWG_MarkerNameDialog::HandleHistory1() { ApplyHistory(1); }
void UWG_MarkerNameDialog::HandleHistory2() { ApplyHistory(2); }
void UWG_MarkerNameDialog::HandleHistory3() { ApplyHistory(3); }
void UWG_MarkerNameDialog::HandleHistory4() { ApplyHistory(4); }
void UWG_MarkerNameDialog::HandleHistory5() { ApplyHistory(5); }
void UWG_MarkerNameDialog::HandleHistory6() { ApplyHistory(6); }
void UWG_MarkerNameDialog::HandleHistory7() { ApplyHistory(7); }

void UWG_MarkerNameDialog::Commit(bool bUseName)
{
	if (bCommitted) return;
	bCommitted = true;

	FString Name;
	if (bUseName && NameInput)
	{
		Name = NameInput->GetText().ToString().TrimStartAndEnd();
	}
	if (Owner)
	{
		if (bRenameMode)
		{
			// 既存マーカーの名前を更新する（新規作成はしない）。
			Owner->RenameMarker(RenameMarkerId, Name);
		}
		else
		{
			// 新規作成：キャンセル時（bUseName=false）は名前なしで作成する。設定時も空欄なら名前なし。
			Owner->CommitMarker(PositionSeconds, Name);
		}
	}
	RemoveFromParent();
}

void UWG_MarkerNameDialog::HandleSet()
{
	Commit(/*bUseName=*/true);
}

void UWG_MarkerNameDialog::HandleCancel()
{
	if (bRenameMode)
	{
		// 編集のキャンセルは何も変更せず閉じるだけ。
		RemoveFromParent();
		return;
	}
	// 新規作成のキャンセルは名前なしでマーカーを作成する（仕様）。
	Commit(/*bUseName=*/false);
}

void UWG_MarkerNameDialog::HandleInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		Commit(/*bUseName=*/true);
	}
}
