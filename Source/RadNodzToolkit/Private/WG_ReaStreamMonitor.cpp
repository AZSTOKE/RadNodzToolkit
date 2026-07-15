// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#include "WG_ReaStreamMonitor.h"

#include "ReaStreamReceiver.h"
#include "ReaperFont.h"
#include "ReaperLang.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Styling/SlateColor.h"
#include "Brushes/SlateRoundedBoxBrush.h"

// ====================================================================
//  カラー / ヘルパー（本体UIと同系のダークテーマ）
// ====================================================================
namespace ReaStreamMonitorUI
{
	static const FLinearColor ScreenBg    = FLinearColor(0.000f, 0.000f, 0.000f, 1.f);
	static const FLinearColor PanelBg     = FLinearColor(0.018f, 0.032f, 0.038f, 1.f);
	static const FLinearColor AccentBlue  = FLinearColor(0.16f, 0.80f, 0.74f, 1.f);
	static const FLinearColor AccentGreen = FLinearColor(0.13f, 0.76f, 0.58f, 1.f);
	static const FLinearColor StopGray    = FLinearColor(0.10f, 0.16f, 0.18f, 1.f);
	static const FLinearColor OkGreen     = FLinearColor(0.16f, 0.88f, 0.72f, 1.f);
	static const FLinearColor WarnRed     = FLinearColor(0.95f, 0.30f, 0.40f, 1.f);
	static const FLinearColor TextPrimary = FLinearColor(0.88f, 0.95f, 0.96f, 1.f);
	static const FLinearColor FieldBg     = FLinearColor(0.030f, 0.052f, 0.060f, 1.f);
	static const FLinearColor FieldText   = FLinearColor(0.88f, 0.96f, 0.96f, 1.f);
	static const FLinearColor FieldEdge   = FLinearColor(0.16f, 0.42f, 0.44f, 1.f);

	static FLinearColor ReadableTextColor(const FLinearColor& Bg)
	{
		const float Lum = 0.299f * Bg.R + 0.587f * Bg.G + 0.114f * Bg.B;
		return (Lum > 0.45f) ? FLinearColor(0.02f, 0.05f, 0.06f, 1.f) : FLinearColor(0.95f, 0.98f, 0.98f, 1.f);
	}

	static UTextBlock* MakeText(UWidgetTree* Tree, const FString& Str, int32 Size, FLinearColor Color, bool bBold = false)
	{
		UTextBlock* T = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(Str));
		T->SetFont(ReaperFont::Get(Size, bBold));
		T->SetColorAndOpacity(FSlateColor(Color));
		return T;
	}

	static void StyleRoundedButton(UButton* Btn, float Radius = 10.f)
	{
		if (!Btn) return;
		const float OutW = 2.2f;
		const FLinearColor Edge(0.20f, 2.40f, 0.90f, 1.f);
		FButtonStyle Style = Btn->GetStyle();
		Style.Normal   = FSlateRoundedBoxBrush(FLinearColor::White,                    Radius, Edge, OutW);
		Style.Hovered  = FSlateRoundedBoxBrush(FLinearColor(1.12f, 1.12f, 1.12f, 1.f), Radius, Edge, OutW);
		Style.Pressed  = FSlateRoundedBoxBrush(FLinearColor(0.85f, 0.85f, 0.85f, 1.f), Radius, Edge, OutW);
		Style.Disabled = FSlateRoundedBoxBrush(FLinearColor(0.5f, 0.5f, 0.5f, 1.f),    Radius, Edge, OutW);
		Btn->SetStyle(Style);
	}

	static UButton* MakeButton(UWidgetTree* Tree, const FString& Label, FLinearColor BgColor,
		FName Name, TObjectPtr<UTextBlock>* OutText)
	{
		UButton* Btn = Tree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
		StyleRoundedButton(Btn);
		Btn->SetBackgroundColor(BgColor);
		UTextBlock* Txt = MakeText(Tree, Label, 20, ReadableTextColor(BgColor), true);
		Txt->SetJustification(ETextJustify::Center);
		Txt->SetAutoWrapText(!ReaperLang::IsJapanese());   // 日本語は折り返し不要。長い翻訳文字列だけボタン幅をはみ出さず2行に折り返す
		Btn->SetContent(Txt);
		if (OutText) { *OutText = Txt; }
		return Btn;
	}

	static UEditableTextBox* MakeInput(UWidgetTree* Tree, const FString& Value, FName Name)
	{
		UEditableTextBox* Box = Tree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), Name);
		const float Radius = 10.f;
		FSlateRoundedBoxBrush Normal(FieldBg, Radius, FieldEdge, 1.2f);
		Box->WidgetStyle.SetBackgroundImageNormal(Normal);
		Box->WidgetStyle.SetBackgroundImageHovered(Normal);
		Box->WidgetStyle.SetBackgroundImageFocused(Normal);
		Box->WidgetStyle.SetBackgroundImageReadOnly(Normal);
		Box->WidgetStyle.SetForegroundColor(FSlateColor(FieldText));
		Box->WidgetStyle.Padding = FMargin(16.f, 13.f);
		Box->WidgetStyle.TextStyle.ColorAndOpacity = FSlateColor(FieldText);
		Box->WidgetStyle.TextStyle.Font = ReaperFont::Get(20, false);
		Box->SetText(FText::FromString(Value));
		return Box;
	}

	// 受信バッファ：レベル(0〜2) → 秒
	static float BufferSecondsByLevel(int32 Lv)
	{
		static const float Tbl[3] = { 0.4f, 1.0f, 2.0f };
		return Tbl[FMath::Clamp(Lv, 0, 2)];
	}
}

