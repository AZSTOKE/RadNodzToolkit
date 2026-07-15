// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#include "WG_RadNodzToolkit.h"
#include "WG_TrackCard.h"
#include "WG_AddTracksDialog.h"
#include "WG_DeleteConfirmDialog.h"
#include "WG_UnlockConfirmDialog.h"
#include "ReaStreamGroupReceiver.h"
#include "WG_ListRow.h"
#include "WG_SheetTab.h"
#include "WG_ColumnPickerButton.h"
#include "WG_SlackRequiredDialog.h"
#include "RigdocksGold_Excel.h"
#include "RigdocksGold_GSheet.h"
#include "GoldClasses.h"
#include "RigdocksSilver_File.h"
#include "RigdocksSilver_Device.h"
#include "RigdocksSilver_Project.h"   // AZ_SetResoucePathFolder
#include "ReaperClasses.h"   // UVarType
#include "WG_MicSettingsDialog.h"
#include "WG_AddServerIdDialog.h"
#include "WG_ConfirmDialog.h"
#include "WG_ReaperScanDialog.h"
#include "WG_MediaItemRow.h"
#include "RigdocksBronze_MediaItem.h"
#include "RigdocksSilver_Loudness.h"
#include "RigdocksSilver_Peak.h"
#include "RigdocksSilver_Slack.h"
#include "RigdocksSilver_TRSC.h"
#include "RigdocksSilver_String.h"
#include "RadnodzClient.h"   // URadnodzClient::SendTimingProbe / GetLast*Time（通信時間の計測）

// RpcBatch: 複数の同期RPCを1往復にまとめて通信回数を減らす（携帯回線対策）。
#include "RadnodzRpcBatch.h"                     // URigdocksRpcCall / URadnodzRpcBatch
#include "Build/RigdocksSilver_Device_Build.h"
#include "Parse/RigdocksSilver_Device_Parse.h"
#include "Build/RigdocksSilver_Track_Build.h"
#include "Parse/RigdocksSilver_Track_Parse.h"
#include "Build/RigdocksSilver_Project_Build.h"
#include "Parse/RigdocksSilver_Project_Parse.h"
#include "Build/RigdocksBronze_MediaItem_Build.h"
#include "Parse/RigdocksBronze_MediaItem_Parse.h"
#include "Build/RigdocksSilver_String_Build.h"
#include "Parse/RigdocksSilver_String_Parse.h"
#include "Build/RigdocksBronze_Cursor_Build.h"
#include "Build/RigdocksGold_GSheet_Build.h"
#include "Build/RigdocksGold_Excel_Build.h"
#include "DataClass/FPeak.h"
#include "ReaperSaveGame.h"
#include "ReaperFont.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"
#include "HAL/PlatformApplicationMisc.h"   // FPlatformApplicationMisc::ClipboardCopy
#include "Misc/Paths.h"
#include "ReaperLang.h"
#include "ReaStreamReceiver.h"
#include "HRTF/HRTFRegistry.h"
#include "Components/ComboBoxString.h"
#include "ReaStreamSender.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#if PLATFORM_ANDROID
#include "AndroidPermissionFunctionLibrary.h"
#endif

#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformProcess.h"   // FPlatformProcess::ComputerName（自端末名）
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WidgetSwitcherSlot.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/Texture2D.h"
#include "Styling/CoreStyle.h"
#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Input/Events.h"
#include "Async/Async.h"

// 列番号(1始まり)⇔Excelの列文字(A,B,...,AA) の相互変換。定義は本ファイル後方（リスト読み込み処理の近く）。
static FString ExcelColumnLetter(int32 Col);
static int32 ExcelColumnLetterToNumber(const FString& Letters);

// ====================================================================
//  カラーパレット（ダークテーマ）
// ====================================================================
namespace ReaperCtrlUI
{
	// === ブラック×ティール テーマ ===
	// 背景4色はテーマ切替（設定）で書き換えるため非const。既定値＝従来テーマ（0:標準）。
	static FLinearColor ScreenBg     = FLinearColor(0.000f, 0.000f, 0.000f, 1.f);  // 黒背景
	static FLinearColor HeaderBg     = FLinearColor(0.022f, 0.040f, 0.046f, 1.f);
	static FLinearColor PanelBg      = FLinearColor(0.018f, 0.032f, 0.038f, 1.f);
	static FLinearColor ListBg       = FLinearColor(0.010f, 0.020f, 0.024f, 1.f);

	// 背景の明るさテーマを ScreenBg/HeaderBg/PanelBg/ListBg へ適用する。
	// 0=標準（従来の黒） / 1=さらに暗め（ほぼ真っ黒） / 2=明るめ。アクセント色は変えない。
	static void ApplyThemeColors(int32 ThemeIdx)
	{
		switch (ThemeIdx)
		{
		case 1: // さらに暗め
			ScreenBg = FLinearColor(0.000f, 0.000f, 0.000f, 1.f);
			HeaderBg = FLinearColor(0.008f, 0.014f, 0.016f, 1.f);
			PanelBg  = FLinearColor(0.006f, 0.011f, 0.013f, 1.f);
			ListBg   = FLinearColor(0.003f, 0.006f, 0.008f, 1.f);
			break;
		case 2: // 明るめ
			ScreenBg = FLinearColor(0.020f, 0.030f, 0.034f, 1.f);
			HeaderBg = FLinearColor(0.060f, 0.090f, 0.100f, 1.f);
			PanelBg  = FLinearColor(0.050f, 0.078f, 0.088f, 1.f);
			ListBg   = FLinearColor(0.035f, 0.058f, 0.066f, 1.f);
			break;
		default: // 0 = 標準（従来）
			ScreenBg = FLinearColor(0.000f, 0.000f, 0.000f, 1.f);
			HeaderBg = FLinearColor(0.022f, 0.040f, 0.046f, 1.f);
			PanelBg  = FLinearColor(0.018f, 0.032f, 0.038f, 1.f);
			ListBg   = FLinearColor(0.010f, 0.020f, 0.024f, 1.f);
			break;
		}
	}

	static const FLinearColor AccentBlue   = FLinearColor(0.16f, 0.80f, 0.74f, 1.f);  // ティール（主アクセント）
	static const FLinearColor AccentGreen  = FLinearColor(0.13f, 0.76f, 0.58f, 1.f);  // ティールグリーン
	static const FLinearColor RecRed       = FLinearColor(0.78f, 0.10f, 0.16f, 1.f);  // 濃いめの赤（RECボタン背景・タイトル"RED"）
	static const FLinearColor StopGray     = FLinearColor(0.10f, 0.16f, 0.18f, 1.f);
	static const FLinearColor LockOrange   = FLinearColor(0.90f, 0.50f, 0.08f, 1.f);  // ロック中の強調色（RECの赤と混同しないよう橙）

	// 白アイコンを載せるボタン用の濃いめ色（明るすぎてアイコンが見えない対策）
	static const FLinearColor BtnBlueDark  = FLinearColor(0.05f, 0.30f, 0.30f, 1.f);  // 濃いティール
	static const FLinearColor BtnGreenDark = FLinearColor(0.05f, 0.28f, 0.21f, 1.f);  // 濃いティールグリーン

	static const FLinearColor TextPrimary  = FLinearColor(0.88f, 0.95f, 0.96f, 1.f);
	static const FLinearColor TextDim      = FLinearColor(0.45f, 0.62f, 0.65f, 1.f);
	static const FLinearColor OkGreen      = FLinearColor(0.16f, 0.88f, 0.72f, 1.f);
	static const FLinearColor WarnRed      = FLinearColor(0.95f, 0.30f, 0.40f, 1.f);

	static const FLinearColor FieldBg      = FLinearColor(0.030f, 0.052f, 0.060f, 1.f);
	static const FLinearColor FieldText    = FLinearColor(0.88f, 0.96f, 0.96f, 1.f);

	// 縁取り/区切り線に使うティールのアクセント
	static const FLinearColor NeonEdge     = FLinearColor(0.16f, 0.80f, 0.74f, 1.f);
	// 入力欄の枠線（控えめなティール）
	static const FLinearColor FieldOutline = FLinearColor(0.16f, 0.42f, 0.44f, 1.f);

	// 明るさから読みやすい文字色（黒 or 白）を選ぶ
	static FLinearColor ReadableTextColor(const FLinearColor& Bg)
	{
		const float Lum = 0.299f * Bg.R + 0.587f * Bg.G + 0.114f * Bg.B;
		return (Lum > 0.45f) ? FLinearColor(0.02f, 0.05f, 0.06f, 1.f) : FLinearColor(0.95f, 0.98f, 0.98f, 1.f);
	}

	// 入力ボックスを「暗い背景＋明るい文字＋角丸＋ティール枠線」に整える（Android向けに大きめ）
	static void StyleDarkInput(UEditableTextBox* Box)
	{
		if (!Box) return;

		const float Radius = 10.f;
		FSlateRoundedBoxBrush Normal(FieldBg, Radius, FieldOutline, 1.2f);
		FSlateRoundedBoxBrush Focused(FieldBg, Radius, NeonEdge, 1.6f); // フォーカス時は明るいティール枠
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

	// ボタンを角丸（白ブラシ＋Tintで動的に色付け）にするヘルパー。
	// 周囲にネオンの縁取り（HDR気味で明るい＝発光しているサイバー感）を付ける。
	static void StyleRoundedButton(UButton* Btn, float Radius = 10.f)
	{
		if (!Btn) return;

		// アウトライン色はTint(ボタン色)で乗算される。緑を強く・赤青を抑えた値にして、
		// どのボタン色でも“緑に光る縁”になるようにする（1.0超でHDR気味に発光感）。
		const float OutW = 2.2f;
		const FLinearColor Edge      (0.20f, 2.40f, 0.90f, 1.f);
		const FLinearColor EdgeHover (0.30f, 3.20f, 1.20f, 1.f);
		const FLinearColor EdgePress (0.15f, 1.60f, 0.60f, 1.f);
		const FLinearColor EdgeDis   (0.20f, 0.60f, 0.35f, 1.f);

		FButtonStyle Style = Btn->GetStyle();
		Style.Normal   = FSlateRoundedBoxBrush(FLinearColor::White,                  Radius, Edge,      OutW);
		Style.Hovered  = FSlateRoundedBoxBrush(FLinearColor(1.12f, 1.12f, 1.12f, 1.f), Radius, EdgeHover, OutW);
		Style.Pressed  = FSlateRoundedBoxBrush(FLinearColor(0.85f, 0.85f, 0.85f, 1.f), Radius, EdgePress, OutW);
		Style.Disabled = FSlateRoundedBoxBrush(FLinearColor(0.5f, 0.5f, 0.5f, 1.f),    Radius, EdgeDis,   OutW);
		Btn->SetStyle(Style);
	}

	// テキストブロック生成ヘルパー
	static UTextBlock* MakeText(UWidgetTree* Tree, const FString& Str, int32 Size, FLinearColor Color, bool bBold = false)
	{
		UTextBlock* T = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(Str));
		T->SetFont(ReaperFont::Get(Size, bBold));
		T->SetColorAndOpacity(FSlateColor(Color));
		return T;
	}

	// ボタン生成ヘルパー（テキスト入り・高さ固定／任意でボタン内アイコン）
	// bIconOnTop＝trueの場合、[アイコン]の下に[テキスト]を縦に並べ、両方中央寄せにする
	// （既定は横並び。削除/全解除/計測調整/Slack/ロックの5ボタンのみ縦並びを使う）。
	static UButton* MakeButton(UWidgetTree* Tree, const FString& Label, FLinearColor BgColor,
		FName Name, TObjectPtr<UTextBlock>* OutText = nullptr, const TCHAR* IconPath = nullptr,
		TObjectPtr<UImage>* OutIcon = nullptr, bool bIconOnTop = false)
	{
		UButton* Btn = Tree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
		StyleRoundedButton(Btn);           // 角丸（白ブラシ）にしてから
		Btn->SetBackgroundColor(BgColor);  // Tintで色付け

		// 背景の明るさに応じて文字色を変える（背景と文字が近くて見えにくいのを防ぐ）
		UTextBlock* Txt = MakeText(Tree, Label, 18, ReadableTextColor(BgColor), true);
		Txt->SetJustification(ETextJustify::Center);
		Txt->SetAutoWrapText(true);   // 長い翻訳文字列（仏語/伊語等）がボタン幅をはみ出さず2行に折り返す

		UTexture2D* IconTex = IconPath ? LoadObject<UTexture2D>(nullptr, IconPath) : nullptr;
		if (IconTex && bIconOnTop)
		{
			// ボタン内に [アイコン]／[テキスト] を縦に並べ、両方中央寄せにする
			UVerticalBox* Box = Tree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
			UImage* Img = Tree->ConstructWidget<UImage>(UImage::StaticClass());
			Img->SetBrushFromTexture(IconTex);
			Img->SetDesiredSizeOverride(FVector2D(30.f, 30.f));
			{
				UVerticalBoxSlot* S = Box->AddChildToVerticalBox(Img);
				S->SetHorizontalAlignment(HAlign_Center);
				S->SetPadding(FMargin(0.f, 0.f, 0.f, 4.f));
			}
			{
				UVerticalBoxSlot* S = Box->AddChildToVerticalBox(Txt);
				S->SetHorizontalAlignment(HAlign_Center);
			}
			Btn->SetContent(Box);
			if (OutIcon) { *OutIcon = Img; }
		}
		else if (IconTex)
		{
			// ボタン内に [アイコン][テキスト] を中央寄せで並べる
			UHorizontalBox* Box = Tree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			{ UTextBlock* Sp = MakeText(Tree, TEXT(""), 1, BgColor); UHorizontalBoxSlot* S = Box->AddChildToHorizontalBox(Sp); S->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); }
			UImage* Img = Tree->ConstructWidget<UImage>(UImage::StaticClass());
			{
				Img->SetBrushFromTexture(IconTex);
				Img->SetDesiredSizeOverride(FVector2D(38.f, 38.f));
				UHorizontalBoxSlot* S = Box->AddChildToHorizontalBox(Img);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));
			}
			{ UHorizontalBoxSlot* S = Box->AddChildToHorizontalBox(Txt); S->SetVerticalAlignment(VAlign_Center); }
			{ UTextBlock* Sp = MakeText(Tree, TEXT(""), 1, BgColor); UHorizontalBoxSlot* S = Box->AddChildToHorizontalBox(Sp); S->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); }
			Btn->SetContent(Box);
			if (OutIcon) { *OutIcon = Img; }
		}
		else
		{
			Btn->SetContent(Txt);
		}
		if (OutText) { *OutText = Txt; }
		return Btn;
	}

	// アイコン画像生成ヘルパー（テクスチャが無ければ空のまま）
	static UImage* MakeIcon(UWidgetTree* Tree, const TCHAR* TexturePath, float Size)
	{
		UImage* Img = Tree->ConstructWidget<UImage>(UImage::StaticClass());
		if (UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, TexturePath))
		{
			Img->SetBrushFromTexture(Tex);
			Img->SetDesiredSizeOverride(FVector2D(Size, Size));
		}
		return Img;
	}

	// 録音を表す丸アイコンを描画で生成する（テクスチャ不要）。
	// 〇の色は PLAY ボタンの三角（▶）と同じ色に合わせる（AccentGreen 上の可読テキスト色）。
	static UWidget* MakeRecDotIcon(UWidgetTree* Tree, float Size)
	{
		const FLinearColor DotColor = ReadableTextColor(AccentGreen);   // ＝PLAYの三角と同色
		UImage* Dot = Tree->ConstructWidget<UImage>(UImage::StaticClass());
		Dot->SetBrush(FSlateRoundedBoxBrush(DotColor, Size * 0.5f, FVector2D(Size, Size)));
		Dot->SetDesiredSizeOverride(FVector2D(Size, Size));
		return Dot;
	}

	// 名前ごとに見分けやすい色を割り当てる（同じ名前は常に同じ色、名前が違えばばらけるようハッシュで選ぶ）。
	static FLinearColor AvatarColorForName(const FString& Name)
	{
		static const FLinearColor Palette[] = {
			FLinearColor(0.90f, 0.30f, 0.30f, 1.f), // red
			FLinearColor(0.95f, 0.55f, 0.15f, 1.f), // orange
			FLinearColor(0.85f, 0.78f, 0.20f, 1.f), // yellow
			FLinearColor(0.30f, 0.75f, 0.40f, 1.f), // green
			FLinearColor(0.20f, 0.65f, 0.65f, 1.f), // teal
			FLinearColor(0.25f, 0.55f, 0.90f, 1.f), // blue
			FLinearColor(0.55f, 0.40f, 0.90f, 1.f), // purple
			FLinearColor(0.90f, 0.35f, 0.65f, 1.f), // pink
			FLinearColor(0.45f, 0.65f, 0.25f, 1.f), // olive
			FLinearColor(0.35f, 0.45f, 0.85f, 1.f), // indigo
		};
		const uint32 Hash = GetTypeHash(Name);
		return Palette[Hash % UE_ARRAY_COUNT(Palette)];
	}

	// 参加者アバター用：名前の先頭1文字（大文字化。全角文字はそのまま）。
	static FString InitialForName(const FString& Name)
	{
		const FString Trimmed = Name.TrimStartAndEnd();
		return Trimmed.IsEmpty() ? TEXT("?") : Trimmed.Left(1).ToUpper();
	}

	// 参加者を表す〇（頭文字入り・名前ごとに色を変える）を生成する。
	static UWidget* MakeAvatarCircle(UWidgetTree* Tree, const FString& Name, float Size)
	{
		const FLinearColor Bg = AvatarColorForName(Name);

		UOverlay* Ov = Tree->ConstructWidget<UOverlay>(UOverlay::StaticClass());

		UImage* Circle = Tree->ConstructWidget<UImage>(UImage::StaticClass());
		Circle->SetBrush(FSlateRoundedBoxBrush(Bg, Size * 0.5f, FVector2D(Size, Size)));
		Circle->SetDesiredSizeOverride(FVector2D(Size, Size));
		{
			UOverlaySlot* OS = Ov->AddChildToOverlay(Circle);
			OS->SetHorizontalAlignment(HAlign_Fill);
			OS->SetVerticalAlignment(VAlign_Fill);
		}

		UTextBlock* Letter = MakeText(Tree, InitialForName(Name), FMath::RoundToInt(Size * 0.48f), ReadableTextColor(Bg), true);
		Letter->SetJustification(ETextJustify::Center);
		{
			UOverlaySlot* OS = Ov->AddChildToOverlay(Letter);
			OS->SetHorizontalAlignment(HAlign_Center);
			OS->SetVerticalAlignment(VAlign_Center);
		}

		USizeBox* SizeBox = Tree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		SizeBox->SetWidthOverride(Size);
		SizeBox->SetHeightOverride(Size);
		SizeBox->AddChild(Ov);
		return SizeBox;
	}

	// 入力ボックス生成ヘルパー
	static UEditableTextBox* MakeInput(UWidgetTree* Tree, const FString& Hint, FName Name)
	{
		UEditableTextBox* Box = Tree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), Name);
		Box->SetHintText(FText::FromString(Hint));
		StyleDarkInput(Box);
		return Box;
	}

	// 暗テーマ用のドロップダウン（濃い背景＋ティール枠＋M PLUSフォント）を生成する。
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

namespace
{
	// ダウンミックス係数の各受信ch（標準レイアウト L R C LFE Ls Rs Lb Rb）のラベル（日本語 / 英語）。
	static void DmChannelLabel(int32 Index, const TCHAR*& OutJP, const TCHAR*& OutEN)
	{
		static const TCHAR* JP[8] = {
			TEXT("L（フロント左）"), TEXT("R（フロント右）"), TEXT("C（センター）"), TEXT("LFE"),
			TEXT("Ls（サラウンド左）"), TEXT("Rs（サラウンド右）"), TEXT("Lb（リア左）"), TEXT("Rb（リア右）") };
		static const TCHAR* EN[8] = {
			TEXT("L (Front L)"), TEXT("R (Front R)"), TEXT("C (Center)"), TEXT("LFE"),
			TEXT("Ls (Surround L)"), TEXT("Rs (Surround R)"), TEXT("Lb (Rear L)"), TEXT("Rb (Rear R)") };
		const int32 i = FMath::Clamp(Index, 0, 7);
		OutJP = JP[i];
		OutEN = EN[i];
	}
}

UWG_RadNodzToolkit::UWG_RadNodzToolkit(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TrackCardClass = UWG_TrackCard::StaticClass();
	AddDialogClass = UWG_AddTracksDialog::StaticClass();
	MicSettingsDialogClass = UWG_MicSettingsDialog::StaticClass();
	EnsureDmGainsDefaults();
	SpatialChannelOrder = EBinauralChannelOrder::Custom;   // ch順マッピングはカスタム固定
}

TSharedRef<SWidget> UWG_RadNodzToolkit::RebuildWidget()
{
	// 実行時は、BP側にデザイナー配置が残っていても必ずC++のUIで上書きする。
	// デザイン時(UMGエディタのプレビュー)はエディタを乱さないよう手を出さない。
	if (WidgetTree && !bUICreated && !IsDesignTime())
	{
		BuildUI();
		bUICreated = true;
	}
	return Super::RebuildWidget();
}

void UWG_RadNodzToolkit::BuildUI()
{
	using namespace ReaperCtrlUI;

	// 保存済みのテーマ（背景の明るさ）を構築前に読み込んで反映する。
	// （LoadSettings は NativeConstruct で BuildUI の後に走るため、ここで先読みする）
	if (UReaperSaveGame* Save = LoadOrCreateSave())
	{
		UIThemeIndex = FMath::Clamp(Save->UIThemeIndex, 0, 2);
	}
	ApplyThemeColors(UIThemeIndex);

	// テーマ切替で色を貼り替えるための背景ボーダー参照をリセット
	ThemeScreenBgBorders.Reset();
	ThemeHeaderBgBorders.Reset();
	ThemePanelBgBorders.Reset();
	ThemeListBgBorders.Reset();

	// ---- ルート：画面全体の背景（端に余白を入れる：Android対応） ----
	UBorder* Screen = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Screen"));
	Screen->SetBrushColor(ScreenBg);
	ThemeScreenBgBorders.Add(Screen);
	Screen->SetPadding(FMargin(32.f, 56.f));
	Screen->SetHorizontalAlignment(HAlign_Fill);
	Screen->SetVerticalAlignment(VAlign_Fill);
	WidgetTree->RootWidget = Screen;

	// 着信ポップアップを常に最前面に重ねられるよう、画面全体をOverlayにする。
	UOverlay* RootOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootOverlay"));
	Screen->SetContent(RootOverlay);

	UVerticalBox* Root = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Root"));
	{
		UOverlaySlot* OS = RootOverlay->AddChildToOverlay(Root);
		OS->SetHorizontalAlignment(HAlign_Fill);
		OS->SetVerticalAlignment(VAlign_Fill);
	}

	// =============================================================
	//  ヘッダー（タイトル + 接続ステータス）
	// =============================================================
	{
		UBorder* Header = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Header"));
		Header->SetBrushColor(HeaderBg);
		ThemeHeaderBgBorders.Add(Header);
		Header->SetPadding(FMargin(28.f, 22.f));

		// ヘッダーは Overlay で構成し、タイトルを全幅の中央に重ねる。
		UOverlay* HeaderOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
		Header->SetContent(HeaderOverlay);

		// --- 背面レイヤー：左にロゴ / 右に接続ステータスアイコン ---
		UHorizontalBox* HRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		{
			UOverlaySlot* OS = HeaderOverlay->AddChildToOverlay(HRow);
			OS->SetHorizontalAlignment(HAlign_Fill);
			OS->SetVerticalAlignment(VAlign_Fill);
		}

		// ロゴ画像（RIGDRED）
		{
			UImage* Logo = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("LogoImage"));
			if (UTexture2D* LogoTex = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/RIGDRED_logo_256.RIGDRED_logo_256")))
			{
				Logo->SetBrushFromTexture(LogoTex);
			}
			Logo->SetDesiredSizeOverride(FVector2D(88.f, 88.f));
			// SizeBoxで実寸を強制する（DesiredSizeOverrideが効かない環境への保険）
			USizeBox* LogoBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("LogoBox"));
			LogoBox->SetWidthOverride(88.f);
			LogoBox->SetHeightOverride(88.f);
			LogoBox->AddChild(Logo);
			UHorizontalBoxSlot* S = HRow->AddChildToHorizontalBox(LogoBox);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 16.f, 0.f));
		}

		// 中央スペーサー（ステータスアイコンを右端へ押し出す）
		{
			UTextBlock* Spacer = MakeText(WidgetTree, TEXT(""), 1, TextDim);
			UHorizontalBoxSlot* S = HRow->AddChildToHorizontalBox(Spacer);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}

		// 接続ステータスアイコン（状態でテクスチャが変わる／接続中は回転）
		{
			StatusIcon = MakeIcon(WidgetTree, TEXT("/Game/Texture/T_RADNODZ_white_nonConect_2.T_RADNODZ_white_nonConect_2"), 88.f);
			StatusIcon->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
			// SizeBoxで実寸を強制する（DesiredSizeOverrideが効かない環境への保険）
			USizeBox* StatusIconBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("StatusIconBox"));
			StatusIconBox->SetWidthOverride(88.f);
			StatusIconBox->SetHeightOverride(88.f);
			StatusIconBox->AddChild(StatusIcon);
			UHorizontalBoxSlot* S = HRow->AddChildToHorizontalBox(StatusIconBox);
			S->SetVerticalAlignment(VAlign_Center);
		}

		// 状態テキスト（記号付き）は廃止し、アイコンのみで表す。
		// （Apply系から参照されるため生成はするが、非表示にしておく）
		StatusText = MakeText(WidgetTree, TEXT(""), 24, WarnRed, true);
		StatusText->SetVisibility(ESlateVisibility::Collapsed);

		// --- 前面レイヤー：タイトル「RIGDRED」＋IP表示をヘッダー全幅の中央に配置 ---
		// 「RIGD」は通常色、「RED」は赤色。タイトル下に自分のIP/Reaper IPを読み取り専用表示。
		UVerticalBox* TitleCol = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());

		UHorizontalBox* TitleBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		{
			UTextBlock* TitlePrefix = MakeText(WidgetTree, TEXT("RIGD"), 34, AccentBlue, true);
			UHorizontalBoxSlot* PS = TitleBox->AddChildToHorizontalBox(TitlePrefix);
			PS->SetVerticalAlignment(VAlign_Center);

			UTextBlock* TitleRed = MakeText(WidgetTree, TEXT("RED"), 34, RecRed, true);
			UHorizontalBoxSlot* RS = TitleBox->AddChildToHorizontalBox(TitleRed);
			RS->SetVerticalAlignment(VAlign_Center);
		}
		{
			UVerticalBoxSlot* S = TitleCol->AddChildToVerticalBox(TitleBox);
			S->SetHorizontalAlignment(HAlign_Center);
		}

		// IP表示（自分 / Reaper）。読み取り専用。
		HeaderIpText = MakeText(WidgetTree, TEXT(""), 14, TextDim, false);
		HeaderIpText->SetJustification(ETextJustify::Center);
		{
			UVerticalBoxSlot* S = TitleCol->AddChildToVerticalBox(HeaderIpText);
			S->SetHorizontalAlignment(HAlign_Center);
			S->SetPadding(FMargin(0.f, 2.f, 0.f, 0.f));
		}

		{
			UOverlaySlot* OS = HeaderOverlay->AddChildToOverlay(TitleCol);
			OS->SetHorizontalAlignment(HAlign_Center);
			OS->SetVerticalAlignment(VAlign_Center);
		}
		UpdateHeaderIpText();

		UVerticalBoxSlot* VS = Root->AddChildToVerticalBox(Header);
		VS->SetHorizontalAlignment(HAlign_Fill);

		// タイトル下にネオンの細いライン（サイバー感）
		{
			UBorder* Line = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("HeaderLine"));
			Line->SetBrushColor(NeonEdge);
			Line->SetPadding(FMargin(0.f));
			USizeBox* LineH = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			LineH->SetHeightOverride(2.f);
			LineH->AddChild(Line);
			UVerticalBoxSlot* LS = Root->AddChildToVerticalBox(LineH);
			LS->SetHorizontalAlignment(HAlign_Fill);
		}
	}

	// =============================================================
	//  ホーム画面（接続 / 追加 / トラック一覧 / 録音操作）
	//  ※各パネルは Root ではなく HomePage に積む。
	//    HomePage は後段の ContentSwitcher の 0 番目のページになる。
	// =============================================================
	HomePage = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("HomePage"));
	// 横画面対応：向きに応じてパネルを入れ替えるための配置箱を用意（中身は ApplyHomeOrientation が組む）。
	// 各パネルは以降で1度だけ生成してメンバーに保持し、HomePage へは直接積まず ApplyHomeOrientation でまとめて配置する。
	HomePortraitCol         = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("HomePortraitCol"));
	HomeLandscapeRow        = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HomeLandscapeRow"));
	HomeLandscapeLeftScroll = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("HomeLandscapeLeftScroll"));

	// =============================================================
	//  操作行（接続 / 通話）
	//  ※IP/Port・相手IPなどの設定値は「設定」タブへ移動。ここは操作ボタンのみ。
	// =============================================================
	{
		UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ActionPanel"));
		Panel->SetBrushColor(PanelBg);
		ThemePanelBgBorders.Add(Panel);
		Panel->SetPadding(FMargin(28.f, 18.f));

		UHorizontalBox* HRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		Panel->SetContent(HRow);

		// 接続/切断トグル
		ConnectButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("接続"), TEXT("Connect")), BtnBlueDark,
			TEXT("ConnectButton"), &ConnectButtonText, TEXT("/Game/Texture/T_icon_conect_1.T_icon_conect_1"));
		{
			USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BtnSize->SetMinDesiredHeight(64.f);
			BtnSize->AddChild(ConnectButton);
			UHorizontalBoxSlot* S = HRow->AddChildToHorizontalBox(BtnSize);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
		}

		// グループトーク：GTALK1 / GTALK2 の2チャンネルを、それぞれ□の枠で囲む。
		// 各枠の中は参加者を表す〇（頭文字入り）の行＋「受信」「送信」の2列。
		// 受信＝タップで受信ON/OFF・長押しで自分のマスターchに設定。
		// 送信＝マスターchはタップでラッチ、サブchは押している間だけ送信（PTT）。
		// 〇の行・色・マスター表示は UpdateGroupTalkUI() で更新する。
		{
			// アクティビティの点滅ドット（送受信中は緑）。
			auto MakeDot = [&](TObjectPtr<UImage>& OutDot) -> UImage*
			{
				UImage* Dot = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
				Dot->SetBrush(FSlateRoundedBoxBrush(FLinearColor::White, 6.f, FVector2D(12.f, 12.f)));
				Dot->SetDesiredSizeOverride(FVector2D(12.f, 12.f));
				Dot->SetColorAndOpacity(FLinearColor(0.20f, 0.22f, 0.24f, 1.f));   // 既定=消灯
				OutDot = Dot;
				return Dot;
			};

			// 「ラベル(受信/送信)＋アイコンボタン＋右下ドット」を1列作るヘルパー。
			// OutCapは言語切替時にApplyLanguageから再適用できるよう、呼び出し側のメンバへ保持させる。
			auto MakeIconColumn = [&](TObjectPtr<UButton>& OutBtn, TObjectPtr<UImage>& OutDot, TObjectPtr<UTextBlock>& OutCap,
				const TCHAR* IconPath, FName Name, const FString& CaptionLabel) -> UWidget*
			{
				UVerticalBox* Col = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());

				// 見出し（受信 / 送信）
				UTextBlock* Cap = MakeText(WidgetTree, CaptionLabel, 13, TextDim, true);
				Cap->SetJustification(ETextJustify::Center);
				OutCap = Cap;
				{
					UVerticalBoxSlot* VS2 = Col->AddChildToVerticalBox(Cap);
					VS2->SetHorizontalAlignment(HAlign_Center);
					VS2->SetPadding(FMargin(0.f, 0.f, 0.f, 2.f));
				}

				// アイコンのみボタン＋右下ドット
				OutBtn = MakeButton(WidgetTree, TEXT(""), StopGray, Name, nullptr, IconPath);
				UOverlay* Ov = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
				{
					UOverlaySlot* OS = Ov->AddChildToOverlay(OutBtn);
					OS->SetHorizontalAlignment(HAlign_Fill);
					OS->SetVerticalAlignment(VAlign_Fill);
				}
				{
					UOverlaySlot* OS = Ov->AddChildToOverlay(MakeDot(OutDot));
					OS->SetHorizontalAlignment(HAlign_Right);
					OS->SetVerticalAlignment(VAlign_Bottom);
					OS->SetPadding(FMargin(0.f, 0.f, 6.f, 6.f));
				}
				USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				BtnSize->SetMinDesiredHeight(58.f);
				BtnSize->AddChild(Ov);
				{
					UVerticalBoxSlot* VS2 = Col->AddChildToVerticalBox(BtnSize);
					VS2->SetHorizontalAlignment(HAlign_Fill);
				}
				return Col;
			};

			// 1チャンネル分の枠（□）を作るヘルパー。中身は参加者〇の行＋[受信列|送信列]。
			auto MakeChannelBox = [&](int32 Ch,
				TObjectPtr<UButton>& RecvBtn, TObjectPtr<UImage>& RecvDot,
				TObjectPtr<UButton>& SendBtn, TObjectPtr<UImage>& SendDot) -> UWidget*
			{
				TObjectPtr<UTextBlock>& RecvCap = GroupTalkRecvCaption[Ch];
				TObjectPtr<UTextBlock>& SendCap = GroupTalkSendCaption[Ch];
				UBorder* Frame = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
				// 半透明の塗り＋アウトラインで「□の枠」に見せる。送信中はUpdateGroupTalkUI()が枠色を変える。
				Frame->SetBrush(FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.18f), 8.f, FieldOutline, 2.f, FVector2D(1.f, 1.f)));
				Frame->SetPadding(FMargin(10.f, 6.f, 10.f, 8.f));
				GroupTalkFrame[Ch] = Frame;

				UVerticalBox* VBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
				Frame->SetContent(VBox);

				// 参加者を表す〇（頭文字入り）の行。高さを固定して、〇が0個でも枠の大きさが変わらないようにする。
				// 中身は UpdateGroupTalkUI() が参加者に合わせて作り直す。
				GroupTalkAvatarRow[Ch] = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
				{
					USizeBox* AvatarRowSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
					AvatarRowSize->SetHeightOverride(32.f);
					AvatarRowSize->AddChild(GroupTalkAvatarRow[Ch]);

					UVerticalBoxSlot* VS2 = VBox->AddChildToVerticalBox(AvatarRowSize);
					VS2->SetHorizontalAlignment(HAlign_Center);
					VS2->SetPadding(FMargin(0.f, 0.f, 0.f, 4.f));
				}

				// [受信列 | 送信列]
				UHorizontalBox* Cols = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
				{
					UWidget* RecvCol = MakeIconColumn(RecvBtn, RecvDot, RecvCap,
						TEXT("/Game/Texture/T_icon_moniter_1.T_icon_moniter_1"),
						*FString::Printf(TEXT("GroupTalkRecv%d"), Ch), ReaperLang::S(TEXT("受信"), TEXT("Recv")));
					UHorizontalBoxSlot* S = Cols->AddChildToHorizontalBox(RecvCol);
					S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
					S->SetPadding(FMargin(0.f, 0.f, 4.f, 0.f));
				}
				{
					UWidget* SendCol = MakeIconColumn(SendBtn, SendDot, SendCap,
						TEXT("/Game/Texture/T_icon_mic_1.T_icon_mic_1"),
						*FString::Printf(TEXT("GroupTalkSend%d"), Ch), ReaperLang::S(TEXT("送信"), TEXT("Send")));
					UHorizontalBoxSlot* S = Cols->AddChildToHorizontalBox(SendCol);
					S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
					S->SetPadding(FMargin(4.f, 0.f, 0.f, 0.f));
				}
				VBox->AddChildToVerticalBox(Cols);
				return Frame;
			};

			// GTALK1 枠
			{
				UWidget* Box0 = MakeChannelBox(0,
					GroupTalkRecvButton[0], GroupTalkRecvDot[0], GroupTalkSendButton[0], GroupTalkSendDot[0]);
				UHorizontalBoxSlot* S = HRow->AddChildToHorizontalBox(Box0);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				S->SetPadding(FMargin(12.f, 0.f, 6.f, 0.f));
			}
			// GTALK2 枠
			{
				UWidget* Box1 = MakeChannelBox(1,
					GroupTalkRecvButton[1], GroupTalkRecvDot[1], GroupTalkSendButton[1], GroupTalkSendDot[1]);
				UHorizontalBoxSlot* S = HRow->AddChildToHorizontalBox(Box1);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				S->SetPadding(FMargin(6.f, 0.f, 0.f, 0.f));
			}
		}

		ActionPanel = Panel;   // 配置は ApplyHomeOrientation() が行う
	}

	// =============================================================
	//  接続先デバイス情報バー（Hz / bit・入力/出力ch）
	//  ※接続ボタンの下、トラック追加ツールバーの上に1行で表示する。
	//    値は接続時・更新時に UpdateDeviceInfo() で取得して反映する。
	// =============================================================
	{
		UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("DeviceInfoPanel"));
		Panel->SetBrushColor(PanelBg);
		ThemePanelBgBorders.Add(Panel);
		Panel->SetPadding(FMargin(28.f, 10.f));

		DeviceInfoText = MakeText(WidgetTree, TEXT(""), 16, TextDim, false);
		DeviceInfoText->SetJustification(ETextJustify::Center);
		Panel->SetContent(DeviceInfoText);

		DeviceInfoPanel = Panel;   // 配置は ApplyHomeOrientation() が行う

		UpdateDeviceInfo();   // 初期表示（未接続時は「-」）
	}

	// =============================================================
	//  トラック追加ツールバー（＋追加＝ダイアログ / 更新）
	// =============================================================
	{
		UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("AddPanel"));
		Panel->SetBrushColor(PanelBg);
		ThemePanelBgBorders.Add(Panel);
		Panel->SetPadding(FMargin(28.f, 18.f));

		UHorizontalBox* HRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		Panel->SetContent(HRow);

		// 追加 / モニター / マイク / メーター / 更新 の5ボタンを等幅(Fill)で並べ、
		// ウィンドウ幅にちょうど収める。各ボタンは固定幅を持たせず均等配分する。
		// 各スロットへ等幅で追加するヘルパー（最後以外は右に間隔を空ける）。
		auto AddToolbarButton = [&](UButton* Btn, bool bLast)
		{
			USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BtnSize->SetMinDesiredHeight(64.f);
			BtnSize->AddChild(Btn);
			UHorizontalBoxSlot* S = HRow->AddChildToHorizontalBox(BtnSize);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));   // 等幅
			if (!bLast) { S->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f)); }
		};

		AddTrackButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("追加"), TEXT("Add")), BtnGreenDark,
			TEXT("AddTrackButton"), &AddTrackButtonText, TEXT("/Game/Texture/T_icon_addTrack_1.T_icon_addTrack_1"));
		AddToolbarButton(AddTrackButton, false);

		// （マイク登録「設定」ボタンと言語切替ボタンは「設定」タブへ移動した）

		// 音声モニター（ReaStream受信）トグル
		MonitorButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("モニター"), TEXT("Monitor")), StopGray,
			TEXT("MonitorButton"), &MonitorButtonText, TEXT("/Game/Texture/T_icon_moniter_1.T_icon_moniter_1"));
		AddToolbarButton(MonitorButton, false);

		// （マイク送信ボタンは廃止。グループトークの送信アイコンで送信するため、ここには置かない。）

		// メーター受信トグル。ラベルは「メーター」のみ（ON/OFFは色で表現）。必要なときだけON（既定OFF）。
		MeterRecvButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("メーター"), TEXT("Meter")), StopGray,
			TEXT("MeterRecvButton"), &MeterRecvButtonText, TEXT("/Game/Texture/T_icon_meter_1.T_icon_meter_1"));
		AddToolbarButton(MeterRecvButton, false);

		UpdateButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("更新"), TEXT("Refresh")), BtnBlueDark,
			TEXT("UpdateButton"), &UpdateButtonText, TEXT("/Game/Texture/T_icon_update_1.T_icon_update_1"));
		AddToolbarButton(UpdateButton, true);

		ToolbarPanel = Panel;   // 配置は ApplyHomeOrientation() が行う
	}

	// =============================================================
	//  トラック一覧（ScrollBox）
	// =============================================================
	{
		UBorder* ListPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ListPanel"));
		ListPanel->SetBrushColor(ListBg);
		ListPanel->SetPadding(FMargin(24.f, 18.f));
		TrackListPanel = ListPanel;   // 録音中に赤枠を出すため参照を保持

		UVerticalBox* ListCol = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
		ListPanel->SetContent(ListCol);

		TrackSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("トラック"), TEXT("Tracks")), 18, TextDim, true);
		{
			UVerticalBoxSlot* S = ListCol->AddChildToVerticalBox(TrackSectionLabel);
			S->SetPadding(FMargin(2.f, 0.f, 0.f, 8.f));
		}

		TrackListScrollBox = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("TrackListScrollBox"));
		{
			UVerticalBoxSlot* S = ListCol->AddChildToVerticalBox(TrackListScrollBox);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}

		// ListPanel(=TrackListPanel) の配置は ApplyHomeOrientation() が行う（縦=Fill / 横=右側Fill）。
	}

	// =============================================================
	//  フッター（状態表示＋録音コントロール）
	// =============================================================
	{
		UBorder* Footer = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Footer"));
		Footer->SetBrushColor(HeaderBg);
		ThemeHeaderBgBorders.Add(Footer);
		Footer->SetPadding(FMargin(28.f, 22.f));

		UVerticalBox* FCol = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
		Footer->SetContent(FCol);

		// 状態表示（再生中 / 録音中 / 停止中）— ボタンの上に大きく表示。
		// Overlayで「中央＝状態テキスト」「左＝削除モードの全選択/全解除」を重ねる。
		UOverlay* StateOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
		{
			UVerticalBoxSlot* S = FCol->AddChildToVerticalBox(StateOverlay);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 14.f));
		}

		// 中央：赤丸（録音中のみ）＋状態テキスト
		{
			UHorizontalBox* StateRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			{
				RecStateDot = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
				const FLinearColor RecDotRed(0.95f, 0.16f, 0.20f, 1.f);   // はっきりした赤丸
				RecStateDot->SetBrush(FSlateRoundedBoxBrush(RecDotRed, 13.f, FVector2D(26.f, 26.f)));
				RecStateDot->SetDesiredSizeOverride(FVector2D(26.f, 26.f));
				RecStateDot->SetVisibility(ESlateVisibility::Collapsed);
				UHorizontalBoxSlot* S = StateRow->AddChildToHorizontalBox(RecStateDot);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
			}
			TransportStateText = MakeText(WidgetTree, ReaperLang::S(TEXT("■ 停止中"), TEXT("■ Stopped")), 28, TextDim, true);
			TransportStateText->SetJustification(ETextJustify::Center);
			{
				UHorizontalBoxSlot* S = StateRow->AddChildToHorizontalBox(TransportStateText);
				S->SetVerticalAlignment(VAlign_Center);
			}
			UOverlaySlot* OS = StateOverlay->AddChildToOverlay(StateRow);
			OS->SetHorizontalAlignment(HAlign_Center);
			OS->SetVerticalAlignment(VAlign_Center);
		}

		// 左：削除モード時のみ表示する「全選択 / 全解除」
		{
			DeleteSelectRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("DeleteSelectRow"));
			DeleteSelectRow->SetVisibility(ESlateVisibility::Collapsed);

			DeleteSelectAllButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("全選択"), TEXT("Select All")), BtnGreenDark,
				TEXT("DeleteSelectAllButton"), &DeleteSelectAllButtonText);
			{
				USizeBox* B1 = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				B1->SetMinDesiredWidth(130.f); B1->SetMinDesiredHeight(56.f);
				B1->AddChild(DeleteSelectAllButton);
				UHorizontalBoxSlot* S1 = DeleteSelectRow->AddChildToHorizontalBox(B1);
				S1->SetVerticalAlignment(VAlign_Center);
				S1->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
			}

			DeleteDeselectAllButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("全解除"), TEXT("Deselect All")), StopGray,
				TEXT("DeleteDeselectAllButton"), &DeleteDeselectAllButtonText);
			{
				USizeBox* B2 = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				B2->SetMinDesiredWidth(130.f); B2->SetMinDesiredHeight(56.f);
				B2->AddChild(DeleteDeselectAllButton);
				UHorizontalBoxSlot* S2 = DeleteSelectRow->AddChildToHorizontalBox(B2);
				S2->SetVerticalAlignment(VAlign_Center);
			}

			UOverlaySlot* OS = StateOverlay->AddChildToOverlay(DeleteSelectRow);
			OS->SetHorizontalAlignment(HAlign_Left);
			OS->SetVerticalAlignment(VAlign_Center);
		}

		// 削除 / 全解除 / 計測調整 / Slack / ロックを等幅(Fill)で横並びにする（縦向きは1段、横向きは
		// ApplyHomeOrientation()が2段に組み替える。InfoRowsWrapは差し替え用の箱で常にFColへ常設する）。
		InfoRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		InfoRowLandscapeTop = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		InfoRowLandscapeBottom = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		InfoRowsWrap = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
		{
			UVerticalBoxSlot* S = FCol->AddChildToVerticalBox(InfoRowsWrap);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 12.f));
		}
		{
			UVerticalBoxSlot* S = InfoRowsWrap->AddChildToVerticalBox(InfoRow);
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		// トラック削除ボタン（押すと選択モードへ→「決定」に変わる）。ゴミ箱アイコン。
		// アイコンを文字の上・中央に置く縦並びレイアウト（bIconOnTop）にしている。
		DeleteTrackButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("削除"), TEXT("Delete")), StopGray,
			TEXT("DeleteTrackButton"), &DeleteTrackButtonText, TEXT("/Game/Texture/T_icon_trash_1.T_icon_trash_1"),
			nullptr, /*bIconOnTop=*/true);
		{
			USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BtnSize->SetMinDesiredHeight(100.f);
			BtnSize->AddChild(DeleteTrackButton);
			UHorizontalBoxSlot* S = InfoRow->AddChildToHorizontalBox(BtnSize);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		}

		ClearArmedButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("全解除"), TEXT("Clear All")), StopGray, TEXT("ClearArmedButton"), &ClearArmedButtonText, TEXT("/Game/Texture/T_icon_reset_1.T_icon_reset_1"), nullptr, /*bIconOnTop=*/true);
		{
			USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BtnSize->SetMinDesiredHeight(100.f);
			BtnSize->AddChild(ClearArmedButton);
			UHorizontalBoxSlot* S = InfoRow->AddChildToHorizontalBox(BtnSize);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		}

		// ラウドネス計測＆音量調整（全メディアを計測し、目標LUFSへ音量を合わせる。実行中はアイコンが回転）
		// 他の4ボタン（削除/全解除/Slack/ロック）と揃えて、アイコンを文字の上・中央に置く縦並びにする。
		{
			NormalizeButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("NormalizeButton"));
			StyleRoundedButton(NormalizeButton);
			NormalizeButton->SetBackgroundColor(BtnBlueDark);

			UVerticalBox* NBContent = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());

			NormalizeIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("NormalizeIcon"));
			if (UTexture2D* IconTex = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/T_icon_loudness_1.T_icon_loudness_1")))
			{
				NormalizeIcon->SetBrushFromTexture(IconTex);
			}
			NormalizeIcon->SetDesiredSizeOverride(FVector2D(30.f, 30.f));
			NormalizeIcon->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
			{
				UVerticalBoxSlot* S = NBContent->AddChildToVerticalBox(NormalizeIcon);
				S->SetHorizontalAlignment(HAlign_Center);
				S->SetPadding(FMargin(0.f, 0.f, 0.f, 4.f));
			}

			NormalizeButtonText = MakeText(WidgetTree, ReaperLang::S(TEXT("計測調整"), TEXT("Normalize")), 18, ReadableTextColor(BtnBlueDark), true);
			NormalizeButtonText->SetJustification(ETextJustify::Center);
			NormalizeButtonText->SetAutoWrapText(true);   // 長い翻訳文字列（独語等）がボタン幅をはみ出さず2行に折り返す
			{
				UVerticalBoxSlot* S = NBContent->AddChildToVerticalBox(NormalizeButtonText);
				S->SetHorizontalAlignment(HAlign_Center);
			}

			NormalizeButton->SetContent(NBContent);

			USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BtnSize->SetMinDesiredHeight(100.f);
			BtnSize->AddChild(NormalizeButton);
			UHorizontalBoxSlot* S = InfoRow->AddChildToHorizontalBox(BtnSize);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		}

		// 全トラックの収録波形名＋ラウドネスをSlackへ共有（未計測は先に計測してから送る）
		SlackShareButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("Slack"), TEXT("Slack")), BtnGreenDark, TEXT("SlackShareButton"), &SlackShareButtonText, TEXT("/Game/Texture/T_icon_slack_1.T_icon_slack_1"), nullptr, /*bIconOnTop=*/true);
		{
			USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BtnSize->SetMinDesiredHeight(100.f);
			BtnSize->AddChild(SlackShareButton);
			UHorizontalBoxSlot* S = InfoRow->AddChildToHorizontalBox(BtnSize);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		}

		// ロック：ON中はPlay/Rec/追加/削除/全解除/計測調整を無効化する誤操作防止（グループトークは対象外）
		// アイコンは初期状態（解除中）で作り、UpdateLockButton()がロック中/解除中で差し替える。
		LockButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("ロック"), TEXT("Lock")), StopGray, TEXT("LockButton"), &LockButtonText,
			TEXT("/Game/Texture/T_icon_release_1.T_icon_release_1"), &LockButtonIcon, /*bIconOnTop=*/true);
		{
			USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BtnSize->SetMinDesiredHeight(100.f);
			BtnSize->AddChild(LockButton);
			UHorizontalBoxSlot* S = InfoRow->AddChildToHorizontalBox(BtnSize);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}

		// 2段目：PLAY / REC の2ボタン。状態に応じて STOP に変わる（横幅いっぱい）
		const float TransportH = 140.f;
		const int32 TransportFont = 32;

		UHorizontalBox* TransportRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		{
			UVerticalBoxSlot* S = FCol->AddChildToVerticalBox(TransportRow);
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		PlayButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("▶ 再生"), TEXT("▶ PLAY")), AccentGreen, TEXT("PlayButton"), &PlayButtonText);
		if (PlayButtonText) { PlayButtonText->SetFont(ReaperFont::Get(TransportFont, true)); }
		{
			PlayButtonSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			PlayButtonSize->SetMinDesiredHeight(TransportH);
			PlayButtonSize->AddChild(PlayButton);
			UHorizontalBoxSlot* S = TransportRow->AddChildToHorizontalBox(PlayButtonSize);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 16.f, 0.f));
		}

		// RECボタン：中にマイク録音アイコン＋テキスト
		RecordButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("RecordButton"));
		StyleRoundedButton(RecordButton);
		RecordButton->SetBackgroundColor(RecRed);
		{
			UHorizontalBox* RecContent = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

			// 左フィラー（中央寄せ用）
			{
				UTextBlock* Sp = MakeText(WidgetTree, TEXT(""), 1, TextDim);
				UHorizontalBoxSlot* S = RecContent->AddChildToHorizontalBox(Sp);
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}

			RecordButtonIcon = MakeRecDotIcon(WidgetTree, 34.f);
			{
				UHorizontalBoxSlot* S = RecContent->AddChildToHorizontalBox(RecordButtonIcon);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
			}

			RecordButtonText = MakeText(WidgetTree, ReaperLang::S(TEXT("録音"), TEXT("REC")), TransportFont, ReadableTextColor(RecRed), true);
			{
				UHorizontalBoxSlot* S = RecContent->AddChildToHorizontalBox(RecordButtonText);
				S->SetVerticalAlignment(VAlign_Center);
			}

			// 右フィラー
			{
				UTextBlock* Sp = MakeText(WidgetTree, TEXT(""), 1, TextDim);
				UHorizontalBoxSlot* S = RecContent->AddChildToHorizontalBox(Sp);
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}

			RecordButton->SetContent(RecContent);
		}
		{
			RecordButtonSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			RecordButtonSize->SetMinDesiredHeight(TransportH);
			RecordButtonSize->AddChild(RecordButton);
			UHorizontalBoxSlot* S = TransportRow->AddChildToHorizontalBox(RecordButtonSize);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}

		FooterPanel = Footer;   // 配置は ApplyHomeOrientation() が行う
	}

	// 全パネルが揃ったので、初期は縦向き配置でホームを組む（以後は NativeTick が向きに応じて切替）。
	ApplyHomeOrientation(false);

	// =============================================================
	//  ホーム/設定の切り替え本体（ContentSwitcher）
	//   index 0 = ホーム画面 / index 1 = 設定画面
	// =============================================================
	{
		ContentSwitcher = WidgetTree->ConstructWidget<UWidgetSwitcher>(UWidgetSwitcher::StaticClass(), TEXT("ContentSwitcher"));

		// 0: ホーム
		if (UWidgetSwitcherSlot* HS = Cast<UWidgetSwitcherSlot>(ContentSwitcher->AddChild(HomePage)))
		{
			HS->SetHorizontalAlignment(HAlign_Fill);
			HS->SetVerticalAlignment(VAlign_Fill);
		}

		// 1: リスト
		if (UWidgetSwitcherSlot* LS = Cast<UWidgetSwitcherSlot>(ContentSwitcher->AddChild(BuildListPage())))
		{
			LS->SetHorizontalAlignment(HAlign_Fill);
			LS->SetVerticalAlignment(VAlign_Fill);
		}

		// 2: 設定
		if (UWidgetSwitcherSlot* SS = Cast<UWidgetSwitcherSlot>(ContentSwitcher->AddChild(BuildSettingsPage())))
		{
			SS->SetHorizontalAlignment(HAlign_Fill);
			SS->SetVerticalAlignment(VAlign_Fill);
		}

		ContentSwitcher->SetActiveWidgetIndex(0);

		UVerticalBoxSlot* VS = Root->AddChildToVerticalBox(ContentSwitcher);
		VS->SetHorizontalAlignment(HAlign_Fill);
		VS->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}

	// =============================================================
	//  下部タブバー（ホーム / 設定）
	// =============================================================
	{
		UVerticalBoxSlot* VS = Root->AddChildToVerticalBox(BuildBottomNav());
		VS->SetHorizontalAlignment(HAlign_Fill);
		VS->SetPadding(FMargin(0.f, 1.f, 0.f, 0.f));
	}

}

// =============================================================
//  設定画面の構築（言語切替 / マイク登録 など）
// =============================================================
UWidget* UWG_RadNodzToolkit::BuildSettingsPage()
{
	using namespace ReaperCtrlUI;

	UBorder* Page = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("SettingsPage"));
	Page->SetBrushColor(ScreenBg);
	ThemeScreenBgBorders.Add(Page);
	Page->SetPadding(FMargin(0.f));
	Page->SetHorizontalAlignment(HAlign_Fill);
	Page->SetVerticalAlignment(VAlign_Fill);

	UScrollBox* Scroll = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("SettingsScroll"));
	Page->SetContent(Scroll);

	// 見出し
	SettingsTitleText = MakeText(WidgetTree, ReaperLang::S(TEXT("設定"), TEXT("Settings")), 30, TextPrimary, true);
	{
		UScrollBoxSlot* S = Cast<UScrollBoxSlot>(Scroll->AddChild(SettingsTitleText));
		if (S) { S->SetPadding(FMargin(28.f, 24.f, 28.f, 16.f)); S->SetHorizontalAlignment(HAlign_Fill); }
	}

	// 1行分の設定項目（左ラベル＋右コントロール）を作るヘルパー。生成した行を返す。
	auto AddSettingRow = [&](UTextBlock* LabelText, UWidget* Control) -> UBorder*
	{
		UBorder* Row = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
		Row->SetBrushColor(PanelBg);
		Row->SetPadding(FMargin(28.f, 20.f));
		ThemePanelBgBorders.Add(Row);

		UHorizontalBox* HRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		Row->SetContent(HRow);

		{
			UHorizontalBoxSlot* S = HRow->AddChildToHorizontalBox(LabelText);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}
		{
			UHorizontalBoxSlot* S = HRow->AddChildToHorizontalBox(Control);
			S->SetVerticalAlignment(VAlign_Center);
		}

		UScrollBoxSlot* SS = Cast<UScrollBoxSlot>(Scroll->AddChild(Row));
		if (SS) { SS->SetPadding(FMargin(0.f, 1.f, 0.f, 0.f)); SS->SetHorizontalAlignment(HAlign_Fill); }
		return Row;
	};

	// セクション見出しを作るヘルパー
	auto AddSectionHeader = [&](UTextBlock* HeaderText)
	{
		UScrollBoxSlot* S = Cast<UScrollBoxSlot>(Scroll->AddChild(HeaderText));
		if (S) { S->SetPadding(FMargin(28.f, 18.f, 28.f, 8.f)); S->SetHorizontalAlignment(HAlign_Fill); }
	};

	// 設定用の入力欄（固定幅）を作るヘルパー。生成した入力欄を OutBox で返す。
	auto MakeFixedInput = [&](const FString& Hint, FName Name, const FString& Value, float Width,
		TObjectPtr<UEditableTextBox>& OutBox) -> UWidget*
	{
		UEditableTextBox* Box = MakeInput(WidgetTree, Hint, Name);
		Box->SetText(FText::FromString(Value));
		OutBox = Box;
		USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		// 幅を固定（最小=最大）。長い入力（Slackトークン等）でも欄が広がらず、ボックス内で横スクロールできる。
		SB->SetMinDesiredWidth(Width);
		SB->SetMaxDesiredWidth(Width);
		SB->AddChild(Box);
		return SB;
	};

	// =========================================================
	//  接続（この端末のIP / Reaper IP / Port）
	// =========================================================
	ConnSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("接続"), TEXT("Connection")), 18, AccentBlue, true);
	AddSectionHeader(ConnSectionLabel);

	// この端末のIP（表示のみ）
	{
		LocalIpRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("この端末のIP"), TEXT("This device IP")), 22, TextPrimary, true);
		FString LocalIP = URadnodzUtility::AZ_GetLocalIP();
		if (LocalIP.IsEmpty()) { LocalIP = ReaperLang::S(TEXT("不明"), TEXT("Unknown")); }
		LocalIPText = MakeText(WidgetTree, LocalIP, 20, TextDim, true);
		AddSettingRow(LocalIpRowLabel, LocalIPText);
	}

	// サーバーID（ドロップダウン + 追加ボタン）
	{
		UTextBlock* Label = MakeText(WidgetTree, ReaperLang::S(TEXT("サーバーID"), TEXT("Server ID")), 22, TextPrimary, true);
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		ServerIdCombo = MakeDarkCombo(WidgetTree);
		ServerIdCombo->OnSelectionChanged.AddDynamic(this, &UWG_RadNodzToolkit::HandleServerIdComboChanged);
		// ドロップダウン項目/選択表示を白文字にする
		ServerIdCombo->OnGenerateWidgetEvent.BindDynamic(this, &UWG_RadNodzToolkit::MakeServerIdComboItem);
		{
			USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			SB->SetMinDesiredWidth(260.f);
			SB->SetMinDesiredHeight(56.f);   // 「追加」ボタンと高さを揃える
			SB->AddChild(ServerIdCombo);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(SB);
			S->SetVerticalAlignment(VAlign_Center);
		}
		AddServerIdButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("追加"), TEXT("Add")), AccentBlue,
			TEXT("AddServerIdButton"), &AddServerIdButtonText);
		AddServerIdButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleAddServerIdClicked);
		{
			USizeBox* BB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BB->SetMinDesiredWidth(110.f); BB->SetMinDesiredHeight(56.f); BB->AddChild(AddServerIdButton);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(BB);
			S->SetPadding(FMargin(8.f, 0.f, 0.f, 0.f)); S->SetVerticalAlignment(VAlign_Center);
		}
		// 削除ボタン（追加ボタンの右隣。押下で確認ポップアップ）
		DeleteServerIdButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("削除"), TEXT("Delete")), RecRed,
			TEXT("DeleteServerIdButton"), &DeleteServerIdButtonText);
		DeleteServerIdButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleDeleteServerIdClicked);
		{
			USizeBox* BB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BB->SetMinDesiredWidth(110.f); BB->SetMinDesiredHeight(56.f); BB->AddChild(DeleteServerIdButton);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(BB);
			S->SetPadding(FMargin(8.f, 0.f, 0.f, 0.f)); S->SetVerticalAlignment(VAlign_Center);
		}
		AddSettingRow(Label, H);
	}

	// Reaper IP（入力欄＋「検索」ボタン。検索は同ネットワークのReaperを一覧表示して選択接続する）
	{
		IpLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("Reaper IP"), TEXT("Reaper IP")), 22, TextPrimary, true);

		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		UWidget* Ctrl = MakeFixedInput(TEXT("192.168.1.1"), TEXT("IPAddressInput"), IPAddress, 300.f, IPAddressInput);
		{
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(Ctrl);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));
		}

		ScanButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("検索"), TEXT("Scan")), AccentBlue, TEXT("ScanButton"), &ScanButtonText);
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredWidth(120.f);
			B->SetMinDesiredHeight(64.f);
			B->AddChild(ScanButton);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(B);
			S->SetVerticalAlignment(VAlign_Center);
		}

		AddSettingRow(IpLabel, H);
	}

	// Port
	{
		PortLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("ポート"), TEXT("Port")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("12345"), TEXT("PortInput"), FString::FromInt(Port), 180.f, PortInput);
		AddSettingRow(PortLabel, Ctrl);
	}

	// --- 認証設定（mTLS）: TLS使用 / serverId 管理 / 公開鍵コピー / 認証（「接続」に含める） ---

	// TLS を使用するか（本体接続・認証の両方に適用）
	{
		UTextBlock* Label = MakeText(WidgetTree, ReaperLang::S(TEXT("TLSを使用"), TEXT("Use TLS")), 22, TextPrimary, true);
		UseTlsButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("UseTlsButton"), &UseTlsButtonText);
		UseTlsButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleUseTlsClicked);
		UpdateUseTlsButton();
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(160.f); B->SetMinDesiredHeight(64.f); B->AddChild(UseTlsButton);
		AddSettingRow(Label, B);
	}

	// サーバーFP（選択中 serverId の期待FP。編集してコミットで更新）
	{
		UTextBlock* Label = MakeText(WidgetTree, ReaperLang::S(TEXT("サーバーFP"), TEXT("Server FP")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("SHA256:..."), TEXT("ServerFpInput"), TEXT(""), 420.f, ServerFpInput);
		ServerFpInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleServerFpCommitted);
		AddSettingRow(Label, Ctrl);
	}

	// 認証（公開鍵コピー / 認証）
	{
		UTextBlock* Label = MakeText(WidgetTree, ReaperLang::S(TEXT("認証"), TEXT("Auth")), 22, TextPrimary, true);
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		auto AddBtn = [&](UButton* Btn, float Width = 150.f)
		{
			USizeBox* BB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BB->SetMinDesiredWidth(Width); BB->SetMinDesiredHeight(56.f); BB->AddChild(Btn);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(BB);
			S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f)); S->SetVerticalAlignment(VAlign_Center);
		};
		CopyPubKeyButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("公開鍵をコピー"), TEXT("Copy public key")), StopGray,
			TEXT("CopyPubKeyButton"), &CopyPubKeyButtonText);
		CopyPubKeyButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleCopyPublicKeyClicked);
		AddBtn(CopyPubKeyButton);
		TrialConnectButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("認証"), TEXT("Authenticate")), AccentBlue,
			TEXT("TrialConnectButton"), &TrialConnectButtonText);
		TrialConnectButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleTrialConnectClicked);
		// 「認証中...」を1行で表示する（MakeButton は既定で折り返し有効なので無効化）。
		if (TrialConnectButtonText) { TrialConnectButtonText->SetAutoWrapText(false); }
		// ボタン幅も広めにする
		AddBtn(TrialConnectButton, 220.f);
		AddSettingRow(Label, H);
	}

	// 状態表示（横幅を確保して1文字ずつ縦折返しにならないようにする）
	{
		UTextBlock* Label = MakeText(WidgetTree, ReaperLang::S(TEXT("状態"), TEXT("Status")), 22, TextPrimary, true);
		AuthStatusText = MakeText(WidgetTree, TEXT("-"), 18, TextDim, false);
		AuthStatusText->SetAutoWrapText(true);
		USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		SB->SetMinDesiredWidth(520.f);
		SB->AddChild(AuthStatusText);
		AddSettingRow(Label, SB);
	}

	// 初期反映（LoadSettings 後にも RefreshServerIdCombo が呼ばれる）
	RefreshServerIdCombo();

	// モニター/マイクのポート（ReaStream既定・固定値、表示のみ）
	{
		ReaStreamPortRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("モニター/マイクのポート"), TEXT("Monitor/Mic port")), 22, TextPrimary, true);
		ReaStreamPortText = MakeText(WidgetTree, FString::FromInt(ReaStreamPort), 20, TextDim, true);
		AddSettingRow(ReaStreamPortRowLabel, ReaStreamPortText);
	}

	// 端末名/IPをヘッダーに表示するか
	{
		ShowDeviceInfoRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("端末名/IPを表示"), TEXT("Show device name/IP")), 22, TextPrimary, true);
		ShowDeviceInfoButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("ShowDeviceInfoButton"), &ShowDeviceInfoButtonText);
		UpdateShowDeviceInfoButton();
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(160.f);
		B->SetMinDesiredHeight(64.f);
		B->AddChild(ShowDeviceInfoButton);
		AddSettingRow(ShowDeviceInfoRowLabel, B);
	}

	// Reaper通信時間（直近の計測結果表示＋「測定」ボタン）
	{
		CommTimeRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("Reaper通信時間"), TEXT("Reaper comm time")), 22, TextPrimary, true);

		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		// 直近の計測結果（未計測は「--」）
		CommTimeText = MakeText(WidgetTree, TEXT("--"), 20, TextDim, true);
		{
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(CommTimeText);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
		}

		// 「測定」ボタン：押下で SendTimingProbe を1往復させて平均通信時間を計測する
		CommTimeMeasureButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("測定"), TEXT("Measure")), AccentBlue,
			TEXT("CommTimeMeasureButton"), &CommTimeMeasureButtonText);
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredWidth(120.f);
			B->SetMinDesiredHeight(64.f);
			B->AddChild(CommTimeMeasureButton);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(B);
			S->SetVerticalAlignment(VAlign_Center);
		}

		AddSettingRow(CommTimeRowLabel, H);
	}

	// =========================================================
	//  表示名（グループトークの在席登録・参加者名簿に使う自分の名前）
	// =========================================================
	IntercomSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("表示名"), TEXT("Display Name")), 18, AccentBlue, true);
	AddSectionHeader(IntercomSectionLabel);

	{
		MemberUserNameRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("自分の名前"), TEXT("My name")), 22, TextPrimary, true);
		UWidget* NameCtrl = MakeFixedInput(ReaperLang::S(TEXT("例: 田中"), TEXT("e.g. Tanaka")), TEXT("MemberUserNameInput"), IntercomUserName, 300.f, MemberUserNameInput);
		AddSettingRow(MemberUserNameRowLabel, NameCtrl);
	}

	// =========================================================
	//  Slack共有（収録波形名＋ラウドネスの共有先）
	// =========================================================
	SlackSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("Slack共有"), TEXT("Slack Share")), 18, AccentBlue, true);
	AddSectionHeader(SlackSectionLabel);

	{
		SlackTokenRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("Botトークン"), TEXT("Bot Token")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("xoxb-..."), TEXT("SlackTokenInput"), SlackToken, 360.f, SlackTokenInput);
		AddSettingRow(SlackTokenRowLabel, Ctrl);
	}
	{
		SlackToUserRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("宛先ユーザー"), TEXT("To User")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("@name / Uxxxx"), TEXT("SlackToUserInput"), SlackToUser, 300.f, SlackToUserInput);
		AddSettingRow(SlackToUserRowLabel, Ctrl);
	}

	// =========================================================
	//  ラウドネス（音量調整の目標値）
	// =========================================================
	LoudnessSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("ラウドネス"), TEXT("Loudness")), 18, AccentBlue, true);
	AddSectionHeader(LoudnessSectionLabel);

	{
		TargetLufsRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("目標ラウドネス(LUFS)"), TEXT("Target (LUFS)")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("-7"), TEXT("TargetLufsInput"), FString::Printf(TEXT("%.0f"), TargetLUFS), 160.f, TargetLufsInput);
		AddSettingRow(TargetLufsRowLabel, Ctrl);
	}
	// --- 計測調整時、ピークの超過（赤クリップ）を防ぐか ---
	{
		LimitPeakRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("ピーク超過を防ぐ"), TEXT("Limit peak")), 22, TextPrimary, true);
		LimitPeakButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("LimitPeakButton"), &LimitPeakButtonText);
		UpdateLimitPeakButton();
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(160.f);
		B->SetMinDesiredHeight(64.f);
		B->AddChild(LimitPeakButton);
		AddSettingRow(LimitPeakRowLabel, B);
	}

	// =========================================================
	//  メーター（更新間隔 / 表示モード）
	// =========================================================
	MeterSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("メーター"), TEXT("Meter")), 18, AccentBlue, true);
	AddSectionHeader(MeterSectionLabel);

	// --- メーター受信 ON/OFF（ホーム画面のボタンと連動。デフォルトOFF）---
	{
		MeterRecvRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("メーター受信"), TEXT("Meter Recv")), 22, TextPrimary, true);
		MeterRecvSettingButton = MakeButton(WidgetTree, TEXT(""), StopGray, TEXT("MeterRecvSettingButton"), &MeterRecvSettingButtonText);
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(160.f);
		B->SetMinDesiredHeight(64.f);
		B->AddChild(MeterRecvSettingButton);
		AddSettingRow(MeterRecvRowLabel, B);
	}

	{
		MeterIntervalRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("更新間隔(ms)"), TEXT("Update interval (ms)")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("200"), TEXT("MeterIntervalInput"), FString::FromInt(MeterIntervalMs), 160.f, MeterIntervalInput);
		AddSettingRow(MeterIntervalRowLabel, Ctrl);
	}
	{
		MeterModeRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("表示"), TEXT("Display")), 22, TextPrimary, true);
		MeterModeButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("MeterModeButton"), &MeterModeButtonText);
		UpdateMeterModeButton();
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(240.f);
		B->SetMinDesiredHeight(64.f);
		B->AddChild(MeterModeButton);
		AddSettingRow(MeterModeRowLabel, B);
	}
	{
		MeterDotScopeRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("〇の数"), TEXT("Dot count")), 22, TextPrimary, true);
		MeterDotScopeButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("MeterDotScopeButton"), &MeterDotScopeButtonText);
		UpdateMeterDotScopeButton();
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(240.f);
		B->SetMinDesiredHeight(64.f);
		B->AddChild(MeterDotScopeButton);
		AddSettingRow(MeterDotScopeRowLabel, B);
	}
	// 簡易説明
	{
		MeterDescText = MakeText(WidgetTree, ReaperLang::S(
			TEXT("フルメーター＝詳細表示（やや重い）。信号〇＝緑:信号あり／赤:なし（軽い）。\n〇は「chごと」か「トラックに1つ」を選択可（1つは取得も減って最も軽い）。\n更新間隔を大きくするほど軽くなります。"),
			TEXT("Full meter = detailed (heavier). Signal dot = green:signal / red:none (light).\nDot can be 'per channel' or 'one per track' (one = fewest queries, lightest).\nLarger update interval = lighter.")),
			15, TextDim, false);
		MeterDescText->SetAutoWrapText(true);
		UScrollBoxSlot* S = Cast<UScrollBoxSlot>(Scroll->AddChild(MeterDescText));
		if (S) { S->SetPadding(FMargin(28.f, 6.f, 28.f, 10.f)); S->SetHorizontalAlignment(HAlign_Fill); }
	}

	// =========================================================
	//  トラック一覧の更新（自動更新 / 手動更新）
	// =========================================================
	TrackRefreshSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("トラック一覧の更新"), TEXT("Track List Refresh")), 18, AccentBlue, true);
	AddSectionHeader(TrackRefreshSectionLabel);

	{
		TrackRefreshModeRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("更新方法"), TEXT("Refresh mode")), 22, TextPrimary, true);
		TrackRefreshModeButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("TrackRefreshModeButton"), &TrackRefreshModeButtonText);
		UpdateTrackRefreshModeButton();
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(240.f);
		B->SetMinDesiredHeight(64.f);
		B->AddChild(TrackRefreshModeButton);
		AddSettingRow(TrackRefreshModeRowLabel, B);
	}
	{
		TrackRefreshIntervalRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("自動更新の間隔(秒)"), TEXT("Auto refresh interval (sec)")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("10"), TEXT("TrackRefreshIntervalInput"), FString::FromInt(TrackRefreshIntervalSec), 160.f, TrackRefreshIntervalInput);
		AddSettingRow(TrackRefreshIntervalRowLabel, Ctrl);
	}

	// =========================================================
	//  リスト（収録リスト：Excel/CSV 読み込み・書き出し）
	// =========================================================
	ListSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("リスト"), TEXT("List")), 18, AccentBlue, true);
	AddSectionHeader(ListSectionLabel);

	// 形式（Excel / CSV）
	{
		ListFormatRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("形式"), TEXT("Format")), 22, TextPrimary, true);
		ListFormatButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("ListFormatButton"), &ListFormatButtonText);
		UpdateListFormatButton();
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(200.f); B->SetMinDesiredHeight(64.f); B->AddChild(ListFormatButton);
		AddSettingRow(ListFormatRowLabel, B);
	}
	// 読み込むファイル（Excel/CSV用。Google Sheetsでは非表示）
	{
		ListFileRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("ファイル"), TEXT("File")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("C:\\path\\to\\list.xlsx"), TEXT("ListFilePathInput"), ListFilePath, 460.f, ListFilePathInput);
		ListFileRow = AddSettingRow(ListFileRowLabel, Ctrl);
	}
	// 読み込むシート（空＝全シート）
	{
		ListSheetRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("シート(空=全シート)"), TEXT("Sheet (empty=all)")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT(""), TEXT("ListSheetInput"), ListSheetName, 300.f, ListSheetInput);
		AddSettingRow(ListSheetRowLabel, Ctrl);
	}
	// 開始行
	{
		ListStartRowRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("開始行"), TEXT("Start row")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("1"), TEXT("ListStartRowInput"), FString::FromInt(ListStartRow), 140.f, ListStartRowInput);
		AddSettingRow(ListStartRowRowLabel, Ctrl);
	}
	// 開始列
	{
		ListStartColRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("開始列"), TEXT("Start col")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("A"), TEXT("ListStartColInput"), ExcelColumnLetter(ListStartCol), 140.f, ListStartColInput);
		AddSettingRow(ListStartColRowLabel, Ctrl);
	}
	// 列数（既定2）
	{
		ListColCountRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("列数"), TEXT("Columns")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("2"), TEXT("ListColCountInput"), FString::FromInt(ListColCount), 140.f, ListColCountInput);
		AddSettingRow(ListColCountRowLabel, Ctrl);
	}
	// 台本一致の対象範囲（シート/全シート）。チェック対象列は「台本一致」ボタン押下時にその場で選ばせる。
	{
		ScriptMatchScopeRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("台本一致の対象範囲"), TEXT("Script match scope")), 22, TextPrimary, true);
		ScriptMatchScopeButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("ScriptMatchScopeButton"), &ScriptMatchScopeButtonText);
		UpdateScriptMatchScopeButton();
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(200.f); B->SetMinDesiredHeight(64.f); B->AddChild(ScriptMatchScopeButton);
		AddSettingRow(ScriptMatchScopeRowLabel, B);
	}
	// 台本一致の対象行フィルタ（見ない/チェック済みのみ/未チェックのみ）
	{
		ScriptMatchCheckFilterRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("台本一致のチェック状態"), TEXT("Script match check state")), 22, TextPrimary, true);
		ScriptMatchCheckFilterButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("ScriptMatchCheckFilterButton"), &ScriptMatchCheckFilterButtonText);
		UpdateScriptMatchCheckFilterButton();
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(200.f); B->SetMinDesiredHeight(64.f); B->AddChild(ScriptMatchCheckFilterButton);
		AddSettingRow(ScriptMatchCheckFilterRowLabel, B);
	}
	// チェックボックス要否
	{
		ListCheckboxRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("チェックボックス"), TEXT("Checkbox")), 22, TextPrimary, true);
		ListCheckboxButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("ListCheckboxButton"), &ListCheckboxButtonText);
		UpdateListCheckboxButton();
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(160.f); B->SetMinDesiredHeight(64.f); B->AddChild(ListCheckboxButton);
		AddSettingRow(ListCheckboxRowLabel, B);
	}
	// 書き出し先（Excel/CSV用。Google Sheetsでは非表示、常に読み込み元と同じスプレッドシートに書き出す）
	{
		ListExportRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("書き出し先"), TEXT("Export to")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("C:\\path\\to\\result.csv"), TEXT("ListExportPathInput"), ListExportPath, 460.f, ListExportPathInput);
		ListExportRow = AddSettingRow(ListExportRowLabel, Ctrl);
	}
	// Google Sheets：スプレッドシートID（形式=Google Sheetsのときのみ表示）
	{
		ListGSheetIdRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("GoogleシートURL"), TEXT("Google Sheet URL")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(ReaperLang::S(TEXT("スプレッドシートのURLを貼り付け"), TEXT("Paste the spreadsheet URL")), TEXT("ListGSheetIdInput"), GoogleSheetSpreadsheetId, 460.f, ListGSheetIdInput);
		ListGSheetIdRow = AddSettingRow(ListGSheetIdRowLabel, Ctrl);
	}
	// Google Sheets：クライアントID（OAuthユーザーアカウント接続用。形式=Google Sheetsのときのみ表示）
	{
		ListGSheetClientIdRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("GoogleSheet クライアントID"), TEXT("GoogleSheet Client ID")), 22, TextPrimary, true);
		UWidget* Ctrl = MakeFixedInput(TEXT("クライアントID"), TEXT("ListGSheetClientIdInput"), GoogleSheetClientId, 460.f, ListGSheetClientIdInput);
		ListGSheetClientIdRow = AddSettingRow(ListGSheetClientIdRowLabel, Ctrl);
	}
	UpdateListFormatRowsVisibility();

	// =========================================================
	//  アプリ設定（言語 / マイク / モニター）
	// =========================================================
	AppSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("アプリ"), TEXT("App")), 18, AccentBlue, true);
	AddSectionHeader(AppSectionLabel);

	// --- 言語切替（9言語：日本語/English/Français/Italiano/Deutsch/Español/한국어/简体中文/繁體中文）---
	LanguageRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("言語"), TEXT("Language")), 22, TextPrimary, true);
	LanguageCombo = MakeDarkCombo(WidgetTree);
	LanguageCombo->OnGenerateWidgetEvent.BindDynamic(this, &UWG_RadNodzToolkit::MakeComboItemLight);
	LanguageCombo->OnSelectionChanged.AddDynamic(this, &UWG_RadNodzToolkit::HandleLanguageComboChanged);
	for (uint8 i = 0; i < static_cast<uint8>(ReaperLang::ELanguage::Count); ++i)
	{
		LanguageCombo->AddOption(ReaperLang::NativeName(static_cast<ReaperLang::ELanguage>(i)));
	}
	RefreshLanguageCombo();
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(200.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(LanguageCombo);
		AddSettingRow(LanguageRowLabel, BtnSize);
	}

	// --- テーマ（背景の明るさ）切替（標準 / さらに暗め / 明るめ をトグル）---
	ThemeRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("テーマ(背景)"), TEXT("Theme (BG)")), 22, TextPrimary, true);
	ThemeButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("ThemeButton"), &ThemeButtonText);
	UpdateThemeButton();
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(220.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(ThemeButton);
		AddSettingRow(ThemeRowLabel, BtnSize);
	}

	// --- 画面の向き固定（自動 / 縦 / 横 をトグル）---
	OrientationRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("画面の向き"), TEXT("Screen Orientation")), 22, TextPrimary, true);
	OrientationButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("OrientationButton"), &OrientationButtonText);
	if (OrientationButtonText) { OrientationButtonText->SetAutoWrapText(false); }   // 自動/縦/横は短いので改行させない
	UpdateOrientationButton();
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(200.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(OrientationButton);
		AddSettingRow(OrientationRowLabel, BtnSize);
	}

	// --- マイク登録（マイク設定ダイアログを開く）---
	MicSettingsRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("マイク登録"), TEXT("Mic Registration")), 22, TextPrimary, true);
	MicSettingsButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("開く"), TEXT("Open")), StopGray,
		TEXT("MicSettingsButton"), &MicSettingsButtonText, TEXT("/Game/Texture/T_icon_setting_1.T_icon_setting_1"));
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(180.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(MicSettingsButton);
		AddSettingRow(MicSettingsRowLabel, BtnSize);
	}

	// --- マイク入力（本体マイク / Bluetoothマイク を切り替え）---
	MicInputRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("マイク入力"), TEXT("Mic Input")), 22, TextPrimary, true);
	MicInputButton = MakeButton(WidgetTree, TEXT(""), AccentBlue,
		TEXT("MicInputButton"), &MicInputButtonText);
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(220.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(MicInputButton);
		AddSettingRow(MicInputRowLabel, BtnSize);
	}
	UpdateMicInputModeButton();

	// --- モニター出力（ステレオへダウンミックス / 受信chそのまま）---
	MonitorOutputRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("モニター出力"), TEXT("Monitor Output")), 22, TextPrimary, true);
	MonitorOutputButton = MakeButton(WidgetTree, TEXT(""), AccentBlue,
		TEXT("MonitorOutputButton"), &MonitorOutputButtonText);
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(240.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(MonitorOutputButton);
		AddSettingRow(MonitorOutputRowLabel, BtnSize);
	}
	UpdateMonitorOutputButton();

	// --- 受信バッファ（遅延 / 音飛び対策）---
	MonitorBufferRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("受信バッファ"), TEXT("Receive Buffer")), 22, TextPrimary, true);
	MonitorBufferButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("MonitorBufferButton"), &MonitorBufferButtonText);
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(200.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(MonitorBufferButton);
		AddSettingRow(MonitorBufferRowLabel, BtnSize);
	}
	UpdateMonitorBufferButton();

	// --- ダウンミックス係数（受信chごとに係数を一覧入力。ステレオダウンミックス時のみ表示）---
	StereoOnlyWidgets.Reset();
	DmCoeffRowLabels.Reset();
	DmCoeffInputs.Reset();
	EnsureDmGainsDefaults();

	// セクション見出し
	DownmixSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("ダウンミックス係数"), TEXT("Downmix Coeff")), 18, AccentBlue, true);
	AddSectionHeader(DownmixSectionLabel);
	StereoOnlyWidgets.Add(DownmixSectionLabel);

	// 各受信ch（標準レイアウト L R C LFE Ls Rs Lb Rb）の係数を1行ずつ入力欄で並べる
	for (int32 i = 0; i < NumDownmixChannels; ++i)
	{
		const TCHAR* LabelJP = nullptr;
		const TCHAR* LabelEN = nullptr;
		DmChannelLabel(i, LabelJP, LabelEN);

		UTextBlock* RowLabel = MakeText(WidgetTree, ReaperLang::S(LabelJP, LabelEN), 22, TextPrimary, true);
		DmCoeffRowLabels.Add(RowLabel);

		UEditableTextBox* Box = MakeInput(WidgetTree, TEXT("0.00"),
			FName(*FString::Printf(TEXT("DmCoeffInput_%d"), i)));
		Box->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), DmGains.IsValidIndex(i) ? DmGains[i] : 0.f)));
		Box->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleDmCoeffCommitted);
		DmCoeffInputs.Add(Box);

		USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		SB->SetMinDesiredWidth(140.f);
		SB->AddChild(Box);

		StereoOnlyWidgets.Add(AddSettingRow(RowLabel, SB));
	}

	// =========================================================
	//  立体音響（バイノーラルの調整：ITD / 頭部減衰 / ch順）
	//  ※モニター出力=立体音響 のときのみ表示する
	// =========================================================
	SpatialOnlyWidgets.Reset();
	CustomOnlyWidgets.Reset();
	SpatialSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("立体音響"), TEXT("3D Spatial")), 18, AccentBlue, true);
	AddSectionHeader(SpatialSectionLabel);
	SpatialOnlyWidgets.Add(SpatialSectionLabel);

	// 立体音響はHRTF前提。HRTF時に使われない「立体感(ITD)」「頭部減衰」設定は表示しない。
	// （簡易バイノーラルへフォールバックする場合は内蔵の既定値で動作する）

	// ch順マッピングは常にカスタム固定（選択UIは廃止）。カスタム配置エディタのみを出す。
	SpatialChannelOrder = EBinauralChannelOrder::Custom;

	// HRTFプロファイル（簡易(内蔵)＋同梱プロファイルからドロップダウンで選択）
	SpatialHrtfRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("HRTF"), TEXT("HRTF")), 22, TextPrimary, true);
	HrtfCombo = MakeDarkCombo(WidgetTree);
	HrtfCombo->OnGenerateWidgetEvent.BindDynamic(this, &UWG_RadNodzToolkit::MakeComboItemLight);
	HrtfCombo->OnSelectionChanged.AddDynamic(this, &UWG_RadNodzToolkit::HandleHrtfComboChanged);
	RefreshHrtfCombo();   // 選択肢（簡易＋登録プロファイル）を埋めて現在値を選択
	{
		USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		SB->SetMinDesiredWidth(260.f);
		SB->SetMinDesiredHeight(64.f);
		SB->AddChild(HrtfCombo);
		SpatialOnlyWidgets.Add(AddSettingRow(SpatialHrtfRowLabel, SB));
	}

	// --- カスタムch配置エディタ（ch順=カスタム のとき有効）---
	EnsureCustomLayoutDefaults();

	// 配置図（中心＝リスナー、周囲に各ch）
	{
		UBorder* DiagBg = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("MappingDiagramBg"));
		DiagBg->SetBrushColor(ListBg);
		ThemeListBgBorders.Add(DiagBg);
		DiagBg->SetPadding(FMargin(8.f));
		DiagBg->SetHorizontalAlignment(HAlign_Center);
		DiagBg->SetVerticalAlignment(VAlign_Center);

		USizeBox* DiagSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		DiagSize->SetWidthOverride(360.f);
		DiagSize->SetHeightOverride(360.f);
		DiagBg->SetContent(DiagSize);

		MappingCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("MappingCanvas"));
		DiagSize->AddChild(MappingCanvas);

		// 中心のリスナー表示
		{
			UTextBlock* Center = MakeText(WidgetTree, TEXT("●"), 22, TextDim, true);
			Center->SetJustification(ETextJustify::Center);
			if (UCanvasPanelSlot* CS = Cast<UCanvasPanelSlot>(MappingCanvas->AddChild(Center)))
			{
				CS->SetAnchors(FAnchors(0.5f, 0.5f));
				CS->SetAlignment(FVector2D(0.5f, 0.5f));
				CS->SetAutoSize(true);
				CS->SetPosition(FVector2D(0.f, 0.f));
			}
		}

		// 各chの点（最大8）
		MappingDots.Reset();
		MappingDotLabels.Reset();
		for (int32 i = 0; i < MaxMappingChannels; ++i)
		{
			UBorder* Dot = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
			Dot->SetBrushColor(AccentBlue);
			Dot->SetPadding(FMargin(2.f));
			Dot->SetHorizontalAlignment(HAlign_Center);
			Dot->SetVerticalAlignment(VAlign_Center);

			UTextBlock* Lbl = MakeText(WidgetTree, FString::FromInt(i + 1), 16, FLinearColor(0.02f, 0.05f, 0.06f, 1.f), true);
			Lbl->SetJustification(ETextJustify::Center);
			Dot->SetContent(Lbl);

			if (UCanvasPanelSlot* CS = Cast<UCanvasPanelSlot>(MappingCanvas->AddChild(Dot)))
			{
				CS->SetAnchors(FAnchors(0.5f, 0.5f));
				CS->SetAlignment(FVector2D(0.5f, 0.5f));
				CS->SetSize(FVector2D(38.f, 38.f));
				CS->SetPosition(FVector2D(0.f, 0.f));
			}
			MappingDots.Add(Dot);
			MappingDotLabels.Add(Lbl);
		}

		UScrollBoxSlot* SS = Cast<UScrollBoxSlot>(Scroll->AddChild(DiagBg));
		if (SS) { SS->SetPadding(FMargin(0.f, 8.f, 0.f, 0.f)); SS->SetHorizontalAlignment(HAlign_Fill); }
		CustomOnlyWidgets.Add(DiagBg);
	}

	// 編集ch選択
	MapEditChRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("編集ch"), TEXT("Edit ch")), 22, TextPrimary, true);
	MapEditChButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("MapEditChButton"), &MapEditChButtonText);
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(140.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(MapEditChButton);
		CustomOnlyWidgets.Add(AddSettingRow(MapEditChRowLabel, BtnSize));
	}

	// 角度 [−][値][＋]
	MapAngleRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("角度"), TEXT("Angle")), 22, TextPrimary, true);
	{
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		MapAngleMinusButton = MakeButton(WidgetTree, TEXT("−"), StopGray, TEXT("MapAngleMinusButton"));
		{ USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass()); B->SetMinDesiredWidth(64.f); B->SetMinDesiredHeight(60.f); B->AddChild(MapAngleMinusButton);
		  UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(B); S->SetVerticalAlignment(VAlign_Center); S->SetPadding(FMargin(0.f,0.f,8.f,0.f)); }
		MapAngleValueText = MakeText(WidgetTree, TEXT("0°"), 22, TextPrimary, true);
		MapAngleValueText->SetJustification(ETextJustify::Center);
		{ USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass()); B->SetMinDesiredWidth(110.f); B->AddChild(MapAngleValueText);
		  UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(B); S->SetVerticalAlignment(VAlign_Center); }
		MapAnglePlusButton = MakeButton(WidgetTree, TEXT("＋"), StopGray, TEXT("MapAnglePlusButton"));
		{ USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass()); B->SetMinDesiredWidth(64.f); B->SetMinDesiredHeight(60.f); B->AddChild(MapAnglePlusButton);
		  UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(B); S->SetVerticalAlignment(VAlign_Center); S->SetPadding(FMargin(8.f,0.f,0.f,0.f)); }
		CustomOnlyWidgets.Add(AddSettingRow(MapAngleRowLabel, H));
	}

	// 距離(distance)はHRTFでは未使用のため表示しない（HRIRは方位ベースで畳み込む）。

	UpdateSpatialButtons();
	UpdateMappingControls();
	UpdateSpatialSettingsVisibility();

	// =========================================================
	//  ライセンス / クレジット（ライセンス表記を持つHRTFが登録されているときのみ表示）
	// =========================================================
	{
		TArray<FString> Attrs;
		FReaperHRTFRegistry::Get().GetAttributions(Attrs);
		if (Attrs.Num() > 0)
		{
			LicenseSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("ライセンス"), TEXT("Licenses")), 18, AccentBlue, true);
			AddSectionHeader(LicenseSectionLabel);

			const FString Body = FString::Join(Attrs, TEXT("\n\n"));
			LicenseText = MakeText(WidgetTree, Body, 14, TextDim);
			LicenseText->SetAutoWrapText(true);
			if (UScrollBoxSlot* S = Cast<UScrollBoxSlot>(Scroll->AddChild(LicenseText)))
			{
				S->SetPadding(FMargin(28.f, 4.f, 28.f, 24.f));
				S->SetHorizontalAlignment(HAlign_Fill);
			}
		}
	}

	return Page;
}

// =============================================================
//  下部タブバー（ホーム / 設定）の構築
// =============================================================
UWidget* UWG_RadNodzToolkit::BuildBottomNav()
{
	using namespace ReaperCtrlUI;

	UBorder* Nav = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("BottomNav"));
	Nav->SetBrushColor(HeaderBg);
	ThemeHeaderBgBorders.Add(Nav);
	Nav->SetPadding(FMargin(0.f));
	Nav->SetHorizontalAlignment(HAlign_Fill);

	UHorizontalBox* NavRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	Nav->SetContent(NavRow);

	// タブ1つ分のセル（上部にアクティブ表示の細線＋ボタン）を作る
	auto MakeTabCell = [&](const FString& Label, FName BtnName, const TCHAR* IconPath,
		TObjectPtr<UButton>& OutBtn, TObjectPtr<UTextBlock>& OutText, TObjectPtr<UBorder>& OutIndicator) -> UWidget*
	{
		UVerticalBox* Cell = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());

		// アクティブ表示の細線（上端）
		UBorder* Indicator = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
		Indicator->SetBrushColor(AccentBlue);
		Indicator->SetPadding(FMargin(0.f));
		{
			USizeBox* IndH = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			IndH->SetHeightOverride(3.f);
			IndH->AddChild(Indicator);
			UVerticalBoxSlot* S = Cell->AddChildToVerticalBox(IndH);
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		// 透明背景のボタン＋中央ラベル
		UButton* Btn = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), BtnName);
		{
			FButtonStyle Style = Btn->GetStyle();
			const FSlateColorBrush Clear(FLinearColor(0.f, 0.f, 0.f, 0.f));
			const FSlateColorBrush Press(FLinearColor(1.f, 1.f, 1.f, 0.06f));
			Style.Normal = Clear;
			Style.Hovered = Press;
			Style.Pressed = Press;
			Style.Disabled = Clear;
			Btn->SetStyle(Style);
		}
		UTextBlock* Txt = MakeText(WidgetTree, Label, 18, TextDim, true);
		Txt->SetJustification(ETextJustify::Center);

		// アイコンがあれば [アイコン][ラベル] を中央寄せで横並びにする
		UTexture2D* IconTex = IconPath ? LoadObject<UTexture2D>(nullptr, IconPath) : nullptr;
		if (IconTex)
		{
			UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			{ UTextBlock* Sp = MakeText(WidgetTree, TEXT(""), 1, TextDim); UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(Sp); S->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); }
			{
				UImage* Img = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
				Img->SetBrushFromTexture(IconTex);
				Img->SetDesiredSizeOverride(FVector2D(34.f, 34.f));
				UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(Img);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));
			}
			{ UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(Txt); S->SetVerticalAlignment(VAlign_Center); }
			{ UTextBlock* Sp = MakeText(WidgetTree, TEXT(""), 1, TextDim); UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(Sp); S->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); }
			Btn->SetContent(Row);
		}
		else
		{
			Btn->SetContent(Txt);
		}
		{
			// ボタン自体を高めにして、タップしやすい大きな押下領域にする（余白では押せる範囲は増えないため）
			USizeBox* BtnH = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BtnH->SetMinDesiredHeight(92.f);
			BtnH->AddChild(Btn);
			UVerticalBoxSlot* S = Cell->AddChildToVerticalBox(BtnH);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetPadding(FMargin(0.f, 4.f, 0.f, 6.f));
		}

		OutBtn = Btn;
		OutText = Txt;
		OutIndicator = Indicator;
		return Cell;
	};

	// ホームタブ
	{
		UWidget* Cell = MakeTabCell(ReaperLang::S(TEXT("ホーム"), TEXT("Home")), TEXT("HomeTabButton"),
			TEXT("/Game/Texture/T_icon_home_1.T_icon_home_1"),
			HomeTabButton, HomeTabText, HomeTabIndicator);
		UHorizontalBoxSlot* S = NavRow->AddChildToHorizontalBox(Cell);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}
	// リストタブ
	{
		UWidget* Cell = MakeTabCell(ReaperLang::S(TEXT("リスト"), TEXT("List")), TEXT("ListTabButton"),
			TEXT("/Game/Texture/T_icon_list_1.T_icon_list_1"),
			ListTabButton, ListTabText, ListTabIndicator);
		UHorizontalBoxSlot* S = NavRow->AddChildToHorizontalBox(Cell);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}
	// 設定タブ
	{
		UWidget* Cell = MakeTabCell(ReaperLang::S(TEXT("設定"), TEXT("Settings")), TEXT("SettingsTabButton"),
			TEXT("/Game/Texture/T_icon_setting_1.T_icon_setting_1"),
			SettingsTabButton, SettingsTabText, SettingsTabIndicator);
		UHorizontalBoxSlot* S = NavRow->AddChildToHorizontalBox(Cell);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}

	return Nav;
}

void UWG_RadNodzToolkit::SetActiveTab(int32 TabIndex)
{
	using namespace ReaperCtrlUI;

	ActiveTab = TabIndex;
	if (ContentSwitcher)
	{
		ContentSwitcher->SetActiveWidgetIndex(TabIndex);
	}

	// 0=ホーム / 1=リスト / 2=設定
	auto SetTab = [&](UTextBlock* Txt, UBorder* Ind, bool bActive)
	{
		if (Txt) { Txt->SetColorAndOpacity(FSlateColor(bActive ? AccentBlue : TextDim)); }
		if (Ind) { Ind->SetVisibility(bActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden); }
	};
	SetTab(HomeTabText,     HomeTabIndicator,     TabIndex == 0);
	SetTab(ListTabText,     ListTabIndicator,     TabIndex == 1);
	SetTab(SettingsTabText, SettingsTabIndicator, TabIndex == 2);
}

void UWG_RadNodzToolkit::HandleHomeTabClicked()
{
	SetActiveTab(0);
}

void UWG_RadNodzToolkit::HandleListTabClicked()
{
	SetActiveTab(1);
}

void UWG_RadNodzToolkit::HandleSettingsTabClicked()
{
	SetActiveTab(2);
}

// ====================================================================
//  「リスト」タブ（収録リスト：Excel/CSV読み込み・チェック・書き出し）
// ====================================================================
UWidget* UWG_RadNodzToolkit::BuildListPage()
{
	using namespace ReaperCtrlUI;

	UBorder* Page = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ListPage"));
	Page->SetBrushColor(ScreenBg);
	ThemeScreenBgBorders.Add(Page);
	Page->SetPadding(FMargin(20.f, 16.f));
	Page->SetHorizontalAlignment(HAlign_Fill);
	Page->SetVerticalAlignment(VAlign_Fill);

	UVerticalBox* Col = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	Page->SetContent(Col);

	// 上部：タイトル＋操作ボタン（読み込み / 書き出し / Slack）
	{
		UHorizontalBox* Top = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		ListTitleText = MakeText(WidgetTree, ReaperLang::S(TEXT("収録リスト"), TEXT("Recording List")), 24, TextPrimary, true);
		ListTitleText->SetAutoWrapText(true);   // 長い翻訳文字列（伊語等）がボタンと重ならないよう2行に折り返す
		{ UHorizontalBoxSlot* S = Top->AddChildToHorizontalBox(ListTitleText); S->SetVerticalAlignment(VAlign_Center); S->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); }

		UButton* LoadBtn = MakeButton(WidgetTree, ReaperLang::S(TEXT("読み込み"), TEXT("Load")), BtnBlueDark, TEXT("ListLoadButton"), &ListLoadButtonText, TEXT("/Game/Texture/T_icon_open_1.T_icon_open_1"));
		LoadBtn->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleListLoadClicked);
		{ USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass()); B->SetMinDesiredWidth(160.f); B->SetMinDesiredHeight(64.f); B->AddChild(LoadBtn);
		  UHorizontalBoxSlot* S = Top->AddChildToHorizontalBox(B); S->SetVerticalAlignment(VAlign_Center); S->SetPadding(FMargin(8.f,0.f,0.f,0.f)); }

		// 「書き出し」ボタンは一旦非表示（HandleListExportClicked等の内部コード・設定は温存）

		UButton* SlackBtn = MakeButton(WidgetTree, TEXT("Slack"), BtnGreenDark, TEXT("ListSlackButton"), nullptr, TEXT("/Game/Texture/T_icon_slack_1.T_icon_slack_1"));
		SlackBtn->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleListSlackClicked);
		{ USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass()); B->SetMinDesiredWidth(120.f); B->SetMinDesiredHeight(64.f); B->AddChild(SlackBtn);
		  UHorizontalBoxSlot* S = Top->AddChildToHorizontalBox(B); S->SetVerticalAlignment(VAlign_Center); S->SetPadding(FMargin(8.f,0.f,0.f,0.f)); }

		UButton* RecCheckBtn = MakeButton(WidgetTree, ReaperLang::S(TEXT("台本一致"), TEXT("Script Match")), BtnBlueDark, TEXT("RecordingCheckButton"), &RecordingCheckButtonText, TEXT("/Game/Texture/T_icon_recCheck_1.T_icon_recCheck_1"));
		RecCheckBtn->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleRecordingCheckClicked);
		{ USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass()); B->SetMinDesiredWidth(180.f); B->SetMinDesiredHeight(64.f); B->AddChild(RecCheckBtn);
		  UHorizontalBoxSlot* S = Top->AddChildToHorizontalBox(B); S->SetVerticalAlignment(VAlign_Center); S->SetPadding(FMargin(8.f,0.f,0.f,0.f)); }

		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(Top);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
	}

	// 状態表示
	ListStatusText = MakeText(WidgetTree, ReaperLang::S(TEXT("「読み込み」で設定したファイルを表示します。"), TEXT("Press Load to show the configured file.")), 16, TextDim, false);
	{
		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(ListStatusText);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 8.f));
	}

	// （マイク登録UIは「設定 → マイク登録」ダイアログ側に集約。リスト側には置かない）

	// 台本一致：文字起こしチェック対象列を選ばせる列ピッカー（「台本一致」ボタン押下時のみ表示）
	{
		UBorder* PickerBg = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ScriptMatchColumnPickerBg"));
		PickerBg->SetBrushColor(HeaderBg);
		ThemeHeaderBgBorders.Add(PickerBg);
		PickerBg->SetPadding(FMargin(10.f, 8.f));
		PickerBg->SetHorizontalAlignment(HAlign_Fill);
		PickerBg->SetVisibility(ESlateVisibility::Collapsed);
		ScriptMatchColumnPickerBg = PickerBg;

		UScrollBox* PickerScroll = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("ScriptMatchColumnPickerScroll"));
		PickerScroll->SetOrientation(Orient_Horizontal);
		PickerScroll->SetScrollBarVisibility(ESlateVisibility::Collapsed);
		PickerScroll->SetConsumeMouseWheel(EConsumeMouseWheel::Always);
		PickerBg->SetContent(PickerScroll);

		ScriptMatchColumnPickerBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ScriptMatchColumnPickerBox"));
		PickerScroll->AddChild(ScriptMatchColumnPickerBox);

		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(PickerBg);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
	}

	// 行一覧（スクロール）
	{
		UBorder* ListBg2 = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ListBg"));
		ListBg2->SetBrushColor(ListBg);
		ThemeListBgBorders.Add(ListBg2);
		ListBg2->SetPadding(FMargin(12.f, 10.f));

		// 外＝横スクロール、内＝縦スクロールの入れ子。
		// これで「行が画面幅を超えたら横スクロール」「行数が多ければ縦スクロール」を両立する。
		UScrollBox* HScroll = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("ListHScroll"));
		HScroll->SetOrientation(Orient_Horizontal);
		ListBg2->SetContent(HScroll);

		UScrollBox* VScroll = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("ListScroll"));
		VScroll->SetOrientation(Orient_Vertical);
		HScroll->AddChild(VScroll);

		ListRowsBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("ListRowsBox"));
		VScroll->AddChild(ListRowsBox);

		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(ListBg2);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}

	// 下部：シート切替（下部ナビと同じタブ式。横幅を超えたら横スクロール）
	{
		ListSheetTabRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		UBorder* TabBg = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("SheetTabBg"));
		TabBg->SetBrushColor(HeaderBg);
		ThemeHeaderBgBorders.Add(TabBg);
		TabBg->SetPadding(FMargin(0.f));
		TabBg->SetHorizontalAlignment(HAlign_Fill);

		ListSheetScroll = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("SheetTabScroll"));
		ListSheetScroll->SetOrientation(Orient_Horizontal);
		ListSheetScroll->SetScrollBarVisibility(ESlateVisibility::Collapsed);
		ListSheetScroll->SetConsumeMouseWheel(EConsumeMouseWheel::Always);
		TabBg->SetContent(ListSheetScroll);

		ListSheetTabsBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("SheetTabsBox"));
		ListSheetScroll->AddChild(ListSheetTabsBox);

		{ UHorizontalBoxSlot* S = ListSheetTabRow->AddChildToHorizontalBox(TabBg); S->SetVerticalAlignment(VAlign_Center); S->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); }

		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(ListSheetTabRow);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 10.f, 0.f, 0.f));
	}

	return Page;
}

void UWG_RadNodzToolkit::RebuildSheetTabs()
{
	if (!ListSheetTabsBox) { return; }

	ListSheetTabsBox->ClearChildren();
	ListSheetTabWidgets.Reset();

	for (int32 i = 0; i < ListSheets.Num(); ++i)
	{
		UWG_SheetTab* Tab = CreateWidget<UWG_SheetTab>(this, UWG_SheetTab::StaticClass());
		if (!Tab) { continue; }
		Tab->InitTab(i, ListSheets[i].SheetName, i == ListActiveSheet, this);
		UHorizontalBoxSlot* S = ListSheetTabsBox->AddChildToHorizontalBox(Tab);
		S->SetVerticalAlignment(VAlign_Fill);
		ListSheetTabWidgets.Add(Tab);
	}
}

void UWG_RadNodzToolkit::RebuildListView()
{
	if (!ListRowsBox) return;
	using namespace ReaperCtrlUI;

	ListRowsBox->ClearChildren();
	ListRowWidgets.Reset();

	if (!ListSheets.IsValidIndex(ListActiveSheet)) { return; }
	const FListSheet& Sheet = ListSheets[ListActiveSheet];

	// 表示する行ぶんのセル配列を先に組み立てる。
	// Row.Cells自体は書き出し対象データのため変更せず、表示用の一時配列に組み立てる。
	// 先頭に行番号（1始まり）を追加し、どの行が何行目かひと目で分かるようにする。
	// （空欄行自体は読み込み時点で既に除外済みのため、ここでは一致度による非表示は行わない。
	//   台本一致を全シート対象で実行した後に0%一致の行まで消えてしまい、リストが空に見える不具合があった）
	struct FDisplayRow { int32 RowIndex; TArray<FString> Cells; bool bChecked; };
	TArray<FDisplayRow> DisplayRows;
	int32 BaseColCount = 0;   // 台本一致の結果列を除いた、読み込み元データそのものの列数
	for (int32 i = 0; i < Sheet.Rows.Num(); ++i)
	{
		const FListRow& Row = Sheet.Rows[i];

		BaseColCount = FMath::Max(BaseColCount, Row.Cells.Num());

		FDisplayRow D;
		D.RowIndex = i;
		D.bChecked = Row.bChecked;
		D.Cells.Add(FString::FromInt(i + 1));
		D.Cells.Append(Row.Cells);
		if (Row.MatchScore >= 0)
		{
			// 一致度→文字起こし結果の順で末尾に追加する（未計算(-1)のときは追加しない）
			D.Cells.Add(FString::Printf(TEXT("%d%%"), Row.MatchScore));
			D.Cells.Add(Row.TranscribedMatch);
		}
		DisplayRows.Add(MoveTemp(D));
	}

	// 見出し行のセル：[# / A / B / C ... /（一致度・文字起こし結果があれば追加）]
	// 行番号列ぶん+1したindexが、台本一致のチェック対象列（ScriptMatchHighlightCol、0始まり）に対応する。
	int32 MaxCols = 0;
	for (const FDisplayRow& D : DisplayRows) { MaxCols = FMath::Max(MaxCols, D.Cells.Num()); }
	TArray<FString> HeaderCells;
	HeaderCells.Add(TEXT("#"));
	for (int32 c = 0; c < BaseColCount; ++c) { HeaderCells.Add(ExcelColumnLetter(c + 1)); }
	if (MaxCols > BaseColCount + 1)
	{
		HeaderCells.Add(ReaperLang::S(TEXT("一致度"), TEXT("Match")));
		HeaderCells.Add(ReaperLang::S(TEXT("文字起こし結果"), TEXT("Transcribed")));
	}

	// 列ごとの最大幅を実測し、行によって開始位置がズレないよう全行（見出し含む）へ同じ幅を渡す。
	TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	TArray<float> ColumnWidths;
	auto MeasureRow = [&](const TArray<FString>& RowCells, bool bBoldFirst)
	{
		for (int32 c = 0; c < RowCells.Num(); ++c)
		{
			const FSlateFontInfo Font = ReaperFont::Get(20, bBoldFirst && c == 0);
			const float W = FontMeasure->Measure(RowCells[c], Font).X + 12.f;   // クリップ防止の余白
			if (!ColumnWidths.IsValidIndex(c)) { ColumnWidths.Add(W); }
			else { ColumnWidths[c] = FMath::Max(ColumnWidths[c], W); }
		}
	};
	MeasureRow(HeaderCells, true);
	for (const FDisplayRow& D : DisplayRows) { MeasureRow(D.Cells, true); }

	// 台本一致でチェック対象に選んだ列（表示上は行番号列ぶん+1したindex）をハイライトする。
	const int32 DisplayHighlightCol = (ScriptMatchHighlightCol >= 0) ? ScriptMatchHighlightCol + 1 : INDEX_NONE;

	// 見出し行
	{
		UWG_ListRow* HeaderRow = CreateWidget<UWG_ListRow>(this, UWG_ListRow::StaticClass());
		if (HeaderRow)
		{
			HeaderRow->InitRow(INDEX_NONE, HeaderCells, false, bListUseCheckbox, this, ColumnWidths, /*bInHeaderRow=*/true, DisplayHighlightCol);
			UVerticalBoxSlot* S = ListRowsBox->AddChildToVerticalBox(HeaderRow);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
		}
	}

	for (const FDisplayRow& D : DisplayRows)
	{
		UWG_ListRow* RowW = CreateWidget<UWG_ListRow>(this, UWG_ListRow::StaticClass());
		if (!RowW) continue;
		RowW->InitRow(D.RowIndex, D.Cells, D.bChecked, bListUseCheckbox, this, ColumnWidths, /*bInHeaderRow=*/false, DisplayHighlightCol);
		UVerticalBoxSlot* S = ListRowsBox->AddChildToVerticalBox(RowW);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
		ListRowWidgets.Add(RowW);
	}
}

void UWG_RadNodzToolkit::SwitchListSheet(int32 SheetIndex)
{
	if (!ListSheets.IsValidIndex(SheetIndex)) return;
	ListActiveSheet = SheetIndex;
	// タブのアクティブ表示を更新
	for (int32 i = 0; i < ListSheetTabWidgets.Num(); ++i)
	{
		if (ListSheetTabWidgets[i]) { ListSheetTabWidgets[i]->SetActiveVisual(i == ListActiveSheet); }
	}
	RebuildListView();
}

void UWG_RadNodzToolkit::SetListRowChecked(int32 RowIndex, bool bChecked)
{
	if (!ListSheets.IsValidIndex(ListActiveSheet)) return;
	FListSheet& Sheet = ListSheets[ListActiveSheet];
	if (Sheet.Rows.IsValidIndex(RowIndex)) { Sheet.Rows[RowIndex].bChecked = bChecked; }
}

// ch数 → レイアウト名（1=モノ / 2=ステレオ / 6=5.1 / 8=7.1 …）
UWidget* UWG_RadNodzToolkit::MakeComboItem(FString Item)
{
	using namespace ReaperCtrlUI;
	// 暗背景でも見えるよう明るい文字色のテキストを返す
	return MakeText(WidgetTree, Item, 18, FieldText, false);
}

UWidget* UWG_RadNodzToolkit::MakeServerIdComboItem(FString Item)
{
	// サーバーIDコンボは白文字（ドロップダウン項目・選択表示の両方に適用される）。
	UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	T->SetText(FText::FromString(Item));
	T->SetFont(ReaperFont::Get(18, false));
	T->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	return T;
}

void UWG_RadNodzToolkit::GetMicCatalog(TArray<FString>& OutNames, TArray<int32>& OutChannels) const
{
	OutNames.Reset();
	OutChannels.Reset();

	// 「マイク」「Mic」を含むシートを探す
	int32 SheetIdx = -1;
	for (int32 i = 0; i < ListSheets.Num(); ++i)
	{
		const FString N = ListSheets[i].SheetName;
		if (N.Contains(TEXT("マイク")) || N.Contains(TEXT("Mic"), ESearchCase::IgnoreCase))
		{
			SheetIdx = i;
			break;
		}
	}
	if (SheetIdx < 0) return;

	// 各行：1列目＝マイク名 / 2列目＝ch数（無ければ1扱い）
	for (const FListRow& Row : ListSheets[SheetIdx].Rows)
	{
		if (Row.Cells.Num() > 0 && !Row.Cells[0].IsEmpty())
		{
			OutNames.Add(Row.Cells[0]);
			const int32 Ch = (Row.Cells.Num() > 1) ? FCString::Atoi(*Row.Cells[1].TrimStartAndEnd()) : 0;
			OutChannels.Add(FMath::Max(1, Ch));
		}
	}
}

void UWG_RadNodzToolkit::HandleListLoadClicked()
{
	if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("読み込み中…"), TEXT("Loading…")))); }

	TArray<FListSheet> Sheets;
	FString Err;
	const bool bOk = (ListFormat == 1) ? ReadListFromCsv(Sheets, Err)
		: (ListFormat == 2) ? ReadListFromGSheet(Sheets, Err)
		: ReadListFromExcel(Sheets, Err);

	if (!bOk)
	{
		if (ListStatusText) { ListStatusText->SetText(FText::FromString(Err)); }
		return;
	}

	ListSheets = MoveTemp(Sheets);
	ListActiveSheet = 0;
	ScriptMatchHighlightCol = -1;   // 新しく読み込んだデータには前回選んだ列のハイライトを持ち越さない

	// シート切替タブを作り直す（下部ナビ風・はみ出したら横スクロール）
	RebuildSheetTabs();

	RebuildListView();

	int32 Total = 0;
	for (const FListSheet& S : ListSheets) { Total += S.Rows.Num(); }
	if (ListStatusText)
	{
		ListStatusText->SetText(FText::FromString(FString::Printf(
			TEXT("%s: %d %s / %d %s"),
			*ReaperLang::S(TEXT("読み込み完了"), TEXT("Loaded")),
			ListSheets.Num(), *ReaperLang::S(TEXT("シート"), TEXT("sheets")),
			Total, *ReaperLang::S(TEXT("行"), TEXT("rows")))));
	}
}

// 列番号(1始まり)を Excel の列文字(A,B,...,AA)へ変換
static FString ExcelColumnLetter(int32 Col)
{
	FString S;
	while (Col > 0)
	{
		const int32 R = (Col - 1) % 26;
		S = FString::Chr((TCHAR)('A' + R)) + S;
		Col = (Col - 1) / 26;
	}
	return S.IsEmpty() ? TEXT("A") : S;
}

// Excel の列文字(A,B,...,AA)を列番号(1始まり)へ変換。英字以外を含む場合は0を返す。
static int32 ExcelColumnLetterToNumber(const FString& Letters)
{
	int32 Col = 0;
	for (const TCHAR Ch : Letters)
	{
		const TCHAR Upper = FChar::ToUpper(Ch);
		if (Upper < TEXT('A') || Upper > TEXT('Z')) { return 0; }
		Col = Col * 26 + (Upper - TEXT('A') + 1);
	}
	return Col;
}

// GoogleシートのURL全文（https://docs.google.com/spreadsheets/d/<ID>/...）からスプレッドシートIDを取り出す。
// URL形式でなければ、入力をそのままID扱いにする（ID直接貼り付けとの後方互換）。
static FString ExtractSpreadsheetIdFromUrl(const FString& In)
{
	const FString Trimmed = In.TrimStartAndEnd();
	const FString Marker = TEXT("/d/");

	const int32 MarkerIdx = Trimmed.Find(Marker, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	if (MarkerIdx == INDEX_NONE)
	{
		return Trimmed;
	}

	const int32 IdStart = MarkerIdx + Marker.Len();
	int32 IdEnd = Trimmed.Len();
	// IdStart以降で最初に現れる区切り文字を探す（FindCharは先頭から探すため、
	// "https://" 等に含まれる"/"に引っかかって正しく区切れないバグがあったので、
	// 探索開始位置を指定できるFindを使う）。
	for (const TCHAR* Stop : { TEXT("/"), TEXT("?"), TEXT("#") })
	{
		const int32 Found = Trimmed.Find(Stop, ESearchCase::IgnoreCase, ESearchDir::FromStart, IdStart);
		if (Found != INDEX_NONE && Found < IdEnd) { IdEnd = Found; }
	}
	const FString Id = Trimmed.Mid(IdStart, IdEnd - IdStart);
	return Id.IsEmpty() ? Trimmed : Id;
}

bool UWG_RadNodzToolkit::ReadListFromExcel(TArray<FListSheet>& OutSheets, FString& OutError)
{
	OutSheets.Reset();
	if (!Client || !bIsConnected) { OutError = ReaperLang::S(TEXT("未接続です"), TEXT("Not connected")); return false; }
	if (ListFilePath.IsEmpty()) { OutError = ReaperLang::S(TEXT("ファイルパスが未設定です"), TEXT("File path is empty")); return false; }

	// パスは「/」区切りに正規化する（バックスラッシュが通信(JSON)を壊し、サーバ側が落ちるのを防ぐ）
	FString Path = ListFilePath;
	Path.ReplaceInline(TEXT("\\"), TEXT("/"));

	// 事前にサーバ上にファイルが存在するか確認（存在しないパスでExcelを開くとサーバが落ちる対策）
	{
		FPathInfo Info = URigdocksSilver_File::AZ_GetPathInfo(Client, Path);
		if (Info.FilePath.IsEmpty() && Info.FileName.IsEmpty())
		{
			OutError = ReaperLang::S(TEXT("ファイルが見つかりません（パスを確認）"), TEXT("File not found (check path)"));
			return false;
		}
	}

	UXLDocument* Doc = URigdocksGold_Excel::AZ_Excel_OpenFile(Client, Path);
	if (Doc->IsNull()) { OutError = ReaperLang::S(TEXT("ファイルを開けません"), TEXT("Cannot open file")); return false; }

	TArray<FExcelSheetInfo> sheetList = URigdocksGold_Excel::AZ_Excel_GetWorksheetList(Client, Doc, TEXT(""), false, TArray<FString>());

	if (Client->GetError().ErrorCode != 0)
	{
		//PrintErrorMsg();
	}
	
	TArray<FExcelSheetInfo> TargetSheetList;

	for (auto sheetInfo : sheetList)
	{
		if (ListSheetName.IsEmpty() || sheetInfo.Name == ListSheetName)
		{
			TargetSheetList.Add(sheetInfo);
		}
	}

	if(TargetSheetList.Num() == 0)
	{
		OutError = ReaperLang::S(TEXT("シートの読み込みに失敗しました。"), TEXT("Fail loading the specified sheet"));

		if (Client->GetError().ErrorCode != 0)
		{


		}

		URigdocksGold_Excel::AZ_Excel_CloseFile(Client, Doc);

		return false;
	}

	const int32 StartRow = FMath::Max(1, ListStartRow);
	const int32 StartCol = FMath::Max(1, ListStartCol);
	const int32 NumCol   = FMath::Clamp(ListColCount, 1, 32);

	for (const FExcelSheetInfo& sheetInfo : TargetSheetList)
	{
		FListSheet S;
		S.SheetName = sheetInfo.Name;

		// 1シートにつき1回の通信で全行をまとめて取得（セルを1個ずつ取るより大幅に軽い）。
		// searchStr=".*"（正規表現・空も一致）で全行ヒット、range="" で使用範囲全体。
		const FString TargetCol = ExcelColumnLetter(StartCol);
		FExcelSheetInfo Rows = URigdocksGold_Excel::AZ_Excel_SearchRowsInWorksheet(
			Client, sheetInfo.Worksheet, TEXT(".*"), TargetCol, TEXT(""), true);

		// 行キー（行番号）を数値で昇順に並べる
		TArray<int32> RowNums;
		RowNums.Reserve(Rows.Cells.Num());
		for (const TPair<FString, FExcelRow>& Pair : Rows.Cells)
		{
			RowNums.Add(FCString::Atoi(*Pair.Key));
		}
		RowNums.Sort();

		for (int32 RowNum : RowNums)
		{
			if (RowNum < StartRow) { continue; }   // 開始行より前はスキップ
			const FExcelRow* Row = Rows.Cells.Find(FString::FromInt(RowNum));
			if (!Row) { continue; }

			FListRow R;
			bool bAny = false;
			for (int32 c = 0; c < NumCol; ++c)
			{
				const int32 ColIdx = StartCol + c;
				// 列キーは「列文字(A/B…)」。保険で「列番号」も探す。
				const FString* V = Row->Cells.Find(ExcelColumnLetter(ColIdx));
				if (!V) { V = Row->Cells.Find(FString::FromInt(ColIdx)); }
				const FString Cell = V ? *V : FString();
				R.Cells.Add(Cell);
				if (!Cell.IsEmpty()) { bAny = true; }
			}
			if (!bAny) { continue; }   // 空行はスキップ
			S.Rows.Add(R);
		}

		OutSheets.Add(S);
	}

	URigdocksGold_Excel::AZ_Excel_CloseFile(Client, Doc);
	return true;
}

bool UWG_RadNodzToolkit::ReadListFromCsv(TArray<FListSheet>& OutSheets, FString& OutError)
{
	OutSheets.Reset();
	if (!Client || !bIsConnected) { OutError = ReaperLang::S(TEXT("未接続です"), TEXT("Not connected")); return false; }
	if (ListFilePath.IsEmpty()) { OutError = ReaperLang::S(TEXT("ファイルパスが未設定です"), TEXT("File path is empty")); return false; }

	FString Path = ListFilePath;
	Path.ReplaceInline(TEXT("\\"), TEXT("/"));   // パスは「/」区切りに正規化（通信を壊さない）
	const FString Dir  = FPaths::GetPath(Path);
	const FString File = FPaths::GetCleanFilename(Path);
	const FString Text = URigdocksSilver_File::AZ_ReadFile(Client, Dir, File);
	if (Text.IsEmpty()) { OutError = ReaperLang::S(TEXT("ファイルが空か読めません"), TEXT("Empty or unreadable file")); return false; }

	TArray<FString> Lines;
	Text.ParseIntoArrayLines(Lines, false);

	const int32 StartRow = FMath::Max(1, ListStartRow);
	const int32 StartCol = FMath::Max(1, ListStartCol);
	const int32 NumCol   = FMath::Clamp(ListColCount, 1, 32);

	FListSheet S;
	S.SheetName = File;
	for (int32 i = StartRow - 1; i < Lines.Num(); ++i)
	{
		// 簡易CSV分割（引用符内のカンマは未対応：Phase1）
		TArray<FString> Cols;
		Lines[i].ParseIntoArray(Cols, TEXT(","), false);

		FListRow R;
		bool bAny = false;
		for (int32 c = 0; c < NumCol; ++c)
		{
			const int32 Idx = (StartCol - 1) + c;
			const FString Cell = Cols.IsValidIndex(Idx) ? Cols[Idx].TrimStartAndEnd() : FString();
			R.Cells.Add(Cell);
			if (!Cell.IsEmpty()) { bAny = true; }
		}
		if (!bAny) { continue; }   // 空行はスキップ
		S.Rows.Add(R);
	}
	OutSheets.Add(S);
	return true;
}

bool UWG_RadNodzToolkit::ReadListFromGSheet(TArray<FListSheet>& OutSheets, FString& OutError)
{
	OutSheets.Reset();
	if (!Client || !bIsConnected) { OutError = ReaperLang::S(TEXT("未接続です"), TEXT("Not connected")); return false; }
	if (GoogleSheetSpreadsheetId.IsEmpty()) { OutError = ReaperLang::S(TEXT("スプレッドシートIDが未設定です"), TEXT("Spreadsheet ID is empty")); return false; }

	UGSheetConnection* Conn = URigdocksGold_GSheet::AZ_GSheet_Connect_UserAccount(Client, GoogleSheetSpreadsheetId, GoogleSheetClientId);
	if (!Conn || Conn->IsNull())
	{
		OutError = ReaperLang::S(TEXT("スプレッドシートに接続できません"), TEXT("Cannot connect to spreadsheet"));
		return false;
	}

	TArray<FGSheetInfo> SheetList = URigdocksGold_GSheet::AZ_GSheet_GetWorksheetList(Client, Conn, TEXT(""), false, TArray<FString>());

	TArray<FGSheetInfo> TargetSheetList;
	for (const FGSheetInfo& SheetInfo : SheetList)
	{
		if (ListSheetName.IsEmpty() || SheetInfo.Name == ListSheetName)
		{
			TargetSheetList.Add(SheetInfo);
		}
	}

	if (TargetSheetList.Num() == 0)
	{
		OutError = ReaperLang::S(TEXT("シートの読み込みに失敗しました。"), TEXT("Fail loading the specified sheet"));
		URigdocksGold_GSheet::AZ_GSheet_Disconnect(Client, Conn);
		return false;
	}

	const int32 StartRow = FMath::Max(1, ListStartRow);
	const int32 StartCol = FMath::Max(1, ListStartCol);
	const int32 NumCol   = FMath::Clamp(ListColCount, 1, 32);

	for (const FGSheetInfo& SheetInfo : TargetSheetList)
	{
		FListSheet S;
		S.SheetName = SheetInfo.Name;

		// 1シートにつき1回の通信で全行をまとめて取得（Excel読み込みと同じ方式）。
		const FString TargetCol = ExcelColumnLetter(StartCol);
		FGSheetInfo Rows = URigdocksGold_GSheet::AZ_GSheet_SearchRowsInWorksheet(
			Client, Conn, SheetInfo.Id, TEXT(".*"), TargetCol, TEXT(""), true);

		// 行番号の昇順に並べる
		TArray<FCellRow> SortedRows = Rows.Cells;
		SortedRows.Sort([](const FCellRow& A, const FCellRow& B) { return A.RowNumber < B.RowNumber; });

		for (const FCellRow& Row : SortedRows)
		{
			if (Row.RowNumber < StartRow) { continue; }   // 開始行より前はスキップ

			FListRow R;
			bool bAny = false;
			for (int32 c = 0; c < NumCol; ++c)
			{
				const int32 ColIdx = StartCol + c;
				// 列キーは「列文字(A/B…)」。保険で「列番号」も探す。
				const FCellItem* V = Row.Cells.Find(ExcelColumnLetter(ColIdx));
				if (!V) { V = Row.Cells.Find(FString::FromInt(ColIdx)); }
				const FString Cell = V ? V->Value : FString();
				R.Cells.Add(Cell);
				if (!Cell.IsEmpty()) { bAny = true; }
			}
			if (!bAny) { continue; }   // 空行はスキップ
			S.Rows.Add(R);
		}

		OutSheets.Add(S);
	}

	URigdocksGold_GSheet::AZ_GSheet_Disconnect(Client, Conn);
	return true;
}

void UWG_RadNodzToolkit::HandleListExportClicked()
{
	if (!Client || !bIsConnected)
	{
		if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("未接続です"), TEXT("Not connected")))); }
		return;
	}

	if (ListFormat == 2)
	{
		// === Google Sheets に書き出す（各セル＋末尾列＝収録済み 1/0）===
		if (GoogleSheetSpreadsheetId.IsEmpty())
		{
			if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("スプレッドシートIDが未設定です"), TEXT("Spreadsheet ID is empty")))); }
			return;
		}
		UGSheetConnection* Conn = URigdocksGold_GSheet::AZ_GSheet_Connect_UserAccount(Client, GoogleSheetSpreadsheetId, GoogleSheetClientId);
		if (!Conn || Conn->IsNull())
		{
			if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("スプレッドシートに接続できません"), TEXT("Failed to connect to spreadsheet")))); }
			return;
		}
		for (const FListSheet& Sh : ListSheets)
		{
			FGSheetInfo WS = URigdocksGold_GSheet::AZ_GSheet_AddWorksheet(Client, Conn, Sh.SheetName);

			// このシートの全セル書き込みをRpcBatchで1往復にまとめる（従来はセル数ぶんの往復）。
			URadnodzRpcBatch* CellBatch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
			for (int32 r = 0; r < Sh.Rows.Num(); ++r)
			{
				const FListRow& R = Sh.Rows[r];
				int32 c = 0;
				for (; c < R.Cells.Num(); ++c)
				{
					CellBatch->Add(URigdocksGold_GSheet_Build::AZ_GSheet_SetCellValue_ByNumber_Build(
						Conn, WS.Id, r + 1, c + 1, UVarType::CreateFromString(R.Cells[c])));
				}
				// 末尾列に収録済み状態
				CellBatch->Add(URigdocksGold_GSheet_Build::AZ_GSheet_SetCellValue_ByNumber_Build(
					Conn, WS.Id, r + 1, c + 1, UVarType::CreateFromString(R.bChecked ? TEXT("1") : TEXT("0"))));
			}
			if (CellBatch->Num() > 0) { CellBatch->Execute(Client); }
		}
		URigdocksGold_GSheet::AZ_GSheet_Disconnect(Client, Conn);

		if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("書き出しました"), TEXT("Exported")))); }
		return;
	}

	if (ListExportPath.IsEmpty())
	{
		if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("書き出し先が未設定です"), TEXT("Export path is empty")))); }
		return;
	}

	// パスは「/」区切りに正規化（バックスラッシュで通信が壊れるのを防ぐ）
	FString ExportPath = ListExportPath;
	ExportPath.ReplaceInline(TEXT("\\"), TEXT("/"));

	if (ListFormat == 0)
	{
		// === Excel に書き出す（各セル＋末尾列＝収録済み 1/0）===
		UXLDocument* Doc = URigdocksGold_Excel::AZ_Excel_CreateDocument(Client, ExportPath, true);
		if (!Doc)
		{
			if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("Excel作成に失敗"), TEXT("Failed to create Excel")))); }
			return;
		}
		for (const FListSheet& Sh : ListSheets)
		{
			UXLWorksheet* WS = URigdocksGold_Excel::AZ_Excel_AddWorksheet(Client, Doc, Sh.SheetName);
			if (!WS) { continue; }

			// このシートの全セル書き込みをRpcBatchで1往復にまとめる（従来はセル数ぶんの往復）。
			URadnodzRpcBatch* CellBatch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
			for (int32 r = 0; r < Sh.Rows.Num(); ++r)
			{
				const FListRow& R = Sh.Rows[r];
				int32 c = 0;
				for (; c < R.Cells.Num(); ++c)
				{
					CellBatch->Add(URigdocksGold_Excel_Build::AZ_Excel_SetCellValue_ByNumber_Build(
						WS, r + 1, c + 1, UVarType::CreateFromString(R.Cells[c])));
				}
				// 末尾列に収録済み状態
				CellBatch->Add(URigdocksGold_Excel_Build::AZ_Excel_SetCellValue_ByNumber_Build(
					WS, r + 1, c + 1, UVarType::CreateFromString(R.bChecked ? TEXT("1") : TEXT("0"))));
			}
			if (CellBatch->Num() > 0) { CellBatch->Execute(Client); }
		}
		URigdocksGold_Excel::AZ_Excel_SaveDocument(Client, Doc, ExportPath);
		URigdocksGold_Excel::AZ_Excel_CloseFile(Client, Doc);
	}
	else
	{
		// === CSV に書き出す（末尾列＝収録済み 1/0）===
		FString Out;
		for (const FListSheet& Sh : ListSheets)
		{
			Out += FString::Printf(TEXT("# %s\r\n"), *Sh.SheetName);
			for (const FListRow& R : Sh.Rows)
			{
				FString Line;
				for (const FString& C : R.Cells) { Line += C + TEXT(","); }
				Line += R.bChecked ? TEXT("1") : TEXT("0");
				Out += Line + TEXT("\r\n");
			}
		}
		const FString Dir  = FPaths::GetPath(ExportPath);
		const FString File = FPaths::GetCleanFilename(ExportPath);
		URigdocksSilver_File::AZ_WriteFile(Client, Dir, File, Out, 0);
	}

	if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("書き出しました"), TEXT("Exported")))); }
}

void UWG_RadNodzToolkit::HandleListSlackClicked()
{
	if (!Client || !bIsConnected) { return; }
	if (SlackToken.IsEmpty() || SlackToUser.IsEmpty())
	{
		SetActiveTab(2);
		if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("Slack設定が必要"), TEXT("Set Slack first")))); }
		return;
	}

	// 収録結果の一覧テキストを作る
	FString Body = ReaperLang::S(TEXT("収録リスト結果"), TEXT("Recording list results")) + TEXT("\n");
	int32 Done = 0, Total = 0;
	for (const FListSheet& Sh : ListSheets)
	{
		Body += FString::Printf(TEXT("\n[%s]\n"), *Sh.SheetName);
		for (const FListRow& R : Sh.Rows)
		{
			++Total; if (R.bChecked) { ++Done; }
			const FString First = R.Cells.Num() > 0 ? R.Cells[0] : FString();
			Body += FString::Printf(TEXT("%s %s\n"), R.bChecked ? TEXT("✅") : TEXT("⬜"), *First);
		}
	}
	Body += FString::Printf(TEXT("\n%s: %d / %d"), *ReaperLang::S(TEXT("収録済み"), TEXT("Done")), Done, Total);

	FSlackMessage Res = URigdocksSilver_Slack::AZ_Slack_PostDirectMessage(Client, SlackToken, SlackToUser, Body, false);
	const bool bOk = Res.Error.IsEmpty() && !Res.TimeStamp.IsEmpty();
	if (ListStatusText)
	{
		ListStatusText->SetText(FText::FromString(bOk
			? ReaperLang::S(TEXT("Slackへ送信しました"), TEXT("Sent to Slack"))
			: ReaperLang::S(TEXT("Slack送信に失敗"), TEXT("Slack send failed"))));
	}
}

void UWG_RadNodzToolkit::EnsureTranscriptionModelLoaded()
{
	if (TrscContext) return; // 既にロード済み（セッション中は使い回す）
	if (!Client || !bIsConnected) return;
	// モデルパスはReaper側で解決されるため空文字で呼ぶ（端末側でパスを持たない）。
	TrscContext = URigdocksSilver_TRSC::AZ_TRSC_LoadModel(Client, TEXT(""));
}

void UWG_RadNodzToolkit::HandleRecordingCheckClicked()
{
	if (!Client || !bIsConnected) { return; }
	if (bScriptMatchJobActive) { return; }   // 実行中は二重起動しない
	if (SlackToken.IsEmpty() || SlackToUser.IsEmpty())
	{
		UWG_SlackRequiredDialog* Dlg = CreateWidget<UWG_SlackRequiredDialog>(this, UWG_SlackRequiredDialog::StaticClass());
		if (Dlg)
		{
			Dlg->Setup(this);
			Dlg->AddToViewport(130);
		}
		return;
	}
	if (ListSheets.Num() == 0)
	{
		if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("リストが未読み込みです"), TEXT("Load a list first")))); }
		return;
	}

	ShowScriptMatchColumnPicker();
}

void UWG_RadNodzToolkit::ShowScriptMatchColumnPicker()
{
	using namespace ReaperCtrlUI;

	if (!ScriptMatchColumnPickerBg || !ScriptMatchColumnPickerBox) return;
	if (!ListSheets.IsValidIndex(ListActiveSheet)) return;

	// 現在表示中シートの最大列数ぶん、Excel列文字（A/B/C…）のボタンを並べる
	int32 ColCount = 0;
	for (const FListRow& Row : ListSheets[ListActiveSheet].Rows)
	{
		ColCount = FMath::Max(ColCount, Row.Cells.Num());
	}

	ScriptMatchColumnPickerBox->ClearChildren();
	ScriptMatchColumnPickerButtons.Reset();

	UTextBlock* Hint = MakeText(WidgetTree, ReaperLang::S(TEXT("文字起こしでチェックする列を選択："), TEXT("Choose the column to check against transcription:")), 18, TextDim, false);
	{
		UHorizontalBoxSlot* S = ScriptMatchColumnPickerBox->AddChildToHorizontalBox(Hint);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 14.f, 0.f));
	}

	for (int32 i = 0; i < ColCount; ++i)
	{
		UWG_ColumnPickerButton* Btn = CreateWidget<UWG_ColumnPickerButton>(this, UWG_ColumnPickerButton::StaticClass());
		if (!Btn) continue;
		Btn->InitButton(i, ExcelColumnLetter(i + 1), this);
		UHorizontalBoxSlot* S = ScriptMatchColumnPickerBox->AddChildToHorizontalBox(Btn);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		ScriptMatchColumnPickerButtons.Add(Btn);
	}

	ScriptMatchColumnPickerBg->SetVisibility(ESlateVisibility::Visible);
}

void UWG_RadNodzToolkit::HideScriptMatchColumnPicker()
{
	if (ScriptMatchColumnPickerBox) { ScriptMatchColumnPickerBox->ClearChildren(); }
	ScriptMatchColumnPickerButtons.Reset();
	if (ScriptMatchColumnPickerBg) { ScriptMatchColumnPickerBg->SetVisibility(ESlateVisibility::Collapsed); }
}

void UWG_RadNodzToolkit::HandleScriptMatchColumnChosen(int32 ColIndex)
{
	HideScriptMatchColumnPicker();
	ScriptMatchHighlightCol = ColIndex;   // 選んだ列をリストの表示上で強調する
	RebuildListView();
	RunScriptMatch(ColIndex);
}

void UWG_RadNodzToolkit::RunScriptMatch(int32 CheckCol)
{
	if (bScriptMatchJobActive) { return; }   // 実行中は二重起動しない
	BeginScriptMatchJob(CheckCol);
}

void UWG_RadNodzToolkit::BeginScriptMatchJob(int32 CheckCol)
{
	if (!Client || !bIsConnected) { return; }

	// 対象メディアの一覧だけ先に集めておく（文字起こし自体は重いのでTickで1件ずつ進める）。
	ScriptMatchJobCheckCol = CheckCol;
	ScriptMatchPendingItems.Reset();
	ScriptMatchPendingNames.Reset();
	ScriptMatchPendingStarts.Reset();
	for (const FTrackDetail& T : TrackList)
	{
		TArray<UMediaItem*> Items;
		TArray<FString> Names;
		TArray<double> Starts;
		GetTrackMediaItems(T.Index, Items, Names, Starts);
		for (int32 i = 0; i < Items.Num(); ++i)
		{
			if (!Items[i]) continue;
			ScriptMatchPendingItems.Add(Items[i]);
			ScriptMatchPendingNames.Add(Names[i]);
			ScriptMatchPendingStarts.Add(Starts[i]);
		}
	}
	ScriptMatchJobCursor = 0;
	ScriptMatchTranscriptions.Reset();

	bScriptMatchJobActive = true;
	// 進行中インジケータは右上のStatusIcon（接続中と同じ縦回転）を流用する。
	bStatusIconSpin = true;
	StatusIconSpinPhase = 0.f;

	if (ListStatusText)
	{
		ListStatusText->SetText(FText::FromString(FString::Printf(TEXT("%s（0/%d）"),
			*ReaperLang::S(TEXT("文字起こし中…"), TEXT("Transcribing…")), ScriptMatchPendingItems.Num())));
	}

	if (ScriptMatchPendingItems.Num() == 0)
	{
		FinishScriptMatchJob();
	}
}

void UWG_RadNodzToolkit::TickScriptMatchJob(float DeltaSeconds)
{
	if (!bScriptMatchJobActive) return;
	if (!Client || !bIsConnected)
	{
		bScriptMatchJobActive = false;
		bStatusIconSpin = false;
		return;
	}

	if (!ScriptMatchPendingItems.IsValidIndex(ScriptMatchJobCursor))
	{
		FinishScriptMatchJob();
		return;
	}

	EnsureTranscriptionModelLoaded();
	if (!TrscContext)
	{
		bScriptMatchJobActive = false;
		bStatusIconSpin = false;
		if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("文字起こしモデルの読み込みに失敗"), TEXT("Failed to load the transcription model")))); }
		return;
	}

	// 1Tickにつきメディア1件だけ文字起こしする（Whisper呼び出し自体が重いため、これだけでUIが固まらなくなる）。
	if (UMediaItem* Item = ScriptMatchPendingItems[ScriptMatchJobCursor])
	{
		FScriptMatchTranscript Entry;
		Entry.Name = ScriptMatchPendingNames[ScriptMatchJobCursor];
		Entry.Start = ScriptMatchPendingStarts[ScriptMatchJobCursor];
		Entry.Length = URigdocksBronze_MediaItem::AZ_GetMediaItemLength(Client, Item);
		Entry.Text = URigdocksSilver_TRSC::AZ_TRSC_FullForMediaItem(Client, TrscContext, Item, TEXT("ja"));
		ScriptMatchTranscriptions.Add(Entry);
	}
	++ScriptMatchJobCursor;

	if (ListStatusText)
	{
		ListStatusText->SetText(FText::FromString(FString::Printf(TEXT("%s（%d/%d）"),
			*ReaperLang::S(TEXT("文字起こし中…"), TEXT("Transcribing…")), ScriptMatchJobCursor, ScriptMatchPendingItems.Num())));
	}
}

void UWG_RadNodzToolkit::FinishScriptMatchJob()
{
	bScriptMatchJobActive = false;
	bStatusIconSpin = false;
	if (StatusIcon) { StatusIcon->SetRenderScale(FVector2D(1.f, 1.f)); }

	const int32 CheckCol = ScriptMatchJobCheckCol;
	const TArray<FScriptMatchTranscript>& Transcriptions = ScriptMatchTranscriptions;

	// 秒 → "H:MM:SS" / "M:SS"（PostLoudnessToSlackと同じ書式）
	auto FmtDur = [](double Sec) -> FString
	{
		if (Sec < 0.0) Sec = 0.0;
		const int32 Total = FMath::RoundToInt(Sec);
		const int32 H = Total / 3600;
		const int32 M = (Total % 3600) / 60;
		const int32 S = Total % 60;
		return (H > 0) ? FString::Printf(TEXT("%d:%02d:%02d"), H, M, S)
		               : FString::Printf(TEXT("%d:%02d"), M, S);
	};

	// しきい値（0-100）：これ未満は「未収録の可能性」、これ以上は「収録済みとして確定」。中間は「内容違いの可能性」。
	// 60%を超えない一致は「一致した」対象として扱わない（収録済み確定・検索からの除外もこの基準で行う）。
	constexpr int32 LowThreshold = 30;
	constexpr int32 HighThreshold = 60;

	// 対象範囲（シート/全シート）で対象シートを絞る
	TArray<FListSheet*> TargetSheets;
	if (ScriptMatchScope == 1)
	{
		for (FListSheet& Sh : ListSheets) { TargetSheets.Add(&Sh); }
	}
	else if (ListSheets.IsValidIndex(ListActiveSheet))
	{
		TargetSheets.Add(&ListSheets[ListActiveSheet]);
	}

	// リストの対象行 × 文字起こし全件ぶんの類似度計算を、1回のRPCバッチにまとめて1往復にする
	// （PostLoudnessToSlackの長さ取得バッチ化と同じ考え方）。
	struct FRowMatchJob { FListSheet* Sheet; int32 RowIndex; TArray<URigdocksRpcCall*> Calls; };
	TArray<FRowMatchJob> Jobs;
	URadnodzRpcBatch* SimBatch = URadnodzRpcBatch::CreateRadnodzRpcBatch();

	for (FListSheet* Sh : TargetSheets)
	{
		for (int32 RowIdx = 0; RowIdx < Sh->Rows.Num(); ++RowIdx)
		{
			// チェック状態フィルタ（見ない/チェック済みのみ/未チェックのみ）で対象外の行はスキップ
			const bool bChecked = Sh->Rows[RowIdx].bChecked;
			if (ScriptMatchCheckFilter == 1 && !bChecked) continue;
			if (ScriptMatchCheckFilter == 2 && bChecked) continue;

			const FString Expected = Sh->Rows[RowIdx].Cells.IsValidIndex(CheckCol) ? Sh->Rows[RowIdx].Cells[CheckCol] : FString();

			FRowMatchJob Job;
			Job.Sheet = Sh;
			Job.RowIndex = RowIdx;
			for (const FScriptMatchTranscript& M : Transcriptions)
			{
				URigdocksRpcCall* Call = URigdocksSilver_String_Build::AZ_CalcStringSimilarity_Build(Expected, M.Text);
				SimBatch->Add(Call);
				Job.Calls.Add(Call);
			}
			Jobs.Add(Job);
		}
	}

	if (SimBatch->Num() > 0)
	{
		SimBatch->Execute(Client);
	}

	// 結果を各行へ書き戻しつつ、Slack報告用の本文を組み立てる。
	// 各メディアが対象行の中で達成した最高スコアも記録し、誰にもマッチしなかったメディア＝
	// 「台本にない収録」として別セクションで報告する。
	TArray<int32> BestScorePerMedia;
	BestScorePerMedia.Init(-1, Transcriptions.Num());

	// 1メディア＝1台本行の前提のため、高一致(HighThreshold以上)で確定したメディアは
	// 以降の行の候補から外す（同じ音声が複数の台本行に重複マッチしないように）。
	TArray<bool> MediaClaimed;
	MediaClaimed.Init(false, Transcriptions.Num());

	FString DetailBody;
	int32 HighCount = 0, WrongContentCount = 0, NotRecordedCount = 0;

	for (FRowMatchJob& Job : Jobs)
	{
		FListRow& Row = Job.Sheet->Rows[Job.RowIndex];
		const FString Expected = Row.Cells.IsValidIndex(CheckCol) ? Row.Cells[CheckCol] : FString();

		int32 BestScore = -1;
		FString BestText;
		int32 BestMediaIdx = INDEX_NONE;
		for (int32 k = 0; k < Job.Calls.Num(); ++k)
		{
			if (MediaClaimed[k]) continue;   // 既に他行で高一致確定した音声は候補から除外
			const int32 Score = URigdocksSilver_String_Parse::AZ_CalcStringSimilarity_Parse(Job.Calls[k]);
			if (Score > BestScore) { BestScore = Score; BestText = Transcriptions[k].Text; BestMediaIdx = k; }
			BestScorePerMedia[k] = FMath::Max(BestScorePerMedia[k], Score);
		}

		Row.MatchScore = BestScore;
		Row.TranscribedMatch = BestText;
		if (BestScore >= HighThreshold)
		{
			Row.bChecked = true;   // 高スコア一致は自動で収録済みチェックON
			++HighCount;
			if (BestMediaIdx != INDEX_NONE) { MediaClaimed[BestMediaIdx] = true; }
		}
		else
		{
			// 「チェック状態を見ない」設定のときだけ、しきい値未満ならチェックを外し直す
			// （メディア削除等で一致しなくなった行を正しく反映する）。
			// チェック済みのみ/未チェックのみ で絞り込んでいる場合は手動作業を壊さないよう変更しない。
			if (ScriptMatchCheckFilter == 0) { Row.bChecked = false; }

			if (BestScore >= LowThreshold) { ++WrongContentCount; }
			else { ++NotRecordedCount; }
		}

		const FString RowName = Row.Cells.Num() > 0 ? Row.Cells[0] : FString();
		DetailBody += FString::Printf(TEXT("%s ・%s\n    %s: %s\n    %s: %s（%s%d%%）\n"),
			Row.bChecked ? TEXT("✅") : TEXT("⬜"), *RowName,
			*ReaperLang::S(TEXT("台本"), TEXT("Script")), *Expected,
			*ReaperLang::S(TEXT("認識"), TEXT("Heard")), *BestText,
			*ReaperLang::S(TEXT("一致度"), TEXT("Match")), FMath::Max(BestScore, 0));
	}

	// 台本にない収録（どの行に対しても60%を超える一致にならなかった＝確定した一致がないメディア）
	FString UnmatchedBody;
	int32 UnmatchedCount = 0;
	for (int32 k = 0; k < Transcriptions.Num(); ++k)
	{
		if (BestScorePerMedia[k] >= HighThreshold) continue;
		++UnmatchedCount;
		UnmatchedBody += FString::Printf(TEXT("・%s | %s | %s\n"), *FmtDur(Transcriptions[k].Start), *Transcriptions[k].Name, *FmtDur(Transcriptions[k].Length));
	}

	// Slackへは問題の有無に関わらず毎回送信する
	FString Body = FString::Printf(TEXT("%s\n%s: %d / %s: %d / %s: %d / %s: %d\n\n"),
		*ReaperLang::S(TEXT("台本一致チェック結果"), TEXT("Script match results")),
		*ReaperLang::S(TEXT("対象行数"), TEXT("Rows checked")), Jobs.Num(),
		*ReaperLang::S(TEXT("収録済み"), TEXT("Recorded")), HighCount,
		*ReaperLang::S(TEXT("内容相違疑い"), TEXT("Wrong content")), WrongContentCount,
		*ReaperLang::S(TEXT("未収録疑い"), TEXT("Not recorded")), NotRecordedCount);
	Body += TEXT("```\n") + DetailBody + TEXT("```");
	Body += FString::Printf(TEXT("\n\n%s（%s: %d）\n"), *ReaperLang::S(TEXT("台本にない収録"), TEXT("Recordings not in the script")), *ReaperLang::S(TEXT("総数"), TEXT("Total")), UnmatchedCount);
	if (UnmatchedCount > 0)
	{
		Body += TEXT("```\n") + UnmatchedBody + TEXT("```");
	}

	URigdocksSilver_Slack::AZ_Slack_PostDirectMessage(Client, SlackToken, SlackToUser, Body, false);

	RebuildListView();

	if (ListStatusText)
	{
		ListStatusText->SetText(FText::FromString(FString::Printf(
			TEXT("%s（%s %d / %s %d）"),
			*ReaperLang::S(TEXT("台本一致完了"), TEXT("Script match done")),
			*ReaperLang::S(TEXT("未収録疑い"), TEXT("not recorded")), NotRecordedCount,
			*ReaperLang::S(TEXT("内容相違疑い"), TEXT("wrong content")), WrongContentCount)));
	}

	ScriptMatchPendingItems.Reset();
	ScriptMatchPendingNames.Reset();
	ScriptMatchPendingStarts.Reset();
	ScriptMatchTranscriptions.Reset();
}

void UWG_RadNodzToolkit::HandleListFormatClicked()
{
	// Excel(0) → CSV(1) → Google Sheets(2) → Excel(0) … と循環
	ListFormat = (ListFormat + 1) % 3;
	UpdateListFormatButton();
	UpdateListFormatRowsVisibility();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateListFormatButton()
{
	if (ListFormatButtonText)
	{
		const FString Label = (ListFormat == 1) ? TEXT("CSV") : (ListFormat == 2) ? TEXT("Google Sheets") : TEXT("Excel");
		ListFormatButtonText->SetText(FText::FromString(Label));
	}
}

void UWG_RadNodzToolkit::UpdateListFormatRowsVisibility()
{
	// Google Sheetsはスプレッドシート単体で完結する（ファイルパス/書き出し先という概念が無く、
	// 書き出しも読み込み元と同じスプレッドシートに対して行うため）。
	const bool bIsGSheet = (ListFormat == 2);
	const ESlateVisibility FilePathVis = bIsGSheet ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
	const ESlateVisibility GSheetVis   = bIsGSheet ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;

	if (ListFileRow)           ListFileRow->SetVisibility(FilePathVis);
	if (ListExportRow)         ListExportRow->SetVisibility(FilePathVis);
	if (ListGSheetIdRow)       ListGSheetIdRow->SetVisibility(GSheetVis);
	if (ListGSheetClientIdRow) ListGSheetClientIdRow->SetVisibility(GSheetVis);
}

void UWG_RadNodzToolkit::HandleListCheckboxClicked()
{
	bListUseCheckbox = !bListUseCheckbox;
	UpdateListCheckboxButton();
	RebuildListView();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateListCheckboxButton()
{
	if (ListCheckboxButtonText)
	{
		ListCheckboxButtonText->SetText(ReaperLang::T(bListUseCheckbox ? TEXT("あり") : TEXT("なし"),
			bListUseCheckbox ? TEXT("On") : TEXT("Off")));
	}
}

void UWG_RadNodzToolkit::HandleScriptMatchScopeClicked()
{
	// シート（現在表示中のみ） → 全シート → シート … と循環
	ScriptMatchScope = (ScriptMatchScope + 1) % 2;
	UpdateScriptMatchScopeButton();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateScriptMatchScopeButton()
{
	if (ScriptMatchScopeButtonText)
	{
		ScriptMatchScopeButtonText->SetText(FText::FromString(ScriptMatchScope == 1
			? ReaperLang::S(TEXT("全シート"), TEXT("All sheets"))
			: ReaperLang::S(TEXT("シート"), TEXT("Sheet"))));
	}
}

void UWG_RadNodzToolkit::HandleScriptMatchCheckFilterClicked()
{
	// 見ない（絞り込みなし） → チェック済みのみ → 未チェックのみ → 見ない … と循環
	ScriptMatchCheckFilter = (ScriptMatchCheckFilter + 1) % 3;
	UpdateScriptMatchCheckFilterButton();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateScriptMatchCheckFilterButton()
{
	if (ScriptMatchCheckFilterButtonText)
	{
		const FString Label = (ScriptMatchCheckFilter == 1)
			? ReaperLang::S(TEXT("チェック済みのみ"), TEXT("Checked only"))
			: (ScriptMatchCheckFilter == 2)
				? ReaperLang::S(TEXT("未チェックのみ"), TEXT("Unchecked only"))
				: ReaperLang::S(TEXT("見ない"), TEXT("Ignore"));
		ScriptMatchCheckFilterButtonText->SetText(FText::FromString(Label));
	}
}

void UWG_RadNodzToolkit::HandleListFilePathCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	ListFilePath = Text.ToString().TrimStartAndEnd();
	SaveSettings();
}
void UWG_RadNodzToolkit::HandleListSheetCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	ListSheetName = Text.ToString().TrimStartAndEnd();
	SaveSettings();
}
void UWG_RadNodzToolkit::HandleListStartRowCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	const FString S = Text.ToString().TrimStartAndEnd();
	if (S.IsNumeric()) { ListStartRow = FMath::Max(1, FCString::Atoi(*S)); }
	SaveSettings();
}
void UWG_RadNodzToolkit::HandleListStartColCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	const FString S = Text.ToString().TrimStartAndEnd();
	const int32 Col = ExcelColumnLetterToNumber(S);
	if (Col > 0) { ListStartCol = Col; }
	// 入力を正規化して表示（小文字→大文字、無効値→元の値の列文字に戻す）
	if (ListStartColInput) { ListStartColInput->SetText(FText::FromString(ExcelColumnLetter(ListStartCol))); }
	SaveSettings();
}
void UWG_RadNodzToolkit::HandleListColCountCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	const FString S = Text.ToString().TrimStartAndEnd();
	if (S.IsNumeric()) { ListColCount = FMath::Clamp(FCString::Atoi(*S), 1, 32); }
	SaveSettings();
}
void UWG_RadNodzToolkit::HandleListExportPathCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	ListExportPath = Text.ToString().TrimStartAndEnd();
	SaveSettings();
}

void UWG_RadNodzToolkit::HandleListGSheetIdCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// URL全文で貼り付けられてもIDだけを抜き出して保持する（ID直接貼り付けもそのまま通る）。
	GoogleSheetSpreadsheetId = ExtractSpreadsheetIdFromUrl(Text.ToString());
	SaveSettings();
}

void UWG_RadNodzToolkit::HandleListGSheetClientIdCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	GoogleSheetClientId = Text.ToString().TrimStartAndEnd();
	SaveSettings();
}

void UWG_RadNodzToolkit::NativeConstruct()
{
	Super::NativeConstruct();

	// 前回の接続設定(IP/Port)を読み込んで入力欄へ反映
	LoadSettings();

	if (ConnectButton && !ConnectButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleConnectClicked))
	{
		ConnectButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleConnectClicked);
	}
	if (IPAddressInput && !IPAddressInput->OnTextChanged.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleReaperIpChanged))
	{
		IPAddressInput->OnTextChanged.AddDynamic(this, &UWG_RadNodzToolkit::HandleReaperIpChanged);
	}
	// IP/ポートの確定で、選択中 serverId に接続先を紐づけ保存する。
	if (IPAddressInput && !IPAddressInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleReaperIpCommitted))
	{
		IPAddressInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleReaperIpCommitted);
	}
	if (PortInput && !PortInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandlePortCommitted))
	{
		PortInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandlePortCommitted);
	}
	UpdateHeaderIpText();
	if (AddTrackButton && !AddTrackButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleAddTrackClicked))
	{
		AddTrackButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleAddTrackClicked);
	}
	if (MicSettingsButton && !MicSettingsButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMicSettingsClicked))
	{
		MicSettingsButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMicSettingsClicked);
	}
	if (MonitorButton && !MonitorButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMonitorClicked))
	{
		MonitorButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMonitorClicked);
	}
	if (MicSendButton && !MicSendButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMicSendClicked))
	{
		MicSendButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMicSendClicked);
	}
	if (MeterRecvButton && !MeterRecvButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMeterReceiveClicked))
	{
		MeterRecvButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMeterReceiveClicked);
	}
	if (MeterRecvSettingButton && !MeterRecvSettingButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMeterReceiveClicked))
	{
		MeterRecvSettingButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMeterReceiveClicked);
	}
	if (ThemeButton && !ThemeButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleThemeClicked))
	{
		ThemeButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleThemeClicked);
	}
	if (OrientationButton && !OrientationButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleOrientationClicked))
	{
		OrientationButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleOrientationClicked);
	}
	if (UpdateButton && !UpdateButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleUpdateClicked))
	{
		UpdateButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleUpdateClicked);
	}
	if (PlayButton && !PlayButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandlePlayClicked))
	{
		PlayButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandlePlayClicked);
	}
	if (RecordButton && !RecordButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleRecordClicked))
	{
		RecordButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleRecordClicked);
	}
	if (ClearArmedButton && !ClearArmedButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleClearArmedClicked))
	{
		ClearArmedButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleClearArmedClicked);
	}
	if (DeleteTrackButton && !DeleteTrackButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleDeleteTrackClicked))
	{
		DeleteTrackButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleDeleteTrackClicked);
	}
	if (DeleteSelectAllButton && !DeleteSelectAllButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleDeleteSelectAll))
	{
		DeleteSelectAllButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleDeleteSelectAll);
	}
	if (DeleteDeselectAllButton && !DeleteDeselectAllButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleDeleteDeselectAll))
	{
		DeleteDeselectAllButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleDeleteDeselectAll);
	}
	if (SlackShareButton && !SlackShareButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleSlackShareClicked))
	{
		SlackShareButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleSlackShareClicked);
	}
	if (LockButton && !LockButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleLockClicked))
	{
		LockButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleLockClicked);
	}
	if (ScanButton && !ScanButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleScanClicked))
	{
		ScanButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleScanClicked);
	}
	if (SlackTokenInput && !SlackTokenInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleSlackTokenCommitted))
	{
		SlackTokenInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleSlackTokenCommitted);
	}
	if (SlackToUserInput && !SlackToUserInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleSlackToUserCommitted))
	{
		SlackToUserInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleSlackToUserCommitted);
	}
	if (NormalizeButton && !NormalizeButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleNormalizeClicked))
	{
		NormalizeButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleNormalizeClicked);
	}
	if (TargetLufsInput && !TargetLufsInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleTargetLufsCommitted))
	{
		TargetLufsInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleTargetLufsCommitted);
	}
	if (MeterIntervalInput && !MeterIntervalInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMeterIntervalCommitted))
	{
		MeterIntervalInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleMeterIntervalCommitted);
	}
	if (MeterModeButton && !MeterModeButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMeterModeClicked))
	{
		MeterModeButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMeterModeClicked);
	}
	if (LimitPeakButton && !LimitPeakButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleLimitPeakClicked))
	{
		LimitPeakButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleLimitPeakClicked);
	}
	if (TrackRefreshModeButton && !TrackRefreshModeButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleTrackRefreshModeClicked))
	{
		TrackRefreshModeButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleTrackRefreshModeClicked);
	}
	if (TrackRefreshIntervalInput && !TrackRefreshIntervalInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleTrackRefreshIntervalCommitted))
	{
		TrackRefreshIntervalInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleTrackRefreshIntervalCommitted);
	}
	if (ShowDeviceInfoButton && !ShowDeviceInfoButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleShowDeviceInfoClicked))
	{
		ShowDeviceInfoButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleShowDeviceInfoClicked);
	}
	if (CommTimeMeasureButton && !CommTimeMeasureButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMeasureCommTimeClicked))
	{
		CommTimeMeasureButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMeasureCommTimeClicked);
	}
	if (MeterDotScopeButton && !MeterDotScopeButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMeterDotScopeClicked))
	{
		MeterDotScopeButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMeterDotScopeClicked);
	}
	if (HomeTabButton && !HomeTabButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleHomeTabClicked))
	{
		HomeTabButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleHomeTabClicked);
	}
	if (SettingsTabButton && !SettingsTabButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleSettingsTabClicked))
	{
		SettingsTabButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleSettingsTabClicked);
	}
	if (ListTabButton && !ListTabButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleListTabClicked))
	{
		ListTabButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleListTabClicked);
	}
	// リスト機能のボタン/入力
	if (ListFormatButton && !ListFormatButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleListFormatClicked))
		ListFormatButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleListFormatClicked);
	if (ListCheckboxButton && !ListCheckboxButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleListCheckboxClicked))
		ListCheckboxButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleListCheckboxClicked);
	if (ScriptMatchScopeButton && !ScriptMatchScopeButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleScriptMatchScopeClicked))
		ScriptMatchScopeButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleScriptMatchScopeClicked);
	if (ScriptMatchCheckFilterButton && !ScriptMatchCheckFilterButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleScriptMatchCheckFilterClicked))
		ScriptMatchCheckFilterButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleScriptMatchCheckFilterClicked);
	if (ListFilePathInput && !ListFilePathInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleListFilePathCommitted))
		ListFilePathInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleListFilePathCommitted);
	if (ListSheetInput && !ListSheetInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleListSheetCommitted))
		ListSheetInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleListSheetCommitted);
	if (ListStartRowInput && !ListStartRowInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleListStartRowCommitted))
		ListStartRowInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleListStartRowCommitted);
	if (ListStartColInput && !ListStartColInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleListStartColCommitted))
		ListStartColInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleListStartColCommitted);
	if (ListColCountInput && !ListColCountInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleListColCountCommitted))
		ListColCountInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleListColCountCommitted);
	if (ListExportPathInput && !ListExportPathInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleListExportPathCommitted))
		ListExportPathInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleListExportPathCommitted);
	if (ListGSheetIdInput && !ListGSheetIdInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleListGSheetIdCommitted))
		ListGSheetIdInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleListGSheetIdCommitted);
	if (ListGSheetClientIdInput && !ListGSheetClientIdInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleListGSheetClientIdCommitted))
		ListGSheetClientIdInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleListGSheetClientIdCommitted);
	// メンバー通話の入力
	if (MemberUserNameInput && !MemberUserNameInput->OnTextCommitted.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMemberUserNameCommitted))
		MemberUserNameInput->OnTextCommitted.AddDynamic(this, &UWG_RadNodzToolkit::HandleMemberUserNameCommitted);
	if (MicInputButton && !MicInputButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMicInputModeClicked))
	{
		MicInputButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMicInputModeClicked);
	}
	if (MonitorOutputButton && !MonitorOutputButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMonitorOutputModeClicked))
	{
		MonitorOutputButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMonitorOutputModeClicked);
	}
	// グループトーク：受信ボタンは押下/離しのみ束縛し、タップ/長押しは離し時刻で判定する
	// （OnClickedは使わない）。送信ボタンはタップ（ラッチ）と押下/離し（PTT）の両方を束縛する。
	if (GroupTalkRecvButton[0])
	{
		if (!GroupTalkRecvButton[0]->OnPressed.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkRecv0Pressed))
			GroupTalkRecvButton[0]->OnPressed.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkRecv0Pressed);
		if (!GroupTalkRecvButton[0]->OnReleased.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkRecv0Released))
			GroupTalkRecvButton[0]->OnReleased.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkRecv0Released);
	}
	if (GroupTalkRecvButton[1])
	{
		if (!GroupTalkRecvButton[1]->OnPressed.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkRecv1Pressed))
			GroupTalkRecvButton[1]->OnPressed.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkRecv1Pressed);
		if (!GroupTalkRecvButton[1]->OnReleased.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkRecv1Released))
			GroupTalkRecvButton[1]->OnReleased.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkRecv1Released);
	}
	if (GroupTalkSendButton[0])
	{
		if (!GroupTalkSendButton[0]->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkSend0Clicked))
			GroupTalkSendButton[0]->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkSend0Clicked);
		if (!GroupTalkSendButton[0]->OnPressed.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkSend0Pressed))
			GroupTalkSendButton[0]->OnPressed.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkSend0Pressed);
		if (!GroupTalkSendButton[0]->OnReleased.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkSend0Released))
			GroupTalkSendButton[0]->OnReleased.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkSend0Released);
	}
	if (GroupTalkSendButton[1])
	{
		if (!GroupTalkSendButton[1]->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkSend1Clicked))
			GroupTalkSendButton[1]->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkSend1Clicked);
		if (!GroupTalkSendButton[1]->OnPressed.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkSend1Pressed))
			GroupTalkSendButton[1]->OnPressed.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkSend1Pressed);
		if (!GroupTalkSendButton[1]->OnReleased.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkSend1Released))
			GroupTalkSendButton[1]->OnReleased.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkSend1Released);
	}
	if (MonitorBufferButton && !MonitorBufferButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMonitorBufferClicked))
	{
		MonitorBufferButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMonitorBufferClicked);
	}
	if (SpatialItdButton && !SpatialItdButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleSpatialItdClicked))
	{
		SpatialItdButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleSpatialItdClicked);
	}
	if (SpatialShadowButton && !SpatialShadowButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleSpatialShadowClicked))
	{
		SpatialShadowButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleSpatialShadowClicked);
	}
	if (SpatialOrderButton && !SpatialOrderButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleSpatialOrderClicked))
	{
		SpatialOrderButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleSpatialOrderClicked);
	}
	if (MapEditChButton && !MapEditChButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMapEditChClicked))
	{
		MapEditChButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMapEditChClicked);
	}
	if (MapAngleMinusButton && !MapAngleMinusButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMapAngleMinusClicked))
	{
		MapAngleMinusButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMapAngleMinusClicked);
	}
	if (MapAnglePlusButton && !MapAnglePlusButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMapAnglePlusClicked))
	{
		MapAnglePlusButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMapAnglePlusClicked);
	}
	if (MapDistMinusButton && !MapDistMinusButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMapDistMinusClicked))
	{
		MapDistMinusButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMapDistMinusClicked);
	}
	if (MapDistPlusButton && !MapDistPlusButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMapDistPlusClicked))
	{
		MapDistPlusButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMapDistPlusClicked);
	}

	// 初期表示はホームタブ（タブの見た目も同期）
	SetActiveTab(0);

	// メーター受信は既定OFF。ホーム/設定の両ボタンの表示を初期化する。
	UpdateMeterReceiveButtons();

	// ピンチ（マグニファイ）ジェスチャを受け取れるよう、ウィジェット自身をヒットテスト対象にする。
	// （子のボタン等は引き続き優先して入力を処理する）
	SetVisibility(ESlateVisibility::Visible);
	ApplyZoom();

	UpdateStatus();
}

// ====================================================================
//  ボタンハンドラ
// ====================================================================
void UWG_RadNodzToolkit::HandleConnectClicked()
{
	// 接続済み or 接続試行中なら切断（=試行キャンセル）、未接続なら接続（1つのボタンでトグル）
	if (bIsConnected || bIsConnecting)
	{
		Disconnect();
		return;
	}

	if (IPAddressInput)
	{
		IPAddress = IPAddressInput->GetText().ToString();
	}
	if (PortInput)
	{
		const FString PortStr = PortInput->GetText().ToString();
		if (PortStr.IsNumeric())
		{
			Port = FCString::Atoi(*PortStr);
		}
	}
	StoreConnToSelectedServer();   // 接続先を選択中 serverId に紐づけて保存
	SaveSettings();   // 接続設定を保存（次回起動時に復元）
	UpdateHeaderIpText();
	Connect();
}

void UWG_RadNodzToolkit::HandleReaperIpChanged(const FText& Text)
{
	UpdateHeaderIpText();
}

void UWG_RadNodzToolkit::UpdateHeaderIpText()
{
	FString Local = URadnodzUtility::AZ_GetLocalIP();
	if (Local.IsEmpty()) { Local = ReaperLang::S(TEXT("不明"), TEXT("Unknown")); }

	// 設定画面の「この端末のIP」もここで一緒に更新し、ヘッダーの自分IPと常に一致させる
	// （取得タイミングがずれていると、Wi-Fi再接続などでIPが変わった際に表示が食い違うため）。
	if (LocalIPText) { LocalIPText->SetText(FText::FromString(Local)); }

	if (!HeaderIpText) return;

	if (!bShowDeviceNameIP)
	{
		HeaderIpText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	HeaderIpText->SetVisibility(ESlateVisibility::Visible);

	FString Reaper = IPAddressInput ? IPAddressInput->GetText().ToString().TrimStartAndEnd() : FString();
	if (Reaper.IsEmpty()) { Reaper = IPAddress; }

	// 自分側：端末名（OSホスト名）＋ローカルIP
	const FString SelfName = FString(FPlatformProcess::ComputerName());
	FString SelfPart = ReaperLang::S(TEXT("自分"), TEXT("Me")) + TEXT(" ");
	if (!SelfName.IsEmpty()) { SelfPart += SelfName + TEXT(" "); }
	SelfPart += Local;

	// 相手側：Reaper端末名（取得済みなら）＋Reaper IP
	FString ReaperPart = TEXT("Reaper ");
	if (!RemoteDeviceName.IsEmpty()) { ReaperPart += RemoteDeviceName + TEXT(" "); }
	ReaperPart += Reaper;

	const FString S = SelfPart + TEXT("   /   ") + ReaperPart;
	HeaderIpText->SetText(FText::FromString(S));
}

// SILVER連携ライブラリ（ビルド済みの外部lib、ソース非公開）が返すビット深度が
// 実機では 32bit のはずが 33 のように±1ずれて返ってくることがあるため、
// 一般的なビット深度（8/16/24/32/64）に丸めて表示する。
static int32 NormalizeBitDepth(int32 RawBitDepth)
{
	static const int32 StandardDepths[] = { 8, 16, 24, 32, 64 };
	for (int32 Depth : StandardDepths)
	{
		if (FMath::Abs(RawBitDepth - Depth) <= 1)
		{
			return Depth;
		}
	}
	return RawBitDepth;
}

void UWG_RadNodzToolkit::UpdateDeviceInfo()
{
	using namespace ReaperCtrlUI;

	// 接続していないときは取得できないので「-」表示にしてキャッシュもクリアする。
	if (!Client || !bIsConnected)
	{
		RecSampleRate = 0.0;
		RecBitDepth   = 0;
		InputChCount  = 0;
		OutputChCount = 0;
		RemoteDeviceName.Empty();
		RefreshDeviceInfoText();
		return;
	}

	// 接続先デバイス情報を同期取得（FetchTracks 同様、ゲームスレッドの同期呼び出し）。
	// 5つの独立した取得をRpcBatchで1往復にまとめる（従来は最大5往復）。
	UReaProject* Proj = GetProject();

	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	URigdocksRpcCall* CallSampleRate = nullptr;
	URigdocksRpcCall* CallBitDepth   = nullptr;
	if (Proj)
	{
		CallSampleRate = URigdocksSilver_Project_Build::AZ_GetRecordingSampleRate_Build(Proj);
		CallBitDepth   = URigdocksSilver_Project_Build::AZ_GetRecordingBitDepth_Build(Proj);
		Batch->Add(CallSampleRate);
		Batch->Add(CallBitDepth);
	}
	URigdocksRpcCall* CallInputCh  = URigdocksSilver_Device_Build::AZ_CountInputChannel_Build();
	URigdocksRpcCall* CallOutputCh = URigdocksSilver_Device_Build::AZ_CountOutputChannel_Build();
	URigdocksRpcCall* CallDevName  = URigdocksSilver_Device_Build::AZ_GetDeviceName_Build();
	Batch->Add(CallInputCh);
	Batch->Add(CallOutputCh);
	Batch->Add(CallDevName);

	Batch->Execute(Client);

	if (Proj)
	{
		RecSampleRate = URigdocksSilver_Project_Parse::AZ_GetRecordingSampleRate_Parse(CallSampleRate);
		RecBitDepth   = NormalizeBitDepth(URigdocksSilver_Project_Parse::AZ_GetRecordingBitDepth_Parse(CallBitDepth));
	}
	InputChCount  = URigdocksSilver_Device_Parse::AZ_CountInputChannel_Parse(CallInputCh);
	OutputChCount = URigdocksSilver_Device_Parse::AZ_CountOutputChannel_Parse(CallOutputCh);
	RemoteDeviceName = URigdocksSilver_Device_Parse::AZ_GetDeviceName_Parse(CallDevName);

	RefreshDeviceInfoText();

	// 相手端末名がヘッダーにも出るよう反映する。
	UpdateHeaderIpText();
}

void UWG_RadNodzToolkit::RefreshDeviceInfoText()
{
	if (!DeviceInfoText) return;

	if (!Client || !bIsConnected)
	{
		const FString S = ReaperLang::S(
			TEXT("- Hz / - bit　・　入力 - / 出力 -"),
			TEXT("- Hz / - bit   in - / out -"));
		DeviceInfoText->SetText(FText::FromString(S));
		return;
	}

	// 例: 「48000 Hz / 24 bit　・　入力 2ch / 出力 2ch」
	const FString HzStr  = FString::Printf(TEXT("%d Hz"), FMath::RoundToInt(RecSampleRate));
	const FString BitStr = FString::Printf(TEXT("%d bit"), RecBitDepth);
	const FString Fmt = ReaperLang::S(
		TEXT("{0} / {1}　・　入力 {2}ch / 出力 {3}ch"),
		TEXT("{0} / {1}   in {2}ch / out {3}ch"));
	const FString S = FString::Format(*Fmt, { FStringFormatArg(HzStr), FStringFormatArg(BitStr), FStringFormatArg(InputChCount), FStringFormatArg(OutputChCount) });
	DeviceInfoText->SetText(FText::FromString(S));
}

void UWG_RadNodzToolkit::HandleAddTrackClicked()
{
	if (bIsLocked) return;

	// ＋追加 → ダイアログを開く
	TSubclassOf<UWG_AddTracksDialog> DlgClass = AddDialogClass;
	if (!DlgClass)
	{
		DlgClass = UWG_AddTracksDialog::StaticClass();
	}

	UWG_AddTracksDialog* Dlg = CreateWidget<UWG_AddTracksDialog>(this, DlgClass);
	if (Dlg)
	{
		Dlg->Setup(this);
		Dlg->AddToViewport(100);
	}
}

void UWG_RadNodzToolkit::HandleMicSettingsClicked()
{
	// 設定 → マイク登録ダイアログを開く
	TSubclassOf<UWG_MicSettingsDialog> DlgClass = MicSettingsDialogClass;
	if (!DlgClass)
	{
		DlgClass = UWG_MicSettingsDialog::StaticClass();
	}

	UWG_MicSettingsDialog* Dlg = CreateWidget<UWG_MicSettingsDialog>(this, DlgClass);
	if (Dlg)
	{
		Dlg->Setup(this);
		Dlg->AddToViewport(100);
	}
}

void UWG_RadNodzToolkit::HandleMonitorClicked()
{
	if (!AudioMonitor)
	{
		AudioMonitor = NewObject<UReaStreamReceiver>(this);
	}

	if (AudioMonitor->IsActive())
	{
		AudioMonitor->Stop();
	}
	else
	{
		// 出力方法（ステレオ / 立体音響 / そのまま）を反映してから受信開始
		ConfigureAudioMonitor();
		// 接続中のReaper以外からのReaStreamパケットは無視する（混線・音割れ対策）。
		AudioMonitor->SetAllowedSenderIP(IPAddress);
		AudioMonitor->Start(this, ReaStreamPort);
	}

	UpdateMonitorButton();
}

void UWG_RadNodzToolkit::UpdateMonitorButton()
{
	using namespace ReaperCtrlUI;

	const bool bOn = AudioMonitor && AudioMonitor->IsActive();
	if (MonitorButton)
	{
		const FLinearColor Bg = bOn ? RecRed : StopGray;   // ON色はマイクと統一
		MonitorButton->SetBackgroundColor(Bg);
		if (MonitorButtonText)
		{
			// ラベルは固定（ON/OFFは色で表現）
			MonitorButtonText->SetText(ReaperLang::T(TEXT("モニター"), TEXT("Monitor")));
			MonitorButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
		}
	}
}

void UWG_RadNodzToolkit::HandleMicSendClicked()
{
	if (!MicSender)
	{
		MicSender = NewObject<UReaStreamSender>(this);
	}

	if (MicSender->IsActive())
	{
		MicSender->Stop();
	}
	else
	{
		// マイク録音権限を要求（Androidのみ。初回は許可ダイアログが出る）
#if PLATFORM_ANDROID
		TArray<FString> Perms;
		if (!UAndroidPermissionFunctionLibrary::CheckPermission(TEXT("android.permission.RECORD_AUDIO")))
		{
			Perms.Add(TEXT("android.permission.RECORD_AUDIO"));
		}
		// Bluetoothマイク利用時は接続権限も要求（Android 12+）
		if (MicInputMode == EMicInputMode::Bluetooth &&
			!UAndroidPermissionFunctionLibrary::CheckPermission(TEXT("android.permission.BLUETOOTH_CONNECT")))
		{
			Perms.Add(TEXT("android.permission.BLUETOOTH_CONNECT"));
		}
		if (Perms.Num() > 0)
		{
			UAndroidPermissionFunctionLibrary::AcquirePermissions(Perms);
		}
#endif
		// 送信先＝接続中のReaperのIP（接続パネルのIP）
		FString Target = IPAddress;
		if (IPAddressInput) { Target = IPAddressInput->GetText().ToString(); }
		MicSender->Start(Target, ReaStreamPort, MicInputMode);
	}

	// マイクキャプチャの開始/停止でオーディオデバイスが再初期化され、モニター再生が止まることがある。
	// モニター受信中なら再起動して再生を復帰させる（送信しながらでもモニターが聞こえるように）。
	RestartMonitorIfActive();

	UpdateMicSendButton();
}

void UWG_RadNodzToolkit::HandleMicInputModeClicked()
{
	// 本体マイク ⇄ Bluetoothマイク をトグル
	MicInputMode = (MicInputMode == EMicInputMode::Builtin)
		? EMicInputMode::Bluetooth
		: EMicInputMode::Builtin;

	UpdateMicInputModeButton();
	SaveSettings();   // 端末に保存（次回起動時に復元）

	// 送信中なら新しい入力経路で再起動して即時反映する
	if (MicSender && MicSender->IsActive())
	{
		FString Target = IPAddress;
		if (IPAddressInput) { Target = IPAddressInput->GetText().ToString(); }
		MicSender->Stop();
		MicSender->Start(Target, ReaStreamPort, MicInputMode);
		UpdateMicSendButton();
	}

}

void UWG_RadNodzToolkit::UpdateMicInputModeButton()
{
	using namespace ReaperCtrlUI;

	if (!MicInputButton || !MicInputButtonText) return;

	const bool bBluetooth = (MicInputMode == EMicInputMode::Bluetooth);
	const FLinearColor Bg = bBluetooth ? AccentGreen : AccentBlue;
	MicInputButton->SetBackgroundColor(Bg);

	FText Label;
	if (bBluetooth)
	{
		// 接続中のBluetoothヘッドセット名を表示。取得できなければ未接続として明示する。
		const FString BtName = UReaStreamSender::GetBluetoothInputDeviceName();
		Label = BtName.IsEmpty()
			? ReaperLang::T(TEXT("Bluetooth（未接続）"), TEXT("Bluetooth (not connected)"))
			: FText::FromString(BtName);
	}
	else
	{
		Label = ReaperLang::T(TEXT("本体マイク"), TEXT("Built-in Mic"));
	}
	MicInputButtonText->SetText(Label);
	MicInputButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
}

void UWG_RadNodzToolkit::HandleMonitorOutputModeClicked()
{
	// ステレオ → 立体音響 → そのまま → ステレオ … と循環
	switch (MonitorOutputMode)
	{
	case EMonitorOutputMode::Stereo:      MonitorOutputMode = EMonitorOutputMode::Spatial;     break;
	case EMonitorOutputMode::Spatial:     MonitorOutputMode = EMonitorOutputMode::Passthrough; break;
	default:                              MonitorOutputMode = EMonitorOutputMode::Stereo;       break;
	}

	UpdateMonitorOutputButton();
	UpdateSpatialSettingsVisibility();   // 立体音響のときだけ調整項目を表示
	SaveSettings();   // 端末に保存（次回起動時に復元）

	// モニター受信中なら新しい出力設定で再起動して即時反映する
	if (AudioMonitor && AudioMonitor->IsActive())
	{
		AudioMonitor->Stop();
		ConfigureAudioMonitor();
		// 接続中のReaper以外からのReaStreamパケットは無視する（混線・音割れ対策）。
		AudioMonitor->SetAllowedSenderIP(IPAddress);
		AudioMonitor->Start(this, ReaStreamPort);
	}
}

void UWG_RadNodzToolkit::UpdateMonitorOutputButton()
{
	using namespace ReaperCtrlUI;

	if (!MonitorOutputButton || !MonitorOutputButtonText) return;

	FText Label;
	FLinearColor Bg = AccentBlue;
	switch (MonitorOutputMode)
	{
	case EMonitorOutputMode::Spatial:
		Label = ReaperLang::T(TEXT("立体音響"), TEXT("3D Spatial"));
		Bg = AccentGreen;
		break;
	case EMonitorOutputMode::Passthrough:
		Label = ReaperLang::T(TEXT("そのまま"), TEXT("As-is"));
		Bg = StopGray;
		break;
	default: // Stereo
		Label = ReaperLang::T(TEXT("ステレオダウンミックス"), TEXT("Stereo Downmix"));
		Bg = AccentBlue;
		break;
	}

	MonitorOutputButton->SetBackgroundColor(Bg);
	MonitorOutputButtonText->SetText(Label);
	MonitorOutputButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
}

void UWG_RadNodzToolkit::UpdateSpatialSettingsVisibility()
{
	// モニター出力=立体音響 のときだけ、立体音響の調整項目を表示する
	const bool bShowSpatial = (MonitorOutputMode == EMonitorOutputMode::Spatial);
	for (UWidget* W : SpatialOnlyWidgets)
	{
		if (W)
		{
			W->SetVisibility(bShowSpatial ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}

	// カスタム配置エディタ（配置図・編集ch・角度・距離）は、立体音響 かつ ch順=カスタム のときだけ表示する。
	// （カスタム以外では効果がないため、淡色表示にせず非表示にして「無効に見える」誤解を防ぐ）
	const bool bShowCustom = bShowSpatial && (SpatialChannelOrder == EBinauralChannelOrder::Custom);
	for (UWidget* W : CustomOnlyWidgets)
	{
		if (W)
		{
			W->SetVisibility(bShowCustom ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}

	// モニター出力=ステレオダウンミックス のときだけ、ダウンミックス係数を表示する
	const bool bShowStereo = (MonitorOutputMode == EMonitorOutputMode::Stereo);
	for (UWidget* W : StereoOnlyWidgets)
	{
		if (W)
		{
			W->SetVisibility(bShowStereo ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}
}

// ====================================================================
//  立体音響（バイノーラル）パラメータ調整
// ====================================================================
namespace
{
	// レベル(0〜3) → 値の対応
	static float ItdMsByLevel(int32 Lv)
	{
		static const float Tbl[4] = { 0.f, 0.30f, 0.65f, 0.90f };
		return Tbl[FMath::Clamp(Lv, 0, 3)];
	}
	static float ShadowAlphaByLevel(int32 Lv)
	{
		// 1.0=減衰なし、小さいほどこもる
		static const float Tbl[4] = { 1.0f, 0.60f, 0.35f, 0.20f };
		return Tbl[FMath::Clamp(Lv, 0, 3)];
	}
	// 受信バッファ：レベル(0〜2) → 秒
	static float BufferSecondsByLevel(int32 Lv)
	{
		static const float Tbl[3] = { 0.4f, 1.0f, 2.0f }; // 低遅延 / 標準 / 安定
		return Tbl[FMath::Clamp(Lv, 0, 2)];
	}
}

void UWG_RadNodzToolkit::ConfigureAudioMonitor()
{
	if (!AudioMonitor) return;
	AudioMonitor->SetOutputMode(MonitorOutputMode);
	AudioMonitor->SetBinauralParams(
		ItdMsByLevel(SpatialItdLevel),
		ShadowAlphaByLevel(SpatialShadowLevel),
		SpatialChannelOrder);
	AudioMonitor->SetCustomLayout(CustomAzimuths, CustomDistances);
	AudioMonitor->SetBufferSeconds(BufferSecondsByLevel(MonitorBufferLevel));
	AudioMonitor->SetDownmixCoeffs(DmGains);
	AudioMonitor->SetHrtfProfile(HrtfProfile);
}

void UWG_RadNodzToolkit::HandleMonitorBufferClicked()
{
	// 低遅延 → 標準 → 安定 → …
	MonitorBufferLevel = (MonitorBufferLevel + 1) % 3;
	UpdateMonitorBufferButton();
	SaveSettings();
	RestartMonitorIfActive();
}

void UWG_RadNodzToolkit::UpdateMonitorBufferButton()
{
	using namespace ReaperCtrlUI;

	if (!MonitorBufferButton || !MonitorBufferButtonText) return;

	FText Label;
	switch (FMath::Clamp(MonitorBufferLevel, 0, 2))
	{
	case 0:  Label = ReaperLang::T(TEXT("低遅延"), TEXT("Low Latency")); break;
	case 2:  Label = ReaperLang::T(TEXT("安定"),   TEXT("Stable"));      break;
	default: Label = ReaperLang::T(TEXT("標準"),   TEXT("Normal"));      break;
	}
	MonitorBufferButton->SetBackgroundColor(AccentBlue);
	MonitorBufferButtonText->SetText(Label);
	MonitorBufferButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(AccentBlue)));
}

void UWG_RadNodzToolkit::EnsureDmGainsDefaults()
{
	if (DmGains.Num() != NumDownmixChannels)
	{
		// 既定: フロントL/R=1.0、C/サラウンド/リア=-3dB(0.7071)、LFE=0（除外）
		const float Def[NumDownmixChannels] = { 1.0f, 1.0f, 0.7071f, 0.0f, 0.7071f, 0.7071f, 0.7071f, 0.7071f };
		DmGains.SetNum(NumDownmixChannels);
		for (int32 i = 0; i < NumDownmixChannels; ++i) { DmGains[i] = Def[i]; }
	}
}

void UWG_RadNodzToolkit::RefreshDmCoeffInputs()
{
	EnsureDmGainsDefaults();
	for (int32 i = 0; i < DmCoeffInputs.Num() && i < DmGains.Num(); ++i)
	{
		if (DmCoeffInputs[i])
		{
			DmCoeffInputs[i]->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), DmGains[i])));
		}
	}
}

void UWG_RadNodzToolkit::HandleDmCoeffCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	EnsureDmGainsDefaults();
	for (int32 i = 0; i < DmCoeffInputs.Num() && i < DmGains.Num(); ++i)
	{
		if (!DmCoeffInputs[i]) continue;
		const FString S = DmCoeffInputs[i]->GetText().ToString().TrimStartAndEnd();
		if (S.IsEmpty()) continue;   // 空欄は現在値を維持
		DmGains[i] = FMath::Clamp(FCString::Atof(*S), 0.0f, 1.5f);
	}
	RefreshDmCoeffInputs();          // クランプ後の値を表示へ反映
	SaveSettings();
	RestartMonitorIfActive();
}

void UWG_RadNodzToolkit::RestartMonitorIfActive()
{
	// モニター受信中なら新しい設定で再起動して即時反映する
	if (AudioMonitor && AudioMonitor->IsActive())
	{
		AudioMonitor->Stop();
		ConfigureAudioMonitor();
		// 接続中のReaper以外からのReaStreamパケットは無視する（混線・音割れ対策）。
		AudioMonitor->SetAllowedSenderIP(IPAddress);
		AudioMonitor->Start(this, ReaStreamPort);
	}
}

void UWG_RadNodzToolkit::EnsureCustomLayoutDefaults()
{
	if (CustomAzimuths.Num() < MaxMappingChannels || CustomDistances.Num() < MaxMappingChannels)
	{
		// 既定：REAPER 8ch 配置（L R C LFE Ls Rs Lb Rb）、距離は1.0
		const float DefAz[MaxMappingChannels] = { -30.f, +30.f, 0.f, 0.f, -110.f, +110.f, -150.f, +150.f };
		CustomAzimuths.SetNum(MaxMappingChannels);
		CustomDistances.SetNum(MaxMappingChannels);
		for (int32 i = 0; i < MaxMappingChannels; ++i)
		{
			CustomAzimuths[i]   = DefAz[i];
			CustomDistances[i]  = 1.0f;
		}
	}
}

void UWG_RadNodzToolkit::HandleSpatialItdClicked()
{
	SpatialItdLevel = (SpatialItdLevel + 1) % 4;
	UpdateSpatialButtons();
	SaveSettings();
	RestartMonitorIfActive();
}

void UWG_RadNodzToolkit::HandleSpatialShadowClicked()
{
	SpatialShadowLevel = (SpatialShadowLevel + 1) % 4;
	UpdateSpatialButtons();
	SaveSettings();
	RestartMonitorIfActive();
}

void UWG_RadNodzToolkit::HandleSpatialOrderClicked()
{
	// ReaperITU → Film → Sequential → Custom → …
	switch (SpatialChannelOrder)
	{
	case EBinauralChannelOrder::ReaperITU:  SpatialChannelOrder = EBinauralChannelOrder::Film;       break;
	case EBinauralChannelOrder::Film:       SpatialChannelOrder = EBinauralChannelOrder::Sequential; break;
	case EBinauralChannelOrder::Sequential: SpatialChannelOrder = EBinauralChannelOrder::Custom;     break;
	default:                                SpatialChannelOrder = EBinauralChannelOrder::ReaperITU;   break;
	}
	UpdateSpatialButtons();
	UpdateMappingControls();
	UpdateSpatialSettingsVisibility();   // ch順=カスタム の切替に応じて配置エディタの表示/非表示を更新
	SaveSettings();
	RestartMonitorIfActive();
}

FString UWG_RadNodzToolkit::HrtfBuiltInLabel()
{
	return ReaperLang::S(TEXT("簡易(内蔵)"), TEXT("Built-in"));
}

FString UWG_RadNodzToolkit::HrtfDisplayNameForInternal(const FString& InternalName)
{
	// 同梱の自前生成プロファイル（実測データではないため「内製」と呼ぶ）だけ翻訳する。
	// 実測データセット（KEMAR等の固有名詞）が増えても、それらはそのまま表示する。
	if (InternalName == TEXT("Generic"))
	{
		return ReaperLang::S(TEXT("内製"), TEXT("Custom-made"));
	}
	return InternalName;
}

FString UWG_RadNodzToolkit::HrtfInternalNameForDisplay(const FString& DisplayName)
{
	if (DisplayName == HrtfDisplayNameForInternal(TEXT("Generic")))
	{
		return TEXT("Generic");
	}
	return DisplayName;
}

UWidget* UWG_RadNodzToolkit::MakeComboItemLight(FString Item)
{
	using namespace ReaperCtrlUI;
	UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	T->SetText(FText::FromString(Item));
	T->SetFont(ReaperFont::Get(18, false));
	T->SetColorAndOpacity(FSlateColor(FieldText));   // 明るい文字（暗背景で読めるように）
	return T;
}

UMediaTrack* UWG_RadNodzToolkit::ResolveParentTrackByName(const FString& Name) const
{
	if (Name.IsEmpty() || Name == TEXT("Master")) return nullptr;
	const FTrackDetail* Found = TrackList.FindByPredicate(
		[&Name](const FTrackDetail& T) { return T.Name == Name; });
	return Found ? Found->Track : nullptr;
}

void UWG_RadNodzToolkit::RefreshHrtfCombo()
{
	if (!HrtfCombo) return;
	HrtfCombo->ClearOptions();

	TArray<FString> Names;
	FReaperHRTFRegistry::Get().GetNames(Names);

	if (Names.Num() == 0)
	{
		// HRTFデータが同梱されていないとき
		HrtfCombo->AddOption(ReaperLang::S(TEXT("（HRTFデータ未同梱）"), TEXT("(no HRTF data)")));
		HrtfCombo->SetSelectedIndex(0);
		return;
	}

	// 登録済みHRTFプロファイル名を、現在の言語での表示名に翻訳して並べる
	// （保存・検索に使う内部名 Names[i] 自体は言語に依存しない固定値のまま）。
	for (const FString& N : Names) { HrtfCombo->AddOption(HrtfDisplayNameForInternal(N)); }

	// 現在の選択が無効/未設定なら先頭プロファイルを既定にする
	if (HrtfProfile.IsEmpty() || Names.IndexOfByKey(HrtfProfile) == INDEX_NONE)
	{
		HrtfProfile = Names[0];
	}
	HrtfCombo->SetSelectedOption(HrtfDisplayNameForInternal(HrtfProfile));
	if (HrtfCombo->GetSelectedIndex() < 0) { HrtfCombo->SetSelectedIndex(0); }
}

void UWG_RadNodzToolkit::HandleHrtfComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// プログラム的な選択（初期化/復元）は無視し、ユーザー操作のみ反映する
	if (SelectionType == ESelectInfo::Direct) return;

	// コンボの表示名から、保存・検索に使う内部名へ戻す
	const FString InternalName = HrtfInternalNameForDisplay(SelectedItem);

	// 実在するHRTFプロファイル名のみ採用（プレースホルダ等は無視）
	if (FReaperHRTFRegistry::Get().FindByName(InternalName))
	{
		HrtfProfile = InternalName;
		SaveSettings();
		RestartMonitorIfActive();   // HRTF切替は受信を再起動して反映
	}
}

void UWG_RadNodzToolkit::UpdateSpatialButtons()
{
	using namespace ReaperCtrlUI;

	auto LevelLabel = [](int32 Lv) -> FText
	{
		switch (FMath::Clamp(Lv, 0, 3))
		{
		case 0:  return ReaperLang::T(TEXT("オフ"), TEXT("Off"));
		case 1:  return ReaperLang::T(TEXT("弱"),   TEXT("Low"));
		case 2:  return ReaperLang::T(TEXT("中"),   TEXT("Mid"));
		default: return ReaperLang::T(TEXT("強"),   TEXT("High"));
		}
	};

	if (SpatialItdButton && SpatialItdButtonText)
	{
		const FLinearColor Bg = (SpatialItdLevel > 0) ? AccentBlue : StopGray;
		SpatialItdButton->SetBackgroundColor(Bg);
		SpatialItdButtonText->SetText(LevelLabel(SpatialItdLevel));
		SpatialItdButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
	}
	if (SpatialShadowButton && SpatialShadowButtonText)
	{
		const FLinearColor Bg = (SpatialShadowLevel > 0) ? AccentBlue : StopGray;
		SpatialShadowButton->SetBackgroundColor(Bg);
		SpatialShadowButtonText->SetText(LevelLabel(SpatialShadowLevel));
		SpatialShadowButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
	}
	if (SpatialOrderButton && SpatialOrderButtonText)
	{
		FText OrderLabel;
		switch (SpatialChannelOrder)
		{
		case EBinauralChannelOrder::Film:       OrderLabel = ReaperLang::T(TEXT("映画"),     TEXT("Film"));       break;
		case EBinauralChannelOrder::Sequential: OrderLabel = ReaperLang::T(TEXT("連番"),     TEXT("Sequential")); break;
		case EBinauralChannelOrder::Custom:     OrderLabel = ReaperLang::T(TEXT("カスタム"), TEXT("Custom"));     break;
		default:                                OrderLabel = ReaperLang::T(TEXT("REAPER"),   TEXT("REAPER"));     break;
		}
		SpatialOrderButton->SetBackgroundColor(AccentBlue);
		SpatialOrderButtonText->SetText(OrderLabel);
		SpatialOrderButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(AccentBlue)));
	}
}

// ====================================================================
//  カスタムch配置エディタ（角度・距離＋図）
// ====================================================================
void UWG_RadNodzToolkit::HandleMapEditChClicked()
{
	EditingChannel = (EditingChannel + 1) % MaxMappingChannels;
	UpdateMappingControls();
}

void UWG_RadNodzToolkit::HandleMapAngleMinusClicked()
{
	EnsureCustomLayoutDefaults();
	float A = CustomAzimuths[EditingChannel] - 15.f;
	if (A < -180.f) A += 360.f;
	CustomAzimuths[EditingChannel] = A;
	UpdateMappingControls();
	SaveSettings();
	RestartMonitorIfActive();
}

void UWG_RadNodzToolkit::HandleMapAnglePlusClicked()
{
	EnsureCustomLayoutDefaults();
	float A = CustomAzimuths[EditingChannel] + 15.f;
	if (A > 180.f) A -= 360.f;
	CustomAzimuths[EditingChannel] = A;
	UpdateMappingControls();
	SaveSettings();
	RestartMonitorIfActive();
}

void UWG_RadNodzToolkit::HandleMapDistMinusClicked()
{
	EnsureCustomLayoutDefaults();
	CustomDistances[EditingChannel] = FMath::Clamp(CustomDistances[EditingChannel] - 0.1f, 0.2f, 2.0f);
	UpdateMappingControls();
	SaveSettings();
	RestartMonitorIfActive();
}

void UWG_RadNodzToolkit::HandleMapDistPlusClicked()
{
	EnsureCustomLayoutDefaults();
	CustomDistances[EditingChannel] = FMath::Clamp(CustomDistances[EditingChannel] + 0.1f, 0.2f, 2.0f);
	UpdateMappingControls();
	SaveSettings();
	RestartMonitorIfActive();
}

void UWG_RadNodzToolkit::UpdateMappingControls()
{
	using namespace ReaperCtrlUI;

	EnsureCustomLayoutDefaults();
	EditingChannel = FMath::Clamp(EditingChannel, 0, MaxMappingChannels - 1);

	if (MapEditChButtonText)
	{
		MapEditChButtonText->SetText(FText::FromString(FString::Printf(TEXT("ch %d"), EditingChannel + 1)));
	}
	if (MapAngleValueText)
	{
		MapAngleValueText->SetText(FText::FromString(FString::Printf(TEXT("%d°"), FMath::RoundToInt(CustomAzimuths[EditingChannel]))));
	}
	if (MapDistValueText)
	{
		MapDistValueText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), CustomDistances[EditingChannel])));
	}

	// カスタム配置エディタは ch順=カスタム のときのみ表示されるため、ここでは淡色化しない（常に全色）。
	UpdateMappingDiagram();
}

void UWG_RadNodzToolkit::UpdateMappingDiagram()
{
	using namespace ReaperCtrlUI;

	if (!MappingCanvas) return;
	EnsureCustomLayoutDefaults();

	const float MaxR = 150.f; // 図の最大半径(px)

	for (int32 i = 0; i < MappingDots.Num(); ++i)
	{
		UBorder* Dot = MappingDots[i];
		if (!Dot) continue;

		const float Az = CustomAzimuths.IsValidIndex(i) ? CustomAzimuths[i] : 0.f;
		const float Dist = CustomDistances.IsValidIndex(i) ? CustomDistances[i] : 1.f;
		const float Rad = FMath::DegreesToRadians(Az);
		const float ScreenR = FMath::Clamp(Dist / 2.0f, 0.1f, 1.0f) * MaxR; // dist 2.0=端

		const float X = FMath::Sin(Rad) * ScreenR;
		const float Y = -FMath::Cos(Rad) * ScreenR; // 前方=上

		if (UCanvasPanelSlot* CS = Cast<UCanvasPanelSlot>(Dot->Slot))
		{
			CS->SetPosition(FVector2D(X, Y));
		}

		// 編集中のchを強調
		const bool bSel = (i == EditingChannel);
		Dot->SetBrushColor(bSel ? AccentGreen : AccentBlue);
	}
}

// ====================================================================
//  表示名・在席登録（グループトークの参加者名簿・自分除外判定に使う）
// ====================================================================
namespace
{
	// 名前をファイル名に使える形へ（不正文字だけ _ に。日本語などはそのまま）
	static FString SanitizePresenceName(const FString& In)
	{
		static const TCHAR BadChars[] = { '\\','/',':','*','?','\"','<','>','|','\t','\r','\n' };
		FString Out = In;
		for (TCHAR& Ch : Out)
		{
			for (TCHAR B : BadChars) { if (Ch == B) { Ch = TEXT('_'); break; } }
		}
		return Out;
	}
}

void UWG_RadNodzToolkit::ResolveAZDataFolder()
{
	if (!Client || !bIsConnected) return;

	// REAPER のリソースパス配下に「AZData」フォルダを用意し、その絶対パスを得る。
	// timeAddType=0 で日時を付けず、毎回同じ AZData フォルダを指す。
	FString OutA, OutB;
	URigdocksSilver_Project::AZ_SetResoucePathFolder(Client, TEXT("AZData"), 0, OutA, OutB);

	// 返り値のうち「AZData を含む絶対パス」を採用（戻り値の順序差に強くする）
	FString Resolved;
	if (OutA.Contains(TEXT("AZData")))      { Resolved = OutA; }
	else if (OutB.Contains(TEXT("AZData"))) { Resolved = OutB; }
	else if (!OutB.IsEmpty())               { Resolved = OutB; }
	else                                    { Resolved = OutA; }

	Resolved.ReplaceInline(TEXT("\\"), TEXT("/"));
	if (Resolved.IsEmpty()) return;

	const bool bChanged = (ResolvedPresenceFolder != Resolved);
	ResolvedPresenceFolder = Resolved;

	// 端末に保存して、次回は接続前でもパスを保持・表示できるようにする
	if (bChanged) { SaveSettings(); }
}

void UWG_RadNodzToolkit::MarkPresenceOffline()
{
	// まだ接続が生きているうちに、自分のグループトーク在席ファイルのタイムスタンプを0（十分に古い値）で
	// 上書きする。ファイル削除APIが無いため、他端末の次回一覧更新（数秒後）で即座に
	// 「オフライン」として除外されるようにするための代替手段。
	if (!Client || !bIsConnected) return;
	const FString Folder0 = GetEffectivePresenceFolder();
	if (IntercomUserName.IsEmpty() || Folder0.IsEmpty()) return;

	const FString MyIP = URadnodzUtility::AZ_GetLocalIP();
	const FString OfflineContent = FString::Printf(TEXT("%s\t%s\t0"), *IntercomUserName, *MyIP);

	FString Folder = Folder0;
	Folder.ReplaceInline(TEXT("\\"), TEXT("/"));
	const FString GroupTalk1File = SanitizePresenceName(IntercomUserName) + TEXT(".grouptalk1.txt");
	const FString GroupTalk2File = SanitizePresenceName(IntercomUserName) + TEXT(".grouptalk2.txt");

	URigdocksSilver_File::AZ_WriteFile(Client, Folder, GroupTalk1File, OfflineContent, 0);
	URigdocksSilver_File::AZ_WriteFile(Client, Folder, GroupTalk2File, OfflineContent, 0);
}

void UWG_RadNodzToolkit::HandleMemberUserNameCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	IntercomUserName = Text.ToString().TrimStartAndEnd();
	SaveSettings();
}

// ===== 横画面対応：ホームのパネルを向きに応じて再配置（reparent）する =====
void UWG_RadNodzToolkit::ApplyHomeOrientation(bool bLandscape)
{
	if (!HomePage) return;

	// いったん全パネルを親から外す（ウィジェット実体・イベント束縛はそのまま、配置だけ変える）。
	UWidget* Panels[] = { ActionPanel, DeviceInfoPanel, ToolbarPanel, TrackListPanel, FooterPanel };
	for (UWidget* P : Panels)
	{
		if (P && P->GetParent()) { P->RemoveFromParent(); }
	}

	if (bLandscape)
	{
		// 左：操作列（縦スクロール）＝ 接続/GTALK → ツールバー → デバイス情報 → フッター。
		if (HomeLandscapeLeftScroll)
		{
			HomeLandscapeLeftScroll->ClearChildren();
			if (ActionPanel)     { HomeLandscapeLeftScroll->AddChild(ActionPanel); }
			if (ToolbarPanel)    { HomeLandscapeLeftScroll->AddChild(ToolbarPanel); }
			if (DeviceInfoPanel) { HomeLandscapeLeftScroll->AddChild(DeviceInfoPanel); }
			if (FooterPanel)     { HomeLandscapeLeftScroll->AddChild(FooterPanel); }
		}

		// 行：[左=操作列(38%)] [右=トラック一覧(62%)]。この比率は携帯で現状問題ないため固定のまま変えない。
		const float LeftRatio = 0.38f;

		// 横向きでは、接続/再生/録音/受信/送信（GTALK）以外のホーム画面ボタンは文字を消してアイコンのみにする
		// （携帯・タブレットいずれでも横向きなら常に適用。全ボタンにアイコンが揃っているため成立する）。
		// ただし削除/全解除/計測調整/Slack/ロックはアイコンのみだと分かりにくいとのことなので、
		// この5個だけは文字を残したまま2段に組み替えて幅に収める（下のInfoRows組み替え処理を参照）。
		{
			auto SetLabelsVisible = [](bool bVisible, std::initializer_list<UTextBlock*> Labels)
			{
				for (UTextBlock* L : Labels)
				{
					if (L) { L->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); }
				}
			};
			SetLabelsVisible(false, {
				AddTrackButtonText, MonitorButtonText, MeterRecvButtonText, UpdateButtonText
			});
			SetLabelsVisible(true, {
				DeleteTrackButtonText, ClearArmedButtonText, NormalizeButtonText, SlackShareButtonText, LockButtonText
			});
		}

		// 再生/録音：横向きは2段レイアウト分だけ縦の余白が減るため、ボタンの高さを詰めて
		// 左列（HomeLandscapeLeftScroll）がスクロールなしで収まるようにする。
		if (PlayButtonSize)   { PlayButtonSize->SetMinDesiredHeight(90.f); }
		if (RecordButtonSize) { RecordButtonSize->SetMinDesiredHeight(90.f); }

		// 削除/全解除/計測調整/Slack/ロック：横向きは幅が足りないため2段（上=削除/全解除/計測調整、
		// 下=Slack/ロック）に組み替える。ボタン本体（SizeBoxで包まれたもの）を移動させるだけで、
		// ボタンオブジェクト自体はポータブル/縦向きと共通のまま使い回す。
		if (InfoRowsWrap && InfoRow && InfoRowLandscapeTop && InfoRowLandscapeBottom)
		{
			auto MoveButtonWrapperIntoRow = [](UHorizontalBox* Row, UButton* Btn, bool bLast)
			{
				if (!Row || !Btn) return;
				UWidget* ToMove = Btn;
				if (USizeBox* Wrap = Cast<USizeBox>(Btn->GetParent())) { ToMove = Wrap; }
				if (ToMove->GetParent()) { ToMove->RemoveFromParent(); }
				UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(ToMove);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				if (!bLast) { S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f)); }
			};
			MoveButtonWrapperIntoRow(InfoRowLandscapeTop, DeleteTrackButton, false);
			MoveButtonWrapperIntoRow(InfoRowLandscapeTop, ClearArmedButton, false);
			MoveButtonWrapperIntoRow(InfoRowLandscapeTop, NormalizeButton, true);
			MoveButtonWrapperIntoRow(InfoRowLandscapeBottom, SlackShareButton, false);
			MoveButtonWrapperIntoRow(InfoRowLandscapeBottom, LockButton, true);

			InfoRowsWrap->ClearChildren();
			{
				UVerticalBoxSlot* S = InfoRowsWrap->AddChildToVerticalBox(InfoRowLandscapeTop);
				S->SetHorizontalAlignment(HAlign_Fill);
				S->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
			}
			{
				UVerticalBoxSlot* S = InfoRowsWrap->AddChildToVerticalBox(InfoRowLandscapeBottom);
				S->SetHorizontalAlignment(HAlign_Fill);
			}
		}

		if (HomeLandscapeRow)
		{
			HomeLandscapeRow->ClearChildren();
			if (HomeLandscapeLeftScroll)
			{
				UHorizontalBoxSlot* S = HomeLandscapeRow->AddChildToHorizontalBox(HomeLandscapeLeftScroll);
				S->SetHorizontalAlignment(HAlign_Fill);
				S->SetVerticalAlignment(VAlign_Fill);
				FSlateChildSize F(ESlateSizeRule::Fill); F.Value = LeftRatio; S->SetSize(F);
			}
			if (TrackListPanel)
			{
				UHorizontalBoxSlot* S = HomeLandscapeRow->AddChildToHorizontalBox(TrackListPanel);
				S->SetHorizontalAlignment(HAlign_Fill);
				S->SetVerticalAlignment(VAlign_Fill);
				FSlateChildSize F(ESlateSizeRule::Fill); F.Value = 1.f - LeftRatio; S->SetSize(F);
			}
		}

		HomePage->ClearChildren();
		if (HomeLandscapeRow)
		{
			UVerticalBoxSlot* S = HomePage->AddChildToVerticalBox(HomeLandscapeRow);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}
	}
	else
	{
		// 縦向きは画面幅いっぱいを使えて文字が収まるため、横向きでアイコンのみに切り替えていても
		// 常に文字表示へ戻す（縦横切り替えのたびアイコンのみのまま残ってしまうのを防ぐ）。
		auto SetLabelsVisible = [](bool bVisible, std::initializer_list<UTextBlock*> Labels)
		{
			for (UTextBlock* L : Labels)
			{
				if (L) { L->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); }
			}
		};
		SetLabelsVisible(true, {
			AddTrackButtonText, MonitorButtonText, MeterRecvButtonText, UpdateButtonText,
			DeleteTrackButtonText, ClearArmedButtonText, NormalizeButtonText, SlackShareButtonText, LockButtonText
		});

		// 再生/録音：縦向きは元の高さに戻す。
		if (PlayButtonSize)   { PlayButtonSize->SetMinDesiredHeight(140.f); }
		if (RecordButtonSize) { RecordButtonSize->SetMinDesiredHeight(140.f); }

		// 削除/全解除/計測調整/Slack/ロック：横向きの2段組みから元の1段(InfoRow)へ戻す。
		if (InfoRowsWrap && InfoRow)
		{
			auto MoveButtonWrapperIntoRow = [](UHorizontalBox* Row, UButton* Btn, bool bLast)
			{
				if (!Row || !Btn) return;
				UWidget* ToMove = Btn;
				if (USizeBox* Wrap = Cast<USizeBox>(Btn->GetParent())) { ToMove = Wrap; }
				if (ToMove->GetParent()) { ToMove->RemoveFromParent(); }
				UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(ToMove);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				if (!bLast) { S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f)); }
			};
			MoveButtonWrapperIntoRow(InfoRow, DeleteTrackButton, false);
			MoveButtonWrapperIntoRow(InfoRow, ClearArmedButton, false);
			MoveButtonWrapperIntoRow(InfoRow, NormalizeButton, false);
			MoveButtonWrapperIntoRow(InfoRow, SlackShareButton, false);
			MoveButtonWrapperIntoRow(InfoRow, LockButton, true);

			InfoRowsWrap->ClearChildren();
			UVerticalBoxSlot* S = InfoRowsWrap->AddChildToVerticalBox(InfoRow);
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		// 縦：従来のタテ積み（接続/GTALK → デバイス情報 → ツールバー → トラック一覧(Fill) → フッター）。
		if (HomePortraitCol)
		{
			HomePortraitCol->ClearChildren();
			auto AddP = [&](UWidget* P, bool bFill)
			{
				if (!P) return;
				UVerticalBoxSlot* S = HomePortraitCol->AddChildToVerticalBox(P);
				S->SetHorizontalAlignment(HAlign_Fill);
				S->SetPadding(FMargin(0.f, 1.f, 0.f, 0.f));
				if (bFill) { S->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); }
			};
			AddP(ActionPanel, false);
			AddP(DeviceInfoPanel, false);
			AddP(ToolbarPanel, false);
			AddP(TrackListPanel, true);
			AddP(FooterPanel, false);
		}

		HomePage->ClearChildren();
		if (HomePortraitCol)
		{
			UVerticalBoxSlot* S = HomePage->AddChildToVerticalBox(HomePortraitCol);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}
	}
}

// ===== グループトーク（2チャンネル・ローカル マスター/サブ） =====
// ボタン束縛用の無引数サンク（OnPressed/OnReleasedは無引数マルチキャストのためch毎に用意）。
void UWG_RadNodzToolkit::HandleGroupTalkRecv0Pressed()
{
	GroupTalkRecvPressStart[0] = FPlatformTime::Seconds();
	bGroupTalkRecvPressing[0] = true;
}
void UWG_RadNodzToolkit::HandleGroupTalkRecv0Released()
{
	if (!bGroupTalkRecvPressing[0]) return;
	bGroupTalkRecvPressing[0] = false;
	const double Held = FPlatformTime::Seconds() - GroupTalkRecvPressStart[0];
	if (Held >= GroupTalkLongPressSec) { GroupTalkOnRecvLongPress(0); }
	else                               { GroupTalkOnRecvTap(0); }
}
void UWG_RadNodzToolkit::HandleGroupTalkRecv1Pressed()
{
	GroupTalkRecvPressStart[1] = FPlatformTime::Seconds();
	bGroupTalkRecvPressing[1] = true;
}
void UWG_RadNodzToolkit::HandleGroupTalkRecv1Released()
{
	if (!bGroupTalkRecvPressing[1]) return;
	bGroupTalkRecvPressing[1] = false;
	const double Held = FPlatformTime::Seconds() - GroupTalkRecvPressStart[1];
	if (Held >= GroupTalkLongPressSec) { GroupTalkOnRecvLongPress(1); }
	else                               { GroupTalkOnRecvTap(1); }
}
void UWG_RadNodzToolkit::HandleGroupTalkSend0Clicked()  { GroupTalkOnSendClicked(0); }
void UWG_RadNodzToolkit::HandleGroupTalkSend0Pressed()  { GroupTalkOnSendPressed(0); }
void UWG_RadNodzToolkit::HandleGroupTalkSend0Released() { GroupTalkOnSendReleased(0); }
void UWG_RadNodzToolkit::HandleGroupTalkSend1Clicked()  { GroupTalkOnSendClicked(1); }
void UWG_RadNodzToolkit::HandleGroupTalkSend1Pressed()  { GroupTalkOnSendPressed(1); }
void UWG_RadNodzToolkit::HandleGroupTalkSend1Released() { GroupTalkOnSendReleased(1); }

void UWG_RadNodzToolkit::GroupTalkOnRecvTap(int32 Ch)
{
	if (Ch < 0 || Ch > 1) return;

	// 名前未設定なら設定タブへ誘導（送受信には自分の名前が必要）。
	if (IntercomUserName.IsEmpty())
	{
		SetActiveTab(2);
		return;
	}

	bGroupTalkRecvOn[Ch] = !bGroupTalkRecvOn[Ch];
	if (bGroupTalkRecvOn[Ch])
	{
		if (GroupTalkMasterCh == -1) { GroupTalkMasterCh = Ch; }   // 初回：最初に受信ONにしたchがマスター
		StartGroupTalkReceiver(Ch);
	}
	else
	{
		StopGroupTalkReceiver(Ch);
	}

	WriteGroupTalkHeartbeat(Ch);
	RefreshGroupTalkRoster();
	ApplyTransmitRouting();
	UpdateGroupTalkUI();
}

void UWG_RadNodzToolkit::GroupTalkOnRecvLongPress(int32 Ch)
{
	if (Ch < 0 || Ch > 1) return;
	GroupTalkMasterCh = Ch;        // このchを自分のマスター（主）にする
	bGroupTalkSubHeld = false;     // マスター切替時は押しっぱなし状態をクリア（取り残し防止）
	ApplyTransmitRouting();
	UpdateGroupTalkUI();
}

void UWG_RadNodzToolkit::GroupTalkOnSendClicked(int32 Ch)
{
	if (Ch != GroupTalkMasterCh) return;   // マスターchのタップのみラッチ反転（サブはPTTのみ）
	bGroupTalkMasterLatch = !bGroupTalkMasterLatch;
	WriteGroupTalkHeartbeat(Ch);
	ApplyTransmitRouting();
	UpdateGroupTalkUI();
}

void UWG_RadNodzToolkit::GroupTalkOnSendPressed(int32 Ch)
{
	if (Ch != GroupTalkSubCh()) return;    // サブchの長押しのみPTT（マスターはOnClickedで処理）
	bGroupTalkSubHeld = true;
	WriteGroupTalkHeartbeat(Ch);
	ApplyTransmitRouting();
	UpdateGroupTalkUI();
}

void UWG_RadNodzToolkit::GroupTalkOnSendReleased(int32 Ch)
{
	if (Ch != GroupTalkSubCh()) return;
	if (!bGroupTalkSubHeld) return;
	bGroupTalkSubHeld = false;
	ApplyTransmitRouting();                // マスターへ戻る（ラッチONなら再開／OFFなら無送信）
	UpdateGroupTalkUI();
}

int32 UWG_RadNodzToolkit::ComputeTransmitTarget() const
{
	if (!bIsConnected || IntercomUserName.IsEmpty()) return -1;
	if (GroupTalkMasterCh == -1) return -1;
	if (bGroupTalkSubHeld && GroupTalkSubCh() != -1) return GroupTalkSubCh();   // PTTが最優先
	if (bGroupTalkMasterLatch) return GroupTalkMasterCh;
	return -1;
}

void UWG_RadNodzToolkit::ApplyTransmitRouting()
{
	const int32 Target = ComputeTransmitTarget();
	if (Target < 0)
	{
		if (GroupTalkSender && GroupTalkSender->IsActive()) { GroupTalkSender->Stop(); }   // マイク解放
		return;
	}

	EnsureGroupTalkSenderStarted();
	if (!GroupTalkSender) return;

	TArray<FString> MemberIPs;
	GroupTalkMembers[Target].GenerateValueArray(MemberIPs);
	GroupTalkSender->SetGroupTargets(MemberIPs, GroupTalkPortForCh(Target));   // 宛先IP＋ポートを排他切替
}

void UWG_RadNodzToolkit::EnsureGroupTalkSenderStarted()
{
	if (!GroupTalkSender) { GroupTalkSender = NewObject<UReaStreamSender>(this); }
	if (GroupTalkSender->IsActive()) return;

	// マイク録音権限（Androidのみ。初回は許可ダイアログ）
#if PLATFORM_ANDROID
	TArray<FString> Perms;
	if (!UAndroidPermissionFunctionLibrary::CheckPermission(TEXT("android.permission.RECORD_AUDIO")))
	{
		Perms.Add(TEXT("android.permission.RECORD_AUDIO"));
	}
	if (MicInputMode == EMicInputMode::Bluetooth &&
		!UAndroidPermissionFunctionLibrary::CheckPermission(TEXT("android.permission.BLUETOOTH_CONNECT")))
	{
		Perms.Add(TEXT("android.permission.BLUETOOTH_CONNECT"));
	}
	if (Perms.Num() > 0)
	{
		UAndroidPermissionFunctionLibrary::AcquirePermissions(Perms);
	}
#endif

	// Start()はTargetIPの妥当性検証が必要なため、ダミーでループバックを指定して開始し、
	// 直後に ApplyTransmitRouting() が本当の送信先IP・ポートで上書きする。
	GroupTalkSender->Start(TEXT("127.0.0.1"), GroupTalkPort0, MicInputMode);
}

void UWG_RadNodzToolkit::StartGroupTalkReceiver(int32 Ch)
{
	if (Ch < 0 || Ch > 1) return;
	if (!bIsConnected) return;
	if (!GroupTalkReceiver[Ch]) { GroupTalkReceiver[Ch] = NewObject<UReaStreamGroupReceiver>(this); }
	if (GroupTalkReceiver[Ch]->IsActive()) { GroupTalkReceiver[Ch]->Stop(); }
	GroupTalkReceiver[Ch]->Start(this, GroupTalkPortForCh(Ch));

	TArray<FString> MemberIPs;
	GroupTalkMembers[Ch].GenerateValueArray(MemberIPs);
	GroupTalkReceiver[Ch]->SetAllowedMembers(MemberIPs);
}

void UWG_RadNodzToolkit::StopGroupTalkReceiver(int32 Ch)
{
	if (Ch < 0 || Ch > 1) return;
	if (GroupTalkReceiver[Ch] && GroupTalkReceiver[Ch]->IsActive()) { GroupTalkReceiver[Ch]->Stop(); }
}

bool UWG_RadNodzToolkit::IsGroupTalkChannelActive(int32 Ch) const
{
	if (Ch < 0 || Ch > 1) return false;
	return bGroupTalkRecvOn[Ch] || (ComputeTransmitTarget() == Ch);
}

void UWG_RadNodzToolkit::WriteGroupTalkHeartbeat(int32 Ch)
{
	if (Ch < 0 || Ch > 1) return;
	if (!Client || !bIsConnected) return;
	const FString Folder0 = GetEffectivePresenceFolder();
	if (IntercomUserName.IsEmpty() || Folder0.IsEmpty()) return;

	const FString MyIP = URadnodzUtility::AZ_GetLocalIP();
	if (MyIP.IsEmpty()) return;

	// そのchでアクティブ（受信ON または 送信中）なら現在時刻、そうでなければ ts=0 で
	// オフライン扱いにし、相手から素早くドロップされるようにする。
	const int64 NowSec = IsGroupTalkChannelActive(Ch) ? FDateTime::UtcNow().ToUnixTimestamp() : 0;
	// 中身：名前 \t IP \t UNIX秒（メンバー在席と同じ形式）
	const FString Content = FString::Printf(TEXT("%s\t%s\t%lld"), *IntercomUserName, *MyIP, NowSec);

	FString Folder = Folder0;
	Folder.ReplaceInline(TEXT("\\"), TEXT("/"));
	const FString FileName = SanitizePresenceName(IntercomUserName)
		+ (Ch == 0 ? TEXT(".grouptalk1.txt") : TEXT(".grouptalk2.txt"));

	URigdocksSilver_File::AZ_WriteFile(Client, Folder, FileName, Content, 0);
}

void UWG_RadNodzToolkit::RefreshGroupTalkRoster()
{
	if (!Client || !bIsConnected) return;
	FString Folder = GetEffectivePresenceFolder();
	if (Folder.IsEmpty()) return;
	Folder.ReplaceInline(TEXT("\\"), TEXT("/"));

	GroupTalkMembers[0].Reset();
	GroupTalkMembers[1].Reset();

	const int64 NowSec = FDateTime::UtcNow().ToUnixTimestamp();
	const int64 StaleSec = 15;   // 参加は素早く反映したいので、在席一覧(30秒)より短めにする

	// AZ_GetFilePathList の search_extension は先頭にドットを付けない形式("txt")でないと
	// 一致せず、".txt" や "*.txt" では常に0件になる（実機ログで確認済み）。
	// ".grouptalk1/2.txt" かどうかの判定はこちら側(EndsWith)で行う。
	TArray<FString> Files = URigdocksSilver_File::AZ_GetFilePathList(Client, Folder, TEXT("txt"));
	for (const FString& Path : Files)
	{
		FString P = Path;
		P.ReplaceInline(TEXT("\\"), TEXT("/"));
		int32 Ch = -1;
		if      (P.EndsWith(TEXT(".grouptalk1.txt"))) { Ch = 0; }
		else if (P.EndsWith(TEXT(".grouptalk2.txt"))) { Ch = 1; }
		else { continue; }   // 通常の在席ファイル等は除外

		const FString Dir  = FPaths::GetPath(P);
		const FString File = FPaths::GetCleanFilename(P);
		const FString Text = URigdocksSilver_File::AZ_ReadFile(Client, Dir.IsEmpty() ? Folder : Dir, File);
		if (Text.IsEmpty()) continue;

		TArray<FString> Parts;
		Text.ParseIntoArray(Parts, TEXT("\t"), false);
		if (Parts.Num() < 3) continue;

		const FString Name = Parts[0].TrimStartAndEnd();
		const FString IP   = Parts[1].TrimStartAndEnd();
		const int64   Ts   = FCString::Atoi64(*Parts[2].TrimStartAndEnd());
		if (Name.IsEmpty() || IP.IsEmpty()) continue;
		if (NowSec - Ts > StaleSec) continue;            // 参加をやめた（書き込みが止まった/オフライン刻印）人は除外
		if (Name == IntercomUserName) continue;          // 自分は除外

		GroupTalkMembers[Ch].Add(Name, IP);
	}

	// 各chの受信許可リストを最新の参加者IPで更新する。
	for (int32 Ch = 0; Ch < 2; ++Ch)
	{
		if (GroupTalkReceiver[Ch])
		{
			TArray<FString> MemberIPs;
			GroupTalkMembers[Ch].GenerateValueArray(MemberIPs);
			GroupTalkReceiver[Ch]->SetAllowedMembers(MemberIPs);
		}
	}

	// 送信先IPが変わった可能性があるので送信ルーティングも更新する。
	ApplyTransmitRouting();
	UpdateGroupTalkUI();
}

void UWG_RadNodzToolkit::UpdateGroupTalkUI()
{
	using namespace ReaperCtrlUI;
	for (int32 Ch = 0; Ch < 2; ++Ch)
	{
		const bool bIsMaster = (GroupTalkMasterCh == Ch);

		// 参加者〇の行を、現在の参加者（自分＋他の参加者）に合わせて作り直す。
		if (GroupTalkAvatarRow[Ch])
		{
			GroupTalkAvatarRow[Ch]->ClearChildren();

			TArray<FString> Names;
			if (IsGroupTalkChannelActive(Ch) && !IntercomUserName.IsEmpty())
			{
				Names.Add(IntercomUserName);
			}
			TArray<FString> OtherNames;
			GroupTalkMembers[Ch].GenerateKeyArray(OtherNames);
			Names.Append(OtherNames);

			for (const FString& Name : Names)
			{
				UWidget* Avatar = MakeAvatarCircle(WidgetTree, Name, 32.f);
				UHorizontalBoxSlot* S = GroupTalkAvatarRow[Ch]->AddChildToHorizontalBox(Avatar);
				S->SetPadding(FMargin(2.f, 0.f));
			}
		}

		// 受信ボタン：OFF=StopGray / ON=BtnGreenDark / ON&マスター=AccentBlue。
		// マスターch設定は「受信中」ではないため、受信していない間は常にStopGray（無着色）にする。
		if (GroupTalkRecvButton[Ch])
		{
			FLinearColor RecvBg = StopGray;
			if (bGroupTalkRecvOn[Ch]) { RecvBg = bIsMaster ? AccentBlue : BtnGreenDark; }
			GroupTalkRecvButton[Ch]->SetBackgroundColor(RecvBg);
		}

		// 送信ボタン：待機=StopGray / マスターラッチ中=RecRed / サブ長押し中=LockOrange。
		FLinearColor SendBg = StopGray;
		bool bSendActive = false;
		if (bIsMaster)
		{
			if (bGroupTalkMasterLatch) { SendBg = RecRed; bSendActive = true; }
		}
		else if (Ch == GroupTalkSubCh())
		{
			if (bGroupTalkSubHeld) { SendBg = LockOrange; bSendActive = true; }
		}
		if (GroupTalkSendButton[Ch])
		{
			GroupTalkSendButton[Ch]->SetBackgroundColor(SendBg);
		}

		// 枠の色も送信状態に合わせて変える：指でボタンを押していて見えなくても、枠の色で分かるようにする。
		if (GroupTalkFrame[Ch])
		{
			const FLinearColor OutlineColor = bSendActive ? SendBg : FieldOutline;
			const float OutlineWidth = bSendActive ? 3.f : 2.f;
			GroupTalkFrame[Ch]->SetBrush(FSlateRoundedBoxBrush(
				FLinearColor(0.f, 0.f, 0.f, 0.18f), 8.f, OutlineColor, OutlineWidth, FVector2D(1.f, 1.f)));
		}
	}
}

void UWG_RadNodzToolkit::UpdateMicSendButton()
{
	using namespace ReaperCtrlUI;

	const bool bOn = MicSender && MicSender->IsActive();
	if (MicSendButton)
	{
		const FLinearColor Bg = bOn ? RecRed : StopGray;
		MicSendButton->SetBackgroundColor(Bg);
		if (MicSendButtonText)
		{
			// ラベルは固定「マイク」（ON/OFFは色で表現）
			MicSendButtonText->SetText(ReaperLang::T(TEXT("マイク"), TEXT("Mic")));
			MicSendButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
		}
	}
}

void UWG_RadNodzToolkit::HandleMeterReceiveClicked()
{
	// ホーム/設定どちらのボタンからでも、同じ状態をトグルする
	SetMeterReceiveEnabled(!bMeterReceiveEnabled);
}

void UWG_RadNodzToolkit::SetMeterReceiveEnabled(bool bEnable)
{
	bMeterReceiveEnabled = bEnable;

	// 次のTickでON直後すぐにポーリングが走るようタイマーをリセット
	MeterPollTimer = 0.0;

	// 各トラックカードのメーター表示/非表示を更新（OFFなら非表示＝取得もしない）
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (Card) { Card->RefreshMeterVisibility(); }
	}

	UpdateMeterReceiveButtons();
}

void UWG_RadNodzToolkit::UpdateMeterReceiveButtons()
{
	using namespace ReaperCtrlUI;

	const bool bOn = bMeterReceiveEnabled;
	const FLinearColor Bg = bOn ? RecRed : StopGray;   // ON色はマイクと統一

	// ホーム画面のボタン：ラベルは固定「メーター」（ON/OFFは色で表現）
	if (MeterRecvButton)
	{
		MeterRecvButton->SetBackgroundColor(Bg);
		if (MeterRecvButtonText)
		{
			MeterRecvButtonText->SetText(ReaperLang::T(TEXT("メーター"), TEXT("Meter")));
			MeterRecvButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
		}
	}

	// 設定画面のボタン（ホームと連動）
	if (MeterRecvSettingButton)
	{
		MeterRecvSettingButton->SetBackgroundColor(Bg);
		if (MeterRecvSettingButtonText)
		{
			MeterRecvSettingButtonText->SetText(ReaperLang::T(bOn ? TEXT("ON") : TEXT("OFF"),
				bOn ? TEXT("ON") : TEXT("OFF")));
			MeterRecvSettingButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
		}
	}
}

void UWG_RadNodzToolkit::HandleLanguageComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// プログラム的な選択（初期化/復元）は無視し、ユーザー操作のみ反映する
	if (SelectionType == ESelectInfo::Direct) return;

	ReaperLang::SetLanguage(ReaperLang::FromNativeName(SelectedItem));
	ApplyLanguage();
	SaveSettings();   // 端末に保存（次回起動時に復元）
}

void UWG_RadNodzToolkit::RefreshLanguageCombo()
{
	if (!LanguageCombo) return;
	LanguageCombo->SetSelectedOption(ReaperLang::NativeName(ReaperLang::GetLanguage()));
}

void UWG_RadNodzToolkit::HandleThemeClicked()
{
	// 標準(0) → さらに暗め(1) → 明るめ(2) → 標準(0) … と循環
	UIThemeIndex = (UIThemeIndex + 1) % 3;
	ApplyTheme();
	UpdateThemeButton();
	SaveSettings();   // 端末に保存（次回起動時に復元）
}

void UWG_RadNodzToolkit::UpdateThemeButton()
{
	if (!ThemeButtonText) return;
	FString Label;
	switch (UIThemeIndex)
	{
	case 1:  Label = ReaperLang::S(TEXT("さらに暗め"), TEXT("Darker"));  break;
	case 2:  Label = ReaperLang::S(TEXT("明るめ"),     TEXT("Lighter")); break;
	default: Label = ReaperLang::S(TEXT("標準（黒）"), TEXT("Default")); break;
	}
	ThemeButtonText->SetText(FText::FromString(Label));
}

void UWG_RadNodzToolkit::ApplyTheme()
{
	using namespace ReaperCtrlUI;

	// 背景4色を選択テーマへ更新し、保持している背景ボーダーへ貼り替える
	ApplyThemeColors(UIThemeIndex);
	for (UBorder* B : ThemeScreenBgBorders) { if (B) { B->SetBrushColor(ScreenBg); } }
	for (UBorder* B : ThemeHeaderBgBorders) { if (B) { B->SetBrushColor(HeaderBg); } }
	for (UBorder* B : ThemePanelBgBorders)  { if (B) { B->SetBrushColor(PanelBg);  } }
	for (UBorder* B : ThemeListBgBorders)   { if (B) { B->SetBrushColor(ListBg);   } }

	// トラック一覧パネルは録音中の赤枠ブラシを使うため、枠の状態を保ったまま地色を更新する
	if (TrackListPanel)
	{
		const float Radius = 8.f;
		if (bIsRecording)
		{
			const FLinearColor RecFrame(0.95f, 0.16f, 0.20f, 1.f);
			TrackListPanel->SetBrush(FSlateRoundedBoxBrush(ListBg, Radius, RecFrame, 5.f));
		}
		else
		{
			TrackListPanel->SetBrush(FSlateRoundedBoxBrush(ListBg, Radius, FLinearColor::Transparent, 0.f));
		}
	}
}

void UWG_RadNodzToolkit::HandleOrientationClicked()
{
	// 自動(0) → 縦(1) → 横(2) → 自動(0) … と循環
	ScreenOrientationLock = (ScreenOrientationLock + 1) % 3;
	ApplyOrientation();
	UpdateOrientationButton();
	SaveSettings();   // 端末に保存（次回起動時に復元）
}

void UWG_RadNodzToolkit::UpdateOrientationButton()
{
	if (!OrientationButtonText) return;
	FString Label;
	switch (ScreenOrientationLock)
	{
	case 1:  Label = ReaperLang::S(TEXT("縦"), TEXT("Portrait"));  break;
	case 2:  Label = ReaperLang::S(TEXT("横"), TEXT("Landscape")); break;
	default: Label = ReaperLang::S(TEXT("自動"), TEXT("Auto"));    break;
	}
	OrientationButtonText->SetText(FText::FromString(Label));
}

void UWG_RadNodzToolkit::ApplyOrientation()
{
	// ScreenOrientationLockに応じて端末の許可する向きをOS側へ反映する
	EScreenOrientation::Type Allowed = EScreenOrientation::FullSensor;
	switch (ScreenOrientationLock)
	{
	case 1:  Allowed = EScreenOrientation::Portrait;      break;
	case 2:  Allowed = EScreenOrientation::LandscapeLeft; break;
	default: Allowed = EScreenOrientation::FullSensor;    break;
	}
	UBlueprintPlatformLibrary::SetAllowedDeviceOrientation(Allowed);
}

void UWG_RadNodzToolkit::ApplyLanguage()
{
	using namespace ReaperCtrlUI;

	// 言語コンボの選択状態を同期
	RefreshLanguageCombo();

	// 静的ラベル/ボタン
	if (AddTrackButtonText)    AddTrackButtonText->SetText(ReaperLang::T(TEXT("追加"), TEXT("Add")));
	if (MicSettingsButtonText) MicSettingsButtonText->SetText(ReaperLang::T(TEXT("開く"), TEXT("Open")));
	if (UpdateButtonText)      UpdateButtonText->SetText(ReaperLang::T(TEXT("更新"), TEXT("Refresh")));
	if (ClearArmedButtonText)  ClearArmedButtonText->SetText(ReaperLang::T(TEXT("全解除"), TEXT("Clear All")));
	if (DeleteTrackButtonText) DeleteTrackButtonText->SetText(ReaperLang::T(bDeleteMode ? TEXT("決定") : TEXT("削除"), bDeleteMode ? TEXT("OK") : TEXT("Delete")));
	// 計測調整ボタン（ジョブ実行中でなければ既定ラベルへ。言語切替で英語/日本語を反映）
	if (NormalizeButtonText && !bMeasureJobActive) NormalizeButtonText->SetText(ReaperLang::T(TEXT("計測調整"), TEXT("Normalize")));
	if (SlackShareButtonText && !bMeasureJobActive)  SlackShareButtonText->SetText(ReaperLang::T(TEXT("Slack"), TEXT("Slack")));
	if (DeleteSelectAllButtonText)   DeleteSelectAllButtonText->SetText(ReaperLang::T(TEXT("全選択"), TEXT("Select All")));
	if (DeleteDeselectAllButtonText) DeleteDeselectAllButtonText->SetText(ReaperLang::T(TEXT("全解除"), TEXT("Deselect All")));
	if (TrackSectionLabel)     TrackSectionLabel->SetText(ReaperLang::T(TEXT("トラック"), TEXT("Tracks")));
	// グループトークの「受信/送信」見出し（ローカル変数のままだと再適用できないため、メンバ化して保持している）
	for (int32 Ch = 0; Ch < 2; ++Ch)
	{
		if (GroupTalkRecvCaption[Ch]) { GroupTalkRecvCaption[Ch]->SetText(ReaperLang::T(TEXT("受信"), TEXT("Recv"))); }
		if (GroupTalkSendCaption[Ch]) { GroupTalkSendCaption[Ch]->SetText(ReaperLang::T(TEXT("送信"), TEXT("Send"))); }
	}
	UpdateLockButton();
	if (ScanButtonText)       ScanButtonText->SetText(ReaperLang::T(TEXT("検索"), TEXT("Scan")));
	if (ListLoadButtonText)   ListLoadButtonText->SetText(ReaperLang::T(TEXT("読み込み"), TEXT("Load")));
	if (ListExportButtonText) ListExportButtonText->SetText(ReaperLang::T(TEXT("書き出し"), TEXT("Export")));
	if (RecordingCheckButtonText) RecordingCheckButtonText->SetText(ReaperLang::T(TEXT("台本一致"), TEXT("Script Match")));
	if (ScriptMatchScopeRowLabel)       ScriptMatchScopeRowLabel->SetText(ReaperLang::T(TEXT("台本一致の対象範囲"), TEXT("Script match scope")));
	if (ScriptMatchCheckFilterRowLabel) ScriptMatchCheckFilterRowLabel->SetText(ReaperLang::T(TEXT("台本一致のチェック状態"), TEXT("Script match check state")));
	UpdateScriptMatchScopeButton();
	UpdateScriptMatchCheckFilterButton();

	// 下部タブ・設定画面のラベル
	if (HomeTabText)           HomeTabText->SetText(ReaperLang::T(TEXT("ホーム"), TEXT("Home")));
	if (SettingsTabText)       SettingsTabText->SetText(ReaperLang::T(TEXT("設定"), TEXT("Settings")));
	if (SettingsTitleText)     SettingsTitleText->SetText(ReaperLang::T(TEXT("設定"), TEXT("Settings")));
	if (ConnSectionLabel)      ConnSectionLabel->SetText(ReaperLang::T(TEXT("接続"), TEXT("Connection")));
	if (LocalIpRowLabel)       LocalIpRowLabel->SetText(ReaperLang::T(TEXT("この端末のIP"), TEXT("This device IP")));
	if (IpLabel)               IpLabel->SetText(ReaperLang::T(TEXT("Reaper IP"), TEXT("Reaper IP")));
	if (PortLabel)             PortLabel->SetText(ReaperLang::T(TEXT("ポート"), TEXT("Port")));
	if (ReaStreamPortRowLabel)   ReaStreamPortRowLabel->SetText(ReaperLang::T(TEXT("モニター/マイクのポート"), TEXT("Monitor/Mic port")));
	if (ShowDeviceInfoRowLabel)  ShowDeviceInfoRowLabel->SetText(ReaperLang::T(TEXT("端末名/IPを表示"), TEXT("Show device name/IP")));
	UpdateShowDeviceInfoButton();
	if (CommTimeRowLabel)        CommTimeRowLabel->SetText(ReaperLang::T(TEXT("Reaper通信時間"), TEXT("Reaper comm time")));
	if (CommTimeMeasureButtonText) CommTimeMeasureButtonText->SetText(ReaperLang::T(TEXT("測定"), TEXT("Measure")));
	if (IntercomSectionLabel)  IntercomSectionLabel->SetText(ReaperLang::T(TEXT("表示名"), TEXT("Display Name")));
	if (MemberUserNameRowLabel) MemberUserNameRowLabel->SetText(ReaperLang::T(TEXT("自分の名前"), TEXT("My name")));
	if (SlackSectionLabel)     SlackSectionLabel->SetText(ReaperLang::T(TEXT("Slack共有"), TEXT("Slack Share")));
	if (SlackTokenRowLabel)    SlackTokenRowLabel->SetText(ReaperLang::T(TEXT("Botトークン"), TEXT("Bot Token")));
	if (SlackToUserRowLabel)   SlackToUserRowLabel->SetText(ReaperLang::T(TEXT("宛先ユーザー"), TEXT("To User")));
	if (LoudnessSectionLabel)  LoudnessSectionLabel->SetText(ReaperLang::T(TEXT("ラウドネス"), TEXT("Loudness")));
	if (TargetLufsRowLabel)    TargetLufsRowLabel->SetText(ReaperLang::T(TEXT("目標ラウドネス(LUFS)"), TEXT("Target (LUFS)")));
	if (LimitPeakRowLabel)     LimitPeakRowLabel->SetText(ReaperLang::T(TEXT("ピーク超過を防ぐ"), TEXT("Limit peak")));
	UpdateLimitPeakButton();
	if (MeterSectionLabel)     MeterSectionLabel->SetText(ReaperLang::T(TEXT("メーター"), TEXT("Meter")));
	if (MeterRecvRowLabel)     MeterRecvRowLabel->SetText(ReaperLang::T(TEXT("メーター受信"), TEXT("Meter Recv")));
	if (MeterIntervalRowLabel) MeterIntervalRowLabel->SetText(ReaperLang::T(TEXT("更新間隔(ms)"), TEXT("Update interval (ms)")));
	if (MeterModeRowLabel)     MeterModeRowLabel->SetText(ReaperLang::T(TEXT("表示"), TEXT("Display")));
	if (MeterDotScopeRowLabel) MeterDotScopeRowLabel->SetText(ReaperLang::T(TEXT("〇の数"), TEXT("Dot count")));
	if (MeterDescText) MeterDescText->SetText(ReaperLang::T(
		TEXT("フルメーター＝詳細表示（やや重い）。信号〇＝緑:信号あり／赤:なし（軽い）。\n〇は「chごと」か「トラックに1つ」を選択可（1つは取得も減って最も軽い）。\n更新間隔を大きくするほど軽くなります。"),
		TEXT("Full meter = detailed (heavier). Signal dot = green:signal / red:none (light).\nDot can be 'per channel' or 'one per track' (one = fewest queries, lightest).\nLarger update interval = lighter.")));
	UpdateMeterModeButton();
	UpdateMeterDotScopeButton();
	if (AppSectionLabel)       AppSectionLabel->SetText(ReaperLang::T(TEXT("アプリ"), TEXT("App")));
	if (LanguageRowLabel)      LanguageRowLabel->SetText(ReaperLang::T(TEXT("言語"), TEXT("Language")));
	if (ThemeRowLabel)         ThemeRowLabel->SetText(ReaperLang::T(TEXT("テーマ(背景)"), TEXT("Theme (BG)")));
	UpdateThemeButton();
	UpdateMeterReceiveButtons();

	if (TrackRefreshSectionLabel)    TrackRefreshSectionLabel->SetText(ReaperLang::T(TEXT("トラック一覧の更新"), TEXT("Track List Refresh")));
	if (TrackRefreshModeRowLabel)    TrackRefreshModeRowLabel->SetText(ReaperLang::T(TEXT("更新方法"), TEXT("Refresh mode")));
	UpdateTrackRefreshModeButton();
	if (TrackRefreshIntervalRowLabel) TrackRefreshIntervalRowLabel->SetText(ReaperLang::T(TEXT("自動更新の間隔(秒)"), TEXT("Auto refresh interval (sec)")));

	// リスト関連
	if (ListTabText)           ListTabText->SetText(ReaperLang::T(TEXT("リスト"), TEXT("List")));
	if (ListTitleText)         ListTitleText->SetText(ReaperLang::T(TEXT("収録リスト"), TEXT("Recording List")));
	// リストが一度も読み込まれていない場合のみ、案内文を現在の言語で再表示する（読み込み後は別の状態文言に上書きされるため対象外）。
	if (ListStatusText && ListSheets.Num() == 0)
	{
		ListStatusText->SetText(ReaperLang::T(TEXT("「読み込み」で設定したファイルを表示します。"), TEXT("Press Load to show the configured file.")));
	}
	if (ListSectionLabel)      ListSectionLabel->SetText(ReaperLang::T(TEXT("リスト"), TEXT("List")));
	if (ListFormatRowLabel)    ListFormatRowLabel->SetText(ReaperLang::T(TEXT("形式"), TEXT("Format")));
	if (ListFileRowLabel)      ListFileRowLabel->SetText(ReaperLang::T(TEXT("ファイル"), TEXT("File")));
	if (ListSheetRowLabel)     ListSheetRowLabel->SetText(ReaperLang::T(TEXT("シート(空=全シート)"), TEXT("Sheet (empty=all)")));
	if (ListStartRowRowLabel)  ListStartRowRowLabel->SetText(ReaperLang::T(TEXT("開始行"), TEXT("Start row")));
	if (ListStartColRowLabel)  ListStartColRowLabel->SetText(ReaperLang::T(TEXT("開始列"), TEXT("Start col")));
	if (ListColCountRowLabel)  ListColCountRowLabel->SetText(ReaperLang::T(TEXT("列数"), TEXT("Columns")));
	if (ListCheckboxRowLabel)  ListCheckboxRowLabel->SetText(ReaperLang::T(TEXT("チェックボックス"), TEXT("Checkbox")));
	if (ListExportRowLabel)    ListExportRowLabel->SetText(ReaperLang::T(TEXT("書き出し先"), TEXT("Export to")));
	if (ListGSheetIdRowLabel)         ListGSheetIdRowLabel->SetText(ReaperLang::T(TEXT("GoogleシートURL"), TEXT("Google Sheet URL")));
	if (ListGSheetClientIdRowLabel)   ListGSheetClientIdRowLabel->SetText(ReaperLang::T(TEXT("GoogleSheet クライアントID"), TEXT("GoogleSheet Client ID")));
	UpdateListCheckboxButton();
	if (MicSettingsRowLabel)   MicSettingsRowLabel->SetText(ReaperLang::T(TEXT("マイク登録"), TEXT("Mic Registration")));
	if (MicInputRowLabel)      MicInputRowLabel->SetText(ReaperLang::T(TEXT("マイク入力"), TEXT("Mic Input")));
	if (MonitorOutputRowLabel) MonitorOutputRowLabel->SetText(ReaperLang::T(TEXT("モニター出力"), TEXT("Monitor Output")));
	if (MonitorBufferRowLabel) MonitorBufferRowLabel->SetText(ReaperLang::T(TEXT("受信バッファ"), TEXT("Receive Buffer")));
	if (DownmixSectionLabel)   DownmixSectionLabel->SetText(ReaperLang::T(TEXT("ダウンミックス係数"), TEXT("Downmix Coeff")));
	for (int32 i = 0; i < DmCoeffRowLabels.Num() && i < NumDownmixChannels; ++i)
	{
		const TCHAR* LabelJP = nullptr;
		const TCHAR* LabelEN = nullptr;
		DmChannelLabel(i, LabelJP, LabelEN);
		if (DmCoeffRowLabels[i]) { DmCoeffRowLabels[i]->SetText(ReaperLang::T(LabelJP, LabelEN)); }
	}
	RefreshDmCoeffInputs();
	if (SpatialSectionLabel)   SpatialSectionLabel->SetText(ReaperLang::T(TEXT("立体音響"), TEXT("3D Spatial")));
	if (SpatialItdRowLabel)    SpatialItdRowLabel->SetText(ReaperLang::T(TEXT("立体感 (ITD)"), TEXT("Width (ITD)")));
	if (SpatialShadowRowLabel) SpatialShadowRowLabel->SetText(ReaperLang::T(TEXT("頭部減衰"), TEXT("Head Shadow")));
	if (SpatialOrderRowLabel)  SpatialOrderRowLabel->SetText(ReaperLang::T(TEXT("ch順マッピング"), TEXT("Channel Order")));
	if (MapEditChRowLabel)     MapEditChRowLabel->SetText(ReaperLang::T(TEXT("編集ch"), TEXT("Edit ch")));
	if (MapAngleRowLabel)      MapAngleRowLabel->SetText(ReaperLang::T(TEXT("角度"), TEXT("Angle")));
	if (MapDistRowLabel)       MapDistRowLabel->SetText(ReaperLang::T(TEXT("距離"), TEXT("Distance")));
	if (SpatialHrtfRowLabel)   SpatialHrtfRowLabel->SetText(ReaperLang::T(TEXT("HRTF"), TEXT("HRTF")));
	RefreshHrtfCombo();   // HRTFプロファイルの表示名（例:「内製」/「Custom-made」）を現在の言語に合わせ直す
	if (LicenseSectionLabel)   LicenseSectionLabel->SetText(ReaperLang::T(TEXT("ライセンス"), TEXT("Licenses")));
	UpdateMicInputModeButton();
	UpdateMonitorOutputButton();
	UpdateMonitorBufferButton();
	UpdateSpatialButtons();
	UpdateMappingControls();

	// 接続先デバイス情報バー（Hz/bit/ch表示）。RPCなしでキャッシュ済みの値から組み立て直す。
	RefreshDeviceInfoText();

	// この端末のIP（値のみ。ラベルは LocalIpRowLabel 側）
	if (LocalIPText)
	{
		FString LocalIP = URadnodzUtility::AZ_GetLocalIP();
		if (LocalIP.IsEmpty()) { LocalIP = ReaperLang::S(TEXT("不明"), TEXT("Unknown")); }
		LocalIPText->SetText(FText::FromString(LocalIP));
	}
	UpdateHeaderIpText();

	// 状態に応じた動的表示を再適用
	if (bIsConnected) { ApplyConnectedUI(); } else { ApplyDisconnectedUI(); }
	UpdateMonitorButton();
	UpdateMicSendButton();
	UpdateTransportButtons();
	ApplyRecordingUI();
	UpdateStatus();

	// タブの選択表示（色・インジケータ）を再適用
	SetActiveTab(ActiveTab);

	// ボタンラベルの折り返し：日本語はもともと短くコンパクトで折り返し不要なため無効にし、
	// それ以外の言語は長い訳文がボタンをはみ出さないよう有効にする（MakeButtonでは常時trueにしている）。
	{
		const bool bWrap = !ReaperLang::IsJapanese();
		UTextBlock* ButtonTexts[] = {
			AddTrackButtonText, MicSettingsButtonText, MonitorButtonText, MicSendButtonText,
			ShowDeviceInfoButtonText, CommTimeMeasureButtonText, MeterRecvButtonText, MeterRecvSettingButtonText,
			ListLoadButtonText, ListExportButtonText, RecordingCheckButtonText, ListFormatButtonText,
			ScriptMatchScopeButtonText, ScriptMatchCheckFilterButtonText, ListCheckboxButtonText,
			UpdateButtonText, ScanButtonText, ClearArmedButtonText, DeleteTrackButtonText,
			DeleteSelectAllButtonText, DeleteDeselectAllButtonText, SlackShareButtonText, NormalizeButtonText,
			LockButtonText, ConnectButtonText, PlayButtonText, RecordButtonText,
			MicInputButtonText, MonitorOutputButtonText, MonitorBufferButtonText,
			SpatialItdButtonText, SpatialShadowButtonText, SpatialOrderButtonText, MapEditChButtonText,
			MeterModeButtonText, MeterDotScopeButtonText, TrackRefreshModeButtonText, ThemeButtonText,
			ListTitleText,
		};
		for (UTextBlock* T : ButtonTexts) { if (T) { T->SetAutoWrapText(bWrap); } }
	}
}

void UWG_RadNodzToolkit::HandleUpdateClicked()
{
	if (bTrackListAutoRefresh)
	{
		// 自動更新モード中は「更新」ボタンがトグルスイッチになる（グレー＝停止中／赤＝自動更新中）。
		bTrackListAutoRefreshRunning = !bTrackListAutoRefreshRunning;
		TrackRefreshTimer = 0.0;
		UpdateUpdateButton();
		return;
	}

	// 手動モード：今まで通り、押した時だけ即時更新
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (Card)
		{
			Card->CommitNameEdit();
		}
	}
	FetchTracks();
}

void UWG_RadNodzToolkit::UpdateUpdateButton()
{
	using namespace ReaperCtrlUI;

	if (!UpdateButton) return;

	if (!bTrackListAutoRefresh)
	{
		// 手動モード：トグル表現なし、通常のボタン色に戻す
		UpdateButton->SetBackgroundColor(BtnBlueDark);
		if (UpdateButtonText) { UpdateButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(BtnBlueDark))); }
		return;
	}

	const FLinearColor Bg = bTrackListAutoRefreshRunning ? RecRed : StopGray;   // モニター/マイク/メーターと同じON/OFF配色
	UpdateButton->SetBackgroundColor(Bg);
	if (UpdateButtonText) { UpdateButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg))); }
}

void UWG_RadNodzToolkit::HandlePlayClicked()
{
	if (bIsLocked) return;

	// 録音中にPlayを押した場合は、一度停止して録音を確定（テイク命名）してから再生する。
	// （録音中に直接再生指示を送ると、再生に移らず録音が止まるだけになるため）
	if (bIsRecording)
	{
		StopRecording();
		StartPlaying();
		return;
	}

	// 再生中なら停止、そうでなければ再生（PLAY⇄STOP）
	if (bIsPlaying)
	{
		StopRecording();
	}
	else
	{
		StartPlaying();
	}
}

void UWG_RadNodzToolkit::HandleRecordClicked()
{
	if (bIsLocked) return;

	// 録音中なら停止、そうでなければ録音（REC⇄STOP）
	if (bIsRecording)
	{
		StopRecording();
	}
	else
	{
		StartRecording();
	}
}

void UWG_RadNodzToolkit::HandleStopClicked()
{
	StopRecording();
}

void UWG_RadNodzToolkit::HandleClearArmedClicked()
{
	if (bIsLocked) return;
	ClearAllArmed();
}

void UWG_RadNodzToolkit::HandleLockClicked()
{
	if (bIsLocked)
	{
		// 解除は誤操作防止のため確認ダイアログ経由のみ（ここでは即座に外さない）
		UWG_UnlockConfirmDialog* Dlg = CreateWidget<UWG_UnlockConfirmDialog>(this, UWG_UnlockConfirmDialog::StaticClass());
		if (Dlg)
		{
			Dlg->Setup(this);
			Dlg->AddToViewport(130);
		}
		return;
	}

	// ロックする側は確認不要（即座にロック）
	bIsLocked = true;
	SetDeleteMode(false);   // ロック時は削除選択モードを強制的に抜ける
	UpdateLockButton();
	ApplyLockToControls();
}

void UWG_RadNodzToolkit::ConfirmUnlock()
{
	bIsLocked = false;
	UpdateLockButton();
	ApplyLockToControls();
}

void UWG_RadNodzToolkit::CancelUnlockConfirm()
{
	// ロックしたままにする（no-op）
}

void UWG_RadNodzToolkit::UpdateLockButton()
{
	using namespace ReaperCtrlUI;

	if (!LockButton) return;
	const FLinearColor Bg = bIsLocked ? LockOrange : StopGray;
	LockButton->SetBackgroundColor(Bg);
	if (LockButtonText)
	{
		LockButtonText->SetText(FText::FromString(bIsLocked
			? ReaperLang::S(TEXT("ロック中"), TEXT("Locked"))
			: ReaperLang::S(TEXT("ロック"), TEXT("Lock"))));
		LockButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
	}
	if (LockButtonIcon)
	{
		const TCHAR* IconPath = bIsLocked
			? TEXT("/Game/Texture/T_icon_lock_1.T_icon_lock_1")
			: TEXT("/Game/Texture/T_icon_release_1.T_icon_release_1");
		if (UTexture2D* IconTex = LoadObject<UTexture2D>(nullptr, IconPath))
		{
			LockButtonIcon->SetBrushFromTexture(IconTex);
		}
	}
}

void UWG_RadNodzToolkit::ApplyLockToControls()
{
	const bool bEnabled = !bIsLocked;
	if (PlayButton)              PlayButton->SetIsEnabled(bEnabled);
	if (RecordButton)            RecordButton->SetIsEnabled(bEnabled);
	if (AddTrackButton)          AddTrackButton->SetIsEnabled(bEnabled);
	if (DeleteTrackButton)       DeleteTrackButton->SetIsEnabled(bEnabled);
	if (ClearArmedButton)        ClearArmedButton->SetIsEnabled(bEnabled);
	if (NormalizeButton)         NormalizeButton->SetIsEnabled(bEnabled);
	if (DeleteSelectAllButton)   DeleteSelectAllButton->SetIsEnabled(bEnabled);
	if (DeleteDeselectAllButton) DeleteDeselectAllButton->SetIsEnabled(bEnabled);

	// 各トラックカード（ミュート/ソロ/REC、展開中メディアの再生）にもロック状態を反映する
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (Card) { Card->SetLocked(bIsLocked); }
	}

	// トラック一覧の枠色にも反映（録音中の赤枠と同じ仕組みを共有）。
	ApplyRecordingUI();
}

void UWG_RadNodzToolkit::SetDeleteMode(bool bOn)
{
	using namespace ReaperCtrlUI;
	bDeleteMode = bOn;

	// 全選択/全解除行の表示
	if (DeleteSelectRow)
	{
		DeleteSelectRow->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	// 削除ボタンの表示（通常=「削除」/ 選択中=「決定」赤）
	if (DeleteTrackButton && DeleteTrackButtonText)
	{
		DeleteTrackButtonText->SetText(ReaperLang::T(bOn ? TEXT("決定") : TEXT("削除"), bOn ? TEXT("OK") : TEXT("Delete")));
		const FLinearColor Bg = bOn ? RecRed : StopGray;
		DeleteTrackButton->SetBackgroundColor(Bg);
		DeleteTrackButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
	}
	// 各カードのチェックボックス表示を切り替え（選択はリセットされる）
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (Card) { Card->SetDeleteMode(bOn); }
	}
}

void UWG_RadNodzToolkit::GatherDeleteTargets(TArray<UMediaTrack*>& OutTracks, TArray<FString>& OutNames) const
{
	OutTracks.Reset();
	OutNames.Reset();

	// チェック中トラックのIndex集合
	TSet<int32> Selected;
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (Card && Card->IsSelectedForDelete()) { Selected.Add(Card->TrackIndex); }
	}
	if (Selected.Num() == 0) return;

	// 対象となる TrackList 上の位置(p) を集める。親（フォルダ）なら配下の子孫も含める。
	const int32 N = TrackList.Num();
	TSet<int32> Positions;
	for (int32 p = 0; p < N; ++p)
	{
		if (!Selected.Contains(TrackList[p].Index)) continue;

		Positions.Add(p);

		const int32 ParentLevel = TrackLevels.IsValidIndex(p) ? TrackLevels[p] : 0;
		const bool bIsParent = TrackParentFlags.IsValidIndex(p) && TrackParentFlags[p];
		if (bIsParent)
		{
			for (int32 j = p + 1; j < N; ++j)
			{
				const int32 Lv = TrackLevels.IsValidIndex(j) ? TrackLevels[j] : 0;
				if (Lv <= ParentLevel) break;   // 階層が戻ったら子の範囲は終わり
				Positions.Add(j);
			}
		}
	}

	// TrackList順（表示順）でトラックポインタと名前を返す
	for (int32 p = 0; p < N; ++p)
	{
		if (!Positions.Contains(p)) continue;
		OutTracks.Add(TrackList[p].Track);
		OutNames.Add(!TrackList[p].Name.IsEmpty() ? TrackList[p].Name : FString::Printf(TEXT("Track %d"), TrackList[p].Index + 1));
	}
}

void UWG_RadNodzToolkit::HandleDeleteTrackClicked()
{
	if (bIsLocked) return;

	// 通常時：選択モードへ入る（ボタンは「決定」に変わる）
	if (!bDeleteMode)
	{
		SetDeleteMode(true);
		return;
	}

	// 「決定」：削除対象（親選択時は子孫も含む）を集めて確認ダイアログへ
	TArray<UMediaTrack*> Targets;
	TArray<FString> Names;
	GatherDeleteTargets(Targets, Names);

	// 何も選択していなければキャンセル扱いで選択モードを抜ける
	if (Names.Num() == 0)
	{
		SetDeleteMode(false);
		return;
	}

	// 確認ダイアログ（暗幕＋削除一覧＋はい/いいえ）を表示
	UWG_DeleteConfirmDialog* Dlg = CreateWidget<UWG_DeleteConfirmDialog>(this, UWG_DeleteConfirmDialog::StaticClass());
	if (Dlg)
	{
		Dlg->Setup(this, Names);
		Dlg->AddToViewport(130);
	}
}

void UWG_RadNodzToolkit::HandleDeleteSelectAll()
{
	if (bIsLocked) return;
	for (UWG_TrackCard* Card : TrackCards) { if (Card) { Card->SetSelectedForDelete(true); } }
}

void UWG_RadNodzToolkit::HandleDeleteDeselectAll()
{
	if (bIsLocked) return;
	for (UWG_TrackCard* Card : TrackCards) { if (Card) { Card->SetSelectedForDelete(false); } }
}

void UWG_RadNodzToolkit::CancelDeleteConfirm()
{
	// 「いいえ」：削除せず選択モードのまま（チェックは維持）。特に処理は不要。
}

void UWG_RadNodzToolkit::ConfirmDeleteSelectedTracks()
{
	if (!Client || !bIsConnected) { SetDeleteMode(false); return; }
	UReaProject* Proj = GetProject();
	if (!Proj) { SetDeleteMode(false); return; }

	// 削除対象（チェック中＋親選択時はその子孫）のトラックポインタを集める
	TArray<UMediaTrack*> Targets;
	TArray<FString> Names;
	GatherDeleteTargets(Targets, Names);

	// 各トラックを「ポインタで単独選択 → 現在のIndexを取得 → 削除」する。
	// ・ポインタ選択なので、削除で他トラックのIndexがずれても正しい対象を特定できる
	// ・選択中トラックの現在Indexを毎回REAPERから取り直すので、2回目以降・全選択でも漏れない
	// ・子→親（TrackList逆順）で削除し、フォルダ削除で子ポインタが無効になるのを避ける
	for (int32 i = Targets.Num() - 1; i >= 0; --i)
	{
		UMediaTrack* Track = Targets[i];
		if (!Track) continue;


		int CurIdx = URigdocksSilver_Track::AZ_GetTrackItemIndex(Client, Track);

		if (CurIdx < 0) continue;
		URigdocksSilver_Track::AZ_DeleteTrackIdSelect(Client, Proj, CurIdx, 0);
	}

	// 選択モードを抜けて一覧を取り直す
	SetDeleteMode(false);
	FetchTracks();
}

void UWG_RadNodzToolkit::HandleSlackTokenCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	SlackToken = Text.ToString().TrimStartAndEnd();
	SaveSettings();
}

void UWG_RadNodzToolkit::HandleSlackToUserCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	SlackToUser = Text.ToString().TrimStartAndEnd();
	SaveSettings();
}

void UWG_RadNodzToolkit::HandleSlackShareClicked()
{
	if (!Client || !bIsConnected) return;
	// すでに計測/共有ジョブが走っているなら二重起動しない
	if (bMeasureJobActive) return;

	// Slackの設定が未入力なら、設定画面へ誘導する
	if (SlackToken.IsEmpty() || SlackToUser.IsEmpty())
	{
		SetActiveTab(2);   // 設定タブ（リスト追加でindexが2に）
		if (SlackShareButtonText)
		{
			SlackShareButtonText->SetText(FText::FromString(ReaperLang::S(TEXT("Slack設定が必要"), TEXT("Set Slack first"))));
		}
		return;
	}

	if (TrackList.Num() == 0) return;

	// 全トラックを対象に計測ジョブを開始し、完了後にSlackへ共有する
	TArray<int32> AllTrackIndices;
	AllTrackIndices.Reserve(TrackList.Num());
	for (const FTrackDetail& T : TrackList)
	{
		AllTrackIndices.Add(T.Index);
	}

	bShareToSlackAfterMeasure = true;
	if (SlackShareButtonText)
	{
		SlackShareButtonText->SetText(FText::FromString(ReaperLang::S(TEXT("計測して共有中…"), TEXT("Measuring & sharing…"))));
	}

	BeginMeasureJob(AllTrackIndices);
}

void UWG_RadNodzToolkit::PostLoudnessToSlack()
{
	if (!Client || !bIsConnected) return;
	if (SlackToken.IsEmpty() || SlackToUser.IsEmpty()) return;

	// 秒 → "H:MM:SS" / "M:SS"
	auto FmtDur = [](double Sec) -> FString
	{
		if (Sec < 0.0) Sec = 0.0;
		const int32 Total = FMath::RoundToInt(Sec);
		const int32 H = Total / 3600;
		const int32 M = (Total % 3600) / 60;
		const int32 S = Total % 60;
		return (H > 0) ? FString::Printf(TEXT("%d:%02d:%02d"), H, M, S)
		               : FString::Printf(TEXT("%d:%02d"), M, S);
	};

	// 全メディアをトラック名なしのフラット一覧として収集する
	struct FMediaRow { FString Name; double Lufs = 0.0; bool bHasLufs = false; double Length = 0.0; };
	TArray<FMediaRow> Rows;
	double TotalLen = 0.0;

	// フェーズA: 全トラックの全メディアを平坦に収集する。名前・開始位置・キャッシュ済みLUFSは
	//   ここで確定させ、長さ(Length)取得は後でまとめて1往復にするため call だけを積む。
	//   （従来はメディア1件ごとに AZ_GetMediaItemLength を同期呼び出ししていた＝件数ぶんの往復）
	URadnodzRpcBatch* LenBatch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<URigdocksRpcCall*> LenCalls;   // Rows と同じ並び順（各行の長さ取得call）
	for (const FTrackDetail& T : TrackList)
	{
		TArray<UMediaItem*> Items;
		TArray<FString> Names;
		TArray<double> Starts;
		GetTrackMediaItems(T.Index, Items, Names, Starts);
		for (int32 i = 0; i < Items.Num(); ++i)
		{
			if (!Items[i]) continue;
			FMediaRow R;
			R.Name = Names[i];
			const FString Key = FString::Printf(TEXT("%s@%.3f"), *Names[i], Starts[i]);
			R.bHasLufs = TryGetCachedMomentaryLUFS(Key, R.Lufs);
			// 長さは _Build 時点でメディアのIDが引数に取り込まれるため、Items[i] はここで使えば十分。
			URigdocksRpcCall* LenCall = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemLength_Build(Items[i]);
			LenBatch->Add(LenCall);
			LenCalls.Add(LenCall);
			Rows.Add(R);
		}
	}

	// フェーズB: 全メディアの長さを1往復でまとめて取得し、Rows へ書き戻す。
	if (LenBatch->Num() > 0)
	{
		LenBatch->Execute(Client);
		for (int32 i = 0; i < Rows.Num(); ++i)
		{
			Rows[i].Length = URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemLength_Parse(LenCalls[i]);
			TotalLen += Rows[i].Length;
		}
	}

	FString Body;
	if (Rows.Num() == 0)
	{
		Body = TEXT(":headphones: 収録メディアがありません");
	}
	else
	{
		// 先頭：メディア総数・総時間
		Body += FString::Printf(TEXT(":headphones: メディア %d 件 / 総時間 %s\n"), Rows.Num(), *FmtDur(TotalLen));
		// 本文：トラック名なし。メディアごとに「名前 | ラウドネス | 長さ」
		Body += TEXT("```\n");
		for (const FMediaRow& R : Rows)
		{
			const FString LufsStr = R.bHasLufs ? FString::Printf(TEXT("%.1f LUFS"), R.Lufs) : TEXT("-- LUFS");
			Body += FString::Printf(TEXT("%s | %s | %s\n"), *R.Name, *LufsStr, *FmtDur(R.Length));
		}
		Body += TEXT("```");
	}

	// SlackアプリのDM（メッセージタブ）へ投稿
	FSlackMessage Res = URigdocksSilver_Slack::AZ_Slack_PostDirectMessage(
		Client, SlackToken, SlackToUser, Body, false);

	// 成否をボタン表示に反映（一時的）
	const bool bOk = Res.Error.IsEmpty() && !Res.TimeStamp.IsEmpty();
	if (SlackShareButtonText)
	{
		SlackShareButtonText->SetText(FText::FromString(
			bOk ? ReaperLang::S(TEXT("Slackへ共有しました"), TEXT("Shared to Slack"))
			    : ReaperLang::S(TEXT("共有に失敗"), TEXT("Share failed"))));
	}
}

void UWG_RadNodzToolkit::UpdateStatus()
{
	// 「REC N track」表記は廃止したため、現状ここで更新する表示はない。
	// （呼び出し箇所は多いため関数自体は残す）
}

void UWG_RadNodzToolkit::Connect()
{
	// 接続ボタンを押すたびに世代を進める。進行中の古い試行は結果到着時に破棄される。
	const int32 MyGen = ++ConnectGeneration;
	bIsConnecting = true;

	// 接続中表示（UIはブロックしない）
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(TEXT("● ") + ReaperLang::S(TEXT("接続中…"), TEXT("Connecting…"))));
		StatusText->SetColorAndOpacity(FSlateColor(ReaperCtrlUI::TextDim));
	}
	// 接続ボタンも「接続中…」表示にして、押下中だと分かるようにする（結果到着でApply*UIが戻す）
	if (ConnectButton && ConnectButtonText)
	{
		ConnectButtonText->SetText(ReaperLang::T(TEXT("接続中…"), TEXT("Connecting…")));
		ConnectButton->SetBackgroundColor(ReaperCtrlUI::StopGray);
		ConnectButtonText->SetColorAndOpacity(FSlateColor(ReaperCtrlUI::ReadableTextColor(ReaperCtrlUI::StopGray)));
	}
	// 接続中アイコン＝Reaperロゴを縦軸回転させる
	SetStatusIconTexture(TEXT("/Game/Texture/T_RADNODZ_white_conect_1.T_RADNODZ_white_conect_1"));
	bStatusIconSpin = true;
	StatusIconSpinPhase = 0.f;

	const FString IP = IPAddress;
	const int32 P = Port;

	// この試行専用のクライアントを使う（古い試行のブロッキング接続と同じClientを共有しないため）。
	// バックグラウンド実行中にGC回収されないよう、ルート登録しておく。
	URadnodzClient* AttemptClient = URadnodzClient::CreateRadnodzClient();
	if (!AttemptClient) { bIsConnecting = false; return; }
	AttemptClient->AddToRoot();

	// TLS(mTLS) を使う設定なら、この試行クライアントに鍵・serverId・期待FPを設定する。
	// (ゲームスレッドで実施。identity をロードすると Connecter 側がセキュアモードになる)
	if (bUseTls)
	{
		AttemptClient->SetStorageDirectory(GetAuthStorageDir());
		AttemptClient->GenerateIdentity();
		const FString Sel = GetSelectedServerId();
		const int32 Idx = FindServerAuthIndex(Sel);
		AttemptClient->SetServerId(Sel);
		AttemptClient->SetExpectedServerFingerprint(Idx != INDEX_NONE ? ServerAuthEntries[Idx].Fingerprint : FString());
	}

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	Async(EAsyncExecution::Thread, [WeakThis, AttemptClient, IP, P, MyGen]()
	{
		bool bOk = false;
		FRigdocksError Err;
		if (AttemptClient)
		{
			bOk = AttemptClient->Connect(IP, P);
			if (!bOk) { Err = AttemptClient->GetError(); }
		}

		// 結果はゲームスレッドで反映する（UMG/後続のReaper呼び出しはゲームスレッド限定）
		AsyncTask(ENamedThreads::GameThread, [WeakThis, AttemptClient, bOk, Err, MyGen]()
		{
			if (UWG_RadNodzToolkit* Self = WeakThis.Get())
			{
				Self->HandleConnectResult(MyGen, AttemptClient, bOk, Err);
			}
			else if (IsValid(AttemptClient))
			{
				AttemptClient->RemoveFromRoot();
			}
		});
	});
}

void UWG_RadNodzToolkit::HandleConnectResult(int32 Gen, URadnodzClient* AttemptClient, bool bSuccess, const FRigdocksError& Error)
{
	// 古い世代（接続ボタン再押下で破棄された試行）の結果は無視する
	if (Gen != ConnectGeneration)
	{
		if (IsValid(AttemptClient))
		{
			AttemptClient->Disconnect();
			AttemptClient->RemoveFromRoot();
		}
		return;
	}

	bIsConnecting = false;
	if (IsValid(AttemptClient))
	{
		AttemptClient->RemoveFromRoot();
	}

	if (bSuccess)
	{
		// 旧Clientがあれば切断して、今回のクライアントに入れ替える
		if (Client && Client != AttemptClient)
		{
			Client->Disconnect();
		}
		Client = AttemptClient;
		bIsConnected = true;

		// 接続成功直後の重い処理（アーム解除・トラック一覧取得・カード生成）は
		// 同期通信＋多数のウィジェット生成でゲームスレッドを数秒ブロックし、
		// その間ステータスアイコンの回転が固まる。次フレームに回して、まず
		// 「接続中…」表示と回転を確実に描画してから実行する。
		TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);
		AsyncTask(ENamedThreads::GameThread, [WeakThis]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self || !Self->bIsConnected || !Self->Client) { return; }

			// 接続時に全トラックのREC(アーム)を解除する
			if (UReaProject* Proj = Self->GetProject())
			{
				URigdocksSilver_Track::AZ_SetTrackAllRecording(Self->Client, Proj, false);
			}
			Self->ArmedTrackIndices.Empty();

			Self->FetchTracks();
			Self->ApplyConnectedUI();
			Self->OnConnected();
			Self->ResolveAZDataFolder();   // REAPERリソース配下の AZData を解決して名簿フォルダに使う

			// 接続時にまず1回、Reaperとの通信時間を計測する（以後は NativeTick で30秒ごと）。
			Self->CommTimeProbeTimer = 0.0;
			Self->MeasureCommTime();

			// 接続監視タイマも接続時点から数え直す（以後 NativeTick で10秒ごとに確認）。
			Self->ConnCheckTimer = 0.0;
		});
	}
	else
	{
		// 失敗した試行のクライアントは破棄（現Clientでなければ）
		if (AttemptClient && AttemptClient != Client)
		{
			AttemptClient->Disconnect();
		}
		ApplyConnectFailedUI();
		OnConnectionFailed(Error.Message);
	}
}

void UWG_RadNodzToolkit::Disconnect()
{
	// 進行中の接続試行があれば世代を進めて破棄する（結果到着時に無視される）。
	// これにより「接続中…」に再度押した場合もキャンセル＝切断として扱える。
	++ConnectGeneration;
	bIsConnecting = false;
	bStatusIconSpin = false;

	// 接続がまだ生きているうちに、自分の在席情報を即座にオフライン扱いにしておく
	// （切断後はRPCが使えず自分では消せないため、切断前に済ませる必要がある）。
	MarkPresenceOffline();

	if (Client && bIsConnected)
	{
		Client->Disconnect();
	}
	bIsConnected = false;
	bIsRecording = false;
	bIsPlaying = false;
	bIsPaused = false;
	bIsStopped = true;
	TrackList.Empty();
	ArmedTrackIndices.Empty();
	MomentaryLUFSCache.Empty();   // 切断時にラウドネスキャッシュを破棄
	bLoudnessPrefetchActive = false;
	ClearTrackMediaCache();       // 切断時にメディア一覧キャッシュも破棄
	bMediaWarmActive = false;

	// グループトークの送受信を停止し、ローカル状態をリセットする
	// （マスターchは再接続時に思い出せるよう保持する）。
	if (GroupTalkSender && GroupTalkSender->IsActive()) { GroupTalkSender->Stop(); }
	for (int32 Ch = 0; Ch < 2; ++Ch)
	{
		StopGroupTalkReceiver(Ch);
		bGroupTalkRecvOn[Ch] = false;
		GroupTalkMembers[Ch].Reset();
	}
	bGroupTalkSubHeld = false;
	bGroupTalkMasterLatch = false;
	UpdateGroupTalkUI();

	ApplyDisconnectedUI();
	OnDisconnected();
}

void UWG_RadNodzToolkit::HandleScanClicked()
{
	OpenReaperScan();
}

void UWG_RadNodzToolkit::OpenReaperScan()
{
	UWG_ReaperScanDialog* Dlg = CreateWidget<UWG_ReaperScanDialog>(GetWorld(), UWG_ReaperScanDialog::StaticClass());
	if (Dlg)
	{
		Dlg->Setup(this);
		Dlg->AddToViewport(120);
	}
}

void UWG_RadNodzToolkit::SelectScannedReaper(const FString& InIP)
{
	IPAddress = InIP.TrimStartAndEnd();
	if (IPAddressInput)
	{
		IPAddressInput->SetText(FText::FromString(IPAddress));
	}
	UpdateHeaderIpText();
	SaveSettings();   // 選んだIPを保存

	// ホーム画面に切り替えてから接続する
	SetActiveTab(0);
	Connect();
}

void UWG_RadNodzToolkit::FetchTracks()
{
	if (!Client || !bIsConnected) return;

	UReaProject* Proj = GetProject();
	if (!Proj) return;

	// 「更新」してもラウドネスのキャッシュは保持する（計測済みは再計測しない）。
	// 新規/変更された波形だけが、次の計測時に未計測として扱われ計測される。

	// メディア一覧キャッシュは破棄する（トラックの増減で並びが変わるため）。
	// 直後に背景先読み（StartMediaCacheWarm）で取り直すので、展開時は基本キャッシュ命中になる。
	ClearTrackMediaCache();

	TrackList = URigdocksSilver_Track::AZ_GetTrackListDetail(Client, Proj);

	// フォルダ階層を判定するため、深度(Depth)が無ければ専用APIから補完する。
	// （AZ_GetTrackListDetail がDepthを返さない場合への保険。Indexで突き合わせる）
	{
		bool bHasDepth = false;
		for (const FTrackDetail& T : TrackList)
		{
			if (T.Depth != 0) { bHasDepth = true; break; }
		}

		if (!bHasDepth)
		{
			TArray<FTrackDetail> DepthList = URigdocksSilver_Track::AZ_GetTrackDepthList(Client, Proj);
			if (DepthList.Num() > 0)
			{
				TMap<int32, int32> DepthByIndex;
				for (const FTrackDetail& D : DepthList)
				{
					DepthByIndex.Add(D.Index, D.Depth);
				}
				for (FTrackDetail& T : TrackList)
				{
					if (const int32* Found = DepthByIndex.Find(T.Index))
					{
						T.Depth = *Found;
					}
				}
			}
		}
	}

	// 各トラックのソロ/ミュート/REC(アーム)状態と、再生/録音/一時停止/停止のトランスポート状態を
	// 実機から取得して反映する。カード生成(ApplyTracksUI→InitTrackCard)は
	// IsTrackSoloed/Muted/Armed を参照するので、ここで先に取得しておく必要がある。
	// トラック数×3+4回の往復をRpcBatchで1往復にまとめる。
	{
		ArmedTrackIndices.Reset();
		SoloedTrackIndices.Reset();
		MutedTrackIndices.Reset();

		struct FTrackStateCalls
		{
			int32 Index = 0;
			URigdocksRpcCall* Solo = nullptr;
			URigdocksRpcCall* Mute = nullptr;
			URigdocksRpcCall* Rec  = nullptr;
		};

		URadnodzRpcBatch* StateBatch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
		TArray<FTrackStateCalls> StateCalls;
		StateCalls.Reserve(TrackList.Num());
		for (const FTrackDetail& T : TrackList)
		{
			FTrackStateCalls C;
			C.Index = T.Index;
			C.Solo  = URigdocksSilver_Track_Build::AZ_GetTrackIdSolo_Build(Proj, T.Index);
			C.Mute  = URigdocksSilver_Track_Build::AZ_GetTrackIdMute_Build(Proj, T.Index);
			C.Rec   = URigdocksSilver_Track_Build::AZ_GetTrackIdRecording_Build(Proj, T.Index);
			StateBatch->Add(C.Solo);
			StateBatch->Add(C.Mute);
			StateBatch->Add(C.Rec);
			StateCalls.Add(C);
		}

		// トラックに依存しないプロジェクト単位のトランスポート状態も同じ往復で取得する
		URigdocksRpcCall* PlayCall  = URigdocksSilver_Project_Build::AZ_GetPlayState_Build(Proj);
		URigdocksRpcCall* RecCall   = URigdocksSilver_Project_Build::AZ_GetRecordState_Build(Proj);
		URigdocksRpcCall* PauseCall = URigdocksSilver_Project_Build::AZ_GetPauseState_Build(Proj);
		URigdocksRpcCall* StopCall  = URigdocksSilver_Project_Build::AZ_GetStopState_Build(Proj);
		StateBatch->Add(PlayCall);
		StateBatch->Add(RecCall);
		StateBatch->Add(PauseCall);
		StateBatch->Add(StopCall);

		if (StateBatch->Num() > 0)
		{
			StateBatch->Execute(Client);
			for (const FTrackStateCalls& C : StateCalls)
			{
				if (URigdocksSilver_Track_Parse::AZ_GetTrackIdSolo_Parse(C.Solo))     { SoloedTrackIndices.Add(C.Index); }
				if (URigdocksSilver_Track_Parse::AZ_GetTrackIdMute_Parse(C.Mute))     { MutedTrackIndices.Add(C.Index); }
				if (URigdocksSilver_Track_Parse::AZ_GetTrackIdRecording_Parse(C.Rec)) { ArmedTrackIndices.AddUnique(C.Index); }
			}

			bIsPlaying   = URigdocksSilver_Project_Parse::AZ_GetPlayState_Parse(PlayCall);
			bIsRecording = URigdocksSilver_Project_Parse::AZ_GetRecordState_Parse(RecCall);
			bIsPaused    = URigdocksSilver_Project_Parse::AZ_GetPauseState_Parse(PauseCall);
			bIsStopped   = URigdocksSilver_Project_Parse::AZ_GetStopState_Parse(StopCall);

			ApplyRecordingUI();
			UpdateTransportButtons();
			NotifyTransportToMediaRow();
		}
	}

	ApplyTracksUI(TrackList);
	OnTracksLoaded(TrackList);

	// 接続先デバイス情報（Hz/bit・入力/出力ch・端末名）を取得して表示へ反映する。
	// FetchTracks は接続直後と「更新」ボタンの両方から呼ばれるので、両方でカバーされる。
	UpdateDeviceInfo();

	// メディア一覧を背景で先読みしておく（接続/更新の直後に少しずつ取得）。
	// これにより、各トラックを展開した時の「毎回の重い読み込み」が無くなる。
	StartMediaCacheWarm();

	// ラウドネスは自動計測しない（各トラックの「計測」ボタンを押すまで数値は出さない）。
}

void UWG_RadNodzToolkit::SetTrackArmed(int32 TrackIndex, bool bArm)
{
	if (!Client || !bIsConnected) return;

	UReaProject* Proj = GetProject();
	if (!Proj) return;

	URigdocksSilver_Track::AZ_SetTrackIdRecording(Client, Proj, TrackIndex, bArm);

	if (bArm)
	{
		ArmedTrackIndices.AddUnique(TrackIndex);

		const FTrackDetail* Found = TrackList.FindByPredicate(
			[TrackIndex](const FTrackDetail& T) { return T.Index == TrackIndex; });
		if (Found && Found->Track)
		{
			// 録音入力のチャンネル数に合わせて、トラックのチャンネル数(I_NCHAN)を確保する。
			// （6ch入力なのにトラックが2chだと、出力メーターが2chしか無く 6本目まで動かないため）
			const int32 InCh = GetTrackInputChannelCount(Found->Track);
			const int32 EvenIn = InCh + (InCh % 2);                 // I_NCHANは偶数
			const int32 Nch = URigdocksSilver_Track::AZ_GetTrackItemNumChannels(Client, Found->Track);
			if (EvenIn > Nch)
			{
				URigdocksSilver_Track::AZ_SetTrackItemNumChannels(Client, Found->Track, FMath::Clamp(EvenIn, 2, 8));
			}

			// サラウンド入力(5.1=6ch以上)ならメーターもMultichannelへ、それ以外はStereoへ切り替える。
			// （メーターモードがStereoのままだと、多chトラックでも2chぶんしか振れない）
			{
				AZ_TrackMeterMode CurMode = AZ_TrackMeterMode::StereoPeaks;
				bool bMeterEnabled = true;
				bool bMeterLufsCh12Only = false;
				URigdocksSilver_Track::AZ_GetTrackItemMeterMode(Client, Found->Track, CurMode, bMeterEnabled, bMeterLufsCh12Only);

				const AZ_TrackMeterMode DesiredMode = (InCh >= 6) ? AZ_TrackMeterMode::MultichannelPeaks : AZ_TrackMeterMode::StereoPeaks;
				if (CurMode != DesiredMode)
				{
					URigdocksSilver_Track::AZ_SetTrackItemMeterMode(Client, Found->Track, DesiredMode, bMeterEnabled, bMeterLufsCh12Only);
				}
			}

			// メーター本数・ラベルを現在のモードに合わせて作り直す（入力を変えてからアームした場合に追従）
			const TArray<FString> Labels = GetMeterLabelsForTrack(Found->Track);
			for (UWG_TrackCard* Card : TrackCards)
			{
				if (Card && Card->TrackIndex == TrackIndex)
				{
					Card->BuildMeter(Labels, bMeterSignalIndicator);
					break;
				}
			}

			// ホールドピーク（最大値線）をリセットして、今回のセッション基準にする。
			// チャンネル数ぶんの往復をRpcBatchで1往復にまとめる。
			URadnodzRpcBatch* ResetBatch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
			for (int32 ch = 0; ch < FMath::Clamp(InCh, 1, 8); ++ch)
			{
				ResetBatch->Add(URigdocksSilver_Track_Build::AZ_ResetTrackItemHoldPeak_Build(Found->Track, ch));
			}
			if (ResetBatch->Num() > 0)
			{
				ResetBatch->Execute(Client);
			}
		}
	}
	else
	{
		ArmedTrackIndices.Remove(TrackIndex);
	}

	ApplyArmChangeUI(TrackIndex, bArm);
	OnTrackArmChanged(TrackIndex, bArm);
}

bool UWG_RadNodzToolkit::IsTrackArmed(int32 TrackIndex) const
{
	return ArmedTrackIndices.Contains(TrackIndex);
}

void UWG_RadNodzToolkit::ClearAllArmed()
{
	if (!Client || !bIsConnected) return;

	UReaProject* Proj = GetProject();
	if (!Proj) return;

	URigdocksSilver_Track::AZ_SetTrackAllRecording(Client, Proj, false);

	TArray<int32> PrevArmed = ArmedTrackIndices;
	ArmedTrackIndices.Empty();

	// 各トラックごとの集約UI更新を抑制し、ループ後に1回だけ更新する。
	bDeferAggregateArmUI = true;
	for (int32 Idx : PrevArmed)
	{
		ApplyArmChangeUI(Idx, false);
		OnTrackArmChanged(Idx, false);
	}
	bDeferAggregateArmUI = false;

	UpdateParentArmVisuals();
	UpdateStatus();
}

int32 UWG_RadNodzToolkit::GetArmedTrackCount() const
{
	return ArmedTrackIndices.Num();
}

void UWG_RadNodzToolkit::SetTrackSolo(int32 TrackIndex, bool bSolo)
{
	if (!Client || !bIsConnected) return;

	UReaProject* Proj = GetProject();
	if (!Proj) return;

	URigdocksSilver_Track::AZ_SetTrackIdSolo(Client, Proj, TrackIndex, bSolo);

	if (bSolo)
	{
		SoloedTrackIndices.Add(TrackIndex);
	}
	else
	{
		SoloedTrackIndices.Remove(TrackIndex);
	}

	RefreshAllTrackDim();
}

bool UWG_RadNodzToolkit::IsTrackSoloed(int32 TrackIndex) const
{
	return SoloedTrackIndices.Contains(TrackIndex);
}

void UWG_RadNodzToolkit::SetTrackMute(int32 TrackIndex, bool bMute)
{
	if (!Client || !bIsConnected) return;

	UReaProject* Proj = GetProject();
	if (!Proj) return;

	URigdocksSilver_Track::AZ_SetTrackIdMute(Client, Proj, TrackIndex, bMute);

	if (bMute)
	{
		MutedTrackIndices.Add(TrackIndex);
	}
	else
	{
		MutedTrackIndices.Remove(TrackIndex);
	}

	RefreshAllTrackDim();
}

void UWG_RadNodzToolkit::RefreshAllTrackDim()
{
	const bool bAnySolo = SoloedTrackIndices.Num() > 0;
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (!Card) continue;
		const bool bMuted  = MutedTrackIndices.Contains(Card->TrackIndex);
		const bool bSoloed = SoloedTrackIndices.Contains(Card->TrackIndex);
		// ミュート中、またはソロ中のトラックがあって自分がソロでない → 薄暗く
		const bool bDim = bMuted || (bAnySolo && !bSoloed);
		Card->SetDimmed(bDim);
	}
}

bool UWG_RadNodzToolkit::IsTrackMuted(int32 TrackIndex) const
{
	return MutedTrackIndices.Contains(TrackIndex);
}

void UWG_RadNodzToolkit::GetTrackMediaItems(int32 TrackIndex, TArray<UMediaItem*>& OutItems,
	TArray<FString>& OutNames, TArray<double>& OutStarts)
{
	OutItems.Reset();
	OutNames.Reset();
	OutStarts.Reset();

	if (!Client || !bIsConnected) return;

	UReaProject* Proj = GetProject();
	if (!Proj) return;

	TArray<UMediaItem*> Items = URigdocksBronze_MediaItem::AZ_GetTrackMediaItemList(Client, Proj, TrackIndex);

	// 各メディアの名前/開始位置を、アイテム数×2往復ではなくRpcBatchで1往復にまとめる。
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<URigdocksRpcCall*> NameCalls;
	TArray<URigdocksRpcCall*> StartCalls;
	for (UMediaItem* It : Items)
	{
		if (!It) continue;
		OutItems.Add(It);
		URigdocksRpcCall* NameCall  = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemName_Build(It);
		URigdocksRpcCall* StartCall = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemStartTimeSeconds_Build(It);
		NameCalls.Add(NameCall);
		StartCalls.Add(StartCall);
		Batch->Add(NameCall);
		Batch->Add(StartCall);
	}

	if (Batch->Num() > 0)
	{
		Batch->Execute(Client);
		for (int32 i = 0; i < NameCalls.Num(); ++i)
		{
			OutNames.Add(URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemName_Parse(NameCalls[i]));
			OutStarts.Add(URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemStartTimeSeconds_Parse(StartCalls[i]));
		}
	}
}

void UWG_RadNodzToolkit::GetTrackMediaItemsCached(int32 TrackIndex, TArray<UMediaItem*>& OutItems,
	TArray<FString>& OutNames, TArray<double>& OutStarts)
{
	// キャッシュにあれば即返す（Reaperへの問い合わせをしない）
	if (const FTrackMediaCacheEntry* Entry = TrackMediaCache.Find(TrackIndex))
	{
		OutItems.Reset();
		OutItems.Reserve(Entry->Items.Num());
		for (const TObjectPtr<UMediaItem>& It : Entry->Items) { OutItems.Add(It.Get()); }
		OutNames  = Entry->Names;
		OutStarts = Entry->Starts;
		return;
	}

	// キャッシュミス：Reaperから取得して保存する
	GetTrackMediaItems(TrackIndex, OutItems, OutNames, OutStarts);

	FTrackMediaCacheEntry NewEntry;
	NewEntry.Items.Reserve(OutItems.Num());
	for (UMediaItem* It : OutItems) { NewEntry.Items.Add(It); }
	NewEntry.Names  = OutNames;
	NewEntry.Starts = OutStarts;
	TrackMediaCache.Add(TrackIndex, MoveTemp(NewEntry));
}

int32 UWG_RadNodzToolkit::GetTrackMediaCount(int32 TrackIndex)
{
	// キャッシュがあれば通信せずに件数を返す
	if (const FTrackMediaCacheEntry* Entry = TrackMediaCache.Find(TrackIndex))
	{
		return Entry->Items.Num();
	}

	if (!Client || !bIsConnected) return 0;
	UReaProject* Proj = GetProject();
	if (!Proj) return 0;

	// 一覧取得の1往復のみ（各メディアの名前/開始位置は取得しない＝軽い）
	TArray<UMediaItem*> Items = URigdocksBronze_MediaItem::AZ_GetTrackMediaItemList(Client, Proj, TrackIndex);
	return Items.Num();
}

void UWG_RadNodzToolkit::InvalidateTrackMediaCache(int32 TrackIndex)
{
	TrackMediaCache.Remove(TrackIndex);
}

void UWG_RadNodzToolkit::ClearTrackMediaCache()
{
	TrackMediaCache.Reset();
}

void UWG_RadNodzToolkit::MoveCursorToSeconds(double Seconds)
{
	if (!Client || !bIsConnected) return;

	UReaProject* Proj = GetProject();
	if (!Proj) return;

	// startEndType=0 → プロジェクト先頭からの絶対位置
	URigdocksBronze_Cursor::AZ_SetPlayCursorPosition(Client, Proj, 0, Seconds);
}

void UWG_RadNodzToolkit::RenameMediaItem(UMediaItem* MediaItem, const FString& NewName)
{
	if (!Client || !bIsConnected || !MediaItem) return;

	URigdocksBronze_MediaItem::AZ_SetMediaItemName(Client, MediaItem, NewName);

	// キャッシュ済み一覧に同じアイテムがあれば名前を更新する（再展開時に古い名前を出さない）
	for (TPair<int32, FTrackMediaCacheEntry>& Pair : TrackMediaCache)
	{
		FTrackMediaCacheEntry& Entry = Pair.Value;
		for (int32 i = 0; i < Entry.Items.Num(); ++i)
		{
			if (Entry.Items[i].Get() == MediaItem && Entry.Names.IsValidIndex(i))
			{
				Entry.Names[i] = NewName;
			}
		}
	}
}

void UWG_RadNodzToolkit::SetMediaItemMute(UMediaItem* MediaItem, bool bMute)
{
	if (!Client || !bIsConnected || !MediaItem) return;

	URigdocksBronze_MediaItem::AZ_SetMediaItemMute(Client, MediaItem, bMute);
}

namespace
{
	// 線形振幅 → dBFS
	static double LinearToDb(double V)
	{
		return (V > 1e-7) ? 20.0 * FMath::LogX(10.0, V) : -120.0;
	}
}

void UWG_RadNodzToolkit::GetMediaItemLevels(UMediaItem* MediaItem, double& OutMomentaryLUFS, double& OutPeakDb, double& OutRmsDb)
{
	OutMomentaryLUFS = 0.0;
	OutPeakDb = -120.0;
	OutRmsDb = -120.0;

	if (!Client || !bIsConnected || !MediaItem) return;

	// 最大モーメンタリー ラウドネス（LUFS）
	double Lufs = 0.0, Pos = 0.0;
	URigdocksSilver_Loudness::AZ_GetMediaItemLoudnessMaxMomentary(Client, MediaItem, Lufs, Pos);

	if (Client->GetError().ErrorCode != 0)
	{
		//PrintErrorMsg();
	}

	OutMomentaryLUFS = Lufs;

	// ピーク一覧からピーク値とRMS(近似)を算出
	TArray<FPeak> Peaks = URigdocksSilver_Peak::AZ_GetMediaItemPeakList(Client, MediaItem);
	if (Peaks.Num() > 0)
	{
		double MaxAbs = 0.0;
		double SumSq = 0.0;
		for (const FPeak& P : Peaks)
		{
			MaxAbs = FMath::Max3(MaxAbs, FMath::Abs(P.MaxPeak), FMath::Abs(P.MinPeak));
			SumSq += (P.MaxPeak * P.MaxPeak + P.MinPeak * P.MinPeak) * 0.5;
		}
		const double Rms = FMath::Sqrt(SumSq / (double)Peaks.Num());
		OutPeakDb = LinearToDb(MaxAbs);
		OutRmsDb = LinearToDb(Rms);
	}
}

double UWG_RadNodzToolkit::GetMediaItemPeakDb(UMediaItem* MediaItem)
{
	// 音量フェーダーの影響を受けない、素材そのものの生ピーク値（dBFS）を返す。
	if (!Client || !bIsConnected || !MediaItem) return -120.0;

	TArray<FPeak> Peaks = URigdocksSilver_Peak::AZ_GetMediaItemPeakList(Client, MediaItem);
	if (Peaks.Num() == 0) return -120.0;

	double MaxAbs = 0.0;
	for (const FPeak& P : Peaks)
	{
		MaxAbs = FMath::Max3(MaxAbs, FMath::Abs(P.MaxPeak), FMath::Abs(P.MinPeak));
	}
	return LinearToDb(MaxAbs);
}

double UWG_RadNodzToolkit::GetMediaItemMomentaryLUFS(UMediaItem* MediaItem)
{
	if (!Client || !bIsConnected || !MediaItem) return 0.0;

	double Lufs = 0.0, Pos = 0.0;
	URigdocksSilver_Loudness::AZ_GetMediaItemLoudnessMaxMomentary(Client, MediaItem, Lufs, Pos);

	if (Client->GetError().ErrorCode != 0)
	{
		//PrintErrorMsg();
	}

	return Lufs;
}

double UWG_RadNodzToolkit::GetMediaItemMomentaryLUFSCached(UMediaItem* MediaItem, const FString& CacheKey)
{
	// キャッシュにあれば再計算しない（メディア再展開時の重い再取得を回避）
	if (!CacheKey.IsEmpty())
	{
		if (const double* Found = MomentaryLUFSCache.Find(CacheKey))
		{
			return *Found;
		}
	}

	const double Lufs = GetMediaItemMomentaryLUFS(MediaItem);

	// 有効に取得できた場合のみキャッシュ（未接続時の0などは保存しない）
	if (!CacheKey.IsEmpty() && Client && bIsConnected && MediaItem)
	{
		MomentaryLUFSCache.Add(CacheKey, Lufs);
	}
	return Lufs;
}

bool UWG_RadNodzToolkit::TryGetCachedMomentaryLUFS(const FString& CacheKey, double& OutLufs) const
{
	if (const double* Found = MomentaryLUFSCache.Find(CacheKey))
	{
		OutLufs = *Found;
		return true;
	}
	return false;
}

void UWG_RadNodzToolkit::BeginMeasureJob(const TArray<int32>& TrackIndices, bool bForceRemeasure)
{
	if (!Client || !bIsConnected) return;

	MeasureTrackQueue = TrackIndices;
	MeasureTrackCursor = 0;
	MeasureItemCursor = 0;
	bMeasureListLoaded = false;
	MeasureItems.Reset();
	MeasureNames.Reset();
	MeasureStarts.Reset();
	MeasureTimer = 0.0;
	bMeasureJobActive = true;
	bMeasureForceRemeasure = bForceRemeasure;

	// 計測対象が無い場合は即終了（後続処理は EndMeasureJob 内で実施）
	if (MeasureTrackQueue.Num() == 0)
	{
		EndMeasureJob();
	}
}

void UWG_RadNodzToolkit::TickMeasureJob(float DeltaSeconds)
{
	if (!bMeasureJobActive) return;
	if (!Client || !bIsConnected) { EndMeasureJob(); return; }
	// 録音/再生中は計測を控える（操作と通信の競合回避）
	if (bIsRecording || bIsPlaying) return;

	// 取得間隔（重いReaper呼び出しを分散させ、UIを止めない）
	MeasureTimer += DeltaSeconds;
	const double Interval = 0.05;
	if (MeasureTimer < Interval) return;
	MeasureTimer = 0.0;

	// 全トラックを処理し終えたら終了
	if (!MeasureTrackQueue.IsValidIndex(MeasureTrackCursor))
	{
		EndMeasureJob();
		return;
	}

	// 現トラックのメディア一覧をこの1ステップで取得
	if (!bMeasureListLoaded)
	{
		TArray<UMediaItem*> Items;
		MeasureNames.Reset();
		MeasureStarts.Reset();
		GetTrackMediaItems(MeasureTrackQueue[MeasureTrackCursor], Items, MeasureNames, MeasureStarts);

		MeasureItems.Reset();
		for (UMediaItem* It : Items) { MeasureItems.Add(It); }

		MeasureItemCursor = 0;
		bMeasureListLoaded = true;
		return;
	}

	// 現トラックのメディアを処理し終えたら次トラックへ
	if (!MeasureItems.IsValidIndex(MeasureItemCursor))
	{
		++MeasureTrackCursor;
		bMeasureListLoaded = false;
		return;
	}

	// メディア1件のラウドネスを計測
	UMediaItem* It = MeasureItems[MeasureItemCursor];
	const FString Key = FString::Printf(TEXT("%s@%.3f"),
		*MeasureNames[MeasureItemCursor], MeasureStarts[MeasureItemCursor]);
	if (It)
	{
		if (bMeasureForceRemeasure)
		{
			// 強制再計測：LUFSはアイテム音量の影響を受けるため、キャッシュを信用せず必ず実測し直す
			// （計測調整で「前回から変化なければ何もしない」を正しく判定するために必要）。
			MomentaryLUFSCache.Add(Key, GetMediaItemMomentaryLUFS(It));
		}
		else if (!MomentaryLUFSCache.Contains(Key))
		{
			GetMediaItemMomentaryLUFSCached(It, Key);
		}
	}
	++MeasureItemCursor;
}

void UWG_RadNodzToolkit::EndMeasureJob()
{
	bMeasureJobActive = false;
	MeasureItems.Reset();
	MeasureNames.Reset();
	MeasureStarts.Reset();
	MeasureTrackQueue.Reset();

	// 計測結果を表示中の全カードのメディア行へ反映
	RefreshAllMediaLevels();

	// 「計測＆音量調整」起点だった場合は、計測完了後に音量を目標値へ調整する
	if (bNormalizeAfterMeasure)
	{
		bNormalizeAfterMeasure = false;
		ApplyLoudnessNormalization();
	}

	// 「Slack共有」起点だった場合は、計測完了後にSlackへ投稿する
	if (bShareToSlackAfterMeasure)
	{
		bShareToSlackAfterMeasure = false;
		PostLoudnessToSlack();
	}
}

void UWG_RadNodzToolkit::RefreshAllMediaLevels()
{
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (Card)
		{
			Card->RefreshVisibleMediaLevels();
		}
	}
}

void UWG_RadNodzToolkit::TickTrackMeters(float DeltaSeconds)
{
	if (!Client || !bIsConnected) return;
	if (!bMeterReceiveEnabled) return;        // メーター受信OFF時はポーリングしない（処理を軽くする）
	if (ArmedTrackIndices.Num() == 0) return;

	// 設定の更新間隔でポーリング（各取得はブリッジ往復のため、携帯負荷に応じて調整可能）
	MeterPollTimer += DeltaSeconds;
	if (MeterPollTimer < (double)MeterIntervalMs / 1000.0) return;
	MeterPollTimer = 0.0;

	// カードを1パスで走査する（旧実装は arm 本数 × 全カード/全トラックの線形検索だった）。
	// メーターが実際に見えている（アーム中の葉トラック、かつ折り畳みで隠れていない）カードだけポーリングする。
	// 隠れているカードへの同期API往復はユーザーに見えないため省く＝携帯回線での無駄を削減。
	// 全アーム中カード×全チャンネルのピーク取得を、1ティック1往復のRpcBatchにまとめる。
	// 従来は「カード数 × チャンネル数」ぶんの同期往復が毎ティック発生していた（携帯回線で顕著な無駄）。
	struct FMeterTarget { UWG_TrackCard* Card; int32 FirstCall; int32 NumCh; };
	TArray<FMeterTarget> Targets;
	TArray<URigdocksRpcCall*> PeakCalls;   // バッチ投入順に対応するcall配列（読み戻し用）
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();

	for (UWG_TrackCard* Card : TrackCards)
	{
		if (!Card || !Card->bIsArmed || Card->bIsParent) continue;

		const ESlateVisibility Vis = Card->GetVisibility();
		if (Vis == ESlateVisibility::Collapsed || Vis == ESlateVisibility::Hidden) continue;

		UMediaTrack* Track = Card->GetTrackObj();
		if (!Track) continue;

		const int32 NumCh = FMath::Clamp(Card->GetMeterChannelCount(), 1, 8);
		FMeterTarget T;
		T.Card = Card;
		T.FirstCall = PeakCalls.Num();
		T.NumCh = NumCh;
		for (int32 ch = 0; ch < NumCh; ++ch)
		{
			URigdocksRpcCall* Call = URigdocksSilver_Track_Build::AZ_GetTrackItemCurrentPeakDB_Build(Track, ch);
			PeakCalls.Add(Call);
			Batch->Add(Call);
		}
		Targets.Add(T);
	}

	if (PeakCalls.Num() == 0) return;   // 表示中のアーム済みカードが無ければ通信しない

	Batch->Execute(Client);

	TArray<float> DbPerCh;   // 取得バッファは使い回してアロケーションを抑える
	for (const FMeterTarget& T : Targets)
	{
		DbPerCh.Reset();
		DbPerCh.Reserve(T.NumCh);
		for (int32 ch = 0; ch < T.NumCh; ++ch)
		{
			DbPerCh.Add((float)URigdocksSilver_Track_Parse::AZ_GetTrackItemCurrentPeakDB_Parse(PeakCalls[T.FirstCall + ch]));
		}
		T.Card->SetMeterChannels(DbPerCh);
	}
}

void UWG_RadNodzToolkit::HandleNormalizeClicked()
{
	if (bIsLocked) return;
	if (!Client || !bIsConnected) return;
	if (bMeasureJobActive) return;          // 計測/共有/調整ジョブ実行中は二重起動しない
	if (TrackList.Num() == 0) return;

	// 全トラックを計測し、完了後に各メディアの音量を目標LUFSへ合わせる
	TArray<int32> AllTrackIndices;
	AllTrackIndices.Reserve(TrackList.Num());
	for (const FTrackDetail& T : TrackList)
	{
		AllTrackIndices.Add(T.Index);
	}

	bNormalizeAfterMeasure = true;
	NormalizeSpinPhase = 0.f;
	// 実行中の表示はアイコンの回転のみで示す（文字を変えるとボタンからはみ出すため、ラベルは「計測調整」のまま）

	// 音量調整の要否を正しく判定するため、キャッシュに関わらず必ず実測し直す
	BeginMeasureJob(AllTrackIndices, /*bForceRemeasure=*/true);
}

void UWG_RadNodzToolkit::HandleTargetLufsCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	const FString S = Text.ToString().TrimStartAndEnd();
	if (!S.IsEmpty())
	{
		// -60〜0 LUFS の範囲にクランプ
		TargetLUFS = FMath::Clamp((float)FCString::Atod(*S), -60.f, 0.f);
	}
	if (TargetLufsInput)
	{
		TargetLufsInput->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), TargetLUFS)));
	}
	SaveSettings();
}

void UWG_RadNodzToolkit::HandleMeterIntervalCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	const FString S = Text.ToString().TrimStartAndEnd();
	if (!S.IsEmpty())
	{
		// 50〜2000ms にクランプ
		MeterIntervalMs = FMath::Clamp(FCString::Atoi(*S), 50, 2000);
	}
	if (MeterIntervalInput)
	{
		MeterIntervalInput->SetText(FText::FromString(FString::FromInt(MeterIntervalMs)));
	}
	SaveSettings();
}

void UWG_RadNodzToolkit::HandleMeterModeClicked()
{
	bMeterSignalIndicator = !bMeterSignalIndicator;
	UpdateMeterModeButton();
	RebuildAllTrackMeters();
	SaveSettings();
}

void UWG_RadNodzToolkit::HandleMeterDotScopeClicked()
{
	bMeterDotPerTrack = !bMeterDotPerTrack;
	UpdateMeterDotScopeButton();
	RebuildAllTrackMeters();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateMeterModeButton()
{
	if (MeterModeButtonText)
	{
		MeterModeButtonText->SetText(FText::FromString(bMeterSignalIndicator
			? ReaperLang::S(TEXT("信号〇（軽い）"), TEXT("Signal dot (light)"))
			: ReaperLang::S(TEXT("フルメーター"), TEXT("Full meter"))));
	}
}

void UWG_RadNodzToolkit::HandleLimitPeakClicked()
{
	bLimitPeakOnNormalize = !bLimitPeakOnNormalize;
	UpdateLimitPeakButton();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateLimitPeakButton()
{
	if (LimitPeakButtonText)
	{
		LimitPeakButtonText->SetText(FText::FromString(bLimitPeakOnNormalize
			? ReaperLang::S(TEXT("あり"), TEXT("On"))
			: ReaperLang::S(TEXT("なし"), TEXT("Off"))));
	}
}

void UWG_RadNodzToolkit::HandleShowDeviceInfoClicked()
{
	bShowDeviceNameIP = !bShowDeviceNameIP;
	UpdateShowDeviceInfoButton();
	UpdateHeaderIpText();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateShowDeviceInfoButton()
{
	if (ShowDeviceInfoButtonText)
	{
		ShowDeviceInfoButtonText->SetText(FText::FromString(bShowDeviceNameIP
			? ReaperLang::S(TEXT("ON"), TEXT("ON"))
			: ReaperLang::S(TEXT("OFF"), TEXT("OFF"))));
	}
}

void UWG_RadNodzToolkit::HandleMeasureCommTimeClicked()
{
	MeasureCommTime();
}

void UWG_RadNodzToolkit::MeasureCommTime()
{
	if (!CommTimeText) return;

	if (!Client || !bIsConnected)
	{
		CommTimeText->SetText(FText::FromString(ReaperLang::S(TEXT("未接続"), TEXT("Not connected"))));
		return;
	}

	// 軽量RPC（AZ_GetErrorCode）を1往復させ、TcpClient内の各時刻（μs, Unixエポック）を更新する。
	if (!Client->SendTimingProbe())
	{
		CommTimeText->SetText(FText::FromString(ReaperLang::S(TEXT("応答なし"), TEXT("No response"))));
		return;
	}

	const int64 SendTime         = Client->GetLastSendTime();          // 端末が送信した時刻
	const int64 ServerRecvTime   = Client->GetLastServerRecvTime();    // Reaperがコマンドを受領した時刻
	const int64 ServerReturnTime = Client->GetLastServerReturnTime();  // Reaperが結果を返した時刻
	const int64 RecvTime         = Client->GetLastRecvTime();          // 端末が結果を受け取り終えた時刻

	if (SendTime == 0 || ServerRecvTime == 0 || ServerReturnTime == 0 || RecvTime == 0)
	{
		CommTimeText->SetText(FText::FromString(TEXT("N/A")));
		return;
	}

	// 接続時間 = 「端末→Reaper（コマンド送信）」と「Reaper→端末（結果取得）」の平均。
	const int64  UplinkUs   = ServerRecvTime - SendTime;        // 端末→Reaper
	const int64  DownlinkUs = RecvTime - ServerReturnTime;      // Reaper→端末
	const double AvgMs = (static_cast<double>(UplinkUs) + static_cast<double>(DownlinkUs)) / 2.0 / 1000.0;

	CommTimeText->SetText(FText::FromString(FString::Printf(TEXT("%.3f ms"), AvgMs)));
}

void UWG_RadNodzToolkit::HandleTrackRefreshModeClicked()
{
	bTrackListAutoRefresh = !bTrackListAutoRefresh;
	TrackRefreshTimer = 0.0;
	bTrackListAutoRefreshRunning = false;   // モード切替時は必ず停止状態から始める（誤って自動更新が走り出さないように）
	UpdateTrackRefreshModeButton();
	UpdateUpdateButton();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateTrackRefreshModeButton()
{
	if (TrackRefreshModeButtonText)
	{
		TrackRefreshModeButtonText->SetText(FText::FromString(bTrackListAutoRefresh
			? ReaperLang::S(TEXT("自動更新"), TEXT("Auto"))
			: ReaperLang::S(TEXT("手動（ボタン）"), TEXT("Manual (button)"))));
	}
}

void UWG_RadNodzToolkit::HandleTrackRefreshIntervalCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	const FString S = Text.ToString().TrimStartAndEnd();
	if (!S.IsEmpty())
	{
		// 1〜300秒にクランプ
		TrackRefreshIntervalSec = FMath::Clamp(FCString::Atoi(*S), 1, 300);
	}
	if (TrackRefreshIntervalInput)
	{
		TrackRefreshIntervalInput->SetText(FText::FromString(FString::FromInt(TrackRefreshIntervalSec)));
	}
	TrackRefreshTimer = 0.0;
	SaveSettings();
}

void UWG_RadNodzToolkit::TickTrackListAutoRefresh(float DeltaSeconds)
{
	if (!bTrackListAutoRefresh || !bTrackListAutoRefreshRunning) return;
	if (!Client || !bIsConnected) return;

	TrackRefreshTimer += DeltaSeconds;
	if (TrackRefreshTimer < (double)TrackRefreshIntervalSec) return;
	TrackRefreshTimer = 0.0;

	for (UWG_TrackCard* Card : TrackCards) { if (Card) { Card->CommitNameEdit(); } }
	FetchTracks();
}

void UWG_RadNodzToolkit::UpdateMeterDotScopeButton()
{
	if (MeterDotScopeButtonText)
	{
		MeterDotScopeButtonText->SetText(FText::FromString(bMeterDotPerTrack
			? ReaperLang::S(TEXT("トラックに1つ"), TEXT("One per track"))
			: ReaperLang::S(TEXT("chごと"), TEXT("Per channel"))));
	}
}

TArray<FString> UWG_RadNodzToolkit::GetMeterLabelsForTrack(UMediaTrack* Track)
{
	// 信号〇＋「トラックに1つ」のときは1個だけ（取得も1chに減る）
	if (bMeterSignalIndicator && bMeterDotPerTrack)
	{
		TArray<FString> One;
		One.Add(ReaperLang::S(TEXT("信号"), TEXT("Signal")));
		return One;
	}
	return GetTrackInputChannelLabels(Track);
}

void UWG_RadNodzToolkit::RebuildAllTrackMeters()
{
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (Card)
		{
			Card->SetMeterMode(bMeterSignalIndicator);
		}
	}
}

void UWG_RadNodzToolkit::ApplyLoudnessNormalization()
{
	if (!Client || !bIsConnected) return;

	const double Target = (double)TargetLUFS;
	TArray<int32> Touched;

	for (const FTrackDetail& T : TrackList)
	{
		TArray<UMediaItem*> Items;
		TArray<FString> Names;
		TArray<double> Starts;
		GetTrackMediaItems(T.Index, Items, Names, Starts);
		if (Items.Num() == 0) continue;

		bool bAdjusted = false;
		for (int32 i = 0; i < Items.Num(); ++i)
		{
			if (!Items[i]) continue;
			const FString Key = FString::Printf(TEXT("%s@%.3f"), *Names[i], Starts[i]);

			double Lufs = 0.0;
			if (!TryGetCachedMomentaryLUFS(Key, Lufs)) continue;   // 未計測はスキップ

			// LUFSはアイテム音量（フェーダー）の影響を受ける値。HandleNormalizeClickedは毎回
			// 強制再計測するため、ここでのLufsは「今まさに聞こえている音の実測値」であり、
			// 既に目標に合っていれば差分はほぼ0になる（＝手動変更や前回調整の有無を自然に検知できる）。
			const double LoudnessDeltaDb = Target - Lufs;

			// 既に目標付近なら何もしない（ピーク判定も行わない＝毎回強制的にゲインを削るバグを防ぐ）
			if (FMath::Abs(LoudnessDeltaDb) <= 0.05) continue;

			double DeltaDb = LoudnessDeltaDb;

			// ピーク超過防止：適用後にピークが赤クリップ域(-0.1dBFS)を超える場合は、
			// 超えないところまでゲインを頭打ちにする。
			// 注意：GetMediaItemPeakDb の値は取得経路が未検証で、異常値を返す可能性があるため、
			// ピーク制限による削減幅には安全上限を設け、暴走してもLUFS計算結果から大きくは外れないようにする。
			if (bLimitPeakOnNormalize)
			{
				constexpr double PeakSafeCeilingDb = -0.1;   // WG_TrackCardのクリップ判定(-0.1dBFS)と揃える
				constexpr double MaxPeakCutDb = 12.0;        // ピーク制限で削れる最大幅（異常値からの暴走を防ぐ安全上限）
				const double CurrentVol = GetMediaItemVolume(Items[i]);
				const double RawPeakDb = GetMediaItemPeakDb(Items[i]);
				const double ProjectedPeakDb = RawPeakDb + CurrentVol + DeltaDb;
				if (ProjectedPeakDb > PeakSafeCeilingDb)
				{
					DeltaDb = FMath::Max((PeakSafeCeilingDb - RawPeakDb) - CurrentVol, LoudnessDeltaDb - MaxPeakCutDb);
				}
			}

			if (FMath::Abs(DeltaDb) > 0.05)
			{
				AddMediaItemVolume(Items[i], DeltaDb);
			}

			// 調整後の実際のラウドネスを見積もってキャッシュを更新する（次回の強制再計測で上書きされるが、
			// 強制再計測を伴わない他のLUFS参照箇所のための整合性維持）。
			MomentaryLUFSCache.Add(Key, Lufs + DeltaDb);
			bAdjusted = true;
		}

		if (bAdjusted) { Touched.Add(T.Index); }
	}

	// 展開中のカードはメディア一覧を作り直して、新しい音量・ラウドネス表示を反映する
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (Card && Touched.Contains(Card->TrackIndex))
		{
			Card->RebuildMediaListIfExpanded();
		}
	}

	// 完了後もボタンは「計測調整」表示のまま（LUFS値を出すと文字がボタンをはみ出すため）。
	if (NormalizeButtonText)
	{
		NormalizeButtonText->SetText(ReaperLang::T(TEXT("計測調整"), TEXT("Normalize")));
	}
}

// ====================================================================
//  ラウドネスのバックグラウンド先読み（フレーム分散・UIを止めない）
// ====================================================================
// ====================================================================
//  メディア一覧のバックグラウンド先読み（接続/更新後・フレーム分散）
// ====================================================================
void UWG_RadNodzToolkit::StartMediaCacheWarm()
{
	bMediaWarmActive = true;
	MediaWarmTrackIndex = 0;
	MediaWarmTimer = 0.0;
}

void UWG_RadNodzToolkit::TickMediaCacheWarm(float DeltaSeconds)
{
	if (!bMediaWarmActive) return;
	if (!Client || !bIsConnected) { bMediaWarmActive = false; return; }
	// 録音/再生中は通信を控える（操作と競合させない）
	if (bIsRecording || bIsPlaying) return;

	// 1トラック/間隔ずつ取得して、重い呼び出しを分散（UIを止めない）
	MediaWarmTimer += DeltaSeconds;
	const double Interval = 0.08;
	if (MediaWarmTimer < Interval) return;
	MediaWarmTimer = 0.0;

	// 全トラック走査が終わったら停止
	if (!TrackList.IsValidIndex(MediaWarmTrackIndex))
	{
		bMediaWarmActive = false;
		return;
	}

	const int32 Idx = TrackList[MediaWarmTrackIndex].Index;
	// まだキャッシュに無いトラックだけ取得（展開済みでロード済みのものは触らない）
	if (!TrackMediaCache.Contains(Idx))
	{
		TArray<UMediaItem*> Items;
		TArray<FString>     Names;
		TArray<double>      Starts;
		GetTrackMediaItemsCached(Idx, Items, Names, Starts);   // 取得＆キャッシュ
	}
	++MediaWarmTrackIndex;
}

void UWG_RadNodzToolkit::StartLoudnessPrefetch()
{
	// 先頭トラックから一巡してキャッシュを埋める
	bLoudnessPrefetchActive = true;
	PrefetchTrackIndex = 0;
	PrefetchItemIndex = 0;
	bPrefetchListLoaded = false;
	PrefetchTimer = 0.0;
	PrefetchItems.Reset();
	PrefetchNames.Reset();
	PrefetchStarts.Reset();
}

void UWG_RadNodzToolkit::TickLoudnessPrefetch(float DeltaSeconds)
{
	if (!bLoudnessPrefetchActive) return;
	if (!Client || !bIsConnected) { bLoudnessPrefetchActive = false; return; }
	// 録音/再生中は取得を控える（操作と通信が競合しないように）
	if (bIsRecording || bIsPlaying) return;

	// 取得間隔（1ステップ/約0.1秒）。重いReaper呼び出しを分散させ、UIを止めない。
	PrefetchTimer += DeltaSeconds;
	const double Interval = 0.10;
	if (PrefetchTimer < Interval) return;
	PrefetchTimer = 0.0;

	// 全トラック走査が終わったら停止
	if (!TrackList.IsValidIndex(PrefetchTrackIndex))
	{
		bLoudnessPrefetchActive = false;
		PrefetchItems.Reset();
		PrefetchNames.Reset();
		PrefetchStarts.Reset();
		return;
	}

	// 現トラックのメディア一覧を未取得なら、この1ステップで取得（重い呼び出しは1回だけ）
	if (!bPrefetchListLoaded)
	{
		TArray<UMediaItem*> Items;
		PrefetchNames.Reset();
		PrefetchStarts.Reset();
		GetTrackMediaItems(TrackList[PrefetchTrackIndex].Index, Items, PrefetchNames, PrefetchStarts);

		// GC回収されないよう UPROPERTY 配列(TObjectPtr)へ保持
		PrefetchItems.Reset();
		for (UMediaItem* It : Items) { PrefetchItems.Add(It); }

		PrefetchItemIndex = 0;
		bPrefetchListLoaded = true;
		return; // 次フレームへ
	}

	// 現トラックのメディアを処理し終えたら次トラックへ
	if (!PrefetchItems.IsValidIndex(PrefetchItemIndex))
	{
		++PrefetchTrackIndex;
		bPrefetchListLoaded = false;
		return;
	}

	// メディア1件のラウドネスをキャッシュ（未取得のもののみ実取得）
	UMediaItem* It = PrefetchItems[PrefetchItemIndex];
	const FString Key = FString::Printf(TEXT("%s@%.3f"),
		*PrefetchNames[PrefetchItemIndex], PrefetchStarts[PrefetchItemIndex]);
	if (It && !MomentaryLUFSCache.Contains(Key))
	{
		GetMediaItemMomentaryLUFSCached(It, Key);
	}
	++PrefetchItemIndex;
}

double UWG_RadNodzToolkit::GetTrackVolume(UMediaTrack* Track)
{
	if (!Client || !bIsConnected || !Track) return 0.0;
	return URigdocksSilver_Track::AZ_GetTrackItemVolume(Client, Track);
}

int32 UWG_RadNodzToolkit::GetTrackNumChannels(UMediaTrack* Track)
{
	if (!Client || !bIsConnected || !Track) return 1;
	const int32 N = URigdocksSilver_Track::AZ_GetTrackItemNumChannels(Client, Track);
	return FMath::Clamp(N, 1, 8);
}

int32 UWG_RadNodzToolkit::GetTrackInputChannelCount(UMediaTrack* Track)
{
	if (!Client || !bIsConnected || !Track) return 1;
	int32 FirstCh = 0;
	int32 NumCh = 0;
	AZ_InputChannelType Type = AZ_InputChannelType::Audio;
	URigdocksSilver_Track::AZ_GetTrackItemInputChannel(Client, Track, FirstCh, NumCh, Type);
	if (NumCh <= 0)
	{
		// 入力が取れない場合はトラックのチャンネル数で代替
		NumCh = URigdocksSilver_Track::AZ_GetTrackItemNumChannels(Client, Track);
	}
	return FMath::Clamp(NumCh, 1, 8);
}

TArray<FString> UWG_RadNodzToolkit::GetTrackInputChannelLabels(UMediaTrack* Track)
{
	TArray<FString> Out;
	if (!Client || !bIsConnected || !Track)
	{
		Out.Add(TEXT("1"));
		return Out;
	}

	int32 FirstCh = 0;
	int32 NumCh = 0;
	AZ_InputChannelType Type = AZ_InputChannelType::Audio;
	URigdocksSilver_Track::AZ_GetTrackItemInputChannel(Client, Track, FirstCh, NumCh, Type);
	if (NumCh <= 0)
	{
		NumCh = URigdocksSilver_Track::AZ_GetTrackItemNumChannels(Client, Track);
	}
	NumCh = FMath::Clamp(NumCh, 1, 8);

	// 全入力チャンネル名の一覧。トラックの入力は FirstCh から NumCh 本。
	const TArray<FString> AllNames = URigdocksSilver_Device::AZ_GetInputChannelList(Client);
	for (int32 ch = 0; ch < NumCh; ++ch)
	{
		const int32 GlobalIdx = FirstCh + ch;
		if (AllNames.IsValidIndex(GlobalIdx) && !AllNames[GlobalIdx].IsEmpty())
		{
			Out.Add(AllNames[GlobalIdx]);
		}
		else
		{
			Out.Add(FString::Printf(TEXT("in %d"), GlobalIdx + 1));   // 名前が取れない場合のフォールバック
		}
	}
	return Out;
}

double UWG_RadNodzToolkit::AddTrackVolume(UMediaTrack* Track, double AddAmount)
{
	if (!Client || !bIsConnected || !Track) return 0.0;
	URigdocksSilver_Track::AZ_SetTrackItemAddVolume(Client, Track, AddAmount);
	return URigdocksSilver_Track::AZ_GetTrackItemVolume(Client, Track);
}

double UWG_RadNodzToolkit::GetMediaItemVolume(UMediaItem* MediaItem)
{
	if (!Client || !bIsConnected || !MediaItem) return 0.0;
	return URigdocksBronze_MediaItem::AZ_GetMediaItemVolume(Client, MediaItem);
}

double UWG_RadNodzToolkit::AddMediaItemVolume(UMediaItem* MediaItem, double AddAmount)
{
	if (!Client || !bIsConnected || !MediaItem) return 0.0;
	URigdocksBronze_MediaItem::AZ_SetMediaItemAddVolume(Client, MediaItem, AddAmount);
	return URigdocksBronze_MediaItem::AZ_GetMediaItemVolume(Client, MediaItem);
}

void UWG_RadNodzToolkit::SetActiveMediaRow(UWG_MediaItemRow* Row)
{
	// 旧アクティブ行のハイライトを解除
	if (ActiveMediaRow.IsValid() && ActiveMediaRow.Get() != Row)
	{
		ActiveMediaRow->SetActive(false);
		ActiveMediaRow->SetPlaying(false);
	}
	ActiveMediaRow = Row;
	if (Row)
	{
		Row->SetActive(true);
	}
}

void UWG_RadNodzToolkit::NotifyTransportToMediaRow()
{
	if (ActiveMediaRow.IsValid())
	{
		ActiveMediaRow->SetPlaying(bIsPlaying);
	}
}

void UWG_RadNodzToolkit::PlayFromMediaRow(UWG_MediaItemRow* Row, UMediaItem* MediaItem, double StartSeconds)
{
	if (!Client || !bIsConnected) return;

	// 既に再生/録音中なら一度停止する。
	// （再生中に再生を再発行してもReaperはシークしないため、必ず止めてから新位置で再生する）
	if (bIsPlaying || bIsRecording)
	{
		URigdocksBronze_Cursor::AZ_StopPlaying(Client);
		bIsPlaying = false;
		bIsRecording = false;
	}

	// この行をアクティブ（カーソル位置）にしてハイライト
	SetActiveMediaRow(Row);

	// メディア先頭へカーソル移動して即再生
	MoveCursorToSeconds(StartSeconds);
	StartPlaying();   // bIsPlaying=true → UpdateTransportButtons / NotifyTransportToMediaRow
}

void UWG_RadNodzToolkit::StopMedia()
{
	StopRecording();  // 停止 → bIsPlaying=false → 各表示が追従
}

void UWG_RadNodzToolkit::AddTrack(const FString& TrackName, bool bArmAfterAdd)
{
	if (!Client || !bIsConnected) return;

	UReaProject* Proj = GetProject();
	if (!Proj) return;

	// 末尾（トラック数の位置）に追加
	int32 InsertIndex = TrackList.Num();
	UMediaTrack* NewTrack = URigdocksSilver_Track::AZ_InsertTrackId(Client, Proj, InsertIndex, TrackName);
	if (!NewTrack) return;

	// リストを再取得してUIを更新
	FetchTracks();

	// 追加されたトラックを特定してイベントを発行
	if (TrackList.Num() > 0)
	{
		const FTrackDetail& Added = TrackList.Last();
		OnTrackAdded(Added);

		if (bArmAfterAdd)
		{
			SetTrackArmed(Added.Index, true);
		}
	}
}

bool UWG_RadNodzToolkit::AddTrackUnique(const FString& TrackName, bool bArmAfterAdd)
{
	if (!Client || !bIsConnected) return false;

	UReaProject* Proj = GetProject();
	if (!Proj) return false;

	int32 OutTrackId = -1;
	UMediaTrack* NewTrack = URigdocksSilver_Track::AZ_InsertTrackSearchUniqueOnly(Client, Proj, TrackName, OutTrackId);

	// OutTrackId == -1 のとき既存トラックが返るので追加なし
	if (!NewTrack || OutTrackId < 0) return false;

	FetchTracks();

	// 追加されたトラックを名前で特定
	for (const FTrackDetail& Detail : TrackList)
	{
		if (Detail.Name == TrackName && Detail.Index == OutTrackId)
		{
			OnTrackAdded(Detail);
			if (bArmAfterAdd)
			{
				SetTrackArmed(Detail.Index, true);
			}
			break;
		}
	}
	return true;
}

void UWG_RadNodzToolkit::AddTracksForMaterial(const FString& Material, const FString& ParentTrackName)
{
	if (!Client || !bIsConnected) return;

	UReaProject* Proj = GetProject();
	if (!Proj) return;

	const FString Mat = Material.TrimStartAndEnd();

	// 登録済みマイクのうち名前が空でないものを採用（入力ch設定も保持）
	TArray<FMicSetting> UseMics;
	for (const FMicSetting& M : Mics)
	{
		FMicSetting T = M;
		T.Name = M.Name.TrimStartAndEnd();
		if (!T.Name.IsEmpty()) { UseMics.Add(T); }
	}
	if (UseMics.Num() == 0) return;

	// 同名マイクの総数を数える（連番要否の判定用）
	TMap<FString, int32> Totals;
	for (const FMicSetting& M : UseMics) { Totals.FindOrAdd(M.Name)++; }

	// 子トラック名（収録素材名_マイク名[_連番]）と、対応するマイク設定を作る
	TArray<FString>     ChildNames;
	TArray<FMicSetting> ChildMics;
	TMap<FString, int32> Running;
	for (const FMicSetting& M : UseMics)
	{
		FString Name = Mat.IsEmpty() ? M.Name : (Mat + TEXT("_") + M.Name);
		if (Totals[M.Name] > 1)
		{
			int32& Idx = Running.FindOrAdd(M.Name);
			++Idx;
			Name += FString::Printf(TEXT("_%d"), Idx);
		}
		ChildNames.Add(Name);
		ChildMics.Add(M);
	}

	// 追加ダイアログで指定された親トラック（"Master"/空/不在なら nullptr＝最上位に追加）
	UMediaTrack* ConfiguredParent = ResolveParentTrackByName(ParentTrackName);

	// 収録素材名が空ならフォルダを作らず追加（各トラックへ入力ch設定を適用）
	if (Mat.IsEmpty())
	{
		// 全子トラックの生成＋設定を1往復のバッチにまとめる（従来はトラック数×数回の往復）。
		// 生成トラックのポインタは、同一バッチ内の生成呼び出しの戻り値(retval 0)を
		// BindRef(argIndex=0) で後続の設定呼び出しへ引き渡す。
		URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
		for (int32 i = 0; i < ChildNames.Num(); ++i)
		{
			const int32 NumCh = FMath::Max(1, ChildMics[i].NumChannels);

			// 親指定があればその配下に、無ければ最上位（末尾）に追加
			int32 InsertIdx = INDEX_NONE;
			if (ConfiguredParent)
			{
				// 既存の親は実ポインタをそのまま渡す（BindRef不要）
				InsertIdx = Batch->Add(URigdocksSilver_Track_Build::AZ_InsertChildTrack_Build(ConfiguredParent, ChildNames[i]));
			}
			else
			{
				InsertIdx = Batch->Add(URigdocksSilver_Track_Build::AZ_InsertTrackId_Build(Proj, TrackList.Num() + i, ChildNames[i]));
				// 末尾追加時にREAPERが直前の開フォルダ配下へ入れてしまうため、最上位(階層0)に強制する
				URigdocksRpcCall* DepthCall = URigdocksSilver_Track_Build::AZ_SetTrackItemDepth_Build(nullptr, 0);
				DepthCall->BindRef(0, InsertIdx, 0);
				Batch->Add(DepthCall);
			}

			// 入力ch設定（生成した新トラックへ適用）
			URigdocksRpcCall* InputCall = URigdocksSilver_Track_Build::AZ_SetTrackItemInputChannel_Build(
				nullptr, FMath::Max(0, ChildMics[i].FirstChannel), NumCh, AZ_InputChannelType::Audio);
			InputCall->BindRef(0, InsertIdx, 0);
			Batch->Add(InputCall);

			// サラウンド（5.1=6ch以上）はトラックを6ch化して6ch録音できるようにする
			if (NumCh >= 6)
			{
				URigdocksRpcCall* NchCall = URigdocksSilver_Track_Build::AZ_SetTrackItemNumChannels_Build(nullptr, NumCh + (NumCh % 2));
				NchCall->BindRef(0, InsertIdx, 0);
				Batch->Add(NchCall);

				// メーターもMultichannelにしないと6ch分振れないため、chに合わせて切り替える
				URigdocksRpcCall* MeterCall = URigdocksSilver_Track_Build::AZ_SetTrackItemMeterMode_Build(
					nullptr, AZ_TrackMeterMode::MultichannelPeaks, /*enabled=*/true, /*lufsChannels1And2Only=*/false);
				MeterCall->BindRef(0, InsertIdx, 0);
				Batch->Add(MeterCall);
			}
		}
		if (Batch->Num() > 0) { Batch->Execute(Client); }
		FetchTracks();
		return;
	}

	// 親トラック（フォルダ）＝ 収録素材名 を作成する。
	// 設定で親トラックが指定されていればその配下に、無ければ最上位（末尾）に作る。
	// 親生成・子生成・各種設定をすべて1往復のバッチにまとめる（従来はトラック数に比例した往復）。
	// 生成トラックのポインタは BindRef(argIndex=0) で「同一バッチ内の生成呼び出しの戻り値(retval 0)」
	// を参照させ、親→子の依存もバッチ内で解決する。
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();

	int32 ParentIdx = INDEX_NONE;
	if (ConfiguredParent)
	{
		ParentIdx = Batch->Add(URigdocksSilver_Track_Build::AZ_InsertChildTrack_Build(ConfiguredParent, Mat));
	}
	else
	{
		ParentIdx = Batch->Add(URigdocksSilver_Track_Build::AZ_InsertTrackId_Build(Proj, TrackList.Num(), Mat));
		// 末尾追加時にREAPERが直前の開フォルダ配下へ入れてしまうため、子を追加する前に最上位(階層0)へ強制する
		URigdocksRpcCall* DepthCall = URigdocksSilver_Track_Build::AZ_SetTrackItemDepth_Build(nullptr, 0);
		DepthCall->BindRef(0, ParentIdx, 0);
		Batch->Add(DepthCall);
	}

	// サラウンド（5.1）の子がいれば、親フォルダのチャンネル数をそれに合わせる
	int32 ParentSurroundCh = 0;

	// その親フォルダの子として 収録素材名_マイク名 を追加し、各子に入力ch設定を適用
	for (int32 i = 0; i < ChildNames.Num(); ++i)
	{
		const int32 NumCh = FMath::Max(1, ChildMics[i].NumChannels);

		// 親は同一バッチ内で生成したトラック（retval 0）を参照する
		URigdocksRpcCall* ChildInsert = URigdocksSilver_Track_Build::AZ_InsertChildTrack_Build(nullptr, ChildNames[i]);
		ChildInsert->BindRef(0, ParentIdx, 0);
		const int32 ChildIdx = Batch->Add(ChildInsert);

		// 入力ch設定（録音入力の割り当て）
		URigdocksRpcCall* InputCall = URigdocksSilver_Track_Build::AZ_SetTrackItemInputChannel_Build(
			nullptr, FMath::Max(0, ChildMics[i].FirstChannel), NumCh, AZ_InputChannelType::Audio);
		InputCall->BindRef(0, ChildIdx, 0);
		Batch->Add(InputCall);

		// サラウンド（5.1=6ch以上）の子は、トラックを6ch化し、親フォルダへ親送り(ParentSend)で接続する。
		// センドは追加せず、親送りを 1〜Nch（firstChIndex=0, numChs=N）で有効化する＝「51で繋ぐ」。
		if (NumCh >= 6)
		{
			const int32 EvenCh = NumCh + (NumCh % 2);   // チャンネル数は偶数にそろえる
			URigdocksRpcCall* NchCall = URigdocksSilver_Track_Build::AZ_SetTrackItemNumChannels_Build(nullptr, EvenCh);
			NchCall->BindRef(0, ChildIdx, 0);
			Batch->Add(NchCall);

			URigdocksRpcCall* SendCall = URigdocksSilver_Track_Build::AZ_SetTrackItemMainSend_Build(
				nullptr, /*sendParent=*/true, /*firstChIndex=*/0, /*numChs=*/EvenCh);
			SendCall->BindRef(0, ChildIdx, 0);
			Batch->Add(SendCall);

			// メーターもMultichannelにしないと6ch分振れないため、chに合わせて切り替える
			URigdocksRpcCall* MeterCall = URigdocksSilver_Track_Build::AZ_SetTrackItemMeterMode_Build(
				nullptr, AZ_TrackMeterMode::MultichannelPeaks, /*enabled=*/true, /*lufsChannels1And2Only=*/false);
			MeterCall->BindRef(0, ChildIdx, 0);
			Batch->Add(MeterCall);

			ParentSurroundCh = FMath::Max(ParentSurroundCh, EvenCh);
		}
	}

	// サラウンドの子がいれば、親フォルダも 5.1(6ch) 以上にして受けられるようにする（＝親トラックを51にする）
	if (ParentSurroundCh >= 6)
	{
		URigdocksRpcCall* ParentNch = URigdocksSilver_Track_Build::AZ_SetTrackItemNumChannels_Build(nullptr, ParentSurroundCh);
		ParentNch->BindRef(0, ParentIdx, 0);
		Batch->Add(ParentNch);

		// 親フォルダも子からのサラウンド送りを受けるので、メーターをMultichannelへ切り替える
		URigdocksRpcCall* ParentMeterCall = URigdocksSilver_Track_Build::AZ_SetTrackItemMeterMode_Build(
			nullptr, AZ_TrackMeterMode::MultichannelPeaks, /*enabled=*/true, /*lufsChannels1And2Only=*/false);
		ParentMeterCall->BindRef(0, ParentIdx, 0);
		Batch->Add(ParentMeterCall);
	}

	if (Batch->Num() > 0) { Batch->Execute(Client); }

	// 一覧を再取得してUI（階層表示）を更新
	FetchTracks();
}

void UWG_RadNodzToolkit::SaveMics(const TArray<FMicSetting>& InMics)
{
	// 名前が空白のみの項目を除外して保持（ch設定はクランプ）
	Mics.Reset();
	for (const FMicSetting& In : InMics)
	{
		FMicSetting M = In;
		M.Name = In.Name.TrimStartAndEnd();
		if (!M.Name.IsEmpty())
		{
			M.FirstChannel = FMath::Max(0, M.FirstChannel);
			M.NumChannels  = FMath::Max(1, M.NumChannels);
			Mics.Add(M);
		}
	}

	// 接続設定を壊さないよう、既存のセーブデータに上書きして保存
	if (UReaperSaveGame* Save = LoadOrCreateSave())
	{
		Save->Mics = Mics;
		Save->MicNames.Reset();   // 旧フォーマットは使わない
		UGameplayStatics::SaveGameToSlot(Save, TEXT("ReaperConnSettings"), 0);
	}
}

TArray<FString> UWG_RadNodzToolkit::GetInputChannelNames()
{
	if (Client && bIsConnected)
	{
		TArray<FString> Names = URigdocksSilver_Device::AZ_GetInputChannelList(Client);
		UE_LOG(LogTemp, Warning, TEXT("[ReaperInput] GetInputChannelNames -> %d 件"), Names.Num());
		return Names;
	}
	UE_LOG(LogTemp, Warning, TEXT("[ReaperInput] GetInputChannelNames -> 未接続"));
	return TArray<FString>();
}

void UWG_RadNodzToolkit::SetTrackInputChannel(UMediaTrack* Track, int32 FirstChannel, int32 NumChannels)
{
	if (!Client || !bIsConnected || !Track) return;
	URigdocksSilver_Track::AZ_SetTrackItemInputChannel(Client, Track,
		FMath::Max(0, FirstChannel), FMath::Max(1, NumChannels), AZ_InputChannelType::Audio);
}

void UWG_RadNodzToolkit::GetTrackInputChannel(UMediaTrack* Track, int32& OutFirstChannel, int32& OutNumChannels)
{
	OutFirstChannel = 0;
	OutNumChannels = 1;
	if (!Client || !bIsConnected || !Track) return;
	AZ_InputChannelType Type = AZ_InputChannelType::Audio;
	URigdocksSilver_Track::AZ_GetTrackItemInputChannel(Client, Track, OutFirstChannel, OutNumChannels, Type);
}

void UWG_RadNodzToolkit::RenameTrack(int32 TrackIndex, const FString& NewName)
{
	if (!Client || !bIsConnected) return;

	UReaProject* Proj = GetProject();
	if (!Proj) return;

	URigdocksSilver_Track::AZ_SetTrackIdName(Client, Proj, TrackIndex, NewName);

	// ローカルのTrackListも更新（一覧の再構築はしない＝編集中のフォーカスを保つ）
	for (FTrackDetail& T : TrackList)
	{
		if (T.Index == TrackIndex)
		{
			T.Name = NewName;
			break;
		}
	}
}

void UWG_RadNodzToolkit::StartRecording()
{
	if (!Client || !bIsConnected || bIsRecording) return;
	if (ArmedTrackIndices.Num() == 0) return;

	// 録音前の各アームトラックのメディア数取得＋録音開始位置の移動＋録音開始を、
	// すべて1往復の RpcBatch にまとめる（従来は「トラック数＋2」回の同期通信で、
	// REC押下時にゲームスレッドが数秒ブロックしメーター/信号表示が遅れていた）。
	// バッチは投入順に実行されるため、
	//   ① 各トラックのメディア数取得（＝録音前の状態を先に確定）
	//   ② 録音開始位置の移動（startEndType=1: 最終波形地点からの相対、setPos=1.0秒）
	//   ③ 録音開始
	// の順で積むことで、件数取得が録音開始より前に確定することが保証される。
	// 録音後、ここより後ろに増えたメディアを「トラック名_連番」に命名するために件数を控える。
	UReaProject* Proj = GetProject();

	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();

	// ① メディア数取得。キャッシュにあれば通信せずその件数を使う（軽量＝名前/開始位置は取らない）。
	PreRecordMediaCounts.Reset();
	TArray<int32>             CountTrackIdx;   // バッチに積んだトラックの Index
	TArray<URigdocksRpcCall*> CountCalls;      // 対応する一覧取得 call（件数の読み戻し用）
	for (int32 Idx : ArmedTrackIndices)
	{
		if (const FTrackMediaCacheEntry* Entry = TrackMediaCache.Find(Idx))
		{
			PreRecordMediaCounts.Add(Idx, Entry->Items.Num());   // キャッシュ命中：通信不要
		}
		else if (Proj)
		{
			URigdocksRpcCall* ListCall = URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaItemList_Build(Proj, Idx);
			CountTrackIdx.Add(Idx);
			CountCalls.Add(ListCall);
			Batch->Add(ListCall);
		}
		else
		{
			PreRecordMediaCounts.Add(Idx, 0);
		}
	}

	// ② 録音開始位置の移動
	if (Proj)
	{
		Batch->Add(URigdocksBronze_Cursor_Build::AZ_SetPlayCursorPosition_Build(Proj, 1, 1.0));
	}

	// ③ 録音開始
	Batch->Add(URigdocksBronze_Cursor_Build::AZ_StartRecording_Build());

	Batch->Execute(Client);

	// バッチで取得した一覧の件数を録音前カウントへ反映する。
	for (int32 i = 0; i < CountCalls.Num(); ++i)
	{
		const TArray<UMediaItem*> Items = URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaItemList_Parse(CountCalls[i]);
		PreRecordMediaCounts.Add(CountTrackIdx[i], Items.Num());
	}

	bIsRecording = true;
	bIsPlaying = false;
	bIsPaused = false;
	bIsStopped = false;
	// 録音時はメディア再生のハイライトを解除
	SetActiveMediaRow(nullptr);
	ApplyRecordingUI();
	UpdateTransportButtons();
	NotifyTransportToMediaRow();
	OnRecordingStarted();
}

void UWG_RadNodzToolkit::StartPlaying()
{
	if (!Client || !bIsConnected) return;

	URigdocksBronze_Cursor::AZ_StartPlaying(Client);
	bIsRecording = false;
	bIsPlaying = true;
	bIsPaused = false;
	bIsStopped = false;

	SetTransportState(ReaperLang::S(TEXT("▶ 再生中"), TEXT("▶ Playing")), ReaperCtrlUI::OkGreen);
	if (RecStateDot) { RecStateDot->SetVisibility(ESlateVisibility::Collapsed); }  // 再生中は赤丸を隠す
	UpdateTransportButtons();
	NotifyTransportToMediaRow();
}

void UWG_RadNodzToolkit::StopRecording()
{
	if (!Client || !bIsConnected) return;

	const bool bWasRecording = bIsRecording;

	URigdocksBronze_Cursor::AZ_StopPlaying(Client);
	bIsRecording = false;
	bIsPlaying = false;
	bIsPaused = false;
	bIsStopped = true;
	ApplyRecordingUI();
	UpdateTransportButtons();
	// 停止：アクティブ行は▶に戻す（ハイライトはカーソル位置として維持）
	NotifyTransportToMediaRow();

	// 録音だった場合のみ、新しく録音されたメディアを「トラック名_連番」に命名する。
	if (bWasRecording)
	{
		RenameRecordedMedia();
	}

	OnRecordingStopped();
}

void UWG_RadNodzToolkit::RenameRecordedMedia()
{
	if (!Client || !bIsConnected) return;

	// 命名・再描画したトラック（FTrackDetail.Index）を控える
	TArray<int32> TouchedTracks;

	for (const TPair<int32, int32>& Pair : PreRecordMediaCounts)
	{
		const int32 TrackIndex = Pair.Key;
		const int32 PrevCount  = Pair.Value;

		// トラック名を取得
		const FTrackDetail* Found = TrackList.FindByPredicate(
			[TrackIndex](const FTrackDetail& T) { return T.Index == TrackIndex; });
		const FString TrackName = Found ? Found->Name : FString();
		if (TrackName.IsEmpty()) continue;

		// 録音後のメディア一覧を取得
		TArray<UMediaItem*> Items;
		TArray<FString> Names;
		TArray<double> Starts;
		GetTrackMediaItems(TrackIndex, Items, Names, Starts);

		// 既存メディアの中から「トラック名_<数字>」形式の最大連番を検出する。
		// 位置ではなく名前で判定するので、並び替え・欠番・他の名前が混在していても
		// 既存の連番の続きから採番できる。
		const FString Prefix = TrackName + TEXT("_");
		auto IsAllDigits = [](const FString& S) -> bool
		{
			if (S.IsEmpty()) return false;
			for (const TCHAR Ch : S) { if (!FChar::IsDigit(Ch)) return false; }
			return true;
		};
		int32 MaxSeq = 0;
		for (int32 i = 0; i < Items.Num() && i < PrevCount; ++i)
		{
			// 今回録音された新規メディア（PrevCount以降）は採番対象なので除外
			const FString& Nm = Names[i];
			if (!Nm.StartsWith(Prefix, ESearchCase::CaseSensitive)) continue;
			const FString Suffix = Nm.RightChop(Prefix.Len());
			if (!IsAllDigits(Suffix)) continue;
			MaxSeq = FMath::Max(MaxSeq, FCString::Atoi(*Suffix));
		}

		// 検出した最大連番の続きから、新規録音メディアを「トラック名_連番」(2桁ゼロ埋め)で命名する。
		int32 NextSeq = MaxSeq + 1;
		for (int32 i = PrevCount; i < Items.Num(); ++i)
		{
			if (!Items[i]) continue;
			const FString NewName = FString::Printf(TEXT("%s%02d"), *Prefix, NextSeq);
			RenameMediaItem(Items[i], NewName);
			++NextSeq;
		}

		if (Items.Num() > PrevCount)
		{
			TouchedTracks.Add(TrackIndex);
			// 録音で内容が変わったので、このトラックのメディアキャッシュは破棄して取り直させる
			InvalidateTrackMediaCache(TrackIndex);
		}
	}

	PreRecordMediaCounts.Reset();

	// 展開中のトラックカードがあればメディア一覧を作り直して新しい名前を反映する
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (Card && TouchedTracks.Contains(Card->TrackIndex))
		{
			Card->RebuildMediaListIfExpanded();
		}
	}
}

void UWG_RadNodzToolkit::UpdateTransportButtons()
{
	using namespace ReaperCtrlUI;

	// PLAYボタン：再生中は STOP（灰）/ それ以外は PLAY（緑）
	if (PlayButton && PlayButtonText)
	{
		PlayButtonText->SetText(FText::FromString(bIsPlaying
			? ReaperLang::S(TEXT("■ 停止"), TEXT("■ STOP"))
			: ReaperLang::S(TEXT("▶ 再生"), TEXT("▶ PLAY"))));
		const FLinearColor Bg = bIsPlaying ? StopGray : AccentGreen;
		PlayButton->SetBackgroundColor(Bg);
		PlayButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
	}

	// RECボタン：録音中は STOP（灰）/ それ以外は REC（赤＋マイクアイコン）
	if (RecordButton && RecordButtonText)
	{
		RecordButtonText->SetText(FText::FromString(bIsRecording
			? ReaperLang::S(TEXT("■ 停止"), TEXT("■ STOP"))
			: ReaperLang::S(TEXT("録音"), TEXT("REC"))));
		const FLinearColor Bg = bIsRecording ? StopGray : RecRed;
		RecordButton->SetBackgroundColor(Bg);
		RecordButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
		// アイコンは REC 表示のときだけ出す
		if (RecordButtonIcon)
		{
			RecordButtonIcon->SetVisibility(bIsRecording ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
	}
}

void UWG_RadNodzToolkit::NativeDestruct()
{
	// 音声モニターを停止（UDPソケット・再生を解放）
	if (AudioMonitor)
	{
		AudioMonitor->Stop();
	}
	// マイク送信を停止
	if (MicSender)
	{
		MicSender->Stop();
	}
	// グループトークの送受信を停止
	if (GroupTalkSender)
	{
		GroupTalkSender->Stop();
	}
	for (int32 Ch = 0; Ch < 2; ++Ch)
	{
		if (GroupTalkReceiver[Ch]) { GroupTalkReceiver[Ch]->Stop(); }
	}

	// 非同期接続の実行中はClientに触れない（バックグラウンドのConnectと競合させない）
	if (Client && bIsConnected && !bIsConnecting)
	{
		MarkPresenceOffline();
		Client->Disconnect();
	}
	Super::NativeDestruct();
}

UReaProject* UWG_RadNodzToolkit::GetProject() const
{
	return UReaProject::GetCurrentProject();
}

// ====================================================================
//  BPイベント（拡張用フック。UI更新はApply...UI()で確実に行うため空でよい）
// ====================================================================
void UWG_RadNodzToolkit::OnConnected_Implementation() {}
void UWG_RadNodzToolkit::OnConnectionFailed_Implementation(const FString& ErrorMessage) {}
void UWG_RadNodzToolkit::OnDisconnected_Implementation() {}
void UWG_RadNodzToolkit::OnTracksLoaded_Implementation(const TArray<FTrackDetail>& Tracks) {}
void UWG_RadNodzToolkit::OnTrackAdded_Implementation(const FTrackDetail& NewTrack) {}
void UWG_RadNodzToolkit::OnRecordingStarted_Implementation() {}
void UWG_RadNodzToolkit::OnRecordingStopped_Implementation() {}
void UWG_RadNodzToolkit::OnTrackArmChanged_Implementation(int32 TrackIndex, bool bArmed) {}

// ====================================================================
//  UI更新（BPのオーバーライドに左右されず必ず実行される）
// ====================================================================
void UWG_RadNodzToolkit::ApplyConnectedUI()
{
	using namespace ReaperCtrlUI;
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(TEXT("● ") + ReaperLang::S(TEXT("接続中"), TEXT("Connected"))));
		StatusText->SetColorAndOpacity(FSlateColor(OkGreen));
	}
	// 接続成功：回転停止＋Reaperロゴ
	bStatusIconSpin = false;
	if (StatusIcon) { StatusIcon->SetRenderScale(FVector2D(1.f, 1.f)); }
	SetStatusIconTexture(TEXT("/Game/Texture/T_RADNODZ_white_conect_1.T_RADNODZ_white_conect_1"));
	// 接続中はボタンを「切断」に
	if (ConnectButton && ConnectButtonText)
	{
		ConnectButtonText->SetText(ReaperLang::T(TEXT("切断"), TEXT("Disconnect")));
		ConnectButton->SetBackgroundColor(StopGray);
		ConnectButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(StopGray)));
	}
	UpdateStatus();
}

void UWG_RadNodzToolkit::ApplyConnectFailedUI()
{
	using namespace ReaperCtrlUI;
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(TEXT("● ") + ReaperLang::S(TEXT("接続失敗"), TEXT("Connection failed"))));
		StatusText->SetColorAndOpacity(FSlateColor(WarnRed));
	}
	// 接続失敗：回転停止＋失敗ロゴ
	bStatusIconSpin = false;
	if (StatusIcon) { StatusIcon->SetRenderScale(FVector2D(1.f, 1.f)); }
	SetStatusIconTexture(TEXT("/Game/Texture/T_RADNODZ_white_nonConect.T_RADNODZ_white_nonConect"));
	if (ConnectButton && ConnectButtonText)
	{
		ConnectButtonText->SetText(ReaperLang::T(TEXT("接続"), TEXT("Connect")));
		ConnectButton->SetBackgroundColor(BtnBlueDark);
		ConnectButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(BtnBlueDark)));
	}
}

void UWG_RadNodzToolkit::ApplyDisconnectedUI()
{
	using namespace ReaperCtrlUI;
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(TEXT("● ") + ReaperLang::S(TEXT("未接続"), TEXT("Not connected"))));
		StatusText->SetColorAndOpacity(FSlateColor(WarnRed));
	}
	// 未接続：回転停止＋未接続ロゴ
	bStatusIconSpin = false;
	if (StatusIcon) { StatusIcon->SetRenderScale(FVector2D(1.f, 1.f)); }
	SetStatusIconTexture(TEXT("/Game/Texture/T_RADNODZ_white_nonConect_2.T_RADNODZ_white_nonConect_2"));
	// 未接続はボタンを「接続」に
	if (ConnectButton && ConnectButtonText)
	{
		ConnectButtonText->SetText(ReaperLang::T(TEXT("接続"), TEXT("Connect")));
		ConnectButton->SetBackgroundColor(BtnBlueDark);
		ConnectButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(BtnBlueDark)));
	}
	// デバイス情報バーを「-」表示に戻し、ヘッダーの相手端末名も消す。
	UpdateDeviceInfo();   // 未接続なのでキャッシュがクリアされ、UpdateHeaderIpText も呼ばれる
	if (TrackListScrollBox)
	{
		TrackListScrollBox->ClearChildren();
	}
	TrackCards.Reset();
	TrackLevels.Reset();
	TrackParentFlags.Reset();
	CollapsedTrackIndices.Reset();
	SoloedTrackIndices.Reset();
	MutedTrackIndices.Reset();
	UpdateStatus();
}

void UWG_RadNodzToolkit::ComputeHierarchy(const TArray<FTrackDetail>& Tracks,
	TArray<int32>& OutLevels, TArray<bool>& OutIsParent) const
{
	const int32 N = Tracks.Num();
	OutLevels.SetNum(N);
	OutIsParent.SetNum(N);

	// FTrackDetail.Depth は各トラックの「累積フォルダ階層レベル」（0=ルート, 1, 2, ...）。
	// レベルをそのままインデントに使う。
	for (int32 i = 0; i < N; ++i)
	{
		OutLevels[i] = FMath::Max(0, Tracks[i].Depth);
	}

	// 直後のトラックが自分より深い階層なら、自分は子を持つ親（フォルダ）。
	for (int32 i = 0; i < N; ++i)
	{
		OutIsParent[i] = (i + 1 < N) && (OutLevels[i + 1] > OutLevels[i]);
	}
}

void UWG_RadNodzToolkit::ApplyTracksUI(const TArray<FTrackDetail>& Tracks)
{
	if (!TrackListScrollBox) return;

	TrackListScrollBox->ClearChildren();
	TrackCards.Reset();

	// 階層レベル・親フラグを算出（カードと同順で保持）
	ComputeHierarchy(Tracks, TrackLevels, TrackParentFlags);

	// 既に存在しない親の畳み込み状態は破棄しておく
	{
		TSet<int32> StillValid;
		for (int32 i = 0; i < Tracks.Num(); ++i)
		{
			if (TrackParentFlags[i])
			{
				StillValid.Add(Tracks[i].Index);
			}
		}
		CollapsedTrackIndices = CollapsedTrackIndices.Intersect(StillValid);
	}

	// 既に存在しないトラックのメディア展開状態は破棄しておく（自動更新でカードが作り直されても、
	// 展開していたトラックは展開したまま復元するため）
	{
		TSet<int32> StillValidTracks;
		for (int32 i = 0; i < Tracks.Num(); ++i)
		{
			StillValidTracks.Add(Tracks[i].Index);
		}
		ExpandedTrackIndices = ExpandedTrackIndices.Intersect(StillValidTracks);
	}

	TSubclassOf<UWG_TrackCard> CardClass = TrackCardClass;
	if (!CardClass)
	{
		CardClass = UWG_TrackCard::StaticClass();
	}

	for (int32 i = 0; i < Tracks.Num(); ++i)
	{
		const FTrackDetail& T = Tracks[i];

		UWG_TrackCard* Card = CreateWidget<UWG_TrackCard>(this, CardClass);
		if (!Card) continue;

		Card->InitTrackCard(T, this);
		Card->SetLocked(bIsLocked);   // ロック中に一覧が作り直された場合も、新しいカードにロック状態を反映する

		const bool bCollapsed = CollapsedTrackIndices.Contains(T.Index);
		Card->SetHierarchy(TrackLevels[i], TrackParentFlags[i], bCollapsed);

		if (ExpandedTrackIndices.Contains(T.Index))
		{
			Card->SetMediaExpanded(true);   // 自動更新前に展開していたトラックは展開したまま復元する
		}

		if (UScrollBoxSlot* CardSlot = Cast<UScrollBoxSlot>(TrackListScrollBox->AddChild(Card)))
		{
			CardSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));
			CardSlot->SetHorizontalAlignment(HAlign_Fill);
		}
		TrackCards.Add(Card);
	}

	RefreshTrackVisibility();
	UpdateParentArmVisuals();
	RefreshAllTrackDim();
	UpdateStatus();
}

void UWG_RadNodzToolkit::RefreshTrackVisibility()
{
	// 親トラックをスタックで辿り、畳み込み中の親の配下にあるカードを非表示にする。
	TArray<int32> OpenParents; // TrackCards上のインデックス

	for (int32 i = 0; i < TrackCards.Num(); ++i)
	{
		const int32 Level = TrackLevels.IsValidIndex(i) ? TrackLevels[i] : 0;

		// 現在のレベル以上の親は閉じた（このトラックを含まない）ので取り除く
		while (OpenParents.Num() > 0 && TrackLevels[OpenParents.Last()] >= Level)
		{
			OpenParents.Pop();
		}

		// 祖先のいずれかが畳み込み中なら非表示
		bool bVisible = true;
		for (int32 ParentPos : OpenParents)
		{
			UWG_TrackCard* ParentCard = TrackCards[ParentPos];
			if (ParentCard && CollapsedTrackIndices.Contains(ParentCard->TrackIndex))
			{
				bVisible = false;
				break;
			}
		}

		if (UWG_TrackCard* Card = TrackCards[i])
		{
			Card->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}

		// このトラックが親なら、以降の子のためにスタックへ積む
		if (TrackParentFlags.IsValidIndex(i) && TrackParentFlags[i])
		{
			OpenParents.Add(i);
		}
	}
}

void UWG_RadNodzToolkit::ToggleCollapse(int32 TrackIndex)
{
	if (CollapsedTrackIndices.Contains(TrackIndex))
	{
		CollapsedTrackIndices.Remove(TrackIndex);
	}
	else
	{
		CollapsedTrackIndices.Add(TrackIndex);
	}

	// 該当する親カードの▽/▷表示を更新
	const bool bNowCollapsed = CollapsedTrackIndices.Contains(TrackIndex);
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (Card && Card->TrackIndex == TrackIndex)
		{
			Card->SetCollapsed(bNowCollapsed);
			break;
		}
	}

	RefreshTrackVisibility();
}

void UWG_RadNodzToolkit::SetTrackMediaExpanded(int32 TrackIndex, bool bExpanded)
{
	if (bExpanded)
	{
		ExpandedTrackIndices.Add(TrackIndex);
	}
	else
	{
		ExpandedTrackIndices.Remove(TrackIndex);
	}
}

void UWG_RadNodzToolkit::GetChildLeafTrackIndices(int32 ParentPos, TArray<int32>& OutLeafTrackIndices) const
{
	OutLeafTrackIndices.Reset();
	if (!TrackLevels.IsValidIndex(ParentPos)) return;

	const int32 ParentLevel = TrackLevels[ParentPos];

	// 親の直後から、親より深い階層が続く範囲が配下。さらに深い親(孫フォルダ)自身は除き、葉だけ集める。
	for (int32 i = ParentPos + 1; i < TrackCards.Num(); ++i)
	{
		if (!TrackLevels.IsValidIndex(i) || TrackLevels[i] <= ParentLevel)
		{
			break; // 親の配下が終わった
		}
		const bool bIsParent = TrackParentFlags.IsValidIndex(i) && TrackParentFlags[i];
		if (!bIsParent && TrackCards[i])
		{
			OutLeafTrackIndices.Add(TrackCards[i]->TrackIndex);
		}
	}
}

void UWG_RadNodzToolkit::ToggleChildrenArmed(int32 ParentTrackIndex)
{
	// 親カードの位置を特定
	int32 ParentPos = INDEX_NONE;
	for (int32 i = 0; i < TrackCards.Num(); ++i)
	{
		if (TrackCards[i] && TrackCards[i]->TrackIndex == ParentTrackIndex)
		{
			ParentPos = i;
			break;
		}
	}
	if (ParentPos == INDEX_NONE) return;

	TArray<int32> Leaves;
	GetChildLeafTrackIndices(ParentPos, Leaves);
	if (Leaves.Num() == 0) return;

	// 子が全てREC中なら全解除、そうでなければ全ON（トグル）
	bool bAllArmed = true;
	for (int32 Idx : Leaves)
	{
		if (!ArmedTrackIndices.Contains(Idx)) { bAllArmed = false; break; }
	}
	const bool bTarget = !bAllArmed;

	// ループ中は各トラックごとの集約UI更新を抑制し、最後に1回だけ更新する。
	// （トラック数が多いと UpdateParentArmVisuals が毎回 O(親×全トラック) 走り、
	//   全体で O(トラック数^2) になって反映が遅くなるのを防ぐ）
	bDeferAggregateArmUI = true;
	for (int32 Idx : Leaves)
	{
		SetTrackArmed(Idx, bTarget);
	}
	bDeferAggregateArmUI = false;

	UpdateParentArmVisuals();
	UpdateStatus();
}

void UWG_RadNodzToolkit::UpdateParentArmVisuals()
{
	// 各親フォルダカードについて、配下の葉が全REC中かを判定して点灯表示を更新する
	for (int32 i = 0; i < TrackCards.Num(); ++i)
	{
		if (!(TrackParentFlags.IsValidIndex(i) && TrackParentFlags[i])) continue;

		TArray<int32> Leaves;
		GetChildLeafTrackIndices(i, Leaves);

		bool bAll = Leaves.Num() > 0;
		for (int32 Idx : Leaves)
		{
			if (!ArmedTrackIndices.Contains(Idx)) { bAll = false; break; }
		}

		if (TrackCards[i])
		{
			TrackCards[i]->SetParentChildrenArmed(bAll);
		}
	}
}

void UWG_RadNodzToolkit::ApplyArmChangeUI(int32 InTrackIndex, bool bArmed)
{
	for (UWG_TrackCard* Card : TrackCards)
	{
		if (Card && Card->TrackIndex == InTrackIndex)
		{
			Card->RefreshArmState(bArmed);
		}
	}

	// 一括ARM中は集約UI更新を抑制する。呼び出し側がループ後にまとめて1回だけ呼ぶ。
	if (bDeferAggregateArmUI)
	{
		return;
	}

	// 子のREC状態が変わったら、親フォルダの点灯表示も追従させる
	UpdateParentArmVisuals();
	UpdateStatus();
}

void UWG_RadNodzToolkit::ApplyRecordingUI()
{
	using namespace ReaperCtrlUI;

	if (bIsRecording)
	{
		// 「●」テキストの代わりに赤丸インジケータを表示する
		SetTransportState(ReaperLang::S(TEXT("録音中"), TEXT("Recording")), RecRed);
		if (RecStateDot) { RecStateDot->SetVisibility(ESlateVisibility::Visible); }
	}
	else if (bIsPaused)
	{
		SetTransportState(ReaperLang::S(TEXT("一時停止中"), TEXT("Paused")), LockOrange);
		if (RecStateDot) { RecStateDot->SetVisibility(ESlateVisibility::Collapsed); }
	}
	else if (bIsPlaying)
	{
		SetTransportState(ReaperLang::S(TEXT("▶ 再生中"), TEXT("▶ Playing")), OkGreen);
		if (RecStateDot) { RecStateDot->SetVisibility(ESlateVisibility::Collapsed); }
	}
	else
	{
		SetTransportState(ReaperLang::S(TEXT("■ 停止中"), TEXT("■ Stopped")), TextDim);
		if (RecStateDot) { RecStateDot->SetVisibility(ESlateVisibility::Collapsed); }
	}

	// 録音中はトラック一覧全体を太い赤枠で囲み、録音中だと一目で分かるようにする。
	// ロック中（非録音時）は同じ枠演出を橙色で行い、操作不可であることを一目で示す。
	if (TrackListPanel)
	{
		const float Radius = 8.f;
		if (bIsRecording)
		{
			const FLinearColor RecFrame(0.95f, 0.16f, 0.20f, 1.f);   // はっきりした赤
			TrackListPanel->SetBrush(FSlateRoundedBoxBrush(ListBg, Radius, RecFrame, 5.f));
		}
		else if (bIsLocked)
		{
			TrackListPanel->SetBrush(FSlateRoundedBoxBrush(ListBg, Radius, LockOrange, 5.f));
		}
		else
		{
			TrackListPanel->SetBrush(FSlateRoundedBoxBrush(ListBg, Radius, FLinearColor::Transparent, 0.f));
		}
	}
}

void UWG_RadNodzToolkit::SetTransportState(const FString& Label, const FLinearColor& Color)
{
	if (TransportStateText)
	{
		TransportStateText->SetText(FText::FromString(Label));
		TransportStateText->SetColorAndOpacity(FSlateColor(Color));
	}
}

// ====================================================================
//  接続設定(IP/Port)・マイク名の保存・読み込み
// ====================================================================
UReaperSaveGame* UWG_RadNodzToolkit::LoadOrCreateSave() const
{
	UReaperSaveGame* Save = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(TEXT("ReaperConnSettings"), 0))
	{
		Save = Cast<UReaperSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ReaperConnSettings"), 0));
	}
	if (!Save)
	{
		Save = Cast<UReaperSaveGame>(UGameplayStatics::CreateSaveGameObject(UReaperSaveGame::StaticClass()));
	}
	return Save;
}

void UWG_RadNodzToolkit::SaveSettings()
{
	UReaperSaveGame* Save = LoadOrCreateSave();
	if (!Save) return;

	Save->IPAddress = IPAddress;
	Save->Port = Port;
	Save->Mics = Mics;           // マイク設定（名前＋入力ch）は維持して保存
	Save->MicInputMode = MicInputMode;
	Save->MonitorOutputMode = MonitorOutputMode;
	Save->bShowDeviceNameIP = bShowDeviceNameIP;
	Save->Language = static_cast<uint8>(ReaperLang::GetLanguage());
	Save->SpatialItdLevel = SpatialItdLevel;
	Save->SpatialShadowLevel = SpatialShadowLevel;
	Save->SpatialChannelOrder = SpatialChannelOrder;
	Save->HrtfProfile = HrtfProfile;
	Save->SpatialCustomAzimuths = CustomAzimuths;
	Save->SpatialCustomDistances = CustomDistances;
	Save->MonitorBufferLevel = MonitorBufferLevel;
	EnsureDmGainsDefaults();
	Save->DmGains = DmGains;
	Save->SlackToken = SlackToken;
	Save->SlackToUser = SlackToUser;
	Save->TargetLUFS = TargetLUFS;
	Save->bLimitPeakOnNormalize = bLimitPeakOnNormalize;
	Save->MeterIntervalMs = MeterIntervalMs;
	Save->bMeterSignalIndicator = bMeterSignalIndicator;
	Save->bMeterDotPerTrack = bMeterDotPerTrack;
	Save->bTrackListAutoRefresh = bTrackListAutoRefresh;
	Save->TrackRefreshIntervalSec = TrackRefreshIntervalSec;
	Save->UIThemeIndex = UIThemeIndex;
	Save->ScreenOrientationLock = ScreenOrientationLock;
	Save->ListFormat = ListFormat;
	Save->ListFilePath = ListFilePath;
	Save->ListSheetName = ListSheetName;
	Save->ListStartRow = ListStartRow;
	Save->ListStartCol = ListStartCol;
	Save->ListColCount = ListColCount;
	Save->bListUseCheckbox = bListUseCheckbox;
	Save->ListExportPath = ListExportPath;
	Save->ScriptMatchScope = ScriptMatchScope;
	Save->ScriptMatchCheckFilter = ScriptMatchCheckFilter;
	Save->GoogleSheetSpreadsheetId = GoogleSheetSpreadsheetId;
	Save->GoogleSheetClientId = GoogleSheetClientId;
	Save->IntercomUserName = IntercomUserName;
	Save->PresenceFolder = PresenceFolder;
	Save->ResolvedPresenceFolder = ResolvedPresenceFolder;
	Save->bUseTls = bUseTls;
	Save->ServerAuthEntries = ServerAuthEntries;
	UGameplayStatics::SaveGameToSlot(Save, TEXT("ReaperConnSettings"), 0);
}

void UWG_RadNodzToolkit::LoadSettings()
{
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("ReaperConnSettings"), 0))
	{
		return;
	}

	UReaperSaveGame* Save = Cast<UReaperSaveGame>(
		UGameplayStatics::LoadGameFromSlot(TEXT("ReaperConnSettings"), 0));
	if (!Save) return;

	IPAddress = Save->IPAddress;
	Port = Save->Port;
	Mics = Save->Mics;
	if (Mics.Num() == 0 && Save->MicNames.Num() > 0)
	{
		// 旧フォーマット（名前のみ）から移行：入力chは既定（開始0/1ch）
		for (const FString& N : Save->MicNames)
		{
			FMicSetting M;
			M.Name = N;
			M.FirstChannel = 0;
			M.NumChannels = 1;
			Mics.Add(M);
		}
	}
	MicInputMode = Save->MicInputMode;
	MonitorOutputMode = Save->MonitorOutputMode;
	bShowDeviceNameIP = Save->bShowDeviceNameIP;
	bUseTls = Save->bUseTls;
	UpdateUseTlsButton();
	ServerAuthEntries = Save->ServerAuthEntries;
	RefreshServerIdCombo();
	SpatialItdLevel = Save->SpatialItdLevel;
	SpatialShadowLevel = Save->SpatialShadowLevel;
	SpatialChannelOrder = EBinauralChannelOrder::Custom;   // 保存値に関わらずカスタム固定
	HrtfProfile = Save->HrtfProfile;
	if (Save->SpatialCustomAzimuths.Num() >= MaxMappingChannels) { CustomAzimuths = Save->SpatialCustomAzimuths; }
	if (Save->SpatialCustomDistances.Num() >= MaxMappingChannels) { CustomDistances = Save->SpatialCustomDistances; }
	MonitorBufferLevel = Save->MonitorBufferLevel;
	EnsureDmGainsDefaults();
	if (Save->DmGains.Num() == NumDownmixChannels)
	{
		for (int32 i = 0; i < NumDownmixChannels; ++i)
		{
			DmGains[i] = FMath::Clamp(Save->DmGains[i], 0.0f, 1.5f);
		}
	}
	else
	{
		// 旧フォーマット（C/サラウンド/リア/LFEの4係数）から移行する。
		DmGains[2] = FMath::Clamp(Save->DmCenter,   0.0f, 1.5f); // C
		DmGains[3] = FMath::Clamp(Save->DmLfe,      0.0f, 1.5f); // LFE
		DmGains[4] = FMath::Clamp(Save->DmSurround, 0.0f, 1.5f); // Ls
		DmGains[5] = FMath::Clamp(Save->DmSurround, 0.0f, 1.5f); // Rs
		DmGains[6] = FMath::Clamp(Save->DmRear,     0.0f, 1.5f); // Lb
		DmGains[7] = FMath::Clamp(Save->DmRear,     0.0f, 1.5f); // Rb
		// フロントL/R(0,1) は既定 1.0 のまま
	}
	EnsureCustomLayoutDefaults();

	SlackToken = Save->SlackToken;
	SlackToUser = Save->SlackToUser;
	TargetLUFS = FMath::Clamp(Save->TargetLUFS, -60.f, 0.f);
	bLimitPeakOnNormalize = Save->bLimitPeakOnNormalize;
	UpdateLimitPeakButton();
	MeterIntervalMs = FMath::Clamp(Save->MeterIntervalMs, 50, 2000);
	bMeterSignalIndicator = Save->bMeterSignalIndicator;
	bMeterDotPerTrack = Save->bMeterDotPerTrack;
	bTrackListAutoRefresh = Save->bTrackListAutoRefresh;
	TrackRefreshIntervalSec = FMath::Clamp(Save->TrackRefreshIntervalSec, 1, 300);
	UIThemeIndex = FMath::Clamp(Save->UIThemeIndex, 0, 2);
	ApplyTheme();          // 読み込んだテーマを背景へ反映
	UpdateThemeButton();

	ScreenOrientationLock = FMath::Clamp(Save->ScreenOrientationLock, 0, 2);
	ApplyOrientation();     // 読み込んだ向き固定を端末へ反映
	UpdateOrientationButton();

	// 言語（9言語）。範囲外の値は日本語にフォールバック
	const uint8 LangCount = static_cast<uint8>(ReaperLang::ELanguage::Count);
	ReaperLang::SetLanguage(Save->Language < LangCount
		? static_cast<ReaperLang::ELanguage>(Save->Language)
		: ReaperLang::ELanguage::JP);
	ApplyLanguage();        // 読み込んだ言語をUI全体へ反映

	// リスト設定
	ListFormat = FMath::Clamp(Save->ListFormat, 0, 2);   // 0=Excel/1=CSV/2=Google Sheets（旧バグ：2を保存しても0-1にクランプされ消えていた）
	ListFilePath = Save->ListFilePath;
	ListSheetName = Save->ListSheetName;
	ListStartRow = FMath::Max(1, Save->ListStartRow);
	ListStartCol = FMath::Max(1, Save->ListStartCol);
	ListColCount = FMath::Clamp(Save->ListColCount, 1, 32);
	bListUseCheckbox = Save->bListUseCheckbox;
	ListExportPath = Save->ListExportPath;
	GoogleSheetSpreadsheetId = Save->GoogleSheetSpreadsheetId;
	GoogleSheetClientId = Save->GoogleSheetClientId;
	ScriptMatchScope = FMath::Clamp(Save->ScriptMatchScope, 0, 1);
	ScriptMatchCheckFilter = FMath::Clamp(Save->ScriptMatchCheckFilter, 0, 2);
	IntercomUserName = Save->IntercomUserName;
	PresenceFolder = Save->PresenceFolder;
	ResolvedPresenceFolder = Save->ResolvedPresenceFolder;   // 前回接続で解決したAZDataパスを復元
	if (MemberUserNameInput) { MemberUserNameInput->SetText(FText::FromString(IntercomUserName)); }
	if (ListFilePathInput)   { ListFilePathInput->SetText(FText::FromString(ListFilePath)); }
	if (ListSheetInput)      { ListSheetInput->SetText(FText::FromString(ListSheetName)); }
	if (ListStartRowInput)   { ListStartRowInput->SetText(FText::FromString(FString::FromInt(ListStartRow))); }
	if (ListStartColInput)   { ListStartColInput->SetText(FText::FromString(ExcelColumnLetter(ListStartCol))); }
	if (ListColCountInput)   { ListColCountInput->SetText(FText::FromString(FString::FromInt(ListColCount))); }
	if (ListExportPathInput) { ListExportPathInput->SetText(FText::FromString(ListExportPath)); }
	if (ListGSheetIdInput) { ListGSheetIdInput->SetText(FText::FromString(GoogleSheetSpreadsheetId)); }
	if (ListGSheetClientIdInput) { ListGSheetClientIdInput->SetText(FText::FromString(GoogleSheetClientId)); }
	UpdateListFormatButton();
	UpdateListFormatRowsVisibility();
	UpdateListCheckboxButton();
	UpdateScriptMatchScopeButton();
	UpdateScriptMatchCheckFilterButton();
	if (SlackTokenInput)  { SlackTokenInput->SetText(FText::FromString(SlackToken)); }
	if (SlackToUserInput) { SlackToUserInput->SetText(FText::FromString(SlackToUser)); }
	if (TargetLufsInput)  { TargetLufsInput->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), TargetLUFS))); }
	if (MeterIntervalInput) { MeterIntervalInput->SetText(FText::FromString(FString::FromInt(MeterIntervalMs))); }
	UpdateMeterModeButton();
	UpdateMeterDotScopeButton();
	if (TrackRefreshIntervalInput) { TrackRefreshIntervalInput->SetText(FText::FromString(FString::FromInt(TrackRefreshIntervalSec))); }
	bTrackListAutoRefreshRunning = false;   // 再起動時は必ず停止状態から始める
	UpdateTrackRefreshModeButton();
	UpdateUpdateButton();

	if (IPAddressInput)
	{
		IPAddressInput->SetText(FText::FromString(IPAddress));
	}
	if (PortInput)
	{
		PortInput->SetText(FText::FromString(FString::FromInt(Port)));
	}
	UpdateShowDeviceInfoButton();
	UpdateHeaderIpText();
	// 復元した設定をボタン表示へ反映
	UpdateMicInputModeButton();
	UpdateMonitorOutputButton();
	UpdateMonitorBufferButton();
	RefreshDmCoeffInputs();
	UpdateSpatialButtons();
	UpdateMappingControls();
	RefreshHrtfCombo();
	UpdateSpatialSettingsVisibility();
}

// ====================================================================
//  ピンチイン/アウトによる表示倍率
// ====================================================================
FReply UWG_RadNodzToolkit::NativeOnTouchGesture(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (InGestureEvent.GetGestureType() == EGestureEvent::Magnify)
	{
		// マグニファイ ジェスチャのデルタ（指を広げると正・狭めると負）を倍率に加算
		const float Delta = InGestureEvent.GetGestureDelta().X;
		ZoomLevel = FMath::Clamp(ZoomLevel + Delta, 0.5f, 3.0f);
		ApplyZoom();
		return FReply::Handled();
	}
	return Super::NativeOnTouchGesture(InGeometry, InGestureEvent);
}

void UWG_RadNodzToolkit::ApplyZoom()
{
	// 中心を基準に全体を拡大/縮小する
	SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
	SetRenderScale(FVector2D(ZoomLevel, ZoomLevel));
}

void UWG_RadNodzToolkit::SetStatusIconTexture(const TCHAR* TexturePath)
{
	if (!StatusIcon) return;
	if (UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, TexturePath))
	{
		StatusIcon->SetBrushFromTexture(Tex);
	}
}

void UWG_RadNodzToolkit::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 横画面対応：画面の縦横比から向きを判定し、変わったらホームのパネル配置を切り替える。
	// （1.05はヒステリシス。縦横がほぼ同じ大きさのときに往復してチラつくのを防ぐ）
	{
		const FVector2D ScreenSize = MyGeometry.GetLocalSize();
		if (ScreenSize.X > 1.f && ScreenSize.Y > 1.f)
		{
			const int32 Want = (ScreenSize.X > ScreenSize.Y * 1.05f) ? 1 : 0;
			if (Want != HomeOrientationApplied)
			{
				ApplyHomeOrientation(Want == 1);
				HomeOrientationApplied = Want;
			}
		}
	}

	// 接続中・台本一致の文字起こし中：アイコンを縦軸まわりに回転（横スケールを cos で 1→0→-1→0→1 とフリップ）
	if (bStatusIconSpin && StatusIcon)
	{
		StatusIconSpinPhase += InDeltaTime * 4.5f;   // 回転速度
		const float ScaleX = FMath::Cos(StatusIconSpinPhase);
		StatusIcon->SetRenderScale(FVector2D(ScaleX, 1.0f));
	}

	// 送信/受信アクティビティの〇（声が流れている間だけ緑に点滅）。
	{
		const FLinearColor DotOff(0.20f, 0.22f, 0.24f, 1.f);
		const FLinearColor Green(0.15f, 0.85f, 0.45f, 1.f);
		const double Now = FPlatformTime::Seconds();
		const float Blink = 0.40f + 0.60f * (0.5f + 0.5f * FMath::Sin((float)Now * 12.0f));   // 連続点滅

		// グループトークの送受信インジケータをch毎に更新する。
		// 送信ドット：そのchが現在の送信先で、かつ直近に声が入っているとき点滅。
		// 受信ドット：そのchの受信ONで、かつ直近にパケットを受信しているとき点滅。
		const int32 GtTxTarget = ComputeTransmitTarget();
		const double GtSendLast = GroupTalkSender ? GroupTalkSender->GetLastVoiceTime() : 0.0;
		for (int32 Ch = 0; Ch < 2; ++Ch)
		{
			const bool bTxOn = (GtTxTarget == Ch) && (GtSendLast > 0.0) && ((Now - GtSendLast) < 0.35);
			const double RxLast = GroupTalkReceiver[Ch] ? GroupTalkReceiver[Ch]->GetLastRecvTime() : 0.0;
			const bool bRxOn = bGroupTalkRecvOn[Ch] && (RxLast > 0.0) && ((Now - RxLast) < 0.35);

			if (GroupTalkSendDot[Ch]) { GroupTalkSendDot[Ch]->SetColorAndOpacity(bTxOn ? FLinearColor(Green.R, Green.G, Green.B, Blink) : DotOff); }
			if (GroupTalkRecvDot[Ch]) { GroupTalkRecvDot[Ch]->SetColorAndOpacity(bRxOn ? FLinearColor(Green.R, Green.G, Green.B, Blink) : DotOff); }
		}
	}

	// グループトーク：どちらかのchがアクティブな間は、両chの参加状況をハートビート書き込み＋
	// 参加者一覧を定期更新する。無音になった参加者（退出・切断）の再生も定期的に掃除する。
	if (bIsConnected && !IntercomUserName.IsEmpty() &&
		(IsGroupTalkChannelActive(0) || IsGroupTalkChannelActive(1)))
	{
		GroupTalkHeartbeatTimer += InDeltaTime;
		if (GroupTalkHeartbeatTimer >= 5.0)   // 5秒ごとに参加状況を更新（素早い反映が望ましいため在席より短め）
		{
			GroupTalkHeartbeatTimer = 0.0;
			WriteGroupTalkHeartbeat(0);
			WriteGroupTalkHeartbeat(1);
		}
		GroupTalkRosterTimer += InDeltaTime;
		if (GroupTalkRosterTimer >= 3.0)   // 3秒ごとに参加者一覧を更新
		{
			GroupTalkRosterTimer = 0.0;
			RefreshGroupTalkRoster();
		}
		GroupTalkPruneTimer += InDeltaTime;
		if (GroupTalkPruneTimer >= 2.0)   // 2秒ごとに、無音の参加者の再生を掃除
		{
			GroupTalkPruneTimer = 0.0;
			for (int32 Ch = 0; Ch < 2; ++Ch)
			{
				if (GroupTalkReceiver[Ch]) { GroupTalkReceiver[Ch]->PruneStalePeers(6.0f); }
			}
		}
	}

	// 接続監視（接続中のみ・1秒ごと）：RigdocksClient が切断されていれば切断状態へ遷移する。
	if (bIsConnected && Client)
	{
		ConnCheckTimer += InDeltaTime;
		if (ConnCheckTimer >= ConnCheckIntervalSec)
		{
			ConnCheckTimer = 0.0;
			if (!Client->IsConnected())
			{
				Disconnect();   // bIsConnected=false / キャッシュ破棄 / 切断UI・OnDisconnected を実施
			}
		}
	}

	// Reaper通信時間の定期計測（接続中のみ・30秒ごと）。接続直後の1回目は接続処理側で実施。
	if (bIsConnected && Client)
	{
		CommTimeProbeTimer += InDeltaTime;
		if (CommTimeProbeTimer >= CommTimeProbeIntervalSec)
		{
			CommTimeProbeTimer = 0.0;
			MeasureCommTime();
		}
	}

	// メディア一覧のバックグラウンド先読み（接続/更新後・少しずつ・UIを止めない）
	TickMediaCacheWarm(InDeltaTime);

	// ラウドネスのバックグラウンド先読み（少しずつ・UIを止めない）
	TickLoudnessPrefetch(InDeltaTime);

	// 入力レベルメーター：アーム中トラックのピークを取得してカードへ反映
	TickTrackMeters(InDeltaTime);

	// トラック一覧の自動更新（設定でONの場合のみ、指定間隔ごとに「更新」相当の処理を行う）
	TickTrackListAutoRefresh(InDeltaTime);

	// 計測ジョブ（Slack共有 / 計測＆音量調整）を少しずつ処理する
	TickMeasureJob(InDeltaTime);

	// 「計測＆音量調整」実行中は、ボタンの計測アイコンを縦回転させる
	if (bMeasureJobActive && bNormalizeAfterMeasure && NormalizeIcon)
	{
		NormalizeSpinPhase += InDeltaTime * 4.5f;
		NormalizeIcon->SetRenderScale(FVector2D(FMath::Cos(NormalizeSpinPhase), 1.0f));
	}
	else if (!bMeasureJobActive && NormalizeIcon)
	{
		NormalizeIcon->SetRenderScale(FVector2D(1.0f, 1.0f));   // 終了で正立に戻す
	}

	// 台本一致：文字起こしジョブを1件ずつ進める（重い処理をTickへ分散し、UIを固まらせない）
	TickScriptMatchJob(InDeltaTime);
}


void UWG_RadNodzToolkit::PrintErrorMsg()
{
	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("Code: %d  %s"), Client->GetError().ErrorCode, *Client->GetError().Message),
		true,   // Screen
		true,   // Log
		FLinearColor::Green,
		2.0f
	);
	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("%s"), *Client->GetError().Details),
		true,   // Screen
		true,   // Log
		FLinearColor::Green,
		2.0f
	);
}

// ============================================================================
//  認証設定（mTLS）: serverId 管理 / 公開鍵コピー / 試し接続
// ============================================================================

FString UWG_RadNodzToolkit::GetAuthStorageDir() const
{
	// モバイルでもアプリサンドボックス配下で永続する保存先。
	return FPaths::Combine(FPaths::ProjectPersistentDownloadDir(), TEXT("RADNODZ"));
}

void UWG_RadNodzToolkit::EnsureAuthClient()
{
	if (!AuthClient)
	{
		AuthClient = URadnodzClient::CreateRadnodzClient();
	}
	AuthClient->SetStorageDirectory(GetAuthStorageDir());
}

int32 UWG_RadNodzToolkit::FindServerAuthIndex(const FString& ServerId) const
{
	for (int32 i = 0; i < ServerAuthEntries.Num(); ++i)
	{
		if (ServerAuthEntries[i].ServerId == ServerId) { return i; }
	}
	return INDEX_NONE;
}

FString UWG_RadNodzToolkit::GetSelectedServerId() const
{
	return ServerIdCombo ? ServerIdCombo->GetSelectedOption() : FString();
}

void UWG_RadNodzToolkit::SetAuthStatus(const FString& Msg)
{
	if (AuthStatusText) { AuthStatusText->SetText(FText::FromString(Msg)); }
}

void UWG_RadNodzToolkit::RefreshServerIdCombo()
{
	if (!ServerIdCombo) { return; }

	const FString Prev = ServerIdCombo->GetSelectedOption();
	ServerIdCombo->ClearOptions();
	for (const FServerAuthEntry& E : ServerAuthEntries)
	{
		ServerIdCombo->AddOption(E.ServerId);
	}

	// 以前の選択を維持。無効なら先頭を選ぶ。
	FString ToSelect = Prev;
	if (ToSelect.IsEmpty() || FindServerAuthIndex(ToSelect) == INDEX_NONE)
	{
		ToSelect = (ServerAuthEntries.Num() > 0) ? ServerAuthEntries[0].ServerId : FString();
	}
	if (!ToSelect.IsEmpty())
	{
		ServerIdCombo->SetSelectedOption(ToSelect);
	}

	// 選択中の期待FPを入力欄へ反映
	if (ServerFpInput)
	{
		const int32 Idx = FindServerAuthIndex(ToSelect);
		ServerFpInput->SetText(FText::FromString(Idx != INDEX_NONE ? ServerAuthEntries[Idx].Fingerprint : FString()));
	}

	// 選択中 serverId に紐づく IP/ポートを接続先へ復元する。
	ApplySelectedServerConn();
}

void UWG_RadNodzToolkit::HandleServerIdComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	const int32 Idx = FindServerAuthIndex(SelectedItem);
	if (ServerFpInput)
	{
		ServerFpInput->SetText(FText::FromString(Idx != INDEX_NONE ? ServerAuthEntries[Idx].Fingerprint : FString()));
	}
	// 選択された serverId に紐づく IP/ポートを接続先へ復元する。
	ApplySelectedServerConn();
}

void UWG_RadNodzToolkit::ApplySelectedServerConn()
{
	const int32 Idx = FindServerAuthIndex(GetSelectedServerId());
	if (Idx == INDEX_NONE) { return; }
	const FServerAuthEntry& E = ServerAuthEntries[Idx];
	// 未紐付け（旧エントリ等で IP 空）のときは現在の接続先を維持する。
	if (E.IPAddress.IsEmpty()) { return; }

	IPAddress = E.IPAddress;
	Port      = E.Port;
	if (IPAddressInput) { IPAddressInput->SetText(FText::FromString(IPAddress)); }
	if (PortInput)      { PortInput->SetText(FText::FromString(FString::FromInt(Port))); }
	UpdateHeaderIpText();
}

void UWG_RadNodzToolkit::StoreConnToSelectedServer()
{
	const int32 Idx = FindServerAuthIndex(GetSelectedServerId());
	if (Idx == INDEX_NONE) { return; }
	ServerAuthEntries[Idx].IPAddress = IPAddress;
	ServerAuthEntries[Idx].Port      = Port;
}

void UWG_RadNodzToolkit::HandleReaperIpCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod != ETextCommit::OnEnter && CommitMethod != ETextCommit::OnUserMovedFocus)
	{
		return;
	}
	IPAddress = Text.ToString().TrimStartAndEnd();
	StoreConnToSelectedServer();   // 選択中 serverId に紐づけて保存
	SaveSettings();
	UpdateHeaderIpText();
}

void UWG_RadNodzToolkit::HandlePortCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod != ETextCommit::OnEnter && CommitMethod != ETextCommit::OnUserMovedFocus)
	{
		return;
	}
	const FString PortStr = Text.ToString().TrimStartAndEnd();
	if (PortStr.IsNumeric())
	{
		Port = FCString::Atoi(*PortStr);
	}
	StoreConnToSelectedServer();   // 選択中 serverId に紐づけて保存
	SaveSettings();
}

void UWG_RadNodzToolkit::HandleAddServerIdClicked()
{
	// ポップアップで新しい serverId を入力させる（登録時に AddServerAuthEntry が呼ばれる）。
	UWG_AddServerIdDialog* Dlg = CreateWidget<UWG_AddServerIdDialog>(this, UWG_AddServerIdDialog::StaticClass());
	if (Dlg)
	{
		Dlg->Setup(this);
		Dlg->AddToViewport(200);
	}
}

void UWG_RadNodzToolkit::AddServerAuthEntry(const FString& NewId)
{
	const FString Id = NewId.TrimStartAndEnd();
	if (Id.IsEmpty())
	{
		SetAuthStatus(ReaperLang::S(TEXT("サーバーID名を入力してください。"), TEXT("Enter a server ID.")));
		return;
	}
	if (FindServerAuthIndex(Id) != INDEX_NONE)
	{
		SetAuthStatus(ReaperLang::S(TEXT("同名のサーバーIDが既に存在します。"), TEXT("Server ID already exists.")));
		return;
	}

	// 新規サーバーIDは接続先を未設定にする（IP/ポート欄は空欄にする）。
	FServerAuthEntry E;
	E.ServerId = Id;
	E.Fingerprint = FString();
	E.IPAddress = FString();   // 空＝未紐付け（IP を入力/接続で紐づくまで復元されない）
	ServerAuthEntries.Add(E);
	SaveSettings();

	RefreshServerIdCombo();
	if (ServerIdCombo) { ServerIdCombo->SetSelectedOption(Id); }
	if (ServerFpInput) { ServerFpInput->SetText(FText::GetEmpty()); }
	// 追加直後は Reaper IP / ポート欄を空欄にする。
	IPAddress.Empty();
	if (IPAddressInput) { IPAddressInput->SetText(FText::GetEmpty()); }
	if (PortInput)      { PortInput->SetText(FText::GetEmpty()); }
	UpdateHeaderIpText();
	SetAuthStatus(FString::Printf(TEXT("%s: %s"), *ReaperLang::S(TEXT("追加しました"), TEXT("Added")), *Id));
}

void UWG_RadNodzToolkit::HandleUseTlsClicked()
{
	bUseTls = !bUseTls;
	UpdateUseTlsButton();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateUseTlsButton()
{
	if (UseTlsButtonText)
	{
		UseTlsButtonText->SetText(FText::FromString(bUseTls
			? ReaperLang::S(TEXT("ON"), TEXT("ON"))
			: ReaperLang::S(TEXT("OFF"), TEXT("OFF"))));
	}
}

void UWG_RadNodzToolkit::HandleDeleteServerIdClicked()
{
	const FString Sel = GetSelectedServerId();
	if (Sel.IsEmpty())
	{
		SetAuthStatus(ReaperLang::S(TEXT("削除するサーバーIDを選択してください。"), TEXT("Select a server ID to delete.")));
		return;
	}

	// 確認ポップアップを表示し、「はい」で ConfirmDeleteServerId を実行する。
	UWG_ConfirmDialog* Dlg = CreateWidget<UWG_ConfirmDialog>(this, UWG_ConfirmDialog::StaticClass());
	if (Dlg)
	{
		Dlg->Setup(ReaperLang::S(TEXT("サーバーIDを削除しますか？"), TEXT("Delete this server ID?")));
		Dlg->OnConfirmed.BindDynamic(this, &UWG_RadNodzToolkit::ConfirmDeleteServerId);
		Dlg->AddToViewport(200);
	}
}

void UWG_RadNodzToolkit::ConfirmDeleteServerId()
{
	const FString Sel = GetSelectedServerId();
	const int32 Idx = FindServerAuthIndex(Sel);
	if (Idx == INDEX_NONE) { return; }

	ServerAuthEntries.RemoveAt(Idx);
	SaveSettings();
	RefreshServerIdCombo();
	SetAuthStatus(FString::Printf(TEXT("%s: %s"), *ReaperLang::S(TEXT("削除しました"), TEXT("Deleted")), *Sel));
}

void UWG_RadNodzToolkit::HandleServerFpCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 入力確定（Enter / フォーカス移動）のときだけ保存する。
	if (CommitMethod != ETextCommit::OnEnter && CommitMethod != ETextCommit::OnUserMovedFocus)
	{
		return;
	}
	const FString Sel = GetSelectedServerId();
	const int32 Idx = FindServerAuthIndex(Sel);
	if (Idx == INDEX_NONE)
	{
		SetAuthStatus(ReaperLang::S(TEXT("先にサーバーIDを選択/追加してください。"), TEXT("Select or add a server ID first.")));
		return;
	}
	ServerAuthEntries[Idx].Fingerprint = Text.ToString().TrimStartAndEnd();
	SaveSettings();
	SetAuthStatus(FString::Printf(TEXT("%s: %s"), *ReaperLang::S(TEXT("FPを更新しました"), TEXT("Fingerprint updated")), *Sel));
}

void UWG_RadNodzToolkit::HandleCopyPublicKeyClicked()
{
	EnsureAuthClient();
	if (!AuthClient->GenerateIdentity())
	{
		SetAuthStatus(ReaperLang::S(TEXT("鍵の用意に失敗しました。"), TEXT("Failed to prepare identity.")));
		return;
	}
	const FString Pub = AuthClient->GetOwnPublicKeyPem();
	if (Pub.IsEmpty())
	{
		SetAuthStatus(ReaperLang::S(TEXT("公開鍵の取得に失敗しました。"), TEXT("Failed to get public key.")));
		return;
	}
	FPlatformApplicationMisc::ClipboardCopy(*Pub);
	SetAuthStatus(FString::Printf(TEXT("%s (FP: %s)"),
		*ReaperLang::S(TEXT("公開鍵をクリップボードにコピーしました"), TEXT("Public key copied to clipboard")),
		*AuthClient->GetOwnFingerprint()));
}

void UWG_RadNodzToolkit::HandleTrialConnectClicked()
{
	using namespace ReaperCtrlUI;

	if (bIsAuthenticating) { return; } // 実行中は再入しない

	const FString Ip = IPAddressInput ? IPAddressInput->GetText().ToString() : IPAddress;
	const int32   P  = PortInput ? FCString::Atoi(*PortInput->GetText().ToString()) : Port;

	// トグルの ON/OFF を正しく反映するため、認証は毎回新しいクライアントで行う
	// (identity をロード済みのクライアントを使い回すと OFF でも TLS になってしまう)。
	URadnodzClient* C = URadnodzClient::CreateRadnodzClient();
	if (!C)
	{
		SetAuthStatus(ReaperLang::S(TEXT("クライアントの生成に失敗しました。"), TEXT("Failed to create client.")));
		return;
	}
	// バックグラウンド接続中に GC 回収されないようルート登録する。
	C->AddToRoot();

	if (bUseTls)
	{
		const FString Sel = GetSelectedServerId();
		if (Sel.IsEmpty())
		{
			C->RemoveFromRoot();
			SetAuthStatus(ReaperLang::S(TEXT("サーバーIDを選択してください。"), TEXT("Select a server ID.")));
			return;
		}
		const int32 Idx = FindServerAuthIndex(Sel);
		const FString Fp = (Idx != INDEX_NONE) ? ServerAuthEntries[Idx].Fingerprint : FString();
		if (Fp.IsEmpty())
		{
			C->RemoveFromRoot();
			SetAuthStatus(ReaperLang::S(TEXT("このサーバーのFPが未設定です。"), TEXT("This server's fingerprint is not set.")));
			return;
		}

		C->SetStorageDirectory(GetAuthStorageDir());
		if (!C->GenerateIdentity())
		{
			C->RemoveFromRoot();
			SetAuthStatus(ReaperLang::S(TEXT("鍵の用意に失敗しました。"), TEXT("Failed to prepare identity.")));
			return;
		}
		C->SetServerId(Sel);
		C->SetExpectedServerFingerprint(Fp);
	}

	// --- 「認証中...」表示（接続ボタンと同じ視覚フィードバック）---
	bIsAuthenticating = true;
	SetAuthStatus(FString::Printf(TEXT("%s %s:%d ..."), *ReaperLang::S(TEXT("認証中"), TEXT("Authenticating")), *Ip, P));
	if (TrialConnectButton && TrialConnectButtonText)
	{
		TrialConnectButtonText->SetText(ReaperLang::T(TEXT("認証中..."), TEXT("Authenticating...")));
		TrialConnectButton->SetBackgroundColor(StopGray);
		TrialConnectButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(StopGray)));
	}
	// 右上ロゴを回転させる（接続中と同じ）
	SetStatusIconTexture(TEXT("/Game/Texture/T_RADNODZ_white_conect_1.T_RADNODZ_white_conect_1"));
	bStatusIconSpin = true;
	StatusIconSpinPhase = 0.f;

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);
	Async(EAsyncExecution::Thread, [WeakThis, C, Ip, P]()
	{
		bool bOk = false;
		FRigdocksError Err;
		if (C)
		{
			bOk = C->Connect(Ip, P);
			if (!bOk) { Err = C->GetError(); }
			else { C->Disconnect(); } // 試し接続なので成功後は切断する
		}

		// 結果はゲームスレッドで反映する（UMG はゲームスレッド限定）
		AsyncTask(ENamedThreads::GameThread, [WeakThis, C, bOk, Err]()
		{
			if (UWG_RadNodzToolkit* Self = WeakThis.Get())
			{
				Self->HandleAuthResult(bOk, Err);
			}
			if (IsValid(C)) { C->RemoveFromRoot(); }
		});
	});
}

void UWG_RadNodzToolkit::HandleAuthResult(bool bSuccess, const FRigdocksError& Error)
{
	using namespace ReaperCtrlUI;

	bIsAuthenticating = false;

	// 「認証」ボタンを元に戻す
	if (TrialConnectButton && TrialConnectButtonText)
	{
		TrialConnectButtonText->SetText(ReaperLang::T(TEXT("認証"), TEXT("Authenticate")));
		TrialConnectButton->SetBackgroundColor(AccentBlue);
		TrialConnectButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(AccentBlue)));
	}

	// 右上ロゴ/上部ステータスは本体の接続状態へ戻す（回転も停止する）
	if (bIsConnected) { ApplyConnectedUI(); }
	else { ApplyDisconnectedUI(); }

	// 認証結果を認証セクションのステータスに表示
	if (bSuccess)
	{
		SetAuthStatus(bUseTls
			? ReaperLang::S(TEXT("接続成功: 認証OK"), TEXT("Connected: auth OK"))
			: ReaperLang::S(TEXT("接続成功(平文)"), TEXT("Connected (plain)")));
	}
	else
	{
		SetAuthStatus(FString::Printf(TEXT("%s code=%d %s"),
			*ReaperLang::S(TEXT("接続失敗:"), TEXT("Failed:")), Error.ErrorCode, *Error.Message));
	}
}