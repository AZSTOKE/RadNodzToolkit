// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_ReaperScanDialog.generated.h"

class UWG_RadNodzToolkit;
class UWG_ReaperScanDialog;
class UTextBlock;
class UButton;
class UScrollBox;
class UBorder;

/**
 * スキャン結果の1行（見つかったReaperのIP）。
 * タップするとそのIPで接続し、ダイアログを閉じる。
 */
UCLASS()
class RADNODZTOOLKIT_API UWG_ReaperScanItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FString& InIP, UWG_ReaperScanDialog* InDialog);

	/** 端末名の解決後に呼ばれ、IPの横の端末名表示を更新する（空なら消す）。 */
	void SetDeviceName(const FString& InName);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:
	void BuildUI();
	UFUNCTION() void HandleClicked();

	UPROPERTY(Transient) TObjectPtr<UWG_ReaperScanDialog> Dialog;
	UPROPERTY(Transient) TObjectPtr<UButton> Btn;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> NameText;   // IPの横に出す端末名
	FString IP;
	FString DeviceName;
	bool bUICreated = false;
};

/**
 * 「Reaper検索」ウインドウ。
 * 同一ネットワーク（/24サブネット）内で、設定中のポートに接続できるホストを探索し、
 * 見つかったIPを一覧表示する。選択するとそのIPで接続する。
 * 何も見つからなければ「同ネットワークにReaperが見つかりません」と表示する。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_ReaperScanDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 親コントローラーを渡して初期化し、探索を開始する。 */
	void Setup(UWG_RadNodzToolkit* InOwner);

	/** 一覧の項目から呼ばれる：IPを確定して接続し、ダイアログを閉じる。 */
	void Pick(const FString& InIP);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:
	void BuildUI();
	void StartScan();
	void ShowResults(const TArray<FString>& FoundIPs);
	// 見つかった各IPへ背景で接続し、端末名を解決して各項目へ反映する。
	void ResolveDeviceNames(const TArray<FString>& FoundIPs);

	UFUNCTION() void HandleClose();
	UFUNCTION() void HandleRescan();

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> StatusText;
	UPROPERTY(Transient) TObjectPtr<UScrollBox> ListBox;
	UPROPERTY(Transient) TObjectPtr<UButton>    RescanButton;
	UPROPERTY(Transient) TObjectPtr<UButton>    CloseButton;

	// IP → 項目ウィジェット。端末名の解決結果を該当項目へ反映するのに使う。
	UPROPERTY(Transient) TMap<FString, TObjectPtr<UWG_ReaperScanItem>> ItemByIP;

	bool bUICreated = false;
	bool bScanning = false;
};
