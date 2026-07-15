// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_ReaperScanDialog.h"
#include "WG_RadNodzToolkit.h"
#include "ReaperFont.h"
#include "ReaperLang.h"

#include "RadnodzClient.h"   // URadnodzUtility::AZ_GetLocalIP / URadnodzClient
#include "RigdocksSilver_Device.h"   // URigdocksSilver_Device::AZ_GetDeviceName

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Brushes/SlateRoundedBoxBrush.h"

#include "Async/Async.h"
#include "Async/ParallelFor.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPAddress.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "HAL/CriticalSection.h"

namespace ReaperScanUI
{
	static const FLinearColor Dim        = FLinearColor(0.f, 0.f, 0.f, 0.62f);   // 背景の暗幕
	static const FLinearColor CardBg     = FLinearColor(0.034f, 0.072f, 0.085f, 1.f);
	static const FLinearColor PanelBg    = FLinearColor(0.046f, 0.100f, 0.118f, 1.f);
	static const FLinearColor TextHi     = FLinearColor(0.85f, 0.93f, 0.94f, 1.f);
	static const FLinearColor TextDim    = FLinearColor(0.40f, 0.58f, 0.61f, 1.f);
	static const FLinearColor Teal       = FLinearColor(0.12f, 0.55f, 0.55f, 1.f);
	static const FLinearColor Gray       = FLinearColor(0.16f, 0.22f, 0.25f, 1.f);
	static const FLinearColor White      = FLinearColor::White;

	static void StyleBtn(UButton* Btn, FLinearColor Bg)
	{
		if (!Btn) return;
		FButtonStyle S = Btn->GetStyle();
		S.Normal  = FSlateRoundedBoxBrush(FLinearColor::White, 10.f);
		S.Hovered = FSlateRoundedBoxBrush(FLinearColor(1.12f, 1.12f, 1.12f, 1.f), 10.f);
		S.Pressed = FSlateRoundedBoxBrush(FLinearColor(0.85f, 0.85f, 0.85f, 1.f), 10.f);
		Btn->SetStyle(S);
		Btn->SetBackgroundColor(Bg);
	}

	// 指定IP:PortへTCP接続できるか（短いタイムアウトで判定）。openなら true。
	static bool ProbeTcp(const FString& IP, int32 Port, double TimeoutMs)
	{
		ISocketSubsystem* SS = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		if (!SS) return false;

		FIPv4Address Addr;
		if (!FIPv4Address::Parse(IP, Addr)) return false;

		TSharedRef<FInternetAddr> InetAddr = SS->CreateInternetAddr();
		InetAddr->SetIp(Addr.Value);
		InetAddr->SetPort(Port);

		FSocket* Sock = SS->CreateSocket(NAME_Stream, TEXT("reaper-probe"), false);
		if (!Sock) return false;

		Sock->SetNonBlocking(true);
		Sock->Connect(*InetAddr);

		// 接続完了（成功 or 拒否）で書き込み可能になる。タイムアウトなら到達不可。
		bool bOk = Sock->Wait(ESocketWaitConditions::WaitForWrite, FTimespan::FromMilliseconds(TimeoutMs));
		if (bOk)
		{
			// 拒否(ConnectionError)は除外し、確立できたものだけを「Reaperあり」とみなす
			bOk = (Sock->GetConnectionState() == SCS_Connected);
		}

		Sock->Close();
		SS->DestroySocket(Sock);
		return bOk;
	}
}

// ====================================================================
//  UWG_ReaperScanItem
// ====================================================================
TSharedRef<SWidget> UWG_ReaperScanItem::RebuildWidget()
{
	if (WidgetTree && !WidgetTree->RootWidget)
	{
		BuildUI();
	}
	return Super::RebuildWidget();
}

void UWG_ReaperScanItem::NativeConstruct()
{
	Super::NativeConstruct();
	if (Btn && !Btn->OnClicked.IsAlreadyBound(this, &UWG_ReaperScanItem::HandleClicked))
	{
		Btn->OnClicked.AddDynamic(this, &UWG_ReaperScanItem::HandleClicked);
	}
}

