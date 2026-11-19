// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "Actions/WG_IRMeasurementDialog.h"
#include "ReaperDialogStyle.h"
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
#include "Components/ComboBoxString.h"
#include "Components/SizeBox.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Brushes/SlateColorBrush.h"

namespace IRDlgUI
{
	using namespace ReaperDialogStyle;

	/** ＋／－ボタンの面（ダイアログの他のボタンより暗い、押せる面の色）。 */
	static const FLinearColor StepBtnBg = FLinearColor(0.075f, 0.185f, 0.205f, 1.f);

	/** ＋／－行の最低の高さ（指で押せる大きさ。中身が大きければ行はこれより伸びる）。 */
	static constexpr float StepRowMinHeight = 72.f;

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
}

void UWG_IRMeasurementDialog::Setup(FOnOk InOnOk, FOnCancel InOnCancel)
{
	OnOk = MoveTemp(InOnOk);
	OnCancel = MoveTemp(InOnCancel);
}

TSharedRef<SWidget> UWG_IRMeasurementDialog::RebuildWidget()
{
	if (WidgetTree && !bUICreated)
	{
		BuildUI();
		bUICreated = true;
	}
	return Super::RebuildWidget();
}

void UWG_IRMeasurementDialog::NativeConstruct()
{
	Super::NativeConstruct();

	if (OkButton && !OkButton->OnClicked.IsAlreadyBound(this, &UWG_IRMeasurementDialog::HandleOk))
		OkButton->OnClicked.AddDynamic(this, &UWG_IRMeasurementDialog::HandleOk);
	if (CancelButton && !CancelButton->OnClicked.IsAlreadyBound(this, &UWG_IRMeasurementDialog::HandleCancel))
		CancelButton->OnClicked.AddDynamic(this, &UWG_IRMeasurementDialog::HandleCancel);

	if (RepeatCountMinusButton && !RepeatCountMinusButton->OnClicked.IsAlreadyBound(this, &UWG_IRMeasurementDialog::HandleRepeatMinus))
		RepeatCountMinusButton->OnClicked.AddDynamic(this, &UWG_IRMeasurementDialog::HandleRepeatMinus);
	if (RepeatCountPlusButton && !RepeatCountPlusButton->OnClicked.IsAlreadyBound(this, &UWG_IRMeasurementDialog::HandleRepeatPlus))
		RepeatCountPlusButton->OnClicked.AddDynamic(this, &UWG_IRMeasurementDialog::HandleRepeatPlus);
	if (NumChannelsMinusButton && !NumChannelsMinusButton->OnClicked.IsAlreadyBound(this, &UWG_IRMeasurementDialog::HandleChannelsMinus))
		NumChannelsMinusButton->OnClicked.AddDynamic(this, &UWG_IRMeasurementDialog::HandleChannelsMinus);
	if (NumChannelsPlusButton && !NumChannelsPlusButton->OnClicked.IsAlreadyBound(this, &UWG_IRMeasurementDialog::HandleChannelsPlus))
		NumChannelsPlusButton->OnClicked.AddDynamic(this, &UWG_IRMeasurementDialog::HandleChannelsPlus);

	if (IntervalSlider && !IntervalSlider->OnValueChanged.IsAlreadyBound(this, &UWG_IRMeasurementDialog::HandleIntervalChanged))
		IntervalSlider->OnValueChanged.AddDynamic(this, &UWG_IRMeasurementDialog::HandleIntervalChanged);
}

