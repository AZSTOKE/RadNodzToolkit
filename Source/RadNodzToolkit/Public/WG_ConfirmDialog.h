// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_ConfirmDialog.generated.h"

class UTextBlock;
class UButton;

/** 「はい」押下時に呼ばれる確定デリゲート。 */
DECLARE_DYNAMIC_DELEGATE(FRadnodzConfirmDelegate);

/**
 * 汎用のはい/いいえ確認モーダル。メッセージを表示し、「はい」で OnConfirmed を実行する。
 * UI はすべて C++ で構築するため、対応する WBP アセットは不要。
 *
 * 使い方:
 *   auto* Dlg = CreateWidget<UWG_ConfirmDialog>(this, UWG_ConfirmDialog::StaticClass());
 *   Dlg->Setup(TEXT("削除しますか？"));
 *   Dlg->OnConfirmed.BindDynamic(this, &UMyClass::DoDelete);
 *   Dlg->AddToViewport(200);
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_ConfirmDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 表示メッセージを設定する。 */
	void Setup(const FString& InMessage);

	/** 「はい」が押されたときに実行されるデリゲート（呼び出し側でバインドする）。 */
	UPROPERTY()
	FRadnodzConfirmDelegate OnConfirmed;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UFUNCTION() void HandleYes();
	UFUNCTION() void HandleNo();

	FString Message;

	UPROPERTY(Transient) TObjectPtr<UTextBlock> MessageText;
	UPROPERTY(Transient) TObjectPtr<UButton>    YesButton;
	UPROPERTY(Transient) TObjectPtr<UButton>    NoButton;

	bool bUICreated = false;
};
