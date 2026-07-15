// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_MediaItemRow.h"
#include "WG_RadNodzToolkit.h"
#include "ReaperFont.h"
#include "ReaperLang.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/SizeBox.h"
#include "Styling/CoreStyle.h"
#include "Brushes/SlateRoundedBoxBrush.h"

namespace MediaRowUI
{
	static const FLinearColor RowBg       = FLinearColor(0.020f, 0.040f, 0.048f, 1.f);
	static const FLinearColor RowActiveBg = FLinearColor(0.060f, 0.140f, 0.110f, 1.f); // 再生(カーソル)位置のハイライト
	static const FLinearColor RowNGBg     = FLinearColor(0.060f, 0.060f, 0.065f, 1.f); // NG(ミュート)時のグレー
	static const FLinearColor DisabledGray= FLinearColor(0.20f, 0.22f, 0.24f, 1.f);
	static const FLinearColor FieldBg     = FLinearColor(0.040f, 0.072f, 0.085f, 1.f);
	static const FLinearColor FieldText   = FLinearColor(0.86f, 0.95f, 0.95f, 1.f);
	static const FLinearColor Outline     = FLinearColor(0.14f, 0.40f, 0.42f, 1.f);
	static const FLinearColor Teal        = FLinearColor(0.16f, 0.78f, 0.72f, 1.f);
	// 再生中(■)はSTOPと同じグレー（赤=録音に見えるのを避ける）
	static const FLinearColor PlayingGray = FLinearColor(0.16f, 0.22f, 0.25f, 1.f);
	static const FLinearColor NGRed       = FLinearColor(0.92f, 0.30f, 0.34f, 1.f);
	static const FLinearColor OKGreen     = FLinearColor(0.13f, 0.74f, 0.55f, 1.f);
	static const FLinearColor DarkText    = FLinearColor(0.03f, 0.06f, 0.06f, 1.f);

	static void StyleInput(UEditableTextBox* Box)
	{
		if (!Box) return;
		FSlateRoundedBoxBrush Brush(FieldBg, 8.f, Outline, 1.1f);
		Box->WidgetStyle.SetBackgroundImageNormal(Brush);
		Box->WidgetStyle.SetBackgroundImageHovered(Brush);
		Box->WidgetStyle.SetBackgroundImageFocused(Brush);
		Box->WidgetStyle.SetBackgroundImageReadOnly(Brush);
		Box->WidgetStyle.SetForegroundColor(FSlateColor(FieldText));
		Box->WidgetStyle.FocusedForegroundColor = FSlateColor(FieldText);
		Box->WidgetStyle.Padding = FMargin(12.f, 9.f);
		Box->WidgetStyle.TextStyle.ColorAndOpacity = FSlateColor(FieldText);
		Box->WidgetStyle.TextStyle.Font = ReaperFont::Get(18, false);
	}

	// 白ブラシの角丸ボタン（色はSetBackgroundColorで付ける）。テキストを OutText に返す。
	static UButton* MakeBtn(UWidgetTree* Tree, const FString& Label, FLinearColor Bg, TObjectPtr<UTextBlock>* OutText = nullptr)
	{
		UButton* Btn = Tree->ConstructWidget<UButton>(UButton::StaticClass());
		FButtonStyle S = Btn->GetStyle();
		S.Normal  = FSlateRoundedBoxBrush(FLinearColor::White, 8.f);
		S.Hovered = FSlateRoundedBoxBrush(FLinearColor(1.12f, 1.12f, 1.12f, 1.f), 8.f);
		S.Pressed = FSlateRoundedBoxBrush(FLinearColor(0.85f, 0.85f, 0.85f, 1.f), 8.f);
		Btn->SetStyle(S);
		Btn->SetBackgroundColor(Bg);

		UTextBlock* T = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(Label));
		T->SetFont(ReaperFont::Get(18, true));
		T->SetColorAndOpacity(FSlateColor(DarkText));
		T->SetJustification(ETextJustify::Center);
		Btn->SetContent(T);
		if (OutText) { *OutText = T; }
		return Btn;
	}
}

