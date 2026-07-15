// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"   // ETextCommit::Type

#include "RadnodzClient.h"
#include "RadnodzUtility.h"
#include "RigdocksSilver_Track.h"
#include "RigdocksBronze_Cursor.h"
#include "ReaperMicInputMode.h"
#include "ReaperMonitorOutputMode.h"
#include "ReaperMicSetting.h"
#include "ReaperSaveGame.h"   // FServerAuthEntry

#include "WG_RadNodzToolkit.generated.h"

class UWG_TrackCard;
class UWG_AddTracksDialog;
class UWG_MicSettingsDialog;
class UReaperSaveGame;
class UReaStreamReceiver;
class UReaStreamSender;
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

	/** 接続先ポート番号（初期値: 12345） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaper|Connection")
	int32 Port = 12345;

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

	// Reaper通信時間の計測（設定タブ・接続セクション、「端末名/IPを表示」の下）。
	// 「測定」ボタンで SendTimingProbe を1往復させ、端末→Reaper（送信）と
	// Reaper→端末（結果取得）の平均時間を CommTimeText に表示する。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       CommTimeRowLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       CommTimeText;                 // 直近の計測結果（未計測は「--」）
	UPROPERTY(Transient) TObjectPtr<UButton>          CommTimeMeasureButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       CommTimeMeasureButtonText;
	UFUNCTION() void HandleMeasureCommTimeClicked();
	/** SendTimingProbe を1往復させ、端末→Reaper/Reaper→端末 の平均通信時間を計測・表示する。 */
	void MeasureCommTime();
	double CommTimeProbeTimer = 0.0;                          // 定期計測用タイマ（秒）
	static constexpr double CommTimeProbeIntervalSec = 30.0;  // 定期計測の間隔（秒）

	// 接続監視：接続中は定期的に RigdocksClient::IsConnected() を確認し、
	// 切断されていれば切断状態へ遷移する。
	double ConnCheckTimer = 0.0;                              // 接続確認用タイマ（秒）
	static constexpr double ConnCheckIntervalSec = 1.0;       // 接続確認の間隔（秒）

	// メーター受信トグル（ホーム画面のボタン）。設定画面のボタンと状態を連動させる。
	UPROPERTY(Transient) TObjectPtr<UButton>          MeterRecvButton;
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
	UPROPERTY(Transient) TObjectPtr<UButton>          UpdateButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       UpdateButtonText;
	// 言語切替（9言語：日本語/English/FIGS/韓国語/中国語簡体・繁体）はプルダウンで選択する。
	UPROPERTY(Transient) TObjectPtr<UComboBoxString>  LanguageCombo;
	UFUNCTION() void HandleLanguageComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	// 言語コンボの選択状態を現在の言語に合わせて更新する
	void RefreshLanguageCombo();
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       TrackSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       IpLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       PortLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>          ScanButton;        // 同ネットワークのReaperを検索
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ScanButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>          PlayButton;
	UPROPERTY(Transient) TObjectPtr<UButton>          RecordButton;
	UPROPERTY(Transient) TObjectPtr<USizeBox>         PlayButtonSize;    // 横向きでは高さを詰めて2段レイアウトの縦スクロールを防ぐ
	UPROPERTY(Transient) TObjectPtr<USizeBox>         RecordButtonSize;
	UPROPERTY(Transient) TObjectPtr<UButton>          ClearArmedButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ClearArmedButtonText;
	// トラック削除（選択→決定→確認→削除）
	UPROPERTY(Transient) TObjectPtr<UButton>          DeleteTrackButton;      // 「削除」⇄「決定」をトグル
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       DeleteTrackButtonText;
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

	// --- 横画面対応：ホームのパネルを向きに応じて再配置（reparent）する ---
	// ホームの各パネルは1度だけ生成し、縦（HomePortraitCol）／横（HomeLandscapeRow）へ付け替える。
	UPROPERTY(Transient) TObjectPtr<UVerticalBox>     HomePage;             // switcher index0 の単一子ホルダ
	UPROPERTY(Transient) TObjectPtr<UBorder>          ActionPanel;          // 接続＋GTALK1/2
	UPROPERTY(Transient) TObjectPtr<UBorder>          DeviceInfoPanel;      // Hz/bit/ch 情報バー
	UPROPERTY(Transient) TObjectPtr<UBorder>          ToolbarPanel;         // 追加/モニター/メーター/更新
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
	// 登録済みサーバー(serverId → 期待FP)の管理と、公開鍵コピー・認証(試し接続)。
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          AuthSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UButton>             UseTlsButton;       // TLS 使用トグル
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          UseTlsButtonText;
	UPROPERTY(Transient) TObjectPtr<UComboBoxString>     ServerIdCombo;      // 登録済み serverId のドロップダウン
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox>    ServerFpInput;      // 選択中 serverId の期待FP（編集可）
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          AuthStatusText;     // 操作結果の表示
	UPROPERTY(Transient) TObjectPtr<UButton>             AddServerIdButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          AddServerIdButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>             DeleteServerIdButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          DeleteServerIdButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>             CopyPubKeyButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          CopyPubKeyButtonText;
	UPROPERTY(Transient) TObjectPtr<UButton>             TrialConnectButton; // 「認証」ボタン
	UPROPERTY(Transient) TObjectPtr<UTextBlock>          TrialConnectButtonText;

	// 接続に TLS(mTLS) を使うか（SaveGame に永続化）。本体接続・認証ボタンの両方に適用。
	UPROPERTY() bool bUseTls = false;

	// serverId → 期待FP の登録一覧（SaveGame に永続化）。
	UPROPERTY() TArray<FServerAuthEntry> ServerAuthEntries;

	// 認証操作用のクライアント（鍵生成・公開鍵取得に使用。遅延生成）。
	UPROPERTY(Transient) TObjectPtr<URadnodzClient> AuthClient;

	// 「認証」試し接続の実行中フラグ（二重実行防止）。
	bool bIsAuthenticating = false;

	UFUNCTION() void HandleServerIdComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION() void HandleAddServerIdClicked();      // ポップアップを開く
	UFUNCTION() void HandleDeleteServerIdClicked();   // 確認ポップアップを開く
	UFUNCTION() void ConfirmDeleteServerId();         // 確認「はい」で実際に削除する
	UFUNCTION() void HandleServerFpCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	// Reaper IP / ポート入力の確定：現在の選択 serverId に紐づけて保存する。
	UFUNCTION() void HandleReaperIpCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandlePortCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void HandleCopyPublicKeyClicked();
	UFUNCTION() void HandleTrialConnectClicked();     // 「認証」: 試し接続
	UFUNCTION() void HandleUseTlsClicked();           // TLS 使用トグル

