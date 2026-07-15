// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_DeleteConfirmDialog.generated.h"

class UWG_RadNodzToolkit;
class UTextBlock;
class UButton;
class UVerticalBox;
class UBorder;

/**
 * トラック削除の最終確認モーダル。
 * 画面全体を暗くし、削除対象トラック名の一覧（多い場合はスクロール）と
 * 「本当に削除していいですか？ はい / いいえ」を表示する。
 * 「はい」で Owner のトラック削除を実行する。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_DeleteConfirmDialog : public UUserWidget
{
	GENERATED_BODY()

public:

	/** 親コントローラーと、削除対象トラック名の一覧を渡して初期化する。 */
	void Setup(UWG_RadNodzToolkit* InOwner, const TArray<FString>& InTrackNames);

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:

	void BuildUI();

	UFUNCTION() void HandleYes();
	UFUNCTION() void HandleNo();

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;

	// 削除対象トラック名（表示用）
	TArray<FString> TrackNames;

	UPROPERTY(Transient) TObjectPtr<UVerticalBox> ListBox;   // 名前一覧の入れ物
	UPROPERTY(Transient) TObjectPtr<UButton>      YesButton;
	UPROPERTY(Transient) TObjectPtr<UButton>      NoButton;

	bool bUICreated = false;
};