void UWG_ReaperScanItem::BuildUI()
{
	using namespace ReaperScanUI;

	// ルートは非クリックのBorderにする（以前は行全体がUButtonで、スクロール操作が
	// ほぼ確実に「押下」として奪われ、一覧をスクロールしようとして誤って選択してしまっていた）。
	// クリック可能な範囲は右端の「接続」ボタンだけに絞り、それ以外（IP/端末名）は
	// タップしてもスクロールジェスチャーとして自然に流れるようにする。
	UBorder* RootBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
	RootBorder->SetBrush(FSlateRoundedBoxBrush(PanelBg, 10.f));

	UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	RootBorder->SetContent(Row);

	UTextBlock* IpText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	IpText->SetText(FText::FromString(IP));
	IpText->SetFont(ReaperFont::Get(22, true));
	IpText->SetColorAndOpacity(FSlateColor(TextHi));
	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(IpText);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(18.f, 14.f, 0.f, 14.f));
	}

	// IPの横に出す端末名。解決前は「取得中…」表示。
	NameText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	NameText->SetText(FText::FromString(DeviceName.IsEmpty()
		? ReaperLang::S(TEXT("端末名 取得中…"), TEXT("Resolving device name…")) : DeviceName));
	NameText->SetFont(ReaperFont::Get(18, false));
	NameText->SetColorAndOpacity(FSlateColor(TextDim));
	{
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(NameText);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		S->SetPadding(FMargin(12.f, 14.f, 0.f, 14.f));
	}

	// クリック可能な範囲はここだけ（「接続」ラベルを囲む小さめのボタン）。
	Btn = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ScanItemBtn"));
	StyleBtn(Btn, Gray);
	UTextBlock* Arrow = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	Arrow->SetText(FText::FromString(ReaperLang::S(TEXT("接続 ▶"), TEXT("Connect ▶"))));
	Arrow->SetFont(ReaperFont::Get(18, true));
	Arrow->SetColorAndOpacity(FSlateColor(Teal));
	Arrow->SetJustification(ETextJustify::Center);
	Btn->SetContent(Arrow);
	{
		USizeBox* BtnSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		BtnSize->SetMinDesiredWidth(120.f);
		BtnSize->SetMinDesiredHeight(56.f);
		BtnSize->AddChild(Btn);
		UHorizontalBoxSlot* S = Row->AddChildToHorizontalBox(BtnSize);
		S->SetVerticalAlignment(VAlign_Center);
		S->SetPadding(FMargin(0.f, 0.f, 18.f, 0.f));
	}

	WidgetTree->RootWidget = RootBorder;
}

void UWG_ReaperScanItem::Init(const FString& InIP, UWG_ReaperScanDialog* InDialog)
{
	IP = InIP;
	Dialog = InDialog;
}

void UWG_ReaperScanItem::SetDeviceName(const FString& InName)
{
	using namespace ReaperScanUI;
	DeviceName = InName;
	if (NameText)
	{
		// 取得できなければ「-」、取得できれば端末名を表示。
		NameText->SetText(FText::FromString(InName.IsEmpty() ? FString(TEXT("-")) : InName));
		NameText->SetColorAndOpacity(FSlateColor(InName.IsEmpty() ? TextDim : TextHi));
	}
}

void UWG_ReaperScanItem::HandleClicked()
{
	if (Dialog)
	{
		Dialog->Pick(IP);
	}
}

// ====================================================================
//  UWG_ReaperScanDialog
// ====================================================================
TSharedRef<SWidget> UWG_ReaperScanDialog::RebuildWidget()
{
	if (WidgetTree && !WidgetTree->RootWidget && !bUICreated)
	{
		BuildUI();
		bUICreated = true;
	}
	return Super::RebuildWidget();
}

void UWG_ReaperScanDialog::NativeConstruct()
{
	Super::NativeConstruct();
	if (CloseButton && !CloseButton->OnClicked.IsAlreadyBound(this, &UWG_ReaperScanDialog::HandleClose))
	{
		CloseButton->OnClicked.AddDynamic(this, &UWG_ReaperScanDialog::HandleClose);
	}
	if (RescanButton && !RescanButton->OnClicked.IsAlreadyBound(this, &UWG_ReaperScanDialog::HandleRescan))
	{
		RescanButton->OnClicked.AddDynamic(this, &UWG_ReaperScanDialog::HandleRescan);
	}
}

