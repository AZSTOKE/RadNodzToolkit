// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "WG_RadNodzToolkit.h"
#include "WG_TrackCard.h"
#include "WG_AddTracksDialog.h"
#include "WG_DeleteConfirmDialog.h"
#include "WG_MarkerNameDialog.h"
#include "WG_MarkerRow.h"
#include "Components/ScrollBoxSlot.h"
#include "WG_UnlockConfirmDialog.h"
#include "ReaStreamGroupReceiver.h"
#include "ReaperBackgroundKeepAlive.h"
#include "ScreenRecorder.h"
#include "GroupTalkMic.h"
#include "WG_ListRow.h"
#include "WG_SheetTab.h"
#include "WG_ColumnPickerButton.h"
#include "WG_SlackRequiredDialog.h"
#include "RigdocksGold_Excel.h"
#include "RigdocksGold_GSheet.h"
#include "GoldClasses.h"
#include "RigdocksSilver_File.h"
#include "RigdocksSilver_Device.h"
#include "RigdocksSilver_Project.h"
#include "ReaperClasses.h"   // UVarType
#include "WG_MicSettingsDialog.h"
#include "WG_AddServerIdDialog.h"
#include "WG_ConfirmDialog.h"
#include "WG_InfoDialog.h"
#include "WG_ReaperScanDialog.h"
#include "WG_MediaItemRow.h"
#include "Actions/RadNodzAction.h"                  // ホーム「アクション」ドロップダウン
#include "Actions/Action_IRMeasurementPrepare.h"
#include "Actions/Action_IRRecord.h"
#include "Actions/Action_IRMeasurement.h"
#include "RigdocksBronze_MediaItem.h"
#include "RigdocksSilver_Loudness.h"
#include "RigdocksSilver_Peak.h"
#include "RigdocksSilver_Slack.h"
#include "RigdocksSilver_TRSC.h"
#include "RigdocksSilver_String.h"
#include "RadnodzClient.h"   // URadnodzClient::SendTimingProbe / GetLast*Time（通信時間の計測）
#include "RigdocksError.h"   // AZ_ErrorCode（GSheet接続失敗の原因判定に使う）

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
#include "Build/RigdocksSilver_Loudness_Build.h"   // ラウドネス計測をワーカーで流すため
#include "Parse/RigdocksSilver_Loudness_Parse.h"
#include "Build/RigdocksSilver_TRSC_Build.h"       // 文字起こしをワーカーで流すため
#include "Parse/RigdocksSilver_TRSC_Parse.h"
#include "Build/RigdocksSilver_String_Build.h"
#include "Parse/RigdocksSilver_String_Parse.h"
#include "Build/RigdocksBronze_Cursor_Build.h"
#include "RigdocksBronze_Cursor.h"                 // AZ_GetPlayCursorPosition（マーカー位置取得）
#include "RigdocksSilver_Marker.h"                 // AZ_AddMarker（マーカー追加）
#include "Build/RigdocksSilver_Marker_Build.h"     // AZ_GetMarkerObject_Build（一覧列挙）
#include "Parse/RigdocksSilver_Marker_Parse.h"     // AZ_GetMarkerObject_Parse
#include "Build/RigdocksGold_GSheet_Build.h"
#include "Build/RigdocksGold_Excel_Build.h"
#include "DataClass/FPeak.h"
#include "ReaperSaveGame.h"
#include "ReaperFont.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"
#include "Misc/Paths.h"
#include "ReaperLang.h"
#include "ReaStreamReceiver.h"
#include "ParaknotSessionClient.h"
#include "HRTF/HRTFRegistry.h"
#include "Components/ComboBoxString.h"
#include "ReaStreamSender.h"
#include "ParaknotRpc.h"
#include "ParaknotReceiver.h"
#include "MonitorOutputProcessor.h"
#include "ParaknotSender.h"
#include "WG_ParaknotPickDialog.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#if PLATFORM_ANDROID
#include "AndroidPermissionFunctionLibrary.h"
#endif

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWaveProcedural.h"   // ローカルIRのTSPを逐次キューして鳴らす
#include "Components/AudioComponent.h"   // 同上（CreateSound2Dの戻り）
#include "HAL/PlatformProcess.h"   // FPlatformProcess::ComputerName（自端末名）
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/ButtonSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Styling/SlateTypes.h"   // FSliderStyle / FProgressBarStyle
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
#include "Engine/Engine.h"                 // GEngine
#include "Engine/GameViewportClient.h"     // AddViewportWidgetContent（カウントダウンオーバーレイ）
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
	// 受信ON＆マスターch用。上に重なる緑の点滅ドットとアイコンが埋もれないよう、
	// 暗く落とした中間のティール。
	static const FLinearColor BtnTealMid   = FLinearColor(0.07f, 0.42f, 0.40f, 1.f);

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

	// 折り返し有効テキストが「縮んだまま戻らない」問題への対策。
	// 折り返すと要求幅(DesiredSize)が折り返し後の狭い幅で報告されるため、Autoサイズの親では
	// 「狭い→折り返す→要求幅が狭い→広がれない」のループになり、画面サイズが元に戻っても
	// 1文字ずつの縦折返しから復帰しない。1行表示時の実測幅を最小要求幅として持たせておくと
	// 要求幅が縮まなくなり、スペースが戻れば自動的に1行表示へ復帰する
	// （Fillスロット配置では割当幅が親で決まるため、幅が足りない間は従来どおり折り返す）。
	static void ApplyUnwrappedMinWidth(UTextBlock* T)
	{
		if (!T || !FSlateApplication::IsInitialized()) { return; }
		TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		const float W = FontMeasure->Measure(T->GetText(), T->GetFont()).X;
		T->SetMinDesiredWidth(W + 2.f);   // 実測誤差による境界上の再折り返しを避ける余白
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
		Txt->SetAutoWrapText(!ReaperLang::IsJapanese());   // 日本語は短く折り返し不要（画面再構築時に1文字ずつ縦折返しになるのを防ぐ）。長い翻訳文字列（仏語/伊語等）だけ2行に折り返す
		ApplyUnwrappedMinWidth(Txt);                       // 折り返しても画面サイズが戻れば1行表示へ復帰できるようにする

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

	// トグルスイッチ生成ヘルパー（丸いトラック＋つまみ）。
	//
	// ON/OFF を文字ではなく「つまみの位置」と「トラックの色」で示す設定項目用。
	// 押下は中の UButton が受けるので、呼び出し側で OnClicked を繋ぎ、状態変更時は
	// トラック色（SetBackgroundColor）とつまみのスロット配置（HAlign_Left/Right）を差し替える。
	static UWidget* MakeToggleSwitch(UWidgetTree* Tree, FName Name,
		TObjectPtr<UButton>& OutButton, TObjectPtr<UImage>& OutKnob)
	{
		const float Width = 84.f;
		const float Height = 44.f;
		const float KnobSize = 32.f;

		UButton* Btn = Tree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
		StyleRoundedButton(Btn, Height * 0.5f);   // 丸いトラック（色は Tint で付ける）

		UImage* Knob = Tree->ConstructWidget<UImage>(UImage::StaticClass());
		Knob->SetBrush(FSlateRoundedBoxBrush(FLinearColor::White, KnobSize * 0.5f,
			FVector2D(KnobSize, KnobSize)));
		Knob->SetDesiredSizeOverride(FVector2D(KnobSize, KnobSize));

		UOverlay* Ov = Tree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
		if (UOverlaySlot* S = Ov->AddChildToOverlay(Knob))
		{
			S->SetVerticalAlignment(VAlign_Center);
			S->SetHorizontalAlignment(HAlign_Left);   // 初期位置。状態は呼び出し側が反映する
			// 余白なし＝つまみはトラックの端から端まで動く（OFFで左端・ONで右端に接する）。
			// つまみの直径(32)はトラックの高さ(44)より小さいので、端でも丸い枠から出ない。
			S->SetPadding(FMargin(0.f));
		}
		Btn->SetContent(Ov);

		// **ボタンの中身はトラック全幅に広げる。**
		// 既定（中央寄せ・中身の希望サイズ）だと Overlay がつまみと同じ幅しか持たず、
		// つまみの HAlign_Left/Right が効かない（＝ON/OFFで動かない）。
		if (UButtonSlot* BS = Cast<UButtonSlot>(Ov->Slot))
		{
			BS->SetHorizontalAlignment(HAlign_Fill);
			BS->SetVerticalAlignment(VAlign_Fill);
			BS->SetPadding(FMargin(0.f));
		}

		USizeBox* Box = Tree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		// つまみを左右に寄せるので、幅は固定でないと意味がない。
		Box->SetMinDesiredWidth(Width);
		Box->SetMaxDesiredWidth(Width);
		Box->SetMinDesiredHeight(Height);
		Box->SetMaxDesiredHeight(Height);
		Box->AddChild(Btn);

		OutButton = Btn;
		OutKnob = Knob;
		return Box;
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

		// 接続状態はアイコンのみで表す。
		// （状態テキストはApply系から参照されるため生成はするが、非表示にしておく）
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

		// 接続/切断トグル ＋ その下に「ローカルIR」（端末単体でTSPを鳴らすモードへの切替）を縦に並べる
		{
			UVerticalBox* ConnCol = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());

			ConnectButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("接続"), TEXT("Connect")), BtnBlueDark,
				TEXT("ConnectButton"), &ConnectButtonText, TEXT("/Game/Texture/T_icon_conect_1.T_icon_conect_1"));
			{
				USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				BtnSize->SetMinDesiredHeight(64.f);
				BtnSize->AddChild(ConnectButton);
				UVerticalBoxSlot* S = ConnCol->AddChildToVerticalBox(BtnSize);
				S->SetHorizontalAlignment(HAlign_Fill);
			}

			// ローカルIR：押すとホーム画面がローカルIR用（回数/間隔＋大きな再生ボタン）に切り替わる。
			LocalIRButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("ローカルIR"), TEXT("Local IR")), StopGray,
				TEXT("LocalIRButton"), &LocalIRButtonText, TEXT("/Game/Texture/T_icon_loudness_1.T_icon_loudness_1"));
			{
				USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				BtnSize->SetMinDesiredHeight(56.f);
				BtnSize->AddChild(LocalIRButton);
				UVerticalBoxSlot* S = ConnCol->AddChildToVerticalBox(BtnSize);
				S->SetHorizontalAlignment(HAlign_Fill);
				S->SetPadding(FMargin(0.f, 8.f, 0.f, 0.f));
			}

			UHorizontalBoxSlot* S = HRow->AddChildToHorizontalBox(ConnCol);
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

		// 上段＝ボタン列 / 下段＝アクション行（ドロップダウン＋実行）
		UVerticalBox* ToolbarCol = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
		Panel->SetContent(ToolbarCol);

		UHorizontalBox* HRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		{
			UVerticalBoxSlot* S = ToolbarCol->AddChildToVerticalBox(HRow);
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		// 追加 / モニター / マイク / メーター / 更新 の5ボタンを等幅(Fill)で並べ、
		// ウィンドウ幅にちょうど収める。各ボタンは固定幅を持たせず均等配分する。
		// 各スロットへ等幅で追加するヘルパー（最後以外は右に間隔を空ける）。
		// **UWidget* で受ける。** モニターだけはメーターを重ねた Overlay を渡す。
		auto AddToolbarButton = [&](UWidget* Btn, bool bLast)
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

		// 音声モニター トグル。
		//
		// **受信レベルのメーターはボタンの下辺に重ねる。** 別の行に置くと
		// どのボタンのメーターなのか分からず、行も 1 段増える。
		// ボタンの上に Overlay で薄いバーを敷き、枠線より内側へ入れて
		// 「ボタンの下線がそのままメーターになっている」見た目にする。
		MonitorButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("モニター"), TEXT("Monitor")), StopGray,
			TEXT("MonitorButton"), &MonitorButtonText, TEXT("/Game/Texture/T_icon_moniter_1.T_icon_moniter_1"));
		{
			UOverlay* Ov = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());

			if (UOverlaySlot* O = Ov->AddChildToOverlay(MonitorButton))
			{
				O->SetHorizontalAlignment(HAlign_Fill);
				O->SetVerticalAlignment(VAlign_Fill);
			}

			MonitorLevelMeter = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());
			{
				// **下地は透明。** 溝は出さず、音が来ている分だけ色が乗る。
				//
				// **塗りのブラシは白にしておく。** 色は毎フレーム
				// SetFillColorAndOpacity で載せるが、あれは乗算なので
				// ブラシに緑を焼き込むと赤を指定したとき黒くなる。
				FProgressBarStyle PS = MonitorLevelMeter->WidgetStyle;
				PS.BackgroundImage = FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.f), 2.f);
				PS.FillImage       = FSlateRoundedBoxBrush(FLinearColor::White, 2.f);
				MonitorLevelMeter->WidgetStyle = PS;
				MonitorLevelMeter->SetFillColorAndOpacity(OkGreen);
			}
			MonitorLevelMeter->SetPercent(0.f);

			// **クリックを奪わせない。** 既定の Visible はヒットテストを受けるので、
			// ボタンの下辺を押したときに反応しなくなる。
			MonitorLevelMeter->SetVisibility(ESlateVisibility::HitTestInvisible);

			USizeBox* MeterBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			MeterBox->SetHeightOverride(6.f);
			MeterBox->AddChild(MonitorLevelMeter);

			if (UOverlaySlot* O = Ov->AddChildToOverlay(MeterBox))
			{
				O->SetHorizontalAlignment(HAlign_Fill);
				O->SetVerticalAlignment(VAlign_Bottom);

				// 角丸(6px)の内側へ収める。左右と下に余白を取る。
				// **下は詰める。** 離すと「ボタンの下線」に見えない。
				O->SetPadding(FMargin(6.f, 0.f, 6.f, 4.f));
			}

			AddToolbarButton(Ov, false);
		}

		// メーター受信トグル。ラベルは「メーター」のみ（ON/OFFは色で表現）。必要なときだけON（既定OFF）。
		MeterRecvButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("メーター"), TEXT("Meter")), StopGray,
			TEXT("MeterRecvButton"), &MeterRecvButtonText, TEXT("/Game/Texture/T_icon_meter_1.T_icon_meter_1"));
		AddToolbarButton(MeterRecvButton, false);

		UpdateButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("更新"), TEXT("Refresh")), BtnBlueDark,
			TEXT("UpdateButton"), &UpdateButtonText, TEXT("/Game/Texture/T_icon_update_1.T_icon_update_1"));
		AddToolbarButton(UpdateButton, true);



		// アクション行：[アクション] [ドロップダウン(Fill)] [実行]
		{
			UHorizontalBox* ARow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			{
				UVerticalBoxSlot* S = ToolbarCol->AddChildToVerticalBox(ARow);
				S->SetHorizontalAlignment(HAlign_Fill);
				S->SetPadding(FMargin(0.f, 12.f, 0.f, 0.f));
			}

			ActionLabelText = MakeText(WidgetTree, ReaperLang::S(TEXT("アクション"), TEXT("Action")), 18, TextPrimary, true);
			{
				UHorizontalBoxSlot* S = ARow->AddChildToHorizontalBox(ActionLabelText);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
			}

			RegisterActions();
			ActionCombo = MakeDarkCombo(WidgetTree);
			ActionCombo->OnGenerateWidgetEvent.BindDynamic(this, &UWG_RadNodzToolkit::MakeComboItemLight);
			RefreshActionCombo();   // 既定は未選択
			{
				USizeBox* ComboSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				ComboSize->SetMinDesiredHeight(56.f);
				ComboSize->AddChild(ActionCombo);
				UHorizontalBoxSlot* S = ARow->AddChildToHorizontalBox(ComboSize);
				S->SetVerticalAlignment(VAlign_Center);
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				S->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));
			}

			ActionRunButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("実行"), TEXT("Run")), BtnBlueDark,
				TEXT("ActionRunButton"), &ActionRunButtonText);
			{
				USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				BtnSize->SetMinDesiredHeight(56.f);
				BtnSize->SetMinDesiredWidth(120.f);
				BtnSize->AddChild(ActionRunButton);
				UHorizontalBoxSlot* S = ARow->AddChildToHorizontalBox(BtnSize);
				S->SetVerticalAlignment(VAlign_Center);
			}
			UpdateActionRunButton();
		}

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

		// 見出しの位置に「トラック / マーカー」切替を置く（ホーム/リスト/設定と同じ“下線＋文字が光る”方式）。
		// 各セル: [下線インジケータ] ＋ [透明ボタン＋中央テキスト]。選択中は文字=ティール＋下線表示。
		TrackMarkerTabRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("TrackMarkerTabRow"));
		{
			auto MakeSegCell = [&](const FString& Label, FName BtnName,
				TObjectPtr<UButton>& OutBtn, TObjectPtr<UTextBlock>& OutText, TObjectPtr<UBorder>& OutInd) -> UWidget*
			{
				UVerticalBox* Cell = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());

				// 透明背景のボタン＋中央ラベル
				UButton* Btn = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), BtnName);
				{
					FButtonStyle Style = Btn->GetStyle();
					const FSlateColorBrush Clear(FLinearColor(0.f, 0.f, 0.f, 0.f));
					const FSlateColorBrush Press(FLinearColor(1.f, 1.f, 1.f, 0.06f));
					Style.Normal = Clear; Style.Hovered = Press; Style.Pressed = Press; Style.Disabled = Clear;
					Btn->SetStyle(Style);
				}
				UTextBlock* Txt = MakeText(WidgetTree, Label, 18, TextDim, true);
				Txt->SetJustification(ETextJustify::Center);
				Btn->SetContent(Txt);
				{
					USizeBox* BtnH = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
					BtnH->SetMinDesiredHeight(52.f);
					BtnH->AddChild(Btn);
					UVerticalBoxSlot* S = Cell->AddChildToVerticalBox(BtnH);
					S->SetHorizontalAlignment(HAlign_Fill);
					S->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				}

				// アクティブ表示の細線（下端）
				UBorder* Ind = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
				Ind->SetBrushColor(AccentBlue);
				Ind->SetPadding(FMargin(0.f));
				{
					USizeBox* IndH = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
					IndH->SetHeightOverride(3.f);
					IndH->AddChild(Ind);
					UVerticalBoxSlot* S = Cell->AddChildToVerticalBox(IndH);
					S->SetHorizontalAlignment(HAlign_Fill);
				}

				OutBtn = Btn; OutText = Txt; OutInd = Ind;
				return Cell;
			};

			UWidget* TrackCell = MakeSegCell(ReaperLang::S(TEXT("トラック"), TEXT("Tracks")),
				TEXT("TrackTabButton"), TrackTabButton, TrackTabButtonText, TrackTabIndicator);
			{
				UHorizontalBoxSlot* S = TrackMarkerTabRow->AddChildToHorizontalBox(TrackCell);
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}
			UWidget* MarkerCell = MakeSegCell(ReaperLang::S(TEXT("マーカー"), TEXT("Markers")),
				TEXT("MarkerTabButton"), MarkerTabButton, MarkerTabButtonText, MarkerTabIndicator);
			{
				UHorizontalBoxSlot* S = TrackMarkerTabRow->AddChildToHorizontalBox(MarkerCell);
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}

			UVerticalBoxSlot* S = ListCol->AddChildToVerticalBox(TrackMarkerTabRow);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));
		}
		UpdateTrackMarkerTabStyles();

		// --- ローカルIR：タブ行の代わりに出す ＋／－（再生回数の増減）---
		// 「トラック」の位置に＋、「マーカー」の位置に－を、タブと同じ幅・同じ高さで置く。
		LocalIRCountRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("LocalIRCountRow"));
		LocalIRCountRow->SetVisibility(ESlateVisibility::Collapsed);
		{
			auto MakeStepCell = [&](const FString& Label, FName BtnName, TObjectPtr<UButton>& OutBtn) -> UWidget*
			{
				TObjectPtr<UTextBlock> Txt;
				OutBtn = MakeButton(WidgetTree, Label, BtnBlueDark, BtnName, &Txt);
				if (Txt) { Txt->SetFont(ReaperFont::Get(34, true)); }   // ＋／－は大きく
				USizeBox* BtnH = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				BtnH->SetMinDesiredHeight(60.f);
				BtnH->AddChild(OutBtn);
				return BtnH;
			};

			{
				UHorizontalBoxSlot* S = LocalIRCountRow->AddChildToHorizontalBox(
					MakeStepCell(TEXT("＋"), TEXT("LocalIRPlusButton"), LocalIRPlusButton));
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
			}
			{
				UHorizontalBoxSlot* S = LocalIRCountRow->AddChildToHorizontalBox(
					MakeStepCell(TEXT("－"), TEXT("LocalIRMinusButton"), LocalIRMinusButton));
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}

			UVerticalBoxSlot* S = ListCol->AddChildToVerticalBox(LocalIRCountRow);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));
		}

		TrackListScrollBox = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("TrackListScrollBox"));
		{
			UVerticalBoxSlot* S = ListCol->AddChildToVerticalBox(TrackListScrollBox);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}

		// --- マーカー一覧（マーカー選択時のみ表示。各行に再生/停止トグル＋編集可能な名前）---
		MarkerListScrollBox = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("MarkerListScrollBox"));
		MarkerListScrollBox->SetVisibility(ESlateVisibility::Collapsed);
		{
			UVerticalBoxSlot* S = ListCol->AddChildToVerticalBox(MarkerListScrollBox);
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}
		MarkerEmptyText = MakeText(WidgetTree, ReaperLang::S(TEXT("マーカーがありません"), TEXT("No markers")), 16, TextDim, false);
		MarkerEmptyText->SetVisibility(ESlateVisibility::Collapsed);
		{
			UVerticalBoxSlot* S = ListCol->AddChildToVerticalBox(MarkerEmptyText);
			S->SetPadding(FMargin(2.f, 6.f, 0.f, 0.f));
		}

		// --- ローカルIR：一覧の代わりに「回数」「間隔」を大きく出す領域 ---
		// 上から [回数ラベル][回数（大）] [間隔ラベル][間隔（大）] [間隔スライダー]。
		LocalIRCenterPanel = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("LocalIRCenterPanel"));
		LocalIRCenterPanel->SetVisibility(ESlateVisibility::Collapsed);
		{
			// 見出し＋大きな数字を1組作るヘルパー。
			auto AddBigValue = [&](const FString& Label, TObjectPtr<UTextBlock>& OutLabel, TObjectPtr<UTextBlock>& OutValue)
			{
				OutLabel = MakeText(WidgetTree, Label, 20, TextDim, true);
				OutLabel->SetJustification(ETextJustify::Center);
				{
					UVerticalBoxSlot* S = LocalIRCenterPanel->AddChildToVerticalBox(OutLabel);
					S->SetHorizontalAlignment(HAlign_Center);
				}
				OutValue = MakeText(WidgetTree, TEXT("1"), 72, TextPrimary, true);
				OutValue->SetJustification(ETextJustify::Center);
				{
					UVerticalBoxSlot* S = LocalIRCenterPanel->AddChildToVerticalBox(OutValue);
					S->SetHorizontalAlignment(HAlign_Center);
					S->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));
				}
			};

			// 上下に余白を入れて、数字が黒い領域の中央寄りに来るようにする。
			{
				UVerticalBoxSlot* S = LocalIRCenterPanel->AddChildToVerticalBox(
					MakeText(WidgetTree, TEXT(""), 1, TextDim));
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}
			AddBigValue(ReaperLang::S(TEXT("回数"), TEXT("Repeats")), LocalIRCountLabel, LocalIRCountValueText);
			AddBigValue(ReaperLang::S(TEXT("間隔"), TEXT("Interval")), LocalIRIntervalLabel, LocalIRIntervalValueText);

			// 再生中の進捗（「2 / 5 回目」）。停止中は空。
			LocalIRProgressText = MakeText(WidgetTree, TEXT(""), 20, AccentBlue, true);
			LocalIRProgressText->SetJustification(ETextJustify::Center);
			{
				UVerticalBoxSlot* S = LocalIRCenterPanel->AddChildToVerticalBox(LocalIRProgressText);
				S->SetHorizontalAlignment(HAlign_Center);
			}
			{
				UVerticalBoxSlot* S = LocalIRCenterPanel->AddChildToVerticalBox(
					MakeText(WidgetTree, TEXT(""), 1, TextDim));
				S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}

			// 間隔スライダー（0〜15秒・1秒刻み）。設定画面のスライダーと同じ見た目にする。
			LocalIRIntervalSlider = WidgetTree->ConstructWidget<USlider>(USlider::StaticClass(), TEXT("LocalIRIntervalSlider"));
			LocalIRIntervalSlider->SetMinValue(0.f);
			LocalIRIntervalSlider->SetMaxValue(15.f);
			LocalIRIntervalSlider->SetStepSize(1.f);
			LocalIRIntervalSlider->MouseUsesStep = true;   // ドラッグも1秒刻みにスナップ
			LocalIRIntervalSlider->SetValue((float)FMath::Clamp(LocalIRIntervalSeconds, 0, 15));
			{
				// スライダー本体：バーは透明にし、丸いつまみだけを見せる（設定画面の WrapSlider と同じ）。
				FSliderStyle SS = LocalIRIntervalSlider->WidgetStyle;
				const FSlateColorBrush ClearBrush(FLinearColor(0.f, 0.f, 0.f, 0.f));
				SS.NormalBarImage   = ClearBrush;
				SS.HoveredBarImage  = ClearBrush;
				SS.DisabledBarImage = ClearBrush;
				const FLinearColor ThumbEdge(0.70f, 1.0f, 0.95f, 1.f);
				SS.NormalThumbImage   = FSlateRoundedBoxBrush(AccentBlue,                             15.f, ThumbEdge, 2.5f, FVector2D(30.f, 30.f));
				SS.HoveredThumbImage  = FSlateRoundedBoxBrush(FLinearColor(0.26f, 0.94f, 0.87f, 1.f), 15.f, ThumbEdge, 2.5f, FVector2D(30.f, 30.f));
				SS.DisabledThumbImage = FSlateRoundedBoxBrush(FLinearColor(0.40f, 0.45f, 0.46f, 1.f), 15.f, FVector2D(30.f, 30.f));
				SS.BarThickness = 14.f;
				LocalIRIntervalSlider->WidgetStyle = SS;

				LocalIRIntervalFill = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());
				{
					const FLinearColor Track(0.055f, 0.10f, 0.115f, 1.f);
					FProgressBarStyle PS = LocalIRIntervalFill->WidgetStyle;
					PS.BackgroundImage = FSlateRoundedBoxBrush(Track,      6.f);
					PS.FillImage       = FSlateRoundedBoxBrush(AccentBlue, 6.f);
					LocalIRIntervalFill->WidgetStyle = PS;
				}
				LocalIRIntervalFill->SetPercent(0.f);

				UOverlay* Ov = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
				{
					USizeBox* TrackBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
					TrackBox->SetHeightOverride(12.f);
					TrackBox->AddChild(LocalIRIntervalFill);
					if (UOverlaySlot* O0 = Ov->AddChildToOverlay(TrackBox))
					{
						O0->SetHorizontalAlignment(HAlign_Fill);
						O0->SetVerticalAlignment(VAlign_Center);
					}
				}
				if (UOverlaySlot* O1 = Ov->AddChildToOverlay(LocalIRIntervalSlider))
				{
					O1->SetHorizontalAlignment(HAlign_Fill);
					O1->SetVerticalAlignment(VAlign_Center);
				}

				USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
				SB->SetMinDesiredHeight(48.f);
				SB->AddChild(Ov);
				UVerticalBoxSlot* S = LocalIRCenterPanel->AddChildToVerticalBox(SB);
				S->SetHorizontalAlignment(HAlign_Fill);
				S->SetPadding(FMargin(8.f, 0.f, 8.f, 4.f));
			}

			UVerticalBoxSlot* S = ListCol->AddChildToVerticalBox(LocalIRCenterPanel);
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
		FooterStateOverlay = StateOverlay;   // ローカルIR中は行ごと隠すため参照を保持
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
			&DeleteTrackButtonIcon, /*bIconOnTop=*/true);
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
			NormalizeButtonText->SetAutoWrapText(!ReaperLang::IsJapanese());   // 日本語は折り返し不要。長い翻訳文字列（独語等）だけボタン幅をはみ出さず2行に折り返す
			ApplyUnwrappedMinWidth(NormalizeButtonText);                       // 折り返しても画面サイズが戻れば1行表示へ復帰できるようにする
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

	// スライダーを「塗り(左=ティール)＋暗いトラック(右)＋丸いつまみ」の見た目にラップするヘルパー。
	// 生成した塗り用 ProgressBar を OutFill で返す（値変更時に RefreshSliderFills で percent を更新する）。
	auto WrapSlider = [&](USlider* Slider, TObjectPtr<UProgressBar>& OutFill) -> UWidget*
	{
		// スライダー本体：バーは透明にし、丸いティールのつまみだけを見せる。
		FSliderStyle SS = Slider->WidgetStyle;
		const FSlateColorBrush ClearBrush(FLinearColor(0.f, 0.f, 0.f, 0.f));
		SS.NormalBarImage   = ClearBrush;
		SS.HoveredBarImage  = ClearBrush;
		SS.DisabledBarImage = ClearBrush;
		const FLinearColor ThumbEdge(0.70f, 1.0f, 0.95f, 1.f);   // つまみの明るいふち
		SS.NormalThumbImage   = FSlateRoundedBoxBrush(AccentBlue,                           15.f, ThumbEdge, 2.5f, FVector2D(30.f, 30.f));
		SS.HoveredThumbImage  = FSlateRoundedBoxBrush(FLinearColor(0.26f, 0.94f, 0.87f, 1.f), 15.f, ThumbEdge, 2.5f, FVector2D(30.f, 30.f));
		SS.DisabledThumbImage = FSlateRoundedBoxBrush(FLinearColor(0.40f, 0.45f, 0.46f, 1.f), 15.f, FVector2D(30.f, 30.f));
		SS.BarThickness = 14.f;
		Slider->WidgetStyle = SS;

		// 塗り：暗いトラック(背景) ＋ ティールの塗り(左) を持つ ProgressBar。
		UProgressBar* Fill = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());
		{
			const FLinearColor Track(0.055f, 0.10f, 0.115f, 1.f);
			FProgressBarStyle PS = Fill->WidgetStyle;
			PS.BackgroundImage = FSlateRoundedBoxBrush(Track,      6.f);
			PS.FillImage       = FSlateRoundedBoxBrush(AccentBlue, 6.f);
			Fill->WidgetStyle = PS;
		}
		Fill->SetPercent(0.f);
		OutFill = Fill;

		UOverlay* Ov = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
		// 細いトラック（塗り）を中央に敷く
		USizeBox* TrackBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		TrackBox->SetHeightOverride(12.f);
		TrackBox->AddChild(Fill);
		if (UOverlaySlot* O0 = Ov->AddChildToOverlay(TrackBox))
		{
			O0->SetHorizontalAlignment(HAlign_Fill);
			O0->SetVerticalAlignment(VAlign_Center);
		}
		// つまみ操作用のスライダーを上に重ねる
		if (UOverlaySlot* O1 = Ov->AddChildToOverlay(Slider))
		{
			O1->SetHorizontalAlignment(HAlign_Fill);
			O1->SetVerticalAlignment(VAlign_Center);
		}
		return Ov;
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

	// この端末のFP（押したときにポップアップで見せる。長いので行には出さない）
	{
		UTextBlock* Label = MakeText(WidgetTree, ReaperLang::S(TEXT("この端末のFP"), TEXT("This device FP")), 22, TextPrimary, true);
		ShowOwnFpButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("表示"), TEXT("Show")), StopGray,
			TEXT("ShowOwnFpButton"), &ShowOwnFpButtonText);
		ShowOwnFpButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleShowOwnFpClicked);
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(160.f); B->SetMinDesiredHeight(56.f); B->AddChild(ShowOwnFpButton);
		AddSettingRow(Label, B);
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

	// 接続先（IP＋ポート＋「検索」ボタン。検索は同ネットワークのReaperを一覧表示して選択接続する）
	{
		IpLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("接続先"), TEXT("Destination")), 22, TextPrimary, true);

		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		UWidget* IpCtrl = MakeFixedInput(TEXT("192.168.1.1"), TEXT("IPAddressInput"), IPAddress, 280.f, IPAddressInput);
		{
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(IpCtrl);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));
		}

		UWidget* PortCtrl = MakeFixedInput(TEXT("23760"), TEXT("PortInput"), FString::FromInt(Port), 140.f, PortInput);
		{
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(PortCtrl);
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

	// --- 認証設定（mTLS）: サーバー管理 / 申請・確認（「接続」に含める）。接続は常に TLS。 ---

	// サーバーFP（選択中サーバーの期待FP。URL登録で入るので表示のみ）
	{
		UTextBlock* Label = MakeText(WidgetTree, ReaperLang::S(TEXT("サーバーFP"), TEXT("Server FP")), 22, TextPrimary, true);
		// 省略せず全部見せる（サーバー画面の値と目で突き合わせるため）。
		// **幅を決め打ちして折り返す。** FP は1行に収まらない長さなので、行の残り幅を
		// 使い切るとラベル「サーバーFP」を押しつぶしてしまう。
		const float FpWidth = 360.f;
		ServerFpText = MakeText(WidgetTree, TEXT("-"), 18, TextDim, false);
		ServerFpText->SetAutoWrapText(true);
		ServerFpText->SetWrapTextAt(FpWidth);
		ServerFpText->SetJustification(ETextJustify::Right);
		USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		SB->SetMinDesiredWidth(FpWidth);
		SB->SetMaxDesiredWidth(FpWidth);
		SB->AddChild(ServerFpText);
		AddSettingRow(Label, SB);
	}

	// 認証（申請 / 確認）
	//   申請 … この端末の公開鍵をサーバーへ提出する（承認はサーバー側の管理者が行う）
	//   確認 … 承認後に本当に繋がるかを試す（疎通確認）
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
		EnrollButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("申請"), TEXT("Request")), StopGray,
			TEXT("EnrollButton"), &EnrollButtonText);
		EnrollButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleEnrollClicked);
		// 「申請中...」を1行で表示する（MakeButton は既定で折り返し有効なので無効化）。
		if (EnrollButtonText) { EnrollButtonText->SetAutoWrapText(false); }
		AddBtn(EnrollButton, 200.f);
		TrialConnectButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("確認"), TEXT("Verify")), AccentBlue,
			TEXT("TrialConnectButton"), &TrialConnectButtonText);
		TrialConnectButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleTrialConnectClicked);
		// 「確認中...」を1行で表示する（同上）。
		if (TrialConnectButtonText) { TrialConnectButtonText->SetAutoWrapText(false); }
		// ボタン幅も広めにする
		AddBtn(TrialConnectButton, 200.f);
		AddSettingRow(Label, H);
	}

	// 状態表示（横幅を確保して1文字ずつ縦折返しにならないようにする）
	{
		UTextBlock* Label = MakeText(WidgetTree, ReaperLang::S(TEXT("状態"), TEXT("Status")), 22, TextPrimary, true);
		AuthStatusText = MakeText(WidgetTree, TEXT("-"), 18, TextDim, false);
		AuthStatusText->SetAutoWrapText(true);
		AuthStatusText->SetJustification(ETextJustify::Right);   // 行の右端（他項目の値）に揃える
		USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		SB->SetMinDesiredWidth(520.f);
		SB->AddChild(AuthStatusText);
		AddSettingRow(Label, SB);
	}

	// 通信時間（直近の計測結果表示＋「測定」ボタン）
	{
		CommTimeRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("通信時間"), TEXT("Comm time")), 22, TextPrimary, true);

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
		UWidget* Ctrl = MakeFixedInput(TEXT("300"), TEXT("MeterIntervalInput"), FString::FromInt(MeterIntervalMs), 160.f, MeterIntervalInput);
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
	// Slack出力の対象（チェックありシートのみ／全シート）
	{
		SlackExportScopeRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("Slack出力"), TEXT("Slack export")), 22, TextPrimary, true);
		SlackExportScopeButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("SlackExportScopeButton"), &SlackExportScopeButtonText);
		UpdateSlackExportScopeButton();
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredWidth(240.f); B->SetMinDesiredHeight(64.f); B->AddChild(SlackExportScopeButton);
		AddSettingRow(SlackExportScopeRowLabel, B);
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

	// --- マーカーオフセット（再生/録音中にマーカーボタンで打つマーカーの、押した瞬間からの前後秒）---
	// 例: -1 なら「押した瞬間の1秒前」にマーカーを置く。範囲±10秒、1秒刻み。
	MarkerOffsetRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("マーカー位置補正"), TEXT("Marker Offset")), 22, TextPrimary, true);
	{
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		MarkerOffsetSlider = WidgetTree->ConstructWidget<USlider>(USlider::StaticClass(), TEXT("MarkerOffsetSlider"));
		MarkerOffsetSlider->SetMinValue(-10.f);
		MarkerOffsetSlider->SetMaxValue(10.f);
		MarkerOffsetSlider->SetStepSize(1.f);
		MarkerOffsetSlider->MouseUsesStep = true;   // ドラッグも1秒刻みにスナップ
		MarkerOffsetSlider->SetValue((float)FMath::Clamp(MarkerOffsetSeconds, -10, 10));
		{
			USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			SB->SetMinDesiredWidth(220.f);
			SB->SetMinDesiredHeight(48.f);
			SB->AddChild(WrapSlider(MarkerOffsetSlider, MarkerOffsetFill));
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(SB);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
		}

		MarkerOffsetValueText = MakeText(WidgetTree, TEXT(""), 22, TextPrimary, true);
		MarkerOffsetValueText->SetJustification(ETextJustify::Center);
		{
			// 値ラベルは固定幅にして、"+10 秒"⇄"0 秒" で幅が変わってもスライダー位置がずれないようにする。
			USizeBox* VB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			VB->SetMinDesiredWidth(90.f);
			VB->SetMaxDesiredWidth(90.f);
			VB->AddChild(MarkerOffsetValueText);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(VB);
			S->SetVerticalAlignment(VAlign_Center);
		}

		AddSettingRow(MarkerOffsetRowLabel, H);
	}
	UpdateMarkerOffsetUI();

	// --- マイク登録（マイク設定ダイアログを開く）---
	// トラック生成に使う設定なので、グループ通話ではなくアプリ側に置く。
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

	// ====== 画面キャプチャ ======
	CaptureSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("画面キャプチャ"), TEXT("Screen Capture")), 18, AccentBlue, true);
	AddSectionHeader(CaptureSectionLabel);

	// --- 画面録画（画面＋モニター音＋マイク音のステレオミックスをMP4でギャラリー保存）---
	// Androidのみ機能。開始時にOS許可ダイアログ→3・2・1カウントダウン→録画。
	ScreenRecRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("画面録画"), TEXT("Screen Recording")), 22, TextPrimary, true);
	ScreenRecButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("録画開始"), TEXT("Record")), AccentBlue,
		TEXT("ScreenRecButton"), &ScreenRecButtonText);
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(200.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(ScreenRecButton);
		AddSettingRow(ScreenRecRowLabel, BtnSize);
	}
	UpdateScreenRecordButton();

	// --- 録画中に経過秒＋停止ボタンを表示するか（ON/OFF）---
	// OFFのときは録画中もオーバーレイを出さず、停止は上の「録画停止」ボタンで行う。
	ScreenRecOverlayRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("録画中の表示"), TEXT("Recording overlay")), 22, TextPrimary, true);
	ScreenRecOverlayButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("ScreenRecOverlayButton"), &ScreenRecOverlayButtonText);
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(160.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(ScreenRecOverlayButton);
		AddSettingRow(ScreenRecOverlayRowLabel, BtnSize);
	}
	UpdateScreenRecOverlayButton();

	// --- 録画にマイク音を含めるか（ON/OFF）---
	ScreenRecMicRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("録画のマイク入力"), TEXT("Recording mic input")), 22, TextPrimary, true);
	ScreenRecMicButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("ScreenRecMicButton"), &ScreenRecMicButtonText);
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(160.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(ScreenRecMicButton);
		AddSettingRow(ScreenRecMicRowLabel, BtnSize);
	}
	UpdateScreenRecMicButton();

	// ====== グループ通話 ======
	GroupTalkSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("グループ通話"), TEXT("Group Talk")), 18, AccentBlue, true);
	AddSectionHeader(GroupTalkSectionLabel);

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

	// --- グループトーク送信音量（送信ゲイン倍率。0〜10を1刻み。既定3）---
	// マイク取り込み(FAudioCapture)は生レベルで小さめなので、送る前に倍率で持ち上げる。
	// これで全受信者＆その録画が一律で大きくなる（受信側ゲインより根本的）。1=素通し / 0=無音。
	GroupTalkSendVolumeRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("グループトーク送信音量"), TEXT("Group talk send vol")), 22, TextPrimary, true);
	{
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		GroupTalkSendVolumeSlider = WidgetTree->ConstructWidget<USlider>(USlider::StaticClass(), TEXT("GroupTalkSendVolumeSlider"));
		// 目盛りは0〜10の10段階。実ゲインは「目盛り×0.5」で最大5.0倍。
		// 高ゲインだと送信時にハードクリップして音が割れるため、上限は5倍に抑えている。
		GroupTalkSendVolumeSlider->SetMinValue(0.f);
		GroupTalkSendVolumeSlider->SetMaxValue(10.f);
		GroupTalkSendVolumeSlider->SetStepSize(1.f);
		GroupTalkSendVolumeSlider->MouseUsesStep = true;
		GroupTalkSendVolumeSlider->SetValue(GroupTalkSendGain * 2.f);
		{
			USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			SB->SetMinDesiredWidth(220.f);
			SB->SetMinDesiredHeight(48.f);
			SB->AddChild(WrapSlider(GroupTalkSendVolumeSlider, GroupTalkSendVolumeFill));
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(SB);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
		}
		GroupTalkSendVolumeValueText = MakeText(WidgetTree, FString::Printf(TEXT("%d"), FMath::RoundToInt(GroupTalkSendGain * 2.f)), 22, TextPrimary, true);
		{
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(GroupTalkSendVolumeValueText);
			S->SetVerticalAlignment(VAlign_Center);
		}
		AddSettingRow(GroupTalkSendVolumeRowLabel, H);
	}

	// --- グループトーク受信音量（再生メイクアップゲイン倍率。0〜10を1刻み。既定1=素通し）---
	// 通常は送信音量で調整するため既定は1.0。自分の受信だけ更に上げたい時に使う。
	GroupTalkVolumeRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("グループトーク受信音量"), TEXT("Group talk recv vol")), 22, TextPrimary, true);
	{
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		GroupTalkVolumeSlider = WidgetTree->ConstructWidget<USlider>(USlider::StaticClass(), TEXT("GroupTalkVolumeSlider"));
		GroupTalkVolumeSlider->SetMinValue(0.f);
		GroupTalkVolumeSlider->SetMaxValue(10.f);
		GroupTalkVolumeSlider->SetStepSize(0.1f);          // 正規化0.1 = 1刻み（範囲0〜10）
		GroupTalkVolumeSlider->MouseUsesStep = true;        // ドラッグも1刻みにスナップ
		GroupTalkVolumeSlider->SetValue(GroupTalkPlaybackGain);
		{
			USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			SB->SetMinDesiredWidth(220.f);
			SB->SetMinDesiredHeight(48.f);
			SB->AddChild(WrapSlider(GroupTalkVolumeSlider, GroupTalkVolumeFill));
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(SB);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
		}

		// 現在値の数値表示（1刻みの整数）
		GroupTalkVolumeValueText = MakeText(WidgetTree, FString::Printf(TEXT("%d"), FMath::RoundToInt(GroupTalkPlaybackGain)), 22, TextPrimary, true);
		{
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(GroupTalkVolumeValueText);
			S->SetVerticalAlignment(VAlign_Center);
		}

		AddSettingRow(GroupTalkVolumeRowLabel, H);
	}

	// --- 送信プレビュー（実際の送信と同じNS＋ゲインの音を折り返し再生。要イヤホン）---
	// 押すと、相手に届くのと同じ「ノイズ抑制が入った処理後の音」が自分に折り返される。
	MicCheckRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("マイク送信チェック"), TEXT("Mic send check")), 22, TextPrimary, true);
	MicCheckButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("MicCheckButton"), &MicCheckButtonText);
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(200.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(MicCheckButton);
		AddSettingRow(MicCheckRowLabel, BtnSize);
	}
	UpdateMicCheckButton();

	// ====== モニター ======
	MonitorSectionLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("モニター"), TEXT("Monitor")), 18, AccentBlue, true);
	AddSectionHeader(MonitorSectionLabel);

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

	// --- モニターの受信方式（PARAKNOT / ReaStream）---
	// ReaStream は従来方式。PARAKNOT が使えない現場のための退避として残す。
	MonitorSourceRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("モニター方式"), TEXT("Monitor source")), 22, TextPrimary, true);
	MonitorSourceButton = MakeButton(WidgetTree, TEXT(""), AccentBlue, TEXT("MonitorSourceButton"), &MonitorSourceButtonText);
	MonitorSourceButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMonitorSourceClicked);
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(200.f);
		BtnSize->SetMinDesiredHeight(64.f);
		BtnSize->AddChild(MonitorSourceButton);
		AddSettingRow(MonitorSourceRowLabel, BtnSize);
	}
	UpdateMonitorSourceButton();

	// --- 配信元（モニターで聞く PARAKNOT の既定）---
	//
	// **選んでおくとモニター開始時のポップアップが出なくなる。**
	// 「自動」のままなら従来どおり毎回選ぶ。
	// Label はマシン単位で全部同じなので、ポートと ch 数で見分ける。
	{
		MonitorInstanceRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("配信元"), TEXT("Broadcast source")), 22, TextPrimary, true);
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		MonitorInstanceCombo = MakeDarkCombo(WidgetTree);
		MonitorInstanceCombo->OnSelectionChanged.AddDynamic(this, &UWG_RadNodzToolkit::HandleMonitorInstanceChanged);
		// **項目を白文字にする。** 既定は黒で、暗い背景だと読めない
		// （サーバーIDコンボと同じ生成関数を使い回す）。
		MonitorInstanceCombo->OnGenerateWidgetEvent.BindDynamic(this, &UWG_RadNodzToolkit::MakeServerIdComboItem);
		{
			USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			SB->SetMinDesiredWidth(260.f);
			SB->SetMinDesiredHeight(56.f);
			SB->AddChild(MonitorInstanceCombo);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(SB);
			S->SetVerticalAlignment(VAlign_Center);
		}

		MonitorInstanceRefreshButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("更新"), TEXT("Refresh")), StopGray,
			TEXT("MonitorInstanceRefreshButton"), &MonitorInstanceRefreshButtonText);
		MonitorInstanceRefreshButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMonitorInstanceRefreshClicked);
		{
			USizeBox* BB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			BB->SetMinDesiredWidth(110.f); BB->SetMinDesiredHeight(56.f); BB->AddChild(MonitorInstanceRefreshButton);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(BB);
			S->SetPadding(FMargin(8.f, 0.f, 0.f, 0.f)); S->SetVerticalAlignment(VAlign_Center);
		}

		AddSettingRow(MonitorInstanceRowLabel, H);
	}
	RefreshMonitorInstanceCombo();

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

	// --- モニター受信音量（端末側でReaperからの受信音を増減。±12dB、1dB刻み。既定+5dB）---
	MonitorVolumeRowLabel = MakeText(WidgetTree, ReaperLang::S(TEXT("受信音量"), TEXT("Receive Volume")), 22, TextPrimary, true);
	{
		UHorizontalBox* H = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		MonitorVolumeSlider = WidgetTree->ConstructWidget<USlider>(USlider::StaticClass(), TEXT("MonitorVolumeSlider"));
		MonitorVolumeSlider->SetMinValue(-12.f);
		MonitorVolumeSlider->SetMaxValue(12.f);
		MonitorVolumeSlider->SetStepSize(1.f);
		MonitorVolumeSlider->MouseUsesStep = true;   // ドラッグも1dB刻みにスナップ
		MonitorVolumeSlider->SetValue((float)FMath::Clamp(FMath::RoundToInt(MonitorVolumeDb), -12, 12));
		{
			USizeBox* SB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			SB->SetMinDesiredWidth(220.f);
			SB->SetMinDesiredHeight(48.f);
			SB->AddChild(WrapSlider(MonitorVolumeSlider, MonitorVolumeFill));
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(SB);
			S->SetVerticalAlignment(VAlign_Center);
			S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
		}

		MonitorVolumeValueText = MakeText(WidgetTree, TEXT(""), 22, TextPrimary, true);
		MonitorVolumeValueText->SetJustification(ETextJustify::Center);
		{
			// 値ラベルは固定幅にして、"+12 dB"⇄"0 dB" で幅が変わってもスライダー位置がずれないようにする。
			USizeBox* VB = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			VB->SetMinDesiredWidth(90.f);
			VB->SetMaxDesiredWidth(90.f);
			VB->AddChild(MonitorVolumeValueText);
			UHorizontalBoxSlot* S = H->AddChildToHorizontalBox(VB);
			S->SetVerticalAlignment(VAlign_Center);
		}

		AddSettingRow(MonitorVolumeRowLabel, H);
	}
	UpdateMonitorVolumeUI();

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

	// ch順マッピングは常にカスタム固定。カスタム配置エディタのみを出す。
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
		// 表の上を左右になぞっても横スクロールできるようにする。
		// Slateのタッチは「内側→外側」に伝播し、ScrollBoxは既定でイベントを消費(Handled)するため、
		// 内側の縦スクロールが指の動きを全部食べてしまい外側の横スクロールに届かない
		//（＝スクロールバーを直接つままないと横に動かせない）。内側だけ消費をやめると、
		// 縦にしきい値を超えれば内側が、横に超えれば外側が掴む＝指なぞりで両方向に動く。
		VScroll->SetConsumePointerInput(false);
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
	// （空欄行自体は読み込み時点で既に除外済み。一致度による非表示は行わない：
	//   台本一致を全シート対象で実行すると0%一致の行まで消え、リストが空に見えるため）
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
	UTextBlock* T = MakeText(WidgetTree, Item, 18, FieldText, false);
	ComboGeneratedItems.Add(T);   // GC回収防止（コンボ内Slateだけが使うためUPROPERTY連鎖に乗らない）
	return T;
}

