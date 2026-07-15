// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"   // ESelectInfo
#include "DataClass/FTrackDetail.h"

#include "WG_TrackCard.generated.h"

class UWG_RadNodzToolkit;
class UWG_MediaItemRow;
class UMediaTrack;
class UTextBlock;
class UButton;
class UBorder;
class UEditableTextBox;
class UVerticalBox;
class UImage;
class UCanvasPanel;
class UComboBoxString;

/**
 * トラック1件を表示するカード型サブウィジェット。
 *
 * UIはすべてC++(RebuildWidget)で構築する。
 * 親(WG_RadNodzToolkit)の OnTracksLoaded から CreateWidget され、
 * InitTrackCard() でトラック情報を受け取る。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_TrackCard : public UUserWidget
{
	GENERATED_BODY()

public:

	/** トラックのインデックス（0始まり）*/
	UPROPERTY(BlueprintReadOnly, Category = "TrackCard")
	int32 TrackIndex = 0;

	/** トラック名 */
	UPROPERTY(BlueprintReadOnly, Category = "TrackCard")
	FString TrackName;

	/** 現在アームされているか */
	UPROPERTY(BlueprintReadOnly, Category = "TrackCard")
	bool bIsArmed = false;

	/** フォルダの階層レベル（0 = ルート）。インデント表示に使う。 */
	UPROPERTY(BlueprintReadOnly, Category = "TrackCard")
	int32 HierarchyLevel = 0;

	/** 子トラックを持つ親（フォルダ）トラックか。 */
	UPROPERTY(BlueprintReadOnly, Category = "TrackCard")
	bool bIsParent = false;

	/** 親トラックが畳み込まれているか。 */
	UPROPERTY(BlueprintReadOnly, Category = "TrackCard")
	bool bIsCollapsed = false;

	/** ソロ中か。 */
	UPROPERTY(BlueprintReadOnly, Category = "TrackCard")
	bool bIsSoloed = false;

	/** ミュート中か。 */
	UPROPERTY(BlueprintReadOnly, Category = "TrackCard")
	bool bIsMuted = false;

	/** メディア一覧を展開表示中か。 */
	UPROPERTY(BlueprintReadOnly, Category = "TrackCard")
	bool bMediaExpanded = false;

	/** （親フォルダ用）配下の子トラックがすべてREC中か。親ボタンの点灯表示に使う。 */
	UPROPERTY(BlueprintReadOnly, Category = "TrackCard")
	bool bChildrenArmed = false;

	/** 親コントローラーへの参照 */
	UPROPERTY(BlueprintReadOnly, Category = "TrackCard")
	TObjectPtr<UWG_RadNodzToolkit> OwnerController;

	/**
	 * 親ウィジェットからカード情報を初期化する。
	 * CreateWidgetした直後に呼ぶ。
	 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void InitTrackCard(const FTrackDetail& Detail, UWG_RadNodzToolkit* Controller);

	/** アームボタンが押された時の処理。SetTrackArmed()を呼びUIを更新する。 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void ToggleArm();

	/** 外部からアーム状態が変化した時にUIを同期するために呼ぶ。 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void RefreshArmState(bool bArmed);

	/**
	 * 階層情報を設定する（親からカード生成直後に呼ぶ）。
	 * @param Level      フォルダ階層レベル（0 = ルート）
	 * @param bInParent  子を持つ親トラックか
	 * @param bCollapsed 畳み込み中か
	 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void SetHierarchy(int32 Level, bool bInParent, bool bCollapsed);

	/** 畳み込み状態だけを更新して▽/▷の表示を切り替える。 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void SetCollapsed(bool bCollapsed);

	/** （親フォルダ用）配下の子が全REC中かを設定し、親RECボタンの見た目を更新する。 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void SetParentChildrenArmed(bool bAllArmed);

	/** カード全体を薄暗く（ミュート/他トラックソロ時）するかを設定する。 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void SetDimmed(bool bDim);

	/** 展開中のメディア行に、計測済みラウドネスをキャッシュから反映する。 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void RefreshVisibleMediaLevels();

	/** 展開中ならメディア一覧を作り直す（録音後の名前反映用）。 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void RebuildMediaListIfExpanded();

	/** メディア一覧の展開/畳み込み状態を外部から直接設定する（自動更新でカードが作り直された際の復元用）。 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void SetMediaExpanded(bool bExpand);

	/**
	 * チャンネル数ぶんのメーターを構築する。各メーターの横にラベル（入力名）を表示する。
	 * @param ChannelLabels  ch0,ch1,... のラベル（入力チャンネル名）。要素数＝メーター本数。
	 * @param bIndicator     true＝信号インジケータ(〇)モード / false＝フルメーター（バー）。
	 */
	void BuildMeter(const TArray<FString>& ChannelLabels, bool bIndicator);

	/** メーター表示モード（フル/〇）を切り替えて作り直す（OwnerControllerから入力名を再取得）。 */
	void SetMeterMode(bool bIndicator);

	/** 入力メーターの表示/非表示を、アーム状態と「メーター受信」ON/OFFに応じて更新する。 */
	void RefreshMeterVisibility();

	/** トラック削除の選択モードを切り替える（ONで右端のチェックボックスを表示・選択をリセット）。 */
	void SetDeleteMode(bool bOn);

	/** 削除選択チェックの状態。 */
	bool IsSelectedForDelete() const { return bSelectedForDelete; }
	void SetSelectedForDelete(bool bSel);

	/** ロック中はミュート/ソロ/REC（および展開中メディアの再生）を無効化し、見た目も暗くする。 */
	void SetLocked(bool bLocked);

	/** 現在の表示メーター数（チャンネル数）。フル/〇 どちらのモードでも有効。 */
	int32 GetMeterChannelCount() const { return FMath::Max(ChFill.Num(), ChDot.Num()); }

	/** このカードが指すトラック（メーター取得などで使う）。 */
	UMediaTrack* GetTrackObj() const { return TrackObj; }

	/**
	 * 各チャンネルの現在ピーク(dB)をメーターへ反映する。
	 * @param CurDbPerCh  ch0,ch1,... の現在ピーク(dB)。要素数が足りない分は据え置き。
	 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void SetMeterChannels(const TArray<float>& CurDbPerCh);

	/** 入力欄の現在のテキストでトラック名を確定する（「更新」ボタンから呼ぶ）。 */
	UFUNCTION(BlueprintCallable, Category = "TrackCard")
	void CommitNameEdit();

	/** アーム状態が変化した時のイベント（C++でUI色を切り替える）。 */
	UFUNCTION(BlueprintNativeEvent, Category = "TrackCard|Events")
	void OnArmStateChanged(bool bArmed);
	virtual void OnArmStateChanged_Implementation(bool bArmed);

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

	// --- C++で構築したUIへの参照 ---
	UPROPERTY(Transient) TObjectPtr<UBorder>          RootBorder;
	UPROPERTY(Transient) TObjectPtr<class USizeBox>   IndentBox;
	UPROPERTY(Transient) TObjectPtr<UButton>          CollapseButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       CollapseGlyph;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       IndexText;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> NameInput;
	UPROPERTY(Transient) TObjectPtr<UButton>          MediaToggleButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MediaToggleGlyph;
	UPROPERTY(Transient) TObjectPtr<UButton>          MuteButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MuteButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          SoloButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SoloButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          ArmButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ArmButtonText;
	// 削除選択用チェックボックス（削除モード時のみ左端に表示）。ゴミ箱アイコンは常時表示し、選択で背景色が変わる。
	UPROPERTY(Transient) TObjectPtr<class USizeBox>   SelectCheckSize;
	UPROPERTY(Transient) TObjectPtr<UButton>          SelectCheckButton;
	UPROPERTY(Transient) TObjectPtr<class UImage>     SelectCheckIcon;
	// 入力（構成ch数／入力ch開始）をカード上で直接選ぶプルダウン。選択即時にReaperへ反映する。
	UPROPERTY(Transient) TObjectPtr<UComboBoxString>  InputFormatCombo;  // 構成（モノ/ステレオ/5.1）
	UPROPERTY(Transient) TObjectPtr<UComboBoxString>  InputChCombo;      // 入力ch（開始）
	UPROPERTY(Transient) TObjectPtr<UButton>          VolMinusButton;
	UPROPERTY(Transient) TObjectPtr<UButton>          VolPlusButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       VolDbText;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox>     ExpandPanel;       // ▶展開で出る領域（音量調整＋メディア一覧）
	UPROPERTY(Transient) TObjectPtr<UVerticalBox>     MediaListBox;
	// 入力レベルメーター（アーム中のみ表示）。チャンネル数ぶんのメーターを縦に並べる。
	// 各チャンネル＝[Canvas（背景／バー／最大値線／クリップ）] ＋ [数値dB]。
	UPROPERTY(Transient) TObjectPtr<class UVerticalBox>        MeterBox;   // メーター全体の入れ物（表示/非表示の単位）
	UPROPERTY(Transient) TArray<TObjectPtr<UBorder>>          ChFill;     // 各chの現在レベルのバー
	UPROPERTY(Transient) TArray<TObjectPtr<UBorder>>          ChHold;     // 各chの最大値の線
	UPROPERTY(Transient) TArray<TObjectPtr<UBorder>>          ChClip;     // 各chのクリップ表示
	UPROPERTY(Transient) TArray<TObjectPtr<UTextBlock>>       ChDbText;   // 各chの数値(dB)
	UPROPERTY(Transient) TArray<TObjectPtr<UBorder>>          ChDot;      // 各chの信号〇（インジケータモード）
	// メーター表示モード（true＝信号〇 / false＝フルメーター）
	bool bIndicatorMode = false;

