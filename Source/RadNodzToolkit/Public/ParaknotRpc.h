// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"

class URadnodzClient;

/**
 * 相手マシンで動いている PARAKNOT インスタンス 1 件。
 * AZ_PARAKNOT_GetInstanceList の戻りから必要な項目だけを取り出したもの。
 */
struct FParaknotInstance
{
	FString InstanceId;
	FString Label;

	/** 0=送信 / 1=受信 */
	int32 Role = 0;

	/** 0=待受 / 1=接続 */
	int32 Transport = 0;

	/**
	 * 待ち受けているポート。0 = まだ開けていない（未認証 / 音声が始まっていない）。
	 *
	 * **Label はマシン単位で全インスタンス同じ**なので、一覧で見分けるには
	 * 実質これと ch 数しか手がかりが無い。
	 */
	int32 ListenPort = 0;

	/** メインチャンネル数。 */
	int32 MainChannelCount = 0;

	/** こちらから音声を押し込める状態か（受信・待受）。 */
	bool CanReceiveInput() const { return Role == 1 && Transport == 0; }

	/** こちらが音声を受け取れる状態か（送信・待受）。 */
	bool CanSendOutput() const { return Role == 0 && Transport == 0; }
};

/** セッション発行の戻り（押し込み / 購読で共通）。 */
struct FParaknotSession
{
	int32   Port = 0;
	FString SessionId;

	/** AEAD のセッション鍵（Base64 の 32byte）。**保存しないこと。** */
	FString Key;
};

/**
 * PARAKNOT の業務 TCP API（`AZ_PARAKNOT_*`）を呼ぶ薄いラッパー。
 *
 * 型付きラッパーが RADNODZ プラグインに無いため、`RpcCall` を直に組んでいる。
 *
 * **どれも同期・ブロッキング。** TCP の往復を待つので、ゲームスレッドから
 * 直接呼ばず背景スレッド（FRadnodzRpcQueue など）から呼ぶこと。
 */
namespace ParaknotRpc
{
	/** 相手マシンで動いている PARAKNOT の一覧。 */
	RADNODZTOOLKIT_API bool ListInstances(URadnodzClient* Client,
		TArray<FParaknotInstance>& OutInstances, FString& OutError);

	/**
	 * 役割と接続の向きを変える。
	 *   role 0=送信(待受) / 1=受信(接続) / 2=受信(待受) / 3=送信(接続)
	 *
	 * **相手が配信中のときに変えると、その配信は止まる。** 利用者に確認すること。
	 */
	RADNODZTOOLKIT_API bool SetRole(URadnodzClient* Client, const FString& InstanceId,
		int32 Role, int32 OutputMode, FString& OutError);

	/**
	 * 音声を**押し込む**ためのセッションを発行する（相手は受信・待受）。
	 * 既に押し込み中の相手が居ると失敗する（受け付けは 1 本だけ）。
	 */
	RADNODZTOOLKIT_API bool OpenInputSession(URadnodzClient* Client, const FString& InstanceId,
		FParaknotSession& OutSession, FString& OutError);

	/**
	 * 音声を**受け取る**ためのセッションを発行する（相手は送信・待受）。
	 * 既に聞いている相手が居れば**同じ鍵が返る**ので、複数端末で同時に聞ける。
	 */
	RADNODZTOOLKIT_API bool OpenOutputSession(URadnodzClient* Client, const FString& InstanceId,
		FParaknotSession& OutSession, FString& OutError);
}