UWidget* UWG_RadNodzToolkit::MakeServerIdComboItem(FString Item)
{
	// 暗い背景のコンボ用に白文字の項目を作る（ドロップダウン項目・選択表示の両方に適用）。
	// **サーバーIDと配信元で共用。**
	UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	T->SetText(FText::FromString(Item));
	T->SetFont(ReaperFont::Get(18, false));
	T->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	ComboGeneratedItems.Add(T);   // GC回収防止
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
	if (ListLoadPendingFrames > 0) { return; }   // 読み込み待機中の二重押しは無視

	// 「読み込み中・・・」を先に出す。
	// ここで SetText しても、そのまま読み込みに入ると画面には出ない（テキストは次の描画で
	// 反映されるが、その前に同期RPCでゲームスレッドが止まってしまうため）。そこでフラグだけ
	// 立てて一旦抜け、1フレーム描画させてから NativeTick が StartListLoad() を呼ぶ。
	if (ListStatusText) { ListStatusText->SetText(FText::FromString(ReaperLang::S(TEXT("読み込み中・・・"), TEXT("Loading...")))); }
	ListLoadPendingFrames = 2;
}

void UWG_RadNodzToolkit::StartListLoad()
{
	// ここはゲームスレッドで同期に読む。
	// 【やってはいけないこと】ワーカースレッドへ逃がして「読み込み中」をアニメさせること。
	// Excel/GSheetのRPCラッパは内部で NewObject する（UXLDocument / UGSheetConnection）ため
	// GT以外からは呼べない。GCを止めて（FGCScopeGuard）回避すると、読み込み中に
	// GCが要求されたときゲームスレッドがGCロック待ちで完全停止し、操作不能になる。
	TArray<FListSheet> Sheets;
	FString Err;
	const bool bOk = (ListFormat == 1) ? ReadListFromCsv(Sheets, Err)
		: (ListFormat == 2) ? ReadListFromGSheet(Sheets, Err)
		: ReadListFromExcel(Sheets, Err);

	FinishListLoad(bOk, MoveTemp(Sheets), Err);
}

void UWG_RadNodzToolkit::FinishListLoad(bool bOk, TArray<FListSheet>&& Sheets, const FString& Error)
{
	if (!bOk)
	{
		if (ListStatusText) { ListStatusText->SetText(FText::FromString(Error)); }
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
	// IdStart以降で最初に現れる区切り文字を探す（FindCharは先頭から探すため
	// "https://" 等に含まれる"/"に引っかかる。探索開始位置を指定できるFindを使う）。
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
		// Reaper側（拡張 > RIGDOCKS > GOLD > SpreadSheet > id:）にクライアントIDを入れ忘れていると
		// ここで認証に進めず失敗する。原因が分かりにくく実際につまづいたので専用の文言を出す。
		const int32 Code = Client->GetError().ErrorCode;
		const bool bLooksLikeClientIdMissing =
			(Code == (int32)AZ_ErrorCode::GSheetAuthFailed) ||
			(Code == (int32)AZ_ErrorCode::GSheetConnectFailed) ||
			(Code == (int32)AZ_ErrorCode::GSheetNotConnected);

		OutError = bLooksLikeClientIdMissing
			? ReaperLang::S(TEXT("Reaper側の設定にクライアントIDが入れられていません（拡張 > RIGDOCKS > GOLD > SpreadSheet > id:）"),
			                TEXT("Client ID is not set on the Reaper side (Extensions > RIGDOCKS > GOLD > SpreadSheet > id:)"))
			: ReaperLang::S(TEXT("スプレッドシートに接続できません"), TEXT("Cannot connect to spreadsheet"));
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

			// このシートの全セル書き込みをRpcBatchで1往復にまとめる。
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

			// このシートの全セル書き込みをRpcBatchで1往復にまとめる。
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

	// 収録結果を組む。
	// Slackのメッセージは等幅ではないので桁は揃えない（コードブロックは文字が小さく
	// 読みにくいため使わない）。✅/⬜ ＋ 全列を「 | 」で繋ぐ。
	const int32 MaxChars = 3500;   // Slackは1通あたり約4000字で切られるので、余裕を見て分割する

	TArray<FString> Messages;
	FString Cur = ReaperLang::S(TEXT("収録リスト結果"), TEXT("Recording list results")) + TEXT("\n");

	auto PushMessage = [&]()
	{
		if (!Cur.IsEmpty()) { Messages.Add(Cur); }
		Cur.Empty();
	};

	int32 Done = 0, Total = 0;

	for (const FListSheet& Sh : ListSheets)
	{
		// Slack出力の対象：0=チェックが1つ以上あるシートのみ / 1=全シート
		if (SlackExportScope == 0)
		{
			bool bAnyChecked = false;
			for (const FListRow& R : Sh.Rows)
			{
				if (R.bChecked) { bAnyChecked = true; break; }
			}
			if (!bAnyChecked) { continue; }
		}

		Cur += FString::Printf(TEXT("\n[%s]\n"), *Sh.SheetName);

		for (const FListRow& R : Sh.Rows)
		{
			++Total; if (R.bChecked) { ++Done; }

			// 空セルは詰める（末尾の空列で「 | 」だけが並ぶのを防ぐ）
			FString Cells;
			for (int32 c = 0; c < R.Cells.Num(); ++c)
			{
				const FString Cell = R.Cells[c].TrimStartAndEnd();
				if (Cell.IsEmpty()) { continue; }
				if (!Cells.IsEmpty()) { Cells += TEXT(" | "); }
				Cells += Cell;
			}

			const FString Line = FString::Printf(TEXT("%s %s\n"), R.bChecked ? TEXT("✅") : TEXT("⬜"), *Cells);

			// 1通に収まらなくなったらそこで送信を分ける（行の途中では切らない）
			if (Cur.Len() + Line.Len() > MaxChars)
			{
				PushMessage();
				Cur = FString::Printf(TEXT("[%s] %s\n"), *Sh.SheetName,
					*ReaperLang::S(TEXT("続き"), TEXT("continued")));
			}
			Cur += Line;
		}
	}

	if (Total == 0)
	{
		if (ListStatusText)
		{
			ListStatusText->SetText(FText::FromString(ReaperLang::S(
				TEXT("送るシートがありません（チェック済みの行がない）"),
				TEXT("No sheet to send (no checked rows)"))));
		}
		return;
	}

	Cur += FString::Printf(TEXT("\n%s: %d / %d"), *ReaperLang::S(TEXT("収録済み"), TEXT("Done")), Done, Total);
	PushMessage();

	// 分割した場合は順に投稿する（1通でも失敗したら失敗扱い）。
	// 失敗時はSlackが返した文言をそのまま画面に出す（原因が分からないと直せないため）。
	bool bOk = true;
	FString FailReason;
	for (const FString& Msg : Messages)
	{
		FSlackMessage Res = URigdocksSilver_Slack::AZ_Slack_PostDirectMessage(Client, SlackToken, SlackToUser, Msg, false);
		if (!Res.Error.IsEmpty() || Res.TimeStamp.IsEmpty())
		{
			bOk = false;
			FailReason = Res.Error.IsEmpty() ? ReaperLang::S(TEXT("応答なし"), TEXT("no response")) : Res.Error;
			UE_LOG(LogTemp, Warning, TEXT("[Slack] post failed: %s (len=%d)"), *FailReason, Msg.Len());
			break;
		}
	}

	if (ListStatusText)
	{
		ListStatusText->SetText(FText::FromString(bOk
			? ReaperLang::S(TEXT("Slackへ送信しました"), TEXT("Sent to Slack"))
			: ReaperLang::S(TEXT("Slack送信に失敗: "), TEXT("Slack send failed: ")) + FailReason));
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

	// 前回ぶんの文字起こしが返るまで次へ進まない。
	if (bScriptMatchRpcInFlight) return;

	// 1件につきメディア1件だけ文字起こしする。
	//
	// 文字起こしはサーバー側で数十秒〜数分かかる**最も重い往復**なので、
	// 解析用の2本目の接続（HeavyQueue）で流し、操作系の往復を待たせない。
	UMediaItem* Item = ScriptMatchPendingItems[ScriptMatchJobCursor];

	if (!Item)
	{
		++ScriptMatchJobCursor;
		return;
	}

	URigdocksRpcCall* LengthCall = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemLength_Build(Item);
	URigdocksRpcCall* TextCall = URigdocksSilver_TRSC_Build::AZ_TRSC_FullForMediaItem_Build(TrscContext, Item, TEXT("ja"));

	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	Batch->Add(LengthCall);
	Batch->Add(TextCall);

	const FString EntryName = ScriptMatchPendingNames[ScriptMatchJobCursor];
	const double EntryStart = ScriptMatchPendingStarts[ScriptMatchJobCursor];

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	bScriptMatchRpcInFlight = true;

	const bool bQueued = ExecuteBatchAsync(Batch, { LengthCall, TextCall, Item, TrscContext },
		NAME_None, /*bHeavy=*/true,
		[WeakThis, LengthCall, TextCall, EntryName, EntryStart]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			FScriptMatchTranscript Entry;
			Entry.Name = EntryName;
			Entry.Start = EntryStart;
			Entry.Length = URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemLength_Parse(LengthCall);
			Entry.Text = URigdocksSilver_TRSC_Parse::AZ_TRSC_FullForMediaItem_Parse(TextCall);
			Self->ScriptMatchTranscriptions.Add(Entry);

			++Self->ScriptMatchJobCursor;
			Self->bScriptMatchRpcInFlight = false;

			if (Self->ListStatusText)
			{
				Self->ListStatusText->SetText(FText::FromString(FString::Printf(TEXT("%s（%d/%d）"),
					*ReaperLang::S(TEXT("文字起こし中…"), TEXT("Transcribing…")),
					Self->ScriptMatchJobCursor, Self->ScriptMatchPendingItems.Num())));
			}
		});

	if (!bQueued)
	{
		bScriptMatchRpcInFlight = false;
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

void UWG_RadNodzToolkit::HandleSlackExportScopeClicked()
{
	SlackExportScope = (SlackExportScope + 1) % 2;   // チェックありシートのみ ⇄ 全シート
	UpdateSlackExportScopeButton();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateSlackExportScopeButton()
{
	if (SlackExportScopeButtonText)
	{
		SlackExportScopeButtonText->SetText(ReaperLang::T(
			SlackExportScope == 1 ? TEXT("全て") : TEXT("チェックありシート"),
			SlackExportScope == 1 ? TEXT("All")  : TEXT("Checked sheets")));
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

	// 同期RPCを流す専用スレッドを先に用意する（以後のRPCはすべてここへ積む）。
	EnsureRpcQueues();

	// バックグラウンド中の定期メンテナンス（在席ハートビート等）を登録する。
	// 監視スレッドから、ゲームスレッド停止中にのみ呼ばれる（GC停止中のためWeakPtr解決は安全）。
	FReaperKeepAlive::SetBackgroundMaintenance(
		[WeakThis = TWeakObjectPtr<UWG_RadNodzToolkit>(this)]()
		{
			if (UWG_RadNodzToolkit* Self = WeakThis.Get())
			{
				Self->BackgroundKeepAliveMaintenance();
			}
		});

	// 前回の接続設定(IP/Port)を読み込んで入力欄へ反映
	LoadSettings();

	// 保存データが無い初回起動でも、既定（自動更新）の状態をボタン表示へ反映しておく
	UpdateTrackRefreshModeButton();
	UpdateUpdateButton();
	// ローカルIRの回数/間隔（保存値）を表示へ反映する。画面自体は通常のホームから始める。
	UpdateLocalIRUI();

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
	if (ScreenRecButton && !ScreenRecButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleScreenRecordClicked))
	{
		ScreenRecButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleScreenRecordClicked);
	}
	if (ScreenRecOverlayButton && !ScreenRecOverlayButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleScreenRecOverlayToggleClicked))
	{
		ScreenRecOverlayButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleScreenRecOverlayToggleClicked);
	}
	if (ScreenRecMicButton && !ScreenRecMicButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleScreenRecMicToggleClicked))
	{
		ScreenRecMicButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleScreenRecMicToggleClicked);
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
	if (ActionRunButton && !ActionRunButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleActionRunClicked))
	{
		ActionRunButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleActionRunClicked);
	}
	if (ActionCombo && !ActionCombo->OnSelectionChanged.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleActionComboChanged))
	{
		ActionCombo->OnSelectionChanged.AddDynamic(this, &UWG_RadNodzToolkit::HandleActionComboChanged);
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
	if (TrackTabButton && !TrackTabButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleTrackTabClicked))
	{
		TrackTabButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleTrackTabClicked);
	}
	if (MarkerTabButton && !MarkerTabButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMarkerTabClicked))
	{
		MarkerTabButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMarkerTabClicked);
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
	if (GroupTalkVolumeSlider)
	{
		if (!GroupTalkVolumeSlider->OnValueChanged.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkVolumeChanged))
			GroupTalkVolumeSlider->OnValueChanged.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkVolumeChanged);
		if (!GroupTalkVolumeSlider->OnMouseCaptureEnd.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkVolumeCaptureEnd))
			GroupTalkVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkVolumeCaptureEnd);
	}
	if (GroupTalkSendVolumeSlider)
	{
		if (!GroupTalkSendVolumeSlider->OnValueChanged.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkSendVolumeChanged))
			GroupTalkSendVolumeSlider->OnValueChanged.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkSendVolumeChanged);
		if (!GroupTalkSendVolumeSlider->OnMouseCaptureEnd.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleGroupTalkSendVolumeCaptureEnd))
			GroupTalkSendVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &UWG_RadNodzToolkit::HandleGroupTalkSendVolumeCaptureEnd);
	}
	if (MonitorVolumeSlider)
	{
		if (!MonitorVolumeSlider->OnValueChanged.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMonitorVolumeChanged))
			MonitorVolumeSlider->OnValueChanged.AddDynamic(this, &UWG_RadNodzToolkit::HandleMonitorVolumeChanged);
		if (!MonitorVolumeSlider->OnMouseCaptureEnd.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMonitorVolumeCaptureEnd))
			MonitorVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &UWG_RadNodzToolkit::HandleMonitorVolumeCaptureEnd);
	}
	if (MarkerOffsetSlider)
	{
		if (!MarkerOffsetSlider->OnValueChanged.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMarkerOffsetChanged))
			MarkerOffsetSlider->OnValueChanged.AddDynamic(this, &UWG_RadNodzToolkit::HandleMarkerOffsetChanged);
		if (!MarkerOffsetSlider->OnMouseCaptureEnd.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMarkerOffsetCaptureEnd))
			MarkerOffsetSlider->OnMouseCaptureEnd.AddDynamic(this, &UWG_RadNodzToolkit::HandleMarkerOffsetCaptureEnd);
	}
	// --- ローカルIR（モード切替 / ＋－ / 間隔スライダー）---
	if (LocalIRButton && !LocalIRButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleLocalIRClicked))
	{
		LocalIRButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleLocalIRClicked);
	}
	if (LocalIRPlusButton && !LocalIRPlusButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleLocalIRPlusClicked))
	{
		LocalIRPlusButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleLocalIRPlusClicked);
	}
	if (LocalIRMinusButton && !LocalIRMinusButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleLocalIRMinusClicked))
	{
		LocalIRMinusButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleLocalIRMinusClicked);
	}
	if (LocalIRIntervalSlider)
	{
		if (!LocalIRIntervalSlider->OnValueChanged.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleLocalIRIntervalChanged))
			LocalIRIntervalSlider->OnValueChanged.AddDynamic(this, &UWG_RadNodzToolkit::HandleLocalIRIntervalChanged);
		if (!LocalIRIntervalSlider->OnMouseCaptureEnd.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleLocalIRIntervalCaptureEnd))
			LocalIRIntervalSlider->OnMouseCaptureEnd.AddDynamic(this, &UWG_RadNodzToolkit::HandleLocalIRIntervalCaptureEnd);
	}

	if (MicCheckButton && !MicCheckButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMicCheckClicked))
	{
		MicCheckButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMicCheckClicked);
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
	if (MicTransportButton && !MicTransportButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleMicTransportClicked))
	{
		MicTransportButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleMicTransportClicked);
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
	if (SlackExportScopeButton && !SlackExportScopeButton->OnClicked.IsAlreadyBound(this, &UWG_RadNodzToolkit::HandleSlackExportScopeClicked))
		SlackExportScopeButton->OnClicked.AddDynamic(this, &UWG_RadNodzToolkit::HandleSlackExportScopeClicked);
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
	// ローカルIR表示中に接続を押したら、まず通常のホーム画面へ戻す（隠していたボタン類を出す）。
	if (bLocalIRMode) { SetLocalIRMode(false); }

	// ボタン操作は明示的な意思表示なので、自動再接続の予約と試行回数をリセットする。
	bAutoReconnectPending = false;
	ConnAutoReconnectTries = 0;

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
	// ポートは未記入/不正なら既定の23760を使う。確定した値を入力欄にも反映して保存する。
	Port = ResolvePortFromInput();
	if (PortInput) { PortInput->SetText(FText::FromString(FString::FromInt(Port))); }
	StoreConnToSelectedServer();   // 接続先を選択中 serverId に紐づけて保存
	SaveSettings();   // 接続設定を保存（次回起動時に復元）
	UpdateHeaderIpText();
	Connect();
}

int32 UWG_RadNodzToolkit::ResolvePortFromInput() const
{
	const FString PortStr = PortInput ? PortInput->GetText().ToString().TrimStartAndEnd() : FString();
	if (PortStr.IsNumeric())
	{
		const int32 V = FCString::Atoi(*PortStr);
		if (V > 0 && V <= 65535) { return V; }
	}
	return 23760;   // 未記入/不正時の既定ポート
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

	// 接続先デバイス情報を取得する。
	// 5つの独立した取得をRpcBatchで1往復にまとめ、
	// 通信はワーカー、値の取り出しと表示更新はゲームスレッドで行う。
	UReaProject* Proj = GetProject();

	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<UObject*> KeepAlive;

	URigdocksRpcCall* CallSampleRate = nullptr;
	URigdocksRpcCall* CallBitDepth   = nullptr;
	if (Proj)
	{
		CallSampleRate = URigdocksSilver_Project_Build::AZ_GetRecordingSampleRate_Build(Proj);
		CallBitDepth   = URigdocksSilver_Project_Build::AZ_GetRecordingBitDepth_Build(Proj);
		Batch->Add(CallSampleRate);
		Batch->Add(CallBitDepth);
		KeepAlive.Add(CallSampleRate);
		KeepAlive.Add(CallBitDepth);
	}
	URigdocksRpcCall* CallInputCh  = URigdocksSilver_Device_Build::AZ_CountInputChannel_Build();
	URigdocksRpcCall* CallOutputCh = URigdocksSilver_Device_Build::AZ_CountOutputChannel_Build();
	URigdocksRpcCall* CallDevName  = URigdocksSilver_Device_Build::AZ_GetDeviceName_Build();
	Batch->Add(CallInputCh);
	Batch->Add(CallOutputCh);
	Batch->Add(CallDevName);
	KeepAlive.Add(CallInputCh);
	KeepAlive.Add(CallOutputCh);
	KeepAlive.Add(CallDevName);

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	ExecuteBatchAsync(Batch, KeepAlive, TEXT("deviceinfo"), /*bHeavy=*/false,
		[WeakThis, CallSampleRate, CallBitDepth, CallInputCh, CallOutputCh, CallDevName]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			if (CallSampleRate && CallBitDepth)
			{
				Self->RecSampleRate = URigdocksSilver_Project_Parse::AZ_GetRecordingSampleRate_Parse(CallSampleRate);
				Self->RecBitDepth   = NormalizeBitDepth(URigdocksSilver_Project_Parse::AZ_GetRecordingBitDepth_Parse(CallBitDepth));
			}
			Self->InputChCount  = URigdocksSilver_Device_Parse::AZ_CountInputChannel_Parse(CallInputCh);
			Self->OutputChCount = URigdocksSilver_Device_Parse::AZ_CountOutputChannel_Parse(CallOutputCh);
			Self->RemoteDeviceName = URigdocksSilver_Device_Parse::AZ_GetDeviceName_Parse(CallDevName);

			Self->RefreshDeviceInfoText();

			// 相手端末名がヘッダーにも出るよう反映する。
			Self->UpdateHeaderIpText();
		});
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

