// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).


#include "WG_AddServerIdDialog.h"
#include "WG_RadNodzToolkit.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include "QrScanner.h"
#include "RadnodzClient.h"
#include "ReaperDialogStyle.h"
#include "ReaperLang.h"

#if PLATFORM_ANDROID
#include "AndroidPermissionFunctionLibrary.h"
#endif

namespace
{
	/** スキャン進行の確認間隔（秒）。デコードもこの間隔で1枚ずつ試す。 */
	constexpr float QrPollInterval = 0.2f;

	/** カメラ権限の許可待ちを諦めるまでの秒数。 */
	constexpr float QrPermissionTimeout = 60.f;

	/**
	 * Start() 直後に「停止中」と判断しないための猶予秒。
	 * Java 側の開始処理は UI スレッドへ積まれるので、状態が立つまで少し遅れる。
	 */
	constexpr float QrStartGrace = 3.f;
}

void UWG_AddServerIdDialog::Setup(UWG_RadNodzToolkit* InOwner)
{
	Owner = InOwner;
}

TSharedRef<SWidget> UWG_AddServerIdDialog::RebuildWidget()
{
	if (!bUICreated && WidgetTree)
	{
		bUICreated = true;
		BuildUI();
	}

	return Super::RebuildWidget();
}

void UWG_AddServerIdDialog::BuildUI()
{
	using namespace ReaperDialogStyle;

	// URL は長いので、パネルの幅を確保しておく。
	UVerticalBox* BodyCol = BuildFrame(this, ReaperLang::S(TEXT("サーバーを登録"), TEXT("Register a server")), 720.f);

	// 見出し＋入力欄を1組積むヘルパー。
	auto AddLabeledInput = [&](const FString& Label, const FString& Hint, FName Name,
		TObjectPtr<UEditableTextBox>& OutBox)
	{
		UTextBlock* L = MakeText(WidgetTree, Label, 18, TextDim, true);
		{
			UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(L);
			S->SetPadding(FMargin(2.f, 0.f, 0.f, 4.f));
			S->SetHorizontalAlignment(HAlign_Fill);
		}

		UEditableTextBox* Input = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), Name);
		Input->SetHintText(FText::FromString(Hint));
		StyleDarkInput(Input, 18);
		Input->OnTextCommitted.AddDynamic(this, &UWG_AddServerIdDialog::HandleInputCommitted);
		{
			UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(Input);
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 14.f));
			S->SetHorizontalAlignment(HAlign_Fill);
		}
		OutBox = Input;
	};

	// サーバー名（一覧に出す表示名）
	AddLabeledInput(ReaperLang::S(TEXT("サーバー名"), TEXT("Server name")),
		TEXT("MyReaper"), TEXT("NameInput"), NameInput);

	// 接続用URL（Bridge の「接続」画面で生成・QR表示できる）
	AddLabeledInput(ReaperLang::S(TEXT("接続用URL"), TEXT("Connection URL")),
		TEXT("rigdocks://server?v=1&id=..."), TEXT("UrlInput"), UrlInput);

	// QRコード撮影（カメラで接続用URLを読む。対応プラットフォームのみ表示）
	if (FQrScanner::IsSupported())
	{
		ScanQrButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("QRコード撮影"), TEXT("Scan QR code")),
			AccentBlue, TEXT("ScanQrButton"), 18);
		ScanQrButton->OnClicked.AddDynamic(this, &UWG_AddServerIdDialog::HandleScanQrClicked);

		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredHeight(60.f);
		B->AddChild(ScanQrButton);

		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(B);
		S->SetPadding(FMargin(0.f, 2.f, 0.f, 14.f));
		S->SetHorizontalAlignment(HAlign_Fill);
	}

	// メッセージ行（読み取り結果・入力エラーの表示）
	MessageText = MakeText(WidgetTree, FString(), 16, NeonEdge, true);
	MessageText->SetAutoWrapText(true);
	{
		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(MessageText);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 12.f));
		S->SetHorizontalAlignment(HAlign_Fill);
	}

	// ボタン行（キャンセル / 登録）
	{
		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		CancelButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("キャンセル"), TEXT("Cancel")), StopGray, TEXT("CancelButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(64.f);
			B->AddChild(CancelButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			S->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
		}

		RegisterButton = MakeButton(WidgetTree, ReaperLang::S(TEXT("登録"), TEXT("Register")), AccentGreen, TEXT("RegisterButton"));
		{
			USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			B->SetMinDesiredHeight(64.f);
			B->AddChild(RegisterButton);
			UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(B);
			S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}

		RegisterButton->OnClicked.AddDynamic(this, &UWG_AddServerIdDialog::HandleRegister);
		CancelButton->OnClicked.AddDynamic(this, &UWG_AddServerIdDialog::HandleCancel);

		UVerticalBoxSlot* S = BodyCol->AddChildToVerticalBox(Row);
		S->SetHorizontalAlignment(HAlign_Fill);
	}
}

void UWG_AddServerIdDialog::NativeDestruct()
{
	// ダイアログが閉じられたらカメラを必ず解放する（画面外でプレビューが残らないように）。
	EndScan();

	Super::NativeDestruct();
}

void UWG_AddServerIdDialog::SetMessage(const FString& Msg, bool bError)
{
	if (MessageText)
	{
		MessageText->SetText(FText::FromString(Msg));
		MessageText->SetColorAndOpacity(FSlateColor(bError ? ReaperDialogStyle::ErrorRed : ReaperDialogStyle::NeonEdge));
	}
}