UWG_ReaStreamMonitor::UWG_ReaStreamMonitor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UWG_ReaStreamMonitor::RebuildWidget()
{
	if (WidgetTree && !bUICreated && !IsDesignTime())
	{
		BuildUI();
		bUICreated = true;
	}
	return Super::RebuildWidget();
}

void UWG_ReaStreamMonitor::BuildUI()
{
	using namespace ReaStreamMonitorUI;

	// 画面全体（黒背景・中央寄せ）
	UBorder* Screen = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Screen"));
	Screen->SetBrushColor(ScreenBg);
	Screen->SetPadding(FMargin(48.f));
	Screen->SetHorizontalAlignment(HAlign_Center);
	Screen->SetVerticalAlignment(VAlign_Center);
	WidgetTree->RootWidget = Screen;

	// 中央のカード
	UBorder* Card = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Card"));
	Card->SetBrushColor(PanelBg);
	Card->SetPadding(FMargin(40.f, 36.f));
	Screen->SetContent(Card);

	USizeBox* CardSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	CardSize->SetMinDesiredWidth(560.f);
	Card->SetContent(CardSize);

	UVerticalBox* Root = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Root"));
	CardSize->AddChild(Root);

	auto AddRow = [&](UWidget* W, float TopPad)
	{
		UVerticalBoxSlot* S = Root->AddChildToVerticalBox(W);
		S->SetPadding(FMargin(0.f, TopPad, 0.f, 0.f));
		S->SetHorizontalAlignment(HAlign_Fill);
	};

	// タイトル
	TitleText = MakeText(WidgetTree, ReaperLang::S(TEXT("ReaStream モニター（受信）"), TEXT("ReaStream Monitor (Receive)")), 28, TextPrimary, true);
	TitleText->SetJustification(ETextJustify::Center);
	AddRow(TitleText, 0.f);

	// ステータス
	StatusText = MakeText(WidgetTree, ReaperLang::S(TEXT("停止中"), TEXT("Stopped")), 22, TextPrimary, true);
	StatusText->SetJustification(ETextJustify::Center);
	AddRow(StatusText, 18.f);

	// ポート行（ラベル＋入力）
	{
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		UTextBlock* Lbl = MakeText(WidgetTree, ReaperLang::S(TEXT("受信ポート"), TEXT("Port")), 22, TextPrimary, true);
		{
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(Lbl);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}
		PortInput = MakeInput(WidgetTree, FString::FromInt(Port), TEXT("PortInput"));
		{
			USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			SB->SetMinDesiredWidth(180.f);
			SB->AddChild(PortInput);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(SB);
			S->SetVerticalAlignment(VAlign_Center);
		}
		AddRow(H, 28.f);
	}

	// 許可する送信元IP行（空欄=全許可。指定するとそのIPからのみ受信）
	{
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		UTextBlock* Lbl = MakeText(WidgetTree, ReaperLang::S(TEXT("許可する送信元IP"), TEXT("Allowed Sender IP")), 22, TextPrimary, true);
		{
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(Lbl);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}
		AllowedIPInput = MakeInput(WidgetTree, TEXT(""), TEXT("AllowedIPInput"));
		AllowedIPInput->SetHintText(ReaperLang::T(TEXT("空欄=全許可"), TEXT("empty = allow all")));
		{
			USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			SB->SetMinDesiredWidth(260.f);
			SB->AddChild(AllowedIPInput);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(SB);
			S->SetVerticalAlignment(VAlign_Center);
		}
		AddRow(H, 16.f);
	}

	// 出力モード行
	{
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		UTextBlock* Lbl = MakeText(WidgetTree, ReaperLang::S(TEXT("出力"), TEXT("Output")), 22, TextPrimary, true);
		{
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(Lbl);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}
		ModeButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("ModeButton"), &ModeButtonText);
		{
			USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			SB->SetMinDesiredWidth(260.f);
			SB->SetMinDesiredHeight(64.f);
			SB->AddChild(ModeButton);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(SB);
			S->SetVerticalAlignment(VAlign_Center);
		}
		AddRow(H, 16.f);
	}

	// 受信バッファ行
	{
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		UTextBlock* Lbl = MakeText(WidgetTree, ReaperLang::S(TEXT("受信バッファ"), TEXT("Receive Buffer")), 22, TextPrimary, true);
		{
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(Lbl);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}
		BufferButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("BufferButton"), &BufferButtonText);
		{
			USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			SB->SetMinDesiredWidth(200.f);
			SB->SetMinDesiredHeight(64.f);
			SB->AddChild(BufferButton);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(SB);
			S->SetVerticalAlignment(VAlign_Center);
		}
		AddRow(H, 16.f);
	}

	// 開始/停止ボタン（大）
	{
		StartStopButton = MakeButton(WidgetTree, TEXT(""), AccentGreen, TEXT("StartStopButton"), &StartStopButtonText);
		USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		SB->SetMinDesiredHeight(84.f);
		SB->AddChild(StartStopButton);
		AddRow(SB, 32.f);
	}

	UpdateModeButton();
	UpdateBufferButton();
	UpdateStartButton();
	SetStatus(ReaperLang::S(TEXT("停止中"), TEXT("Stopped")), TextPrimary);
}