// =====================================================================
//  画面録画（画面＋モニター音＋マイク音のステレオミックス→MP4→ギャラリー保存）
//  ・Androidのみ。開始時にOSのMediaProjection許可ダイアログが必須。
//  ・許可後にアプリ側で3・2・1カウントダウンし、その後に実キャプチャを開始する。
// =====================================================================

void UWG_RadNodzToolkit::HandleScreenRecordClicked()
{
	switch (ScreenRecState)
	{
	case EScreenRecState::Idle:
		// 許可ダイアログを出し、結果待ちへ。結果は TickScreenRecord で拾う。
		FScreenRecorder::RequestPermission();
		ScreenRecState = EScreenRecState::WaitingPermission;
		break;

	case EScreenRecState::WaitingPermission:
	case EScreenRecState::CountingDown:
		// 開始準備中に再タップされたらキャンセル扱いにする。
		HideScreenRecCountdown();
		ScreenRecState = EScreenRecState::Idle;
		break;

	case EScreenRecState::Recording:
		// 録画中なら停止＝MP4確定・ギャラリー保存。経過秒／停止ボタンのオーバーレイも消す。
		FScreenRecorder::Stop();
		FScreenRecorder::HideRecordingOverlay();
		ScreenRecState = EScreenRecState::Idle;
		break;
	}
	UpdateScreenRecordButton();
}

void UWG_RadNodzToolkit::TickScreenRecord(float InDeltaTime)
{
	switch (ScreenRecState)
	{
	case EScreenRecState::WaitingPermission:
	{
		const int32 Perm = FScreenRecorder::GetPermissionState();
		if (Perm == 1)
		{
			// 許可された → カウントダウン開始
			ScreenRecCountdownRemain = ScreenRecCountdownSec;
			ScreenRecShownNumber = -1;   // 次のフレームで必ず表示更新させる
			ScreenRecState = EScreenRecState::CountingDown;
			UpdateScreenRecordButton();
		}
		else if (Perm == -1)
		{
			// 拒否された → 元へ
			ScreenRecState = EScreenRecState::Idle;
			UpdateScreenRecordButton();
		}
		break;
	}

	case EScreenRecState::CountingDown:
	{
		ScreenRecCountdownRemain -= InDeltaTime;
		if (ScreenRecCountdownRemain <= 0.0)
		{
			// カウントダウン終了 → 実キャプチャ開始（マイクを含めるかは設定に従う）。
			HideScreenRecCountdown();
			FScreenRecorder::BeginCapture(bScreenRecIncludeMic);
			// 設定が「録画中の表示=ON」のときだけ、右上に経過秒＋停止ボタンを出す。
			// OFFのときは何も出さず、停止は設定の「録画停止」ボタンで行う。
			if (bScreenRecShowOverlay)
			{
				FScreenRecorder::ShowRecordingOverlay();
			}
			ScreenRecState = EScreenRecState::Recording;
			ScreenRecStartGrace = 0.0;   // 起動猶予の計測を開始
			UpdateScreenRecordButton();
		}
		else
		{
			const int32 Num = FMath::Max(1, FMath::CeilToInt((float)ScreenRecCountdownRemain));
			if (Num != ScreenRecShownNumber)
			{
				ScreenRecShownNumber = Num;
				ShowScreenRecCountdown(Num);
			}
		}
		break;
	}

	case EScreenRecState::Recording:
		// 起動直後は前面サービスが IsRecording()=true を立てるまで数百ms待つ必要がある。
		// 猶予中は外部停止判定をせず、UIを録画中に保つ（即Idleへ戻る競合を防ぐ）。
		ScreenRecStartGrace += InDeltaTime;
		if (ScreenRecStartGrace >= ScreenRecStartGraceSec)
		{
			// 猶予後：システムの「共有を停止」やオーバーレイの停止ボタン等で
			// 外部から止められたら状態を戻し、オーバーレイも消す。
			if (!FScreenRecorder::IsRecording())
			{
				FScreenRecorder::HideRecordingOverlay();
				ScreenRecState = EScreenRecState::Idle;
				UpdateScreenRecordButton();
			}
		}
		break;

	default:
		break;
	}
}

void UWG_RadNodzToolkit::UpdateScreenRecordButton()
{
	using namespace ReaperCtrlUI;
	if (!ScreenRecButton || !ScreenRecButtonText) { return; }

	FString Label;
	FLinearColor Bg;
	switch (ScreenRecState)
	{
	case EScreenRecState::Recording:
		Label = ReaperLang::S(TEXT("録画停止"), TEXT("Stop"));
		Bg = RecRed;
		break;
	case EScreenRecState::WaitingPermission:
		Label = ReaperLang::S(TEXT("許可待ち…"), TEXT("Waiting…"));
		Bg = StopGray;
		break;
	case EScreenRecState::CountingDown:
		Label = ReaperLang::S(TEXT("開始中…"), TEXT("Starting…"));
		Bg = StopGray;
		break;
	case EScreenRecState::Idle:
	default:
		Label = ReaperLang::S(TEXT("録画開始"), TEXT("Record"));
		Bg = AccentBlue;
		break;
	}
	ScreenRecButton->SetBackgroundColor(Bg);
	ScreenRecButtonText->SetText(FText::FromString(Label));
	ScreenRecButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
}

void UWG_RadNodzToolkit::ShowScreenRecCountdown(int32 Number)
{
	using namespace ReaperCtrlUI;

	// 初回に全画面オーバーレイ（半透明黒＋巨大な数字）を生成する。
	if (!ScreenRecCountdownOverlay)
	{
		ScreenRecCountdownOverlay = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ScreenRecCountdownOverlay"));
		ScreenRecCountdownOverlay->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.6f));
		ScreenRecCountdownOverlay->SetHorizontalAlignment(HAlign_Fill);
		ScreenRecCountdownOverlay->SetVerticalAlignment(VAlign_Fill);
		ScreenRecCountdownOverlay->SetPadding(FMargin(0.f));
		// カウントダウン中も下のRIGDREDを操作して録画開始位置へ移動・待機できるよう、
		// オーバーレイ（と子の数字）はヒットテスト対象外にしてタッチを透過させる。
		ScreenRecCountdownOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);

		ScreenRecCountdownText = MakeText(WidgetTree, TEXT("3"), 160, FLinearColor::White, true);
		ScreenRecCountdownText->SetJustification(ETextJustify::Center);

		// 中央寄せ
		UBorder* Inner = ScreenRecCountdownOverlay;
		Inner->SetHorizontalAlignment(HAlign_Center);
		Inner->SetVerticalAlignment(VAlign_Center);
		Inner->SetContent(ScreenRecCountdownText);
	}

	if (ScreenRecCountdownText)
	{
		ScreenRecCountdownText->SetText(FText::AsNumber(Number));
	}

	// ビューポート最前面へ追加（未追加のときのみ）。
	if (!bScreenRecOverlayInViewport && GEngine && GEngine->GameViewport)
	{
		ScreenRecOverlaySlate = ScreenRecCountdownOverlay->TakeWidget();
		GEngine->GameViewport->AddViewportWidgetContent(ScreenRecOverlaySlate.ToSharedRef(), 1000);
		bScreenRecOverlayInViewport = true;
	}
}

void UWG_RadNodzToolkit::HideScreenRecCountdown()
{
	if (bScreenRecOverlayInViewport && GEngine && GEngine->GameViewport && ScreenRecOverlaySlate.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(ScreenRecOverlaySlate.ToSharedRef());
	}
	bScreenRecOverlayInViewport = false;
	ScreenRecOverlaySlate.Reset();
}

