// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ReaperMicInputMode.h"
#include "ReaperMonitorOutputMode.h"
#include "ReaperMicSetting.h"
#include "ReaperSaveGame.generated.h"

/**
 * サーバー認証設定の1件（ユーザー命名のサーバー名と、その接続情報＋期待フィンガープリント）。
 * Fingerprint は "SHA256:<base64>"。
 *
 * 接続用URL（Bridge が QR で配る rigdocks://server?...）から登録するため、
 * 表示名（ServerId）とピン留めキー（NodeId）を分けて持つ。
 */
USTRUCT()
struct FServerAuthEntry
{
	GENERATED_BODY()

	/** ユーザーが付けたサーバー名。ドロップダウンの表示＝一覧内のキー。 */
	UPROPERTY()
	FString ServerId;

	/**
	 * 接続用URLの id（サーバー側の論理ノードID）。mTLS のピン留めキーに使う。
	 * 空＝URL登録より前に作られた旧エントリで、その場合は ServerId をキーに使う。
	 */
	UPROPERTY()
	FString NodeId;

	UPROPERTY()
	FString Fingerprint;

	/** この serverId に紐づく Reaper IP。ドロップダウン選択時に接続先へ復元する（未設定＝空）。 */
	UPROPERTY()
	FString IPAddress;

	/** この serverId に紐づく Reaper ポート。 */
	UPROPERTY()
	int32 Port = 23760;
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
	int32 Port = 23760;

	/** 登録済みのマイク名一覧（旧フォーマット。互換読み込み用。新規は Mics を使う）。 */
	UPROPERTY()
	TArray<FString> MicNames;

	/** 登録済みマイク設定一覧（名前＋入力チャンネル設定）。トラック生成時に各トラックへ適用する。 */
	UPROPERTY()
	TArray<FMicSetting> Mics;

	/** マイク送信に使う入力経路（本体マイク / Bluetoothマイク）。 */
	UPROPERTY()
	EMicInputMode MicInputMode = EMicInputMode::Builtin;

	/**
	 * マイク送信の方式。true=PARAKNOT（暗号化・押し込み） / false=ReaStream（従来）。
	 * **既定は PARAKNOT。** ReaStream は PARAKNOT が使えない現場向けの退避。
	 */
	UPROPERTY()
	bool bUseParaknotMic = true;

	/** モニター受信音声の出力方法（ステレオ / 立体音響 / そのまま）。 */
	UPROPERTY()
	EMonitorOutputMode MonitorOutputMode = EMonitorOutputMode::Stereo;

	/** REAPER の再生音をどの経路で受け取るか(PARAKNOT / ReaStream)。 */
	UPROPERTY()
	EMonitorSource MonitorSource = EMonitorSource::Paraknot;

	/**
	 * モニターで聞く配信元の既定。**待受ポートで覚える。** 0 = 毎回選ぶ。
	 *
	 * InstanceId はプラグインを挿し直すたびに変わるので保存に向かない。
	 */
	UPROPERTY()
	int32 MonitorListenPort = 0;

	/** ヘッダーに端末名/IPを表示するか。 */
	UPROPERTY()
	bool bShowDeviceNameIP = true;

	/** 画面録画：録画中に経過秒＋停止ボタンのオーバーレイを表示するか（既定=表示）。 */
	UPROPERTY()
	bool bScreenRecShowOverlay = true;

	/** 画面録画：録画にマイク音を含めるか（既定=含めない）。 */
	UPROPERTY()
	bool bScreenRecIncludeMic = false;

	/** グループトーク受信音の再生ゲイン（メイクアップゲイン倍率。既定1.0=素通し）。 */
	UPROPERTY()
	float GroupTalkPlaybackGain = 1.0f;

	/** グループトーク送信ゲイン（送る前にマイク信号へ掛ける倍率。既定3.0）。 */
	UPROPERTY()
	float GroupTalkSendGain = 1.5f;   // UIの目盛り3（実ゲインは目盛り×0.5）

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

	/** モニター受信音量（dB。端末側で受信音を増減する。既定+5dB、範囲±12dB）。 */
	UPROPERTY()
	float MonitorVolumeDb = 5.0f;

	/** マーカー設定時のオフセット秒（押した瞬間から前後何秒にマーカーを置くか。既定-1秒、範囲±10秒）。 */
	UPROPERTY()
	int32 MarkerOffsetSeconds = -1;

	/** ローカルIR：TSPの再生回数（既定1回）。 */
	UPROPERTY()
	int32 LocalIRPlayCount = 1;

	/** ローカルIR：TSPの再生間隔（秒。既定5秒、範囲0〜15秒）。 */
	UPROPERTY()
	int32 LocalIRIntervalSeconds = 5;

	/** 過去に入力したマーカー名の履歴（新しい順。マーカー名ダイアログで再利用する）。 */
	UPROPERTY()
	TArray<FString> MarkerNameHistory;

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
	// メーター取得の周期(ms)。無線回線での往復を見込んだ既定値。
	// 実際の周期は直近RTTに応じて自動で伸びる（GetPollingScale）。
	int32 MeterIntervalMs = 300;

	/** メーター表示を「信号インジケータ（〇）」にするか（false＝フルメーター）。 */
	UPROPERTY()
	bool bMeterSignalIndicator = false;

	/** 信号〇を「トラックに1つ」にするか（true＝1つ／false＝chごと）。1つにすると取得も軽い。 */
	UPROPERTY()
	bool bMeterDotPerTrack = false;

	/** トラック一覧を自動更新するか（false＝「更新」ボタンでの手動更新）。 */
	UPROPERTY()
	bool bTrackListAutoRefresh = true;

	/** トラック一覧の自動更新間隔（秒）。 */
	UPROPERTY()
	int32 TrackRefreshIntervalSec = 3;

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
	UPROPERTY() int32   SlackExportScope = 0;        // Slack出力の対象：0=チェックありシートのみ/1=全シート
	UPROPERTY() int32   ScriptMatchScope = 0;        // 台本一致の対象範囲：0=シート/1=全シート
	UPROPERTY() int32   ScriptMatchCheckFilter = 0;  // 台本一致の対象行フィルタ：0=見ない/1=チェック済みのみ/2=未チェックのみ

	// --- サーバー認証（mTLS）。接続は常に TLS ---
	/** 登録済みサーバーの認証設定一覧（serverId → 期待フィンガープリント）。 */
	UPROPERTY() TArray<FServerAuthEntry> ServerAuthEntries;

	/** 最後に選択していた serverId（次回起動時にドロップダウンの選択を復元する）。 */
	UPROPERTY() FString SelectedServerId;

	// --- メンバー通話（在席名簿）---
	UPROPERTY() FString IntercomUserName;      // 自分の表示名
	UPROPERTY() FString PresenceFolder;        // 在席名簿フォルダ（手動指定・任意）
	UPROPERTY() FString ResolvedPresenceFolder; // 接続時に解決した RadNodzToolkit/GroupTalk の絶対パス（次回起動時の既定に使う）
};
