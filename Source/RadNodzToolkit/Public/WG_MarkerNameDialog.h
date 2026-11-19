// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"   // ETextCommit::Type
#include "WG_MarkerNameDialog.generated.h"

class UWG_RadNodzToolkit;
class UEditableTextBox;
class UButton;

/**
 * マーカー名を設定するためのモーダルダイアログ。
 * マーカーボタン（再生中/録音中に「削除」から切り替わる）を押すと、押した瞬間の
 * プレイ位置（＋オフセット秒）を固定してこのダイアログを開く。
 * 「設定」または Enter で入力した名前、「キャンセル」で名前なしのまま、
 * いずれも Owner->CommitMarker(Position, Name) を呼んでマーカーを作成する。
 * UI はすべて C++ で構築するため、対応する WBP アセットは不要。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_MarkerNameDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 新規作成モード：親ウィジェットと、確定済みのマーカー位置（秒）を渡して初期化する。 */
	void Setup(UWG_RadNodzToolkit* InOwner, double InPositionSeconds);

	/** リネームモード：既存マーカーの名前を編集する。確定で Owner->RenameMarker を呼ぶ（新規作成しない）。 */
	void SetupRename(UWG_RadNodzToolkit* InOwner, int32 InMarkerId, const FString& InCurrentName);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UFUNCTION() void HandleSet();
	UFUNCTION() void HandleCancel();
	UFUNCTION() void HandleInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	// 履歴ボタン（固定8個ぶんのハンドラ。押すと対応する履歴名を入力欄へ流し込む）。
	UFUNCTION() void HandleHistory0();
	UFUNCTION() void HandleHistory1();
	UFUNCTION() void HandleHistory2();
	UFUNCTION() void HandleHistory3();
	UFUNCTION() void HandleHistory4();
	UFUNCTION() void HandleHistory5();
	UFUNCTION() void HandleHistory6();
	UFUNCTION() void HandleHistory7();
	void ApplyHistory(int32 Index);

	// マーカーを作成してダイアログを閉じる。bUseName=false のときは名前なしで作成する。
	void Commit(bool bUseName);

	// 履歴セクションに表示できる最大件数（HandleHistory0..7 に対応）。
	static constexpr int32 MaxHistoryButtons = 8;

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox>   NameInput;
	UPROPERTY(Transient) TObjectPtr<UButton>            SetButton;
	UPROPERTY(Transient) TObjectPtr<UButton>            CancelButton;

	// 表示中の履歴名（ボタンのインデックスと1対1で対応）。
	TArray<FString> HistoryValues;

	double PositionSeconds = 0.0;
	bool bUICreated = false;
	bool bCommitted = false;   // 二重確定防止（設定→RemoveFromParent 後にキャンセルが飛ぶのを防ぐ）

	// リネームモード（既存マーカー名の編集）。false=新規作成モード。
	bool    bRenameMode = false;
	int32   RenameMarkerId = 0;
	FString InitialName;       // リネーム時に入力欄へ初期表示する名前
};