void UWG_AddServerIdDialog::Commit()
{
	const FString Name = NameInput ? NameInput->GetText().ToString().TrimStartAndEnd() : FString();
	const FString Url  = UrlInput  ? UrlInput->GetText().ToString().TrimStartAndEnd()  : FString();

	if (!Owner)
	{
		RemoveFromParent();
		return;
	}

	// 失敗した理由はダイアログに出し、閉じずに直せるようにする。
	FString Error;
	if (!Owner->AddServerFromPairingUrl(Name, Url, Error))
	{
		SetMessage(Error, /*bError=*/true);
		return;
	}

	RemoveFromParent();
}

void UWG_AddServerIdDialog::HandleRegister()
{
	Commit();
}

void UWG_AddServerIdDialog::HandleCancel()
{
	RemoveFromParent();
}

void UWG_AddServerIdDialog::HandleInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		Commit();
	}
}

void UWG_AddServerIdDialog::HandleScanQrClicked()
{
	if (!FQrScanner::IsSupported())
	{
		SetMessage(ReaperLang::S(TEXT("この端末ではカメラ読み取りに対応していません。URLを貼り付けてください。"),
			TEXT("Camera scanning is not supported here. Paste the URL instead.")), /*bError=*/true);
		return;
	}

	// 起動中に押されたら二重に開かない。
	if (bWaitingCameraPermission || FQrScanner::GetState() != EQrScanState::Idle)
	{
		return;
	}

#if PLATFORM_ANDROID
	// カメラ権限は許可ダイアログが閉じるまで結果が分からないので、
	// 許可されるまでポーリングして待つ（待っている間もこのダイアログは操作できる）。
	if (!UAndroidPermissionFunctionLibrary::CheckPermission(TEXT("android.permission.CAMERA")))
	{
		TArray<FString> Perms;
		Perms.Add(TEXT("android.permission.CAMERA"));
		UAndroidPermissionFunctionLibrary::AcquirePermissions(Perms);

		bWaitingCameraPermission = true;
		PermissionWaitSeconds = 0.f;
		SetMessage(ReaperLang::S(TEXT("カメラの使用を許可してください。"), TEXT("Please allow camera access.")));

		if (UWorld* W = GetWorld())
		{
			W->GetTimerManager().SetTimer(ScanPollTimer, this, &UWG_AddServerIdDialog::PollScan,
				QrPollInterval, true);
		}
		return;
	}
#endif

	BeginScan();
}

void UWG_AddServerIdDialog::BeginScan()
{
	ScanGraceSeconds = QrStartGrace;

	FQrScanner::Start();

	SetMessage(ReaperLang::S(TEXT("QRコードを写してください。"), TEXT("Point the camera at the QR code.")));

	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().SetTimer(ScanPollTimer, this, &UWG_AddServerIdDialog::PollScan,
			QrPollInterval, true);
	}
}

void UWG_AddServerIdDialog::EndScan()
{
	bWaitingCameraPermission = false;
	ScanGraceSeconds = 0.f;

	FQrScanner::Stop();

	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().ClearTimer(ScanPollTimer);
	}
}

void UWG_AddServerIdDialog::PollScan()
{
	// --- カメラ権限の許可待ち ---
	if (bWaitingCameraPermission)
	{
#if PLATFORM_ANDROID
		if (UAndroidPermissionFunctionLibrary::CheckPermission(TEXT("android.permission.CAMERA")))
		{
			bWaitingCameraPermission = false;
			BeginScan();
			return;
		}
#endif
		PermissionWaitSeconds += QrPollInterval;
		if (PermissionWaitSeconds >= QrPermissionTimeout)
		{
			EndScan();
			SetMessage(ReaperLang::S(TEXT("カメラの許可が得られませんでした。URLを貼り付けてください。"),
				TEXT("Camera access was not granted. Paste the URL instead.")), /*bError=*/true);
		}
		return;
	}

	if (ScanGraceSeconds > 0.f)
	{
		ScanGraceSeconds -= QrPollInterval;
	}

	switch (FQrScanner::GetState())
	{
	case EQrScanState::Scanning:
	{
		FString Text;
		if (!FQrScanner::PollDecode(Text))
		{
			// まだ画面内にQRコードが無いだけ。次の1枚を待つ。
			break;
		}

		EndScan();

		if (UrlInput)
		{
			UrlInput->SetText(FText::FromString(Text));
		}

		// 読めた文字列が接続用URLかどうかを、ここで一度見てやる
		// （別のQRコードを読んでしまったときに「登録」を押すまで気付けないのを避ける）。
		FRadnodzPairingInfo Info;
		if (URadnodzClient::ParsePairingUrl(Text, Info))
		{
			SetMessage(FString::Printf(TEXT("%s %s:%d"),
				*ReaperLang::S(TEXT("読み取りました:"), TEXT("Scanned:")), *Info.Host, Info.Port));
		}
		else
		{
			SetMessage(ReaperLang::S(TEXT("接続用URLのQRコードではありませんでした。"),
				TEXT("That QR code is not a connection URL.")), /*bError=*/true);
		}
		break;
	}

	case EQrScanState::Cancelled:
		EndScan();
		SetMessage(ReaperLang::S(TEXT("読み取りを中止しました。"), TEXT("Scanning cancelled.")), /*bError=*/true);
		break;

	case EQrScanState::Failed:
		EndScan();
		SetMessage(ReaperLang::S(TEXT("カメラを起動できませんでした。URLを貼り付けてください。"),
			TEXT("Could not start the camera. Paste the URL instead.")), /*bError=*/true);
		break;

	case EQrScanState::Idle:
		// 猶予のあいだは起動待ちとして扱う（Java側の開始処理はUIスレッド経由で少し遅れる）。
		if (ScanGraceSeconds <= 0.f)
		{
			EndScan();
		}
		break;

	case EQrScanState::Starting:
	default:
		break;
	}
}
