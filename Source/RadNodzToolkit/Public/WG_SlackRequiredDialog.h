// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_SlackRequiredDialog.generated.h"

class UWG_RadNodzToolkit;
class UTextBlock;
class UButton;
class UVerticalBox;
class UBorder;

/**
 * Slack未設定のまま「台本一致」等を押したときの案内モーダル。
 * 「OK」で単に閉じる。「設定へ」は設定タブへ切り替えてから閉じる。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_SlackRequiredDialog : public UUserWidget
{
	GENERATED_BODY()

public:

	/** 親コントローラーを渡して初期化する。 */
	void Setup(UWG_RadNodzToolkit* InOwner);

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:

	void BuildUI();

	UFUNCTION() void HandleOk();
	UFUNCTION() void HandleGoToSettings();

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;

	UPROPERTY(Transient) TObjectPtr<UButton> OkButton;
	UPROPERTY(Transient) TObjectPtr<UButton> GoToSettingsButton;

	bool bUICreated = false;
};