UWidget* UWG_IRMeasurementDialog::MakeStepperRow(const FString& Label, FName MinusName, FName PlusName,
	TObjectPtr<UButton>& OutMinus, TObjectPtr<UButton>& OutPlus, TObjectPtr<UTextBlock>& OutValue)
{
	using namespace IRDlgUI;

	UVerticalBox* Col = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());

	// 見出し
	{
		UTextBlock* L = MakeText(WidgetTree, Label, 18, TextDim, true);
		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(L);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 4.f));
	}

	// 「－　値　＋」
	UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

	auto MakeStepButton = [&](const FString& Glyph, FName Name) -> UWidget*
	{
		UButton* Btn = MakeButton(WidgetTree, Glyph, StepBtnBg, Name, 34);
		if (UTextBlock* Txt = Cast<UTextBlock>(Btn->GetContent()))
		{
			Txt->SetColorAndOpacity(FSlateColor(NeonEdge));   // 面は暗いので字をティールで光らせる
			Txt->SetAutoWrapText(false);
		}
		// 高さは SetHeightOverride で固定しない。M PLUS 1 は行の高さがフォントサイズより
		// かなり大きいので、固定するとテキストが枠に収まらず下へはみ出して「中央からずれて」見える
		// （ホームの＋／－も SetMinDesiredHeight で組んである）。
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetWidthOverride(104.f);
		B->SetMinDesiredHeight(StepRowMinHeight);
		B->AddChild(Btn);
		if (Name == MinusName) { OutMinus = Btn; } else { OutPlus = Btn; }
		return B;
	};

	// 3つのセルは行の高さいっぱいに伸ばす（VAlign_Fill）。
	// 中央寄せにすると、いちばん高いセル（数字）だけ背景が高くなって段差が出る。
	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(MakeStepButton(TEXT("－"), MinusName));
		S->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		S->SetVerticalAlignment(VAlign_Fill);
	}

	// 現在値（一段沈めた面に大きく出す。角丸は＋／－ボタンと同じ）
	{
		UBorder* ValuePanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
		ValuePanel->SetBrush(FSlateRoundedBoxBrush(SunkenBg, 10.f));
		ValuePanel->SetBrushColor(FLinearColor::White);   // ブラシ側の色をそのまま出す
		ValuePanel->SetPadding(FMargin(8.f, 6.f));
		ValuePanel->SetHorizontalAlignment(HAlign_Center);
		ValuePanel->SetVerticalAlignment(VAlign_Center);

		OutValue = MakeText(WidgetTree, TEXT("1"), 36, TextPrimary, true);
		OutValue->SetJustification(ETextJustify::Center);
		ValuePanel->SetContent(OutValue);

		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredHeight(StepRowMinHeight);
		B->AddChild(ValuePanel);

		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		S->SetPadding(FMargin(12.f, 0.f, 12.f, 0.f));
		S->SetVerticalAlignment(VAlign_Fill);
	}

	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(MakeStepButton(TEXT("＋"), PlusName));
		S->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		S->SetVerticalAlignment(VAlign_Fill);
	}

	{
		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(Row);
		S->SetHorizontalAlignment(HAlign_Fill);
	}

	return Col;
}