void UWG_ReaStreamMonitor::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartStopButton && !StartStopButton->OnClicked.IsAlreadyBound(this, &UWG_ReaStreamMonitor::HandleStartStopClicked))
	{
		StartStopButton->OnClicked.AddDynamic(this, &UWG_ReaStreamMonitor::HandleStartStopClicked);
	}
	if (ModeButton && !ModeButton->OnClicked.IsAlreadyBound(this, &UWG_ReaStreamMonitor::HandleModeClicked))
	{
		ModeButton->OnClicked.AddDynamic(this, &UWG_ReaStreamMonitor::HandleModeClicked);
	}
	if (BufferButton && !BufferButton->OnClicked.IsAlreadyBound(this, &UWG_ReaStreamMonitor::HandleBufferClicked))
	{
		BufferButton->OnClicked.AddDynamic(this, &UWG_ReaStreamMonitor::HandleBufferClicked);
	}
	if (AllowedIPInput && !AllowedIPInput->OnTextCommitted.IsAlreadyBound(this, &UWG_ReaStreamMonitor::HandleAllowedIPCommitted))
	{
		AllowedIPInput->OnTextCommitted.AddDynamic(this, &UWG_ReaStreamMonitor::HandleAllowedIPCommitted);
	}
}

void UWG_ReaStreamMonitor::NativeDestruct()
{
	if (Receiver)
	{
		Receiver->Stop();
	}
	Super::NativeDestruct();
}

void UWG_ReaStreamMonitor::ApplyReceiverConfig()
{
	using namespace ReaStreamMonitorUI;

	if (!Receiver) return;
	Receiver->SetOutputMode(OutputMode);
	// 立体音響の既定パラメータ（中）。ステレオダウンミックス係数は受信機の既定値を使用。
	Receiver->SetBinauralParams(0.65f, 0.35f, EBinauralChannelOrder::ReaperITU);
	Receiver->SetBufferSeconds(ReaStreamMonitorUI::BufferSecondsByLevel(BufferLevel));
	// 送信元IP制限（空欄=全許可）
	Receiver->SetAllowedSenderIP(AllowedIPInput ? AllowedIPInput->GetText().ToString().TrimStartAndEnd() : FString());
}

void UWG_ReaStreamMonitor::HandleAllowedIPCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 受信中でも即時に反映（フィルタはアトミックでスレッドセーフ）
	if (Receiver)
	{
		Receiver->SetAllowedSenderIP(Text.ToString().TrimStartAndEnd());
	}
}

void UWG_ReaStreamMonitor::RestartIfActive()
{
	if (Receiver && Receiver->IsActive())
	{
		Receiver->Stop();
		ApplyReceiverConfig();
		Receiver->Start(this, Port);
	}
}

