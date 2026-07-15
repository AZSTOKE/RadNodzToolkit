// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_SheetTab.generated.h"

class UWG_RadNodzToolkit;
class UTextBlock;
class UButton;
class UBorder;

/**
 * 「リスト」タブ下部のシート切替ボタン1つ分。
 * 下のホーム/リスト/設定タブと同じ見た目（上端のアクティブ線＋ラベル）。
 * 横に並べて、はみ出したら横スクロールさせる。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_SheetTab : public UUserWidget
{
	GENERATED_BODY()

public:

	/** タブを初期化する。 */
	void InitTab(int32 InIndex, const FString& InLabel, bool bActive, UWG_RadNodzToolkit* InOwner);

	/** アクティブ表示（文字色＋上端の線）を切り替える。 */
	void SetActiveVisual(bool bActive);

	int32 TabIndex = 0;

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:

	void BuildUI();

	UFUNCTION() void HandleClicked();

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;
	UPROPERTY(Transient) TObjectPtr<UButton>    TabButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> TabText;
	UPROPERTY(Transient) TObjectPtr<UBorder>    Indicator;

	FString Label;
	bool bActive    = false;
	bool bUICreated = false;
};