void UWG_IRMeasurementDialog::BuildUI()
{
	using namespace IRDlgUI;

	UVerticalBox* BodyCol = BuildFrame(this, ReaperLang::S(TEXT("IR計測準備"), TEXT("Prepare IR Measurement")), 720.f);

	auto AddField = [&](UWidget* Field, float BottomPad)
	{
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(Field);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, BottomPad));
	};
	auto AddLabel = [&](const FString& Label)
	{
		UTextBlock* L = MakeText(WidgetTree, Label, 18, TextDim, true);
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(L);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 4.f));
	};

	// 計測回数（TSP の「掃引→無音」を繰り返す回数）
	AddField(MakeStepperRow(FString::Printf(TEXT("%s (%d-%d)"),
			*ReaperLang::S(TEXT("計測回数"), TEXT("Measurement count")), MinRepeatCount, MaxRepeatCount),
		TEXT("RepeatCountMinusButton"), TEXT("RepeatCountPlusButton"),
		RepeatCountMinusButton, RepeatCountPlusButton, RepeatCountValueText), 16.f);

	// チャンネル数（録音トラック「ch. n」の本数）
	AddField(MakeStepperRow(FString::Printf(TEXT("%s (%d-%d)"),
			*ReaperLang::S(TEXT("チャンネル数"), TEXT("Number of channels")), MinNumChannels, MaxNumChannels),
		TEXT("NumChannelsMinusButton"), TEXT("NumChannelsPlusButton"),
		NumChannelsMinusButton, NumChannelsPlusButton, NumChannelsValueText), 16.f);

	UpdateCountUI();

	// TSPの種類（0=対数 / 1=線形）
	AddLabel(ReaperLang::S(TEXT("TSPの種類"), TEXT("TSP type")));
	TspTypeCombo = MakeDarkCombo(WidgetTree);
	TspTypeCombo->OnGenerateWidgetEvent.BindDynamic(this, &UWG_IRMeasurementDialog::MakeComboItem);
	TspTypeCombo->AddOption(ReaperLang::S(TEXT("対数"), TEXT("Logarithmic")));
	TspTypeCombo->AddOption(ReaperLang::S(TEXT("線形"), TEXT("Linear")));
	TspTypeCombo->SetSelectedIndex(0);
	AddField(TspTypeCombo, 16.f);

	// 再生の間隔（掃引と掃引の間に入れる無音の秒数）。見出しの右端に現在値を出す。
	{
		UHorizontalBox* LabelRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		{
			UTextBlock* L = MakeText(WidgetTree, FString::Printf(TEXT("%s (%d-%d%s)"),
				*ReaperLang::S(TEXT("再生の間隔"), TEXT("Playback interval")),
				MinIntervalSec, MaxIntervalSec, *ReaperLang::S(TEXT("秒"), TEXT(" s"))), 18, TextDim, true);
			UHorizontalBoxSlot* S = LabelRow->AddChildToHorizontalBox(L);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetVerticalAlignment(VAlign_Center);
		}
		IntervalValueText = MakeText(WidgetTree, TEXT(""), 24, TextPrimary, true);
		{
			UHorizontalBoxSlot* S = LabelRow->AddChildToHorizontalBox(IntervalValueText);
			S->SetHorizontalAlignment(HAlign_Right);
			S->SetVerticalAlignment(VAlign_Center);
		}
		{
			UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(LabelRow);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetPadding(FMargin(2.f, 0.f, 2.f, 4.f));
		}

		// スライダー本体：バーは透明にし、丸いつまみだけを見せる（ホームの間隔スライダーと同じ見た目）。
		IntervalSlider = WidgetTree->ConstructWidget<USlider>(USlider::StaticClass(), TEXT("IntervalSlider"));
		IntervalSlider->SetMinValue((float)MinIntervalSec);
		IntervalSlider->SetMaxValue((float)MaxIntervalSec);
		IntervalSlider->SetStepSize(1.f);
		IntervalSlider->MouseUsesStep = true;   // ドラッグも1秒刻みにスナップ
		IntervalSlider->SetValue((float)IntervalSeconds);
		{
			FSliderStyle SS = IntervalSlider->WidgetStyle;
			const FSlateColorBrush ClearBrush(FLinearColor(0.f, 0.f, 0.f, 0.f));
			SS.NormalBarImage   = ClearBrush;
			SS.HoveredBarImage  = ClearBrush;
			SS.DisabledBarImage = ClearBrush;
			const FLinearColor ThumbEdge(0.70f, 1.0f, 0.95f, 1.f);
			SS.NormalThumbImage   = FSlateRoundedBoxBrush(NeonEdge,                              15.f, ThumbEdge, 2.5f, FVector2D(30.f, 30.f));
			SS.HoveredThumbImage  = FSlateRoundedBoxBrush(FLinearColor(0.26f, 0.94f, 0.87f, 1.f), 15.f, ThumbEdge, 2.5f, FVector2D(30.f, 30.f));
			SS.DisabledThumbImage = FSlateRoundedBoxBrush(FLinearColor(0.40f, 0.45f, 0.46f, 1.f), 15.f, FVector2D(30.f, 30.f));
			SS.BarThickness = 14.f;
			IntervalSlider->WidgetStyle = SS;
		}

		IntervalFill = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());
		{
			FProgressBarStyle PS = IntervalFill->WidgetStyle;
			PS.BackgroundImage = FSlateRoundedBoxBrush(SunkenBg, 6.f);
			PS.FillImage       = FSlateRoundedBoxBrush(NeonEdge, 6.f);
			IntervalFill->WidgetStyle = PS;
		}
		IntervalFill->SetPercent(0.f);

		UOverlay* Ov = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
		{
			USizeBox* TrackBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			TrackBox->SetHeightOverride(12.f);
			TrackBox->AddChild(IntervalFill);
			if (UOverlaySlot* O0 = Ov->AddChildToOverlay(TrackBox))
			{
				O0->SetHorizontalAlignment(HAlign_Fill);
				O0->SetVerticalAlignment(VAlign_Center);
			}
		}
		if (UOverlaySlot* O1 = Ov->AddChildToOverlay(IntervalSlider))
		{
			O1->SetHorizontalAlignment(HAlign_Fill);
			O1->SetVerticalAlignment(VAlign_Center);
		}

		USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		SB->SetMinDesiredHeight(48.f);
		SB->AddChild(Ov);
		AddField(SB, 18.f);

		UpdateIntervalUI();
	}

	// ボタン行
	{
		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		CancelButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("キャンセル"), TEXT("Cancel")), StopGray, TEXT("CancelButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(72.f); B->AddChild(CancelButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
		}

		OkButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("OK"), TEXT("OK")), AccentGreen, TEXT("OkButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(72.f); B->AddChild(OkButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}

		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(Row);
		S->SetHorizontalAlignment(HAlign_Fill);
	}
}