void UWG_ReaStreamMonitor::HandleStartStopClicked()
{
	using namespace ReaStreamMonitorUI;

	if (!Receiver)
	{
		Receiver = NewObject<UReaStreamReceiver>(this);
	}

	if (Receiver->IsActive())
	{
		Receiver->Stop();
		SetStatus(ReaperLang::S(TEXT("停止中"), TEXT("Stopped")), TextPrimary);
	}
	else
	{
		// ポート欄を反映
		if (PortInput)
		{
			const int32 P = FCString::Atoi(*PortInput->GetText().ToString());
			if (P > 0 && P < 65536) { Port = P; }
		}
		ApplyReceiverConfig();
		const bool bOk = Receiver->Start(this, Port);
		if (bOk)
		{
			SetStatus(FString::Printf(TEXT("%s : %d"),
				*ReaperLang::S(TEXT("受信中"), TEXT("Receiving")), Port), OkGreen);
		}
		else
		{
			SetStatus(ReaperLang::S(TEXT("受信開始に失敗しました"), TEXT("Failed to start")), WarnRed);
		}
	}
	UpdateStartButton();
}

void UWG_ReaStreamMonitor::HandleModeClicked()
{
	// そのまま(サラウンド) → 立体音響 → ステレオ → …
	switch (OutputMode)
	{
	case EMonitorOutputMode::Passthrough: OutputMode = EMonitorOutputMode::Spatial;     break;
	case EMonitorOutputMode::Spatial:     OutputMode = EMonitorOutputMode::Stereo;      break;
	default:                              OutputMode = EMonitorOutputMode::Passthrough; break;
	}
	UpdateModeButton();
	RestartIfActive();
}

void UWG_ReaStreamMonitor::HandleBufferClicked()
{
	BufferLevel = (BufferLevel + 1) % 3;
	UpdateBufferButton();
	RestartIfActive();
}

void UWG_ReaStreamMonitor::UpdateModeButton()
{
	using namespace ReaStreamMonitorUI;

	if (!ModeButton || !ModeButtonText) return;

	FText Label;
	FLinearColor Bg = AccentBlue;
	switch (OutputMode)
	{
	case EMonitorOutputMode::Spatial:
		Label = ReaperLang::T(TEXT("立体音響"), TEXT("3D Spatial"));
		Bg = AccentGreen;
		break;
	case EMonitorOutputMode::Stereo:
		Label = ReaperLang::T(TEXT("ステレオ"), TEXT("Stereo"));
		Bg = AccentBlue;
		break;
	default: // Passthrough
		Label = ReaperLang::T(TEXT("そのまま（サラウンド）"), TEXT("As-is (Surround)"));
		Bg = AccentBlue;
		break;
	}
	ModeButton->SetBackgroundColor(Bg);
	ModeButtonText->SetText(Label);
	ModeButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
}

void UWG_ReaStreamMonitor::UpdateBufferButton()
{
	using namespace ReaStreamMonitorUI;

	if (!BufferButton || !BufferButtonText) return;

	FText Label;
	switch (FMath::Clamp(BufferLevel, 0, 2))
	{
	case 0:  Label = ReaperLang::T(TEXT("低遅延"), TEXT("Low Latency")); break;
	case 2:  Label = ReaperLang::T(TEXT("安定"),   TEXT("Stable"));      break;
	default: Label = ReaperLang::T(TEXT("標準"),   TEXT("Normal"));      break;
	}
	BufferButton->SetBackgroundColor(AccentBlue);
	BufferButtonText->SetText(Label);
	BufferButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(AccentBlue)));
}

void UWG_ReaStreamMonitor::UpdateStartButton()
{
	using namespace ReaStreamMonitorUI;

	if (!StartStopButton || !StartStopButtonText) return;

	const bool bActive = (Receiver && Receiver->IsActive());
	const FLinearColor Bg = bActive ? WarnRed : AccentGreen;
	StartStopButton->SetBackgroundColor(Bg);
	StartStopButtonText->SetText(bActive
		? ReaperLang::T(TEXT("停止"), TEXT("Stop"))
		: ReaperLang::T(TEXT("受信開始"), TEXT("Start")));
	StartStopButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
}

void UWG_ReaStreamMonitor::SetStatus(const FString& Message, const FLinearColor& Color)
{
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(Message));
		StatusText->SetColorAndOpacity(FSlateColor(Color));
	}
}