void UWG_RadNodzToolkit::HandleScreenRecOverlayToggleClicked()
{
	bScreenRecShowOverlay = !bScreenRecShowOverlay;
	UpdateScreenRecOverlayButton();
	// 録画中に切り替えられたら、現在の表示状態も即反映する。
	if (ScreenRecState == EScreenRecState::Recording)
	{
		if (bScreenRecShowOverlay) { FScreenRecorder::ShowRecordingOverlay(); }
		else                       { FScreenRecorder::HideRecordingOverlay(); }
	}
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateScreenRecOverlayButton()
{
	if (ScreenRecOverlayButtonText)
	{
		ScreenRecOverlayButtonText->SetText(FText::FromString(bScreenRecShowOverlay
			? ReaperLang::S(TEXT("ON"), TEXT("ON"))
			: ReaperLang::S(TEXT("OFF"), TEXT("OFF"))));
	}
}

void UWG_RadNodzToolkit::HandleScreenRecMicToggleClicked()
{
	bScreenRecIncludeMic = !bScreenRecIncludeMic;
	UpdateScreenRecMicButton();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateScreenRecMicButton()
{
	if (ScreenRecMicButtonText)
	{
		ScreenRecMicButtonText->SetText(FText::FromString(bScreenRecIncludeMic
			? ReaperLang::S(TEXT("ON"), TEXT("ON"))
			: ReaperLang::S(TEXT("OFF"), TEXT("OFF"))));
	}
}

void UWG_RadNodzToolkit::HandleMonitorClicked()
{
	// 停止は方式を問わず共通。
	if (IsMonitorActive())
	{
		StopMonitor();
	}
	else if (MonitorSource == EMonitorSource::Paraknot)
	{
		// **判定は「モニター方式」。** bUseParaknotMic(音声経路)はマイク送信の設定で、
		// モニターの向きとは別物。
		//
		// PARAKNOT 経路は、開始までに業務TCPの往復（一覧 → 役割 → セッション）が
		// 要る。**ここでは投げるだけ**で、実際の開始は応答が返ってから。
		StartParaknotFlow(EParaknotPurpose::Monitor);

		UpdateMonitorButton();   // 「準備中」を出す
		return;
	}
	else
	{
		if (!AudioMonitor)
		{
			AudioMonitor = NewObject<UReaStreamReceiver>(this);
		}

		// 出力方法（ステレオ / 立体音響 / そのまま）を反映してから受信開始
		ConfigureAudioMonitor();
		// 接続中のReaper以外からのReaStreamパケットは無視する（混線・音割れ対策）。
		// ※ PARAKNOT 方式では鍵が合わないパケットは届かないので効かない(害も無い)。
		AudioMonitor->SetAllowedSenderIP(IPAddress);

		AudioMonitor->Start(this, ReaStreamPort);
	}

	UpdateMonitorButton();
	UpdateKeepAlive();   // モニターの開始/停止に合わせてバックグラウンド継続を切替
}

bool UWG_RadNodzToolkit::IsMonitorActive() const
{
	return (AudioMonitor && AudioMonitor->IsActive())
		|| (ParaknotMonitor && ParaknotMonitor->IsActive());
}

void UWG_RadNodzToolkit::StopMonitor()
{
	// **どちらも止める。** 設定を切り替えた直後は、前の方式の受信機が
	// 動いたままになっていることがある。
	if (AudioMonitor) { AudioMonitor->Stop(); }
	if (ParaknotMonitor)   { ParaknotMonitor->Stop(); }
}

// 受信レベルのメーターを塗り直す。
//
// **dB 目盛にする。** 線形のままだと会話程度の音量(-30dB 前後)でバーが
// ほとんど動かず、「届いていない」のと見分けが付かない。-60dB〜0dB を
// 0..1 に写して、静かな音でも振れるようにする。
void UWG_RadNodzToolkit::UpdateMonitorLevelMeter()
{
	if (!MonitorLevelMeter) return;

	const float Linear = (ParaknotMonitor && ParaknotMonitor->IsActive()) ? ParaknotMonitor->GetMeterLevel() : 0.f;

	float Percent = 0.f;

	if (Linear > 0.000001f)
	{
		const float Db = 20.f * FMath::LogX(10.f, Linear);   // 0..1 -> -inf..0 dB
		Percent = FMath::Clamp((Db + 60.f) / 60.f, 0.f, 1.f);
	}

	MonitorLevelMeter->SetPercent(Percent);

	// **振り切れたら色で知らせる。** 0dB 近辺はクリップしている可能性がある。
	using namespace ReaperCtrlUI;
	MonitorLevelMeter->SetFillColorAndOpacity(Percent > 0.98f ? WarnRed : OkGreen);
}

void UWG_RadNodzToolkit::UpdateMonitorButton()
{
	using namespace ReaperCtrlUI;

	const bool bOn = IsMonitorActive();

	// PARAKNOT 経路は押してすぐには始まらない（相手との往復がある）ので、
	// その間は別の色で「準備中」を示す（マイクと同じ扱い）。
	const bool bStarting = bParaknotMicStarting
		&& (ParaknotPurpose == EParaknotPurpose::Monitor);

	if (MonitorButton)
	{
		const FLinearColor Bg = bOn ? RecRed : (bStarting ? AccentBlue : StopGray);   // ON色はマイクと統一
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
	// 停止は方式を問わず共通。
	if (IsMicSendActive())
	{
		StopMicSend();
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
		// PARAKNOT 方式は、開始までに業務TCPの往復（一覧 → 役割 → セッション）が
		// 要る。**ここでは投げるだけ**で、実際の開始は応答が返ってから。
		if (bUseParaknotMic)
		{
			StartParaknotFlow(EParaknotPurpose::Mic);

			UpdateMicSendButton();   // 「準備中」を出す
			return;
		}

		if (!MicSender)
		{
			MicSender = NewObject<UReaStreamSender>(this);
		}

		// 送信先＝接続中のReaperのIP（接続パネルのIP）
		FString Target = IPAddress;
		if (IPAddressInput) { Target = IPAddressInput->GetText().ToString(); }
		MicSender->Start(Target, ReaStreamPort, MicInputMode);
	}

	// マイクキャプチャの開始/停止でオーディオデバイスが再初期化され、モニター再生が止まることがある。
	// モニター受信中なら再起動して再生を復帰させる（送信しながらでもモニターが聞こえるように）。
	RestartMonitorIfActive();

	UpdateMicSendButton();
	UpdateKeepAlive();   // マイク送信の開始/停止に合わせてバックグラウンド継続を切替
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
	else if (RigMicSender && RigMicSender->IsActive())
	{
		// PARAKNOT 側は鍵を握り直す必要があるので、セッションから張り直す
		// （相手は既に受信・待受なので確認は挟まらない）。
		RigMicSender->Stop();
		StartParaknotFlow(EParaknotPurpose::Mic);
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

	// モニター受信中なら新しい出力設定で即時反映する（ReaStream / PARAKNOT とも）
	RestartMonitorIfActive();
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

FMonitorOutputSettings UWG_RadNodzToolkit::MakeMonitorOutputSettings() const
{
	FMonitorOutputSettings Settings;
	Settings.Mode = MonitorOutputMode;
	Settings.BinItdMs = ItdMsByLevel(SpatialItdLevel);
	Settings.BinShadowAlpha = ShadowAlphaByLevel(SpatialShadowLevel);
	Settings.ChannelOrder = SpatialChannelOrder;
	Settings.CustomAzimuths = CustomAzimuths;
	Settings.CustomDistances = CustomDistances;
	Settings.HrtfProfileName = HrtfProfile;
	Settings.DownmixGains = DmGains;
	return Settings;
}

void UWG_RadNodzToolkit::ConfigureAudioMonitor()
{
	if (!AudioMonitor) return;
	AudioMonitor->SetOutputSettings(MakeMonitorOutputSettings());
	AudioMonitor->SetBufferSeconds(BufferSecondsByLevel(MonitorBufferLevel));
	// 端末側の受信音量(dB)を線形ゲインに変換して反映する。
	AudioMonitor->SetPlaybackGain(FMath::Pow(10.f, (float)MonitorVolumeDb / 20.f));
}

void UWG_RadNodzToolkit::HandleMonitorSourceClicked()
{
	MonitorSource = (MonitorSource == EMonitorSource::Paraknot)
		? EMonitorSource::ReaStream
		: EMonitorSource::Paraknot;

	UpdateMonitorSourceButton();
	SaveSettings();

	// 受信中なら新しい方式で張り直す。
	RestartMonitorIfActive();
}

void UWG_RadNodzToolkit::UpdateMonitorSourceButton()
{
	if (MonitorSourceButtonText)
	{
		MonitorSourceButtonText->SetText(FText::FromString(
			MonitorSource == EMonitorSource::Paraknot ? TEXT("PARAKNOT") : TEXT("ReaStream")));
	}
}

// 配信元の候補を取り直す。**接続していないと取れない**(一覧はサーバー越し)。
//
// **ポップアップと同じ ParaknotRpc を使う。** 別経路で引くと、同じ画面の
// 2 箇所が違う一覧を見ることになる。
void UWG_RadNodzToolkit::HandleMonitorInstanceRefreshClicked()
{
	if (!Client || !bIsConnected)
	{
		MonitorInstances.Reset();
		RefreshMonitorInstanceCombo();
		return;
	}

	URadnodzClient* C = Client;
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	// **業務TCPは背景スレッドで。** ゲームスレッドで待つと画面が止まる。
	EnqueueControlRpc(NAME_None, [WeakThis, C]()
	{
		TArray<FParaknotInstance> Instances;
		FString Error;

		const bool bOk = ParaknotRpc::ListInstances(C, Instances, Error);

		AsyncTask(ENamedThreads::GameThread, [WeakThis, Instances, Error, bOk]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			if (bOk)
			{
				Self->MonitorInstances = Instances;
			}
			else
			{
				Self->MonitorInstances.Reset();
				UE_LOG(LogTemp, Warning, TEXT("[Monitor] 配信元の一覧を取得できません: %s"), *Error);
			}

			Self->RefreshMonitorInstanceCombo();
		});
	});
}

void UWG_RadNodzToolkit::HandleMonitorInstanceChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// プログラム的な選択（初期化/復元）は無視し、ユーザー操作のみ反映する。
	// BuildUI 中の RefreshMonitorInstanceCombo は LoadSettings より前に走るため、
	// ここで保存すると未読込の既定値でセーブデータ全体（登録サーバー等）を上書きしてしまう。
	if (SelectionType == ESelectInfo::Direct) return;

	// 先頭は「毎回選ぶ」。それ以外は MonitorInstances の添字と 1 ずれで対応する。
	const int32 Index = MonitorInstanceCombo ? MonitorInstanceCombo->GetSelectedIndex() : 0;

	// **覚えるのは待受ポート。** InstanceId はプラグインを挿し直すたびに
	// 変わる(起動時に乱数で採る)ので、既定として保存しても次回必ず外れる。
	// ポートは利用者が決める値で、プロジェクトを開き直しても変わらない。
	MonitorListenPort = (Index >= 1 && MonitorInstances.IsValidIndex(Index - 1))
		? MonitorInstances[Index - 1].ListenPort
		: 0;

	SaveSettings();
	RestartMonitorIfActive();
}

// 候補の表示名。ポップアップの行と同じ書き方にする。
FString UWG_RadNodzToolkit::MakeMonitorInstanceLabel(const FParaknotInstance& Instance)
{
	const FString Name = Instance.Label.IsEmpty() ? Instance.InstanceId : Instance.Label;

	const FString PortText = (Instance.ListenPort > 0)
		? FString::FromInt(Instance.ListenPort)
		: ReaperLang::S(TEXT("ポートなし"), TEXT("no port"));

	return FString::Printf(TEXT("%s (%s)  %dch"),
		*PortText, *Name, Instance.MainChannelCount);
}

// 候補を ComboBox へ流し込む。**保存してある選択は、一覧に無くても項目として残す**
// (まだ更新していない / 相手が落ちている)。黙って「毎回選ぶ」に戻すと、
// 設定したつもりが効いていないことに気づけない。
void UWG_RadNodzToolkit::RefreshMonitorInstanceCombo()
{
	if (!MonitorInstanceCombo) return;

	MonitorInstanceCombo->ClearOptions();
	// 先頭は「毎回選ぶ」。保存値が 0 のときの表示でもある。
	MonitorInstanceCombo->AddOption(ReaperLang::S(TEXT("毎回選ぶ"), TEXT("Ask each time")));

	int32 ToSelect = 0;
	bool  bFound = false;

	for (int32 i = 0; i < MonitorInstances.Num(); ++i)
	{
		MonitorInstanceCombo->AddOption(MakeMonitorInstanceLabel(MonitorInstances[i]));

		if (MonitorListenPort > 0 && MonitorInstances[i].ListenPort == MonitorListenPort)
		{
			ToSelect = i + 1;
			bFound = true;
		}
	}

	if (MonitorListenPort > 0 && !bFound)
	{
		MonitorInstanceCombo->AddOption(FString::Printf(TEXT("%d (?)"), MonitorListenPort));
		ToSelect = MonitorInstanceCombo->GetOptionCount() - 1;
	}

	MonitorInstanceCombo->SetSelectedIndex(ToSelect);
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
		// ※ PARAKNOT 方式では鍵が合わないパケットは届かないので効かない(害も無い)。
		AudioMonitor->SetAllowedSenderIP(IPAddress);

		AudioMonitor->Start(this, ReaStreamPort);
	}

	// **PARAKNOT 経路は購読を張り直さない。** 鍵を取り直すところからやり直しになり、
	// マイクの開始のたびに業務TCPの往復が増えるため。出力設定を渡し直すと
	// 再生（UE 側の音源）だけが作り直される。
	if (ParaknotMonitor && ParaknotMonitor->IsActive())
	{
		ParaknotMonitor->SetOutputSettings(MakeMonitorOutputSettings());
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
	ComboGeneratedItems.Add(T);   // GC回収防止（回収されるとセクション初表示時にクラッシュする）
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

void UWG_RadNodzToolkit::ResolveGroupTalkFolder()
{
	if (!Client || !bIsConnected) return;

	// 名簿フォルダは RIGDOCKS Bridge フォルダと同じ階層の RadNodzToolkit/GroupTalk に置く
	// （REAPER が未起動でも解決できるようにするため）。
	//   Windows : %APPDATA%/RadNodzToolkit/GroupTalk
	//   macOS   : ~/Library/Application Support/RadNodzToolkit/GroupTalk
	// 親フォルダは接続先（Bridge が動いている PC）の OS・ユーザーで決まるため、
	// SILVER の AZ_GetAppDataPath（= RIGDOCKS Bridge フォルダの親）で問い合わせる。
	// アプリ固有のフォルダ名はサーバーに持たせず、ここで連結する。
	// フォルダは AZ_WriteFile（ハートビート書き込み）が無ければ作る。

	// 同期 RPC なのでゲームスレッドでは呼ばず、他の RPC と同じ制御用キューで実行する。
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);
	URadnodzClient* RpcClient = Client;

	EnqueueControlRpc(TEXT("resolvegtfolder"), [WeakThis, RpcClient]()
	{
		if (!IsValid(RpcClient)) { return; }

		const FString ParentPath = URigdocksSilver_File::AZ_GetAppDataPath(RpcClient);
		const FRigdocksError Err = RpcClient->GetError();
		const bool bOk = (Err.ErrorCode == 0) && !ParentPath.IsEmpty();
		const FString Error = bOk ? FString()
			: FString::Printf(TEXT("AZ_GetAppDataPath が失敗しました（エラーコード %d: %s）"),
				Err.ErrorCode, Err.Details.IsEmpty() ? *Err.Message : *Err.Details);

		AsyncTask(ENamedThreads::GameThread, [WeakThis, RpcClient, bOk, ParentPath, Error]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self || Self->Client != RpcClient) { return; }   // 切断・再接続で古くなった結果は捨てる

			if (!bOk)
			{
				// 旧版の Bridge（関数が無い）など。保存済みのフォルダがあればそれを使い続ける。
				UE_LOG(LogTemp, Warning, TEXT("[RIGDRED GroupTalk] グループトーク用フォルダを解決できませんでした: %s（保存済み: %s）"),
					*Error, Self->GetEffectivePresenceFolder().IsEmpty() ? TEXT("なし") : *Self->GetEffectivePresenceFolder());
				return;
			}

			FString Resolved = ParentPath;
			Resolved.ReplaceInline(TEXT("\\"), TEXT("/"));
			Resolved.RemoveFromEnd(TEXT("/"));
			Resolved += TEXT("/RadNodzToolkit/GroupTalk");

			const bool bChanged = (Self->ResolvedPresenceFolder != Resolved);
			Self->ResolvedPresenceFolder = Resolved;

			if (bChanged)
			{
				// 端末に保存して、次回は接続前でもパスを保持・表示できるようにする
				Self->SaveSettings();

				// 解決前に参加していたら、新しいフォルダで在席を書き直して名簿も読み直す
				if (Self->IsGroupTalkChannelActive(0) || Self->IsGroupTalkChannelActive(1))
				{
					Self->DispatchGroupTalkPresenceAsync(true, true);
				}
			}
		});
	});
}

bool UWG_RadNodzToolkit::BuildPresenceOfflineWrite(FString& OutFolder, FString& OutFile1,
	FString& OutFile2, FString& OutContent) const
{
	// 在席オフライン書き込みの中身を組み立てるだけ（通信はしない）。
	// メンバー変数を読むのでゲームスレッドから呼ぶこと。
	const FString Folder0 = GetEffectivePresenceFolder();

	if (IntercomUserName.IsEmpty() || Folder0.IsEmpty())
	{
		return false;
	}

	const FString MyIP = URadnodzUtility::AZ_GetLocalIP();

	OutContent = FString::Printf(TEXT("%s\t%s\t0"), *IntercomUserName, *MyIP);

	OutFolder = Folder0;
	OutFolder.ReplaceInline(TEXT("\\"), TEXT("/"));

	OutFile1 = SanitizePresenceName(IntercomUserName) + TEXT(".grouptalk1.txt");
	OutFile2 = SanitizePresenceName(IntercomUserName) + TEXT(".grouptalk2.txt");

	return true;
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

		// 横向きは縦の余白を全体的に詰める（左列がスクロールなしに収まりやすくする）。
		// ① 上部パネル（接続/GTALK・ツールバー・デバイス情報）の上下余白を縮める。
		if (ActionPanel)     { ActionPanel->SetPadding(FMargin(28.f, 10.f)); }
		if (ToolbarPanel)    { ToolbarPanel->SetPadding(FMargin(28.f, 10.f)); }
		if (DeviceInfoPanel) { DeviceInfoPanel->SetPadding(FMargin(28.f, 6.f)); }
		// ② フッター：状態表示(■停止中)の上の余白だけ詰める（上を小さく／下は据え置き）。
		if (FooterPanel)     { FooterPanel->SetPadding(FMargin(28.f, 10.f, 28.f, 22.f)); }
		// ③ 再生／録音ボタンの間隔を詰める（PlayButton右側パディング 16→6）。
		if (PlayButtonSize)
		{
			if (UHorizontalBoxSlot* PS = Cast<UHorizontalBoxSlot>(PlayButtonSize->Slot))
			{
				PS->SetPadding(FMargin(0.f, 0.f, 6.f, 0.f));
			}
		}

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

		// 縦向きは横向きで詰めた余白を元へ戻す（縦横を切り替えても崩れないように）。
		if (ActionPanel)     { ActionPanel->SetPadding(FMargin(28.f, 18.f)); }
		if (ToolbarPanel)    { ToolbarPanel->SetPadding(FMargin(28.f, 18.f)); }
		if (DeviceInfoPanel) { DeviceInfoPanel->SetPadding(FMargin(28.f, 10.f)); }
		if (FooterPanel)     { FooterPanel->SetPadding(FMargin(28.f, 22.f)); }
		if (PlayButtonSize)
		{
			if (UHorizontalBoxSlot* PS = Cast<UHorizontalBoxSlot>(PlayButtonSize->Slot))
			{
				PS->SetPadding(FMargin(0.f, 0.f, 16.f, 0.f));
			}
		}

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

	// 向きごとの高さ・余白を入れ直したあとに、ローカルIR用の表示/非表示とボタンサイズを被せる。
	ApplyLocalIRLayout(bLandscape);
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

	// 在席ハートビート＋名簿更新は同期RPC。ゲームスレッドで呼ぶと（定期ワーカーと同一
	// ソケットで衝突し）接続破損＋ブロックでフリーズするため、ワーカーへ回す。
	DispatchGroupTalkPresenceAsync(true, true);
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
	DispatchGroupTalkPresenceAsync(true, false);   // 同期RPCはワーカーへ（ゲームスレッドをブロックしない）
	ApplyTransmitRouting();
	UpdateGroupTalkUI();
}

void UWG_RadNodzToolkit::GroupTalkOnSendPressed(int32 Ch)
{
	if (Ch != GroupTalkSubCh()) return;    // サブchの長押しのみPTT（マスターはOnClickedで処理）
	bGroupTalkSubHeld = true;
	DispatchGroupTalkPresenceAsync(true, false);   // 同期RPCはワーカーへ（ゲームスレッドをブロックしない）
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

	// 送信は UReaStreamSender（FAudioCapture）で行う。
	if (Target < 0)
	{
		FGroupTalkMic::Stop();                                                             // マイク送信チェックが動いていれば止める
		if (GroupTalkSender && GroupTalkSender->IsActive()) { GroupTalkSender->Stop(); }   // マイク解放
		UpdateKeepAlive();   // 送信停止に合わせてバックグラウンド継続を切替
		return;
	}

	TArray<FString> MemberIPs;
	GroupTalkMembers[Target].GenerateValueArray(MemberIPs);
	const int32 TxPort = GroupTalkPortForCh(Target);

	FGroupTalkMic::Stop();   // マイク送信チェックが動いていれば止める（マイクの奪い合いを防ぐ）
	EnsureGroupTalkSenderStarted();
	if (!GroupTalkSender) { UpdateKeepAlive(); return; }
	GroupTalkSender->SetGroupTargets(MemberIPs, TxPort);   // 宛先IP＋ポートを排他切替

	UpdateKeepAlive();   // 送信開始に合わせてバックグラウンド継続を切替
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
	GroupTalkSender->SetSendGain(GroupTalkSendGain);   // 送信音量設定を反映

	// グループトークは参加者全員へ同じ音を配るフルメッシュなので、無音の端末まで
	// 送り続けると台数の2乗でWi-Fiを圧迫し、遅延の原因になる。喋っていない間は送信を止める。
	// （Reaperへマイクを送る MicSender 側はOFFのまま。録音に穴があくため）
	GroupTalkSender->SetDtxEnabled(true);
}

void UWG_RadNodzToolkit::StartGroupTalkReceiver(int32 Ch)
{
	if (Ch < 0 || Ch > 1) return;
	if (!bIsConnected) return;
	if (!GroupTalkReceiver[Ch]) { GroupTalkReceiver[Ch] = NewObject<UReaStreamGroupReceiver>(this); }
	if (GroupTalkReceiver[Ch]->IsActive()) { GroupTalkReceiver[Ch]->Stop(); }
	GroupTalkReceiver[Ch]->Start(this, GroupTalkPortForCh(Ch));
	GroupTalkReceiver[Ch]->SetPlaybackGain(GroupTalkPlaybackGain);   // 現在の音量設定を反映

	TArray<FString> MemberIPs;
	GroupTalkMembers[Ch].GenerateValueArray(MemberIPs);
	GroupTalkReceiver[Ch]->SetAllowedMembers(MemberIPs);

	UpdateKeepAlive();   // 受信開始に合わせてバックグラウンド継続を切替
}

void UWG_RadNodzToolkit::StopGroupTalkReceiver(int32 Ch)
{
	if (Ch < 0 || Ch > 1) return;
	if (GroupTalkReceiver[Ch] && GroupTalkReceiver[Ch]->IsActive()) { GroupTalkReceiver[Ch]->Stop(); }
	UpdateKeepAlive();   // 受信停止に合わせてバックグラウンド継続を切替
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

void UWG_RadNodzToolkit::DispatchGroupTalkPresenceAsync(bool bHeartbeat, bool bRoster)
{
	// ★ゲームスレッドから同期RPCを呼んではいけない★（電波が悪いとそのまま
	// ブロックし、ANRフリーズになる。実機ANRで確認済み）。
	// ボタン操作起因の在席ハートビート/名簿更新も、定期処理と同じ制御用キューへ
	// 合流させ、ゲームスレッドは絶対にブロックさせない。UI更新だけGTへ戻す。
	if (!bIsConnected || !Client) { return; }
	if (!bHeartbeat && !bRoster)  { return; }

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	EnqueueControlRpc(NAME_None, [WeakThis, bHeartbeat, bRoster]()
	{
		if (UWG_RadNodzToolkit* Self = WeakThis.Get())
		{
			if (Self->bIsConnected && Self->Client)
			{
				if (bHeartbeat) { Self->WriteGroupTalkHeartbeat(0); Self->WriteGroupTalkHeartbeat(1); }
				if (bRoster)    { Self->RefreshGroupTalkRosterBackground(); }
			}
		}
		AsyncTask(ENamedThreads::GameThread, [WeakThis]()
		{
			if (UWG_RadNodzToolkit* S = WeakThis.Get()) { S->UpdateGroupTalkUI(); }
		});
	});
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

// ====================================================================
//  バックグラウンド継続（画面OFF/他アプリ表示でも音声・通話を続ける）
// ====================================================================

void UWG_RadNodzToolkit::UpdateKeepAlive()
{
	// キープアライブ（前面サービス＋監視スレッド）を有効にする条件：
	//   ・Reaperに接続中             → 画面OFF中もサーバー側タイムアウトで
	//                                   制御接続(TCP)が切れないよう心拍を打ち続ける
	//   ・モニター受信/マイク送信/グループトークが動作中 → 音声を継続させる
	// いずれかが該当する間だけ有効化する（すべて非該当なら停止してバッテリーを節約）。
	const bool bMonitorOn   = IsMonitorActive();
	const bool bMicOn       = IsMicSendActive();
	const bool bGroupTalkOn = bGroupTalkRecvOn[0] || bGroupTalkRecvOn[1]
		|| (ComputeTransmitTarget() >= 0)
		|| (GroupTalkSender && GroupTalkSender->IsActive());
	FReaperKeepAlive::SetActive(bIsConnected || bMonitorOn || bMicOn || bGroupTalkOn);
}

void UWG_RadNodzToolkit::BackgroundKeepAliveMaintenance()
{
	// ★監視スレッドから呼ばれる★（UI操作・UObject生成禁止）。
	// ゲームスレッドが止まっている間の定期通信を肩代わりする。
	if (!bIsConnected || !Client) return;

	// 【重要】ここは他スレッドと同じ制御接続(TCP)を使う。
	// RPCは自分で実行せず、制御用キュー（1本のスレッドで順番に実行）へ積む。
	// 監視スレッドから直接RPCを流すと、ゲームスレッドや在席更新と同時に同じソケットへ
	// 書き込んでリクエストが混線し、サーバー側が接続を切るため。
	if (!ControlQueue) { return; }

	const double Now = FPlatformTime::Seconds();

	// 接続キープ（グループトーク非使用でも実行）：
	// 画面OFF中はNativeTickが止まり、Reaperへ何も送らなくなるため、サーバー側の
	// タイムアウトで制御接続(TCP)が切られてしまう（＝復帰時に切断状態へ戻る）。
	// これを防ぐため、軽量RPCを定期的に1往復させて接続を維持する。
	if (Now - BgConnKeepLast.Load() >= BgConnKeepIntervalSec)
	{
		BgConnKeepLast.Store(Now);

		URadnodzClient* KeepAliveClient = Client;

		ControlQueue->EnqueueUnique(TEXT("keepalive"), [KeepAliveClient]()
		{
			if (IsValid(KeepAliveClient)) { KeepAliveClient->SendTimingProbe(); }
		});
	}

	// 在席ハートビートを書き続けないと、他の参加者の名簿から自分が消えて
	// 相手からの送信が止まってしまう（＝画面を消すと聞こえなくなる）ため、
	// グループトーク使用中はここで肩代わりする。
	const bool bNeedGroupTalk = !IntercomUserName.IsEmpty()
		&& (IsGroupTalkChannelActive(0) || IsGroupTalkChannelActive(1));

	if (bNeedGroupTalk)
	{
		const bool bDoHeartbeat = (Now - GtBgHeartbeatLast.Load() >= 5.0);   // 前面時のNativeTickと同じ5秒周期
		const bool bDoRoster    = (Now - GtBgRosterLast.Load() >= 3.0);      // 前面時と同じ3秒周期

		if (bDoHeartbeat) { GtBgHeartbeatLast.Store(Now); }
		if (bDoRoster)    { GtBgRosterLast.Store(Now); }

		if (bDoHeartbeat || bDoRoster)
		{
			TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

			ControlQueue->EnqueueUnique(TEXT("gtpresence"), [WeakThis, bDoHeartbeat, bDoRoster]()
			{
				UWG_RadNodzToolkit* Self = WeakThis.Get();
				if (!Self || !Self->bIsConnected || !Self->Client) { return; }

				if (bDoHeartbeat) { Self->WriteGroupTalkHeartbeat(0); Self->WriteGroupTalkHeartbeat(1); }
				if (bDoRoster)    { Self->RefreshGroupTalkRosterBackground(); }
			});
		}
	}
}

void UWG_RadNodzToolkit::RefreshGroupTalkRosterBackground()
{
	// RefreshGroupTalkRoster のバックグラウンド版。
	// UI更新（UpdateGroupTalkUI）と送信の開始/停止（EnsureGroupTalkSenderStarted等）は
	// ゲームスレッド専用のため行わず、以下のスレッドセーフな更新だけを行う：
	//   - 受信許可IP一覧（UReaStreamGroupReceiver::SetAllowedMembers はロック保護）
	//   - 送信先IP一覧（UReaStreamSender::SetGroupTargets はロック保護。送信中のみ）
	if (!Client || !bIsConnected) return;
	FString Folder = GetEffectivePresenceFolder();
	if (Folder.IsEmpty()) return;
	Folder.ReplaceInline(TEXT("\\"), TEXT("/"));

	GroupTalkMembers[0].Reset();
	GroupTalkMembers[1].Reset();

	const int64 NowSec = FDateTime::UtcNow().ToUnixTimestamp();
	const int64 StaleSec = 15;

	TArray<FString> Files = URigdocksSilver_File::AZ_GetFilePathList(Client, Folder, TEXT("txt"));
	for (const FString& Path : Files)
	{
		FString P = Path;
		P.ReplaceInline(TEXT("\\"), TEXT("/"));
		int32 Ch = -1;
		if      (P.EndsWith(TEXT(".grouptalk1.txt"))) { Ch = 0; }
		else if (P.EndsWith(TEXT(".grouptalk2.txt"))) { Ch = 1; }
		else { continue; }

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
		if (NowSec - Ts > StaleSec) continue;
		if (Name == IntercomUserName) continue;

		GroupTalkMembers[Ch].Add(Name, IP);
	}

	for (int32 Ch = 0; Ch < 2; ++Ch)
	{
		if (GroupTalkReceiver[Ch])
		{
			TArray<FString> MemberIPs;
			GroupTalkMembers[Ch].GenerateValueArray(MemberIPs);
			GroupTalkReceiver[Ch]->SetAllowedMembers(MemberIPs);
		}
	}

	// 送信中なら宛先を最新の参加者へ更新する（開始/停止はしない）。
	const int32 Target = ComputeTransmitTarget();
	if (Target >= 0 && GroupTalkSender && GroupTalkSender->IsActive())
	{
		TArray<FString> MemberIPs;
		GroupTalkMembers[Target].GenerateValueArray(MemberIPs);
		GroupTalkSender->SetGroupTargets(MemberIPs, GroupTalkPortForCh(Target));
	}
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

		// 受信ボタン：OFF=StopGray / ON=BtnGreenDark / ON&マスター=BtnTealMid。
		// マスターch設定は「受信中」ではないため、受信していない間は常にStopGray（無着色）にする。
		// ONの色はいずれも暗めに揃える：右下の受信ドット（緑の点滅）とヘッドホンアイコンが
		// 明るい背景だと埋もれて、受信しているのかどうか判別できなくなるため。
		if (GroupTalkRecvButton[Ch])
		{
			FLinearColor RecvBg = StopGray;
			if (bGroupTalkRecvOn[Ch]) { RecvBg = bIsMaster ? BtnTealMid : BtnGreenDark; }
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

	const bool bOn = IsMicSendActive();
	if (MicSendButton)
	{
		// PARAKNOT 経路は押してすぐには始まらない（相手との往復がある）ので、
		// その間は別の色で「準備中」を示す。無反応に見えると連打される。
		const bool bStarting = bParaknotMicStarting
			&& (ParaknotPurpose == EParaknotPurpose::Mic);

		const FLinearColor Bg = bOn ? RecRed : (bStarting ? AccentBlue : StopGray);
		MicSendButton->SetBackgroundColor(Bg);
		if (MicSendButtonText)
		{
			// ラベルは固定「マイク」（ON/OFFは色で表現）
			MicSendButtonText->SetText(ReaperLang::T(TEXT("マイク"), TEXT("Mic")));
			MicSendButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
		}
	}
}

// ====================================================================
//  マイク送信の方式（ReaStream / PARAKNOT）
// ====================================================================

bool UWG_RadNodzToolkit::IsMicSendActive() const
{
	return (MicSender && MicSender->IsActive())
		|| (RigMicSender && RigMicSender->IsActive());
}

void UWG_RadNodzToolkit::StopMicSend()
{
	// **どちらも止める。** 設定を切り替えた直後は、前の方式の送信機が
	// 動いたままになっていることがある。
	if (MicSender)    { MicSender->Stop(); }
	if (RigMicSender) { RigMicSender->Stop(); }
}

void UWG_RadNodzToolkit::ShowMicMessage(const FString& Message)
{
	UWG_InfoDialog* Dlg = CreateWidget<UWG_InfoDialog>(this, UWG_InfoDialog::StaticClass());
	if (Dlg)
	{
		Dlg->Setup(ReaperLang::S(TEXT("マイク送信"), TEXT("Mic send")), Message);
		Dlg->AddToViewport(200);
	}
}

void UWG_RadNodzToolkit::HandleMicTransportClicked()
{
	// 動いている最中に経路を変えると、どちらで繋がっているのか分からなくなるので
	// **送信もモニターも止める**。
	if (IsMicSendActive() || IsMonitorActive())
	{
		StopMicSend();
		StopMonitor();
		UpdateKeepAlive();
	}

	bUseParaknotMic = !bUseParaknotMic;

	// 前の経路で選んだ相手は引き継がない。
	ParaknotMicInstanceId.Reset();
	ParaknotMicLabel.Reset();

	UpdateMicTransportButton();
	UpdateMicSendButton();
	UpdateMonitorButton();
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateMicTransportButton()
{
	if (MicTransportButtonText)
	{
		MicTransportButtonText->SetText(FText::FromString(bUseParaknotMic
			? TEXT("PARAKNOT")
			: TEXT("ReaStream")));
	}
}

void UWG_RadNodzToolkit::TickParaknotMonitorRecovery(float DeltaSeconds)
{
	// モニターしていないときは判定そのものを持ち越さない。
	// **-1 は「基準がまだ無い」印。** 復号失敗の通算値はモニターを張り直しても
	// 戻らない（AudioLink が生存期間で通算する）ので、0 と比べると繋ぎ直した
	// 直後に「増えた」と誤判定してしまう。
	if (!ParaknotMonitor || !ParaknotMonitor->IsActive())
	{
		ParaknotMonitorLastDecryptFailures = -1;
		ParaknotMonitorNotReceivingSince = 0.0;
		ParaknotMonitorRecoverFailures = 0;
		return;
	}

	ParaknotMonitorCheckTimer += DeltaSeconds;

	if (ParaknotMonitorCheckTimer < ParaknotMonitorCheckIntervalSec)
	{
		return;
	}

	ParaknotMonitorCheckTimer = 0.0f;

	if (bParaknotMonitorRecovering)
	{
		return;   // 取り直しの往復中
	}

	const double Now = FPlatformTime::Seconds();

	const int32 Failures = ParaknotMonitor->GetDecryptFailureCount();

	// 基準が無い（開始直後 / 繋ぎ直した直後）。今の値を基準にして次から見る。
	if (ParaknotMonitorLastDecryptFailures < 0)
	{
		ParaknotMonitorLastDecryptFailures = Failures;
		return;
	}

	// **増えたときだけ「鍵が食い違っている」とみなす。** 通算値なので減ることは
	// 無いが、差分で見ておけば数え方が変わっても誤判定しない。
	const bool bDecryptFailing = (Failures > ParaknotMonitorLastDecryptFailures);

	ParaknotMonitorLastDecryptFailures = Failures;

	if (ParaknotMonitor->IsReceiving())
	{
		ParaknotMonitorNotReceivingSince = 0.0;

		// 音は来ているので、取り直す理由は無い。
		if (!bDecryptFailing)
		{
			return;
		}
	}
	else if (ParaknotMonitorNotReceivingSince == 0.0)
	{
		ParaknotMonitorNotReceivingSince = Now;
	}

	// **鍵が違って捨てている**（相手は送っているのに復号できない）。すぐ取り直す。
	const bool bWantRecover = bDecryptFailing
		// 音が来ないまま時間が経った。原因が分からないときの保険
		// （相手が止まっているだけのこともあるが、その場合は失うものが無い）。
		|| (ParaknotMonitorNotReceivingSince > 0.0
			&& (Now - ParaknotMonitorNotReceivingSince) >= ParaknotMonitorSilenceTimeoutSec);

	if (!bWantRecover)
	{
		return;
	}

	// **立て続けにやり直さない。** 相手が落ちている間は何度やっても戻らないので、
	// 間隔を空けて業務TCPを無駄に往復させない。
	if (ParaknotMonitorLastRecoverTime > 0.0
		&& (Now - ParaknotMonitorLastRecoverTime) < ParaknotMonitorRecoverCooldownSec)
	{
		return;
	}

	RecoverParaknotMonitor();
}

void UWG_RadNodzToolkit::RecoverParaknotMonitor()
{
	if (ParaknotMicInstanceId.IsEmpty() || !Client || !bIsConnected || !ParaknotMonitor)
	{
		return;
	}

	bParaknotMonitorRecovering = true;
	ParaknotMonitorLastRecoverTime = FPlatformTime::Seconds();

	UE_LOG(LogTemp, Warning, TEXT("[PARAKNOT Monitor] 鍵を取り直します"));

	// 音声の宛先は制御と同じマシン（接続パネルのIP）。
	FString Target = IPAddress;
	if (IPAddressInput) { Target = IPAddressInput->GetText().ToString(); }

	URadnodzClient* C = Client;
	const FString InstanceId = ParaknotMicInstanceId;
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	// **役割は変えない。** ここで直したいのは鍵だけで、相手のモードを触ると
	// 利用者に無断で配信を止めることになる。
	EnqueueControlRpc(NAME_None, [WeakThis, C, InstanceId, Target]()
	{
		FParaknotSession Session;
		FString Error;

		bool bOk = ParaknotRpc::OpenOutputSession(C, InstanceId, Session, Error);

		// **駄目なら配信元を選び直す。** InstanceId は相手プロセス限りの値なので、
		// 接続先を変えたり REAPER を開き直したりすると必ず無効になる。
		// そのまま同じ ID で叩き続けても戻らないので、一覧から配信中のものを
		// 拾い直して乗り換える。
		FString PickedId;

		if (!bOk)
		{
			TArray<FParaknotInstance> Instances;
			FString ListError;

			if (ParaknotRpc::ListInstances(C, Instances, ListError))
			{
				for (const FParaknotInstance& Instance : Instances)
				{
					// 配信(送信 + 待受)しているものだけ。受信側から鍵を貰っても音は来ない。
					if (!Instance.CanSendOutput() || Instance.InstanceId == InstanceId)
					{
						continue;
					}

					if (ParaknotRpc::OpenOutputSession(C, Instance.InstanceId, Session, Error))
					{
						PickedId = Instance.InstanceId;
						bOk = true;
						break;
					}
				}
			}
		}

		const int32 Port = Session.Port;
		const FString Key = Session.Key;

		AsyncTask(ENamedThreads::GameThread, [WeakThis, bOk, Port, Key, Target, PickedId]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			Self->bParaknotMonitorRecovering = false;

			// 待っている間に利用者が止めていた。
			if (!Self->ParaknotMonitor || !Self->ParaknotMonitor->IsActive()) { return; }

			// 取れなかった（相手が落ちた / 配信が 1 つも無い）。
			if (!bOk)
			{
				Self->ParaknotMonitorRecoverFailures++;

				// **諦めどきを作る。** ここで「次の周期でまた試す」とだけして
				// おくと、無音タイマーが 20 秒を超えたままなので、クールダウン
				// (5 秒) ごとに業務 TCP を永久に叩き続けることになる。
				if (Self->ParaknotMonitorRecoverFailures >= ParaknotMonitorMaxRecoverFailures)
				{
					UE_LOG(LogTemp, Warning,
						TEXT("[PARAKNOT Monitor] 配信元を見つけられないのでモニターを止めます"));

					Self->StopMonitor();
					Self->UpdateKeepAlive();
					Self->UpdateMonitorButton();

					Self->ShowMicMessage(ReaperLang::S(
						TEXT("配信中のPARAKNOTが見つかりません。接続先と配信元を確認してください。"),
						TEXT("No broadcasting PARAKNOT found. Check the target and the broadcast source.")));

					return;
				}

				// **無音タイマーを引き直す。** 据え置きだと次の周期で即また走る。
				Self->ParaknotMonitorNotReceivingSince = FPlatformTime::Seconds();

				return;
			}

			Self->ParaknotMonitorRecoverFailures = 0;

			// 一覧から選び直していたら、以後はそちらを相手にする。
			if (!PickedId.IsEmpty())
			{
				UE_LOG(LogTemp, Warning,
					TEXT("[PARAKNOT Monitor] 配信元を %s へ切り替えました"), *PickedId);

				Self->ParaknotMicInstanceId = PickedId;
				Self->ParaknotMicLabel.Reset();
			}

			Self->ParaknotMonitor->Stop();

			Self->ParaknotMonitor->SetPlaybackGain(
				FMath::Pow(10.f, (float)Self->MonitorVolumeDb / 20.f));
			Self->ParaknotMonitor->SetOutputSettings(Self->MakeMonitorOutputSettings());

			Self->ParaknotMonitor->Start(Self, Target, Port, Key);

			// 繋ぎ直したので、判定の基準も取り直す（-1 = 次の周期で入れ直す）。
			Self->ParaknotMonitorLastDecryptFailures = -1;
			Self->ParaknotMonitorNotReceivingSince = 0.0;

			Self->UpdateMonitorButton();
		});
	});
}

void UWG_RadNodzToolkit::StartParaknotFlow(EParaknotPurpose Purpose)
{
	if (bParaknotMicStarting)
	{
		return;   // 往復中。連打で二重に投げない
	}

	if (!Client || !bIsConnected)
	{
		ShowMicMessage(ReaperLang::S(
			TEXT("先にReaperへ接続してください。"),
			TEXT("Connect to Reaper first.")));
		return;
	}

	// **この先の選択・確認・発行はすべてこの用途で動く。** 一覧が返るまでに
	// 別の用途で押されることは無い（準備中は二重に投げない）。
	ParaknotPurpose = Purpose;

	bParaknotMicStarting = true;

	URadnodzClient* C = Client;
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	EnqueueControlRpc(NAME_None, [WeakThis, C]()
	{
		TArray<FParaknotInstance> Instances;
		FString Error;

		const bool bOk = ParaknotRpc::ListInstances(C, Instances, Error);

		AsyncTask(ENamedThreads::GameThread, [WeakThis, Instances, Error, bOk]()
		{
			if (UWG_RadNodzToolkit* Self = WeakThis.Get())
			{
				Self->OnParaknotInstancesFetched(Instances, bOk ? FString() : Error);
			}
		});
	});
}

void UWG_RadNodzToolkit::OnParaknotInstancesFetched(const TArray<FParaknotInstance>& Instances,
	const FString& Error)
{
	// ここから先はダイアログ待ちになる。**待っている間は「準備中」を解く。**
	// 利用者が閉じただけで押せなくなると、やり直しができない。
	bParaknotMicStarting = false;
	UpdateMicSendButton();

	if (!Error.IsEmpty())
	{
		ShowMicMessage(Error);
		return;
	}

	if (Instances.Num() == 0)
	{
		ShowMicMessage(ReaperLang::S(
			TEXT("PARAKNOTが見つかりません。スタジオ側でPARAKNOTを起動してください。"),
			TEXT("No PARAKNOT found. Start PARAKNOT on the studio machine.")));
		return;
	}

	ParaknotCandidates = Instances;

	// **モニターは「配信元」の既定があればそれを使い、選ばせない。**
	// 設定タブで選んでおけば毎回聞かれずに済む。「自動」(空)のままなら
	// 従来どおりポップアップで選ぶ。
	//
	// マイク送信には効かせない。あちらの宛先はモニターとは別の相手。
	if (ParaknotPurpose == EParaknotPurpose::Monitor && MonitorListenPort > 0)
	{
		for (int32 i = 0; i < ParaknotCandidates.Num(); ++i)
		{
			if (ParaknotCandidates[i].ListenPort == MonitorListenPort)
			{
				HandleParaknotInstancePicked(i);
				return;
			}
		}

		// 既定のポートで待っているものが居ない(ポートを変えた / 相手が落ちた)。
		// **黙って別のものを聞かせない**ので、選び直してもらう。
		UE_LOG(LogTemp, Warning,
			TEXT("[Monitor] 既定の配信元 (ポート %d) が見つかりません。選び直してください。"),
			MonitorListenPort);
	}

	// 1件なら選ばせない。
	if (ParaknotCandidates.Num() == 1)
	{
		HandleParaknotInstancePicked(0);
		return;
	}

	const bool bMic = (ParaknotPurpose == EParaknotPurpose::Mic);

	TArray<FString> Items;
	TArray<bool>    Dimmed;

	for (const FParaknotInstance& Instance : ParaknotCandidates)
	{
		// 「ポート(名前) チャンネル数」。
		//
		// **Label はマシン単位で全インスタンス同じ**なので、名前だけでは
		// 見分けが付かない。ポートと ch 数が実質の識別子になる。
		// ポートが 0 なのは待受を開けていないとき（未認証 / 音声が未開始）。
		const FString Name = Instance.Label.IsEmpty() ? Instance.InstanceId : Instance.Label;

		const FString PortText = (Instance.ListenPort > 0)
			? FString::FromInt(Instance.ListenPort)
			: ReaperLang::S(TEXT("ポートなし"), TEXT("no port"));

		Items.Add(FString::Printf(TEXT("%s (%s)  %dch"),
			*PortText, *Name, Instance.MainChannelCount));

		// **そのままでは使えないものを暗く出す。** 用途で望ましいモードが
		// 逆になる（モニターは相手が配信、マイク送信は相手が受信待受）。
		const bool bReady = bMic ? Instance.CanReceiveInput() : Instance.CanSendOutput();

		Dimmed.Add(!bReady);
	}

	const FString Title = bMic
		? ReaperLang::S(TEXT("送信先のPARAKNOTを選択"), TEXT("Choose a PARAKNOT to send to"))
		: ReaperLang::S(TEXT("受信元のPARAKNOTを選択"), TEXT("Choose a PARAKNOT to listen to"));

	UWG_ParaknotPickDialog* Dlg = CreateWidget<UWG_ParaknotPickDialog>(this, UWG_ParaknotPickDialog::StaticClass());
	if (Dlg)
	{
		Dlg->Setup(Title, Items, Dimmed);
		Dlg->OnPicked.BindDynamic(this, &UWG_RadNodzToolkit::HandleParaknotInstancePicked);
		Dlg->AddToViewport(200);
	}
}

void UWG_RadNodzToolkit::HandleParaknotInstancePicked(int32 Index)
{
	if (!ParaknotCandidates.IsValidIndex(Index))
	{
		return;
	}

	const FParaknotInstance& Instance = ParaknotCandidates[Index];

	ParaknotMicInstanceId = Instance.InstanceId;
	ParaknotMicLabel = Instance.Label;

	// 用途ごとに「そのまま使えるモード」が違う。
	//   マイク送信 → 相手は受信(待受)でなければならない
	//   モニター   → 相手は送信(待受)でなければならない
	const bool bMic = (ParaknotPurpose == EParaknotPurpose::Mic);
	const bool bReady = bMic ? Instance.CanReceiveInput() : Instance.CanSendOutput();

	if (bReady)
	{
		BeginParaknotSession(false);
		return;
	}

	// **勝手に切り替えない。** 相手のモードを変えると、そのインスタンスが
	// 今やっている送信 / 受信は止まる。
	const FString Name = ParaknotMicLabel.IsEmpty() ? ParaknotMicInstanceId : ParaknotMicLabel;

	// **翻訳文字列を書式にしない。** FString::Printf の書式はリテラルである必要があり、
	// ReaperLang::S の戻りを渡すとコンパイルが通らない。差し込みは名前ひとつなので連結で組む。
	const FString Wanted = bMic
		? ReaperLang::S(TEXT("」は受信(待受)になっていません。\n切り替えますか？\n\n"),
			TEXT("\" is not receiving on listen.\nSwitch it?\n\n"))
		: ReaperLang::S(TEXT("」は配信モードになっていません。\n切り替えますか？\n\n"),
			TEXT("\" is not sending on listen.\nSwitch it?\n\n"));

	const FString Message =
		ReaperLang::S(TEXT("「"), TEXT("\"")) + Name + Wanted
		+ ReaperLang::S(TEXT("※そのPARAKNOTが今やり取りしている音声は止まります。"),
			TEXT("Any audio it is currently handling will stop."));

	UWG_ConfirmDialog* Dlg = CreateWidget<UWG_ConfirmDialog>(this, UWG_ConfirmDialog::StaticClass());
	if (Dlg)
	{
		Dlg->Setup(Message);
		Dlg->OnConfirmed.BindDynamic(this, &UWG_RadNodzToolkit::ConfirmParaknotRoleChange);
		Dlg->AddToViewport(200);
	}
}

void UWG_RadNodzToolkit::ConfirmParaknotRoleChange()
{
	BeginParaknotSession(true);
}

void UWG_RadNodzToolkit::BeginParaknotSession(bool bNeedRoleChange)
{
	if (ParaknotMicInstanceId.IsEmpty() || !Client || !bIsConnected)
	{
		ShowMicMessage(ReaperLang::S(
			TEXT("送信先を特定できませんでした。"),
			TEXT("Could not resolve the target.")));
		return;
	}

	bParaknotMicStarting = true;
	UpdateMicSendButton();
	UpdateMonitorButton();

	// 音声の宛先は制御と同じマシン（接続パネルのIP）。
	FString Target = IPAddress;
	if (IPAddressInput) { Target = IPAddressInput->GetText().ToString(); }

	URadnodzClient* C = Client;
	const FString InstanceId = ParaknotMicInstanceId;
	const FString Name = IntercomUserName.IsEmpty() ? TEXT("Mic") : IntercomUserName;
	const EMicInputMode Mode = MicInputMode;
	const bool bMic = (ParaknotPurpose == EParaknotPurpose::Mic);
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	EnqueueControlRpc(NAME_None, [WeakThis, C, InstanceId, bNeedRoleChange, Target, Name, Mode, bMic]()
	{
		FString Error;
		FParaknotSession Session;
		bool bOk = true;

		if (bNeedRoleChange)
		{
			// マイク送信は相手を受信(待受)=2 に、モニターは送信(待受)=0 にする。
			bOk = ParaknotRpc::SetRole(C, InstanceId, bMic ? 2 : 0, 0, Error);
		}

		if (bOk)
		{
			// **役割を変えた直後は少し待つ必要がある。** 待受ポートの申告は
			// PARAKNOT 側のポーリング（3秒間隔）で RIGDOCKS へ届くので、
			// すぐに発行しようとすると相手の端点がまだ登録されていない。
			const int32 MaxAttempts = bNeedRoleChange ? 6 : 1;

			for (int32 Attempt = 0; Attempt < MaxAttempts; ++Attempt)
			{
				if (Attempt > 0) { FPlatformProcess::Sleep(1.0f); }

				bOk = bMic ? ParaknotRpc::OpenInputSession(C, InstanceId, Session, Error)
				           : ParaknotRpc::OpenOutputSession(C, InstanceId, Session, Error);

				if (bOk) { break; }
			}
		}

		const int32 Port = Session.Port;
		const FString Key = Session.Key;

		AsyncTask(ENamedThreads::GameThread, [WeakThis, bOk, Error, Port, Key, Target, Name, Mode, bMic]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			Self->bParaknotMicStarting = false;

			if (!bOk)
			{
				Self->ShowMicMessage(Error);
				Self->UpdateMicSendButton();
				Self->UpdateMonitorButton();
				return;
			}

			if (bMic)
			{
				if (!Self->RigMicSender)
				{
					Self->RigMicSender = NewObject<UParaknotSender>(Self);
				}

				if (!Self->RigMicSender->Start(Target, Port, Key, Name, Mode))
				{
					Self->ShowMicMessage(ReaperLang::S(
						TEXT("マイクを開始できませんでした（権限/デバイスを確認してください）。"),
						TEXT("Could not start the mic (check permission/device).")));
				}

				// マイクキャプチャの開始でオーディオデバイスが再初期化され、
				// モニター再生が止まることがある（ReaStream 経路と同じ扱い）。
				Self->RestartMonitorIfActive();
			}
			else
			{
				if (!Self->ParaknotMonitor)
				{
					Self->ParaknotMonitor = NewObject<UParaknotReceiver>(Self);
				}

				// 端末側の受信音量(dB)を線形ゲインに変換して反映する
				// （ReaStream 経路の ConfigureAudioMonitor と同じ計算）。
				Self->ParaknotMonitor->SetPlaybackGain(
					FMath::Pow(10.f, (float)Self->MonitorVolumeDb / 20.f));
				// 出力方法（ステレオ / 立体音響 / そのまま）と調整値を反映してから開始
				Self->ParaknotMonitor->SetOutputSettings(Self->MakeMonitorOutputSettings());

				if (!Self->ParaknotMonitor->Start(Self, Target, Port, Key))
				{
					Self->ShowMicMessage(ReaperLang::S(
						TEXT("モニターを開始できませんでした。"),
						TEXT("Could not start the monitor.")));
				}
			}

			Self->UpdateMicSendButton();
			Self->UpdateMonitorButton();
			Self->UpdateKeepAlive();
		});
	});
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
	if (ActionLabelText)       ActionLabelText->SetText(ReaperLang::T(TEXT("アクション"), TEXT("Action")));
	if (ActionRunButtonText)   ActionRunButtonText->SetText(ReaperLang::T(TEXT("実行"), TEXT("Run")));
	RefreshActionCombo();
	if (ClearArmedButtonText)  ClearArmedButtonText->SetText(ReaperLang::T(TEXT("全解除"), TEXT("Clear All")));
	if (DeleteTrackButtonText) DeleteTrackButtonText->SetText(ReaperLang::T(bDeleteMode ? TEXT("決定") : TEXT("削除"), bDeleteMode ? TEXT("OK") : TEXT("Delete")));
	// 計測調整ボタン（ジョブ実行中でなければ既定ラベルへ。言語切替で英語/日本語を反映）
	if (NormalizeButtonText && !bMeasureJobActive) NormalizeButtonText->SetText(ReaperLang::T(TEXT("計測調整"), TEXT("Normalize")));
	if (SlackShareButtonText && !bMeasureJobActive)  SlackShareButtonText->SetText(ReaperLang::T(TEXT("Slack"), TEXT("Slack")));
	if (DeleteSelectAllButtonText)   DeleteSelectAllButtonText->SetText(ReaperLang::T(TEXT("全選択"), TEXT("Select All")));
	if (DeleteDeselectAllButtonText) DeleteDeselectAllButtonText->SetText(ReaperLang::T(TEXT("全解除"), TEXT("Deselect All")));
	// ローカルIR（モード切替ボタンと、回数/間隔の見出し）
	if (LocalIRButtonText)     LocalIRButtonText->SetText(ReaperLang::T(TEXT("ローカルIR"), TEXT("Local IR")));
	if (LocalIRCountLabel)     LocalIRCountLabel->SetText(ReaperLang::T(TEXT("回数"), TEXT("Repeats")));
	if (LocalIRIntervalLabel)  LocalIRIntervalLabel->SetText(ReaperLang::T(TEXT("間隔"), TEXT("Interval")));
	UpdateLocalIRUI();
	if (TrackTabButtonText)    TrackTabButtonText->SetText(ReaperLang::T(TEXT("トラック"), TEXT("Tracks")));
	if (MarkerTabButtonText)   MarkerTabButtonText->SetText(ReaperLang::T(TEXT("マーカー"), TEXT("Markers")));
	if (MarkerEmptyText)       MarkerEmptyText->SetText(ReaperLang::T(TEXT("マーカーがありません"), TEXT("No markers")));
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
	if (SlackExportScopeRowLabel)       SlackExportScopeRowLabel->SetText(ReaperLang::T(TEXT("Slack出力"), TEXT("Slack export")));
	if (ScriptMatchScopeRowLabel)       ScriptMatchScopeRowLabel->SetText(ReaperLang::T(TEXT("台本一致の対象範囲"), TEXT("Script match scope")));
	if (ScriptMatchCheckFilterRowLabel) ScriptMatchCheckFilterRowLabel->SetText(ReaperLang::T(TEXT("台本一致のチェック状態"), TEXT("Script match check state")));
	UpdateSlackExportScopeButton();
	UpdateScriptMatchScopeButton();
	UpdateScriptMatchCheckFilterButton();

	// 下部タブ・設定画面のラベル
	if (HomeTabText)           HomeTabText->SetText(ReaperLang::T(TEXT("ホーム"), TEXT("Home")));
	if (SettingsTabText)       SettingsTabText->SetText(ReaperLang::T(TEXT("設定"), TEXT("Settings")));
	if (SettingsTitleText)     SettingsTitleText->SetText(ReaperLang::T(TEXT("設定"), TEXT("Settings")));
	if (ConnSectionLabel)      ConnSectionLabel->SetText(ReaperLang::T(TEXT("接続"), TEXT("Connection")));
	if (LocalIpRowLabel)       LocalIpRowLabel->SetText(ReaperLang::T(TEXT("この端末のIP"), TEXT("This device IP")));
	if (IpLabel)               IpLabel->SetText(ReaperLang::T(TEXT("接続先"), TEXT("Destination")));
	if (ReaStreamPortRowLabel)   ReaStreamPortRowLabel->SetText(ReaperLang::T(TEXT("モニター/マイクのポート"), TEXT("Monitor/Mic port")));
	UpdateMicTransportButton();
	if (ShowDeviceInfoRowLabel)  ShowDeviceInfoRowLabel->SetText(ReaperLang::T(TEXT("端末名/IPを表示"), TEXT("Show device name/IP")));
	UpdateShowDeviceInfoButton();
	if (CommTimeRowLabel)        CommTimeRowLabel->SetText(ReaperLang::T(TEXT("通信時間"), TEXT("Comm time")));
	if (CommTimeMeasureButtonText) CommTimeMeasureButtonText->SetText(ReaperLang::T(TEXT("測定"), TEXT("Measure")));
	if (IntercomSectionLabel)  IntercomSectionLabel->SetText(ReaperLang::T(TEXT("表示名"), TEXT("Display Name")));
	if (MemberUserNameRowLabel) MemberUserNameRowLabel->SetText(ReaperLang::T(TEXT("自分の名前"), TEXT("My name")));
	if (SlackSectionLabel)     SlackSectionLabel->SetText(ReaperLang::T(TEXT("Slack共有"), TEXT("Slack Share")));
	if (SlackTokenRowLabel)    SlackTokenRowLabel->SetText(ReaperLang::T(TEXT("Botトークン"), TEXT("Bot Token")));
	if (SlackToUserRowLabel)   SlackToUserRowLabel->SetText(ReaperLang::T(TEXT("宛先ユーザー"), TEXT("To User")));
	if (LoudnessSectionLabel)  LoudnessSectionLabel->SetText(ReaperLang::T(TEXT("ラウドネス"), TEXT("Loudness")));
	if (TargetLufsRowLabel)    TargetLufsRowLabel->SetText(ReaperLang::T(TEXT("目標ラウドネス(LUFS)"), TEXT("Target (LUFS)")));
	if (GroupTalkSendVolumeRowLabel) GroupTalkSendVolumeRowLabel->SetText(ReaperLang::T(TEXT("グループトーク送信音量"), TEXT("Group talk send vol")));
	if (GroupTalkVolumeRowLabel) GroupTalkVolumeRowLabel->SetText(ReaperLang::T(TEXT("グループトーク受信音量"), TEXT("Group talk recv vol")));
	if (MicCheckRowLabel) MicCheckRowLabel->SetText(ReaperLang::T(TEXT("マイク送信チェック"), TEXT("Mic send check")));
	UpdateMicCheckButton();
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
	if (CaptureSectionLabel)   CaptureSectionLabel->SetText(ReaperLang::T(TEXT("画面キャプチャ"), TEXT("Screen Capture")));
	if (GroupTalkSectionLabel) GroupTalkSectionLabel->SetText(ReaperLang::T(TEXT("グループ通話"), TEXT("Group Talk")));
	if (MonitorSectionLabel)   MonitorSectionLabel->SetText(ReaperLang::T(TEXT("モニター"), TEXT("Monitor")));
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
	if (ScreenRecOverlayRowLabel) ScreenRecOverlayRowLabel->SetText(ReaperLang::T(TEXT("録画中の表示"), TEXT("Recording overlay")));
	UpdateScreenRecOverlayButton();
	if (ScreenRecMicRowLabel)  ScreenRecMicRowLabel->SetText(ReaperLang::T(TEXT("録画のマイク入力"), TEXT("Recording mic input")));
	UpdateScreenRecMicButton();
	if (MonitorOutputRowLabel) MonitorOutputRowLabel->SetText(ReaperLang::T(TEXT("モニター出力"), TEXT("Monitor Output")));
	if (MonitorSourceRowLabel) MonitorSourceRowLabel->SetText(ReaperLang::T(TEXT("モニター方式"), TEXT("Monitor source")));
	if (MonitorInstanceRowLabel) MonitorInstanceRowLabel->SetText(ReaperLang::T(TEXT("配信元"), TEXT("Broadcast source")));
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
			// 設定画面（接続まわり）のボタン。リストから漏れていると日本語でも折り返しが残り、
			// 画面再構築時に1文字ずつ縦折返しになったまま戻らない。
			AddServerIdButtonText, DeleteServerIdButtonText, ShowOwnFpButtonText,
			LimitPeakButtonText,
		};
		// 最小要求幅も現在のラベル/フォントで再計測する（折り返し後に画面サイズが戻ったとき
		// 1行表示へ復帰させるため。言語切替でラベル長が変わっても正しい幅になる）。
		for (UTextBlock* T : ButtonTexts) { if (T) { T->SetAutoWrapText(bWrap); ApplyUnwrappedMinWidth(T); } }
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
	// ローカルIR：REAPERではなく端末からTSPを鳴らす（押すたびに再生⇄停止）。
	if (bLocalIRMode)
	{
		if (bLocalIRPlaying) { StopLocalIRPlayback(); }
		else                 { StartLocalIRPlayback(); }
		return;
	}

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

void UWG_RadNodzToolkit::UpdateDeleteMarkerButton()
{
	using namespace ReaperCtrlUI;
	if (!DeleteTrackButton || !DeleteTrackButtonText) return;

	// 再生中/録音中は「削除」ボタンを「マーカー」ボタンに切り替える。
	const bool bMarkerMode = (bIsPlaying || bIsRecording);
	if (bMarkerMode)
	{
		// 再生/録音中は削除できないので、選択モードに入っていたら抜けておく。
		if (bDeleteMode) { SetDeleteMode(false); }

		DeleteTrackButtonText->SetText(ReaperLang::T(TEXT("マーカー"), TEXT("Marker")));
		const FLinearColor Bg = AccentBlue;
		DeleteTrackButton->SetBackgroundColor(Bg);
		DeleteTrackButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
		if (DeleteTrackButtonIcon) { DeleteTrackButtonIcon->SetVisibility(ESlateVisibility::Collapsed); }
	}
	else
	{
		// 停止中：削除ボタンの通常/選択モード表示へ戻す（SetDeleteModeと同じ見た目）。
		DeleteTrackButtonText->SetText(ReaperLang::T(bDeleteMode ? TEXT("決定") : TEXT("削除"),
			bDeleteMode ? TEXT("OK") : TEXT("Delete")));
		const FLinearColor Bg = bDeleteMode ? RecRed : StopGray;
		DeleteTrackButton->SetBackgroundColor(Bg);
		DeleteTrackButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
		if (DeleteTrackButtonIcon) { DeleteTrackButtonIcon->SetVisibility(ESlateVisibility::Visible); }
	}
}

void UWG_RadNodzToolkit::HandleAddMarkerClicked()
{
	if (!Client || !bIsConnected) return;
	UReaProject* Proj = GetProject();
	if (!Proj) return;

	// 押した瞬間のプレイ位置(秒)を取得し、オフセット秒を足して確定する（0秒未満にはしない）。
	double Pos = URigdocksBronze_Cursor::AZ_GetPlayCursorPosition(Client, Proj);
	Pos = FMath::Max(0.0, Pos + (double)MarkerOffsetSeconds);

	// マーカー名入力ダイアログを開く。設定/キャンセルいずれも CommitMarker でマーカーを作成する
	// （キャンセル時は名前なし）。位置はここで固定するので、名前入力に時間がかかっても押した瞬間基準になる。
	UWG_MarkerNameDialog* Dlg = CreateWidget<UWG_MarkerNameDialog>(this, UWG_MarkerNameDialog::StaticClass());
	if (Dlg)
	{
		Dlg->Setup(this, Pos);
		Dlg->AddToViewport(130);
	}
	else
	{
		// ダイアログ生成に失敗しても、名前なしでマーカーだけは打っておく。
		CommitMarker(Pos, FString());
	}
}

void UWG_RadNodzToolkit::CommitMarker(double PositionSeconds, const FString& Name)
{
	if (!Client || !bIsConnected) return;
	UReaProject* Proj = GetProject();
	if (!Proj) return;

	// 色は0（REAPER既定色）。名前は空欄可。
	FireAndForgetRpc({ Proj }, [Proj, PositionSeconds, Name](URadnodzClient* C)
	{
		URigdocksSilver_Marker::AZ_AddMarker(C, Proj, PositionSeconds, Name, 0);
	});

	// 入力された名前は履歴に記録する（次回ダイアログで再利用できるように）。
	const FString Trimmed = Name.TrimStartAndEnd();
	if (!Trimmed.IsEmpty())
	{
		// 既存の同名を除いて先頭へ入れ直し（新しい順・重複なし）、上限件数で切り詰める。
		MarkerNameHistory.RemoveAll([&Trimmed](const FString& S) { return S.Equals(Trimmed, ESearchCase::CaseSensitive); });
		MarkerNameHistory.Insert(Trimmed, 0);
		if (MarkerNameHistory.Num() > MaxMarkerNameHistory)
		{
			MarkerNameHistory.SetNum(MaxMarkerNameHistory);
		}
		SaveSettings();
	}

	// マーカー表示中なら、追加したマーカーを一覧へ即反映する。
	if (bMarkerViewActive)
	{
		FetchMarkers();
		PopulateMarkerList();
	}
}

void UWG_RadNodzToolkit::HandleTrackTabClicked()
{
	SetMarkerViewActive(false);
}

void UWG_RadNodzToolkit::HandleMarkerTabClicked()
{
	SetMarkerViewActive(true);
}

void UWG_RadNodzToolkit::UpdateTrackMarkerTabStyles()
{
	using namespace ReaperCtrlUI;
	// 選択中＝文字がティールに光る＋下線を表示、非選択＝暗い文字＋下線を隠す（ホーム/リスト/設定と同じ）。
	auto SetSeg = [&](UTextBlock* Txt, UBorder* Ind, bool bActive)
	{
		if (Txt) { Txt->SetColorAndOpacity(FSlateColor(bActive ? AccentBlue : TextDim)); }
		if (Ind) { Ind->SetVisibility(bActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden); }
	};
	SetSeg(TrackTabButtonText,  TrackTabIndicator,  !bMarkerViewActive);
	SetSeg(MarkerTabButtonText, MarkerTabIndicator, bMarkerViewActive);
}

void UWG_RadNodzToolkit::SetMarkerViewActive(bool bActive)
{
	bMarkerViewActive = bActive;
	UpdateTrackMarkerTabStyles();

	// トラック一覧 ⇄ マーカー一覧の表示を切り替える。
	if (TrackListScrollBox)  { TrackListScrollBox->SetVisibility(bActive ? ESlateVisibility::Collapsed : ESlateVisibility::Visible); }
	if (MarkerListScrollBox) { MarkerListScrollBox->SetVisibility(bActive ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); }

	if (bActive)
	{
		FetchMarkers();
		PopulateMarkerList();
	}
	else if (MarkerEmptyText)
	{
		MarkerEmptyText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWG_RadNodzToolkit::FetchMarkers()
{
	MarkerList.Reset();
	if (!Client || !bIsConnected) { return; }
	UReaProject* Proj = GetProject();
	if (!Proj) { return; }

	// 「全マーカー取得」APIが無いため、id を 0..MaxScan-1 まで1往復のバッチで引き、
	// 実在するものだけを拾う（終端はゼロ値で判定）。マーカー数が MaxScan を超える運用は想定しない。
	const int32 MaxScan = 128;
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<URigdocksRpcCall*> Calls;
	Calls.Reserve(MaxScan);
	for (int32 i = 0; i < MaxScan; ++i)
	{
		URigdocksRpcCall* C = URigdocksSilver_Marker_Build::AZ_GetMarkerObject_Build(Proj, i);
		Batch->Add(C);
		Calls.Add(C);
	}
	if (Batch->Num() > 0) { Batch->Execute(Client); }

	// 重複（範囲外idが既存を返すケースの保険）を除きつつ実在マーカーを集める。
	TSet<FString> Seen;
	for (int32 i = 0; i < Calls.Num(); ++i)
	{
		FMarker M = URigdocksSilver_Marker_Parse::AZ_GetMarkerObject_Parse(Calls[i]);
		const bool bPresent = (M.Number > 0) || (!M.Name.IsEmpty()) || (M.StartTime > 0.0);
		if (!bPresent) { continue; }
		const FString Key = FString::Printf(TEXT("%d|%.4f|%s"), M.Number, M.StartTime, *M.Name);
		if (Seen.Contains(Key)) { continue; }
		Seen.Add(Key);
		// Set系API(AZ_SetMarkerName/Position)で使うidは、いま引いた id(i) と同じ。Index に保持しておく。
		M.Index = i;
		MarkerList.Add(M);
	}

	// 位置（時刻）順に並べる。
	MarkerList.Sort([](const FMarker& A, const FMarker& B) { return A.StartTime < B.StartTime; });
}

void UWG_RadNodzToolkit::PopulateMarkerList()
{
	if (!MarkerListScrollBox) { return; }

	MarkerListScrollBox->ClearChildren();
	MarkerRows.Reset();
	ActiveMarkerRow = nullptr;

	for (int32 i = 0; i < MarkerList.Num(); ++i)
	{
		const FMarker& M = MarkerList[i];

		// 時刻を mm:ss.d 表記に整形。
		const double T = FMath::Max(0.0, M.StartTime);
		const int32 Total = (int32)T;
		const int32 Min = Total / 60;
		const int32 Sec = Total % 60;
		const int32 Deci = (int32)((T - (double)Total) * 10.0);
		const FString TimeStr = FString::Printf(TEXT("%02d:%02d.%d"), Min, Sec, Deci);

		UWG_MarkerRow* Row = CreateWidget<UWG_MarkerRow>(this, UWG_MarkerRow::StaticClass());
		if (!Row) { continue; }
		// MarkerId は FetchMarkers で Index に入れた「引いた時のid」を使う。
		Row->Setup(this, i, M.Index, M.Name, M.StartTime, TimeStr);
		MarkerRows.Add(Row);

		if (UPanelSlot* PS = MarkerListScrollBox->AddChild(Row))
		{
			if (UScrollBoxSlot* SS = Cast<UScrollBoxSlot>(PS))
			{
				SS->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));
				SS->SetHorizontalAlignment(HAlign_Fill);
			}
		}
	}

	if (MarkerEmptyText)
	{
		MarkerEmptyText->SetVisibility((bMarkerViewActive && MarkerList.Num() == 0)
			? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UWG_RadNodzToolkit::SetActiveMarkerRow(UWG_MarkerRow* Row)
{
	// 旧アクティブ行の再生/選択表示を解除する。
	if (ActiveMarkerRow.IsValid() && ActiveMarkerRow.Get() != Row)
	{
		ActiveMarkerRow->SetActive(false);
		ActiveMarkerRow->SetPlaying(false);
	}
	ActiveMarkerRow = Row;
	if (Row) { Row->SetActive(true); }
}

void UWG_RadNodzToolkit::NotifyTransportToMarkerRow()
{
	if (ActiveMarkerRow.IsValid())
	{
		ActiveMarkerRow->SetPlaying(bIsPlaying);
	}
}

void UWG_RadNodzToolkit::PlayFromMarkerRow(UWG_MarkerRow* Row, double StartSeconds)
{
	if (!Client || !bIsConnected) { return; }

	// 既に再生/録音中なら一度停止してから新しい位置で再生する（Reaperは再生中の再発行でシークしない）。
	if (bIsPlaying || bIsRecording)
	{
		// 停止 → カーソル移動 → 再生 は同じ制御用キューに順番に積まれるので、
		// 実行順は保たれる（ゲームスレッドは待たない）。
		FireAndForgetRpc({}, [](URadnodzClient* C)
		{
			URigdocksBronze_Cursor::AZ_StopPlaying(C);
		});

		bIsPlaying = false;
		bIsRecording = false;
	}

	SetActiveMarkerRow(Row);       // この行をアクティブ（カーソル位置）にする
	MoveCursorToSeconds(StartSeconds);
	StartPlaying();                // bIsPlaying=true → NotifyTransport* で▶/■が追従
}

void UWG_RadNodzToolkit::StopMarkerPlayback()
{
	StopRecording();   // 再生/録音を停止（内部で AZ_StopPlaying を呼ぶ）→ 表示が追従
}

void UWG_RadNodzToolkit::RenameMarker(int32 MarkerId, const FString& NewName)
{
	if (!Client || !bIsConnected) { return; }
	UReaProject* Proj = GetProject();
	if (!Proj) { return; }

	FireAndForgetRpc({ Proj }, [Proj, MarkerId, NewName](URadnodzClient* C)
	{
		URigdocksSilver_Marker::AZ_SetMarkerName(C, Proj, MarkerId, NewName);
	});

	// 取得済みリストの名前も更新しておく（再取得コストを避ける）。
	for (FMarker& M : MarkerList)
	{
		if (M.Index == MarkerId) { M.Name = NewName; break; }
	}
	// 表示中の該当行のラベルも即更新する。
	for (UWG_MarkerRow* Row : MarkerRows)
	{
		if (Row && Row->GetMarkerId() == MarkerId) { Row->SetName(NewName); break; }
	}
}

void UWG_RadNodzToolkit::OpenMarkerRename(int32 MarkerId, const FString& CurrentName)
{
	if (!Client || !bIsConnected) { return; }

	UWG_MarkerNameDialog* Dlg = CreateWidget<UWG_MarkerNameDialog>(this, UWG_MarkerNameDialog::StaticClass());
	if (Dlg)
	{
		Dlg->SetupRename(this, MarkerId, CurrentName);
		Dlg->AddToViewport(130);
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

	// 再生中/録音中は削除ボタンが「マーカー」ボタンに切り替わっている。
	// このときはトラック削除ではなく、プレイ位置へのマーカー設定を行う。
	if (bIsPlaying || bIsRecording)
	{
		HandleAddMarkerClicked();
		return;
	}

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

	// 成否をボタン表示に反映。成功時は元の「Slack」へ戻す
	// （「Slackへ共有しました」だとボタンからはみ出し、戻す処理もないため表示しない）。
	const bool bOk = Res.Error.IsEmpty() && !Res.TimeStamp.IsEmpty();
	if (SlackShareButtonText)
	{
		SlackShareButtonText->SetText(FText::FromString(
			bOk ? FString(TEXT("Slack"))
			    : ReaperLang::S(TEXT("共有に失敗"), TEXT("Share failed"))));
	}
}

void UWG_RadNodzToolkit::UpdateStatus()
{
	// 現状ここで更新する表示はない（呼び出し箇所が多いため関数は残している）。
}

// ====================================================================
//  RPC の実行スレッド（同期RPCでゲームスレッドを止めないための集約先）
// ====================================================================

void UWG_RadNodzToolkit::EnsureRpcQueues()
{
	// ★ゲームスレッドからのみ呼ぶ★（生成の競合を避けるため）。
	if (!ControlQueue)
	{
		ControlQueue = MakeUnique<FRadnodzRpcQueue>(TEXT("RadnodzRpcControl"));
	}

	if (!HeavyQueue)
	{
		HeavyQueue = MakeUnique<FRadnodzRpcQueue>(TEXT("RadnodzRpcHeavy"));
	}
}

void UWG_RadNodzToolkit::ShutdownRpcQueues()
{
	// デストラクタが実行中ジョブの完了を待つ。既に切断していればRPCは即座に
	// 戻るので、ここで長く待たされることは無い。
	ControlQueue.Reset();
	HeavyQueue.Reset();
}

bool UWG_RadNodzToolkit::EnqueueControlRpc(FName Key, TFunction<void()> Job)
{
	EnsureRpcQueues();

	if (Key.IsNone())
	{
		ControlQueue->Enqueue(MoveTemp(Job));

		return true;
	}

	return ControlQueue->EnqueueUnique(Key, MoveTemp(Job));
}

bool UWG_RadNodzToolkit::EnqueueHeavyRpc(FName Key, TFunction<void()> Job)
{
	EnsureRpcQueues();

	if (Key.IsNone())
	{
		HeavyQueue->Enqueue(MoveTemp(Job));

		return true;
	}

	return HeavyQueue->EnqueueUnique(Key, MoveTemp(Job));
}

URadnodzClient* UWG_RadNodzToolkit::GetAnalysisClient() const
{
	// 2本目が使えるならそちら。張れていなければ制御接続へフォールバックする
	// （遅延は戻るが、機能そのものは止めない）。
	if (bHeavyClientReady.Load() && HeavyClient)
	{
		return HeavyClient;
	}

	return Client;
}

double UWG_RadNodzToolkit::GetPollingScale() const
{
	// 回線が悪いほど周期を伸ばす。往復に時間がかかる状態で短い周期を保っても
	// 要求が積み上がるだけで、他のRPCを待たせる原因にしかならない。
	const double Rtt = LastRoundTripMs.Load();

	if (Rtt <= 0.0 || Rtt < 20.0) { return 1.0; }
	if (Rtt < 60.0)               { return 1.5; }
	if (Rtt < 150.0)              { return 2.0; }

	return 3.0;
}

void UWG_RadNodzToolkit::FireAndForgetRpc(const TArray<UObject*>& KeepAlive,
	TFunction<void(URadnodzClient*)> Body)
{
	// 値を返さない単発の操作（REC/ソロ/ミュート/リネーム/トランスポート等）を、
	// ゲームスレッドを止めずに投げる。
	//
	// UI とローカル状態は呼び出し側が先に更新してよい（楽観的更新）。
	// 失敗しても次の一覧取得で実機の状態に揃うため、ここでは結果を待たない。
	if (!Client || !bIsConnected || !Body)
	{
		return;
	}

	// UReaProject / UMediaItem などは呼び出しのたびに作られる一時 UObject で、
	// 誰も参照していないとGCに回収される。ワーカーが使い終わるまで保持する。
	const int32 Token = HoldRpcObjects(KeepAlive);

	URadnodzClient* C = Client;

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	EnqueueControlRpc(NAME_None, [WeakThis, C, Token, Body]()
	{
		if (IsValid(C)) { Body(C); }

		AsyncTask(ENamedThreads::GameThread, [WeakThis, Token]()
		{
			if (UWG_RadNodzToolkit* Self = WeakThis.Get())
			{
				Self->ReleaseRpcObjects(Token);
			}
		});
	});
}

int32 UWG_RadNodzToolkit::HoldRpcObjects(const TArray<UObject*>& Objects)
{
	const int32 Token = NextRpcRefToken++;

	FRadnodzRpcRefSet Set;
	Set.Objects.Reserve(Objects.Num());

	for (UObject* Obj : Objects)
	{
		if (Obj) { Set.Objects.Add(Obj); }
	}

	RpcRefsInFlight.Add(Token, MoveTemp(Set));

	return Token;
}

void UWG_RadNodzToolkit::ReleaseRpcObjects(int32 Token)
{
	RpcRefsInFlight.Remove(Token);
}

bool UWG_RadNodzToolkit::ExecuteBatchAsync(URadnodzRpcBatch* Batch, const TArray<UObject*>& KeepAlive,
	FName Key, bool bHeavy, TFunction<void()> OnDoneGameThread)
{
	if (!Batch || !Client || !bIsConnected)
	{
		return false;
	}

	// Execute 中にGCで回収されないよう、バッチと各 RpcCall の参照を残す。
	TArray<UObject*> Refs = KeepAlive;
	Refs.Add(Batch);

	const int32 Token = HoldRpcObjects(Refs);

	// 解析用は2本目の接続。張れていなければ制御接続へフォールバックする。
	URadnodzClient* TargetClient = bHeavy ? GetAnalysisClient() : Client;

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	// 参照は RpcRefsInFlight が持っているので、ワーカーへは生ポインタで渡してよい。
	auto Job = [WeakThis, Batch, TargetClient, Token, OnDoneGameThread]()
	{
		if (IsValid(Batch) && IsValid(TargetClient))
		{
			// ★ここでするのは通信だけ★（UObject を作る Parse はGTで行う）
			Batch->Execute(TargetClient);
		}

		AsyncTask(ENamedThreads::GameThread, [WeakThis, Token, OnDoneGameThread]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();

			if (!Self) { return; }

			if (OnDoneGameThread) { OnDoneGameThread(); }

			Self->ReleaseRpcObjects(Token);
		});
	};

	const bool bQueued = bHeavy
		? EnqueueHeavyRpc(Key, MoveTemp(Job))
		: EnqueueControlRpc(Key, MoveTemp(Job));

	if (!bQueued)
	{
		ReleaseRpcObjects(Token);
	}

	return bQueued;
}

void UWG_RadNodzToolkit::StartHeavyClientConnect()
{
	// 制御接続が成立した直後に、同じホストへもう1本つなぐ。
	// ラウドネス解析は1回で数十秒かかるため、制御接続に載せると
	// その間ずっと操作系のRPCが後ろに並んでしまう。
	ShutdownHeavyClient();

	URadnodzClient* NewClient = URadnodzClient::CreateRadnodzClient();

	if (!NewClient) { return; }

	NewClient->AddToRoot();   // 接続が終わるまでGCから守る

	// 接続は常に TLS(mTLS)。
	{
		NewClient->SetStorageDirectory(GetAuthStorageDir());
		NewClient->GenerateIdentity();

		const FString Sel = GetSelectedServerId();
		const int32 Idx = FindServerAuthIndex(Sel);
		NewClient->SetServerId(Idx != INDEX_NONE ? GetServerPinKey(ServerAuthEntries[Idx]) : Sel);
		NewClient->SetExpectedServerFingerprint(Idx != INDEX_NONE ? ServerAuthEntries[Idx].Fingerprint : FString());
	}

	const FString Ip = IPAddress;
	const int32 P = Port;
	const int32 MyGen = ConnectGeneration;

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	EnqueueHeavyRpc(NAME_None, [WeakThis, NewClient, Ip, P, MyGen]()
	{
		const bool bOk = IsValid(NewClient) ? NewClient->Connect(Ip, P) : false;

		AsyncTask(ENamedThreads::GameThread, [WeakThis, NewClient, bOk, MyGen]()
		{
			if (IsValid(NewClient)) { NewClient->RemoveFromRoot(); }

			UWG_RadNodzToolkit* Self = WeakThis.Get();

			// 接続世代が進んでいる＝この試行はもう用済み。
			if (!Self || MyGen != Self->ConnectGeneration || !Self->bIsConnected || !bOk)
			{
				if (IsValid(NewClient)) { NewClient->Disconnect(); }

				if (Self && !bOk)
				{
					UE_LOG(LogTemp, Warning,
						TEXT("[RIGDRED] 解析用の2本目の接続を確立できませんでした。制御接続で代用します"));
				}

				return;
			}

			Self->HeavyClient = NewClient;   // UPROPERTY なのでここからはGC対象外
			Self->bHeavyClientReady.Store(true);
		});
	});
}

void UWG_RadNodzToolkit::ShutdownHeavyClient()
{
	bHeavyClientReady.Store(false);

	if (HeavyClient)
	{
		// クライアントライブラリ側が「shutdown で待機中の受信を返らせてから閉じる」ため、
		// 解析の途中でもここは待たされない。
		HeavyClient->Disconnect();
		HeavyClient = nullptr;
	}
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

	// 接続は常に TLS(mTLS)。この試行クライアントに鍵・serverId・期待FPを設定する。
	// (ゲームスレッドで実施。identity をロードするとクライアントライブラリ側がセキュアモードになる)
	{
		AttemptClient->SetStorageDirectory(GetAuthStorageDir());
		AttemptClient->GenerateIdentity();
		// ピン留めキーは URL の nodeId（サーバー名を変えてもピンは変わらない）。
		const FString Sel = GetSelectedServerId();
		const int32 Idx = FindServerAuthIndex(Sel);
		AttemptClient->SetServerId(Idx != INDEX_NONE ? GetServerPinKey(ServerAuthEntries[Idx]) : Sel);
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

		// 復帰できたので自動再接続の予約と試行回数、生存監視のカウンタをリセットする
		bAutoReconnectPending = false;
		ConnAutoReconnectTries = 0;
		ConnHealthFailCount = 0;
		ConnHealthTimer = 0.0;

		// 接続中は画面OFFでも接続を保つため、キープアライブ（前面サービス＋心拍）を開始する。
		UpdateKeepAlive();
		BgConnKeepLast.Store(FPlatformTime::Seconds());   // 接続直後は心拍間隔を空けてから開始

		// ラウドネス解析など時間のかかるAPI用に、もう1本つないでおく。
		// 非同期なので、張れるまでは制御接続で代用される。
		StartHeavyClientConnect();

		// 接続成功直後の重い処理（トラック一覧取得・カード生成）は、同期通信と
		// 多数のウィジェット生成でゲームスレッドを数秒ブロックする。通信部分は
		// ワーカーへ出し、UI構築だけゲームスレッドで行う。
		TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);
		AsyncTask(ENamedThreads::GameThread, [WeakThis]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self || !Self->bIsConnected || !Self->Client) { return; }

			// 接続時にRECアームは解除しない（再接続のたびに録音待機が落ちないように）。
			// FetchTracks() が現在のREC状態を読み取って ArmedTrackIndices／各カードに反映する。
			Self->FetchTracks();
			Self->ApplyConnectedUI();
			Self->OnConnected();
			Self->ResolveGroupTalkFolder();   // Bridge フォルダと同じ階層の RadNodzToolkit/GroupTalk を名簿フォルダに使う

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

	// 待機中のRPCジョブを捨ててから接続を落とす。
	// 積んだままにすると、切断後に「未接続なので何もしない」ジョブが
	// 大量に流れるだけになる。
	if (ControlQueue) { ControlQueue->Clear(); }
	if (HeavyQueue)   { HeavyQueue->Clear(); }

	// 接続がまだ生きているうちに、自分の在席情報をオフライン扱いにしておく
	// （切断後はRPCが使えず自分では消せないため、切断前に済ませる必要がある）。
	//
	// ただしゲームスレッドで待ってはいけない。キューが空いていれば
	// 「オフライン書き込み → 切断」を1つのジョブとして順に実行させ、
	// 実行中のジョブがある（＝応答待ちで止まっている可能性がある）ときは
	// 書き込みを諦めてその場で接続を落とす。
	if (Client && bIsConnected)
	{
		const bool bQueueBusy = !ControlQueue || ControlQueue->IsBusy();

		FString OfflineFolder;
		FString OfflineFile1;
		FString OfflineFile2;
		FString OfflineContent;
		const bool bHasPresence = !bQueueBusy
			&& BuildPresenceOfflineWrite(OfflineFolder, OfflineFile1, OfflineFile2, OfflineContent);

		if (bQueueBusy)
		{
			// クライアントライブラリ側の Disconnect は shutdown で待機中の受信を返らせてから
			// 閉じるので、ここで待たされることは無い。
			Client->Disconnect();
		}
		else
		{
			URadnodzClient* C = Client;

			EnqueueControlRpc(NAME_None,
				[C, bHasPresence, OfflineFolder, OfflineFile1, OfflineFile2, OfflineContent]()
				{
					if (!IsValid(C)) { return; }

					if (bHasPresence)
					{
						URigdocksSilver_File::AZ_WriteFile(C, OfflineFolder, OfflineFile1, OfflineContent, 0);
						URigdocksSilver_File::AZ_WriteFile(C, OfflineFolder, OfflineFile2, OfflineContent, 0);
					}

					C->Disconnect();
				});
		}
	}

	ShutdownHeavyClient();

	// 捨てたジョブの完了コールバックはもう走らないので、実行中フラグと
	// GC保持枠をここで戻す。戻さないと再接続後にその種類のジョブが
	// 二度と積まれなくなる（フラグが立ちっぱなしになる）。
	bMeterRpcInFlight = false;
	bWarmRpcInFlight = false;
	bTracksRpcInFlight = false;
	bMeasureRpcInFlight = false;
	bProbeRpcInFlight = false;
	bScriptMatchRpcInFlight = false;
	bHealthProbeInFlight = false;
	RpcRefsInFlight.Empty();

	// ローカル状態の破棄はワーカーのRPCと重ならないよう排他の下で行う。
	FScopeLock RpcLock(&ClientRpcLock);

	bIsConnected = false;

	// **選んでいた PARAKNOT も捨てる。** 次に繋ぐ相手が同じとは限らず、
	// InstanceId は相手プロセス限りの値。持ち越すと鍵の取り直しが
	// 失敗し続ける (接続先を変えたときに無限ループになっていた)。
	ParaknotMicInstanceId.Reset();
	ParaknotMicLabel.Reset();
	ParaknotMonitorRecoverFailures = 0;

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
	UpdateKeepAlive();   // グループトーク停止後の状態でバックグラウンド継続を再判定

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
	FetchTracksAsync(nullptr);
}

void UWG_RadNodzToolkit::FetchTracksAsync(TFunction<void()> OnDone)
{
	// トラック一覧の取得。
	//
	// 「一覧＋深度＋トランスポート状態」で1往復、「各トラックのソロ/ミュート/REC」で
	// もう1往復。どちらも Build(GT) → Execute(ワーカー) → Parse(GT) の順で流す。
	// Parse は UMediaTrack を生成するのでゲームスレッドから動かせない。
	if (!Client || !bIsConnected)
	{
		if (OnDone) { OnDone(); }
		return;
	}

	// 取得中に重ねて呼ばれたら無視する（接続直後と自動更新が重なることがある）。
	if (bTracksRpcInFlight)
	{
		if (OnDone) { OnDone(); }
		return;
	}

	UReaProject* Proj = GetProject();
	if (!Proj)
	{
		if (OnDone) { OnDone(); }
		return;
	}

	// 「更新」してもラウドネスのキャッシュは保持する（計測済みは再計測しない）。
	// 新規/変更された波形だけが、次の計測時に未計測として扱われ計測される。

	// メディア一覧キャッシュはここでは破棄しない。破棄すると展開中の一覧が一度空になり、
	// 中身が変わっていなくても画面が描き替わってしまうため。
	// 代わりに背景先読み（StartMediaCacheWarm(true)）で取り直し、内容が変わったトラックだけ
	// 表示を作り直す。トラックの増減時は ApplyTracksUI 側でまとめて破棄する。

	// --- 1往復目：一覧・深度・トランスポート状態 ---
	//
	// 深度は「AZ_GetTrackListDetail が Depth を返さない場合」の保険だが、
	// 同じ往復に載せておけば必要になっても追加の往復が要らない。
	URigdocksRpcCall* ListCall  = URigdocksSilver_Track_Build::AZ_GetTrackListDetail_Build(Proj);
	URigdocksRpcCall* DepthCall = URigdocksSilver_Track_Build::AZ_GetTrackDepthList_Build(Proj);
	URigdocksRpcCall* PlayCall  = URigdocksSilver_Project_Build::AZ_GetPlayState_Build(Proj);
	URigdocksRpcCall* RecCall   = URigdocksSilver_Project_Build::AZ_GetRecordState_Build(Proj);
	URigdocksRpcCall* PauseCall = URigdocksSilver_Project_Build::AZ_GetPauseState_Build(Proj);
	URigdocksRpcCall* StopCall  = URigdocksSilver_Project_Build::AZ_GetStopState_Build(Proj);

	URadnodzRpcBatch* ListBatch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	ListBatch->Add(ListCall);
	ListBatch->Add(DepthCall);
	ListBatch->Add(PlayCall);
	ListBatch->Add(RecCall);
	ListBatch->Add(PauseCall);
	ListBatch->Add(StopCall);

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	bTracksRpcInFlight = true;

	const bool bQueued = ExecuteBatchAsync(ListBatch,
		{ ListCall, DepthCall, PlayCall, RecCall, PauseCall, StopCall },
		NAME_None, /*bHeavy=*/false,
		[WeakThis, ListCall, DepthCall, PlayCall, RecCall, PauseCall, StopCall, OnDone]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			// ここはゲームスレッド。UMediaTrack を作る Parse はここで行う。
			Self->TrackList = URigdocksSilver_Track_Parse::AZ_GetTrackListDetail_Parse(ListCall);

			// フォルダ階層を判定するため、深度(Depth)が無ければ専用APIの結果で補完する。
			// （Indexで突き合わせる）
			bool bHasDepth = false;
			for (const FTrackDetail& T : Self->TrackList)
			{
				if (T.Depth != 0) { bHasDepth = true; break; }
			}

			if (!bHasDepth)
			{
				const TArray<FTrackDetail> DepthList = URigdocksSilver_Track_Parse::AZ_GetTrackDepthList_Parse(DepthCall);

				if (DepthList.Num() > 0)
				{
					TMap<int32, int32> DepthByIndex;
					for (const FTrackDetail& D : DepthList)
					{
						DepthByIndex.Add(D.Index, D.Depth);
					}
					for (FTrackDetail& T : Self->TrackList)
					{
						if (const int32* Found = DepthByIndex.Find(T.Index))
						{
							T.Depth = *Found;
						}
					}
				}
			}

			Self->bIsPlaying   = URigdocksSilver_Project_Parse::AZ_GetPlayState_Parse(PlayCall);
			Self->bIsRecording = URigdocksSilver_Project_Parse::AZ_GetRecordState_Parse(RecCall);
			Self->bIsPaused    = URigdocksSilver_Project_Parse::AZ_GetPauseState_Parse(PauseCall);
			Self->bIsStopped   = URigdocksSilver_Project_Parse::AZ_GetStopState_Parse(StopCall);

			Self->ApplyRecordingUI();
			Self->UpdateTransportButtons();
			Self->NotifyTransportToMediaRow();

			// 続けて各トラックの状態を取りに行く（もう1往復）。
			Self->FetchTrackStatesAsync(OnDone);
		});

	if (!bQueued)
	{
		bTracksRpcInFlight = false;

		if (OnDone) { OnDone(); }
	}
}

void UWG_RadNodzToolkit::FetchTrackStatesAsync(TFunction<void()> OnDone)
{
	// --- 2往復目：各トラックのソロ/ミュート/REC(アーム) ---
	//
	// カード生成(ApplyTracksUI→InitTrackCard)は IsTrackSoloed/Muted/Armed を
	// 参照するので、UIを組み立てる前にここで取得しておく必要がある。
	// トラック数×3回の往復をRpcBatchで1往復にまとめる。
	ArmedTrackIndices.Reset();
	SoloedTrackIndices.Reset();
	MutedTrackIndices.Reset();

	UReaProject* Proj = GetProject();

	if (!Client || !bIsConnected || !Proj || TrackList.Num() == 0)
	{
		bTracksRpcInFlight = false;
		FinishFetchTracks();

		if (OnDone) { OnDone(); }
		return;
	}

	struct FTrackStateCalls
	{
		int32 Index = 0;
		URigdocksRpcCall* Solo = nullptr;
		URigdocksRpcCall* Mute = nullptr;
		URigdocksRpcCall* Rec  = nullptr;
	};

	URadnodzRpcBatch* StateBatch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<FTrackStateCalls> StateCalls;
	TArray<UObject*> KeepAlive;
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

		KeepAlive.Add(C.Solo);
		KeepAlive.Add(C.Mute);
		KeepAlive.Add(C.Rec);
	}

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	const bool bQueued = ExecuteBatchAsync(StateBatch, KeepAlive, NAME_None, /*bHeavy=*/false,
		[WeakThis, StateCalls, OnDone]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			for (const FTrackStateCalls& C : StateCalls)
			{
				if (URigdocksSilver_Track_Parse::AZ_GetTrackIdSolo_Parse(C.Solo))     { Self->SoloedTrackIndices.Add(C.Index); }
				if (URigdocksSilver_Track_Parse::AZ_GetTrackIdMute_Parse(C.Mute))     { Self->MutedTrackIndices.Add(C.Index); }
				if (URigdocksSilver_Track_Parse::AZ_GetTrackIdRecording_Parse(C.Rec)) { Self->ArmedTrackIndices.AddUnique(C.Index); }
			}

			Self->bTracksRpcInFlight = false;
			Self->FinishFetchTracks();

			if (OnDone) { OnDone(); }
		});

	if (!bQueued)
	{
		bTracksRpcInFlight = false;
		FinishFetchTracks();

		if (OnDone) { OnDone(); }
	}
}

void UWG_RadNodzToolkit::FinishFetchTracks()
{
	// 一覧が揃ったのでUIを組み立てる（ここは必ずゲームスレッド）。
	ApplyTracksUI(TrackList);
	OnTracksLoaded(TrackList);

	// 接続先デバイス情報（Hz/bit・入力/出力ch・端末名）を取得して表示へ反映する。
	// FetchTracks は接続直後と「更新」ボタンの両方から呼ばれるので、両方でカバーされる。
	UpdateDeviceInfo();

	// メディア一覧を背景で先読みしておく（接続/更新の直後に少しずつ取得）。
	// これにより、各トラックを展開した時の「毎回の重い読み込み」が無くなる。
	// 併せて既存キャッシュも取り直し、内容が変わったトラックの一覧だけを作り直す。
	StartMediaCacheWarm(true);

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

	FireAndForgetRpc({ Proj }, [Proj](URadnodzClient* C)
	{
		URigdocksSilver_Track::AZ_SetTrackAllRecording(C, Proj, false);
	});

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

	FireAndForgetRpc({ Proj }, [Proj, TrackIndex, bSolo](URadnodzClient* C)
	{
		URigdocksSilver_Track::AZ_SetTrackIdSolo(C, Proj, TrackIndex, bSolo);
	});

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

	FireAndForgetRpc({ Proj }, [Proj, TrackIndex, bMute](URadnodzClient* C)
	{
		URigdocksSilver_Track::AZ_SetTrackIdMute(C, Proj, TrackIndex, bMute);
	});

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

void UWG_RadNodzToolkit::FetchTrackMediaItemsAsync(int32 TrackIndex, bool bHeavy, FName Key,
	TFunction<void(const TArray<UMediaItem*>&, const TArray<FString>&, const TArray<double>&)> OnDone)
{
	// GetTrackMediaItems の非同期版。
	//
	// 「一覧取得」→「各メディアの名前/開始位置」の2往復を、どちらも
	// Build(GT) → Execute(ワーカー) → Parse(GT) の順で流す。
	// Parse は UMediaItem を生成するのでゲームスレッドから動かせない。
	if (!Client || !bIsConnected || !OnDone)
	{
		if (OnDone) { OnDone({}, {}, {}); }
		return;
	}

	UReaProject* Proj = GetProject();
	if (!Proj)
	{
		OnDone({}, {}, {});
		return;
	}

	URigdocksRpcCall* ListCall = URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaItemList_Build(Proj, TrackIndex);
	URadnodzRpcBatch* ListBatch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	ListBatch->Add(ListCall);

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	const bool bQueued = ExecuteBatchAsync(ListBatch, { ListCall }, Key, bHeavy,
		[WeakThis, ListCall, bHeavy, Key, OnDone]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			// ここはゲームスレッド。UMediaItem の生成はここで行う。
			TArray<UMediaItem*> Items = URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaItemList_Parse(ListCall);

			if (Items.Num() == 0 || !Self->Client || !Self->bIsConnected)
			{
				OnDone(Items, {}, {});
				return;
			}

			// 2往復目：各メディアの名前と開始位置を1回のバッチで取る。
			URadnodzRpcBatch* InfoBatch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
			TArray<URigdocksRpcCall*> NameCalls;
			TArray<URigdocksRpcCall*> StartCalls;
			TArray<UObject*> KeepAlive;

			for (UMediaItem* It : Items)
			{
				if (!It) { continue; }

				URigdocksRpcCall* NameCall  = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemName_Build(It);
				URigdocksRpcCall* StartCall = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemStartTimeSeconds_Build(It);

				NameCalls.Add(NameCall);
				StartCalls.Add(StartCall);
				InfoBatch->Add(NameCall);
				InfoBatch->Add(StartCall);

				KeepAlive.Add(NameCall);
				KeepAlive.Add(StartCall);
				KeepAlive.Add(It);   // 2往復目の間、生成済みのメディアも保持しておく
			}

			const bool bInfoQueued = Self->ExecuteBatchAsync(InfoBatch, KeepAlive, NAME_None, bHeavy,
				[WeakThis, Items, NameCalls, StartCalls, OnDone]()
				{
					if (!WeakThis.IsValid()) { return; }

					TArray<FString> Names;
					TArray<double>  Starts;
					Names.Reserve(NameCalls.Num());
					Starts.Reserve(StartCalls.Num());

					for (int32 i = 0; i < NameCalls.Num(); ++i)
					{
						Names.Add(URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemName_Parse(NameCalls[i]));
						Starts.Add(URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemStartTimeSeconds_Parse(StartCalls[i]));
					}

					OnDone(Items, Names, Starts);
				});

			if (!bInfoQueued)
			{
				// 名前/開始位置が取れないまま Items だけ返すと、呼び出し側で
				// 配列長が食い違う。取得できなかった扱いにして揃える。
				OnDone({}, {}, {});
			}
		});

	if (!bQueued)
	{
		OnDone({}, {}, {});
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
	FireAndForgetRpc({ Proj }, [Proj, Seconds](URadnodzClient* C)
	{
		URigdocksBronze_Cursor::AZ_SetPlayCursorPosition(C, Proj, 0, Seconds);
	});
}

void UWG_RadNodzToolkit::RenameMediaItem(UMediaItem* MediaItem, const FString& NewName)
{
	if (!Client || !bIsConnected || !MediaItem) return;

	FireAndForgetRpc({ MediaItem }, [MediaItem, NewName](URadnodzClient* C)
	{
		URigdocksBronze_MediaItem::AZ_SetMediaItemName(C, MediaItem, NewName);
	});

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

	FireAndForgetRpc({ MediaItem }, [MediaItem, bMute](URadnodzClient* C)
	{
		URigdocksBronze_MediaItem::AZ_SetMediaItemMute(C, MediaItem, bMute);
	});
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

	// 前回ぶんの計測が返るまで次へ進まない。
	// ラウドネス解析は1件で数十秒かかることがあるため、ここを守らないと
	// 解析要求だけが積み上がる。
	if (bMeasureRpcInFlight) return;

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

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	// 現トラックのメディア一覧をこの1ステップで取得（通信はワーカー）
	if (!bMeasureListLoaded)
	{
		bMeasureRpcInFlight = true;

		FetchTrackMediaItemsAsync(MeasureTrackQueue[MeasureTrackCursor], /*bHeavy=*/true, NAME_None,
			[WeakThis](const TArray<UMediaItem*>& Items,
				const TArray<FString>& Names, const TArray<double>& Starts)
			{
				UWG_RadNodzToolkit* Self = WeakThis.Get();
				if (!Self) { return; }

				Self->MeasureNames  = Names;
				Self->MeasureStarts = Starts;

				Self->MeasureItems.Reset();
				for (UMediaItem* It : Items) { Self->MeasureItems.Add(It); }

				Self->MeasureItemCursor = 0;
				Self->bMeasureListLoaded = true;
				Self->bMeasureRpcInFlight = false;
			});

		return;
	}

	// 現トラックのメディアを処理し終えたら次トラックへ
	if (!MeasureItems.IsValidIndex(MeasureItemCursor))
	{
		++MeasureTrackCursor;
		bMeasureListLoaded = false;
		return;
	}

	// メディア1件のラウドネスを計測する。
	//
	// **これが一番重い往復**（サーバ側でメディア全体を解析するため、素材が長いと
	// 数十秒かかる）。解析用の2本目の接続で流し、制御接続を占有させない。
	UMediaItem* It = MeasureItems[MeasureItemCursor];
	const FString Key = FString::Printf(TEXT("%s@%.3f"),
		*MeasureNames[MeasureItemCursor], MeasureStarts[MeasureItemCursor]);

	// 強制再計測でなく、既に計測済みならそのまま次へ（通信しない）。
	if (!It || (!bMeasureForceRemeasure && MomentaryLUFSCache.Contains(Key)))
	{
		++MeasureItemCursor;
		return;
	}

	URigdocksRpcCall* LufsCall = URigdocksSilver_Loudness_Build::AZ_GetMediaItemLoudnessMaxMomentary_Build(It);
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	Batch->Add(LufsCall);

	bMeasureRpcInFlight = true;

	const bool bQueued = ExecuteBatchAsync(Batch, { LufsCall, It }, NAME_None, /*bHeavy=*/true,
		[WeakThis, LufsCall, Key]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			double Lufs = 0.0;
			double Pos = 0.0;
			URigdocksSilver_Loudness_Parse::AZ_GetMediaItemLoudnessMaxMomentary_Parse(LufsCall, Lufs, Pos);

			// キャッシュの書き込みはゲームスレッドだけで行う。
			Self->MomentaryLUFSCache.Add(Key, Lufs);

			++Self->MeasureItemCursor;
			Self->bMeasureRpcInFlight = false;
		});

	if (!bQueued)
	{
		bMeasureRpcInFlight = false;
	}
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

	// 前回ぶんがまだ返っていなければ今回は見送る（要求を積み増さない）。
	if (bMeterRpcInFlight) return;

	// 設定の更新間隔でポーリング。回線が遅いときは自動で間隔を伸ばす
	// （往復に時間がかかる状態で短い周期を保っても要求が溜まるだけ）。
	MeterPollTimer += DeltaSeconds;
	if (MeterPollTimer < (double)MeterIntervalMs / 1000.0 * GetPollingScale()) return;
	MeterPollTimer = 0.0;

	// カードを1パスで走査する。
	// メーターが実際に見えている（アーム中の葉トラック、かつ折り畳みで隠れていない）カードだけポーリングする。
	// 隠れているカードへの同期API往復はユーザーに見えないため省く＝携帯回線での無駄を削減。
	// 全アーム中カード×全チャンネルのピーク取得を、1ティック1往復のRpcBatchにまとめる。
	//
	// **組み立て（Build）はここ＝ゲームスレッドで行う。** 通信だけワーカーへ出し、
	// 値の反映（Parse）は完了後にゲームスレッドへ戻ってから行う。
	struct FMeterTarget { TWeakObjectPtr<UWG_TrackCard> Card; int32 FirstCall; int32 NumCh; };
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

	TArray<UObject*> KeepAlive;
	KeepAlive.Reserve(PeakCalls.Num());
	for (URigdocksRpcCall* Call : PeakCalls) { KeepAlive.Add(Call); }

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	bMeterRpcInFlight = true;

	const bool bQueued = ExecuteBatchAsync(Batch, KeepAlive, TEXT("meter"), /*bHeavy=*/false,
		[WeakThis, PeakCalls, Targets]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			Self->bMeterRpcInFlight = false;

			TArray<float> DbPerCh;   // 取得バッファは使い回してアロケーションを抑える
			for (const FMeterTarget& T : Targets)
			{
				UWG_TrackCard* Card = T.Card.Get();
				if (!Card) { continue; }   // 一覧の作り直しで消えたカードは飛ばす

				DbPerCh.Reset();
				DbPerCh.Reserve(T.NumCh);
				for (int32 ch = 0; ch < T.NumCh; ++ch)
				{
					DbPerCh.Add((float)URigdocksSilver_Track_Parse::AZ_GetTrackItemCurrentPeakDB_Parse(PeakCalls[T.FirstCall + ch]));
				}
				Card->SetMeterChannels(DbPerCh);
			}
		});

	if (!bQueued)
	{
		bMeterRpcInFlight = false;
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

void UWG_RadNodzToolkit::HandleGroupTalkVolumeChanged(float NewValue)
{
	// ドラッグ中：1刻みに丸めて即反映（保存は離した時にまとめて行う）。
	const float Snapped = (float)FMath::Clamp(FMath::RoundToInt(NewValue), 0, 10);
	if (Snapped != GroupTalkPlaybackGain)
	{
		GroupTalkPlaybackGain = Snapped;
		ApplyGroupTalkPlaybackGain();   // 再生中の受信音へ即反映
	}
	// スライダーのつまみ位置と数値表示を丸めた値に合わせる。
	if (GroupTalkVolumeSlider && GroupTalkVolumeSlider->GetValue() != Snapped)
	{
		GroupTalkVolumeSlider->SetValue(Snapped);
	}
	if (GroupTalkVolumeValueText)
	{
		GroupTalkVolumeValueText->SetText(FText::AsNumber(FMath::RoundToInt(GroupTalkPlaybackGain)));
	}
	RefreshSliderFills();
}

void UWG_RadNodzToolkit::HandleGroupTalkVolumeCaptureEnd()
{
	// つまみを離したタイミングで端末に保存する。
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateGroupTalkVolumeUI()
{
	if (GroupTalkVolumeSlider) { GroupTalkVolumeSlider->SetValue(GroupTalkPlaybackGain); }
	if (GroupTalkVolumeValueText) { GroupTalkVolumeValueText->SetText(FText::AsNumber(FMath::RoundToInt(GroupTalkPlaybackGain))); }
	RefreshSliderFills();
}

void UWG_RadNodzToolkit::HandleMonitorVolumeChanged(float NewValue)
{
	// ドラッグ中：1dB刻みに丸めて即反映（保存は離した時にまとめて行う）。
	const float Snapped = (float)FMath::Clamp(FMath::RoundToInt(NewValue), -12, 12);
	if (Snapped != MonitorVolumeDb)
	{
		MonitorVolumeDb = Snapped;
		ApplyMonitorVolume();   // モニター再生中の受信音へ即反映
	}
	// スライダーのつまみ位置は丸めた値に合わせる（差分時のみ。OnValueChanged再入を避ける）。
	if (MonitorVolumeSlider && MonitorVolumeSlider->GetValue() != Snapped)
	{
		MonitorVolumeSlider->SetValue(Snapped);
	}
	// 数値表示（"+5 dB"等）を直接更新する。
	if (MonitorVolumeValueText)
	{
		const int32 Db = FMath::RoundToInt(MonitorVolumeDb);
		MonitorVolumeValueText->SetText(FText::FromString(
			FString::Printf(TEXT("%s%d dB"), Db > 0 ? TEXT("+") : TEXT(""), Db)));
	}
	RefreshSliderFills();
}

void UWG_RadNodzToolkit::HandleMonitorVolumeCaptureEnd()
{
	// つまみを離したタイミングで端末に保存する。
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateMonitorVolumeUI()
{
	const int32 Db = FMath::Clamp(FMath::RoundToInt(MonitorVolumeDb), -12, 12);
	if (MonitorVolumeSlider) { MonitorVolumeSlider->SetValue((float)Db); }
	if (MonitorVolumeValueText)
	{
		// 正の値には + を付けて "+5 dB" / "-3 dB" / "0 dB" のように表示する。
		MonitorVolumeValueText->SetText(FText::FromString(
			FString::Printf(TEXT("%s%d dB"), Db > 0 ? TEXT("+") : TEXT(""), Db)));
	}
	RefreshSliderFills();
}

void UWG_RadNodzToolkit::ApplyMonitorVolume()
{
	// dB → 線形ゲイン倍率（10^(dB/20)）。±12dB は約0.25〜3.98倍。
	const float Linear = FMath::Pow(10.f, (float)MonitorVolumeDb / 20.f);
	if (AudioMonitor)    { AudioMonitor->SetPlaybackGain(Linear); }
	if (ParaknotMonitor) { ParaknotMonitor->SetPlaybackGain(Linear); }
}

void UWG_RadNodzToolkit::RefreshSliderFills()
{
	// 各スライダーの現在値(0〜1正規化)を塗り(ProgressBar)の percent に反映する。
	auto SetFill = [](UProgressBar* Fill, USlider* S)
	{
		if (!Fill || !S) { return; }
		const float Mn = S->GetMinValue();
		const float Mx = S->GetMaxValue();
		const float P = (Mx > Mn) ? (S->GetValue() - Mn) / (Mx - Mn) : 0.f;
		Fill->SetPercent(FMath::Clamp(P, 0.f, 1.f));
	};
	SetFill(MonitorVolumeFill,      MonitorVolumeSlider);
	SetFill(MarkerOffsetFill,       MarkerOffsetSlider);
	SetFill(GroupTalkVolumeFill,    GroupTalkVolumeSlider);
	SetFill(GroupTalkSendVolumeFill, GroupTalkSendVolumeSlider);
}

void UWG_RadNodzToolkit::HandleMarkerOffsetChanged(float NewValue)
{
	// ドラッグ中：1秒刻みに丸めて即反映（保存は離した時にまとめて行う）。
	const int32 Snapped = FMath::Clamp(FMath::RoundToInt(NewValue), -10, 10);
	MarkerOffsetSeconds = Snapped;
	// スライダーのつまみ位置は丸めた値に合わせる（差分時のみ。OnValueChanged再入を避ける）。
	if (MarkerOffsetSlider && MarkerOffsetSlider->GetValue() != (float)Snapped)
	{
		MarkerOffsetSlider->SetValue((float)Snapped);
	}
	// 数値表示（"-1 秒"等）を直接更新する。
	if (MarkerOffsetValueText)
	{
		MarkerOffsetValueText->SetText(FText::FromString(
			FString::Printf(TEXT("%s%d %s"), Snapped > 0 ? TEXT("+") : TEXT(""), Snapped,
				*ReaperLang::S(TEXT("秒"), TEXT("s")))));
	}
	RefreshSliderFills();
}

void UWG_RadNodzToolkit::HandleMarkerOffsetCaptureEnd()
{
	// つまみを離したタイミングで端末に保存する。
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateMarkerOffsetUI()
{
	const int32 Sec = FMath::Clamp(MarkerOffsetSeconds, -10, 10);
	if (MarkerOffsetSlider) { MarkerOffsetSlider->SetValue((float)Sec); }
	if (MarkerOffsetValueText)
	{
		// "+2 秒" / "-1 秒" / "0 秒" のように表示する。
		MarkerOffsetValueText->SetText(FText::FromString(
			FString::Printf(TEXT("%s%d %s"), Sec > 0 ? TEXT("+") : TEXT(""), Sec,
				*ReaperLang::S(TEXT("秒"), TEXT("s")))));
	}
	RefreshSliderFills();
}

void UWG_RadNodzToolkit::HandleGroupTalkSendVolumeChanged(float NewValue)
{
	// 目盛りは0〜10の整数。実ゲインはその半分（最大5.0倍）。
	const int32 Step = FMath::Clamp(FMath::RoundToInt(NewValue), 0, 10);
	const float NewGain = (float)Step * 0.5f;
	if (NewGain != GroupTalkSendGain)
	{
		GroupTalkSendGain = NewGain;
		ApplyGroupTalkSendGain();   // 送信機へ即反映
	}
	if (GroupTalkSendVolumeSlider && GroupTalkSendVolumeSlider->GetValue() != (float)Step)
	{
		GroupTalkSendVolumeSlider->SetValue((float)Step);
	}
	if (GroupTalkSendVolumeValueText)
	{
		GroupTalkSendVolumeValueText->SetText(FText::AsNumber(Step));
	}
	RefreshSliderFills();
}

void UWG_RadNodzToolkit::HandleGroupTalkSendVolumeCaptureEnd()
{
	SaveSettings();
}

void UWG_RadNodzToolkit::UpdateGroupTalkSendVolumeUI()
{
	// 目盛り＝ゲイン×2（ゲイン最大5.0 → 目盛り最大10）
	const int32 Step = FMath::Clamp(FMath::RoundToInt(GroupTalkSendGain * 2.f), 0, 10);
	if (GroupTalkSendVolumeSlider) { GroupTalkSendVolumeSlider->SetValue((float)Step); }
	if (GroupTalkSendVolumeValueText) { GroupTalkSendVolumeValueText->SetText(FText::AsNumber(Step)); }
	RefreshSliderFills();
}

void UWG_RadNodzToolkit::ApplyGroupTalkSendGain()
{
	FGroupTalkMic::SetGain(GroupTalkSendGain);   // マイク送信チェック
	if (GroupTalkSender) { GroupTalkSender->SetSendGain(GroupTalkSendGain); }   // 送信
}

void UWG_RadNodzToolkit::HandleMicCheckClicked()
{
	// マイク送信チェック：送信ゲインを掛けた音を自分へ折り返し再生し、
	// 相手に届く音をイヤホンで確認する。
	if (FGroupTalkMic::IsPreviewRunning()) { FGroupTalkMic::Stop(); }
	else                                   { FGroupTalkMic::StartPreview(GroupTalkSendGain); }
	UpdateMicCheckButton();
}

void UWG_RadNodzToolkit::UpdateMicCheckButton()
{
	using namespace ReaperCtrlUI;
	if (!MicCheckButton || !MicCheckButtonText) { return; }
	const bool bOn = FGroupTalkMic::IsPreviewRunning();
	const FString Label = bOn ? ReaperLang::S(TEXT("停止"), TEXT("Stop"))
							  : ReaperLang::S(TEXT("開始（要イヤホン）"), TEXT("Start (earphones)"));
	const FLinearColor Bg = bOn ? RecRed : AccentBlue;
	MicCheckButton->SetBackgroundColor(Bg);
	MicCheckButtonText->SetText(FText::FromString(Label));
	MicCheckButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
}

void UWG_RadNodzToolkit::ApplyGroupTalkPlaybackGain()
{
	for (int32 Ch = 0; Ch < 2; ++Ch)
	{
		if (GroupTalkReceiver[Ch]) { GroupTalkReceiver[Ch]->SetPlaybackGain(GroupTalkPlaybackGain); }
	}
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

namespace
{
	/**
	 * 軽量RPC（AZ_CheckLicense）を Rounds 回往復させ、片道の平均時間(ms)を出す。
	 *
	 * TcpClient は RPC ごとに 4 つの時刻（μs, Unixエポック）を持つ。サーバー側の
	 * 受領/返却時刻はレスポンスに載って返るので、上り・下りを別々に出せる。
	 * 表示するのは「上り」と「下り」の平均＝片道の実測値。
	 *
	 * 一度も測れなければ false（未応答・時刻が埋まらなかった）。
	 * ブロッキング通信なので、呼び出し側のスレッドで待つ点に注意。
	 */
	bool ProbeCommTimeMs(URadnodzClient* C, int32 Rounds, double& OutMs)
	{
		if (!C) { return false; }

		double Sum = 0.0;
		int32  Ok = 0;

		for (int32 i = 0; i < Rounds; ++i)
		{
			if (!C->SendTimingProbe()) { continue; }

			const int64 SendTime         = C->GetLastSendTime();          // 端末が送信した時刻
			const int64 ServerRecvTime   = C->GetLastServerRecvTime();    // サーバーが受領した時刻
			const int64 ServerReturnTime = C->GetLastServerReturnTime();  // サーバーが返した時刻
			const int64 RecvTime         = C->GetLastRecvTime();          // 端末が受け取り終えた時刻

			if (SendTime == 0 || ServerRecvTime == 0 || ServerReturnTime == 0 || RecvTime == 0)
			{
				continue;
			}

			const double UplinkUs   = static_cast<double>(ServerRecvTime - SendTime);
			const double DownlinkUs = static_cast<double>(RecvTime - ServerReturnTime);

			Sum += (UplinkUs + DownlinkUs) / 2.0 / 1000.0;
			++Ok;
		}

		if (Ok == 0) { return false; }

		OutMs = Sum / Ok;

		return true;
	}
}

void UWG_RadNodzToolkit::MeasureCommTime()
{
	if (!CommTimeText) return;

	// 本体接続が生きているならそれで測る（接続直後の1回目と30秒ごとの定期計測もここ）。
	// 往復はワーカーで行い、表示の更新だけゲームスレッドへ戻す。
	if (Client && bIsConnected)
	{
		if (bProbeRpcInFlight) { return; }

		URadnodzClient* C = Client;
		TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

		bProbeRpcInFlight = true;

		// 心拍を兼ねるので、直前の実行時刻もここで更新しておく。
		BgConnKeepLast.Store(FPlatformTime::Seconds());

		EnqueueControlRpc(TEXT("probe"), [WeakThis, C]()
		{
			double Ms = 0.0;
			const bool bOk = IsValid(C) && ProbeCommTimeMs(C, 1, Ms);

			// 端末側の時計だけで測った往復時間。
			// 上り/下りの内訳は端末とPCの時計ずれを含むため、ポーリング間隔の
			// 自動調整にはこちら（ずれの影響を受けない値）を使う。
			double RttMs = 0.0;

			if (bOk)
			{
				const int64 SendTime = C->GetLastSendTime();
				const int64 RecvTime = C->GetLastRecvTime();

				if (SendTime > 0 && RecvTime > SendTime)
				{
					RttMs = static_cast<double>(RecvTime - SendTime) / 1000.0;
				}
			}

			AsyncTask(ENamedThreads::GameThread, [WeakThis, bOk, Ms, RttMs]()
			{
				UWG_RadNodzToolkit* Self = WeakThis.Get();
				if (!Self) { return; }

				Self->bProbeRpcInFlight = false;

				if (RttMs > 0.0) { Self->LastRoundTripMs.Store(RttMs); }

				if (Self->CommTimeText)
				{
					Self->CommTimeText->SetText(FText::FromString(bOk
						? FString::Printf(TEXT("%.3f ms"), Ms)
						: ReaperLang::S(TEXT("応答なし"), TEXT("No response"))));
				}
			});
		});

		return;
	}

	// 未接続なら、この計測専用のクライアントで RADNODZ へ疎通してから測る。
	MeasureCommTimeByConnecting();
}

void UWG_RadNodzToolkit::MeasureCommTimeByConnecting()
{
	using namespace ReaperCtrlUI;

	if (!CommTimeText || bIsMeasuringCommTime) { return; }

	const FString Ip = IPAddressInput ? IPAddressInput->GetText().ToString().TrimStartAndEnd() : IPAddress;
	const int32   P  = ResolvePortFromInput();
	if (PortInput) { PortInput->SetText(FText::FromString(FString::FromInt(P))); }
	if (Ip.IsEmpty())
	{
		CommTimeText->SetText(FText::FromString(ReaperLang::S(TEXT("接続先が未設定"), TEXT("No destination"))));
		return;
	}

	URadnodzClient* C = URadnodzClient::CreateRadnodzClient();
	if (!C)
	{
		CommTimeText->SetText(FText::FromString(ReaperLang::S(TEXT("測定不可"), TEXT("Unavailable"))));
		return;
	}
	// 計測中に GC 回収されないようルート登録する。
	C->AddToRoot();

	// 本体接続と同じ条件で繋ぐ（常に TLS。鍵・ピン留めも同じものを使う）。
	{
		const int32 Idx = FindServerAuthIndex(GetSelectedServerId());
		if (Idx == INDEX_NONE || ServerAuthEntries[Idx].Fingerprint.IsEmpty())
		{
			C->RemoveFromRoot();
			CommTimeText->SetText(FText::FromString(ReaperLang::S(TEXT("サーバー未選択"), TEXT("No server"))));
			return;
		}
		C->SetStorageDirectory(GetAuthStorageDir());
		if (!C->GenerateIdentity())
		{
			C->RemoveFromRoot();
			CommTimeText->SetText(FText::FromString(ReaperLang::S(TEXT("鍵の用意に失敗"), TEXT("No identity"))));
			return;
		}
		C->SetServerId(GetServerPinKey(ServerAuthEntries[Idx]));
		C->SetExpectedServerFingerprint(ServerAuthEntries[Idx].Fingerprint);
	}

	const int32 MyGen = ++CommTimeGeneration;
	bIsMeasuringCommTime = true;
	CommTimeText->SetText(FText::FromString(ReaperLang::S(TEXT("測定中..."), TEXT("Measuring..."))));

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);
	Async(EAsyncExecution::Thread, [WeakThis, C, Ip, P, MyGen]()
	{
		bool   bConnected = false;
		bool   bOk = false;
		double Ms = 0.0;

		if (C)
		{
			bConnected = C->Connect(Ip, P);
			if (bConnected)
			{
				// 3往復の平均を採る。ハンドシェイク直後の1往復目は遅めに出やすいので、
				// 1回だけだと実際より悪い値が残る。
				bOk = ProbeCommTimeMs(C, 3, Ms);
				C->Disconnect();
			}
		}

		// 結果はゲームスレッドで反映する（UMG はゲームスレッド限定）
		AsyncTask(ENamedThreads::GameThread, [WeakThis, C, bConnected, bOk, Ms, MyGen]()
		{
			if (UWG_RadNodzToolkit* Self = WeakThis.Get())
			{
				// 古い計測の結果は捨てる（押し直し等で世代が進んでいる場合）。
				if (Self->CommTimeGeneration == MyGen)
				{
					Self->bIsMeasuringCommTime = false;
					if (Self->CommTimeText)
					{
						Self->CommTimeText->SetText(FText::FromString(
							!bConnected ? ReaperLang::S(TEXT("接続失敗"), TEXT("Connect failed"))
							: bOk        ? FString::Printf(TEXT("%.3f ms"), Ms)
							             : ReaperLang::S(TEXT("応答なし"), TEXT("No response"))));
					}
				}
			}
			if (IsValid(C)) { C->RemoveFromRoot(); }
		});
	});
}

void UWG_RadNodzToolkit::HandleTrackRefreshModeClicked()
{
	bTrackListAutoRefresh = !bTrackListAutoRefresh;
	TrackRefreshTimer = 0.0;
	// 自動更新に切り替えたら、そのまま動き出す（ホームの「更新」トグルも自動でONにする）。
	// 手動に戻したときは停止状態にしておく。
	bTrackListAutoRefreshRunning = bTrackListAutoRefresh;
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

void UWG_RadNodzToolkit::TickConnectionHealth(float DeltaSeconds)
{
	// 原因調査用の計測：切断はせず、応答の有無だけをログに残す。
	// 「モニター再生→停止→OFF」で本当にRPCが通らなくなるのかを、実操作と突き合わせて確かめる。
	// 疎通の確認は制御用キューで行い、判定とログ出力はゲームスレッドへ戻す。
	if (bConnHealthProbeLogOnly && bIsConnected && !bIsConnecting && Client && !bHealthProbeInFlight)
	{
		ConnHealthTimer += DeltaSeconds;
		if (ConnHealthTimer >= ConnHealthIntervalSec)
		{
			ConnHealthTimer = 0.0;

			URadnodzClient* C = Client;
			TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);
			const int32 MonitorOn = IsMonitorActive() ? 1 : 0;
			const int32 SendingOn = (ComputeTransmitTarget() >= 0) ? 1 : 0;

			bHealthProbeInFlight = true;

			EnqueueControlRpc(TEXT("health"), [WeakThis, C, MonitorOn, SendingOn]()
			{
				const bool bOk = IsValid(C) && C->SendTimingProbe();

				AsyncTask(ENamedThreads::GameThread, [WeakThis, bOk, MonitorOn, SendingOn]()
				{
					UWG_RadNodzToolkit* Self = WeakThis.Get();
					if (!Self) { return; }

					Self->bHealthProbeInFlight = false;

					if (bOk) { Self->ConnHealthFailCount = 0; }
					else     { ++Self->ConnHealthFailCount; }

					UE_LOG(LogTemp, Verbose, TEXT("[RIGDRED Probe] %s 連続失敗=%d モニター=%d 送信中=%d"),
						bOk ? TEXT("OK") : TEXT("NG"),
						Self->ConnHealthFailCount, MonitorOn, SendingOn);
				});
			});
		}
	}

	// 【現在は無効】応答が無いことを検出しても切断しない（誤検出による切断→再接続の繰り返しで、
	// サーバー側に不要な接続を増やさないため）。
	if (!bConnHealthCheckEnabled) return;

	// 接続中のときだけ監視する（接続試行中は結果待ちなので触らない）。
	if (!bIsConnected || bIsConnecting || !Client)
	{
		ConnHealthTimer = 0.0;
		ConnHealthFailCount = 0;
		return;
	}

	if (bHealthProbeInFlight) return;

	ConnHealthTimer += DeltaSeconds;
	if (ConnHealthTimer < ConnHealthIntervalSec) return;
	ConnHealthTimer = 0.0;

	// 軽量RPC（AZ_GetErrorCode相当）を1往復。生きていれば成功する。
	// 往復はキューで行い、判定はゲームスレッドへ戻ってから。
	URadnodzClient* C = Client;
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	bHealthProbeInFlight = true;

	EnqueueControlRpc(TEXT("health"), [WeakThis, C]()
	{
		const bool bOk = IsValid(C) && C->SendTimingProbe();

		AsyncTask(ENamedThreads::GameThread, [WeakThis, bOk]()
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			Self->bHealthProbeInFlight = false;

			if (bOk)
			{
				Self->ConnHealthFailCount = 0;
				return;
			}

			// モニター再生や通話でRPCが一時的に詰まることがあるので、連続失敗が続いたときだけ切断とみなす。
			// 15秒×4回＝約60秒まったく応答が無い状態は、詰まりではなく接続が死んでいる。
			if (++Self->ConnHealthFailCount < ConnHealthFailThreshold) { return; }

			UE_LOG(LogTemp, Warning, TEXT("[RIGDRED] Reaperが約%.0f秒応答しないため切断とみなします（半開き検出）"),
				ConnHealthIntervalSec * ConnHealthFailThreshold);

			Self->ConnHealthFailCount = 0;
			Self->Disconnect();             // 切断状態へ遷移し、切断UIを反映する
			Self->RequestAutoReconnect();   // 自動再接続を予約
		});
	});
}

void UWG_RadNodzToolkit::RequestAutoReconnect()
{
	// 意図しない切断からの復帰を予約する。試行回数は接続成功／手動操作でリセットされる。
	bAutoReconnectPending = true;
	ConnReconnectTimer = 0.0;
}

void UWG_RadNodzToolkit::TickAutoReconnect(float DeltaSeconds)
{
	if (!bAutoReconnectPending) return;

	// 接続できた、または手動で接続処理が走っているなら予約を解除する。
	if (bIsConnected || bIsConnecting)
	{
		bAutoReconnectPending = false;
		ConnReconnectTimer = 0.0;
		return;
	}

	// 一定間隔を空けてから試す（回線復帰待ちの猶予を作る）。
	ConnReconnectTimer += DeltaSeconds;
	if (ConnReconnectTimer < ConnReconnectDelaySec) return;
	ConnReconnectTimer = 0.0;

	if (ConnAutoReconnectTries >= ConnAutoReconnectMaxTries)
	{
		// 上限まで試してダメなら未接続のまま止め、ユーザーの操作に委ねる。
		UE_LOG(LogTemp, Warning, TEXT("[RIGDRED] 自動再接続を %d 回試みましたが復帰しませんでした"), ConnAutoReconnectMaxTries);
		bAutoReconnectPending = false;
		return;
	}

	++ConnAutoReconnectTries;
	UE_LOG(LogTemp, Warning, TEXT("[RIGDRED] 自動再接続 %d/%d 回目"), ConnAutoReconnectTries, ConnAutoReconnectMaxTries);
	Connect();
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
void UWG_RadNodzToolkit::StartMediaCacheWarm(bool bRefreshExisting)
{
	bMediaWarmActive = true;
	bMediaWarmRefresh = bRefreshExisting;
	MediaWarmTrackIndex = 0;
	MediaWarmTimer = 0.0;

	// 走査を最初からやり直すので、前の周回で飛んでいる取得の結果は捨てる。
	// （そのまま受け取ると、リセットしたばかりの走査位置を進めてしまう）
	++MediaWarmGeneration;
	bWarmRpcInFlight = false;
}

void UWG_RadNodzToolkit::TickMediaCacheWarm(float DeltaSeconds)
{
	if (!bMediaWarmActive) return;
	if (!Client || !bIsConnected) { bMediaWarmActive = false; bMediaWarmRefresh = false; return; }
	// 録音/再生中は通信を控える（操作と競合させない）
	if (bIsRecording || bIsPlaying) return;

	// 前回ぶんの取得が終わるまで次へ進まない（要求を積み増さない）。
	if (bWarmRpcInFlight) return;

	// 1トラック/間隔ずつ取得して、重い呼び出しを分散（UIを止めない）。
	// 間隔は回線品質に応じて自動で伸びる。
	MediaWarmTimer += DeltaSeconds;
	const double Interval = 0.25 * GetPollingScale();
	if (MediaWarmTimer < Interval) return;
	MediaWarmTimer = 0.0;

	// 全トラック走査が終わったら停止
	if (!TrackList.IsValidIndex(MediaWarmTrackIndex))
	{
		bMediaWarmActive = false;
		bMediaWarmRefresh = false;
		return;
	}

	const int32 Idx = TrackList[MediaWarmTrackIndex].Index;

	// 「更新」直後はキャッシュ済みでも取り直す。そうでなければ未キャッシュのみ。
	const bool bRefresh = bMediaWarmRefresh;

	if (!bRefresh && TrackMediaCache.Contains(Idx))
	{
		++MediaWarmTrackIndex;
		return;
	}

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	bWarmRpcInFlight = true;

	const int32 MyGeneration = MediaWarmGeneration;

	// 先読みは解析用キュー（2本目の接続）で流す。操作系の往復を待たせないため。
	FetchTrackMediaItemsAsync(Idx, /*bHeavy=*/true, NAME_None,
		[WeakThis, Idx, bRefresh, MyGeneration](const TArray<UMediaItem*>& Items,
			const TArray<FString>& Names, const TArray<double>& Starts)
		{
			UWG_RadNodzToolkit* Self = WeakThis.Get();
			if (!Self) { return; }

			// 走査がやり直された後に届いた結果は捨てる。
			if (MyGeneration != Self->MediaWarmGeneration) { return; }

			Self->bWarmRpcInFlight = false;
			++Self->MediaWarmTrackIndex;

			// キャッシュの書き込みはゲームスレッドだけで行う
			// （切断時の破棄と競合させない）。
			const FTrackMediaCacheEntry* Old = Self->TrackMediaCache.Find(Idx);
			const bool bChanged = !Old || Old->Names != Names || Old->Starts != Starts;

			FTrackMediaCacheEntry NewEntry;
			NewEntry.Items.Reserve(Items.Num());
			for (UMediaItem* It : Items) { NewEntry.Items.Add(It); }
			NewEntry.Names  = Names;
			NewEntry.Starts = Starts;
			Self->TrackMediaCache.Add(Idx, MoveTemp(NewEntry));

			// 取り直しのときだけ、中身が変わった展開中の一覧を作り直す
			// （変化が無ければ描画には一切触らない）。
			if (bRefresh && bChanged)
			{
				for (UWG_TrackCard* Card : Self->TrackCards)
				{
					if (Card && Card->TrackIndex == Idx)
					{
						Card->RebuildMediaListIfExpanded();
					}
				}
			}
		});
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
	// マーカー表示の再生中行(▶/■)も同じトランスポート状態に追従させる。
	NotifyTransportToMarkerRow();
}

void UWG_RadNodzToolkit::PlayFromMediaRow(UWG_MediaItemRow* Row, UMediaItem* MediaItem, double StartSeconds)
{
	if (!Client || !bIsConnected) return;

	// 既に再生/録音中なら一度停止する。
	// （再生中に再生を再発行してもReaperはシークしないため、必ず止めてから新位置で再生する）
	if (bIsPlaying || bIsRecording)
	{
		// 停止 → カーソル移動 → 再生 は同じ制御用キューに順番に積まれるので、
		// 実行順は保たれる（ゲームスレッドは待たない）。
		FireAndForgetRpc({}, [](URadnodzClient* C)
		{
			URigdocksBronze_Cursor::AZ_StopPlaying(C);
		});

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

	// リストを再取得してUIを更新する。取得は非同期なので、
	// 「追加されたトラックを特定する」処理は完了後に実行する。
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	FetchTracksAsync([WeakThis, bArmAfterAdd]()
	{
		UWG_RadNodzToolkit* Self = WeakThis.Get();
		if (!Self) { return; }

		// 追加されたトラックを特定してイベントを発行
		if (Self->TrackList.Num() > 0)
		{
			const FTrackDetail& Added = Self->TrackList.Last();
			Self->OnTrackAdded(Added);

			if (bArmAfterAdd)
			{
				Self->SetTrackArmed(Added.Index, true);
			}
		}
	});
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

	// 一覧の取り直しは非同期。特定とイベント発行は完了後に行う。
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

	FetchTracksAsync([WeakThis, TrackName, OutTrackId, bArmAfterAdd]()
	{
		UWG_RadNodzToolkit* Self = WeakThis.Get();
		if (!Self) { return; }

		// 追加されたトラックを名前で特定
		for (const FTrackDetail& Detail : Self->TrackList)
		{
			if (Detail.Name == TrackName && Detail.Index == OutTrackId)
			{
				Self->OnTrackAdded(Detail);
				if (bArmAfterAdd)
				{
					Self->SetTrackArmed(Detail.Index, true);
				}
				break;
			}
		}
	});

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
		// 全子トラックの生成＋設定を1往復のバッチにまとめる。
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
	// 親生成・子生成・各種設定をすべて1往復のバッチにまとめる。
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

// ===== ホーム「アクション」ドロップダウン =====

void UWG_RadNodzToolkit::RegisterActions()
{
	if (Actions.Num() > 0) { return; }

	// ドロップダウンの並び順。新しいアクションはここに追加する。
	Actions.Add(NewObject<UAction_IRMeasurementPrepare>(this));
	Actions.Add(NewObject<UAction_IRRecord>(this));
	Actions.Add(NewObject<UAction_IRMeasurement>(this));
}

void UWG_RadNodzToolkit::RefreshActionCombo()
{
	if (!ActionCombo) { return; }

	const int32 Selected = ActionCombo->GetSelectedIndex();

	ActionCombo->ClearOptions();
	for (const URadNodzAction* Action : Actions)
	{
		ActionCombo->AddOption(Action ? Action->GetDisplayName() : FString());
	}

	// 言語切替で作り直しても選択は維持する（未選択のままなら未選択）
	if (Selected >= 0 && Selected < Actions.Num())
	{
		ActionCombo->SetSelectedIndex(Selected);
	}
	else
	{
		ActionCombo->ClearSelection();
	}

	UpdateActionRunButton();
}

void UWG_RadNodzToolkit::UpdateActionRunButton()
{
	if (!ActionRunButton) { return; }

	const bool bSelected = ActionCombo && Actions.IsValidIndex(ActionCombo->GetSelectedIndex());
	ActionRunButton->SetIsEnabled(bSelected && !bActionBusy);
}

void UWG_RadNodzToolkit::HandleActionComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UpdateActionRunButton();
}

void UWG_RadNodzToolkit::HandleActionRunClicked()
{
	if (bIsLocked || bActionBusy || !ActionCombo) { return; }

	URadNodzAction* Action = Actions.IsValidIndex(ActionCombo->GetSelectedIndex())
		? Actions[ActionCombo->GetSelectedIndex()].Get() : nullptr;
	if (!Action) { return; }

	BeginActionBusy();
	Action->Execute(this);
}

void UWG_RadNodzToolkit::BeginActionBusy()
{
	bActionBusy = true;
	UpdateActionRunButton();
}

void UWG_RadNodzToolkit::EndActionBusy()
{
	bActionBusy = false;
	UpdateActionRunButton();
}

bool UWG_RadNodzToolkit::RunActionBatchAsync(URadnodzRpcBatch* Batch, bool bHeavy, TFunction<void()> OnDone,
	const TArray<UObject*>& KeepAlive)
{
	return ExecuteBatchAsync(Batch, KeepAlive, NAME_None, bHeavy, MoveTemp(OnDone));
}

void UWG_RadNodzToolkit::ShowActionInfo(const FString& Title, const FString& Message)
{
	if (UWG_InfoDialog* Dlg = CreateWidget<UWG_InfoDialog>(this, UWG_InfoDialog::StaticClass()))
	{
		Dlg->Setup(Title, Message);
		Dlg->AddToViewport(200);
	}
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
		UE_LOG(LogTemp, Verbose, TEXT("[ReaperInput] GetInputChannelNames -> %d 件"), Names.Num());
		return Names;
	}
	UE_LOG(LogTemp, Verbose, TEXT("[ReaperInput] GetInputChannelNames -> 未接続"));
	return TArray<FString>();
}

void UWG_RadNodzToolkit::SetTrackInputChannel(UMediaTrack* Track, int32 FirstChannel, int32 NumChannels)
{
	if (!Client || !bIsConnected || !Track) return;

	const int32 First = FMath::Max(0, FirstChannel);
	const int32 Num = FMath::Max(1, NumChannels);

	FireAndForgetRpc({ Track }, [Track, First, Num](URadnodzClient* C)
	{
		URigdocksSilver_Track::AZ_SetTrackItemInputChannel(C, Track, First, Num, AZ_InputChannelType::Audio);
	});
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

	FireAndForgetRpc({ Proj }, [Proj, TrackIndex, NewName](URadnodzClient* C)
	{
		URigdocksSilver_Track::AZ_SetTrackIdName(C, Proj, TrackIndex, NewName);
	});

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
	// すべて1往復の RpcBatch にまとめる（REC押下時にゲームスレッドを長くブロックしないため）。
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

	FireAndForgetRpc({}, [](URadnodzClient* C)
	{
		URigdocksBronze_Cursor::AZ_StartPlaying(C);
	});

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

	FireAndForgetRpc({}, [](URadnodzClient* C)
	{
		URigdocksBronze_Cursor::AZ_StopPlaying(C);
	});

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
	// ローカルIR中はREAPERではなく端末のTSP再生状態を見る。
	if (PlayButton && PlayButtonText)
	{
		const bool bPlayingNow = bLocalIRMode ? bLocalIRPlaying : bIsPlaying;
		PlayButtonText->SetText(FText::FromString(bPlayingNow
			? ReaperLang::S(TEXT("■ 停止"), TEXT("■ STOP"))
			: ReaperLang::S(TEXT("▶ 再生"), TEXT("▶ PLAY"))));
		const FLinearColor Bg = bPlayingNow ? StopGray : AccentGreen;
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

	// 再生/録音中は「削除」ボタンを「マーカー」ボタンへ切り替える（停止時は元へ戻す）。
	UpdateDeleteMarkerButton();
}

// ====================================================================
//  ローカルIR（REAPERに繋がず、端末単体でTSP信号を鳴らすモード）
//
//  「接続」ボタンの下の「ローカルIR」でホーム画面を切り替える。
//  再生ボタンを押すと、10秒のTSP（対数スイープ）＋指定秒の無音を
//  指定回数くり返して端末から鳴らす。
// ====================================================================
namespace
{
	// TSPの仕様はREAPER側で使っている素材（10秒スイープ＋無音・-6dBFS・20Hzから上昇）に合わせている。
	constexpr double LocalIRSweepSeconds   = 10.0;
	constexpr double LocalIRSweepStartHz   = 20.0;
	constexpr double LocalIRSweepAmp       = 0.5011872;   // -6 dBFS
	constexpr double LocalIRSweepFadeSec   = 0.010;       // 末尾のフェードアウト（切れ際のノイズ防止）
	constexpr double LocalIRTwoPi          = 6.28318530717958647692;   // 位相は倍精度で積むのでπも倍精度で持つ
	constexpr int32  LocalIRMaxPlayCount   = 99;
	constexpr int32  LocalIRMaxIntervalSec = 15;
}

void UWG_RadNodzToolkit::HandleLocalIRClicked()
{
	SetLocalIRMode(!bLocalIRMode);
}

void UWG_RadNodzToolkit::HandleLocalIRPlusClicked()
{
	LocalIRPlayCount = FMath::Clamp(LocalIRPlayCount + 1, 1, LocalIRMaxPlayCount);
	UpdateLocalIRUI();
	SaveSettings();
}

void UWG_RadNodzToolkit::HandleLocalIRMinusClicked()
{
	LocalIRPlayCount = FMath::Clamp(LocalIRPlayCount - 1, 1, LocalIRMaxPlayCount);
	UpdateLocalIRUI();
	SaveSettings();
}

void UWG_RadNodzToolkit::HandleLocalIRIntervalChanged(float NewValue)
{
	// ドラッグ中：1秒刻みに丸めて即反映（保存は離した時）。
	const int32 Snapped = FMath::Clamp(FMath::RoundToInt(NewValue), 0, LocalIRMaxIntervalSec);
	LocalIRIntervalSeconds = Snapped;
	// つまみ位置は丸めた値に合わせる（差分時のみ。OnValueChangedの再入を避ける）。
	if (LocalIRIntervalSlider && LocalIRIntervalSlider->GetValue() != (float)Snapped)
	{
		LocalIRIntervalSlider->SetValue((float)Snapped);
	}
	UpdateLocalIRUI();
}

void UWG_RadNodzToolkit::HandleLocalIRIntervalCaptureEnd()
{
	SaveSettings();
}

// ホーム画面をローカルIR⇄通常へ切り替える。
void UWG_RadNodzToolkit::SetLocalIRMode(bool bOn)
{
	using namespace ReaperCtrlUI;

	// モードを抜けるときは鳴っているTSPを止める。
	if (!bOn && bLocalIRPlaying) { StopLocalIRPlayback(); }

	bLocalIRMode = bOn;

	// 一覧（黒い領域）：トラック/マーカー ⇄ 回数/間隔の大表示
	if (TrackMarkerTabRow)  { TrackMarkerTabRow->SetVisibility(bOn ? ESlateVisibility::Collapsed : ESlateVisibility::Visible); }
	if (LocalIRCountRow)    { LocalIRCountRow->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); }
	if (LocalIRCenterPanel) { LocalIRCenterPanel->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); }
	if (bOn)
	{
		if (TrackListScrollBox)  { TrackListScrollBox->SetVisibility(ESlateVisibility::Collapsed); }
		if (MarkerListScrollBox) { MarkerListScrollBox->SetVisibility(ESlateVisibility::Collapsed); }
		if (MarkerEmptyText)     { MarkerEmptyText->SetVisibility(ESlateVisibility::Collapsed); }
	}
	else
	{
		// 元のトラック/マーカー一覧へ戻す（選択していた側を復元する）。
		SetMarkerViewActive(bMarkerViewActive);
	}

	// ローカルIRボタン自身の見た目（ON＝ティール）
	if (LocalIRButton && LocalIRButtonText)
	{
		const FLinearColor Bg = bOn ? AccentBlue : StopGray;
		LocalIRButton->SetBackgroundColor(Bg);
		LocalIRButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(Bg)));
	}

	ApplyLocalIRLayout(HomeOrientationApplied == 1);
	UpdateTransportButtons();
	UpdateLocalIRUI();
}

// ローカルIR中に隠すもの（ツールバー・状態表示・削除/全解除/計測調整/Slack/ロック・録音）と、
// 空いた分だけ大きくする再生ボタンの高さを適用する。向きが変わったときにも呼び直す。
void UWG_RadNodzToolkit::ApplyLocalIRLayout(bool bLandscape)
{
	const ESlateVisibility NormalVis = bLocalIRMode ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;

	if (ToolbarPanel)       { ToolbarPanel->SetVisibility(NormalVis); }        // 追加/モニター/メーター/更新＋アクション行
	if (FooterStateOverlay) { FooterStateOverlay->SetVisibility(NormalVis); }  // ■停止中
	if (InfoRowsWrap)       { InfoRowsWrap->SetVisibility(NormalVis); }        // 削除/全解除/計測調整/Slack/ロック
	if (RecordButtonSize)   { RecordButtonSize->SetVisibility(NormalVis); }    // 録音

	// 再生ボタン：ローカルIR中は隠したボタンの分も含めて大きくする。
	if (PlayButtonSize)
	{
		const float H = bLocalIRMode ? (bLandscape ? 200.f : 300.f) : (bLandscape ? 90.f : 140.f);
		PlayButtonSize->SetMinDesiredHeight(H);
		// 録音ボタンが消えるので右の間隔も要らない。
		if (UHorizontalBoxSlot* PS = Cast<UHorizontalBoxSlot>(PlayButtonSize->Slot))
		{
			const float RightPad = bLocalIRMode ? 0.f : (bLandscape ? 6.f : 16.f);
			PS->SetPadding(FMargin(0.f, 0.f, RightPad, 0.f));
		}
	}

	// ロック中にローカルIRへ入るとロックボタンが隠れて解除できないため、
	// ローカルIRの再生ボタンだけはロックに関係なく押せるようにする。
	if (PlayButton) { PlayButton->SetIsEnabled(bLocalIRMode ? true : !bIsLocked); }
}

// 回数・間隔・再生進捗の表示を現在値に合わせる。
void UWG_RadNodzToolkit::UpdateLocalIRUI()
{
	const int32 Count    = FMath::Clamp(LocalIRPlayCount, 1, LocalIRMaxPlayCount);
	const int32 Interval = FMath::Clamp(LocalIRIntervalSeconds, 0, LocalIRMaxIntervalSec);

	if (LocalIRCountValueText)
	{
		LocalIRCountValueText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
	}
	if (LocalIRIntervalValueText)
	{
		LocalIRIntervalValueText->SetText(FText::FromString(
			FString::Printf(TEXT("%d %s"), Interval, *ReaperLang::S(TEXT("秒"), TEXT("s")))));
	}
	if (LocalIRIntervalSlider && LocalIRIntervalSlider->GetValue() != (float)Interval)
	{
		LocalIRIntervalSlider->SetValue((float)Interval);
	}
	if (LocalIRIntervalFill)
	{
		LocalIRIntervalFill->SetPercent((float)Interval / (float)LocalIRMaxIntervalSec);
	}

	// 再生中は「2 / 5」と進捗を出す。停止中は空にする。
	if (LocalIRProgressText)
	{
		FString Progress;
		if (bLocalIRPlaying)
		{
			const double CycleSec = LocalIRSweepSeconds + (double)Interval;
			const double Elapsed  = FPlatformTime::Seconds() - LocalIRPlayStartTime;
			const int32  Current  = FMath::Clamp((int32)(Elapsed / CycleSec) + 1, 1, Count);
			Progress = FString::Printf(TEXT("%s %d / %d"),
				*ReaperLang::S(TEXT("再生中"), TEXT("Playing")), Current, Count);
		}
		LocalIRProgressText->SetText(FText::FromString(Progress));
	}

	// ＋／－・間隔は再生中さわれないようにする（鳴らしている途中で変わると分かりにくいため）。
	if (LocalIRPlusButton)     { LocalIRPlusButton->SetIsEnabled(!bLocalIRPlaying); }
	if (LocalIRMinusButton)    { LocalIRMinusButton->SetIsEnabled(!bLocalIRPlaying); }
	if (LocalIRIntervalSlider) { LocalIRIntervalSlider->SetIsEnabled(!bLocalIRPlaying); }
}

// 1回分（TSPスイープ＋間隔の無音）のPCMを作る。
// スイープは対数（指数）掃引：f(t) = f1 * exp(k t)、位相 = 2πf1/k * (exp(k t) - 1)。
void UWG_RadNodzToolkit::BuildLocalIRCyclePcm()
{
	const int32 SR = FMath::Max(LocalIRSampleRate, 8000);
	const int32 SweepN = FMath::RoundToInt(LocalIRSweepSeconds * SR);
	const int32 GapN   = FMath::Clamp(LocalIRIntervalSeconds, 0, LocalIRMaxIntervalSec) * SR;

	// 上端は再生レートのナイキストまで（素材も192kHzのナイキスト近くまで上げている）。
	const double F1 = LocalIRSweepStartHz;
	const double F2 = (double)SR * 0.5;
	const double K  = FMath::Loge(F2 / F1) / LocalIRSweepSeconds;

	LocalIRCyclePcm.Reset();
	LocalIRCyclePcm.SetNumZeroed(SweepN + GapN);   // 後半の無音はゼロのまま

	for (int32 i = 0; i < SweepN; ++i)
	{
		const double T = (double)i / (double)SR;
		const double Phase = LocalIRTwoPi * F1 / K * (FMath::Exp(K * T) - 1.0);
		double V = LocalIRSweepAmp * FMath::Sin(Phase);

		// 終端だけ短くフェードアウトする（最高域で急に切れるときのプツッを防ぐ）。
		const double Remain = LocalIRSweepSeconds - T;
		if (Remain < LocalIRSweepFadeSec) { V *= FMath::Max(Remain, 0.0) / LocalIRSweepFadeSec; }

		LocalIRCyclePcm[i] = (int16)FMath::Clamp(V * 32767.0, -32767.0, 32767.0);
	}
}

// TSPの再生を開始する（回数ぶんくり返す）。
void UWG_RadNodzToolkit::StartLocalIRPlayback()
{
	StopLocalIRPlayback();   // 念のため前回分を片付ける

	LocalIRSampleRate = 48000;   // 端末の既定レート。異なる場合はエンジン側でリサンプルされる
	BuildLocalIRCyclePcm();
	if (LocalIRCyclePcm.Num() <= 0) { return; }

	LocalIRSoundWave = NewObject<USoundWaveProcedural>(this);
	LocalIRSoundWave->SetSampleRate(LocalIRSampleRate);
	LocalIRSoundWave->NumChannels = 1;
	LocalIRSoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	LocalIRSoundWave->SoundGroup = SOUNDGROUP_Default;
	LocalIRSoundWave->bLooping = false;

	LocalIRAudioComponent = UGameplayStatics::CreateSound2D(this, LocalIRSoundWave);
	if (!LocalIRAudioComponent)
	{
		LocalIRSoundWave = nullptr;
		LocalIRCyclePcm.Empty();
		return;
	}
	LocalIRAudioComponent->bAutoDestroy = false;

	LocalIRCycleCursor    = 0;
	LocalIRCyclesLeft     = FMath::Clamp(LocalIRPlayCount, 1, LocalIRMaxPlayCount);
	bLocalIRPlaying       = true;
	LocalIRPlayStartTime  = FPlatformTime::Seconds();
	LocalIRDrainStartTime = 0.0;

	TickLocalIRPlayback();   // 先に数秒分を積んでから鳴らし始める
	LocalIRAudioComponent->Play();

	UpdateTransportButtons();
	UpdateLocalIRUI();
}

// 再生を止めて音声リソースを解放する。
void UWG_RadNodzToolkit::StopLocalIRPlayback()
{
	if (LocalIRAudioComponent)
	{
		LocalIRAudioComponent->Stop();
		LocalIRAudioComponent->DestroyComponent();
		LocalIRAudioComponent = nullptr;
	}
	if (LocalIRSoundWave)
	{
		LocalIRSoundWave->ResetAudio();
		LocalIRSoundWave = nullptr;
	}
	LocalIRCyclePcm.Empty();
	LocalIRCycleCursor    = 0;
	LocalIRCyclesLeft     = 0;
	LocalIRDrainStartTime = 0.0;

	if (bLocalIRPlaying)
	{
		bLocalIRPlaying = false;
		UpdateTransportButtons();
		UpdateLocalIRUI();
	}
}

// NativeTickから：キューが減ったらPCMを補充し、鳴り終わったら止める。
void UWG_RadNodzToolkit::TickLocalIRPlayback()
{
	if (!bLocalIRPlaying || !LocalIRSoundWave) { return; }

	// 2秒分を下回ったら1秒ずつ積み増す（毎フレーム呼ばれるので余裕をもって先行させる）。
	const int32 ChunkSamples  = LocalIRSampleRate;
	const int32 LowWaterBytes = LocalIRSampleRate * 2 * (int32)sizeof(int16);
	while (LocalIRCyclesLeft > 0 &&
		(int32)LocalIRSoundWave->GetAvailableAudioByteCount() < LowWaterBytes)
	{
		const int32 N = FMath::Min(ChunkSamples, LocalIRCyclePcm.Num() - LocalIRCycleCursor);
		if (N <= 0) { break; }
		LocalIRSoundWave->QueueAudio(
			reinterpret_cast<const uint8*>(LocalIRCyclePcm.GetData() + LocalIRCycleCursor),
			N * sizeof(int16));
		LocalIRCycleCursor += N;
		if (LocalIRCycleCursor >= LocalIRCyclePcm.Num())
		{
			LocalIRCycleCursor = 0;
			--LocalIRCyclesLeft;   // 1回分を積み終えた
		}
	}

	// 全部積み終わってキューも空になったら、出力に残った分を鳴らし切ってから止める。
	if (LocalIRCyclesLeft <= 0 && LocalIRSoundWave->GetAvailableAudioByteCount() == 0)
	{
		if (LocalIRDrainStartTime <= 0.0)
		{
			LocalIRDrainStartTime = FPlatformTime::Seconds();
		}
		else if (FPlatformTime::Seconds() - LocalIRDrainStartTime > 0.5)
		{
			StopLocalIRPlayback();
			return;
		}
	}

	// 進捗（再生中 n / N）の更新
	UpdateLocalIRUI();
}

void UWG_RadNodzToolkit::NativeDestruct()
{
	// ローカルIRのTSP再生を止める（音声リソースを解放）
	StopLocalIRPlayback();

	// カウントダウンオーバーレイが残っていれば片付ける（録画サービス自体は独立して継続する）
	HideScreenRecCountdown();

	// バックグラウンド継続を停止（前面サービス停止・監視スレッド終了・メンテナンス解除）
	FReaperKeepAlive::SetBackgroundMaintenance(nullptr);
	FReaperKeepAlive::SetActive(false);

	// 音声モニターを停止（方式を問わず。UDPソケット・再生を解放）
	StopMonitor();
	// マイク送信を停止（方式を問わず）
	StopMicSend();
	// グループトークの送受信を停止
	if (GroupTalkSender)
	{
		GroupTalkSender->Stop();
	}
	for (int32 Ch = 0; Ch < 2; ++Ch)
	{
		if (GroupTalkReceiver[Ch]) { GroupTalkReceiver[Ch]->Stop(); }
	}
	FGroupTalkMic::Stop();   // マイク送信チェックが動いていれば止める

	// 非同期接続の実行中はClientに触れない（バックグラウンドのConnectと競合させない）
	if (Client && bIsConnected && !bIsConnecting)
	{
		MarkPresenceOffline();
		Client->Disconnect();
	}

	ShutdownHeavyClient();

	// **接続を落としてからRPCスレッドを畳む。**
	// こうしておくと実行中のジョブは即座に戻るので、ここで待たされない。
	// （逆順だと「応答待ちのジョブの完了待ち」でアプリ終了が固まる）
	ShutdownRpcQueues();

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

	// カードは「位置ごとに使い回す」。トラックが増減した場合も、先頭から共通する範囲の
	// カードは作り直さず中身だけ差分更新し、増えた分の追加／減った分の取り外しだけを行う。
	// 一覧をまるごと作り直すと、変わっていない行まで一瞬消えて背景が透けるため
	// 「全体が暗くなる」ように見えてしまう。
	const int32 OldNum = TrackCards.Num();
	const int32 NewNum = Tracks.Num();
	const int32 CommonNum = FMath::Min(OldNum, NewNum);

	// 減った分のカードだけを末尾から取り外す（残りのカードには触れない）
	for (int32 i = OldNum - 1; i >= NewNum; --i)
	{
		if (TrackCards[i])
		{
			TrackListScrollBox->RemoveChild(TrackCards[i]);
		}
	}
	if (NewNum < OldNum)
	{
		TrackCards.SetNum(NewNum);
	}

	// 共通範囲：中身だけ更新する
	for (int32 i = 0; i < CommonNum; ++i)
	{
		UWG_TrackCard* Card = TrackCards[i];
		if (!Card) continue;

		// トラックの増減で位置がずれ、この行が別トラックを指すようになった場合は、
		// Indexで引いているメディア一覧キャッシュが食い違うので捨てて取り直す。
		const bool bIdentityChanged = (Card->TrackName != Tracks[i].Name);
		if (bIdentityChanged)
		{
			InvalidateTrackMediaCache(Tracks[i].Index);
		}

		Card->UpdateTrackCard(Tracks[i], this);
		Card->SetLocked(bIsLocked);

		// 階層（インデント/畳み込み）は変化したカードだけ描き替える
		const bool bCollapsed = CollapsedTrackIndices.Contains(Tracks[i].Index);
		if (Card->HierarchyLevel != TrackLevels[i]
			|| Card->bIsParent != TrackParentFlags[i]
			|| Card->bIsCollapsed != bCollapsed)
		{
			Card->SetHierarchy(TrackLevels[i], TrackParentFlags[i], bCollapsed);
		}

		if (bIdentityChanged)
		{
			Card->RebuildMediaListIfExpanded();
		}
	}

	// 増えた分だけ新しいカードを作って末尾に追加する
	if (NewNum > OldNum)
	{
		TSubclassOf<UWG_TrackCard> CardClass = TrackCardClass;
		if (!CardClass)
		{
			CardClass = UWG_TrackCard::StaticClass();
		}

		for (int32 i = OldNum; i < NewNum; ++i)
		{
			const FTrackDetail& T = Tracks[i];

			// この位置に前のトラックのメディアが残っていることがあるので捨てておく
			InvalidateTrackMediaCache(T.Index);

			UWG_TrackCard* Card = CreateWidget<UWG_TrackCard>(this, CardClass);
			if (!Card) continue;

			Card->InitTrackCard(T, this);
			Card->SetLocked(bIsLocked);   // ロック中に行が増えた場合も、新しいカードにロック状態を反映する

			const bool bCollapsed = CollapsedTrackIndices.Contains(T.Index);
			Card->SetHierarchy(TrackLevels[i], TrackParentFlags[i], bCollapsed);

			if (ExpandedTrackIndices.Contains(T.Index))
			{
				Card->SetMediaExpanded(true);   // 展開していたトラックは展開したまま復元する
			}

			if (UScrollBoxSlot* CardSlot = Cast<UScrollBoxSlot>(TrackListScrollBox->AddChild(Card)))
			{
				CardSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));
				CardSlot->SetHorizontalAlignment(HAlign_Fill);
			}
			TrackCards.Add(Card);
		}
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
	Save->bUseParaknotMic = bUseParaknotMic;
	Save->MonitorOutputMode = MonitorOutputMode;
	Save->bShowDeviceNameIP = bShowDeviceNameIP;
	Save->bScreenRecShowOverlay = bScreenRecShowOverlay;
	Save->bScreenRecIncludeMic = bScreenRecIncludeMic;
	Save->GroupTalkPlaybackGain = GroupTalkPlaybackGain;
	Save->GroupTalkSendGain = GroupTalkSendGain;
	Save->Language = static_cast<uint8>(ReaperLang::GetLanguage());
	Save->SpatialItdLevel = SpatialItdLevel;
	Save->SpatialShadowLevel = SpatialShadowLevel;
	Save->SpatialChannelOrder = SpatialChannelOrder;
	Save->HrtfProfile = HrtfProfile;
	Save->SpatialCustomAzimuths = CustomAzimuths;
	Save->SpatialCustomDistances = CustomDistances;
	Save->MonitorBufferLevel = MonitorBufferLevel;
	Save->MonitorVolumeDb = MonitorVolumeDb;
	Save->MarkerOffsetSeconds = MarkerOffsetSeconds;
	Save->LocalIRPlayCount = LocalIRPlayCount;
	Save->LocalIRIntervalSeconds = LocalIRIntervalSeconds;
	Save->MarkerNameHistory = MarkerNameHistory;
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
	Save->SlackExportScope = SlackExportScope;
	Save->ScriptMatchScope = ScriptMatchScope;
	Save->ScriptMatchCheckFilter = ScriptMatchCheckFilter;
	Save->GoogleSheetSpreadsheetId = GoogleSheetSpreadsheetId;
	Save->GoogleSheetClientId = GoogleSheetClientId;
	Save->IntercomUserName = IntercomUserName;
	Save->PresenceFolder = PresenceFolder;
	Save->ResolvedPresenceFolder = ResolvedPresenceFolder;
	Save->MonitorSource = MonitorSource;
	Save->MonitorListenPort = MonitorListenPort;
	Save->ServerAuthEntries = ServerAuthEntries;
	Save->SelectedServerId = SelectedServerId;
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
	bUseParaknotMic = Save->bUseParaknotMic;
	MonitorOutputMode = Save->MonitorOutputMode;
	bShowDeviceNameIP = Save->bShowDeviceNameIP;
	bScreenRecShowOverlay = Save->bScreenRecShowOverlay;
	bScreenRecIncludeMic = Save->bScreenRecIncludeMic;
	GroupTalkPlaybackGain = (float)FMath::Clamp(FMath::RoundToInt(Save->GroupTalkPlaybackGain), 0, 10);
	// 送信ゲインは0.5刻み・最大5.0。上限を10.0から5.0へ下げたので、
	// それ以前の保存値（最大10.0）が入っていた場合はここで5.0に丸められる。
	GroupTalkSendGain = FMath::Clamp(FMath::RoundToInt(Save->GroupTalkSendGain * 2.f), 0, 10) * 0.5f;
	MonitorSource = Save->MonitorSource;
	MonitorListenPort = Save->MonitorListenPort;
	RefreshMonitorInstanceCombo();   // 保存済みの配信元を表示へ反映
	ServerAuthEntries = Save->ServerAuthEntries;
	SelectedServerId = Save->SelectedServerId;   // 前回の選択を復元（RefreshServerIdCombo が参照する）
	RefreshServerIdCombo();
	SpatialItdLevel = Save->SpatialItdLevel;
	SpatialShadowLevel = Save->SpatialShadowLevel;
	SpatialChannelOrder = EBinauralChannelOrder::Custom;   // 保存値に関わらずカスタム固定
	HrtfProfile = Save->HrtfProfile;
	if (Save->SpatialCustomAzimuths.Num() >= MaxMappingChannels) { CustomAzimuths = Save->SpatialCustomAzimuths; }
	if (Save->SpatialCustomDistances.Num() >= MaxMappingChannels) { CustomDistances = Save->SpatialCustomDistances; }
	MonitorBufferLevel = Save->MonitorBufferLevel;
	MonitorVolumeDb = (float)FMath::Clamp(FMath::RoundToInt(Save->MonitorVolumeDb), -12, 12);
	MarkerOffsetSeconds = FMath::Clamp(Save->MarkerOffsetSeconds, -10, 10);
	LocalIRPlayCount = FMath::Clamp(Save->LocalIRPlayCount, 1, 99);
	LocalIRIntervalSeconds = FMath::Clamp(Save->LocalIRIntervalSeconds, 0, 15);
	MarkerNameHistory = Save->MarkerNameHistory;
	if (MarkerNameHistory.Num() > MaxMarkerNameHistory) { MarkerNameHistory.SetNum(MaxMarkerNameHistory); }
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
	ListFormat = FMath::Clamp(Save->ListFormat, 0, 2);   // 0=Excel/1=CSV/2=Google Sheets
	ListFilePath = Save->ListFilePath;
	ListSheetName = Save->ListSheetName;
	ListStartRow = FMath::Max(1, Save->ListStartRow);
	ListStartCol = FMath::Max(1, Save->ListStartCol);
	ListColCount = FMath::Clamp(Save->ListColCount, 1, 32);
	bListUseCheckbox = Save->bListUseCheckbox;
	ListExportPath = Save->ListExportPath;
	GoogleSheetSpreadsheetId = Save->GoogleSheetSpreadsheetId;
	GoogleSheetClientId = Save->GoogleSheetClientId;
	SlackExportScope = FMath::Clamp(Save->SlackExportScope, 0, 1);
	ScriptMatchScope = FMath::Clamp(Save->ScriptMatchScope, 0, 1);
	ScriptMatchCheckFilter = FMath::Clamp(Save->ScriptMatchCheckFilter, 0, 2);
	IntercomUserName = Save->IntercomUserName;
	PresenceFolder = Save->PresenceFolder;
	ResolvedPresenceFolder = Save->ResolvedPresenceFolder;   // 前回接続で解決した名簿フォルダを復元
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
	UpdateSlackExportScopeButton();
	UpdateScriptMatchScopeButton();
	UpdateScriptMatchCheckFilterButton();
	if (SlackTokenInput)  { SlackTokenInput->SetText(FText::FromString(SlackToken)); }
	if (SlackToUserInput) { SlackToUserInput->SetText(FText::FromString(SlackToUser)); }
	if (TargetLufsInput)  { TargetLufsInput->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), TargetLUFS))); }
	UpdateGroupTalkVolumeUI();       // 復元した受信音量をスライダー/数値へ反映
	UpdateGroupTalkSendVolumeUI();   // 復元した送信音量をスライダー/数値へ反映
	UpdateMonitorVolumeUI();         // 復元したモニター受信音量(dB)をスライダー/数値へ反映
	UpdateMarkerOffsetUI();          // 復元したマーカー位置補正(秒)をスライダー/数値へ反映
	ApplyGroupTalkPlaybackGain();   // 復元した音量を（存在すれば）受信機へ反映
	ApplyGroupTalkSendGain();       // 復元した送信音量を（存在すれば）送信機へ反映
	if (MeterIntervalInput) { MeterIntervalInput->SetText(FText::FromString(FString::FromInt(MeterIntervalMs))); }
	UpdateMeterModeButton();
	UpdateMeterDotScopeButton();
	if (TrackRefreshIntervalInput) { TrackRefreshIntervalInput->SetText(FText::FromString(FString::FromInt(TrackRefreshIntervalSec))); }
	// 自動更新の設定なら、起動時からホームの「更新」トグルをONにして動かす
	bTrackListAutoRefreshRunning = bTrackListAutoRefresh;
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
	UpdateMicTransportButton();
	UpdateHeaderIpText();
	// 復元した設定をボタン表示へ反映
	UpdateMicInputModeButton();
	UpdateScreenRecOverlayButton();
	UpdateScreenRecMicButton();
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

	// 画面録画の状態機械（許可待ち→カウントダウン→録画中）。前面でのみ進める（背景時は
	// 録画は前面サービス側で継続するため、ここでの進行は不要）。
	TickScreenRecord(InDeltaTime);

	// ローカルIR：TSPのPCMを切らさないよう先行して積み、鳴り終わったら止める。
	TickLocalIRPlayback();

	// PARAKNOT モニターの鍵が入れ替わっていないか見る（入れ替わっていたら取り直す）。
	TickParaknotMonitorRecovery(InDeltaTime);

	// 受信レベルのメーター（「モニター」ボタンの下）。
	UpdateMonitorLevelMeter();

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

	// リスト読み込み：「読み込み中・・・」を1フレーム描かせてから、実際の読み込みを始める
	// （HandleListLoadClicked から直接読むと、その文字が出ないまま画面が止まってしまうため）。
	if (ListLoadPendingFrames > 0)
	{
		if (--ListLoadPendingFrames == 0) { StartListLoad(); }
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

	// バックグラウンド中（画面OFF/他アプリ表示）は、ここから下の定期処理を行わない。
	// ・在席ハートビート等は監視スレッド（BackgroundKeepAliveMaintenance）が肩代わりする。
	//   ゲームスレッドは停止中に短時間だけ動かされる（ポンプ）ことがあり、そのときに
	//   ここのRPCが走ると監視スレッドのRPCと同じ接続を同時に使ってしまうため。
	// ・メーター取得や一覧自動更新などはバックグラウンドでは不要。
	if (FReaperKeepAlive::IsInBackground())
	{
		return;
	}

	// グループトーク：どちらかのchがアクティブな間は、両chの参加状況をハートビート書き込み＋
	// 参加者一覧を定期更新する。無音になった参加者（退出・切断）の再生も定期的に掃除する。
	if (bIsConnected && !IntercomUserName.IsEmpty() &&
		(IsGroupTalkChannelActive(0) || IsGroupTalkChannelActive(1)))
	{
		// 掃除は純ローカル処理（RPCなし）なのでゲームスレッドで従来通り。
		GroupTalkPruneTimer += InDeltaTime;
		if (GroupTalkPruneTimer >= 2.0)   // 2秒ごとに、無音の参加者の再生を掃除
		{
			GroupTalkPruneTimer = 0.0;
			for (int32 Ch = 0; Ch < 2; ++Ch)
			{
				if (GroupTalkReceiver[Ch]) { GroupTalkReceiver[Ch]->PruneStalePeers(6.0f); }
			}
		}

		// ハートビート書き込み＋名簿更新は同期RPC。電波が悪いと数十秒ブロックしうるため、
		// ゲームスレッドでは行わずワーカースレッドで実行する（UI更新だけGTへ戻す）。
		// 多重起動しないよう bGtRpcInFlight でガードし、実行中は次の周期を見送る。
		GroupTalkHeartbeatTimer += InDeltaTime;
		GroupTalkRosterTimer    += InDeltaTime;
		const bool bDoHeartbeat = (GroupTalkHeartbeatTimer >= 5.0);
		const bool bDoRoster    = (GroupTalkRosterTimer >= 3.0);
		if ((bDoHeartbeat || bDoRoster) && !bGtRpcInFlight.Load())
		{
			if (bDoHeartbeat) { GroupTalkHeartbeatTimer = 0.0; }
			if (bDoRoster)    { GroupTalkRosterTimer = 0.0; }
			bGtRpcInFlight.Store(true);
			TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);

			// 他のRPCと同じ制御用キューへ合流させる（1本のスレッドで順番に実行）。
			EnqueueControlRpc(TEXT("gtpresence"), [WeakThis, bDoHeartbeat, bDoRoster]()
			{
				if (UWG_RadNodzToolkit* Self = WeakThis.Get())
				{
					if (Self->bIsConnected && Self->Client)
					{
						if (bDoHeartbeat) { Self->WriteGroupTalkHeartbeat(0); Self->WriteGroupTalkHeartbeat(1); }
						if (bDoRoster)    { Self->RefreshGroupTalkRosterBackground(); }   // UI非依存・スレッドセーフ版
					}
				}
				// UI更新と実行中フラグ解除はゲームスレッドで行う。
				AsyncTask(ENamedThreads::GameThread, [WeakThis]()
				{
					if (UWG_RadNodzToolkit* S = WeakThis.Get())
					{
						S->UpdateGroupTalkUI();
						S->bGtRpcInFlight.Store(false);
					}
				});
			});
		}
	}

	// 意図しない切断からの自動再接続。切断中に動かす必要があるので、
	// 接続中のみ通る下のロック区間の外で処理する。
	TickAutoReconnect(InDeltaTime);

	// --- ここから下の定期処理は「タイマを進めて、RPCをキューへ積む」だけを行う。
	//     通信はワーカースレッド（ControlQueue / HeavyQueue）で走り、結果の反映だけが
	//     ゲームスレッドへ戻ってくる。したがってこの区間はロックも待ちも発生せず、
	//     電波が悪くてもゲームスレッドは止まらない。
	if (bIsConnected && Client)
	{
		// 接続監視（1秒ごと）：切断されていれば切断状態へ遷移。
		// IsConnected() は select+peek だけで通信しないため、ここで呼んでよい。
		ConnCheckTimer += InDeltaTime;
		if (ConnCheckTimer >= ConnCheckIntervalSec)
		{
			ConnCheckTimer = 0.0;
			if (!Client->IsConnected())
			{
				Disconnect();   // bIsConnected=false / キャッシュ破棄 / 切断UI・OnDisconnected を実施
				RequestAutoReconnect();   // 回線断・サーバ消失なので自動復帰を試みる
			}
		}

		// 前面での接続キープ（心拍）。
		// 画面が点いている間も無通信区間が続くと、経路の途中（AP や NAT）に
		// 接続を落とされることがある。軽量RPCを一定間隔で1往復させて防ぐ。
		// バックグラウンド側（BackgroundKeepAliveMaintenance）と最終実行時刻を
		// 共有しているので、前面/背面が入れ替わっても二重には打たない。
		if (bIsConnected && Client && !bProbeRpcInFlight)
		{
			const double NowSec = FPlatformTime::Seconds();

			if (NowSec - BgConnKeepLast.Load() >= BgConnKeepIntervalSec)
			{
				BgConnKeepLast.Store(NowSec);

				URadnodzClient* KeepAliveClient = Client;

				EnqueueControlRpc(TEXT("keepalive"), [KeepAliveClient]()
				{
					if (IsValid(KeepAliveClient)) { KeepAliveClient->SendTimingProbe(); }
				});
			}
		}

		// Reaper通信時間の定期計測（30秒ごと）。接続直後の1回目は接続処理側で実施。
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

		// 接続の生存監視（約60秒無応答なら切断とみなして自動再接続）。
		// 自動更新より先に行い、死んだ接続のまま一覧取得を走らせない。
		TickConnectionHealth(InDeltaTime);

		// トラック一覧の自動更新（設定でONの場合のみ、指定間隔ごとに「更新」相当の処理を行う）
		TickTrackListAutoRefresh(InDeltaTime);

		// 計測ジョブ（Slack共有 / 計測＆音量調整）を少しずつ処理する
		TickMeasureJob(InDeltaTime);
	}

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

	// 台本一致：文字起こしジョブを1件ずつ進める（RPCは解析用キューで実行するため
	// ここではキューへ積むだけ＝ゲームスレッドはブロックしない）。
	TickScriptMatchJob(InDeltaTime);
}

