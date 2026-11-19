// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzCommonClass.h"
#include "TcpClient.h"
#include "Engine/DataAsset.h"
#include "RadnodzClient.generated.h"

USTRUCT(BlueprintType)
struct RADNODZCLIENTPLUGIN_API FRigdocksError
{
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RADNODZ|Error")
	int ErrorCode{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RADNODZ|Error")
	FString Message;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RADNODZ|Error")
	FString Details;

};

// 認可済みサーバ(ピン留め)1件。
USTRUCT(BlueprintType)
struct RADNODZCLIENTPLUGIN_API FRadnodzPinnedServer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "RADNODZ|Security")
	FString ServerId;

	UPROPERTY(BlueprintReadOnly, Category = "RADNODZ|Security")
	FString Fingerprint;

	UPROPERTY(BlueprintReadOnly, Category = "RADNODZ|Security")
	FString PublicKeyPem;
};

// 接続用 URL(QR コード)から読み取ったサーバの情報。
//
// URL は公開情報しか含まないが改ざん検知は無いため、適用する前に
// Fingerprint をサーバ画面と目視で突き合わせてもらう前提。
USTRUCT(BlueprintType)
struct RADNODZCLIENTPLUGIN_API FRadnodzPairingInfo
{
	GENERATED_BODY()

	/** ピン留めキーとなる論理サーバ ID。 */
	UPROPERTY(BlueprintReadOnly, Category = "RADNODZ|Security")
	FString NodeId;

	UPROPERTY(BlueprintReadOnly, Category = "RADNODZ|Security")
	FString Host;

	UPROPERTY(BlueprintReadOnly, Category = "RADNODZ|Security")
	int32 Port = 0;

	/** サーバのフィンガープリント "SHA256:<base64>"。 */
	UPROPERTY(BlueprintReadOnly, Category = "RADNODZ|Security")
	FString Fingerprint;
};

// 公開鍵の申請(Enroll)の結果。
UENUM(BlueprintType)
enum class ERadnodzEnrollStatus : uint8
{
	/** サーバに届かなかった / TLS を張れなかった。 */
	Failed,

	/** 申請済み(既に申請済みの場合も含む)。サーバ管理者の承認待ち。 */
	Pending,

	/** 既に登録済み。通常の接続がそのまま通る。 */
	Approved,

	/** サーバが今は申請を受け付けていない。 */
	Closed,

	/** 却下された(所持証明の不一致、プール上限など)。 */
	Rejected,
};

UCLASS(BlueprintType)
class RADNODZCLIENTPLUGIN_API URadnodzClient : public URadnodzCommonClass
{
public:

	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Client")
	static URadnodzClient* CreateRadnodzClient();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Client")
	bool Connect(const FString& ip, int port);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Client")
	FString Send(const FString& message);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Client")
	void Disconnect();

	UFUNCTION(BlueprintPure, Category = "RADNODZ|Client")
	bool IsConnected() const;
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Client")
	FRigdocksError GetError();
	/** 直近の Send() でクライアントがコマンドを送信した時刻。 */
	UFUNCTION(BlueprintPure, Category = "RADNODZ|Client")
	int64 GetLastSendTime() const;

	/** 直近の Send() でサーバが処理を受け取った時刻(サーバから受領)。 */
	UFUNCTION(BlueprintPure, Category = "RADNODZ|Client")
	int64 GetLastServerRecvTime() const;

	/** 直近の Send() でサーバが処理を返した時刻(サーバから受領)。 */
	UFUNCTION(BlueprintPure, Category = "RADNODZ|Client")
	int64 GetLastServerReturnTime() const;

	/** 直近の Send() でクライアントが結果を受け取り終えた時刻。 */
	UFUNCTION(BlueprintPure, Category = "RADNODZ|Client")
	int64 GetLastRecvTime() const;

	UFUNCTION(BlueprintPure, Category = "RADNODZ|Client")
	static int64 GetNowUnixMicros();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Client")
	bool SendTimingProbe();
	RADNODZ_client::TcpClient& GetClient();

	// ===== 宛先(サーバモジュール) =====
	// ホストは外部公開の TCP を 1 本だけ持ち、受けた接続をこの ID の
	// サーバモジュールへ振り分ける。既定はこのプラグインのビルド時の
	// 製品 ID(RADNODZ)なので、通常は触らなくてよい。
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Client")
	void SetTargetProductId(const FString& ProductId);

	UFUNCTION(BlueprintPure, Category = "RADNODZ|Client")
	FString GetTargetProductId() const;

