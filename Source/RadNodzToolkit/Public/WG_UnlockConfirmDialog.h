// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_UnlockConfirmDialog.generated.h"

class UWG_RadNodzToolkit;
class UTextBlock;
class UButton;
class UBorder;

/**
 * ロック解除の最終確認モーダル。
 * 画面全体を暗くし、「ロックを解除してよろしいですか？ はい / いいえ」を表示する。
 * 誤操作でロックが外れないよう、解除は必ずこのダイアログの「はい」を経由する。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_UnlockConfirmDialog : public UUserWidget
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

	UFUNCTION() void HandleYes();
	UFUNCTION() void HandleNo();

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;

	UPROPERTY(Transient) TObjectPtr<UButton> YesButton;
	UPROPERTY(Transient) TObjectPtr<UButton> NoButton;

	bool bUICreated = false;
};