// ============================================================================
//  認証設定（mTLS）: サーバー管理 / 自端末FPの表示 / 鍵の申請 / 疎通確認
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

	// 以前の選択を維持（再構築前のUI選択→保存済みの選択の順で優先）。無効なら先頭を選ぶ。
	FString ToSelect = Prev;
	if (ToSelect.IsEmpty() || FindServerAuthIndex(ToSelect) == INDEX_NONE)
	{
		ToSelect = SelectedServerId;   // 起動時の復元：SaveGame から読み込んだ選択
	}
	if (ToSelect.IsEmpty() || FindServerAuthIndex(ToSelect) == INDEX_NONE)
	{
		ToSelect = (ServerAuthEntries.Num() > 0) ? ServerAuthEntries[0].ServerId : FString();
	}
	if (!ToSelect.IsEmpty())
	{
		ServerIdCombo->SetSelectedOption(ToSelect);
	}
	SelectedServerId = ToSelect;   // 確定した選択を記録

	// 選択中の期待FPを表示へ反映
	SetServerFpText(FindServerAuthIndex(ToSelect));

	// 選択中 serverId に紐づく IP/ポートを接続先へ復元する。
	ApplySelectedServerConn();
}

void UWG_RadNodzToolkit::SetServerFpText(int32 EntryIndex)
{
	if (!ServerFpText) { return; }

	const bool bHasFp = ServerAuthEntries.IsValidIndex(EntryIndex)
		&& !ServerAuthEntries[EntryIndex].Fingerprint.IsEmpty();

	ServerFpText->SetText(FText::FromString(bHasFp
		? ServerAuthEntries[EntryIndex].Fingerprint
		: TEXT("-")));
}

