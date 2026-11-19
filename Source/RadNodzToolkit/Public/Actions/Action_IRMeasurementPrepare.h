// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Actions/RadNodzAction.h"
#include "Action_IRMeasurementPrepare.generated.h"

/**
 * 「IR計測準備」アクション。
 *
 *   1. ダイアログで 計測回数・チャンネル数・TSPの種類・再生の間隔 を入力させる（キャンセルで終了）。
 *   2. 親トラック「IR Measurement」を末尾に追加し、子に「TSP」「ch. 1」～「ch. N」「IR」を作る。
 *   3. TSP 信号を生成し、「TSP」トラックのプロジェクト末尾＋1秒の位置へ挿入、その範囲にリージョンを作る。
 *   4. 「ch. n」の録音時モニタリングを OFF にし、入力を n 番（モノラル）に割り当てる。
 *      （録音モードは入れない。「IR測定」が録音の間だけ入れる）
 *   5. 作ったトラック・アイテムの情報を FIRMeasurementSession に保存する（「IR測定」「IR算出」が使う）。
 */
UCLASS()
class RADNODZTOOLKIT_API UAction_IRMeasurementPrepare : public URadNodzAction
{
	GENERATED_BODY()

public:

	virtual FString GetDisplayName() const override;
	virtual void Execute(UWG_RadNodzToolkit* Owner) override;

	// TSP 信号の生成パラメータ（GOLD IR API の既定値に揃える）
	// （掃引と掃引の間の無音＝「再生の間隔」はダイアログのスライダーで指定する）
	static constexpr double LogSweepSec       = 10.0;
	static constexpr double LinearSweepSec    = 5.0;
	static constexpr double LogStartFreq      = 20.0;
	static constexpr double LogEndFreq        = 0.0;    // 0 = ナイキストの95%
	static constexpr int32  FallbackSampleRate = 48000;

	// TSP の挿入位置（プロジェクト末尾からの秒。既存のメディアと重ならないよう空ける）
	static constexpr double InsertOffsetFromEndSec = 1.0;

private:

	// ダイアログで OK が押された後の本処理（1段目：プロジェクト情報の取得）
	void StartPrepare(UWG_RadNodzToolkit* Owner, int32 RepeatCount, int32 NumChannels, bool bLogTSP, double IntervalSec);

	// 2段目：フォルダ作成・TSP生成・トラック作成・挿入・録音設定を1往復で行う
	void RunPrepareBatch(UWG_RadNodzToolkit* Owner, int32 RepeatCount, int32 NumChannels, bool bLogTSP, double IntervalSec,
		const FString& ProjectPath, int32 SampleRate, int32 TrackCount);

	static FString ErrorTitle();
};