void UWG_IRMeasurementDialog::UpdateCountUI()
{
	RepeatCount = FMath::Clamp(RepeatCount, MinRepeatCount, MaxRepeatCount);
	NumChannels = FMath::Clamp(NumChannels, MinNumChannels, MaxNumChannels);

	// 桁区切りを入れたくないので FText::AsNumber は使わない
	if (RepeatCountValueText) { RepeatCountValueText->SetText(FText::FromString(FString::FromInt(RepeatCount))); }
	if (NumChannelsValueText) { NumChannelsValueText->SetText(FText::FromString(FString::FromInt(NumChannels))); }

	// 端に達したボタンは押せなくする（押しても変わらない方向を分かるようにする）
	if (RepeatCountMinusButton) { RepeatCountMinusButton->SetIsEnabled(RepeatCount > MinRepeatCount); }
	if (RepeatCountPlusButton)  { RepeatCountPlusButton->SetIsEnabled(RepeatCount < MaxRepeatCount); }
	if (NumChannelsMinusButton) { NumChannelsMinusButton->SetIsEnabled(NumChannels > MinNumChannels); }
	if (NumChannelsPlusButton)  { NumChannelsPlusButton->SetIsEnabled(NumChannels < MaxNumChannels); }
}

void UWG_IRMeasurementDialog::UpdateIntervalUI()
{
	IntervalSeconds = FMath::Clamp(IntervalSeconds, MinIntervalSec, MaxIntervalSec);

	// つまみ位置は丸めた値に合わせる（差分時のみ。OnValueChanged の再入を避ける）
	if (IntervalSlider && IntervalSlider->GetValue() != (float)IntervalSeconds)
	{
		IntervalSlider->SetValue((float)IntervalSeconds);
	}
	if (IntervalFill)
	{
		const float Range = (float)FMath::Max(1, MaxIntervalSec - MinIntervalSec);
		IntervalFill->SetPercent((float)(IntervalSeconds - MinIntervalSec) / Range);
	}
	if (IntervalValueText)
	{
		IntervalValueText->SetText(FText::FromString(FString::Printf(TEXT("%d%s"),
			IntervalSeconds, *ReaperLang::S(TEXT(" 秒"), TEXT(" s")))));
	}
}

void UWG_IRMeasurementDialog::HandleRepeatMinus()
{
	RepeatCount -= 1;
	UpdateCountUI();
}

void UWG_IRMeasurementDialog::HandleRepeatPlus()
{
	RepeatCount += 1;
	UpdateCountUI();
}

void UWG_IRMeasurementDialog::HandleChannelsMinus()
{
	NumChannels -= 1;
	UpdateCountUI();
}

void UWG_IRMeasurementDialog::HandleChannelsPlus()
{
	NumChannels += 1;
	UpdateCountUI();
}

void UWG_IRMeasurementDialog::HandleIntervalChanged(float NewValue)
{
	IntervalSeconds = FMath::RoundToInt(NewValue);
	UpdateIntervalUI();
}

void UWG_IRMeasurementDialog::HandleOk()
{
	// 値は＋／－とスライダーでしか動かないので、ここで検証するものは無い。
	const int32 OutRepeat   = FMath::Clamp(RepeatCount, MinRepeatCount, MaxRepeatCount);
	const int32 OutChannels = FMath::Clamp(NumChannels, MinNumChannels, MaxNumChannels);
	const bool  bLogTSP     = !TspTypeCombo || TspTypeCombo->GetSelectedIndex() != 1;
	const double IntervalSec = (double)FMath::Clamp(IntervalSeconds, MinIntervalSec, MaxIntervalSec);

	// コールバック内で別ダイアログを開くことがあるため、先に閉じてから呼ぶ
	FOnOk Callback = MoveTemp(OnOk);
	OnCancel = nullptr;
	RemoveFromParent();

	if (Callback) { Callback(OutRepeat, OutChannels, bLogTSP, IntervalSec); }
}

void UWG_IRMeasurementDialog::HandleCancel()
{
	FOnCancel Callback = MoveTemp(OnCancel);
	OnOk = nullptr;
	RemoveFromParent();

	if (Callback) { Callback(); }
}

UWidget* UWG_IRMeasurementDialog::MakeComboItem(FString Item)
{
	using namespace IRDlgUI;
	UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	T->SetText(FText::FromString(Item));
	T->SetFont(ReaperFont::Get(18, false));
	T->SetColorAndOpacity(FSlateColor(FieldText));
	ComboGeneratedItems.Add(T);   // GC回収防止
	return T;
}