void UWG_RadNodzToolkit::HandleServerIdComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	const int32 Idx = FindServerAuthIndex(SelectedItem);
	SetServerFpText(Idx);
	// 選択された serverId に紐づく IP/ポートを接続先へ復元する。
	ApplySelectedServerConn();

	// 選択を記録。ユーザー操作(Directでない)のときだけ保存する
	// （SetSelectedOptionによるプログラム的変更=Direct はロード中にも発生するため保存しない）。
	SelectedServerId = SelectedItem;
	if (SelectionType != ESelectInfo::Direct)
	{
		SaveSettings();
	}
}

void UWG_RadNodzToolkit::ApplySelectedServerConn()
{
	const int32 Idx = FindServerAuthIndex(GetSelectedServerId());
	if (Idx == INDEX_NONE) { return; }
	const FServerAuthEntry& E = ServerAuthEntries[Idx];
	// 未紐付け（旧エントリ等で IP 空）のときは現在の接続先を維持する。
	if (E.IPAddress.IsEmpty()) { return; }

	// **接続先が変わるなら、選んでいた PARAKNOT は捨てる。**
	// InstanceId は相手プロセス限りの値で、別マシンでは存在しない。
	// 残すと鍵の取り直しが延々と失敗し続ける。
	if (IPAddress != E.IPAddress)
	{
		ParaknotMicInstanceId.Reset();
		ParaknotMicLabel.Reset();
	}

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
	// ポップアップでサーバー名と接続用URLを入力させる（登録時に AddServerFromPairingUrl が呼ばれる）。
	UWG_AddServerIdDialog* Dlg = CreateWidget<UWG_AddServerIdDialog>(this, UWG_AddServerIdDialog::StaticClass());
	if (Dlg)
	{
		Dlg->Setup(this);
		Dlg->AddToViewport(200);
	}
}

