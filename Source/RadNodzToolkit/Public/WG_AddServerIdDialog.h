// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"   // ETextCommit::Type
#include "WG_AddServerIdDialog.generated.h"

class UWG_RadNodzToolkit;
class UEditableTextBox;
class UButton;
class UTextBlock;

/**
 * 新しいサーバーを登録するためのモーダルダイアログ（設定 > サーバー > 追加）。
 *
 * 登録には Bridge（サーバー側の管理画面）が配る接続用URLを使う。
 *   rigdocks://server?v=1&id=<nodeId>&h=<host>&p=<port>&fp=<fingerprint>
 * URL には接続先とフィンガープリントが入っているので、IP・ポート・FPの手入力が要らず、
 * 初回接続から TOFU（初回無条件信頼）にならない。
 *
 * URLの入れ方は2通り：
 *   ・「QRコード撮影」でカメラを起動し、Bridge が表示するQRを読む（Androidのみ）
 *   ・URL欄へ直接貼り付ける
 *
 * 「登録」で サーバー名 と URL を Owner->AddServerFromPairingUrl() に渡す。
 * 失敗（名前の重複・URL不正など）のときはダイアログを閉じず、理由をその場に出す。
 * UI はすべて C++ で構築するため、対応する WBP アセットは不要。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_AddServerIdDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 親ウィジェットを渡して初期化する。 */
	void Setup(UWG_RadNodzToolkit* InOwner);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeDestruct() override;

private:
	void BuildUI();

	UFUNCTION() void HandleRegister();
	UFUNCTION() void HandleCancel();
	UFUNCTION() void HandleScanQrClicked();
	UFUNCTION() void HandleInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	void Commit();

	/** カメラ権限の取得待ち／読み取り待ちを一定間隔で進める（タイマーから呼ばれる）。 */
	void PollScan();

	/** スキャンを開始する（権限は取得済みの前提）。 */
	void BeginScan();

	/** スキャンを終了してタイマーを止める。 */
	void EndScan();

	/** ダイアログ内のメッセージ行を更新する（bError で赤く出す）。 */
	void SetMessage(const FString& Msg, bool bError = false);

	UPROPERTY(Transient) TObjectPtr<UWG_RadNodzToolkit> Owner;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox>   NameInput;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox>   UrlInput;
	UPROPERTY(Transient) TObjectPtr<UButton>            ScanQrButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>         MessageText;
	UPROPERTY(Transient) TObjectPtr<UButton>            RegisterButton;
	UPROPERTY(Transient) TObjectPtr<UButton>            CancelButton;

	bool bUICreated = false;

	/** カメラ権限の許可待ち中か（許可されたらスキャンを始める）。 */
	bool bWaitingCameraPermission = false;

	/** 権限待ちの経過秒（いつまでも待たずに諦める）。 */
	float PermissionWaitSeconds = 0.f;

	/** 開始直後に「停止中」と誤判定しないための猶予秒（Java側の起動はUIスレッド経由で遅れる）。 */
	float ScanGraceSeconds = 0.f;

	/** スキャン進行用のポーリングタイマー。 */
	FTimerHandle ScanPollTimer;
};
