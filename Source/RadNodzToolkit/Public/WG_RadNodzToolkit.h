// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"   // ETextCommit::Type

#include "RadnodzClient.h"
#include "RadnodzUtility.h"
#include "RigdocksSilver_Track.h"
#include "RigdocksSilver_Marker.h"   // FMarker（マーカー一覧表示に使う）
#include "RigdocksBronze_Cursor.h"
#include "ReaperMicInputMode.h"
#include "ReaperMonitorOutputMode.h"
#include "ParaknotRpc.h"
#include "ReaperMicSetting.h"
#include "ReaperSaveGame.h"   // FServerAuthEntry
#include "RadnodzRpcQueue.h"  // 同期RPCを実行する専用スレッド
#include "Actions/IRMeasurementSession.h"   // FIRMeasurementSession（IR計測アクション間で共有）

#include "ParaknotRpc.h"      // FParaknotInstance（PARAKNOT の候補）

#include "WG_RadNodzToolkit.generated.h"

/**
 * ワーカースレッドで実行中のRPCが使う UObject（RpcCall / RpcBatch）を
 * GCから守るための保持枠。
 *
 * UObject の生成と破棄はゲームスレッド専用なので、RPC は
 *   Build（GT・UObject生成） → Execute（ワーカー・通信のみ） → Parse（GT・UObject生成）
 * の3段に分けて流す。ワーカーが Execute している間、生成済みの UObject を
 * 誰も参照していないとGCに回収されるため、ここで参照を保持する。
 */
USTRUCT()
struct FRadnodzRpcRefSet
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<UObject>> Objects;
};

class UWG_TrackCard;
class UWG_MarkerRow;
class URadnodzRpcBatch;
class UWG_AddTracksDialog;
class UWG_MicSettingsDialog;
class UReaperSaveGame;
class UReaStreamReceiver;
class UReaStreamSender;
class UParaknotSender;
class UParaknotReceiver;
struct FMonitorOutputSettings;
class UReaStreamGroupReceiver;
class UMediaItem;
class UTranscriptionContext;
class UWG_MediaItemRow;
class UVerticalBox;
class UHorizontalBox;
class UScrollBox;
class USizeBox;
class UButton;
class UTextBlock;
class UEditableTextBox;
class UBorder;
class UWidgetSwitcher;
class UComboBoxString;
class USlider;
class URadNodzAction;

/**
 * 1トラック分のメディア（テイク）一覧キャッシュ。
 * 展開のたびにReaperへ重い再取得をしないよう、取得結果を保持する。
 */
USTRUCT()
struct FTrackMediaCacheEntry
{
	GENERATED_BODY()

	UPROPERTY(Transient) TArray<TObjectPtr<UMediaItem>> Items;
	UPROPERTY(Transient) TArray<FString>                Names;
	UPROPERTY(Transient) TArray<double>                 Starts;
};

/** 「リスト」タブの1行（読み込んだ各列の値＋収録済みチェック）。 */
USTRUCT()
struct FListRow
{
	GENERATED_BODY()

	UPROPERTY(Transient) TArray<FString> Cells;        // 各列の値
	UPROPERTY(Transient) bool            bChecked = false;   // 収録済みチェック

	// 「収録チェック」ボタン（文字起こし突き合わせ）の結果。Cellsには混ぜず別管理する
	// （Cellsは読み込み元データそのものとして扱いたいため）。
	UPROPERTY(Transient) FString TranscribedMatch;     // 一番近かった文字起こし結果（未計算は空文字）
	UPROPERTY(Transient) int32   MatchScore = -1;      // 類似度(0-100、-1=未計算)
};

/** 「リスト」タブの1シート分のデータ。 */
USTRUCT()
struct FListSheet
{
	GENERATED_BODY()

	UPROPERTY(Transient) FString          SheetName;
	UPROPERTY(Transient) TArray<FListRow> Rows;
};

/** 台本一致：文字起こしジョブが1件ずつ蓄積する結果（UObjectを含まないため非UPROPERTYで保持）。 */
struct FScriptMatchTranscript
{
	FString Name;
	FString Text;
	double  Start = 0.0;
	double  Length = 0.0;
};

/**
 * Reaper録音コントローラーのウィジェット。
 * UIとロジックはすべてC++(RebuildWidget)で構築する。
 *
 * 使い方:
 *   - このC++クラスを直接 CreateWidget して AddToViewport するだけで動く。
 *   - もしくは空のWidget Blueprint(親=このクラス)を作って使ってもよい。
 */
UCLASS(Blueprintable)
class RADNODZTOOLKIT_API UWG_RadNodzToolkit : public UUserWidget
{
	GENERATED_BODY()

public:

	UWG_RadNodzToolkit(const FObjectInitializer& ObjectInitializer);

	// ----------------------------------------------------------------
	// 接続設定
	// ----------------------------------------------------------------

	/** 接続先IPアドレス */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaper|Connection")
	FString IPAddress = TEXT("192.168.1.1");

	/** 接続先ポート番号（初期値: 23760） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaper|Connection")
	int32 Port = 23760;

	// ----------------------------------------------------------------
	// 状態
	// ----------------------------------------------------------------

	/** 接続中かどうか */
	UPROPERTY(BlueprintReadOnly, Category = "Reaper|State")
	bool bIsConnected = false;

	/** 録音中かどうか */
	UPROPERTY(BlueprintReadOnly, Category = "Reaper|State")
	bool bIsRecording = false;

	/** 取得済みのトラック一覧 */
	UPROPERTY(BlueprintReadOnly, Category = "Reaper|Tracks")
	TArray<FTrackDetail> TrackList;

	/** アーム中のトラックIndexセット */
	UPROPERTY(BlueprintReadOnly, Category = "Reaper|Tracks")
	TArray<int32> ArmedTrackIndices;

	/** 登録済みマイク設定一覧（名前＋入力ch設定。設定ダイアログで編集・端末に保存される）。 */
	UPROPERTY(BlueprintReadOnly, Category = "Reaper|Tracks")
	TArray<FMicSetting> Mics;

	// ----------------------------------------------------------------
	// 操作関数（BP・C++両方から呼べる）
	// ----------------------------------------------------------------

	/**
	 * Reaperに接続する。
	 * 成功時 → OnConnected / 失敗時 → OnConnectionFailed を呼ぶ。
	 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Connection")
	void Connect();

	/** 接続を切断する。 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Connection")
	void Disconnect();

	/** 同一ネットワーク内のReaperを探索する別ウインドウを開く。 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Connection")
	void OpenReaperScan();

	/** 探索一覧で選ばれたIPを採用して接続する（スキャンダイアログから呼ばれる）。 */
	void SelectScannedReaper(const FString& InIP);

	/**
	 * Reaperからトラック一覧を取得してTrackListを更新する。
	 * 完了後 → OnTracksLoaded を呼ぶ。
	 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void FetchTracks();

	/**
	 * 指定トラックのアームON/OFFを切り替える（複数同時選択対応）。
	 * @param TrackIndex  FTrackDetailのIndexと一致する値
	 * @param bArm        true=アームON / false=アームOFF
	 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void SetTrackArmed(int32 TrackIndex, bool bArm);

	/**
	 * アーム中かどうかを返す。
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reaper|Tracks")
	bool IsTrackArmed(int32 TrackIndex) const;

	/** 全トラックのアームを解除する。 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void ClearAllArmed();

	/**
	 * 親（フォルダ）トラックの畳み込み状態を切り替える。
	 * 畳み込むと配下の子トラックカードが非表示になる。
	 * @param TrackIndex  対象の親トラックのIndex
	 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void ToggleCollapse(int32 TrackIndex);

	/**
	 * トラックのメディア一覧「展開中」状態を記録する。トラック一覧の自動更新でカードが
	 * 作り直されても、展開していたトラックは展開したまま復元するために使う。
	 * @param TrackIndex  対象トラックのIndex
	 * @param bExpanded   展開中か
	 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void SetTrackMediaExpanded(int32 TrackIndex, bool bExpanded);

	/**
	 * 親（フォルダ）トラック配下の子トラックを、まとめてREC ON/OFFする。
	 * 親トラック自身はRECしない。子が全てREC中なら全解除、そうでなければ全ON（トグル）。
	 * @param ParentTrackIndex  対象の親トラックのIndex
	 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void ToggleChildrenArmed(int32 ParentTrackIndex);

	/** アーム中のトラック数を返す。 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reaper|Tracks")
	int32 GetArmedTrackCount() const;

	/** 指定トラックのソロON/OFFを切り替える。 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void SetTrackSolo(int32 TrackIndex, bool bSolo);

	/** 指定トラックがソロ中か。 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reaper|Tracks")
	bool IsTrackSoloed(int32 TrackIndex) const;

	/** 指定トラックのミュートON/OFFを切り替える。 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void SetTrackMute(int32 TrackIndex, bool bMute);

	/** 指定トラックがミュート中か。 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reaper|Tracks")
	bool IsTrackMuted(int32 TrackIndex) const;

	/**
	 * 指定トラックのメディアアイテム（テイク）一覧を取得する。
	 * @param TrackIndex  対象トラックのIndex
	 * @param OutItems    各メディアアイテム
	 * @param OutNames    各メディア名
	 * @param OutStarts   各メディアの開始位置（秒）
	 */
	void GetTrackMediaItems(int32 TrackIndex, TArray<UMediaItem*>& OutItems,
		TArray<FString>& OutNames, TArray<double>& OutStarts);

	/**
	 * メディア一覧をキャッシュ経由で取得する。
	 * キャッシュにあればReaperへ問い合わせず即返す（メディア展開のたびの重い再取得を防ぐ）。
	 * 無ければ GetTrackMediaItems() で取得してキャッシュする。
	 * キャッシュは FetchTracks()（接続/更新/トラック増減）でクリアされ、接続/更新時に背景で再先読みされる。
	 */
	void GetTrackMediaItemsCached(int32 TrackIndex, TArray<UMediaItem*>& OutItems,
		TArray<FString>& OutNames, TArray<double>& OutStarts);

	/**
	 * 指定トラックのメディア（テイク）件数だけを軽量に取得する。
	 * キャッシュがあれば通信ゼロ、無ければ一覧取得1回のみ（各メディアの名前/開始位置は取らない）。
	 * 録音開始時の件数控えなど「数だけ欲しい」用途に使う。
	 */
	int32 GetTrackMediaCount(int32 TrackIndex);

	/** 指定トラックのメディアキャッシュを破棄する（録音後など、その場で内容が変わったとき）。 */
	void InvalidateTrackMediaCache(int32 TrackIndex);

	/** 全トラックのメディアキャッシュを破棄する。 */
	void ClearTrackMediaCache();

	/** メーター受信（入力レベルの取得・表示）が有効か。トラックカードのメーター表示判定に使う。 */
	bool IsMeterReceiveEnabled() const { return bMeterReceiveEnabled; }

	/** リスト行（UWG_ListRow）から：収録済みチェックの状態を保存する。 */
	void SetListRowChecked(int32 RowIndex, bool bChecked);

	/** シートタブ（UWG_SheetTab）から：表示シートを切り替える。 */
	void SwitchListSheet(int32 SheetIndex);

	/** 列ピッカー（UWG_ColumnPickerButton）から：選ばれた列で台本一致を実行する。 */
	void HandleScriptMatchColumnChosen(int32 ColIndex);

	/** モーダルダイアログ（UWG_SlackRequiredDialog等）から：表示するタブを切り替える（0=ホーム/1=リスト/2=設定）。 */
	void SetActiveTab(int32 TabIndex);

	/** 再生カーソルを指定秒（プロジェクト先頭からの絶対位置）へ移動する。 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Recording")
	void MoveCursorToSeconds(double Seconds);

	/** メディア行から：そのメディア先頭へカーソル移動して即再生する。 */
	void PlayFromMediaRow(UWG_MediaItemRow* Row, UMediaItem* MediaItem, double StartSeconds);

	/** メディア行から：再生を停止する。 */
	void StopMedia();

	/** メディアアイテムの名前を変更する。 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void RenameMediaItem(UMediaItem* MediaItem, const FString& NewName);

	/** メディアアイテムのミュートON/OFFを設定する。 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void SetMediaItemMute(UMediaItem* MediaItem, bool bMute);

	/**
	 * メディアアイテムのレベルを取得する。
	 * @param OutMomentaryLUFS  最大モーメンタリー ラウドネス（LUFS）
	 * @param OutPeakDb         ピーク値（dBFS）
	 * @param OutRmsDb          RMS値（dBFS, ピーク一覧からの近似）
	 */
	void GetMediaItemLevels(UMediaItem* MediaItem, double& OutMomentaryLUFS, double& OutPeakDb, double& OutRmsDb);

	/** メディアアイテムの最大モーメンタリー ラウドネス（LUFS）のみを取得する（軽量）。 */
	double GetMediaItemMomentaryLUFS(UMediaItem* MediaItem);

	/**
	 * モーメンタリー ラウドネスをキャッシュ付きで取得する。
	 * 同じCacheKeyで2回目以降は再取得せずキャッシュ値を返す（メディア再展開時の再計算を防ぐ）。
	 * キャッシュは FetchTracks()（更新）・切断でクリアされる。
	 */
	double GetMediaItemMomentaryLUFSCached(UMediaItem* MediaItem, const FString& CacheKey);

	/**
	 * キャッシュ済みのモーメンタリー ラウドネスを参照する（未計測なら計算しない）。
	 * 「計測」ボタンを押すまで数値を出さないため、メディア行はこの参照のみを使う。
	 * @return キャッシュに存在すれば true（OutLufsに値）。未計測なら false。
	 */
	bool TryGetCachedMomentaryLUFS(const FString& CacheKey, double& OutLufs) const;

	/** 表示中の全トラックカードのメディア行に、キャッシュ済みラウドネスを反映する。 */
	void RefreshAllMediaLevels();

	// ---- 音量±調整 ----

	/** トラック音量を取得する（AZ_GetTrackItemVolume）。 */
	double GetTrackVolume(class UMediaTrack* Track);
	/** メーター表示が「信号インジケータ（〇）」モードか。トラックカードがメーター構築時に参照する。 */
	bool IsMeterIndicatorMode() const { return bMeterSignalIndicator; }

	/**
	 * トラックのメーター用ラベルを返す。
	 * 信号〇モード＋「トラックに1つ」のときは {"信号"} の1要素、それ以外は入力chの名前一覧。
	 */
	TArray<FString> GetMeterLabelsForTrack(class UMediaTrack* Track);

	/** トラックのチャンネル数(I_NCHAN)を取得する（未接続時は1）。 */
	int32 GetTrackNumChannels(class UMediaTrack* Track);
	/** トラックの録音入力チャンネル数を取得する（メーターの本数に使う。無効時はI_NCHAN、未接続時は1）。 */
	int32 GetTrackInputChannelCount(class UMediaTrack* Track);
	/** トラックの録音入力チャンネル名一覧を取得する（メーターのラベル用。要素数＝入力ch数）。 */
	TArray<FString> GetTrackInputChannelLabels(class UMediaTrack* Track);
	/** トラック音量を AddAmount だけ加算し（AZ_SetTrackItemAddVolume）、加算後の値を返す。 */
	double AddTrackVolume(class UMediaTrack* Track, double AddAmount);

	/** メディア音量を取得する（AZ_GetMediaItemVolume）。 */
	double GetMediaItemVolume(UMediaItem* MediaItem);
	/** メディア音量を AddAmount だけ加算し（AZ_SetMediaItemAddVolume）、加算後の値を返す。 */
	double AddMediaItemVolume(UMediaItem* MediaItem, double AddAmount);
	/** メディアアイテムのピーク値（dBFS、音量フェーダーの影響を受けない生の値）を取得する。 */
	double GetMediaItemPeakDb(UMediaItem* MediaItem);

