// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "ReaperClasses.h"   // UMediaTrack / UMediaItem
#include "IRMeasurementSession.generated.h"

/**
 * 「IR計測準備」で作ったトラック・TSP信号の情報。「IR測定」「IR算出」がこれを使う。
 *
 * アプリ内メモリにのみ保持する（REAPER のトラック/アイテムのポインタは
 * REAPER・アプリを再起動すると使えないため、端末には保存しない）。
 */
USTRUCT()
struct FIRMeasurementSession
{
	GENERATED_BODY()

	/** 準備が完了しているか。 */
	UPROPERTY(Transient) bool bValid = false;

	/** TSP の種類（true=対数 / false=線形）。IR 算出 API の選択に使う。 */
	UPROPERTY(Transient) bool bLogTSP = true;

	/** 計測ファイルの出力フォルダ（REAPER 側のパス）。 */
	UPROPERTY(Transient) FString OutputFolder;

	/** TSP 信号（WAV）と TSP 情報ファイル（json）の絶対パス。 */
	UPROPERTY(Transient) FString TspWavPath;
	UPROPERTY(Transient) FString TspInfoPath;

	/** TSP アイテムの開始位置と長さ（秒）。 */
	UPROPERTY(Transient) double TspStart = 0.0;
	UPROPERTY(Transient) double TspLength = 0.0;

	/** 計測回数（TSP の繰り返し回数）。 */
	UPROPERTY(Transient) int32 RepeatCount = 1;

	/** 親トラック「IR Measurement」と、その子トラック。 */
	UPROPERTY(Transient) TObjectPtr<UMediaTrack> ParentTrack;
	UPROPERTY(Transient) TObjectPtr<UMediaTrack> TspTrack;
	UPROPERTY(Transient) TArray<TObjectPtr<UMediaTrack>> ChannelTracks;   // 「ch. 1」～「ch. N」（録音用）
	UPROPERTY(Transient) TObjectPtr<UMediaTrack> IrTrack;

	/** TSP トラックに置いた TSP 信号のアイテム。 */
	UPROPERTY(Transient) TObjectPtr<UMediaItem> TspItem;
};
