// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_IRMeasurementDialog.generated.h"

class UTextBlock;
class UButton;
class UComboBoxString;
class USlider;
class UProgressBar;
class UWidget;

/**
 * 「IR計測準備」の入力ダイアログ。
 * 計測回数・チャンネル数・TSPの種類（対数／線形）・再生の間隔を入力させ、OK／キャンセルを返す。
 *
 * 数値は手入力させず、＋／－ボタンとスライダーだけで決める（端末のソフトキーボードを
 * 出さずに済み、範囲外の値も入らない）。
 *
 * 使い方:
 *   auto* Dlg = CreateWidget<UWG_IRMeasurementDialog>(Owner, UWG_IRMeasurementDialog::StaticClass());
 *   Dlg->Setup(OnOk, OnCancel);
 *   Dlg->AddToViewport(100);
 */
UCLASS()
class RADNODZTOOLKIT_API UWG_IRMeasurementDialog : public UUserWidget
{
	GENERATED_BODY()

public:

	/** OK時: (計測回数, チャンネル数, 対数TSPか, 再生の間隔[秒]) / キャンセル時: 引数なし。 */
	using FOnOk = TFunction<void(int32 RepeatCount, int32 NumChannels, bool bLogTSP, double IntervalSec)>;
	using FOnCancel = TFunction<void()>;

	void Setup(FOnOk InOnOk, FOnCancel InOnCancel);

	// 入力の範囲（極端な値で REAPER に大量のトラックや長大な信号を作らせないため）
	static constexpr int32 MinRepeatCount = 1;
	static constexpr int32 MaxRepeatCount = 100;
	static constexpr int32 MinNumChannels = 1;
	static constexpr int32 MaxNumChannels = 64;

	// 再生の間隔（TSP の掃引と掃引の間に入れる無音の秒数）。1秒刻みのスライダーで指定する。
	// 0 秒にすると残響が次の掃引に被って IR を算出できないため、下限は 1 秒。
	static constexpr int32 MinIntervalSec     = 1;
	static constexpr int32 MaxIntervalSec     = 15;
	static constexpr int32 DefaultIntervalSec = 5;

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:

	void BuildUI();

	/**
	 * 「見出し　－　現在値　＋」の1行を本体へ積む。
	 * 値そのものの保持は呼び出し側（RepeatCount / NumChannels）が行う。
	 */
	UWidget* MakeStepperRow(const FString& Label, FName MinusName, FName PlusName,
		TObjectPtr<UButton>& OutMinus, TObjectPtr<UButton>& OutPlus, TObjectPtr<UTextBlock>& OutValue);

	// 現在値を表示へ反映し、下限／上限に達したボタンを無効化する
	void UpdateCountUI();

	// スライダーの現在値を「5 秒」の表示・つまみ位置・塗りに反映する
	void UpdateIntervalUI();

	UFUNCTION() void HandleOk();
	UFUNCTION() void HandleCancel();
	UFUNCTION() void HandleRepeatMinus();
	UFUNCTION() void HandleRepeatPlus();
	UFUNCTION() void HandleChannelsMinus();
	UFUNCTION() void HandleChannelsPlus();
	UFUNCTION() void HandleIntervalChanged(float NewValue);
	UFUNCTION() UWidget* MakeComboItem(FString Item);

	FOnOk OnOk;
	FOnCancel OnCancel;

	// OnGenerateWidgetEvent で生成した項目の参照保持（保持しないとGCで回収されクラッシュする）
	UPROPERTY(Transient) TArray<TObjectPtr<UWidget>> ComboGeneratedItems;

	UPROPERTY(Transient) TObjectPtr<UTextBlock>       RepeatCountValueText;
	UPROPERTY(Transient) TObjectPtr<UButton>          RepeatCountMinusButton;
	UPROPERTY(Transient) TObjectPtr<UButton>          RepeatCountPlusButton;

	UPROPERTY(Transient) TObjectPtr<UTextBlock>       NumChannelsValueText;
	UPROPERTY(Transient) TObjectPtr<UButton>          NumChannelsMinusButton;
	UPROPERTY(Transient) TObjectPtr<UButton>          NumChannelsPlusButton;

	UPROPERTY(Transient) TObjectPtr<UComboBoxString>  TspTypeCombo;      // 0=対数 / 1=線形
	UPROPERTY(Transient) TObjectPtr<USlider>          IntervalSlider;    // 再生の間隔（1〜15秒・1秒刻み）
	UPROPERTY(Transient) TObjectPtr<UProgressBar>     IntervalFill;      // スライダーの塗り
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       IntervalValueText; // 「5 秒」の表示
	UPROPERTY(Transient) TObjectPtr<UButton>          OkButton;
	UPROPERTY(Transient) TObjectPtr<UButton>          CancelButton;

	int32 RepeatCount     = MinRepeatCount;
	int32 NumChannels     = MinNumChannels;
	int32 IntervalSeconds = DefaultIntervalSec;

	bool bUICreated = false;
};
