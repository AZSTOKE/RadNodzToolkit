// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_AddTracksDialog.generated.h"

class UWG_RadNodzToolkit;
class UEditableTextBox;
class UTextBlock;
class UButton;
class UVerticalBox;
class UBorder;
class UComboBoxString;

/**
 * 「＋追加」で開くモーダルダイアログ。
 * 収録素材名のみを入力すると、設定で登録済みのマイク本数分だけ
 *   収録素材名_マイク名[_連番]
 * の形式でトラックを一括追加する（マイク名の登録は「設定」ダイアログで行う）。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_AddTracksDialog : public UUserWidget
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

	// 登録済みマイク名のプレビュー表示を作り直す
	void RebuildMicPreview();
	// 親トラックの選択肢（先頭="Master"＋接続中のトラック名）を埋める
	void PopulateParentCombo();
	// 親トラックコンボの項目生成（"Master" は赤で区別）
	UFUNCTION() UWidget* MakeParentItem(FString Item);

	UFUNCTION() void HandleAdd();
	UFUNCTION() void HandleCancel();

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;

	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> MaterialInput;
	UPROPERTY(Transient) TObjectPtr<UComboBoxString>  ParentCombo;   // 追加先の親トラック（先頭 Master＝最上位）
	UPROPERTY(Transient) TObjectPtr<UVerticalBox>     MicPreviewBox;
	UPROPERTY(Transient) TObjectPtr<UButton>          AddButton;
	UPROPERTY(Transient) TObjectPtr<UButton>          CancelButton;

	bool bUICreated = false;
};
