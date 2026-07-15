// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ReaperMicInputMode.h"
#include "ReaperMonitorOutputMode.h"
#include "ReaperMicSetting.h"
#include "ReaperSaveGame.generated.h"

/**
 * サーバー認証設定の1件（ユーザー命名の serverId と、その期待フィンガープリント）。
 * serverId はピン留めキー、Fingerprint は "SHA256:<base64>"。
 */
USTRUCT()
struct FServerAuthEntry
{
	GENERATED_BODY()

	UPROPERTY()
	FString ServerId;

	UPROPERTY()
	FString Fingerprint;

	/** この serverId に紐づく Reaper IP。ドロップダウン選択時に接続先へ復元する（未設定＝空）。 */
	UPROPERTY()
	FString IPAddress;

	/** この serverId に紐づく Reaper ポート。 */
	UPROPERTY()
	int32 Port = 12345;
};

/**
 * 接続設定（IP/ポート）を端末に保存するためのセーブデータ。
 */
UCLASS()
class RADNODZTOOLKIT_API UReaperSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString IPAddress = TEXT("192.168.1.1");

	UPROPERTY()
	int32 Port = 12345;

	/** 登録済みのマイク名一覧（旧フォーマット。互換読み込み用。新規は Mics を使う）。 */
	UPROPERTY()
	TArray<FString> MicNames;

	/** 登録済みマイク設定一覧（名前＋入力チャンネル設定）。トラック生成時に各トラックへ適用する。 */
	UPROPERTY()
	TArray<FMicSetting> Mics;

	/** マイク送信に使う入力経路（本体マイク / Bluetoothマイク）。 */
	UPROPERTY()
	EMicInputMode MicInputMode = EMicInputMode::Builtin;

	/** モニター受信音声の出力方法（ステレオ / 立体音響 / そのまま）。 */
	UPROPERTY()
	EMonitorOutputMode MonitorOutputMode = EMonitorOutputMode::Stereo;

	/** ヘッダーに端末名/IPを表示するか。 */
	UPROPERTY()
	bool bShowDeviceNameIP = true;

	/** 表示言語（ReaperLang::ELanguageの値。0=日本語）。 */
	UPROPERTY()
	uint8 Language = 0;

	/** 立体音響：ITDの強さレベル（0=オフ/1=弱/2=中/3=強）。 */
	UPROPERTY()
	int32 SpatialItdLevel = 2;

	/** 立体音響：頭部減衰の強さレベル（0=オフ/1=弱/2=中/3=強）。 */
	UPROPERTY()
	int32 SpatialShadowLevel = 2;

	/** 立体音響：ch順マッピング。 */
	UPROPERTY()
	EBinauralChannelOrder SpatialChannelOrder = EBinauralChannelOrder::ReaperITU;

	/** 立体音響：選択中HRTFプロファイル名（空＝内蔵の簡易バイノーラル）。 */
	UPROPERTY()
	FString HrtfProfile;

	/** 立体音響：カスタム配置の各ch方位角（度）。Customマッピング時に使用。 */
	UPROPERTY()
	TArray<float> SpatialCustomAzimuths;

	/** 立体音響：カスタム配置の各ch距離（0.2〜2.0）。Customマッピング時に使用。 */
	UPROPERTY()
	TArray<float> SpatialCustomDistances;

	/** 受信バッファ（遅延）レベル（0=低遅延/1=標準/2=安定）。 */
	UPROPERTY()
	int32 MonitorBufferLevel = 1;

	/**
	 * 旧フォーマットのステレオダウンミックス係数（個別：センター/サラウンド/リア/LFE）。
	 * 新フォーマット（DmGains）へ移行するため、読み込み時の互換用として残している。
	 */
	UPROPERTY()
	float DmCenter = 0.7071f;
	UPROPERTY()
	float DmSurround = 0.7071f;
	UPROPERTY()
	float DmRear = 0.7071f;
	UPROPERTY()
	float DmLfe = 0.0f;

	/** ダウンミックス係数（受信ch・標準レイアウト L R C LFE Ls Rs Lb Rb ごとのL/Rへ混ぜる量）。 */
	UPROPERTY()
	TArray<float> DmGains;

	/** Slack共有に使うBotトークン（xoxb-...）。 */
	UPROPERTY()
	FString SlackToken;

	/** Slack共有先（DM宛先ユーザー名 または ユーザーID）。アプリのメッセージ(DM)に届く。 */
	UPROPERTY()
	FString SlackToUser;

	/** 音量調整（ノーマライズ）の目標ラウドネス（LUFS）。 */
	UPROPERTY()
	float TargetLUFS = -7.f;

	/** 計測調整でピークの超過（赤クリップ）を防ぐか。 */
	UPROPERTY()
	bool bLimitPeakOnNormalize = true;

	/** メーターの更新間隔（ミリ秒）。大きいほど軽い。 */
	UPROPERTY()
	int32 MeterIntervalMs = 200;

	/** メーター表示を「信号インジケータ（〇）」にするか（false＝フルメーター）。 */
	UPROPERTY()
	bool bMeterSignalIndicator = false;

	/** 信号〇を「トラックに1つ」にするか（true＝1つ／false＝chごと）。1つにすると取得も軽い。 */
	UPROPERTY()
	bool bMeterDotPerTrack = false;

	/** トラック一覧を自動更新するか（false＝「更新」ボタンでの手動更新）。 */
	UPROPERTY()
	bool bTrackListAutoRefresh = false;

	/** トラック一覧の自動更新間隔（秒）。 */
	UPROPERTY()
	int32 TrackRefreshIntervalSec = 10;

	/** UIテーマ（背景の明るさ）。0=標準（黒） / 1=さらに暗め / 2=明るめ。 */
	UPROPERTY()
	int32 UIThemeIndex = 0;

	/** 画面の向き固定。0=自動 / 1=縦 / 2=横。 */
	UPROPERTY()
	int32 ScreenOrientationLock = 0;

	// --- リスト（収録リスト）設定 ---
	UPROPERTY() int32   ListFormat = 0;        // 0=Excel / 1=CSV / 2=Google Sheets
	UPROPERTY() FString ListFilePath;          // 読み込むファイル（サーバ側パス）
	UPROPERTY() FString ListSheetName;         // 読み込むシート（空＝全シート）
	UPROPERTY() int32   ListStartRow = 1;      // 読み込み開始行
	UPROPERTY() int32   ListStartCol = 1;      // 読み込み開始列
	UPROPERTY() int32   ListColCount = 2;      // 読み込む列数
	UPROPERTY() bool    bListUseCheckbox = true;   // チェックボックス要否
	UPROPERTY() FString ListExportPath;        // 書き出し先（サーバ側パス）
	UPROPERTY() FString GoogleSheetSpreadsheetId;  // Google SheetsのスプレッドシートID
	UPROPERTY() FString GoogleSheetClientId;       // Google SheetsのクライアントID
	UPROPERTY() int32   ScriptMatchScope = 0;        // 台本一致の対象範囲：0=シート/1=全シート
	UPROPERTY() int32   ScriptMatchCheckFilter = 0;  // 台本一致の対象行フィルタ：0=見ない/1=チェック済みのみ/2=未チェックのみ

	// --- サーバー認証（mTLS）---
	/** 接続に TLS(mTLS) を使用するか。 */
	UPROPERTY() bool bUseTls = false;

	/** 登録済みサーバーの認証設定一覧（serverId → 期待フィンガープリント）。 */
	UPROPERTY() TArray<FServerAuthEntry> ServerAuthEntries;

	// --- メンバー通話（在席名簿）---
	UPROPERTY() FString IntercomUserName;      // 自分の表示名
	UPROPERTY() FString PresenceFolder;        // 在席名簿フォルダ（手動指定・任意）
	UPROPERTY() FString ResolvedPresenceFolder; // 接続時に解決した AZData の絶対パス（次回起動時の既定に使う）
};