	/**
	 * 新しいトラックをReaperに追加する。
	 * @param TrackName  追加するトラック名（空文字可）
	 * @param bArmAfterAdd  追加後すぐにアームするか
	 * 追加後 → FetchTracks() でTrackListを更新し OnTracksLoaded を呼ぶ。
	 * 成功時 → OnTrackAdded を追加で呼ぶ。
	 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void AddTrack(const FString& TrackName, bool bArmAfterAdd = false);

	/**
	 * 同名のトラックが存在しない場合のみ追加する（重複防止版）。
	 * @param TrackName  追加するトラック名
	 * @param bArmAfterAdd  追加後すぐにアームするか
	 * @return 実際に追加されたか（重複で追加されなかった場合 false）
	 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	bool AddTrackUnique(const FString& TrackName, bool bArmAfterAdd = false);

	/**
	 * 登録済みマイク（MicNames）の本数分だけトラックを一括追加する。
	 * 生成されるトラック名は「収録素材名_マイク名」（素材名が空ならマイク名のみ）。
	 * 同名マイクが複数登録されている場合のみ末尾に連番を付ける。
	 * @param Material  収録素材名（例: ドラム）
	 * @param ParentTrackName  追加先の親トラック名（"Master"/空＝最上位・親なし）。追加ダイアログで選択する。
	 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void AddTracksForMaterial(const FString& Material, const FString& ParentTrackName = TEXT("Master"));

	/** 登録済みマイク設定を取得する。 */
	const TArray<FMicSetting>& GetMics() const { return Mics; }

	/** マイク設定一覧を更新して端末に保存する（マイク登録ダイアログから呼ぶ）。 */
	void SaveMics(const TArray<FMicSetting>& InMics);

	/**
	 * リストの「マイク」シートからマイク名とch数の一覧を返す。
	 * 設定のマイク登録ダイアログで、名前のプルダウン候補として使う。未読込時は空。
	 */
	void GetMicCatalog(TArray<FString>& OutNames, TArray<int32>& OutChannels) const;

	/**
	 * Reaperの入力チャンネル名一覧を返す（index→「input 0」等）。未接続時は空配列。
	 * マイク登録/トラック入力ダイアログのドロップダウン用。
	 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	TArray<FString> GetInputChannelNames();

	/** 指定トラックの入力チャンネル設定（開始ch・ch数, Audio）を設定する。 */
	void SetTrackInputChannel(class UMediaTrack* Track, int32 FirstChannel, int32 NumChannels);

	/** 指定トラックの現在の入力チャンネル設定を取得する（未接続時は 開始0/1ch）。 */
	void GetTrackInputChannel(class UMediaTrack* Track, int32& OutFirstChannel, int32& OutNumChannels);

	/**
	 * 指定トラックの名前を変更する。
	 * @param TrackIndex  対象トラックのIndex
	 * @param NewName     新しい名前
	 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Tracks")
	void RenameTrack(int32 TrackIndex, const FString& NewName);

	/** 録音を開始する。アーム済みトラックが録音対象になる。 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Recording")
	void StartRecording();

	/** 再生を開始する。 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Recording")
	void StartPlaying();

	/** 録音/再生を停止する。 */
	UFUNCTION(BlueprintCallable, Category = "Reaper|Recording")
	void StopRecording();

	// ----------------------------------------------------------------
	// BPイベント（ビジュアル側でオーバーライドしてUI更新する）
	// ----------------------------------------------------------------

	/** 接続成功時に呼ばれる。UIをConnected状態に更新する。 */
	UFUNCTION(BlueprintNativeEvent, Category = "Reaper|Events")
	void OnConnected();
	virtual void OnConnected_Implementation();

	/** 接続失敗時に呼ばれる。 */
	UFUNCTION(BlueprintNativeEvent, Category = "Reaper|Events")
	void OnConnectionFailed(const FString& ErrorMessage);
	virtual void OnConnectionFailed_Implementation(const FString& ErrorMessage);

	/** 切断時に呼ばれる。 */
	UFUNCTION(BlueprintNativeEvent, Category = "Reaper|Events")
	void OnDisconnected();
	virtual void OnDisconnected_Implementation();

	/** トラック一覧取得完了時に呼ばれる。ScrollBoxへの追加などUIを更新する。 */
	UFUNCTION(BlueprintNativeEvent, Category = "Reaper|Events")
	void OnTracksLoaded(const TArray<FTrackDetail>& Tracks);
	virtual void OnTracksLoaded_Implementation(const TArray<FTrackDetail>& Tracks);

	/**
	 * AddTrack / AddTrackUnique でトラックが追加された時に呼ばれる。
	 * @param NewTrack  追加されたトラックの詳細
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Reaper|Events")
	void OnTrackAdded(const FTrackDetail& NewTrack);
	virtual void OnTrackAdded_Implementation(const FTrackDetail& NewTrack);

	/** 録音開始時に呼ばれる。 */
	UFUNCTION(BlueprintNativeEvent, Category = "Reaper|Events")
	void OnRecordingStarted();
	virtual void OnRecordingStarted_Implementation();

	/** 録音停止時に呼ばれる。 */
	UFUNCTION(BlueprintNativeEvent, Category = "Reaper|Events")
	void OnRecordingStopped();
	virtual void OnRecordingStopped_Implementation();

