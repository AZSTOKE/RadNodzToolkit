// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"   // ETextCommit::Type
#include "ReaperMonitorOutputMode.h"
#include "WG_ReaStreamMonitor.generated.h"

class UReaStreamReceiver;
class UButton;
class UTextBlock;
class UEditableTextBox;

/**
 * ReaStream 音声「受信・再生」専用のウィジェット（Windows向け）。
 *
 * Reaperコントローラー本体（接続/録音/トラック操作）から音声受信機能だけを切り出したもの。
 * 外部公開時に「制御は出さず音声だけ通す」用途を想定し、ここには制御系の機能を一切持たせない。
 *
 * 既存の UReaStreamReceiver をそのまま再利用し、出力方法を
 *   そのまま(サラウンド) / 立体音響(バイノーラル) / ステレオ
 * の3モードで切り替えられる。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_ReaStreamMonitor : public UUserWidget
{
	GENERATED_BODY()

public:

	UWG_ReaStreamMonitor(const FObjectInitializer& ObjectInitializer);

	/** 受信ポート（ReaStream既定: 58710）。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReaStream")
	int32 Port = 58710;

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:

	void BuildUI();

	// 受信機に現在の設定（出力モード・バッファ・立体音響パラメータ）を反映する（Start前に呼ぶ）
	void ApplyReceiverConfig();
	// 受信中なら新しい設定で再起動して即時反映する
	void RestartIfActive();

	void UpdateModeButton();
	void UpdateBufferButton();
	void UpdateStartButton();
	void SetStatus(const FString& Message, const FLinearColor& Color);

	UFUNCTION() void HandleStartStopClicked();
	UFUNCTION() void HandleModeClicked();
	UFUNCTION() void HandleBufferClicked();
	// 許可する送信元IP欄が確定されたとき：受信中なら即時にフィルタを反映する
	UFUNCTION() void HandleAllowedIPCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UPROPERTY(Transient) TObjectPtr<UReaStreamReceiver> Receiver;

	UPROPERTY(Transient) TObjectPtr<UTextBlock>       TitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       StatusText;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> PortInput;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> AllowedIPInput;
	UPROPERTY(Transient) TObjectPtr<UButton>          ModeButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ModeButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          BufferButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       BufferButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          StartStopButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       StartStopButtonText;

	/** 出力方法（既定はサラウンドそのまま）。 */
	EMonitorOutputMode OutputMode = EMonitorOutputMode::Passthrough;

	/** 受信バッファ（遅延）レベル（0=低遅延/1=標準/2=安定）。 */
	int32 BufferLevel = 1;

	bool bUICreated = false;
};
