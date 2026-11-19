// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"   // ETextCommit::Type
#include "WG_MarkerRow.generated.h"

class UWG_RadNodzToolkit;
class UButton;
class UTextBlock;
class UEditableTextBox;
class UBorder;

/**
 * 「マーカー」表示の1行。左に再生/停止トグル（▶⇄■）、中央に編集可能なマーカー名、右に時刻。
 * ・「▶」でそのマーカー位置へカーソル移動＋即再生。再生中は「■」(停止)に切替。
 * ・名前はテキストボックスで、確定するとマーカー名をリネームする。
 * UI はすべて C++ で構築するため、対応する WBP アセットは不要。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_MarkerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 行を初期化する。InMarkerId は Set 系APIで使うマーカーid、InName は名前、InStartTime はジャンプ先の秒。 */
	void Setup(UWG_RadNodzToolkit* InOwner, int32 InRowIndex, int32 InMarkerId,
		const FString& InName, double InStartTime, const FString& InTimeLabel);

	/** 再生中かどうかを設定し、▶/■ 表示と背景を更新する。 */
	void SetPlaying(bool bInPlaying);

	/** カーソル位置（選択）としてハイライトするか。 */
	void SetActive(bool bInActive);

	/** 表示中のマーカー名を更新する（リネーム確定後の反映用）。 */
	void SetName(const FString& NewName);

	double GetStartTime() const { return StartTime; }
	int32 GetMarkerId() const { return MarkerId; }
	int32 RowIndex = 0;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:
	UFUNCTION() void HandlePlayClicked();
	UFUNCTION() void HandleEditClicked();   // 「編集」ボタン → 編集ダイアログを開く
	void RefreshVisual();

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;
	UPROPERTY(Transient) TObjectPtr<UBorder>            RootBorder;
	UPROPERTY(Transient) TObjectPtr<UButton>            PlayButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>         PlayGlyph;
	UPROPERTY(Transient) TObjectPtr<UButton>            EditButton;   // 名前編集ダイアログを開くボタン
	UPROPERTY(Transient) TObjectPtr<UTextBlock>         EditGlyph;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>         NameText;     // 表示のみ（タップ不可）
	UPROPERTY(Transient) TObjectPtr<UTextBlock>         TimeText;

	int32   MarkerId = 0;
	FString MarkerName;
	FString TimeLabel;
	double  StartTime = 0.0;
	bool    bPlaying = false;
	bool    bActive = false;
	bool    bUICreated = false;
};
