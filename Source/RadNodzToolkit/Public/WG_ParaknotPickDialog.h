// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_ParaknotPickDialog.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;

/** 選ばれた項目のインデックスを渡す確定デリゲート。 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FParaknotPickDelegate, int32, Index);

/**
 * 押し込み先の PARAKNOT インスタンスを選ぶモーダル。
 *
 * 候補が複数あるときだけ出す（1 件なら呼び出し側が自動で決める）。
 * UI はすべて C++ で構築するため、対応する WBP アセットは不要
 * （UWG_ConfirmDialog と同じ作り）。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_ParaknotPickDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * 選択肢を設定する。
	 * @param InTitle   見出し
	 * @param InItems   1 行ぶんの表示文字列（表示順が OnPicked のインデックスになる）
	 * @param InDimmed  true の行を**明るい背景**で出す（false の行＝すぐ使える
	 *                  ほうを暗く沈ませる）。**押せなくはしない。**
	 *                  「そのままでは使えない」ことを示すだけで、選べば
	 *                  呼び出し側がモードの切り替えを確認する。
	 *                  省略時は全行「すぐ使える」扱い。
	 *
	 * **区別は背景の濃淡で付ける。文字はどちらも白。** 文字色を暗くすると
	 * 背景に溶けて読めなくなる。
	 *
	 * **どちらを使えるとみなすかは呼び出し側が決める。** 用途（モニター /
	 * マイク送信）で望ましいモードが逆なので、ここでは判断できない。
	 */
	void Setup(const FString& InTitle, const TArray<FString>& InItems,
		const TArray<bool>& InDimmed = TArray<bool>());

	/** 行が押されたときに実行される（呼び出し側でバインドする）。 */
	UPROPERTY()
	FParaknotPickDelegate OnPicked;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UFUNCTION() void HandleCancel();

	// 行ごとのハンドラ。**ボタン 1 つに 1 つの UFUNCTION が要る**ので、
	// 上限を決めて用意する（PARAKNOT を 8 つ以上挿す運用は想定しない）。
	UFUNCTION() void HandlePick0();
	UFUNCTION() void HandlePick1();
	UFUNCTION() void HandlePick2();
	UFUNCTION() void HandlePick3();
	UFUNCTION() void HandlePick4();
	UFUNCTION() void HandlePick5();
	UFUNCTION() void HandlePick6();
	UFUNCTION() void HandlePick7();

	void Pick(int32 Index);

	static constexpr int32 MaxItems = 8;

	FString Title;
	TArray<FString> Items;

	/** Items と同じ並び。true の行を明るい背景で出す（= そのままでは使えない）。 */
	TArray<bool> Dimmed;

	UPROPERTY(Transient) TObjectPtr<UTextBlock>    TitleText;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox>  ItemBox;
	UPROPERTY(Transient) TObjectPtr<UButton>       CancelButton;

	bool bUICreated = false;
};
