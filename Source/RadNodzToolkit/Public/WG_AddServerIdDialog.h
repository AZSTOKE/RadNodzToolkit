// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"   // ETextCommit::Type
#include "WG_AddServerIdDialog.generated.h"

class UWG_RadNodzToolkit;
class UEditableTextBox;
class UButton;

/**
 * 新しいサーバーIDを登録するためのモーダルダイアログ（認証設定から開く）。
 * 「登録」で入力した serverId を Owner->AddServerAuthEntry() に渡して追加する。
 * UI はすべて C++ で構築するため、対応する WBP アセットは不要。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_AddServerIdDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 親ウィジェットを渡して初期化する。 */
	void Setup(UWG_RadNodzToolkit* InOwner);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UFUNCTION() void HandleRegister();
	UFUNCTION() void HandleCancel();
	UFUNCTION() void HandleInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	void Commit();

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox>   IdInput;
	UPROPERTY(Transient) TObjectPtr<UButton>            RegisterButton;
	UPROPERTY(Transient) TObjectPtr<UButton>            CancelButton;

	bool bUICreated = false;
};