private:

	void BuildUI();
	void RefreshVisuals();
	void RefreshHierarchyVisuals();
	void RefreshSoloVisual();
	void RefreshMuteVisual();
	// ロック中のM/S/Rボタンの不透明度を、押せないことを示しつつON/OFFが分かるように更新する
	// （ON=濃く／OFF=薄く。Reaper側の操作で状態が変わった場合もここで反映される）。
	void RefreshLockedButtonOpacity();
	void RefreshVolumeText();
	void BuildMediaList();
	void ApplyName();

	UFUNCTION()
	void HandleArmClicked();

	UFUNCTION()
	void HandleSoloClicked();

	UFUNCTION()
	void HandleMuteClicked();

	UFUNCTION()
	void HandleVolMinusClicked();

	UFUNCTION()
	void HandleVolPlusClicked();

	UFUNCTION()
	void HandleMediaToggleClicked();

	UFUNCTION()
	void HandleCollapseClicked();

	// 入力プルダウン（構成／入力ch）の選択が変わったら、その場でReaperへ反映する
	UFUNCTION()
	void HandleInputSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	// 入力プルダウンの選択肢と現在値を、現在のトラック設定から作り直す
	void RefreshInputControls();

	// 入力プルダウンのドロップダウン項目を明るい文字色で生成する
	UFUNCTION()
	UWidget* MakeInputComboItem(FString Item);

	// RefreshInputControls中の programmatic な選択で反映が走らないようにするガード
	bool bInitializingInput = false;

	UFUNCTION()
	void HandleNameCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void HandleSelectCheckClicked();

	// 削除選択チェックの見た目（✓/空・色）を更新する
	void RefreshSelectCheckVisual();

	// 削除選択中か
	bool bSelectedForDelete = false;

	// ロック中か（true＝ミュート/ソロ/REC操作を無効化）
	bool bIsLocked = false;

	// 音量操作用のトラックオブジェクト（FTrackDetail.Track）
	UPROPERTY(Transient) TObjectPtr<UMediaTrack> TrackObj;

	// 現在のトラック音量
	double VolumeDb = 0.0;

	// 各chのメーター状態（0..1、スムージング済みレベル／最大値線／クリップラッチ）
	TArray<float> ChLevel;
	TArray<float> ChHoldLevel;
	TArray<bool>  ChClipped;

	// 各chのメーター表示を初期状態（無音）に戻す
	void ResetMeters();
};
