// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_MediaItemRow.generated.h"

class UWG_RadNodzToolkit;
class UMediaItem;
class UEditableTextBox;
class UButton;
class UTextBlock;
class UBorder;

/**
 * トラックカード内に表示する「メディアアイテム1件」の行。
 * ・「▶」でそのメディア先頭へカーソル移動＋即再生。再生中は「▢」(停止)に切替。
 * ・再生(=カーソル)中の行は背景色でハイライト。
 * ・名前は編集可能（コミットでリネーム）。
 * ・「NG」で末尾に "_NG" 付与。既に "_NG" の行は「OK」に変わり、押すと "_NG" を削除。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_MediaItemRow : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitRow(UMediaItem* InItem, const FString& InName, double InStart, UWG_RadNodzToolkit* InOwner);

	/** この行がカーソル位置（アクティブ）かどうかを設定し、ハイライトを更新する。 */
	void SetActive(bool bInActive);

	/** 再生中かどうかを設定し、▶/▢ の表示を更新する。 */
	void SetPlaying(bool bInPlaying);

	/** ロック中は再生（▶）ボタンを無効化し、見た目も暗くする。 */
	void SetLocked(bool bLocked);

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:

	void BuildUI();
	void CommitName();
	void RefreshRowVisual();
	// 名前末尾の "_NG" を判定し、NG/OKボタン・ミュート・グレーアウト/再生不可を反映する
	void UpdateNGState();
	// レベル（ラウドネス/ピーク/RMS）を取得して表示に反映する
	void FetchAndShowLevels();
	void ApplyLevelsText();

public:
	/**
	 * キャッシュ済みのラウドネスを読み直して表示を更新する（「計測」ボタン後の反映用）。
	 * 未計測なら "--" のまま。自分では計測（計算）を行わない。
	 */
	void RefreshLevelsFromCache();

private:

	void RefreshVolumeText();

	UFUNCTION() void HandleSelectClicked();
	UFUNCTION() void HandleNGClicked();
	UFUNCTION() void HandleNameCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleVolMinusClicked();
	UFUNCTION() void HandleVolPlusClicked();

	UPROPERTY(Transient) TObjectPtr<UMediaItem>            Item;
	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit>  Owner;
	UPROPERTY(Transient) TObjectPtr<UBorder>               RootBorder;
	UPROPERTY(Transient) TObjectPtr<UButton>               SelectButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>            SelectGlyph;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox>      NameInput;
	UPROPERTY(Transient) TObjectPtr<UButton>               NGButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>            NGButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>            LevelsText;
	UPROPERTY(Transient) TObjectPtr<UButton>               VolMinusButton;
	UPROPERTY(Transient) TObjectPtr<UButton>               VolPlusButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>            VolDbText;

	FString MediaName;
	double  StartTime = 0.0;
	double  MomentaryLUFS = 0.0;
	double  PeakDb = -120.0;
	double  RmsDb = -120.0;
	double  VolumeDb = 0.0;
	bool    bLevelsFetched = false;
	bool    bRowActive = false;
	bool    bRowPlaying = false;
	bool    bIsLocked = false;
	bool    bIsNG = false;   // 名前末尾が "_NG"（＝ミュート・再生不可・グレーアウト）
};