void UWG_ReaperScanDialog::BuildUI()
{
	using namespace ReaperScanUI;

	// 背景の暗幕（タップ無効なBorder）
	UBorder* Dimmer = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ScanDimmer"));
	Dimmer->SetBrushColor(Dim);
	Dimmer->SetHorizontalAlignment(HAlign_Center);
	Dimmer->SetVerticalAlignment(VAlign_Center);
	Dimmer->SetPadding(FMargin(24.f));
	WidgetTree->RootWidget = Dimmer;

	// 中央のカード
	USizeBox* CardSize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	CardSize->SetWidthOverride(680.f);
	CardSize->SetMaxDesiredHeight(900.f);
	Dimmer->SetContent(CardSize);

	UBorder* Card = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ScanCard"));
	Card->SetBrush(FSlateRoundedBoxBrush(FLinearColor::White, 16.f));
	Card->SetBrushColor(CardBg);
	Card->SetPadding(FMargin(26.f, 24.f));
	CardSize->AddChild(Card);

	UVerticalBox* Col = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	Card->SetContent(Col);

	// タイトル
	{
		UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Title->SetText(FText::FromString(ReaperLang::S(TEXT("Reaper検索（同ネットワーク）"), TEXT("Search for Reaper (same network)"))));
		Title->SetFont(ReaperFont::Get(26, true));
		Title->SetColorAndOpacity(FSlateColor(TextHi));
		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(Title);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 12.f));
	}

	// ステータス
	StatusText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ScanStatus"));
	StatusText->SetText(FText::FromString(ReaperLang::S(TEXT("検索中…"), TEXT("Searching…"))));
	StatusText->SetFont(ReaperFont::Get(18, false));
	StatusText->SetColorAndOpacity(FSlateColor(TextDim));
	{
		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(StatusText);
		S->SetPadding(FMargin(2.f, 0.f, 0.f, 12.f));
	}

	// 結果リスト
	ListBox = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("ScanList"));
	{
		USizeBox* LS = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		LS->SetMinDesiredHeight(120.f);
		LS->SetMaxDesiredHeight(560.f);
		LS->AddChild(ListBox);
		UVerticalBoxSlot* S = Col->AddChildToVerticalBox(LS);
		S->SetHorizontalAlignment(HAlign_Fill);
		S->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
	}

	// 下部ボタン：再検索 / 閉じる
	UHorizontalBox* BtnRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	Col->AddChildToVerticalBox(BtnRow);

	RescanButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("RescanButton"));
	StyleBtn(RescanButton, Teal);
	{
		UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(ReaperLang::S(TEXT("再検索"), TEXT("Rescan"))));
		T->SetFont(ReaperFont::Get(20, true));
		T->SetColorAndOpacity(FSlateColor(White));
		T->SetJustification(ETextJustify::Center);
		T->SetAutoWrapText(!ReaperLang::IsJapanese());
		RescanButton->SetContent(T);
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredHeight(72.f); B->AddChild(RescanButton);
		UHorizontalBoxSlot* S = BtnRow->AddChildToHorizontalBox(B);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		S->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
	}

	CloseButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ScanCloseButton"));
	StyleBtn(CloseButton, Gray);
	{
		UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		T->SetText(FText::FromString(ReaperLang::S(TEXT("閉じる"), TEXT("Close"))));
		T->SetFont(ReaperFont::Get(20, true));
		T->SetColorAndOpacity(FSlateColor(White));
		T->SetJustification(ETextJustify::Center);
		T->SetAutoWrapText(!ReaperLang::IsJapanese());
		CloseButton->SetContent(T);
		USizeBox* B = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		B->SetMinDesiredHeight(72.f); B->AddChild(CloseButton);
		UHorizontalBoxSlot* S = BtnRow->AddChildToHorizontalBox(B);
		S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		S->SetPadding(FMargin(8.f, 0.f, 0.f, 0.f));
	}
}

void UWG_ReaperScanDialog::Setup(UWG_RadNodzToolkit* InOwner)
{
	Owner = InOwner;
	StartScan();
}

void UWG_ReaperScanDialog::HandleClose()
{
	RemoveFromParent();
}

