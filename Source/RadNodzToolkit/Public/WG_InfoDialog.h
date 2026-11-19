// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_InfoDialog.generated.h"

class UTextBlock;
class UButton;
class UBorder;

/**
 * 汎用の情報表示モーダル（見出し＋本文＋「OK」）。
 * 選ばせるものが無く「見せて閉じるだけ」の用途に使う（例: この端末のFP）。
 * UI はすべて C++ で構築するため、対応する WBP アセットは不要。
 *
 * 使い方:
 *   auto* Dlg = CreateWidget<UWG_InfoDialog>(this, UWG_InfoDialog::StaticClass());
 *   Dlg->Setup(TEXT("この端末のFP"), Fingerprint);
 *   Dlg->AddToViewport(200);
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_InfoDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 見出しと本文を設定する（見出しが空なら見出し行は出さない）。 */
	void Setup(const FString& InTitle, const FString& InMessage);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	void BuildUI();

	UFUNCTION() void HandleOk();

	FString Title;
	FString Message;

	UPROPERTY(Transient) TObjectPtr<UBorder>    HeaderBorder;   // 見出し帯（タイトルが空なら畳む）
	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> MessageText;
	UPROPERTY(Transient) TObjectPtr<UButton>    OkButton;

	bool bUICreated = false;
};