FString UWG_RadNodzToolkit::GetServerPinKey(const FServerAuthEntry& Entry)
{
	return Entry.NodeId.IsEmpty() ? Entry.ServerId : Entry.NodeId;
}

bool UWG_RadNodzToolkit::AddServerFromPairingUrl(const FString& Name, const FString& Url, FString& OutError)
{
	OutError.Empty();

	const FString ServerName = Name.TrimStartAndEnd();
	if (ServerName.IsEmpty())
	{
		OutError = ReaperLang::S(TEXT("サーバー名を入力してください。"), TEXT("Enter a server name."));
		return false;
	}
	if (FindServerAuthIndex(ServerName) != INDEX_NONE)
	{
		OutError = ReaperLang::S(TEXT("同名のサーバーが既に存在します。"), TEXT("That server name already exists."));
		return false;
	}

	const FString PairingUrl = Url.TrimStartAndEnd();
	if (PairingUrl.IsEmpty())
	{
		OutError = ReaperLang::S(TEXT("接続用URLを入力してください（QRコード撮影でも入力できます）。"),
			TEXT("Enter the connection URL (or scan the QR code)."));
		return false;
	}

	FRadnodzPairingInfo Info;
	if (!URadnodzClient::ParsePairingUrl(PairingUrl, Info))
	{
		OutError = ReaperLang::S(TEXT("接続用URLの形式が正しくありません。"), TEXT("The connection URL is malformed."));
		return false;
	}

	// 同じサーバー（nodeId）を別名で二重登録させない。ピン留めキーが重複すると
	// 「名前は違うのに同じ信頼情報を指す」エントリになって扱いが分かりにくくなる。
	for (const FServerAuthEntry& E : ServerAuthEntries)
	{
		if (!E.NodeId.IsEmpty() && E.NodeId == Info.NodeId)
		{
			OutError = FString::Printf(TEXT("%s (%s)"),
				*ReaperLang::S(TEXT("このサーバーは既に登録されています"), TEXT("This server is already registered")),
				*E.ServerId);
			return false;
		}
	}

	FServerAuthEntry Entry;
	Entry.ServerId    = ServerName;
	Entry.NodeId      = Info.NodeId;
	Entry.Fingerprint = Info.Fingerprint;
	Entry.IPAddress   = Info.Host;
	Entry.Port        = Info.Port;
	ServerAuthEntries.Add(Entry);

	SelectedServerId = ServerName;   // 追加したサーバーを選択として記録・保存

	// URL から取れた接続先をそのまま現在の接続先にする。
	IPAddress = Info.Host;
	Port      = Info.Port;
	if (IPAddressInput) { IPAddressInput->SetText(FText::FromString(IPAddress)); }
	if (PortInput)      { PortInput->SetText(FText::FromString(FString::FromInt(Port))); }

	SaveSettings();

	RefreshServerIdCombo();
	if (ServerIdCombo) { ServerIdCombo->SetSelectedOption(ServerName); }
	SetServerFpText(FindServerAuthIndex(ServerName));
	UpdateHeaderIpText();

	SetAuthStatus(FString::Printf(TEXT("%s: %s (%s:%d)"),
		*ReaperLang::S(TEXT("追加しました"), TEXT("Added")), *ServerName, *Info.Host, Info.Port));

	return true;
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
	RefreshServerIdCombo();   // 先に選択を次候補へ確定させる（SelectedServerId が更新される）
	SaveSettings();           // 確定した選択を保存
	SetAuthStatus(FString::Printf(TEXT("%s: %s"), *ReaperLang::S(TEXT("削除しました"), TEXT("Deleted")), *Sel));
}