	// ===== セキュア通信: アイデンティティ(mTLS) =====
	/** 鍵/証明書の保存先を指定する(GenerateIdentity/Connect より前に呼ぶ)。 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	void SetStorageDirectory(const FString& Dir);

	/** ECDSA P-256 鍵ペア+自己署名証明書を用意する(冪等: 無ければ生成、あればロード)。 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	bool GenerateIdentity();

	/** 明示的なローテーション(常に新規生成)。既存の全ペアリングは無効化される。 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	bool RegenerateIdentity();

	/** ペアリング用: 自身の公開鍵 PEM(サーバの authorized に登録する)。 */
	UFUNCTION(BlueprintPure, Category = "RADNODZ|Security")
	FString GetOwnPublicKeyPem();

	/** 自身のフィンガープリント "SHA256:<base64>"。 */
	UFUNCTION(BlueprintPure, Category = "RADNODZ|Security")
	FString GetOwnFingerprint();

	// ===== セキュア通信: サーバ検証(ポリシー方式) =====
	/** ピン留めキーとなる論理サーバ ID(LAN で IP が変わっても同一サーバと識別)。Connect より前に。 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	void SetServerId(const FString& Id);

	/** 期待するサーバのフィンガープリント。設定するとその値に一致した場合のみ接続を許可(ピン留め)する。 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	void SetExpectedServerFingerprint(const FString& Fingerprint);

	/** 初回接続時(未ピンのサーバ)を無条件に信頼してピン留めするか(TOFU)。既定 false。 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	void SetTrustOnFirstUse(bool bEnable);

	// ===== セキュア通信: ピン管理 =====
	/** ピン留め済みサーバの一覧。 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	TArray<FRadnodzPinnedServer> ListPinnedServers();

	/** 指定した論理サーバ ID のピンを削除する(信頼リセット)。 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	bool RemovePinnedServer(const FString& ServerId);

	// ===== ペアリング(QR コード / URL) =====
	/**
	 * 接続用 URL を解析するだけ(接続先の設定もピン留めもしない)。
	 *
	 * QR を読んだ直後にこれで中身を取り出し、Fingerprint を利用者へ見せて
	 * サーバ画面と突き合わせてもらう。確認が取れてから
	 * ApplyServerPairingUrl を呼ぶ、という順序を想定している。
	 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	static bool ParsePairingUrl(const FString& Url, FRadnodzPairingInfo& OutInfo);

	/**
	 * 画像ファイル(PNG / JPEG / BMP)から QR コードを読む。
	 *
	 * 任意角度の回転・傾き・照明ムラ・大きな画像の中に置かれたコードにも
	 * 対応する。
	 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	static bool DecodeQrImageFile(const FString& FilePath, FString& OutText, FString& OutError);

	/**
	 * 展開済みの 8bit グレースケールから QR コードを読む
	 * (カメラ映像など、画像ファイルを経由しない場合用)。
	 */
	static bool DecodeQrGrayscale(const uint8* Gray, int32 Width, int32 Height, int32 Stride,
		FString& OutText, FString& OutError);

	/**
	 * 接続用 URL を適用する(接続先の設定とピン留めを一度に行う)。
	 *
	 * 適用した時点で FP を持っているので、初回接続に TOFU の隙間ができない。
	 * ピンをファイルへ書くため、SetStorageDirectory を先に呼んでおくこと。
	 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	bool ApplyServerPairingUrl(const FString& Url);

	/**
	 * 接続する前にサーバをピン留めする(FP を URL 以外の手段で受け取った場合)。
	 * PublicKeyPem は省略可。
	 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	bool PinServer(const FString& ServerId, const FString& Fingerprint, const FString& PublicKeyPem);

	/**
	 * URL で設定した接続先へ接続する(IP / ポートを引数で渡さない Connect)。
	 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Client")
	bool ConnectToPairedServer();

	/**
	 * 自身の公開鍵をサーバへ申請する。承認はサーバの管理者が行う。
	 *
	 * TLS が要るので、GenerateIdentity 済みかつサーバがピン留め / 検証済みで
	 * あること。ClientName は管理者へ見せる表示名で、検証はされない。
	 */
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Security")
	ERadnodzEnrollStatus Enroll(const FString& ClientName);

private:
	// ポリシーに基づく検証コールバックを client に設定する。
	void EnsureVerifier();

	RADNODZ_client::TcpClient client;

	// サーバ検証ポリシー
	FString ExpectedServerFingerprint;
	bool bTrustOnFirstUse = false;
};