void UWG_ReaperScanDialog::HandleRescan()
{
	StartScan();
}

void UWG_ReaperScanDialog::Pick(const FString& InIP)
{
	if (Owner)
	{
		Owner->SelectScannedReaper(InIP);
	}
	RemoveFromParent();
}

void UWG_ReaperScanDialog::StartScan()
{
	if (bScanning || !Owner) return;

	const FString LocalIP = URadnodzUtility::AZ_GetLocalIP();
	const int32 Port = Owner->Port;

	// 「192.168.50.8」→ base「192.168.50.」, 自分のオクテット 8
	int32 SelfOctet = -1;
	FString Base;
	{
		TArray<FString> Parts;
		LocalIP.ParseIntoArray(Parts, TEXT("."), true);
		if (Parts.Num() == 4)
		{
			Base = Parts[0] + TEXT(".") + Parts[1] + TEXT(".") + Parts[2] + TEXT(".");
			SelfOctet = FCString::Atoi(*Parts[3]);
		}
	}

	if (Base.IsEmpty())
	{
		if (StatusText)
		{
			StatusText->SetText(FText::FromString(ReaperLang::S(TEXT("この端末のIPが取得できませんでした"), TEXT("Could not get this device's IP"))));
		}
		return;
	}

	bScanning = true;
	if (ListBox) { ListBox->ClearChildren(); }
	if (StatusText)
	{
		{
			const FString Fmt = ReaperLang::S(TEXT("検索中…  {0}0〜255 : ポート{1}"), TEXT("Searching…  {0}0-255 : port {1}"));
			StatusText->SetText(FText::FromString(FString::Format(*Fmt, { FStringFormatArg(Base), FStringFormatArg(Port) })));
		}
	}

	TWeakObjectPtr<UWG_ReaperScanDialog> WeakThis(this);

	Async(EAsyncExecution::Thread, [WeakThis, Base, Port]()
	{
		FCriticalSection Mutex;
		TArray<FString> Found;

		// /24 全ホストをホストごとに専用スレッドで並列にプローブする。
		// ParallelFor はCPUコア数分の並列度しか出ず、ネットワーク待ちが支配的なこの処理では
		// 遅すぎる（254件を数コアで捌くと合計待ち時間が積み上がる）ため、254スレッドを
		// 同時に立てて一斉にプローブする（LAN内の短時間バーストなので問題ない）。
		// タイムアウトも 350ms→150ms に短縮（LAN内なら生きているホストは数msで応答するため）。
		// ※ 自分自身も対象に含める（同一PCでREAPERが動いている場合に検出するため）。
		constexpr double ProbeTimeoutMs = 150.0;
		TArray<TFuture<void>> Futures;
		Futures.Reserve(254);
		for (int32 Idx = 0; Idx < 254; ++Idx)
		{
			const int32 Host = Idx + 1;           // 1..254
			const FString IP = Base + FString::FromInt(Host);
			Futures.Add(Async(EAsyncExecution::Thread, [IP, Port, &Mutex, &Found]()
			{
				if (ReaperScanUI::ProbeTcp(IP, Port, ProbeTimeoutMs))
				{
					FScopeLock Lock(&Mutex);
					Found.Add(IP);
				}
			}));
		}
		for (TFuture<void>& F : Futures) { F.Wait(); }

		// 同一PC（ローカルループバック）も確認する。REAPERが 127.0.0.1 のみで待ち受ける場合に拾う。
		if (ReaperScanUI::ProbeTcp(TEXT("127.0.0.1"), Port, ProbeTimeoutMs))
		{
			FScopeLock Lock(&Mutex);
			Found.AddUnique(TEXT("127.0.0.1"));
		}

		// 末尾オクテット順にソート
		Found.Sort([](const FString& A, const FString& B)
		{
			auto Last = [](const FString& S) {
				int32 Dot; return S.FindLastChar('.', Dot) ? FCString::Atoi(*S.RightChop(Dot + 1)) : 0;
			};
			return Last(A) < Last(B);
		});

		AsyncTask(ENamedThreads::GameThread, [WeakThis, Found]()
		{
			if (UWG_ReaperScanDialog* Self = WeakThis.Get())
			{
				Self->ShowResults(Found);
			}
		});
	});
}