void UWG_RadNodzToolkit::HandleShowOwnFpClicked()
{
	// この端末の鍵のフィンガープリント。サーバー側（Bridge）の承認待ち一覧に出る値と
	// 突き合わせるために見せる。鍵がまだ無ければここで作られる。
	EnsureAuthClient();
	if (!AuthClient->GenerateIdentity())
	{
		SetAuthStatus(ReaperLang::S(TEXT("鍵の用意に失敗しました。"), TEXT("Failed to prepare identity.")));
		return;
	}

	const FString Fp = AuthClient->GetOwnFingerprint();
	const FString Body = Fp.IsEmpty()
		? ReaperLang::S(TEXT("フィンガープリントを取得できませんでした。"), TEXT("Could not read the fingerprint."))
		: Fp;

	UWG_InfoDialog* Dlg = CreateWidget<UWG_InfoDialog>(this, UWG_InfoDialog::StaticClass());
	if (Dlg)
	{
		Dlg->Setup(ReaperLang::S(TEXT("この端末のFP"), TEXT("This device FP")), Body);
		Dlg->AddToViewport(200);
	}
}

void UWG_RadNodzToolkit::HandleEnrollClicked()
{
	using namespace ReaperCtrlUI;

	// 二重実行を防ぐ（申請は中断できないので、押し直しは無視する）。
	if (bIsEnrolling)
	{
		return;
	}

	const FString Sel = GetSelectedServerId();
	const int32 Idx = FindServerAuthIndex(Sel);
	if (Idx == INDEX_NONE)
	{
		SetAuthStatus(ReaperLang::S(TEXT("サーバーIDを選択してください。"), TEXT("Select a server ID.")));
		return;
	}

	const FServerAuthEntry& Entry = ServerAuthEntries[Idx];
	if (Entry.Fingerprint.IsEmpty())
	{
		// FP が無いとサーバーを検証できず、申請先が本物か分からない。
		SetAuthStatus(ReaperLang::S(TEXT("このサーバーのFPが未設定です。"), TEXT("This server's fingerprint is not set.")));
		return;
	}

	const FString Ip = IPAddressInput ? IPAddressInput->GetText().ToString().TrimStartAndEnd() : IPAddress;
	const int32   P  = ResolvePortFromInput();
	if (PortInput) { PortInput->SetText(FText::FromString(FString::FromInt(P))); }
	if (Ip.IsEmpty())
	{
		SetAuthStatus(ReaperLang::S(TEXT("接続先IPが未設定です。"), TEXT("The server address is not set.")));
		return;
	}

	URadnodzClient* C = URadnodzClient::CreateRadnodzClient();
	if (!C)
	{
		SetAuthStatus(ReaperLang::S(TEXT("クライアントの生成に失敗しました。"), TEXT("Failed to create client.")));
		return;
	}
	// 申請中に GC 回収されないようルート登録する。
	C->AddToRoot();

	// 申請も TLS で行う（鍵を用意する）。
	C->SetStorageDirectory(GetAuthStorageDir());
	if (!C->GenerateIdentity())
	{
		C->RemoveFromRoot();
		SetAuthStatus(ReaperLang::S(TEXT("鍵の用意に失敗しました。"), TEXT("Failed to prepare identity.")));
		return;
	}
	C->SetServerId(GetServerPinKey(Entry));
	C->SetExpectedServerFingerprint(Entry.Fingerprint);

	// 管理者の一覧に出る表示名。どの端末からの申請かが分かるよう端末名を使う。
	const FString ClientName = FPlatformProcess::ComputerName();

	const int32 MyGen = ++EnrollGeneration;
	bIsEnrolling = true;
	SetAuthStatus(FString::Printf(TEXT("%s %s:%d ..."), *ReaperLang::S(TEXT("申請中"), TEXT("Requesting")), *Ip, P));
	if (EnrollButton && EnrollButtonText)
	{
		EnrollButtonText->SetText(ReaperLang::T(TEXT("申請中..."), TEXT("Requesting...")));
		EnrollButton->SetBackgroundColor(StopGray);
		EnrollButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(StopGray)));
	}

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);
	Async(EAsyncExecution::Thread, [WeakThis, C, Ip, P, ClientName, MyGen]()
	{
		ERadnodzEnrollStatus Status = ERadnodzEnrollStatus::Failed;
		FRigdocksError Err;
		if (C)
		{
			// Enroll は自分で接続しに行くが、TcpClient には「接続先だけを設定する」APIが無い。
			// Connect(ip, port) が接続先を保持するので、一度呼んでから切断し、
			// 空いた状態で Enroll を呼ぶ。未承認の端末はこの最初の接続に失敗するが、
			// 接続先は保持されるので申請そのものには影響しない。
			C->Connect(Ip, P);
			C->Disconnect();

			Status = C->Enroll(ClientName);
			if (Status == ERadnodzEnrollStatus::Failed) { Err = C->GetError(); }
		}

		// 結果はゲームスレッドで反映する（UMG はゲームスレッド限定）
		AsyncTask(ENamedThreads::GameThread, [WeakThis, C, Status, Err, MyGen]()
		{
			if (UWG_RadNodzToolkit* Self = WeakThis.Get())
			{
				// 古い申請の結果は捨てる（押し直し等で世代が進んでいる場合）。
				if (Self->EnrollGeneration == MyGen)
				{
					Self->HandleEnrollResult(Status, Err);
				}
			}
			if (IsValid(C)) { C->RemoveFromRoot(); }
		});
	});
}

void UWG_RadNodzToolkit::HandleEnrollResult(ERadnodzEnrollStatus Status, const FRigdocksError& Error)
{
	using namespace ReaperCtrlUI;

	bIsEnrolling = false;

	// 「申請」ボタンを元に戻す
	if (EnrollButton && EnrollButtonText)
	{
		EnrollButtonText->SetText(ReaperLang::T(TEXT("申請"), TEXT("Request")));
		EnrollButton->SetBackgroundColor(StopGray);
		EnrollButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(StopGray)));
	}

	switch (Status)
	{
	case ERadnodzEnrollStatus::Pending:
		SetAuthStatus(ReaperLang::S(TEXT("申請しました。サーバー側で承認されるのを待ってください。"),
			TEXT("Request submitted. Waiting for the operator to approve it.")));
		break;

	case ERadnodzEnrollStatus::Approved:
		SetAuthStatus(ReaperLang::S(TEXT("この端末は既に承認済みです。"), TEXT("This device is already approved.")));
		break;

	case ERadnodzEnrollStatus::Closed:
		SetAuthStatus(ReaperLang::S(TEXT("サーバーが今は申請を受け付けていません。"),
			TEXT("The server is not accepting requests right now.")));
		break;

	case ERadnodzEnrollStatus::Rejected:
		SetAuthStatus(ReaperLang::S(TEXT("申請が却下されました。"), TEXT("The request was refused.")));
		break;

	default:
		SetAuthStatus(FString::Printf(TEXT("%s code=%d %s"),
			*ReaperLang::S(TEXT("申請失敗:"), TEXT("Request failed:")), Error.ErrorCode, *Error.Message));
		break;
	}
}

void UWG_RadNodzToolkit::HandleTrialConnectClicked()
{
	using namespace ReaperCtrlUI;

	// 実行中にもう一度押されたら「停止（キャンセル）」する（つながるまで待ち続けるのを中断できる）。
	if (bIsAuthenticating)
	{
		CancelTrialConnect();
		return;
	}

	const FString Ip = IPAddressInput ? IPAddressInput->GetText().ToString() : IPAddress;
	// ポートは未記入/不正なら既定の23760を使う。
	const int32   P  = ResolvePortFromInput();
	if (PortInput) { PortInput->SetText(FText::FromString(FString::FromInt(P))); }

	// 認証は毎回新しいクライアントで行う（選択中サーバーの鍵・ピン留めを確実に反映するため）。
	URadnodzClient* C = URadnodzClient::CreateRadnodzClient();
	if (!C)
	{
		SetAuthStatus(ReaperLang::S(TEXT("クライアントの生成に失敗しました。"), TEXT("Failed to create client.")));
		return;
	}
	// バックグラウンド接続中に GC 回収されないようルート登録する。
	C->AddToRoot();

	// 接続は常に TLS(mTLS)。
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
		// ピン留めキーは URL の nodeId（旧エントリはサーバー名で代用）。
		C->SetServerId(GetServerPinKey(ServerAuthEntries[Idx]));
		C->SetExpectedServerFingerprint(Fp);
	}

	// --- 「認証中...」表示（接続ボタンと同じ視覚フィードバック）---
	// 世代番号を進め、この試行を識別する。キャンセル/再実行されると番号が変わり、古い結果は無視される。
	const int32 MyGen = ++AuthGeneration;
	bIsAuthenticating = true;
	AuthInFlightClient = C;   // キャンセル時に Disconnect でブロッキング接続を中断するため保持
	SetAuthStatus(FString::Printf(TEXT("%s %s:%d ..."), *ReaperLang::S(TEXT("確認中"), TEXT("Verifying")), *Ip, P));
	if (TrialConnectButton && TrialConnectButtonText)
	{
		TrialConnectButtonText->SetText(ReaperLang::T(TEXT("確認中..."), TEXT("Verifying...")));
		TrialConnectButton->SetBackgroundColor(StopGray);
		TrialConnectButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(StopGray)));
	}
	// 右上ロゴを回転させる（接続中と同じ）
	SetStatusIconTexture(TEXT("/Game/Texture/T_RADNODZ_white_conect_1.T_RADNODZ_white_conect_1"));
	bStatusIconSpin = true;
	StatusIconSpinPhase = 0.f;

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakThis(this);
	Async(EAsyncExecution::Thread, [WeakThis, C, Ip, P, MyGen]()
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
		AsyncTask(ENamedThreads::GameThread, [WeakThis, C, bOk, Err, MyGen]()
		{
			if (UWG_RadNodzToolkit* Self = WeakThis.Get())
			{
				// キャンセル or 新しい試行に置き換わっていれば、この古い結果は反映しない。
				if (MyGen == Self->AuthGeneration)
				{
					Self->AuthInFlightClient = nullptr;
					Self->HandleAuthResult(bOk, Err);
				}
			}
			if (IsValid(C)) { C->RemoveFromRoot(); }
		});
	});
}

void UWG_RadNodzToolkit::CancelTrialConnect()
{
	using namespace ReaperCtrlUI;

	// 世代を進めて、進行中の非同期結果を無効化する。
	++AuthGeneration;
	bIsAuthenticating = false;

	// 進行中のブロッキング接続を中断する（ソケットを閉じて Connect を早期に失敗させる）。
	if (AuthInFlightClient)
	{
		AuthInFlightClient->Disconnect();
		AuthInFlightClient = nullptr;
	}

	// 「確認」ボタンを元に戻す。
	if (TrialConnectButton && TrialConnectButtonText)
	{
		TrialConnectButtonText->SetText(ReaperLang::T(TEXT("確認"), TEXT("Verify")));
		TrialConnectButton->SetBackgroundColor(AccentBlue);
		TrialConnectButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(AccentBlue)));
	}

	// 右上ロゴ/上部ステータスは本体の接続状態へ戻す（回転も停止する）。
	if (bIsConnected) { ApplyConnectedUI(); }
	else { ApplyDisconnectedUI(); }

	SetAuthStatus(ReaperLang::S(TEXT("確認を中止しました。"), TEXT("Verification canceled.")));
}

void UWG_RadNodzToolkit::HandleAuthResult(bool bSuccess, const FRigdocksError& Error)
{
	using namespace ReaperCtrlUI;

	bIsAuthenticating = false;

	// 「確認」ボタンを元に戻す
	if (TrialConnectButton && TrialConnectButtonText)
	{
		TrialConnectButtonText->SetText(ReaperLang::T(TEXT("確認"), TEXT("Verify")));
		TrialConnectButton->SetBackgroundColor(AccentBlue);
		TrialConnectButtonText->SetColorAndOpacity(FSlateColor(ReadableTextColor(AccentBlue)));
	}

	// 右上ロゴ/上部ステータスは本体の接続状態へ戻す（回転も停止する）
	if (bIsConnected) { ApplyConnectedUI(); }
	else { ApplyDisconnectedUI(); }

	// 認証結果を認証セクションのステータスに表示
	if (bSuccess)
	{
		SetAuthStatus(ReaperLang::S(TEXT("接続成功: 認証OK"), TEXT("Connected: auth OK")));
	}
	else
	{
		SetAuthStatus(FString::Printf(TEXT("%s code=%d %s"),
			*ReaperLang::S(TEXT("接続失敗:"), TEXT("Failed:")), Error.ErrorCode, *Error.Message));
	}
}