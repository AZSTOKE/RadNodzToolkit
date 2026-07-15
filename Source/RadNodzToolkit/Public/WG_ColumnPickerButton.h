// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_ColumnPickerButton.generated.h"

class UWG_RadNodzToolkit;
class UTextBlock;
class UButton;

/**
 * 「台本一致」ボタン押下時に表示する、列選択ピッカーの1ボタン分。
 * クリックすると、その列を文字起こしチェック対象として台本一致処理を即実行する。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_ColumnPickerButton : public UUserWidget
{
	GENERATED_BODY()

public:

	/** ボタンを初期化する。InLabelはExcel列文字（A/B/C…）。 */
	void InitButton(int32 InColIndex, const FString& InLabel, UWG_RadNodzToolkit* InOwner);

	int32 ColIndex = 0;

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:

	void BuildUI();

	UFUNCTION() void HandleClicked();

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;
	UPROPERTY(Transient) TObjectPtr<UButton>    ColButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ColText;

	FString Label;
	bool bUICreated = false;
};