	/**
	 * トラックのアーム状態が変化した時に呼ばれる。
	 * トラック行のUIを更新(赤いハイライト等)するために使う。
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Reaper|Events")
	void OnTrackArmChanged(int32 TrackIndex, bool bArmed);
	virtual void OnTrackArmChanged_Implementation(int32 TrackIndex, bool bArmed);

	// ----------------------------------------------------------------
	// UI設定
	// ----------------------------------------------------------------

	/** 生成するトラックカードのクラス（未指定ならC++のUWG_TrackCardを使用）。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaper|UI")
	TSubclassOf<UWG_TrackCard> TrackCardClass;

	/** ＋追加で開くダイアログのクラス（未指定ならC++のUWG_AddTracksDialogを使用）。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaper|UI")
	TSubclassOf<UWG_AddTracksDialog> AddDialogClass;

	/** 設定で開くマイク登録ダイアログのクラス（未指定ならC++のUWG_MicSettingsDialogを使用）。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaper|UI")
	TSubclassOf<UWG_MicSettingsDialog> MicSettingsDialogClass;

	// ----------------------------------------------------------------
	// 表示倍率（ピンチイン/アウト）
	// ----------------------------------------------------------------

	/** 現在の表示倍率。ピンチ操作で 0.5〜3.0 の範囲で変化する。 */
	UPROPERTY(BlueprintReadOnly, Category = "Reaper|UI")
	float ZoomLevel = 1.0f;

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// ピンチイン/アウト（マグニファイ ジェスチャ）で全体をズームする
	virtual FReply NativeOnTouchGesture(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	// --- C++で構築したUIへの参照 ---
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> IPAddressInput;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> PortInput;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       LocalIPText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       HeaderIpText;   // タイトル下：自分IP/Reaper IP（読み取り専用）
	UPROPERTY(Transient) TObjectPtr<UButton>          ConnectButton;
	UPROPERTY(Transient) TObjectPtr<UButton>          AddTrackButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       AddTrackButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          MicSettingsButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MicSettingsButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          MonitorButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          MicSendButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MicSendButtonText;

	// 画面録画（設定画面）：録画開始/停止ボタンとラベル。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ScreenRecRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          ScreenRecButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ScreenRecButtonText;
	// 録画中に経過秒＋停止ボタンのオーバーレイを表示するか（ON/OFFトグル）。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ScreenRecOverlayRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          ScreenRecOverlayButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ScreenRecOverlayButtonText;
	// 録画にマイク音を含めるか（ON/OFFトグル）。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ScreenRecMicRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          ScreenRecMicButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ScreenRecMicButtonText;
	// カウントダウン(3・2・1)全画面オーバーレイ。
	UPROPERTY(Transient) TObjectPtr<UBorder>          ScreenRecCountdownOverlay;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ScreenRecCountdownText;

	// 接続先デバイス情報バー（接続ボタン下・ツールバー上）。
	// 録音サンプリングレート/ビット深度・入力/出力ch数を1行で表示する。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       DeviceInfoText;

	// 接続先デバイス情報のキャッシュ（接続時・更新時に取得）。
	double  RecSampleRate = 0.0;   // 録音サンプリングレート(Hz)
	int32   RecBitDepth   = 0;     // 録音ビット深度(bit)
	int32   InputChCount  = 0;     // 入力チャンネル数
	int32   OutputChCount = 0;     // 出力チャンネル数
	FString RemoteDeviceName;      // 接続先Reaper PCの端末名（ヘッダー表示にも使う）

	// 接続先デバイス情報（Hz/bit/ch・端末名）をReaperから取得して表示へ反映する。
	void UpdateDeviceInfo();
	// キャッシュ済みの値からDeviceInfoTextの文言だけを組み立て直す（RPC無し。言語切替時の再適用に使う）。
	void RefreshDeviceInfoText();

	/** ヘッダーに端末名/IPを表示するか。端末に保存される（既定ON）。 */
	bool bShowDeviceNameIP = true;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ShowDeviceInfoRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          ShowDeviceInfoButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ShowDeviceInfoButtonText;
	UFUNCTION() void HandleShowDeviceInfoClicked();
	void UpdateShowDeviceInfoButton();

	// 通信時間の計測（設定タブ・接続セクション、「状態」の下）。
	// 軽量RPCを往復させ、端末→サーバー（送信）と サーバー→端末（結果取得）の
	// 平均時間を CommTimeText に表示する。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       CommTimeRowLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       CommTimeText;                 // 直近の計測結果（未計測は「--」）
	UPROPERTY(Transient) TObjectPtr<UButton>          CommTimeMeasureButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       CommTimeMeasureButtonText;
	UFUNCTION() void HandleMeasureCommTimeClicked();
	/**
	 * 通信時間を計測・表示する。
	 * 本体接続が生きていればそれで1往復、未接続なら計測用の接続を張って測る。
	 */
	void MeasureCommTime();

	/** 計測専用のクライアントで RADNODZ へ疎通し、非同期に計測して表示する。 */
	void MeasureCommTimeByConnecting();

	/** 計測用接続での計測中フラグ（二重実行防止）。 */
	bool bIsMeasuringCommTime = false;

	/** 計測の世代番号。古い非同期結果を無視するのに使う。 */
	int32 CommTimeGeneration = 0;
	double CommTimeProbeTimer = 0.0;                          // 定期計測用タイマ（秒）
	static constexpr double CommTimeProbeIntervalSec = 30.0;  // 定期計測の間隔（秒）

	// 接続監視：接続中は定期的に RigdocksClient::IsConnected() を確認し、
	// 切断されていれば切断状態へ遷移する。
	double ConnCheckTimer = 0.0;                              // 接続確認用タイマ（秒）
	static constexpr double ConnCheckIntervalSec = 1.0;       // 接続確認の間隔（秒）

	// メーター受信トグル（ホーム画面のボタン）。設定画面のボタンと状態を連動させる。
	UPROPERTY(Transient) TObjectPtr<UButton>          MeterRecvButton;

	/**
	 * 「モニター」ボタンの下に出す受信レベルのメーター。
	 * PARAKNOT から届いている音のピークを dB 目盛で出す（NativeTick で更新）。
	 */
	UPROPERTY(Transient) TObjectPtr<class UProgressBar> MonitorLevelMeter;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MeterRecvButtonText;
	// メーター受信トグル（設定画面の行）。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MeterRecvRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MeterRecvSettingButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MeterRecvSettingButtonText;

	/** メーター受信（入力レベルの取得・表示）が有効か。必要なときだけONにする。既定OFF（処理を軽くするため）。 */
	bool bMeterReceiveEnabled = false;

	// ============================================================
	//  「リスト」タブ（収録リスト：Excel/CSV読み込み・チェック・書き出し）
	// ============================================================
	// --- 設定値（端末に保存） ---
	int32   ListFormat   = 0;          // 0=Excel(.xlsx) / 1=CSV / 2=Google Sheets
	FString ListFilePath;              // 読み込むファイル（サーバ側のフルパス。Excel/CSV用）
	FString ListSheetName;             // 読み込むシート名（空＝全シート。Excel/Google Sheets共通）
	int32   ListStartRow = 1;          // 読み込み開始行（1始まり）
	int32   ListStartCol = 1;          // 読み込み開始列（1始まり）
	int32   ListColCount = 2;          // 読み込む列数（既定2）
	bool    bListUseCheckbox = true;   // チェックボックスを表示するか
	FString ListExportPath;            // 書き出し先（サーバ側のフルパス。Excel/CSV用）
	FString GoogleSheetSpreadsheetId;  // Google SheetsのスプレッドシートID
	FString GoogleSheetClientId;       // Google SheetsのクライアントID（OAuthユーザーアカウント接続用）
	int32   SlackExportScope = 0;        // Slack出力の対象：0=チェックありシートのみ / 1=全シート
	int32   ScriptMatchScope = 0;        // 台本一致の対象範囲：0=シート（現在表示中のみ）/ 1=全シート
	int32   ScriptMatchCheckFilter = 0;  // 台本一致の対象行フィルタ：0=見ない（絞り込みなし）/ 1=チェック済みのみ / 2=未チェックのみ

	// --- データモデル ---
	UPROPERTY(Transient) TArray<FListSheet> ListSheets;    // 読み込んだ全シート
	int32 ListActiveSheet = 0;                             // 表示中のシート

	// --- マイク登録カタログ ---
	// マイク名一覧はリストの「マイク」シート由来。登録UIは設定のマイク登録ダイアログ側に集約した。
	// （取得は GetMicCatalog()。ここではドロップダウン項目の見た目だけ用意する）
	// コンボのドロップダウン項目を明るい文字色で生成する（暗背景で見えるように）
	UFUNCTION() UWidget* MakeComboItem(FString Item);

	// サーバーIDコンボ用：ドロップダウン項目/選択表示を白文字で生成する。
	UFUNCTION() UWidget* MakeServerIdComboItem(FString Item);

	// OnGenerateWidgetEvent で生成したコンボ項目ウィジェットの参照保持。
	// これらはどのパネルにも属さない（UPROPERTYの参照連鎖に乗らない）ため、保持しないと
	// GCで回収され、Slate側が解放済みのスタイルを参照してクラッシュする
	// （例：立体音響セクションを初表示した瞬間にHRTFコンボの中身が落ちる）。
	UPROPERTY(Transient) TArray<TObjectPtr<UWidget>> ComboGeneratedItems;

	// --- リスト画面ウィジェット ---
	UPROPERTY(Transient) TObjectPtr<UVerticalBox>   ListRowsBox;     // 行を並べる
	UPROPERTY(Transient) TObjectPtr<UHorizontalBox> ListSheetTabRow; // 下のシート切替の入れ物
	UPROPERTY(Transient) TObjectPtr<class UScrollBox>     ListSheetScroll;   // 横スクロール（タブがはみ出したら）
	UPROPERTY(Transient) TObjectPtr<UHorizontalBox>      ListSheetTabsBox;  // シートタブを横並びにする
	UPROPERTY(Transient) TArray<TObjectPtr<class UWG_SheetTab>> ListSheetTabWidgets; // 各シートタブ
	UPROPERTY(Transient) TObjectPtr<UTextBlock>     ListStatusText;  // 状態表示
	UPROPERTY(Transient) TObjectPtr<UTextBlock>     ListTitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>     ListLoadButtonText;    // 「読み込み」ボタンのラベル（言語切替で更新）
	UPROPERTY(Transient) TObjectPtr<UTextBlock>     ListExportButtonText;  // 「書き出し」ボタンのラベル（言語切替で更新）
	UPROPERTY(Transient) TObjectPtr<UTextBlock>     RecordingCheckButtonText; // 「台本一致」ボタンのラベル（言語切替で更新）
	UPROPERTY(Transient) TArray<TObjectPtr<class UWG_ListRow>> ListRowWidgets;
	// 台本一致でチェック対象に選んだ列（Row.Cellsの0始まりindex、-1=未選択）。選んだ列のセルをハイライトする。
	int32 ScriptMatchHighlightCol = -1;

	// --- 台本一致：チェック対象列ピッカー（ボタン押下時にその場で列を選ばせる） ---
	UPROPERTY(Transient) TObjectPtr<UBorder>        ScriptMatchColumnPickerBg;
	UPROPERTY(Transient) TObjectPtr<UHorizontalBox> ScriptMatchColumnPickerBox;
	UPROPERTY(Transient) TArray<TObjectPtr<class UWG_ColumnPickerButton>> ScriptMatchColumnPickerButtons;
	void ShowScriptMatchColumnPicker();
	void HideScriptMatchColumnPicker();
	void RunScriptMatch(int32 CheckCol);

	// --- 台本一致：文字起こしジョブ（重いWhisper呼び出しを1件ずつTickで進め、UIを固まらせない） ---
	// 進行中インジケータは専用アイコンを持たず、右上のStatusIcon（bStatusIconSpin）を流用する。
	bool  bScriptMatchJobActive = false;
	int32 ScriptMatchJobCheckCol = 0;
	UPROPERTY(Transient) TArray<TObjectPtr<UMediaItem>> ScriptMatchPendingItems;
	TArray<FString> ScriptMatchPendingNames;
	TArray<double>  ScriptMatchPendingStarts;
	int32 ScriptMatchJobCursor = 0;
	TArray<FScriptMatchTranscript> ScriptMatchTranscriptions;
	void BeginScriptMatchJob(int32 CheckCol);
	void TickScriptMatchJob(float DeltaSeconds);
	void FinishScriptMatchJob();

	// --- リスト設定UI（設定画面） ---
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListFormatRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>    ListFormatButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListFormatButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListFileRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> ListFilePathInput;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListSheetRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> ListSheetInput;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListStartRowRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> ListStartRowInput;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListStartColRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> ListStartColInput;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListColCountRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> ListColCountInput;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> SlackExportScopeRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>    SlackExportScopeButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> SlackExportScopeButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ScriptMatchScopeRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>    ScriptMatchScopeButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ScriptMatchScopeButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ScriptMatchCheckFilterRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>    ScriptMatchCheckFilterButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ScriptMatchCheckFilterButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListCheckboxRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>    ListCheckboxButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListCheckboxButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListExportRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> ListExportPathInput;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListGSheetIdRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> ListGSheetIdInput;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ListGSheetClientIdRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> ListGSheetClientIdInput;
	// 形式（Excel/CSV/Google Sheets）に応じて表示/非表示を切り替える行の入れ物。
	UPROPERTY(Transient) TObjectPtr<UBorder> ListFileRow;         // ファイル（Excel/CSV用。Google Sheetsでは非表示）
	UPROPERTY(Transient) TObjectPtr<UBorder> ListExportRow;       // 書き出し先（Excel/CSV用。Google Sheetsでは非表示）
	UPROPERTY(Transient) TObjectPtr<UBorder> ListGSheetIdRow;     // GoogleSheet ID（Google Sheetsのみ表示）
	UPROPERTY(Transient) TObjectPtr<UBorder> ListGSheetClientIdRow; // GoogleSheet クライアントID（Google Sheetsのみ表示）
	void UpdateListFormatRowsVisibility();

	// --- リスト機能のメソッド ---
	UWidget* BuildListPage();                       // リストタブの画面を構築
	void RebuildListView();                         // ListSheets[ListActiveSheet] を描画
	void RebuildSheetTabs();                        // 下部のシート切替タブを作り直す

	UFUNCTION() void HandleListTabClicked();
	UFUNCTION() void HandleListLoadClicked();       // 設定のファイルを読み込んでリスト表示
	UFUNCTION() void HandleListExportClicked();     // 結果を書き出す
	UFUNCTION() void HandleListSlackClicked();      // 結果一覧をSlackへ送る
	UFUNCTION() void HandleRecordingCheckClicked(); // 「台本一致」押下：ガード確認後、列ピッカーを表示する
	void EnsureTranscriptionModelLoaded();          // Whisperモデルの遅延ロード（未ロードなら読み込んで保持）
	UFUNCTION() void HandleListFormatClicked();     // Excel⇄CSV 切替
	UFUNCTION() void HandleListCheckboxClicked();   // チェックボックス要否 切替
	UFUNCTION() void HandleSlackExportScopeClicked();       // Slack出力の対象（チェックありシート/全て）切替
	void UpdateSlackExportScopeButton();
	UFUNCTION() void HandleScriptMatchScopeClicked();       // 台本一致の対象範囲（シート/全シート）切替
	UFUNCTION() void HandleScriptMatchCheckFilterClicked(); // 台本一致の対象行フィルタ（見ない/チェック済み/未チェック）切替
	void UpdateScriptMatchScopeButton();
	void UpdateScriptMatchCheckFilterButton();
	UFUNCTION() void HandleListFilePathCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleListSheetCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleListStartRowCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleListStartColCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleListColCountCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleListExportPathCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleListGSheetIdCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleListGSheetClientIdCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	void UpdateListFormatButton();
	void UpdateListCheckboxButton();
	// 各形式の読み込み（OutSheets を埋める）。成功で true。
	bool ReadListFromExcel(TArray<FListSheet>& OutSheets, FString& OutError);
	bool ReadListFromCsv(TArray<FListSheet>& OutSheets, FString& OutError);
	bool ReadListFromGSheet(TArray<FListSheet>& OutSheets, FString& OutError);
	// 実際の読み込み本体。「読み込み中・・・」を1フレーム描かせた後に NativeTick から呼ばれる。
	void StartListLoad();
	// 読み込み結果を画面へ反映する（表の作り直し・状態表示の更新）。
	void FinishListLoad(bool bOk, TArray<FListSheet>&& Sheets, const FString& Error);
	// >0 の間は読み込み開始待ち。毎ティック減らし、0になった時点で StartListLoad() を呼ぶ。
	int32 ListLoadPendingFrames = 0;
	UPROPERTY(Transient) TObjectPtr<UButton>          UpdateButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       UpdateButtonText;
	// 言語切替（9言語：日本語/English/FIGS/韓国語/中国語簡体・繁体）はプルダウンで選択する。
	UPROPERTY(Transient) TObjectPtr<UComboBoxString>  LanguageCombo;
	UFUNCTION() void HandleLanguageComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	// 言語コンボの選択状態を現在の言語に合わせて更新する
	void RefreshLanguageCombo();
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       TrackSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       IpLabel;           // 「接続先」行のラベル（IP＋ポート＋検索）
	UPROPERTY(Transient) TObjectPtr<UButton>          ScanButton;        // 同ネットワークのReaperを検索
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ScanButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          PlayButton;
	UPROPERTY(Transient) TObjectPtr<UButton>          RecordButton;
	UPROPERTY(Transient) TObjectPtr<USizeBox>         PlayButtonSize;    // 横向きでは高さを詰めて2段レイアウトの縦スクロールを防ぐ
	UPROPERTY(Transient) TObjectPtr<USizeBox>         RecordButtonSize;
	UPROPERTY(Transient) TObjectPtr<UButton>          ClearArmedButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ClearArmedButtonText;
	// トラック削除（選択→決定→確認→削除）
	UPROPERTY(Transient) TObjectPtr<UButton>          DeleteTrackButton;      // 「削除」⇄「決定」をトグル（再生/録音中は「マーカー」に切替）
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       DeleteTrackButtonText;
	UPROPERTY(Transient) TObjectPtr<class UImage>     DeleteTrackButtonIcon;  // ゴミ箱アイコン（マーカーモード時は非表示）
	UPROPERTY(Transient) TObjectPtr<class UHorizontalBox> DeleteSelectRow;    // 削除モード時のみ表示する全選択/全解除行
	UPROPERTY(Transient) TObjectPtr<UButton>          DeleteSelectAllButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       DeleteSelectAllButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          DeleteDeselectAllButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       DeleteDeselectAllButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          SlackShareButton;      // 全トラックの波形名＋ラウドネスをSlackへ共有
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SlackShareButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          NormalizeButton;       // 全メディアを計測して目標LUFSへ音量調整
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       NormalizeButtonText;
	UPROPERTY(Transient) TObjectPtr<class UImage>     NormalizeIcon;         // 実行中に縦回転する計測アイコン

	// 削除/全解除/計測調整/Slack/ロック：縦向きは1段、横向きは文字が読めるよう2段に組み替えて入れる箱。
	UPROPERTY(Transient) TObjectPtr<class UHorizontalBox> InfoRow;                 // 縦向き：1段（5個横並び）
	UPROPERTY(Transient) TObjectPtr<class UHorizontalBox> InfoRowLandscapeTop;     // 横向き：上段（削除/全解除/計測調整）
	UPROPERTY(Transient) TObjectPtr<class UHorizontalBox> InfoRowLandscapeBottom;  // 横向き：下段（Slack/ロック）
	UPROPERTY(Transient) TObjectPtr<class UVerticalBox>   InfoRowsWrap;           // 上記のいずれかを差し替えて入れる箱

	// --- ロック（誤操作防止） ---
	// ロック中はPlay/Rec/トラック追加/削除/全解除/計測調整を無効化する。グループトークは対象外。
	// 端末には保存しない（アプリ再起動時は必ず解除された状態に戻る）。
	UPROPERTY(Transient) TObjectPtr<UButton>          LockButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       LockButtonText;
	UPROPERTY(Transient) TObjectPtr<class UImage>     LockButtonIcon;   // ロック中/解除中でアイコンを差し替える
	bool bIsLocked = false;
	UFUNCTION() void HandleLockClicked();
	void UpdateLockButton();
	void ApplyLockToControls();
	UPROPERTY(Transient) TObjectPtr<UScrollBox>       TrackListScrollBox;
	UPROPERTY(Transient) TObjectPtr<UBorder>          TrackListPanel;   // 録音中に赤枠を出すトラック一覧パネル

	// --- トラック / マーカー 切替（一覧見出しの位置に置くセグメント選択）---
	UPROPERTY(Transient) TObjectPtr<class UHorizontalBox> TrackMarkerTabRow;   // 「トラック|マーカー」タブ行
	UPROPERTY(Transient) TObjectPtr<UButton>          TrackTabButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       TrackTabButtonText;
	UPROPERTY(Transient) TObjectPtr<UBorder>          TrackTabIndicator;   // 選択中に光る下線
	UPROPERTY(Transient) TObjectPtr<UButton>          MarkerTabButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MarkerTabButtonText;
	UPROPERTY(Transient) TObjectPtr<UBorder>          MarkerTabIndicator;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MarkerEmptyText;   // マーカーが無いときの案内
	UPROPERTY(Transient) TObjectPtr<UScrollBox>       MarkerListScrollBox;
	UPROPERTY(Transient) TArray<TObjectPtr<UWG_MarkerRow>> MarkerRows;
	TArray<FMarker> MarkerList;                       // 取得済みマーカー（StartTime昇順。Index=Set系APIで使うid）
	TWeakObjectPtr<UWG_MarkerRow> ActiveMarkerRow;    // 現在再生中（=カーソル位置）のマーカー行
	bool  bMarkerViewActive = false;                  // false=トラック表示 / true=マーカー表示

	// =========================================================
	//  ローカルIR（端末単体でTSP信号を鳴らすモード）
	//
	//  接続ボタンの下の「ローカルIR」でホーム画面がこのモードへ切り替わる。
	//   ・トラック/マーカーのタブ → ＋／－（再生回数の増減）
	//   ・一覧の黒い領域          → 回数／間隔の大きな数字＋間隔スライダー
	//   ・状態表示（■停止中）と削除/全解除/計測調整/Slack/ロック、ツールバーは隠す
	//   ・再生ボタンだけを残して大きくし、押すとTSPを指定回数くり返す
	//  モードを抜けるのは「ローカルIR」をもう一度押すか「接続」を押したとき。
	// =========================================================
	UPROPERTY(Transient) TObjectPtr<UButton>             LocalIRButton;          // 接続ボタンの下に置くモード切替
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          LocalIRButtonText;
	UPROPERTY(Transient) TObjectPtr<class UHorizontalBox> LocalIRCountRow;       // タブ行の代わりに出す ＋／－ の行
	UPROPERTY(Transient) TObjectPtr<UButton>             LocalIRPlusButton;      // 「トラック」の位置＝＋
	UPROPERTY(Transient) TObjectPtr<UButton>             LocalIRMinusButton;     // 「マーカー」の位置＝－
	UPROPERTY(Transient) TObjectPtr<UVerticalBox>        LocalIRCenterPanel;     // 黒い領域に出す回数/間隔の大表示
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          LocalIRCountLabel;      // 「回数」
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          LocalIRCountValueText;  // 回数の数字（大）
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          LocalIRIntervalLabel;   // 「間隔」
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          LocalIRIntervalValueText; // 間隔の数字（大）
	UPROPERTY(Transient) TObjectPtr<USlider>             LocalIRIntervalSlider;  // 間隔（0〜15秒・1秒刻み）
	UPROPERTY(Transient) TObjectPtr<class UProgressBar>  LocalIRIntervalFill;    // スライダーの塗り
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          LocalIRProgressText;    // 再生中の進捗（「2 / 5 回目」）
	UPROPERTY(Transient) TObjectPtr<class UOverlay>      FooterStateOverlay;     // ■停止中の行（ローカルIR中は隠す）

	bool  bLocalIRMode = false;          // ローカルIR画面を表示中か
	int32 LocalIRPlayCount = 1;          // TSPの再生回数（初回は1）
	int32 LocalIRIntervalSeconds = 5;    // TSPの再生間隔（秒。0〜15）

	// --- TSP再生（USoundWaveProceduralへ逐次キューして鳴らす）---
	UPROPERTY(Transient) TObjectPtr<class USoundWaveProcedural> LocalIRSoundWave;
	UPROPERTY(Transient) TObjectPtr<class UAudioComponent>      LocalIRAudioComponent;
	TArray<int16> LocalIRCyclePcm;       // 1回分（TSPスイープ＋間隔の無音）のPCM
	int32  LocalIRSampleRate = 48000;
	int32  LocalIRCycleCursor = 0;       // LocalIRCyclePcm のどこまでキューしたか
	int32  LocalIRCyclesLeft = 0;        // まだキューしていない回数
	bool   bLocalIRPlaying = false;
	double LocalIRPlayStartTime = 0.0;   // 進捗表示用（再生開始時刻）
	double LocalIRDrainStartTime = 0.0;  // キューを積み終えた時刻（鳴り終わり待ち）

	UFUNCTION() void HandleLocalIRClicked();
	UFUNCTION() void HandleLocalIRPlusClicked();
	UFUNCTION() void HandleLocalIRMinusClicked();
	UFUNCTION() void HandleLocalIRIntervalChanged(float NewValue);
	UFUNCTION() void HandleLocalIRIntervalCaptureEnd();
	void SetLocalIRMode(bool bOn);        // 画面をローカルIR⇄通常へ切り替える
	void ApplyLocalIRLayout(bool bLandscape);   // 隠すボタン・再生ボタンの大きさを現在のモード/向きに合わせる
	void UpdateLocalIRUI();               // 回数/間隔/進捗の表示を現在値に合わせる
	void StartLocalIRPlayback();          // TSPの再生を開始（指定回数くり返す）
	void StopLocalIRPlayback();           // 再生を止めて音声リソースを解放する
	void TickLocalIRPlayback();           // NativeTickから：PCMの補充と終了判定
	void BuildLocalIRCyclePcm();          // 1回分（スイープ＋無音）のPCMを作る

public:
	// マーカー行からの操作（行内の再生/停止トグル・名前編集）。
	void PlayFromMarkerRow(UWG_MarkerRow* Row, double StartSeconds);
	void StopMarkerPlayback();
	void RenameMarker(int32 MarkerId, const FString& NewName);
	// 名前欄タップ：マーカー名の編集ダイアログを開く。
	void OpenMarkerRename(int32 MarkerId, const FString& CurrentName);
private:
	// 再生中マーカー行の管理・トランスポート反映。
	void SetActiveMarkerRow(UWG_MarkerRow* Row);
	void NotifyTransportToMarkerRow();
	// トラック/マーカー切替
	UFUNCTION() void HandleTrackTabClicked();
	UFUNCTION() void HandleMarkerTabClicked();
	void SetMarkerViewActive(bool bActive);
	void UpdateTrackMarkerTabStyles();
	// マーカー一覧を実機から取得して MarkerList に反映し、行ウィジェットを作り直す。
	void FetchMarkers();
	void PopulateMarkerList();

	// --- 横画面対応：ホームのパネルを向きに応じて再配置（reparent）する ---
	// ホームの各パネルは1度だけ生成し、縦（HomePortraitCol）／横（HomeLandscapeRow）へ付け替える。
	UPROPERTY(Transient) TObjectPtr<UVerticalBox>     HomePage;             // switcher index0 の単一子ホルダ
	UPROPERTY(Transient) TObjectPtr<UBorder>          ActionPanel;          // 接続＋GTALK1/2
	UPROPERTY(Transient) TObjectPtr<UBorder>          DeviceInfoPanel;      // Hz/bit/ch 情報バー
	UPROPERTY(Transient) TObjectPtr<UBorder>          ToolbarPanel;         // 追加/モニター/メーター/更新＋アクション行

	// --- アクション（ツールバー下段：「アクション」ドロップダウン＋「実行」） ---
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ActionLabelText;
	UPROPERTY(Transient) TObjectPtr<UComboBoxString>  ActionCombo;          // 既定は未選択
	UPROPERTY(Transient) TObjectPtr<UButton>          ActionRunButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ActionRunButtonText;
	// ドロップダウンの並び順＝この配列の順（Actions/ のクラスを RegisterActions() で登録する）
	UPROPERTY(Transient) TArray<TObjectPtr<URadNodzAction>> Actions;
	// IR計測準備で作ったトラック・TSP の情報（IR測定・IR算出が使う。アプリ内メモリのみ）
	UPROPERTY(Transient) FIRMeasurementSession IRMeasurementSession;
	// アクション実行中（多重実行防止。完了時に EndActionBusy で戻す）
	bool bActionBusy = false;
	void RegisterActions();
	void RefreshActionCombo();          // 選択肢を現在の言語で作り直す（選択は維持）
	void UpdateActionRunButton();       // 未選択・実行中は「実行」を無効にする
	UFUNCTION() void HandleActionRunClicked();
	UFUNCTION() void HandleActionComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UPROPERTY(Transient) TObjectPtr<UBorder>          FooterPanel;          // 状態＋各種ボタン＋PLAY/REC
	UPROPERTY(Transient) TObjectPtr<UVerticalBox>     HomePortraitCol;      // 縦向き配置（従来のタテ積み）
	UPROPERTY(Transient) TObjectPtr<UHorizontalBox>   HomeLandscapeRow;     // 横向き配置（左操作列＋右トラック）
	UPROPERTY(Transient) TObjectPtr<UScrollBox>       HomeLandscapeLeftScroll;   // 横向き時の左操作列（縦スクロール）
	int32 HomeOrientationApplied = -1;   // -1=未適用 / 0=縦 / 1=横
	void ApplyHomeOrientation(bool bLandscape);   // ホームのパネルを向きに応じて再配置する
	UPROPERTY(Transient) TObjectPtr<class UImage>     StatusIcon;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       StatusText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ConnectButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       PlayButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       RecordButtonText;
	UPROPERTY(Transient) TObjectPtr<UWidget>          RecordButtonIcon;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       TransportStateText;
	UPROPERTY(Transient) TObjectPtr<class UImage>     RecStateDot;   // 「録音中」の左に出す赤丸（録音中のみ表示）

	// --- 下部タブ（ホーム / リスト / 設定）と画面切り替え ---
	UPROPERTY(Transient) TObjectPtr<UWidgetSwitcher>  ContentSwitcher;   // ホーム=0 / リスト=1 / 設定=2
	UPROPERTY(Transient) TObjectPtr<UButton>          HomeTabButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       HomeTabText;
	UPROPERTY(Transient) TObjectPtr<UBorder>          HomeTabIndicator;
	UPROPERTY(Transient) TObjectPtr<UButton>          ListTabButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ListTabText;
	UPROPERTY(Transient) TObjectPtr<UBorder>          ListTabIndicator;
	UPROPERTY(Transient) TObjectPtr<UButton>          SettingsTabButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SettingsTabText;
	UPROPERTY(Transient) TObjectPtr<UBorder>          SettingsTabIndicator;

	// --- 設定画面のテキスト（言語切替で更新する） ---
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SettingsTitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ConnSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       LocalIpRowLabel;

	// --- 認証設定（mTLS）---
	// 登録済みサーバー(サーバー名 → 接続先・期待FP)の管理と、鍵の申請・疎通確認。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          AuthSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UComboBoxString>     ServerIdCombo;      // 登録済みサーバーのドロップダウン
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          ServerFpText;       // 選択中サーバーの期待FP（表示のみ・全文表示）
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          AuthStatusText;     // 操作結果の表示
	UPROPERTY(Transient) TObjectPtr<UButton>             AddServerIdButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          AddServerIdButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>             DeleteServerIdButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          DeleteServerIdButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>             ShowOwnFpButton;    // 「この端末のFP」の「表示」ボタン
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          ShowOwnFpButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>             EnrollButton;       // 「申請」ボタン（公開鍵をサーバーへ申請）
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          EnrollButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>             TrialConnectButton; // 「確認」ボタン（疎通確認の試し接続）
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          TrialConnectButtonText;

	// serverId → 期待FP の登録一覧（SaveGame に永続化）。
	UPROPERTY() TArray<FServerAuthEntry> ServerAuthEntries;

	// 選択中の serverId（SaveGame に永続化し、次回起動時にドロップダウン選択を復元する）。
	FString SelectedServerId;

	// 認証操作用のクライアント（鍵生成・公開鍵取得に使用。遅延生成）。
	UPROPERTY(Transient) TObjectPtr<URadnodzClient> AuthClient;

	// 「認証」試し接続の実行中フラグ（二重実行防止）。
	bool bIsAuthenticating = false;

	// 実行中の「認証」試し接続クライアント（キャンセル時に Disconnect でブロッキング接続を中断する）。
	UPROPERTY(Transient) TObjectPtr<URadnodzClient> AuthInFlightClient;

	// 「確認」試し接続の世代番号。開始/キャンセルのたびに進め、古い非同期結果を無視するのに使う。
	int32 AuthGeneration = 0;

	// 「申請」(Enroll) の実行中フラグ（二重実行防止）。
	bool bIsEnrolling = false;

	// 「申請」の世代番号。古い非同期結果を無視するのに使う。
	int32 EnrollGeneration = 0;

	// 実行中の「認証」試し接続を停止（キャンセル）する。ボタン/ロゴ/ステータスを元へ戻す。
	void CancelTrialConnect();

	UFUNCTION() void HandleServerIdComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION() void HandleAddServerIdClicked();      // ポップアップを開く
	UFUNCTION() void HandleDeleteServerIdClicked();   // 確認ポップアップを開く
	UFUNCTION() void ConfirmDeleteServerId();         // 確認「はい」で実際に削除する
	// Reaper IP / ポート入力の確定：現在の選択 serverId に紐づけて保存する。
	UFUNCTION() void HandleReaperIpCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandlePortCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleShowOwnFpClicked();        // 「表示」: この端末のFPをポップアップで見せる
	UFUNCTION() void HandleEnrollClicked();           // 「申請」: 公開鍵をサーバーへ申請する
	UFUNCTION() void HandleTrialConnectClicked();     // 「確認」: 疎通確認の試し接続

public:
	/**
	 * ポップアップから新しいサーバーを登録する。
	 *
	 * 接続用URL（Bridge が QR / 文字列で配る rigdocks://server?...）を解析し、
	 * 接続先(host/port)・期待FP・ピン留めキー(nodeId)をまとめて1件追加する。
	 * 手入力の項目はサーバー名だけになる。
	 *
	 * @param Name     一覧に表示するサーバー名（重複不可）
	 * @param Url      接続用URL（必須）
	 * @param OutError 失敗理由（利用者向けの文言）
	 * @return 登録できたか
	 */
	bool AddServerFromPairingUrl(const FString& Name, const FString& Url, FString& OutError);

private:
	/**
	 * エントリの mTLS ピン留めキー。
	 * URL 登録なら nodeId、URL 登録より前の旧エントリ（nodeId 空）はサーバー名で代用する。
	 */
	static FString GetServerPinKey(const FServerAuthEntry& Entry);

	/** 「確認」試し接続の結果をゲームスレッドで反映する（ボタン/ロゴ/ステータス復帰）。 */
	void HandleAuthResult(bool bSuccess, const FRigdocksError& Error);

	/** 「申請」(Enroll) の結果をゲームスレッドで反映する（ボタン/ステータス復帰）。 */
	void HandleEnrollResult(ERadnodzEnrollStatus Status, const FRigdocksError& Error);

	/** 認証情報の保存先ディレクトリ（モバイルでも永続なサンドボックス配下）。 */
	FString GetAuthStorageDir() const;
	/** AuthClient を用意し保存先を設定する。 */
	void EnsureAuthClient();
	/** ServerIdCombo を ServerAuthEntries で再構築し、選択中の期待FPを ServerFpText に反映する。 */
	void RefreshServerIdCombo();

	/** 「サーバーFP」の表示を更新する（範囲外/未設定なら "-"）。 */
	void SetServerFpText(int32 EntryIndex);
	/** 現在ドロップダウンで選択されている serverId（無ければ空）。 */
	FString GetSelectedServerId() const;
	/** ServerAuthEntries から serverId の添字を返す（無ければ INDEX_NONE）。 */
	int32 FindServerAuthIndex(const FString& ServerId) const;
	/** 選択中 serverId に紐づく IP/ポートを接続先(IPAddress/Port＋入力欄)へ復元する（未紐付けなら現在値を維持）。 */
	void ApplySelectedServerConn();
	/** 現在の IPAddress/Port を選択中 serverId のエントリへ書き込む（選択なしなら何もしない）。 */
	void StoreConnToSelectedServer();
	/** 認証セクションのステータス表示を更新する。 */
	void SetAuthStatus(const FString& Msg);

	/** ポート入力欄から接続ポートを解決する。未記入/不正(0や範囲外)のときは既定の23760を返す。 */
	int32 ResolvePortFromInput() const;

	/** モニター受信/マイク送信が使うReaStream既定ポート（固定値）。設定画面には参照用に表示のみ（編集不可）。 */
	static constexpr int32 ReaStreamPort = 58710;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ReaStreamPortRowLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ReaStreamPortText;

	/** 設定：マイク送信の方式（ReaStream / PARAKNOT）切替。 */
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MicTransportRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MicTransportButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MicTransportButtonText;

	/** グループトーク用ポート番号のベース値（固定値）。モニターとは別ポートにすることで、
	 *  グループトーク使用中でもモニター受信を同時に使えるようにしている。
	 *  GroupTalkPort0/1 はこの値から連番で決まる。 */
	static constexpr int32 IntercomPort = ReaStreamPort + 1;

	UPROPERTY(Transient) TObjectPtr<UTextBlock>       IntercomSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       AppSectionLabel;
	// 「アプリ」に混在していた項目を用途ごとに分けたセクション見出し
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       CaptureSectionLabel;     // 画面キャプチャ
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       GroupTalkSectionLabel;   // グループ通話
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorSectionLabel;     // モニター
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       LanguageRowLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MicSettingsRowLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MicInputRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MicInputButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MicInputButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorOutputRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MonitorOutputButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorOutputButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorBufferRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MonitorBufferButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorBufferButtonText;
	// モニター受信音量（dB。端末側で受信音を±12dBの範囲で増減。スライダー）
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorVolumeRowLabel;
	UPROPERTY(Transient) TObjectPtr<class USlider>    MonitorVolumeSlider;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorVolumeValueText;

	// 各スライダーの「塗り」表示（左＝ティール塗り／右＝暗いトラックの二層表示に使う ProgressBar）。
	UPROPERTY(Transient) TObjectPtr<class UProgressBar> MonitorVolumeFill;
	UPROPERTY(Transient) TObjectPtr<class UProgressBar> MarkerOffsetFill;
	UPROPERTY(Transient) TObjectPtr<class UProgressBar> GroupTalkVolumeFill;
	UPROPERTY(Transient) TObjectPtr<class UProgressBar> GroupTalkSendVolumeFill;

	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorSourceRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MonitorSourceButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorSourceButtonText;

	// 配信元(PARAKNOT インスタンス)の選択。PARAKNOT 方式のときだけ意味がある。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorInstanceRowLabel;
	UPROPERTY(Transient) TObjectPtr<UComboBoxString>  MonitorInstanceCombo;
	UPROPERTY(Transient) TObjectPtr<UButton>          MonitorInstanceRefreshButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MonitorInstanceRefreshButtonText;

	// 各スライダーの現在値から塗り(ProgressBar)のパーセントを更新する。
	void RefreshSliderFills();

	// --- ダウンミックス係数の編集UI（ステレオダウンミックス時のみ表示）---
	// 受信ch（標準レイアウト L R C LFE Ls Rs Lb Rb）ごとに係数を一覧入力する。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>               DownmixSectionLabel;
	UPROPERTY(Transient) TArray<TObjectPtr<UTextBlock>>       DmCoeffRowLabels;
	UPROPERTY(Transient) TArray<TObjectPtr<UEditableTextBox>> DmCoeffInputs;
	// ステレオダウンミックス時のみ表示する行
	UPROPERTY(Transient) TArray<TObjectPtr<UWidget>>  StereoOnlyWidgets;

	/** 受信バッファ（遅延）レベル（0=低遅延/1=標準/2=安定）。端末に保存される。 */
	int32 MonitorBufferLevel = 1;

	/** ダウンミックス係数（受信ch・標準レイアウト L R C LFE Ls Rs Lb Rb ごと）。端末に保存される。 */
	TArray<float> DmGains;

	/** ダウンミックス対象chの数（標準レイアウト最大）。 */
	static constexpr int32 NumDownmixChannels = 8;

	// --- 立体音響(バイノーラル)の調整UI ---
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SpatialSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SpatialItdRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          SpatialItdButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SpatialItdButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SpatialShadowRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          SpatialShadowButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SpatialShadowButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SpatialOrderRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          SpatialOrderButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SpatialOrderButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SpatialHrtfRowLabel;     // HRTFプロファイル選択
	UPROPERTY(Transient) TObjectPtr<UComboBoxString>  HrtfCombo;               // 簡易(内蔵)＋登録プロファイルから選択

	// --- ライセンス/クレジット表示（HRTFデータ同梱時に表示） ---
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       LicenseSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       LicenseText;

	// --- カスタムch配置エディタ（角度・距離＋図） ---
	UPROPERTY(Transient) TObjectPtr<class UCanvasPanel> MappingCanvas;
	UPROPERTY(Transient) TArray<TObjectPtr<UBorder>>    MappingDots;
	UPROPERTY(Transient) TArray<TObjectPtr<UTextBlock>> MappingDotLabels;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MapEditChRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MapEditChButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MapEditChButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MapAngleRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MapAngleMinusButton;
	UPROPERTY(Transient) TObjectPtr<UButton>          MapAnglePlusButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MapAngleValueText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MapDistRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MapDistMinusButton;
	UPROPERTY(Transient) TObjectPtr<UButton>          MapDistPlusButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MapDistValueText;

	/** 立体音響パラメータ（レベル 0=オフ/1=弱/2=中/3=強, ch順）。端末に保存される。 */
	int32 SpatialItdLevel = 2;
	int32 SpatialShadowLevel = 2;
	EBinauralChannelOrder SpatialChannelOrder = EBinauralChannelOrder::ReaperITU;

	/** 選択中のHRTFプロファイル名（空＝内蔵の簡易バイノーラル）。端末に保存される。 */
	FString HrtfProfile;

	/** カスタムch配置（最大8ch分の角度[度]・距離[0.2〜2.0]）。端末に保存される。 */
	TArray<float> CustomAzimuths;
	TArray<float> CustomDistances;

	// 立体音響モードのときだけ表示する設定ウィジェット群（見出し・各行）
	UPROPERTY(Transient) TArray<TObjectPtr<UWidget>> SpatialOnlyWidgets;
	// 立体音響モード かつ ch順=カスタム のときだけ表示するウィジェット群（配置図・編集ch・角度・距離）
	UPROPERTY(Transient) TArray<TObjectPtr<UWidget>> CustomOnlyWidgets;
	// モニター出力=立体音響（さらにカスタム配置エディタはch順=カスタム）のときのみ上記を表示する
	void UpdateSpatialSettingsVisibility();
	/** カスタム配置エディタで編集中のch（0始まり）。 */
	int32 EditingChannel = 0;
	/** 編集対象として表示するch数。 */
	static constexpr int32 MaxMappingChannels = 8;

	// --- 表示名・在席登録（グループトークの参加者名簿・自分除外判定に使う）---
	// 仕組み：サーバ上の共有フォルダに「<名前>.grouptalk1/2.txt」等をハートビート書き込みし、
	// グループトーク側がそれを読んで参加者を表示する（詳細はグループトークのセクション参照）。
	FString IntercomUserName;            // 自分の表示名（端末に保存）
	FString PresenceFolder;              // 在席名簿フォルダ（手動指定・任意。空なら自動解決したフォルダを使用）
	FString ResolvedPresenceFolder;      // 接続時に解決した <Bridgeフォルダの親>/RadNodzToolkit/GroupTalk の絶対パス（自動）

	// 実際に使う名簿フォルダ（自動解決を優先、無ければ手動指定）
	FString GetEffectivePresenceFolder() const { return !ResolvedPresenceFolder.IsEmpty() ? ResolvedPresenceFolder : PresenceFolder; }
	// 接続時に、接続先 PC の RIGDOCKS Bridge フォルダと同じ階層にある
	// RadNodzToolkit/GroupTalk（絶対パス）を解決して保持する。REAPER には依存しない。
	// 同期 RPC は ControlQueue で実行するので、ゲームスレッドから呼んでよい。
	void ResolveGroupTalkFolder();
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MemberUserNameRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> MemberUserNameInput;

	void MarkPresenceOffline();             // 切断直前に、グループトークの在席ファイルを即座にオフライン扱いにする
	// 在席オフライン書き込みの内容だけを組み立てる（通信しない。ゲームスレッド専用）。
	// 切断時に「書き込み→切断」をワーカーで順に実行するために使う。
	bool BuildPresenceOfflineWrite(FString& OutFolder, FString& OutFile1,
		FString& OutFile2, FString& OutContent) const;
	UFUNCTION() void HandleMemberUserNameCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	// --- グループトーク（チーム共通の放送チャンネルを2つ持つ。ローカルにマスター/サブを切替） ---
	// 仕組み：メンバー通話と同じ在席共有フォルダに「<名前>.grouptalk1.txt」/「<名前>.grouptalk2.txt」で
	// 各チャンネルの参加状況を書き込み、それを読んだ参加者同士が互いのIPを知って ReaStream 形式で
	// P2P送受信する（Reaperは経由しない）。受信ONの全員が「そのchで今話している人」の声を聞く放送型。
	// マスターは「自分の2chのどちらを主にするか」というローカル概念（他ユーザーとは無関係）。
	// 送信は排他：マスターはタップでラッチON/OFF、サブは押している間だけ送信（PTT）。マイクは同時に1chのみ。
	static constexpr int32  GroupTalkPort0 = IntercomPort + 1;   // 58712（トーク1）
	static constexpr int32  GroupTalkPort1 = IntercomPort + 2;   // 58713（トーク2）
	static constexpr double GroupTalkLongPressSec = 0.6;         // 受信アイコン長押し判定（マスター切替）

	UPROPERTY(Transient) TObjectPtr<UButton> GroupTalkRecvButton[2];   // 受信1 / 受信2
	UPROPERTY(Transient) TObjectPtr<UButton> GroupTalkSendButton[2];   // 送信1 / 送信2
	// チャンネル枠（□）本体。送信中は指でボタンが隠れても分かるよう枠の色も変える（UpdateGroupTalkUI）。
	UPROPERTY(Transient) TObjectPtr<class UBorder> GroupTalkFrame[2];
	// 参加者を表す〇（頭文字入り）を並べる行。参加者が変わるたび UpdateGroupTalkUI() が作り直す。
	UPROPERTY(Transient) TObjectPtr<UHorizontalBox> GroupTalkAvatarRow[2];
	// 送受信アクティビティの〇（声が流れている間だけ緑に点滅）。各ボタンの右下に重ねて表示。
	UPROPERTY(Transient) TObjectPtr<class UImage> GroupTalkRecvDot[2];
	UPROPERTY(Transient) TObjectPtr<class UImage> GroupTalkSendDot[2];
	// 見出し（受信/送信）。ApplyLanguageで再適用するため保持する（保持しないと言語切替で更新されない）。
	UPROPERTY(Transient) TObjectPtr<UTextBlock> GroupTalkRecvCaption[2];
	UPROPERTY(Transient) TObjectPtr<UTextBlock> GroupTalkSendCaption[2];

	// 送信は排他のため単一センダー（宛先ポートを実行時に切替）。受信はch毎に1個。
	UPROPERTY(Transient) TObjectPtr<UReaStreamSender>        GroupTalkSender;
	UPROPERTY(Transient) TObjectPtr<UReaStreamGroupReceiver> GroupTalkReceiver[2];

	bool  bGroupTalkRecvOn[2] = { false, false };   // 各chの受信ON/OFF
	int32 GroupTalkMasterCh   = -1;                 // -1=未設定 / 0 / 1
	bool  bGroupTalkMasterLatch = false;            // マスターchの送信ラッチ
	bool  bGroupTalkSubHeld     = false;            // サブchの送信を長押し中（PTT）

	double GroupTalkRecvPressStart[2] = { 0.0, 0.0 };   // 受信ボタン押下時刻（長押し判定用）
	bool   bGroupTalkRecvPressing[2]  = { false, false };

	double GroupTalkHeartbeatTimer = 0.0;   // 参加状況を書き込む間隔タイマー
	double GroupTalkRosterTimer    = 0.0;   // 参加者一覧を読み直す間隔タイマー
	double GroupTalkPruneTimer     = 0.0;   // 無音になった参加者の再生を掃除する間隔タイマー
	TMap<FString, FString> GroupTalkMembers[2];   // ch毎：名前 → IP（自分以外の現在の参加者）
	float GroupTalkPlaybackGain = 1.0f;           // 受信音の再生ゲイン倍率（設定・保存対象。既定1.0=素通し）
	// 送信ゲイン倍率（送る前にマイク信号へ掛ける）。0.5刻み・最大5.0で、UIの目盛り0〜10に対応。
	// 既定1.5＝目盛り3。
	// （高すぎると送信時にハードクリップして音が割れる）
	float GroupTalkSendGain = 1.5f;

	// --- バックグラウンド継続（画面OFF/他アプリ表示でも音声・通話を続ける）---
	// モニター/マイク/グループトークの動作状態に応じてキープアライブ
	// （Android前面サービス＋監視スレッド）を開始/停止する。
	void UpdateKeepAlive();
	// バックグラウンド中の定期メンテナンス（★監視スレッドから呼ばれる★UI操作禁止）。
	// グループトークの在席ハートビート書き込みと参加者名簿の更新を行う。
	void BackgroundKeepAliveMaintenance();
	// RefreshGroupTalkRoster のバックグラウンド版（UI更新・送信開始を行わない）。
	void RefreshGroupTalkRosterBackground();
	// ボタン操作起因の在席ハートビート/名簿RPCを「ワーカー＋ClientRpcLock」で実行する。
	// ゲームスレッドで同期RPCを呼ぶとソケット衝突＋ブロックでフリーズ(ANR)するため、
	// 定期ワーカーと同じ経路に合流させる。完了後にUI更新だけGTへ戻す。
	void DispatchGroupTalkPresenceAsync(bool bHeartbeat, bool bRoster);
	// バックグラウンドメンテナンスの最終実行時刻（監視スレッドで読み書き）
	TAtomic<double> GtBgHeartbeatLast { 0.0 };
	TAtomic<double> GtBgRosterLast { 0.0 };
	// バックグラウンド中の接続キープ（軽量RPC往復）の最終実行時刻と間隔。
	// サーバー側タイムアウト前に必ず1往復するよう、余裕をもって短めにする。
	TAtomic<double> BgConnKeepLast { 0.0 };
	static constexpr double BgConnKeepIntervalSec = 10.0;

	// REAPERへの同期RPCは電波が悪いと数十秒ブロックする。ゲームスレッド上で呼ぶとUIが
	// フリーズするため、以下で「同期RPCの直列化」と「ゲームスレッドを絶対にブロックさせない」を実現する：
	//   ・全Client RPCは ClientRpcLock で直列化（同一ソケットの同時使用による通信破損を防ぐ）。
	//   ・ワーカースレッド側は Lock（待ってよい）、ゲームスレッド側は TryLock（取れなければその周期はスキップ）。
	// これによりワーカーのRPCがブロック中でもゲームスレッドは止まらない。
	FCriticalSection ClientRpcLock;
	// グループトークの定期RPC（ハートビート/名簿）をワーカーで実行中か（多重起動防止）。
	TAtomic<bool> bGtRpcInFlight { false };

	// ================================================================
	//  RPC の実行スレッド（同期RPCでゲームスレッドを止めないための集約先）
	// ================================================================
	//
	// RPCは「送って応答を待つ」同期通信なので、ゲームスレッドで呼ぶと
	// 電波が悪いときにそのままUIが固まる（Androidでは5秒でANR）。
	// 定期処理・一覧取得・重い解析・値を返さない操作は、すべて下記の
	// キューへ積んでワーカースレッドで実行し、UI更新だけGTへ戻す。
	//
	// キューを2本に分けているのは接続を分けるため。ラウドネス解析は1回で
	// 数十秒かかるので、同じ接続に載せると操作系が全部その後ろに並ぶ。
	TUniquePtr<FRadnodzRpcQueue> ControlQueue;   // 操作・一覧・メーター・心拍
	TUniquePtr<FRadnodzRpcQueue> HeavyQueue;     // ラウドネス計測・メディア先読み

	// 重い解析専用の2本目の接続。制御接続と同じホストへ別途つなぐ。
	// 張れなかった場合は制御接続へフォールバックする（機能は止めない）。
	UPROPERTY(Transient)
	TObjectPtr<URadnodzClient> HeavyClient;

	// HeavyClient が使える状態か（ワーカーから読むのでアトミック）。
	TAtomic<bool> bHeavyClientReady { false };

	// キューの生成・破棄
	void EnsureRpcQueues();
	void ShutdownRpcQueues();

	// RPCジョブの投入。Key を付けると同じ用途の未実行ジョブが捨てられる
	// （回線が遅いときにジョブが溜まり続けるのを防ぐ）。
	// 捨てられた場合は false を返す。
	bool EnqueueControlRpc(FName Key, TFunction<void()> Job);
	bool EnqueueHeavyRpc(FName Key, TFunction<void()> Job);

	// 解析に使うクライアント。2本目が無ければ制御接続を返す。
	URadnodzClient* GetAnalysisClient() const;

	// 2本目の接続を非同期で確立する / 落とす。
	void StartHeavyClientConnect();
	void ShutdownHeavyClient();

	// ワーカーで実行中のRPCが使う UObject を GC から守るための保持枠。
	// ゲームスレッドで登録し、完了時にゲームスレッドで解放する。
	UPROPERTY(Transient)
	TMap<int32, FRadnodzRpcRefSet> RpcRefsInFlight;

	int32 NextRpcRefToken = 1;

	int32 HoldRpcObjects(const TArray<UObject*>& Objects);
	void  ReleaseRpcObjects(int32 Token);

	/**
	 * 値を返さない単発の操作RPCを、ゲームスレッドを止めずに投げる。
	 *
	 * UI・ローカル状態は呼び出し側で先に更新してよい（楽観的更新。失敗しても
	 * 次の一覧取得で実機の状態に揃う）。
	 *
	 * @param KeepAlive Body が使う一時 UObject（UReaProject / UMediaItem 等）。
	 *                  これらは呼び出しのたびに作られ誰も参照していないため、
	 *                  渡さないとワーカーが使う前にGCで回収される。
	 */
	void FireAndForgetRpc(const TArray<UObject*>& KeepAlive, TFunction<void(URadnodzClient*)> Body);

	/**
	 * バッチをワーカーで実行し、完了後にゲームスレッドで OnDone を呼ぶ。
	 *
	 * **Build と Parse はゲームスレッドで行うこと。** ワーカーがするのは
	 * Execute（通信）だけ。Parse は UObject を作るものがあり、UObject の
	 * 生成はゲームスレッド専用のため。
	 *
	 * @param Batch     実行するバッチ（ゲームスレッドで組み立て済み）
	 * @param KeepAlive Execute 中にGCから守る UObject（Batch と各 RpcCall）
	 * @param Key       同じ用途の未実行ジョブがあれば捨てる（NAME_None で無効）
	 * @param bHeavy    true なら解析用キュー（2本目の接続）で実行する
	 * @return 積めたら true
	 */
	bool ExecuteBatchAsync(URadnodzRpcBatch* Batch, const TArray<UObject*>& KeepAlive,
		FName Key, bool bHeavy, TFunction<void()> OnDoneGameThread);

	/**
	 * GetTrackMediaItems の非同期版（通信はワーカー、UObject生成はゲームスレッド）。
	 * OnDone は必ずゲームスレッドで呼ばれる（失敗時は空配列）。
	 */
	void FetchTrackMediaItemsAsync(int32 TrackIndex, bool bHeavy, FName Key,
		TFunction<void(const TArray<UMediaItem*>&, const TArray<FString>&, const TArray<double>&)> OnDone);

	/**
	 * FetchTracks の本体。取得が終わったら OnDone をゲームスレッドで呼ぶ。
	 * 一覧を取り直してから続けたい処理（トラック追加後の特定など）で使う。
	 */
	void FetchTracksAsync(TFunction<void()> OnDone);

	// FetchTracksAsync の2往復目（各トラックのソロ/ミュート/REC）。
	void FetchTrackStatesAsync(TFunction<void()> OnDone);

	// 一覧が揃ったあとのUI組み立て（必ずゲームスレッド）。
	void FinishFetchTracks();

	// 各ジョブの多重起動防止（ワーカーが実行中はゲームスレッドで次を積まない）。
	bool bMeterRpcInFlight = false;
	bool bWarmRpcInFlight = false;
	bool bTracksRpcInFlight = false;
	bool bMeasureRpcInFlight = false;
	bool bProbeRpcInFlight = false;
	bool bScriptMatchRpcInFlight = false;
	bool bHealthProbeInFlight = false;

	// 直近の往復時間（ミリ秒）。回線が悪いときにポーリング間隔を自動で
	// 伸ばすために使う。MeasureCommTime のジョブが更新する。
	TAtomic<double> LastRoundTripMs { 0.0 };

	// 直近RTTから決まるポーリング間隔の倍率（1.0〜3.0）。
	double GetPollingScale() const;

	// --- 画面録画の状態機械 ---
	enum class EScreenRecState : uint8
	{
		Idle,               // 未録画
		WaitingPermission,  // MediaProjection許可ダイアログの結果待ち
		CountingDown,       // 3・2・1カウントダウン中
		Recording,          // 録画中
	};
	EScreenRecState ScreenRecState = EScreenRecState::Idle;
	double ScreenRecCountdownRemain = 0.0;   // カウントダウン残り秒
	int32  ScreenRecShownNumber     = 0;     // 現在オーバーレイに表示中の数字（更新検知用）
	static constexpr double ScreenRecCountdownSec = 3.0;
	// 録画開始直後の猶予：前面サービスが起動して IsRecording()=true を立てるまで
	// 数百msかかるため、この間に IsRecording() を見ると即 Idle に戻ってしまう。
	// Recording 遷移からこの秒数は外部停止判定を行わない。
	double ScreenRecStartGrace      = 0.0;   // 経過秒（Recording遷移からの積算）
	static constexpr double ScreenRecStartGraceSec = 2.5;
	bool   bScreenRecOverlayInViewport = false;   // カウントダウンオーバーレイをビューポートに追加済みか
	TSharedPtr<SWidget> ScreenRecOverlaySlate;    // 追加/削除に使うSlateウィジェット参照
	// 録画中に経過秒＋停止ボタンのオーバーレイ（右上）を出すか（既定=表示）。
	// OFFのときは録画中も出さず、停止は設定の「録画停止」ボタンで行う。
	bool   bScreenRecShowOverlay = true;
	// 録画にマイク音を含めるか（既定=含めない＝モニター音のみ録音）。
	bool   bScreenRecIncludeMic = false;

	FORCEINLINE int32 GroupTalkSubCh() const { return (GroupTalkMasterCh == 0) ? 1 : (GroupTalkMasterCh == 1 ? 0 : -1); }
	FORCEINLINE int32 GroupTalkPortForCh(int32 Ch) const { return (Ch == 0) ? GroupTalkPort0 : GroupTalkPort1; }

	// 各ボタンの無引数UFUNCTIONサンク（OnPressed/OnReleasedは無引数マルチキャストのためch毎に必要）
	UFUNCTION() void HandleGroupTalkRecv0Pressed();
	UFUNCTION() void HandleGroupTalkRecv0Released();
	UFUNCTION() void HandleGroupTalkRecv1Pressed();
	UFUNCTION() void HandleGroupTalkRecv1Released();
	UFUNCTION() void HandleGroupTalkSend0Clicked();
	UFUNCTION() void HandleGroupTalkSend1Clicked();
	UFUNCTION() void HandleGroupTalkSend0Pressed();
	UFUNCTION() void HandleGroupTalkSend0Released();
	UFUNCTION() void HandleGroupTalkSend1Pressed();
	UFUNCTION() void HandleGroupTalkSend1Released();

	void  GroupTalkOnRecvTap(int32 Ch);        // 受信タップ：受信ON/OFF
	void  GroupTalkOnRecvLongPress(int32 Ch);  // 受信長押し：自分のマスターchに設定
	void  GroupTalkOnSendClicked(int32 Ch);    // 送信タップ：マスターchならラッチ反転
	void  GroupTalkOnSendPressed(int32 Ch);    // 送信押下：サブchならPTT開始
	void  GroupTalkOnSendReleased(int32 Ch);   // 送信離し：サブchならPTT終了
	int32 ComputeTransmitTarget() const;       // 現在の送信先ch（0/1）、無送信なら-1
	void  ApplyTransmitRouting();              // 送信先をComputeTransmitTargetに合わせて更新
	void  StartGroupTalkReceiver(int32 Ch);
	void  StopGroupTalkReceiver(int32 Ch);
	void  EnsureGroupTalkSenderStarted();
	void  WriteGroupTalkHeartbeat(int32 Ch);   // 該当chの参加状況をサーバへ書き込む
	void  RefreshGroupTalkRoster();            // 両chの参加者一覧を読んで送受信先を更新する
	void  UpdateGroupTalkUI();                 // 4ボタンの色/マスター表示を更新
	bool  IsGroupTalkChannelActive(int32 Ch) const;   // 受信ON または そのchへ送信中

	// --- Slack共有の設定UI（設定画面） ---
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SlackSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SlackTokenRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> SlackTokenInput;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       SlackToUserRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> SlackToUserInput;

	// --- ラウドネス/音量調整の設定UI（設定画面） ---
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       LoudnessSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       TargetLufsRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> TargetLufsInput;
	// グループトーク受信音量（再生メイクアップゲイン倍率）のスライダー（0〜10、1刻み）＋数値表示。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       GroupTalkVolumeRowLabel;
	UPROPERTY(Transient) TObjectPtr<USlider>          GroupTalkVolumeSlider;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       GroupTalkVolumeValueText;
	// グループトーク送信音量（送信ゲイン倍率）のスライダー（0〜10、1刻み）＋数値表示。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       GroupTalkSendVolumeRowLabel;
	UPROPERTY(Transient) TObjectPtr<USlider>          GroupTalkSendVolumeSlider;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       GroupTalkSendVolumeValueText;
	// マイク送信チェック（送信ゲインを掛けた音を自分へ折り返し再生）ボタン。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MicCheckRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MicCheckButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MicCheckButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       LimitPeakRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          LimitPeakButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       LimitPeakButtonText;

	// --- メーターの設定UI（設定画面） ---
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MeterSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MeterIntervalRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> MeterIntervalInput;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MeterModeRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MeterModeButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MeterModeButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MeterDotScopeRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          MeterDotScopeButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MeterDotScopeButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MeterDescText;          // メーター設定の簡易説明

	// --- トラック一覧の更新方法（自動更新 / ボタン手動更新）の設定UI（設定画面） ---
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       TrackRefreshSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       TrackRefreshModeRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          TrackRefreshModeButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       TrackRefreshModeButtonText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       TrackRefreshIntervalRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> TrackRefreshIntervalInput;
	UFUNCTION() void HandleTrackRefreshModeClicked();
	void UpdateTrackRefreshModeButton();
	UFUNCTION() void HandleTrackRefreshIntervalCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	void TickTrackListAutoRefresh(float DeltaSeconds);
	void UpdateUpdateButton();   // ホーム画面の「更新」ボタン（自動更新モード中はトグルスイッチとして機能する）

	/** トラック一覧を自動更新するか（false＝「更新」ボタンでの手動更新）。端末に保存される。 */
	bool bTrackListAutoRefresh = true;
	/** 自動更新モード中、実際に自動更新が動作中か（グレー=停止中／赤=動作中）。再起動時は必ず停止状態に戻る。 */
	bool bTrackListAutoRefreshRunning = true;
	/** 自動更新の間隔（秒）。端末に保存される。 */
	int32 TrackRefreshIntervalSec = 3;
	/** 自動更新の経過時間の積算用タイマー。 */
	double TrackRefreshTimer = 0.0;

	// --- テーマ（背景の明るさ）切替UI（設定画面） ---
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ThemeRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          ThemeButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ThemeButtonText;

	/** UIテーマ（背景の明るさ）。0=標準（黒） / 1=さらに暗め / 2=明るめ。端末に保存される。 */
	int32 UIThemeIndex = 0;

	// テーマ切替時に地色を貼り替えるため、背景ボーダーを役割ごとに保持する
	UPROPERTY(Transient) TArray<TObjectPtr<UBorder>>  ThemeScreenBgBorders;
	UPROPERTY(Transient) TArray<TObjectPtr<UBorder>>  ThemeHeaderBgBorders;
	UPROPERTY(Transient) TArray<TObjectPtr<UBorder>>  ThemePanelBgBorders;
	UPROPERTY(Transient) TArray<TObjectPtr<UBorder>>  ThemeListBgBorders;

	// --- 画面の向き固定切替UI（設定画面） ---
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       OrientationRowLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          OrientationButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       OrientationButtonText;

	// マーカーオフセット（アプリ欄。押した瞬間から前後何秒にマーカーを置くか。スライダー±10秒）
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MarkerOffsetRowLabel;
	UPROPERTY(Transient) TObjectPtr<class USlider>    MarkerOffsetSlider;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MarkerOffsetValueText;

	/** 画面の向き固定。0=自動 / 1=縦 / 2=横。端末に保存される。 */
	int32 ScreenOrientationLock = 0;

	/** モニター受信音量（dB。端末側でReaperからの受信音を増減。既定+5dB、範囲±12dB）。端末に保存される。 */
	float MonitorVolumeDb = 5.0f;

	/** マーカー設定時のオフセット秒（既定-1秒、範囲±10秒）。端末に保存される。 */
	int32 MarkerOffsetSeconds = -1;

	/** 過去に入力したマーカー名の履歴（新しい順）。マーカー名ダイアログで再利用する。端末に保存される。 */
	TArray<FString> MarkerNameHistory;

	/** マーカー名履歴の最大保持件数。 */
	static constexpr int32 MaxMarkerNameHistory = 12;

	/** メーターの更新間隔（ミリ秒）。端末に保存される。 */
	// メーター取得の周期(ms)。実際の周期は直近RTTに応じて自動で伸びる。
	int32 MeterIntervalMs = 300;
	/** メーター表示を「信号インジケータ(〇)」にするか（false＝フルメーター）。端末に保存される。 */
	bool  bMeterSignalIndicator = false;
	/** 信号〇を「トラックに1つ」にするか（true＝1つ／false＝chごと）。端末に保存される。 */
	bool  bMeterDotPerTrack = false;

	/** Slack共有に使うBotトークン（xoxb-...）。端末に保存される。 */
	FString SlackToken;
	/** Slack共有先（DM宛先ユーザー名 または ユーザーID）。端末に保存される。 */
	FString SlackToUser;

	/** 計測ジョブ完了後にSlackへ共有するか（「Slack共有」ボタン経由）。 */
	bool bShareToSlackAfterMeasure = false;

	/** マイク送信に使う入力経路（本体マイク / Bluetoothマイク）。端末に保存される。 */
	EMicInputMode MicInputMode = EMicInputMode::Builtin;

	/**
	 * マイク送信の方式。true=PARAKNOT（暗号化・押し込み） / false=ReaStream（従来）。
	 * 端末に保存される。**既定は PARAKNOT。**
	 */
	bool bUseParaknotMic = true;

	/**
	 * PARAKNOT 経路で今から張ろうとしているセッションの用途。
	 *
	 * 一覧の取得 → 選択 → 役割の確認 → セッション発行 という流れは
	 * マイク送信とモニター受信で同じで、**求める相手のモードと発行する
	 * セッションだけが逆**なので、どちらの用途かをここで覚えておく。
	 */
	enum class EParaknotPurpose : uint8
	{
		Mic,       // 端末のマイク → PARAKNOT（押し込み。相手は受信・待受）
		Monitor    // PARAKNOT → 端末（購読。相手は送信・待受）
	};

	EParaknotPurpose ParaknotPurpose = EParaknotPurpose::Mic;

	/**
	 * 相手に選んだ PARAKNOT の instanceId と表示名。
	 *
	 * **保存しない。** instanceId は PARAKNOT を起動し直すと変わるため、
	 * 起動をまたいで使い回せない（毎回一覧から選び直す）。
	 */
	FString ParaknotMicInstanceId;
	FString ParaknotMicLabel;

	/** 一覧取得〜セッション発行の往復中か（ボタンに「準備中」を出すため）。 */
	bool bParaknotMicStarting = false;

	/** 選択ダイアログに出している候補（ゲームスレッド専有）。 */
	TArray<FParaknotInstance> ParaknotCandidates;

	/** モニター受信音声の出力方法（ステレオ / 立体音響 / そのまま）。端末に保存される。 */
	EMonitorOutputMode MonitorOutputMode = EMonitorOutputMode::Stereo;

	/**
	 * REAPER の再生音をどの経路で受け取るか。端末に保存される。
	 *
	 * 既定は PARAKNOT(暗号化・鍵は RADNODZ 経由)。ReaStream は従来方式で、
	 * PARAKNOT が使えないときに設定タブから戻せるよう残してある。
	 */
	EMonitorSource MonitorSource = EMonitorSource::Paraknot;

	/**
	 * モニターで聞く配信元の**既定**。待受ポートで覚える。0 = 毎回選ぶ。
	 *
	 * **InstanceId では覚えない。** あれはプラグインを挿し直すたびに乱数で
	 * 採り直されるので、保存しても次回必ず外れる。ポートは利用者が決める値で、
	 * プロジェクトを開き直しても変わらない（セッション鍵の宛先もポート単位）。
	 *
	 * 入っていればモニター開始時に選択ポップアップを出さずにこれを使う。
	 * そのポートで待っているものが居なければ、黙って別のものにせず
	 * ポップアップへ回す。
	 */
	int32 MonitorListenPort = 0;

	/** 配信元の候補（更新ボタンで取り直す）。表示順と ComboBox の並びは一致させる。 */
	TArray<FParaknotInstance> MonitorInstances;

	/** 現在表示中のタブ（0=ホーム / 1=設定）。 */
	int32 ActiveTab = 0;

	/** 再生中フラグ（PLAY/STOP切り替え用）。実機のGetPlayStateで更新時に取得し直す。 */
	bool bIsPlaying = false;

	/** 一時停止中フラグ。実機のGetPauseStateで更新時に取得し直す。 */
	bool bIsPaused = false;

	/** 停止中フラグ。実機のGetStopStateで更新時に取得し直す。 */
	bool bIsStopped = true;

	// 接続ステータスアイコン：接続中や台本一致の文字起こし中は縦軸まわりに回転（左右フリップ風）させる
	// （進行中インジケータとして共通利用。専用アイコンを別途持たない）
	bool  bStatusIconSpin = false;
	float StatusIconSpinPhase = 0.f;

	// ステータスアイコンのテクスチャを差し替える
	void SetStatusIconTexture(const TCHAR* TexturePath);

	/** 画面上に並んでいるトラックカード。 */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UWG_TrackCard>> TrackCards;

	/** ReaStream音声モニター受信機。 */
	UPROPERTY(Transient)
	TObjectPtr<UReaStreamReceiver> AudioMonitor;

	/** マイク音声のReaStream送信機。 */
	UPROPERTY(Transient)
	TObjectPtr<UReaStreamSender> MicSender;

	/**
	 * マイク音声のPARAKNOT送信機（押し込み送信）。
	 *
	 * ReaStream との違いは相手と経路で、送るのは同じマイク。
	 * どちらを使うかは設定（bUseParaknotMic）で切り替える。
	 */
	UPROPERTY(Transient)
	TObjectPtr<UParaknotSender> RigMicSender;

	/**
	 * PARAKNOT の配信を受け取るモニター（購読）。
	 *
	 * ReaStream のモニター（AudioMonitor）と同じ役目で、相手と経路だけが違う。
	 * どちらを使うかは設定（bUseParaknotMic）で切り替える。
	 */
	UPROPERTY(Transient)
	TObjectPtr<UParaknotReceiver> ParaknotMonitor;

private:

	void BuildUI();
	void UpdateStatus();

	// ----------------------------------------------------------------
	// トラック階層（フォルダ）と畳み込み
	// ----------------------------------------------------------------

	// 各カード（TrackCardsと同順）の階層レベルと親フラグ
	TArray<int32> TrackLevels;
	TArray<bool>  TrackParentFlags;

	// 畳み込み中の親トラックのIndex集合（更新後も維持する）
	TSet<int32> CollapsedTrackIndices;

	// メディア一覧を展開中のトラックのIndex集合（自動更新でカードが作り直されても展開状態を維持する）
	TSet<int32> ExpandedTrackIndices;

	// ソロ中のトラックIndex集合
	TSet<int32> SoloedTrackIndices;

	// ミュート中のトラックIndex集合
	TSet<int32> MutedTrackIndices;

	// Tracksのフォルダ深度から各トラックの階層レベルと親フラグを求める
	void ComputeHierarchy(const TArray<FTrackDetail>& Tracks,
		TArray<int32>& OutLevels, TArray<bool>& OutIsParent) const;

	// 畳み込み状態に応じて各カードの表示/非表示を更新する
	void RefreshTrackVisibility();

	// 親カードの位置(TrackCards上のindex)から、配下の子トラック(葉=非親)のIndex一覧を集める
	void GetChildLeafTrackIndices(int32 ParentPos, TArray<int32>& OutLeafTrackIndices) const;

	// 各親フォルダカードの「子が全REC中か」の表示を更新する
	void UpdateParentArmVisuals();

	// ミュート/ソロ状態に応じて各トラックカードの薄暗さ（ディミング）を更新する
	// ・ミュート中のトラックは薄暗く
	// ・どれかがソロ中なら、ソロでないトラックを薄暗く
	void RefreshAllTrackDim();

	// 接続処理中フラグ
	bool bIsConnecting = false;

	// 接続試行の世代番号。接続ボタンを押すたびに増やし、古い試行の結果を無視するために使う。
	int32 ConnectGeneration = 0;

	// 非同期接続の結果をゲームスレッドで受け取る（古い世代の結果は破棄）
	void HandleConnectResult(int32 Gen, class URadnodzClient* AttemptClient, bool bSuccess, const FRigdocksError& Error);

	// --- 接続の生存監視（半開き検出）---
	// Reaper側の接続だけが消えると、端末のTCPはESTABLISHEDのまま残り、UIは「接続中」表示なのに
	// RPCが一切通らなくなる（0Hz表示・トラック一覧が空・在席情報が更新されず名簿から自分が消える）。
	// この状態は毎秒チェック（Client->IsConnected()）では検出できない（ソケットは生きて見えるため）。
	// 軽量RPCを定期的に往復させ、長時間まったく応答がない場合だけ切断とみなす。
	//
	// 【閾値の根拠】モニター再生と通話が同時に走るとRPCが数秒詰まることがあるため、
	// 15秒間隔・4回連続（＝約60秒無応答）とし、一時的な詰まりでは発火しないようにしている。
	void TickConnectionHealth(float DeltaSeconds);
	// 現在は無効（誤検出による切断→再接続の繰り返しを避けるため）。
	static constexpr bool bConnHealthCheckEnabled = false;
	// 調査用：切断はせずRPCの応答有無だけを15秒ごとにログへ残す。
	// 【現在は無効】この計測自体が SendTimingProbe というRPCを1往復させるため、
	// 観測が原因で接続を壊している可能性が消せなかった。まず何も足していない状態を
	// 基準にするために止めている。切り分けが済むまで true に戻さないこと。
	static constexpr bool bConnHealthProbeLogOnly = false;
	double ConnHealthTimer = 0.0;              // 次の確認までの経過時間
	int32  ConnHealthFailCount = 0;            // 連続失敗回数（成功で即0に戻す）
	static constexpr double ConnHealthIntervalSec  = 15.0;  // 確認間隔
	static constexpr int32  ConnHealthFailThreshold = 4;    // 連続失敗がこの回数に達したら切断とみなす

	// 意図しない切断（回線断・サーバ消失・半開き）のあと、数回だけ自動で再接続を試みる。
	// 「接続」ボタンによる手動切断では動かさない。
	void RequestAutoReconnect();               // 意図しない切断を検出したら呼ぶ
	void TickAutoReconnect(float DeltaSeconds);
	bool   bAutoReconnectPending = false;
	double ConnReconnectTimer = 0.0;
	int32  ConnAutoReconnectTries = 0;
	static constexpr double ConnReconnectDelaySec = 5.0;   // 再試行の間隔
	static constexpr int32  ConnAutoReconnectMaxTries = 5; // 上限（超えたら未接続のまま止める）

	// 録音停止後、新しく録音されたメディアを「トラック名_連番」(2桁ゼロ埋め・トラック内通し)に命名する
	void RenameRecordedMedia();

	// 録音/再生/停止の状態表示（フッターのボタン上）
	void SetTransportState(const FString& Label, const FLinearColor& Color);

	// PLAY/REC ボタンの表示（状態に応じて PLAY⇄STOP / REC⇄STOP を切り替える）
	void UpdateTransportButtons();

	// モーメンタリー ラウドネスのキャッシュ（キー＝メディア名＋開始位置）
	TMap<FString, double> MomentaryLUFSCache;

	// 録音開始時点の各アームトラックのメディア数（録音後、新規メディアを「トラック名_連番」に命名するため）
	TMap<int32, int32> PreRecordMediaCounts;

	// --- メディア一覧キャッシュ（展開のたびの重い再取得を避ける） ---
	// トラックIndex → そのトラックのメディア一覧。FetchTracksでクリア＆背景で再先読みする。
	UPROPERTY(Transient) TMap<int32, FTrackMediaCacheEntry> TrackMediaCache;

	// 接続/更新後、全トラックのメディア一覧をバックグラウンドで先読み（UIを止めない）
	// bRefreshExisting=true なら、キャッシュ済みのトラックも取り直して内容を突き合わせ、
	// 変わったトラックの展開中一覧だけを作り直す（「更新」時のちらつきを防ぐ）。
	void   StartMediaCacheWarm(bool bRefreshExisting = false);
	void   TickMediaCacheWarm(float DeltaSeconds);   // NativeTickから少しずつ処理
	bool   bMediaWarmActive = false;
	bool   bMediaWarmRefresh = false;                // キャッシュ済みも取り直すか
	int32  MediaWarmTrackIndex = 0;                  // TrackList上のインデックス
	// 走査の世代。やり直した後に届いた前周回の取得結果を捨てるために使う。
	int32  MediaWarmGeneration = 0;
	double MediaWarmTimer = 0.0;                     // 取得間隔の累積時間

	// --- ラウドネスのバックグラウンド先読み（フレーム分散・UIを止めない） ---
	void StartLoudnessPrefetch();              // 接続/更新後に先頭から一巡を開始
	void TickLoudnessPrefetch(float DeltaSeconds); // NativeTickから少しずつ処理

	bool   bLoudnessPrefetchActive = false;
	int32  PrefetchTrackIndex = 0;             // TrackList上のインデックス
	int32  PrefetchItemIndex = 0;              // 現トラックのメディアインデックス
	bool   bPrefetchListLoaded = false;        // 現トラックのメディア一覧取得済み
	double PrefetchTimer = 0.0;                // 取得間隔の累積時間
	UPROPERTY(Transient) TArray<TObjectPtr<UMediaItem>> PrefetchItems;
	TArray<FString> PrefetchNames;
	TArray<double>  PrefetchStarts;

	// --- ラウドネス計測ジョブ（フレーム分散・UIを止めない）。完了後にSlack共有 or 音量調整を行う ---
	// bForceRemeasure＝true の場合、キャッシュ済みでも必ず実機から再計測する（計測調整で使用。
	// LUFSはアイテム音量の影響を受けるため、調整の要否を正しく判定するには毎回実測が必要）。
	void BeginMeasureJob(const TArray<int32>& TrackIndices, bool bForceRemeasure = false);
	void TickMeasureJob(float DeltaSeconds);   // NativeTickから少しずつ処理
	void EndMeasureJob();                       // 完了/中断時：結果反映＋後続処理

	bool   bMeasureForceRemeasure = false;      // 今回の計測ジョブがキャッシュを無視して強制再計測するか
	bool   bMeasureJobActive = false;
	double MeasureTimer = 0.0;
	TArray<int32> MeasureTrackQueue;            // 計測対象トラック（FTrackDetail.Index）
	int32  MeasureTrackCursor = 0;
	bool   bMeasureListLoaded = false;
	int32  MeasureItemCursor = 0;
	UPROPERTY(Transient) TArray<TObjectPtr<UMediaItem>> MeasureItems;
	TArray<FString> MeasureNames;
	TArray<double>  MeasureStarts;

	// --- ラウドネス計測＆音量調整（ノーマライズ） ---
	// 全メディアを計測し、各メディアの音量を目標ラウドネス(TargetLUFS)へ合わせる。
	UFUNCTION() void HandleNormalizeClicked();
	UFUNCTION() void HandleTargetLufsCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	// グループトーク音量スライダー：ドラッグ中（1刻みに丸めて即反映）と、離した時（保存）。
	UFUNCTION() void HandleGroupTalkVolumeChanged(float NewValue);
	UFUNCTION() void HandleGroupTalkVolumeCaptureEnd();
	// スライダー位置と数値表示を現在値へ合わせる。
	void UpdateGroupTalkVolumeUI();
	// 現在のグループトーク受信音量を両chの受信機へ適用する。
	void ApplyGroupTalkPlaybackGain();

	// モニター受信音量スライダー：ドラッグ中（1dB刻みに丸めて即反映）と、離した時（保存）。
	UFUNCTION() void HandleMonitorVolumeChanged(float NewValue);
	UFUNCTION() void HandleMonitorVolumeCaptureEnd();
	// スライダー位置と数値表示（"+5 dB"等）を現在値へ合わせる。
	void UpdateMonitorVolumeUI();
	// 現在のモニター受信音量(dB)を線形ゲインに変換してモニター受信機へ適用する。
	void ApplyMonitorVolume();

	// マーカーオフセット・スライダー：ドラッグ中（1秒刻みに丸めて即反映）と、離した時（保存）。
	UFUNCTION() void HandleMarkerOffsetChanged(float NewValue);
	UFUNCTION() void HandleMarkerOffsetCaptureEnd();
	// スライダー位置と数値表示（"-1 秒"等）を現在値へ合わせる。
	void UpdateMarkerOffsetUI();

	// 再生/録音中はトラック削除ボタンを「マーカー」ボタンに切り替える（トランスポート更新時に呼ぶ）。
	void UpdateDeleteMarkerButton();
	// マーカーボタン押下：押した瞬間のプレイ位置(＋オフセット秒)を固定し、名前入力ダイアログを開く。
	void HandleAddMarkerClicked();

public:
	// 名前入力ダイアログから呼ばれ、確定位置(秒)＋名前でマーカーを作成する（名前空欄可）。
	void CommitMarker(double PositionSeconds, const FString& Name);
	// マーカー名ダイアログが履歴ボタンを作るために参照する（新しい順）。
	const TArray<FString>& GetMarkerNameHistory() const { return MarkerNameHistory; }
private:

	// グループトーク送信音量スライダー：ドラッグ中（1刻みに丸めて即反映）と、離した時（保存）。
	UFUNCTION() void HandleGroupTalkSendVolumeChanged(float NewValue);
	UFUNCTION() void HandleGroupTalkSendVolumeCaptureEnd();
	void UpdateGroupTalkSendVolumeUI();
	// 現在の送信音量を送信機（GroupTalkSender）へ適用する。
	void ApplyGroupTalkSendGain();

	// マイク送信チェックの開始/停止トグルとボタン表示更新。
	UFUNCTION() void HandleMicCheckClicked();
	void UpdateMicCheckButton();
	void ApplyLoudnessNormalization();          // 計測結果から各メディアの音量を目標値へ調整
	bool  bNormalizeAfterMeasure = false;       // 計測ジョブ完了後に音量調整するか
	float NormalizeSpinPhase = 0.f;             // 実行中アイコンの回転位相

	/** 目標ラウドネス（LUFS）。音量調整の到達目標。端末に保存される。 */
	float TargetLUFS = -7.f;

	/** 計測調整でピークの超過（赤クリップ）を防ぐか。ONの場合、目標ラウドネスに合わせるとピークが超えてしまう時は超えない音量を上限にする。端末に保存される。 */
	bool bLimitPeakOnNormalize = true;

	// --- 入力レベルメーター ---
	// アーム中トラックのピーク(dB)を定期取得し、各トラックカードのメーターへ反映する。
	double MeterPollTimer = 0.0;
	void TickTrackMeters(float DeltaSeconds);

	// 再生(カーソル)中のメディア行
	TWeakObjectPtr<UWG_MediaItemRow> ActiveMediaRow;

	// アクティブなメディア行を切り替える（旧行のハイライト解除＋新行をハイライト）
	void SetActiveMediaRow(UWG_MediaItemRow* Row);

	// 現在の再生状態をアクティブなメディア行の▶/▢表示へ反映する
	void NotifyTransportToMediaRow();

	// BPのオーバーライドに左右されず、必ずC++でUIを更新するための内部関数
	void ApplyConnectedUI();
	void ApplyConnectFailedUI();
	void ApplyDisconnectedUI();
	void ApplyTracksUI(const TArray<FTrackDetail>& Tracks);
	void ApplyArmChangeUI(int32 InTrackIndex, bool bArmed);
	void ApplyRecordingUI();

	// 複数トラックを一括ARMする間だけtrueにし、ApplyArmChangeUI内の
	// 集約UI更新（UpdateParentArmVisuals/UpdateStatus）を抑制する。
	// ループ終了後に呼び出し側でまとめて1回だけ更新するため。
	bool bDeferAggregateArmUI = false;

	// 接続設定(IP/Port)・マイク名の保存・読み込み
	void SaveSettings();
	void LoadSettings();

	// セーブデータを読み込む（無ければ新規作成）。IP/Port/マイク名を一括管理する。
	UReaperSaveGame* LoadOrCreateSave() const;

	// 表示倍率をルートに反映する
	void ApplyZoom();

	/** C++UIを構築済みかどうか（多重構築防止）。 */
	bool bUICreated = false;

	// 音声モニター（ReaStream受信）のON/OFFと表示更新
	void UpdateMonitorButton();

	/** 受信レベルのメーターを今の値で塗り直す（NativeTick から毎フレーム）。 */
	void UpdateMonitorLevelMeter();

	// マイク送信のON/OFFと表示更新（方式は bUseParaknotMic で決まる）
	void UpdateMicSendButton();

	// ----------------------------------------------------------------
	// マイク送信の方式（ReaStream / PARAKNOT）
	// ----------------------------------------------------------------
	//
	// 送るマイクは同じで、相手と経路だけが違う。
	//   ReaStream : REAPER のトラックに挿した ReaStream へ平文 UDP（従来）
	//   PARAKNOT : スタジオの PARAKNOT へ暗号化して押し込む
	//
	// PARAKNOT 側は開始までに業務TCPの往復（一覧 → 必要なら役割変更 →
	// セッション発行）が要るので、**押した瞬間には始まらない**。
	// 往復の間は bParaknotMicStarting を立ててボタンに「準備中」を出す。

	/** どちらかの方式で送信中か。 */
	bool IsMicSendActive() const;

	/** 方式を問わずマイク送信を止める。 */
	void StopMicSend();

	/** モニター（受信）がどちらかの方式で動いているか。 */
	bool IsMonitorActive() const;

	/** 方式を問わずモニターを止める。 */
	void StopMonitor();

	/** PARAKNOT 経路のセッションを張り始める（一覧の取得から。非同期）。 */
	void StartParaknotFlow(EParaknotPurpose Purpose);

	/** 一覧が取れたところ（ゲームスレッド）。1件なら自動、複数なら選択ダイアログ。 */
	void OnParaknotInstancesFetched(const TArray<FParaknotInstance>& Instances, const FString& Error);

	/** 選択ダイアログで選ばれた（受信・待受でなければ確認してから切り替える）。 */
	UFUNCTION() void HandleParaknotInstancePicked(int32 Index);

	/** 「受信(待受)へ切り替える」に同意した。 */
	UFUNCTION() void ConfirmParaknotRoleChange();

	/** 役割変更（必要なら）→ セッション発行 → 送信開始（非同期）。 */
	void BeginParaknotSession(bool bNeedRoleChange);

	/** マイク送信まわりの短いお知らせを出す。 */
	void ShowMicMessage(const FString& Message);

	// ----------------------------------------------------------------
	// PARAKNOT モニターの自動復帰
	// ----------------------------------------------------------------
	//
	// **配信側の鍵は入れ替わることがある。** 誰も新規接続しないまま 300 秒
	// （RIGDOCKS のセッション有効期間）が過ぎた後に別の端末が参加すると、
	// その端末が新しい鍵を受け取り、先に聞いていた側は復号できなくなる。
	//
	// 症状は「無言で音が止まる」なので、復号失敗の増加を見て検出し、
	// セッションを取り直して繋ぎ直す（PARAKNOT 自身の受信側と同じ考え方）。

	/** 毎フレーム呼ぶ。中で間隔を見て、必要なときだけ取り直す。 */
	void TickParaknotMonitorRecovery(float DeltaSeconds);

	/** セッションを取り直してモニターを繋ぎ直す（非同期）。 */
	void RecoverParaknotMonitor();

	/**
	 * 復号失敗の通算値（前回見た値）。増えていれば鍵が食い違っている。
	 * **-1 は「基準がまだ無い」印**（開始直後 / 繋ぎ直した直後）。通算値は
	 * 張り直しても戻らないので、0 と比べると誤判定する。
	 */
	int32 ParaknotMonitorLastDecryptFailures = -1;

	/** 音が来なくなった時刻（0 = 来ている）。原因不明のときの保険に使う。 */
	double ParaknotMonitorNotReceivingSince = 0.0;

	/** 直近に取り直した時刻。**立て続けにやり直さない**ための間隔に使う。 */
	double ParaknotMonitorLastRecoverTime = 0.0;

	/** 取り直しの往復中か。 */
	bool bParaknotMonitorRecovering = false;

	/** 復帰の判定間隔（秒）。 */
	static constexpr float ParaknotMonitorCheckIntervalSec = 1.0f;

	/** 判定間隔の累積。 */
	float ParaknotMonitorCheckTimer = 0.0f;

	/** 音が来ないまま取り直すまでの猶予（秒）。原因が分からないときの保険。 */
	static constexpr double ParaknotMonitorSilenceTimeoutSec = 20.0;

	/** 取り直しを繰り返さないための最短間隔（秒）。 */
	static constexpr double ParaknotMonitorRecoverCooldownSec = 5.0;

	/**
	 * 鍵の取り直しに連続で失敗した回数。成功で 0 に戻る。
	 *
	 * **打ち切りが無いと永久に叩き続ける。** 失敗しても無音タイマーは
	 * 超えたままなので、クールダウンごとに業務 TCP を往復し続けてしまう。
	 */
	int32 ParaknotMonitorRecoverFailures = 0;

	/** この回数だけ続けて失敗したらモニターを止めて理由を出す（約 100 秒）。 */
	static constexpr int32 ParaknotMonitorMaxRecoverFailures = 5;

	/** 設定：マイク送信方式の切替ボタン。 */
	UFUNCTION() void HandleMicTransportClicked();
	void UpdateMicTransportButton();

	// メーター受信 ON/OFF（ホーム・設定のボタン連動。必要なときだけON、既定OFF）
	UFUNCTION() void HandleMeterReceiveClicked();   // どちらのボタンからも同じ状態をトグル
	void SetMeterReceiveEnabled(bool bEnable);       // 状態設定＋メーター表示・ボタン表示を更新
	void UpdateMeterReceiveButtons();                // ホーム/設定 両ボタンの表示を更新

	// 言語切替を反映（恒久UIのテキストを再設定する）
	void ApplyLanguage();

	// テーマ（背景の明るさ）切替
	UFUNCTION() void HandleThemeClicked();   // 設定画面の切替ボタン（標準→暗め→明るめ循環）
	void UpdateThemeButton();                 // 切替ボタンの表示更新
	void ApplyTheme();                        // UIThemeIndex を背景ボーダーへ反映

	UFUNCTION() void HandleOrientationClicked();   // 設定画面の切替ボタン（自動→縦→横循環）
	void UpdateOrientationButton();                 // 切替ボタンの表示更新
	void ApplyOrientation();                        // ScreenOrientationLock を端末の向き固定へ反映

	UFUNCTION() void HandleConnectClicked();
	// Reaper IP入力欄の変更でヘッダのIP表示を更新する
	UFUNCTION() void HandleReaperIpChanged(const FText& Text);
	// ヘッダの「自分IP / Reaper IP」表示を更新する
	void UpdateHeaderIpText();
	UFUNCTION() void HandleAddTrackClicked();
	UFUNCTION() void HandleMicSettingsClicked();
	// 画面録画ボタン：録画中なら停止、そうでなければ許可要求→カウントダウン→開始。
	UFUNCTION() void HandleScreenRecordClicked();
	// 録画の状態機械（許可待ち→カウントダウン→録画中）を毎フレーム進める。
	void TickScreenRecord(float InDeltaTime);
	// 録画ボタンの表示（ラベル/色）を現在状態に合わせて更新する。
	void UpdateScreenRecordButton();
	// カウントダウン用オーバーレイを生成/表示/更新/破棄する。
	void ShowScreenRecCountdown(int32 Number);
	void HideScreenRecCountdown();
	// 「録画中に秒数＋停止ボタンを表示」トグル。
	UFUNCTION() void HandleScreenRecOverlayToggleClicked();
	void UpdateScreenRecOverlayButton();
	// 「録画にマイク音を含める」トグル。
	UFUNCTION() void HandleScreenRecMicToggleClicked();
	void UpdateScreenRecMicButton();
	UFUNCTION() void HandleMonitorClicked();
	UFUNCTION() void HandleMicSendClicked();
	UFUNCTION() void HandleUpdateClicked();
	UFUNCTION() void HandlePlayClicked();
	UFUNCTION() void HandleRecordClicked();
	UFUNCTION() void HandleStopClicked();
	UFUNCTION() void HandleClearArmedClicked();

	// --- トラック削除（チェック選択→決定→確認ダイアログ→削除） ---
	UFUNCTION() void HandleDeleteTrackClicked();    // 「削除」で選択モードへ、「決定」で確認へ
	UFUNCTION() void HandleDeleteSelectAll();       // 全トラックを選択
	UFUNCTION() void HandleDeleteDeselectAll();     // 全トラックの選択を解除
	void SetDeleteMode(bool bOn);                   // 削除選択モードの切替（ボタン表示・各カードのチェック表示）
	// チェック中トラック＋（親が選ばれていれば）その子孫トラックを集める（表示順）。
	// OutTracks=削除対象のトラックポインタ / OutNames=表示用の名前。
	void GatherDeleteTargets(TArray<UMediaTrack*>& OutTracks, TArray<FString>& OutNames) const;
public:
	void ConfirmDeleteSelectedTracks();             // 確認「はい」：選択トラックを削除
	void CancelDeleteConfirm();                     // 確認「いいえ」：削除せず選択モードへ戻る
	void ConfirmUnlock();                           // ロック解除確認「はい」：実際にロックを解除する
	void CancelUnlockConfirm();                     // ロック解除確認「いいえ」：ロックしたままにする（no-op）

	// --- アクション（Actions/）から使う窓口 ---
	URadnodzClient* GetConnectedClient() const { return bIsConnected ? Client : nullptr; }   // 未接続なら nullptr
	UReaProject* GetCurrentProject() const { return GetProject(); }
	// バッチをワーカーで実行し、完了後にゲームスレッドで OnDone を呼ぶ（Build/Parse は GT で行うこと）。
	// bHeavy=true なら解析用の2本目の接続で実行する。積めなかったら false。
	bool RunActionBatchAsync(URadnodzRpcBatch* Batch, bool bHeavy, TFunction<void()> OnDone,
		const TArray<UObject*>& KeepAlive = TArray<UObject*>());
	void ShowActionInfo(const FString& Title, const FString& Message);   // OK だけのダイアログ
	void BeginActionBusy();
	void EndActionBusy();                                                // アクション完了（キャンセル・失敗含む）で必ず呼ぶ
	bool IsActionBusy() const { return bActionBusy; }
	FIRMeasurementSession& GetIRMeasurementSession() { return IRMeasurementSession; }
private:
	bool bDeleteMode = false;

	UFUNCTION() void HandleScanClicked();   // Reaper検索ウインドウを開く
	UFUNCTION() void HandleMeterIntervalCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleMeterModeClicked();      // フルメーター⇄信号〇 切替
	UFUNCTION() void HandleMeterDotScopeClicked();  // 〇の数：chごと⇄トラックに1つ
	UFUNCTION() void HandleLimitPeakClicked();      // 計測調整時のピーク超過防止 ON/OFF
	void UpdateLimitPeakButton();                   // ピーク超過防止ボタンの表示を更新
	void UpdateMeterModeButton();                   // メーター表示モードのボタン表示を更新
	void UpdateMeterDotScopeButton();               // 〇の数ボタンの表示を更新
	void RebuildAllTrackMeters();                   // 全トラックカードのメーターを現在のモードで作り直す
	// 全トラックの波形名＋ラウドネスをSlackへ共有（未計測は先に計測してから送信）
	UFUNCTION() void HandleSlackShareClicked();
	UFUNCTION() void HandleSlackTokenCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleSlackToUserCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	// 計測済みのラウドネスを集計してSlackに投稿する
	void PostLoudnessToSlack();
	UFUNCTION() void HandleHomeTabClicked();
	UFUNCTION() void HandleSettingsTabClicked();
	UFUNCTION() void HandleMicInputModeClicked();
	UFUNCTION() void HandleMonitorOutputModeClicked();
	UFUNCTION() void HandleMonitorBufferClicked();
	// ダウンミックス係数の入力欄が確定されたとき：全chの係数を読み取り反映する
	UFUNCTION() void HandleDmCoeffCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	// 受信バッファ（遅延）ボタン表示を更新
	void UpdateMonitorBufferButton();

	UFUNCTION() void HandleMonitorSourceClicked();
	void UpdateMonitorSourceButton();

	/** 配信元の候補を取り直して ComboBox へ流し込む（要接続）。 */
	UFUNCTION() void HandleMonitorInstanceRefreshClicked();
	UFUNCTION() void HandleMonitorInstanceChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	void RefreshMonitorInstanceCombo();

	/** 候補 1 件ぶんの表示名（ポップアップの行と同じ書き方）。 */
	static FString MakeMonitorInstanceLabel(const struct FParaknotInstance& Instance);


	// ダウンミックス係数の入力欄に現在値（DmGains）を反映する
	void RefreshDmCoeffInputs();
	// ダウンミックス係数を既定値で初期化（未設定・要素数不一致時）
	void EnsureDmGainsDefaults();
	UFUNCTION() void HandleSpatialItdClicked();
	UFUNCTION() void HandleSpatialShadowClicked();
	UFUNCTION() void HandleSpatialOrderClicked();
	// HRTFプロファイルのドロップダウン：項目の明色生成・選択変更
	UFUNCTION() UWidget* MakeComboItemLight(FString Item);
	// 親トラック名から UMediaTrack* を解決（"Master"/空/不在なら nullptr＝最上位）
	class UMediaTrack* ResolveParentTrackByName(const FString& Name) const;
	UFUNCTION() void HandleHrtfComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	// HRTFコンボの選択肢・選択状態を現在値に合わせて更新する
	void RefreshHrtfCombo();
	// 簡易(内蔵)を表すコンボ表示名
	static FString HrtfBuiltInLabel();
	// HRTFプロファイルの内部名（保存・検索キー。言語に依存しない）⇄ コンボ表示名（現在の言語で翻訳）の相互変換。
	// 保存データやHRTF検索は内部名で行うため、表示だけ言語切替に追従させても既存の選択は壊れない。
	static FString HrtfDisplayNameForInternal(const FString& InternalName);
	static FString HrtfInternalNameForDisplay(const FString& DisplayName);
	UFUNCTION() void HandleMapEditChClicked();
	UFUNCTION() void HandleMapAngleMinusClicked();
	UFUNCTION() void HandleMapAnglePlusClicked();
	UFUNCTION() void HandleMapDistMinusClicked();
	UFUNCTION() void HandleMapDistPlusClicked();

	// 立体音響パラメータのボタン表示を更新
	void UpdateSpatialButtons();
	// カスタム配置の数値表示と図を更新
	void UpdateMappingControls();
	// カスタム配置の図（各chの点の位置）を更新
	void UpdateMappingDiagram();
	// カスタム配置を既定値（REAPER 8ch）で初期化（未設定時）
	void EnsureCustomLayoutDefaults();
	// モニター再起動（受信中のみ。出力設定の即時反映用）。
	// ReaStream は受信ごと張り直し、PARAKNOT は再生だけ作り直す（購読はそのまま）。
	void RestartMonitorIfActive();
	// AudioMonitor に出力モードと立体音響パラメータを反映する（Start前に呼ぶ）
	void ConfigureAudioMonitor();
	// 現在の設定からモニターの出力設定を組み立てる（ReaStream / PARAKNOT 共通）
	FMonitorOutputSettings MakeMonitorOutputSettings() const;

	// マイク入力モードのボタン表示（本体 / Bluetooth）を現在値に合わせて更新する
	void UpdateMicInputModeButton();
	// モニター出力モードのボタン表示（ステレオ / そのまま）を現在値に合わせて更新する
	void UpdateMonitorOutputButton();

	// 設定画面（言語切替・マイク登録など）を構築して返す
	UWidget* BuildSettingsPage();
	// 下部のタブバー（ホーム / 設定）を構築して返す
	UWidget* BuildBottomNav();

	UPROPERTY()
	URadnodzClient* Client = nullptr;

	// 文字起こし(Whisper)モデルのコンテキスト。EnsureTranscriptionModelLoaded()で遅延ロードし、
	// 一度読み込んだらセッション中は保持し続ける（毎回のロードは重いため）。
	UPROPERTY()
	UTranscriptionContext* TrscContext = nullptr;

	UReaProject* GetProject() const;
};
