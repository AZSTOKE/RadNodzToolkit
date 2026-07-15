// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"   // ESelectInfo
#include "ReaperMicSetting.h"
#include "WG_MicSettingsDialog.generated.h"

class UWG_RadNodzToolkit;
class UEditableTextBox;
class UTextBlock;
class UButton;
class UVerticalBox;
class UBorder;
class UComboBoxString;

/**
 * 「設定」で開くマイク登録ダイアログ。
 * ＋／－でマイク名の入力欄を増減し、入力したマイク名を端末に保存する。
 * ここで登録したマイクが、トラック追加時の生成本数になる。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_MicSettingsDialog : public UUserWidget
{
	GENERATED_BODY()

public:

	/** 親コントローラーを渡して初期化する（既存の登録マイクを読み込む）。 */
	void Setup(UWG_RadNodzToolkit* InOwner);

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:

	void BuildUI();
	void RebuildMicList();

	UFUNCTION() void HandlePlus();
	UFUNCTION() void HandleMinus();
	UFUNCTION() void HandleSave();
	UFUNCTION() void HandleCancel();
	// 入力chを上から順番に自動割り当てする（各マイクの構成ch数ぶんずつ詰めて並べる）
	UFUNCTION() void HandleAutoAssignInputCh();

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;

	UPROPERTY(Transient) TObjectPtr<UTextBlock>   CountText;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> MicListBox;
	UPROPERTY(Transient) TObjectPtr<UButton>      MinusButton;
	UPROPERTY(Transient) TObjectPtr<UButton>      PlusButton;
	UPROPERTY(Transient) TObjectPtr<UButton>      SaveButton;
	UPROPERTY(Transient) TObjectPtr<UButton>      CancelButton;
	UPROPERTY(Transient) TObjectPtr<UButton>      AutoAssignButton;   // 入力ch自動割り当て

	UPROPERTY(Transient) TArray<TObjectPtr<UEditableTextBox>> MicInputs;
	// 各マイク行のch数(モノ/ステレオ/5.1)・入力ch(開始)の選択
	UPROPERTY(Transient) TArray<TObjectPtr<UComboBoxString>> MicChCountCombos;
	UPROPERTY(Transient) TArray<TObjectPtr<UComboBoxString>> MicInputChCombos;
	// 各行「一覧から選択」プルダウン（マイクシート由来の名前を選ぶと上のマイク名欄へ流し込む）
	UPROPERTY(Transient) TArray<TObjectPtr<UComboBoxString>> MicCatalogCombos;
	// 各カタログコンボの前回選択（どの行が変わったかの特定用）
	TArray<FString> MicCatalogLastSel;

	// Setupで取得したマイク名カタログ（リストの「マイク」シート由来。未読込なら空）
	TArray<FString> CatalogNames;
	TArray<int32>   CatalogChannels;

	// カタログのプルダウンで選ばれたら、その行のマイク名欄・構成へ反映する
	UFUNCTION() void HandleCatalogPick(FString SelectedItem, ESelectInfo::Type SelectionType);

	// Setup で読み込んだ既存マイク設定（UI構築後に各行へ反映する）
	UPROPERTY(Transient) TArray<FMicSetting> InitialMics;

	// Reaperの入力チャンネル名（接続時のみ。空なら番号で代替する）
	TArray<FString> InputChannelNames;

	// ch数コンボの index ⇔ 実ch数 の変換
	static int32 NumChannelsToIndex(int32 NumCh);
	static int32 IndexToNumChannels(int32 Index);
	// 入力chコンボの選択肢を埋める（接続時=名前 / 未接続=番号）
	void FillInputChannelOptions(UComboBoxString* Combo, int32 SelectIndex);
	// コンボ項目を明るい文字色で生成する（暗い背景で読めるように）
	UFUNCTION() UWidget* MakeComboItem(FString Item);

	int32 MicCount = 1;
	bool bUICreated = false;
	bool bSeeded = false;
};
