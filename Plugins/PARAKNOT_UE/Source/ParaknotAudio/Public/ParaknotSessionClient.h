// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ParaknotSessionClient.generated.h"

class URadnodzClient;

/** PARAKNOT インスタンス 1 件（AZ_PARAKNOT_GetInstanceList の 1 要素）。 */
USTRUCT(BlueprintType)
struct PARAKNOTAUDIO_API FParaknotInstanceInfo
{
	GENERATED_BODY()

	/** 外部制御で対象を指す 8 桁の ID。 */
	UPROPERTY(BlueprintReadOnly, Category = "PARAKNOT")
	FString InstanceId;

	/** 画面に出す名前（任意）。 */
	UPROPERTY(BlueprintReadOnly, Category = "PARAKNOT")
	FString Label;

	/** 0 = 送信 / 1 = 受信。**配信は 0。** */
	UPROPERTY(BlueprintReadOnly, Category = "PARAKNOT")
	int32 Role = 0;

	/**
	 * 待ち受けているポート。0 = まだ開けていない（未認証 / 音声が始まっていない）。
	 *
	 * **Label はマシン単位で全インスタンス同じ**なので、一覧で見分けるには
	 * 実質これと ch 数しか手がかりが無い。
	 */
	UPROPERTY(BlueprintReadOnly, Category = "PARAKNOT")
	int32 ListenPort = 0;

	/** メインチャンネル数。 */
	UPROPERTY(BlueprintReadOnly, Category = "PARAKNOT")
	int32 MainChannelCount = 0;

	/** 配信（送信）になっているか。 */
	bool IsBroadcasting() const { return Role == 0; }

	/** 一覧に出す表示名。例: 「8420 / 2ch (6dbff390)」 */
	FString GetDisplayName() const
	{
		return FString::Printf(TEXT("%d / %dch (%s)"), ListenPort, MainChannelCount, *InstanceId);
	}
};

/** 購読に必要な接続先一式。 */
USTRUCT(BlueprintType)
struct PARAKNOTAUDIO_API FParaknotSessionInfo
{
	GENERATED_BODY()

	/** 配信側の IP。REAPER が動いている PC。 */
	UPROPERTY(BlueprintReadOnly, Category = "PARAKNOT")
	FString Host;

	/** 配信側の待受ポート。 */
	UPROPERTY(BlueprintReadOnly, Category = "PARAKNOT")
	int32 Port = 0;

	UPROPERTY(BlueprintReadOnly, Category = "PARAKNOT")
	FString SessionId;

	/** AEAD のセッション鍵（Base64 の 32byte）。**保存しないこと**（Transient でシリアライズ対象から外してある）。 */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "PARAKNOT")
	FString SessionKeyBase64;

	bool IsValid() const { return !Host.IsEmpty() && Port > 0 && !SessionKeyBase64.IsEmpty(); }
};

/**
 * PARAKNOT の配信に繋ぐための「接続先の解決」。
 *
 * **RADNODZ の業務 TCP をそのまま使う。** PARAKNOT の関数も RADNODZ サーバー経由で
 * 呼べるので、PARAKNOT 専用の接続も宛先指定も要らない。既に繋がっている
 * URadnodzClient をそのまま渡せばよい。
 *
 * ここで取れた host / port / 鍵を UParaknotAudioReceiver::Start() へ渡すと
 * 音が流れ始める。**音声はこの TCP を通らない**（AudioLink の UDP で直接）。
 */
UCLASS()
class PARAKNOTAUDIO_API UParaknotSessionClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * 相手の PC で動いている PARAKNOT の一覧を取る。
	 *
	 * @param Client   接続済みの RADNODZ クライアント
	 * @param OutList  見つかったインスタンス
	 * @param OutError 失敗理由（表示用）
	 */
	static bool ListInstances(URadnodzClient* Client, TArray<FParaknotInstanceInfo>& OutList,
		FString& OutError);

	/**
	 * 対象インスタンスを**配信**（送信 + 待受）に切り替える。
	 *
	 * 既に配信なら呼ぶ必要はない。受信側のインスタンスへ購読を申し込んでも
	 * 音は来ないので、配信でないものを選んだときに使う。
	 */
	static bool SetBroadcastMode(URadnodzClient* Client, const FString& InstanceId,
		FString& OutError);

	/**
	 * 配信中のインスタンスから購読用のセッションを受け取る。
	 *
	 * **既に聞いている相手が居れば、その鍵に相乗りする**（サーバー側の規則）。
	 * 配信は鍵を 1 本しか持てないので、購読者ごとに新しい鍵を作ると
	 * 先に聞いていた側が無音になる。
	 *
	 * @param Host 配信側の IP。こちらからは分からないので呼び出し側が渡す
	 *             （RADNODZ で繋いでいる相手の IP をそのまま使えばよい）。
	 */
	static bool OpenBroadcastSession(URadnodzClient* Client, const FString& InstanceId,
		const FString& Host, FParaknotSessionInfo& OutSession, FString& OutError);

	/**
	 * 一覧 → 対象を決める → セッションを取る、までのまとめ。
	 *
	 * @param PreferredInstanceId 聞きたいインスタンス。空なら自動で選ぶ。
	 *                            **指定があって配信していなければ配信へ切り替える**
	 *                            （名指しなので勝手に変えてよい）。
	 *                            見つからなければ自動選択へ落とす。
	 *
	 * 自動選択のときは配信中のものを採る。**配信中が 1 つも無い場合、
	 * 勝手に切り替えるのはインスタンスが 1 つだけのとき**に限る。
	 * 複数あるのに黙って先頭を配信へ変えると、聞きたくないトラックの
	 * インスタンスが配信にされてしまうため、その場合は選べと返す。
	 */
	static bool ConnectToBroadcast(URadnodzClient* Client, const FString& Host,
		const FString& PreferredInstanceId, FParaknotSessionInfo& OutSession, FString& OutError);
};