public:
	/** ポップアップから新しい serverId を登録する（重複時は無視）。 */
	void AddServerAuthEntry(const FString& NewId);

private:
	/** TLS 使用トグルボタンの表示を更新する。 */
	void UpdateUseTlsButton();

	/** 「認証」試し接続の結果をゲームスレッドで反映する（ボタン/ロゴ/ステータス復帰）。 */
	void HandleAuthResult(bool bSuccess, const FRigdocksError& Error);

	/** 認証情報の保存先ディレクトリ（モバイルでも永続なサンドボックス配下）。 */
	FString GetAuthStorageDir() const;
	/** AuthClient を用意し保存先を設定する。 */
	void EnsureAuthClient();
	/** ServerIdCombo を ServerAuthEntries で再構築し、選択中の期待FPを ServerFpInput に反映する。 */
	void RefreshServerIdCombo();
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

	/** モニター受信/マイク送信が使うReaStream既定ポート（固定値）。設定画面には参照用に表示のみ（編集不可）。 */
	static constexpr int32 ReaStreamPort = 58710;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ReaStreamPortRowLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       ReaStreamPortText;

	/** グループトーク用ポート番号のベース値（固定値）。モニターとは別ポートにすることで、
	 *  グループトーク使用中でもモニター受信を同時に使えるようにしている。
	 *  GroupTalkPort0/1 はこの値から連番で決まるため、名前は歴史的経緯でIntercomPortのまま。 */
	static constexpr int32 IntercomPort = ReaStreamPort + 1;

	UPROPERTY(Transient) TObjectPtr<UTextBlock>       IntercomSectionLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       AppSectionLabel;
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
	FString PresenceFolder;              // 在席名簿フォルダ（手動指定・任意。空ならAZDataを自動使用）
	FString ResolvedPresenceFolder;      // 接続時にREAPERリソース配下の AZData を解決した絶対パス（自動）

	// 実際に使う名簿フォルダ（自動解決を優先、無ければ手動指定）
	FString GetEffectivePresenceFolder() const { return !ResolvedPresenceFolder.IsEmpty() ? ResolvedPresenceFolder : PresenceFolder; }
	// 接続時に REAPER リソースパス配下の AZData フォルダ（絶対パス）を解決して保持する
	void ResolveAZDataFolder();
	UPROPERTY(Transient) TObjectPtr<UTextBlock>       MemberUserNameRowLabel;
	UPROPERTY(Transient) TObjectPtr<UEditableTextBox> MemberUserNameInput;

	void MarkPresenceOffline();             // 切断直前に、グループトークの在席ファイルを即座にオフライン扱いにする
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
	bool bTrackListAutoRefresh = false;
	/** 自動更新モード中、実際に自動更新が動作中か（グレー=停止中／赤=動作中）。再起動時は必ず停止状態に戻る。 */
	bool bTrackListAutoRefreshRunning = false;
	/** 自動更新の間隔（秒）。端末に保存される。 */
	int32 TrackRefreshIntervalSec = 10;
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

	/** 画面の向き固定。0=自動 / 1=縦 / 2=横。端末に保存される。 */
	int32 ScreenOrientationLock = 0;

	/** メーターの更新間隔（ミリ秒）。端末に保存される。 */
	int32 MeterIntervalMs = 200;
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

	/** モニター受信音声の出力方法（ステレオ / 立体音響 / そのまま）。端末に保存される。 */
	EMonitorOutputMode MonitorOutputMode = EMonitorOutputMode::Stereo;

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
	void   StartMediaCacheWarm();
	void   TickMediaCacheWarm(float DeltaSeconds);   // NativeTickから少しずつ処理
	bool   bMediaWarmActive = false;
	int32  MediaWarmTrackIndex = 0;                  // TrackList上のインデックス
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

	// マイク送信（ReaStream送信）のON/OFFと表示更新
	void UpdateMicSendButton();

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
	// モニター再起動（受信中のみ。立体音響設定の即時反映用）
	void RestartMonitorIfActive();
	// AudioMonitor に出力モードと立体音響パラメータを反映する（Start前に呼ぶ）
	void ConfigureAudioMonitor();

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

	// エラー出力
	void PrintErrorMsg();
};