void UWG_MediaItemRow::InitRow(UMediaItem* InItem, const FString& InName, double InStart, UWG_RadNodzToolkit* InOwner)
{
	Item = InItem;
	Owner = InOwner;
	MediaName = InName;
	StartTime = InStart;

	if (NameInput && !NameInput->HasKeyboardFocus())
	{
		NameInput->SetText(FText::FromString(MediaName));
	}
	UpdateNGState();
	FetchAndShowLevels();
}

void UWG_MediaItemRow::FetchAndShowLevels()
{
	if (Owner && Item)
	{
		// ラウドネスは自動計測しない。「計測」ボタンで計測済みならキャッシュから表示する。
		// キー＝メディア名＋開始位置（同一メディアの再オープンを同一視）。
		const FString CacheKey = FString::Printf(TEXT("%s@%.3f"), *MediaName, StartTime);
		double Cached = 0.0;
		bLevelsFetched = Owner->TryGetCachedMomentaryLUFS(CacheKey, Cached);
		if (bLevelsFetched)
		{
			MomentaryLUFS = Cached;
		}

		// メディア音量も取得
		VolumeDb = Owner->GetMediaItemVolume(Item);
	}
	ApplyLevelsText();
	RefreshVolumeText();
}

void UWG_MediaItemRow::RefreshLevelsFromCache()
{
	if (Owner && Item)
	{
		const FString CacheKey = FString::Printf(TEXT("%s@%.3f"), *MediaName, StartTime);
		double Cached = 0.0;
		bLevelsFetched = Owner->TryGetCachedMomentaryLUFS(CacheKey, Cached);
		if (bLevelsFetched)
		{
			MomentaryLUFS = Cached;
		}
	}
	ApplyLevelsText();
}

void UWG_MediaItemRow::RefreshVolumeText()
{
	if (VolDbText)
	{
		VolDbText->SetText(FText::FromString(FString::Printf(TEXT("%.1f dB"), VolumeDb)));
	}
}

void UWG_MediaItemRow::HandleVolMinusClicked()
{
	if (!Owner || !Item) return;
	VolumeDb = Owner->AddMediaItemVolume(Item, -1.0);
	RefreshVolumeText();
}

void UWG_MediaItemRow::HandleVolPlusClicked()
{
	if (!Owner || !Item) return;
	VolumeDb = Owner->AddMediaItemVolume(Item, +1.0);
	RefreshVolumeText();
}

void UWG_MediaItemRow::ApplyLevelsText()
{
	if (!LevelsText) return;

	if (!bLevelsFetched)
	{
		LevelsText->SetText(FText::FromString(TEXT("Momentary: --")));
		return;
	}

	LevelsText->SetText(FText::FromString(
		FString::Printf(TEXT("Momentary: %.1f LUFS"), MomentaryLUFS)));
}

TSharedRef<SWidget> UWG_MediaItemRow::RebuildWidget()
{
	if (WidgetTree && !WidgetTree->RootWidget)
	{
		BuildUI();
	}
	return Super::RebuildWidget();
}

void UWG_MediaItemRow::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectButton && !SelectButton->OnClicked.IsAlreadyBound(this, &UWG_MediaItemRow::HandleSelectClicked))
		SelectButton->OnClicked.AddDynamic(this, &UWG_MediaItemRow::HandleSelectClicked);
	if (NGButton && !NGButton->OnClicked.IsAlreadyBound(this, &UWG_MediaItemRow::HandleNGClicked))
		NGButton->OnClicked.AddDynamic(this, &UWG_MediaItemRow::HandleNGClicked);
	if (NameInput && !NameInput->OnTextCommitted.IsAlreadyBound(this, &UWG_MediaItemRow::HandleNameCommitted))
		NameInput->OnTextCommitted.AddDynamic(this, &UWG_MediaItemRow::HandleNameCommitted);
	if (VolMinusButton && !VolMinusButton->OnClicked.IsAlreadyBound(this, &UWG_MediaItemRow::HandleVolMinusClicked))
		VolMinusButton->OnClicked.AddDynamic(this, &UWG_MediaItemRow::HandleVolMinusClicked);
	if (VolPlusButton && !VolPlusButton->OnClicked.IsAlreadyBound(this, &UWG_MediaItemRow::HandleVolPlusClicked))
		VolPlusButton->OnClicked.AddDynamic(this, &UWG_MediaItemRow::HandleVolPlusClicked);

	if (NameInput && !NameInput->HasKeyboardFocus())
	{
		NameInput->SetText(FText::FromString(MediaName));
	}
	UpdateNGState();
	ApplyLevelsText();
	RefreshVolumeText();
}