void UWG_ReaperScanDialog::ShowResults(const TArray<FString>& FoundIPs)
{
	using namespace ReaperScanUI;

	bScanning = false;
	if (!ListBox) return;
	ListBox->ClearChildren();
	ItemByIP.Reset();

	if (FoundIPs.Num() == 0)
	{
		if (StatusText)
		{
			StatusText->SetText(FText::FromString(ReaperLang::S(TEXT("同ネットワークにReaperが見つかりません"), TEXT("No Reaper found on the same network"))));
			StatusText->SetColorAndOpacity(FSlateColor(FLinearColor(0.95f, 0.45f, 0.30f, 1.f)));
		}
		return;
	}

	if (StatusText)
	{
		{
			const FString Fmt = ReaperLang::S(TEXT("{0} 件見つかりました（タップで接続）"), TEXT("{0} found (tap to connect)"));
			StatusText->SetText(FText::FromString(FString::Format(*Fmt, { FStringFormatArg(FoundIPs.Num()) })));
		}
		StatusText->SetColorAndOpacity(FSlateColor(TextDim));
	}

	for (const FString& IP : FoundIPs)
	{
		UWG_ReaperScanItem* Item = CreateWidget<UWG_ReaperScanItem>(this, UWG_ReaperScanItem::StaticClass());
		if (!Item) continue;
		Item->Init(IP, this);
		ItemByIP.Add(IP, Item);
		UScrollBoxSlot* S = Cast<UScrollBoxSlot>(ListBox->AddChild(Item));
		if (S)
		{
			S->SetHorizontalAlignment(HAlign_Fill);
			S->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
		}
	}

	// 見つかった各IPの端末名を背景で解決して各項目へ反映する（UIはブロックしない）。
	ResolveDeviceNames(FoundIPs);
}

void UWG_ReaperScanDialog::ResolveDeviceNames(const TArray<FString>& FoundIPs)
{
	if (FoundIPs.Num() == 0 || !Owner) return;

	const int32 Port = Owner->Port;

	// 各IP用のクライアントをゲームスレッドで生成しておく（NewObjectはゲームスレッド限定）。
	// GC回収を防ぐため AddToRoot し、名前解決後に RemoveFromRoot する。
	TArray<FString> IPs = FoundIPs;
	TArray<URadnodzClient*> Clients;
	Clients.Reserve(IPs.Num());
	for (int32 i = 0; i < IPs.Num(); ++i)
	{
		URadnodzClient* C = URadnodzClient::CreateRadnodzClient();
		if (C) { C->AddToRoot(); }
		Clients.Add(C);
	}

	TWeakObjectPtr<UWG_ReaperScanDialog> WeakThis(this);

	Async(EAsyncExecution::Thread, [WeakThis, IPs, Clients, Port]()
	{
		TArray<FString> Names;
		Names.SetNum(IPs.Num());

		// 各IPへ接続して端末名を取得（少数なので並列化）。
		// AZ_GetDeviceName は FString を返すだけで UObject 生成が無く、スレッド安全。
		ParallelFor(IPs.Num(), [&](int32 Idx)
		{
			URadnodzClient* C = Clients[Idx];
			if (!C) { return; }
			if (C->Connect(IPs[Idx], Port))
			{
				Names[Idx] = URigdocksSilver_Device::AZ_GetDeviceName(C);
			}
		});

		// 結果反映とクライアントの後片付けはゲームスレッドで行う。
		AsyncTask(ENamedThreads::GameThread, [WeakThis, IPs, Clients, Names]()
		{
			UWG_ReaperScanDialog* Self = WeakThis.Get();
			for (int32 Idx = 0; Idx < IPs.Num(); ++Idx)
			{
				if (URadnodzClient* C = Clients[Idx])
				{
					C->Disconnect();
					C->RemoveFromRoot();
				}
				if (Self)
				{
					if (TObjectPtr<UWG_ReaperScanItem>* Found = Self->ItemByIP.Find(IPs[Idx]))
					{
						if (*Found) { (*Found)->SetDeviceName(Names[Idx]); }
					}
				}
			}
		});
	});
}
