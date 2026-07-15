// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_ListRow.generated.h"

class UWG_RadNodzToolkit;
class UButton;
class UHorizontalBox;
class UBorder;
class UTextBlock;
class UImage;

/**
 * 「リスト」タブの1行。読み込んだ各列の値を横並びで表示する。
 * 各セルは読み取り専用の入力欄（コピー可・1行・幅を超えたら横スクロール）。
 * 左端に収録済みチェックボックス（チェックでアイコン表示＋行グレーアウト）。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_ListRow : public UUserWidget
{
	GENERATED_BODY()

public:

	/**
	 * 1行を初期化する。
	 * InColumnWidthsは列ごとの最小幅（実測ピクセル）。他行と揃えるために使う（省略可）。
	 * bInHeaderRowがtrueなら見出し行として構築する（チェックボックスは非活性、文字はやや薄い強調表示）。
	 * InHighlightColIndexは強調表示する列（台本一致でチェック対象に選んだ列）のindex。-1で無効。
	 */
	void InitRow(int32 InRowIndex, const TArray<FString>& InCells, bool bChecked, bool bShowCheckbox, UWG_RadNodzToolkit* InOwner,
		const TArray<float>& InColumnWidths = TArray<float>(), bool bInHeaderRow = false, int32 InHighlightColIndex = INDEX_NONE);

	/** チェック状態の見た目（チェックアイコン＋グレーアウト）を更新する。 */
	void RefreshChecked(bool bChecked);

	int32 RowIndex = 0;

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:

	void BuildUI();

	/** チェックボタンの見た目（枠付き空ボックス／緑塗り＋チェックアイコン）を適用する。 */
	void ApplyCheckStyle(bool bChecked);

	UFUNCTION() void HandleCheckClicked();

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;

	UPROPERTY(Transient) TObjectPtr<UBorder>         RootBorder;
	UPROPERTY(Transient) TObjectPtr<class USizeBox>  CheckSize;
	UPROPERTY(Transient) TObjectPtr<UButton>         CheckButton;
	UPROPERTY(Transient) TObjectPtr<UImage>          CheckIcon;     // チェック時に表示するアイコン
	UPROPERTY(Transient) TObjectPtr<UHorizontalBox>  CellsBox;
	UPROPERTY(Transient) TArray<TObjectPtr<UTextBlock>> CellTexts;   // 各列（最小幅＋内容で自動拡張・横スクロールで全文表示）

	TArray<FString> Cells;
	TArray<float> ColumnWidths;
	bool bIsChecked = false;
	bool bShowCheck = true;
	bool bIsHeaderRow = false;
	int32 HighlightColIndex = INDEX_NONE;

	bool bUICreated = false;
};