void UWG_MediaItemRow::BuildUI()
{
	using namespace MediaRowUI;

	RootBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("MediaRowBorder"));
	RootBorder->SetBrush(FSlateRoundedBoxBrush(FLinearColor::White, 8.f));
	RootBorder->SetBrushColor(RowBg);
	RootBorder->SetPadding(FMargin(10.f, 7.f));
	WidgetTree->RootWidget = RootBorder;

	// 縦並び：[操作行] ＋ [レベル表示行]
	UVerticalBox* Col = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	RootBorder->SetContent(Col);

	UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	Col->AddChildToVerticalBox(Row);

	// ▶ 再生 / ▢ 停止
	SelectButton = MakeBtn(WidgetTree, TEXT("▶"), Teal, &SelectGlyph);
	{
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(54.f); B->SetMinDesiredHeight(46.f); B->AddChild(SelectButton);
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
	}

	// メディア名（編集可能）
	NameInput = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("MediaNameInput"));
	NameInput->SetHintText(FText::FromString(ReaperLang::S(TEXT("メディア名…"), TEXT("Media name…"))));
	StyleInput(NameInput);
	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(NameInput);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
	}

	// メディア音量 −  （名前の右に横並び）
	VolMinusButton = MakeBtn(WidgetTree, TEXT("－"), Teal);
	{
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(46.f); B->SetMinDesiredHeight(46.f); B->AddChild(VolMinusButton);
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 4.f, 0.f));
	}

	VolDbText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("MediaVolDbText"));
	VolDbText->SetText(FText::FromString(TEXT("0.0 dB")));
	VolDbText->SetFont(ReaperFont::Get(15, true));
	VolDbText->SetColorAndOpacity(FSlateColor(FieldText));
	VolDbText->SetJustification(ETextJustify::Center);
	{
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(70.f); B->AddChild(VolDbText);
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
	}

	VolPlusButton = MakeBtn(WidgetTree, TEXT("＋"), Teal);
	{
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(46.f); B->SetMinDesiredHeight(46.f); B->AddChild(VolPlusButton);
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(4.f, 0.f, 12.f, 0.f));
	}

	// NG / OK 切替
	NGButton = MakeBtn(WidgetTree, TEXT("NG"), NGRed, &NGButtonText);
	{
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(64.f); B->SetMinDesiredHeight(46.f); B->AddChild(NGButton);
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
		S->SetVerticalAlignment(VAlign_Center);
	}

	// 2段目：レベル表示（モーメンタリー ラウドネス）
	LevelsText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("LevelsText"));
	LevelsText->SetFont(ReaperFont::Get(15, false));
	LevelsText->SetColorAndOpacity(FSlateColor(FLinearColor(0.55f, 0.72f, 0.74f, 1.f)));
	{
		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(LevelsText);
		S->SetPadding(FMargin(56.f, 5.f, 0.f, 0.f));   // ▶ボタン幅ぶん右へ寄せる
	}

	ApplyLevelsText();
	RefreshVolumeText();
}

void UWG_MediaItemRow::SetActive(bool bInActive)
{
	bRowActive = bInActive;
	RefreshRowVisual();
}

void UWG_MediaItemRow::SetPlaying(bool bInPlaying)
{
	bRowPlaying = bInPlaying;
	RefreshRowVisual();
}

void UWG_MediaItemRow::SetLocked(bool bLocked)
{
	bIsLocked = bLocked;
	if (SelectButton)
	{
		SelectButton->SetIsEnabled(!bLocked);
		// ロック中：押せないことは暗さで示しつつ、再生中は濃く／停止中は薄くして今の状態が分かるようにする。
		SelectButton->SetRenderOpacity(bLocked ? (bRowPlaying ? 0.85f : 0.35f) : 1.0f);
	}
}

void UWG_MediaItemRow::RefreshRowVisual()
{
	using namespace MediaRowUI;

	if (RootBorder)
	{
		// NG(ミュート)時はグレー、それ以外は 再生位置ハイライト or 通常
		RootBorder->SetBrushColor(bIsNG ? RowNGBg : (bRowActive ? RowActiveBg : RowBg));
	}
	if (SelectGlyph)
	{
		// 再生中はSTOPと同じ塗りつぶし四角(■)
		SelectGlyph->SetText(FText::FromString(bRowPlaying ? TEXT("■") : TEXT("▶")));
		// NG時は薄い文字 / グレー背景では明るい文字 / ティール背景では濃い文字
		const FLinearColor GlyphCol = bIsNG ? FLinearColor(0.45f, 0.48f, 0.50f, 1.f)
			: (bRowPlaying ? FieldText : DarkText);
		SelectGlyph->SetColorAndOpacity(FSlateColor(GlyphCol));
	}
	if (SelectButton)
	{
		// NG時は再生不可（無効化＋グレー）。ロック中はNG状態に関わらず常に無効化する。
		SelectButton->SetIsEnabled(!bIsNG && !bIsLocked);
		SelectButton->SetBackgroundColor(bIsNG ? DisabledGray : (bRowPlaying ? PlayingGray : Teal));
	}

	// NG時は行を薄暗くするが、OK(NG解除)ボタンだけは「押せる」と分かるよう常に明るく保つ。
	// ロック中は再生(SelectButton)も、Reaper側の操作で本関数が再度呼ばれても
	// 常に暗いままにする（ロックの見た目が意図せず解除されないようにするため）。
	const float Op = bIsNG ? 0.55f : 1.0f;
	if (SelectButton) SelectButton->SetRenderOpacity(bIsLocked ? (bRowPlaying ? 0.85f : 0.35f) : Op);
	if (NameInput)    NameInput->SetRenderOpacity(Op);
	if (NGButton)     NGButton->SetRenderOpacity(1.0f);
	SetRenderOpacity(1.0f);   // 行全体は下げない（OKを明るく保つため）
}

void UWG_MediaItemRow::UpdateNGState()
{
	using namespace MediaRowUI;

	const FString Cur = NameInput ? NameInput->GetText().ToString() : MediaName;
	bIsNG = Cur.EndsWith(TEXT("_NG"));

	// NG/OK ボタン表示
	if (NGButtonText && NGButton)
	{
		NGButtonText->SetText(FText::FromString(bIsNG ? TEXT("OK") : TEXT("NG")));
		NGButton->SetBackgroundColor(bIsNG ? OKGreen : NGRed);
	}

	// NGメディアはミュート（不要なので再生もしない）
	if (Owner && Item)
	{
		Owner->SetMediaItemMute(Item, bIsNG);
	}

	RefreshRowVisual();
}

void UWG_MediaItemRow::HandleSelectClicked()
{
	if (!Owner) return;
	if (bIsLocked) return;
	if (bIsNG) return;   // NG(ミュート)メディアは再生しない

	if (bRowPlaying)
	{
		// 再生中 → 停止
		Owner->StopMedia();
	}
	else
	{
		// そのメディア先頭へカーソル移動＋即再生
		Owner->PlayFromMediaRow(this, Item, StartTime);
	}
}

void UWG_MediaItemRow::HandleNGClicked()
{
	if (!NameInput) return;

	FString Cur = NameInput->GetText().ToString();
	if (Cur.EndsWith(TEXT("_NG")))
	{
		// 既に _NG → 削除（OK化）
		Cur = Cur.LeftChop(3);
	}
	else
	{
		// _NG を付与
		Cur += TEXT("_NG");
	}

	NameInput->SetText(FText::FromString(Cur));
	CommitName();
	UpdateNGState();   // NG/OK・ミュート・グレーアウトを反映
}

void UWG_MediaItemRow::HandleNameCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter || CommitMethod == ETextCommit::OnUserMovedFocus)
	{
		CommitName();
		UpdateNGState();   // 手入力で _NG を付けた場合もミュート/表示に追従
	}
}

void UWG_MediaItemRow::CommitName()
{
	if (!NameInput) return;
	const FString NewName = NameInput->GetText().ToString();
	if (NewName == MediaName) return;

	MediaName = NewName;
	if (Owner && Item)
	{
		Owner->RenameMediaItem(Item, NewName);
	}
}
