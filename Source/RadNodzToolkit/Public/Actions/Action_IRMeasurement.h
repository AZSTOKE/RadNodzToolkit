// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Actions/RadNodzAction.h"
#include "Action_IRMeasurement.generated.h"

class UMediaItem;

/**
 * 「IR算出」アクション（「IR計測準備」「IR測定」の後に実行する）。
 *
 *   1. 各「ch. n」トラックから、保存しておいた TSP の範囲を含む録音アイテムを探す。
 *   2. そのアイテムを TSP の開始・終了位置で分割して範囲外を削除（トリミング）し、新規テイクにレンダリングする。
 *   3. レンダリングしたファイルから IR を算出する（ch ごとに1グループ → ch 数分の多チャンネル IR）。
 *   4. 算出した IR を「IR」トラックの TSP 開始位置に挿入する。
 */
UCLASS()
class RADNODZTOOLKIT_API UAction_IRMeasurement : public URadNodzAction
{
	GENERATED_BODY()

public:

	virtual FString GetDisplayName() const override;
	virtual void Execute(UWG_RadNodzToolkit* Owner) override;

	// 位置比較の許容誤差（秒）。REAPER の位置は double なので完全一致は期待しない。
	static constexpr double PositionEpsilon = 0.001;

private:

	// 1段目の結果：各 ch トラックのアイテム一覧 → 2段目：各アイテムの位置・長さ
	void FetchItemRanges(UWG_RadNodzToolkit* Owner, const TArray<TArray<UMediaItem*>>& ItemsPerChannel);

	// 3段目：トリミング（分割＋範囲外の削除）。
	// AZ_SplitMediaItem の戻り値（左右のアイテム）は入れ子の配列で、バッチの BindRef では参照できないため、
	// 分割ごとに1往復して GT で取り出し、次のバッチへ実ポインタで渡す。
	//   Step 0: 全アイテムの選択解除 ＋ TSP 開始位置で分割
	//   Step 1: 開始前の片を削除 ＋ TSP 終了位置で分割
	// 分割は「選択中のアイテムを編集カーソル位置で分割」する REAPER アクションで行われるため、
	// 録音直後に全 ch のアイテムが選択されたままだと他の ch まで一緒に分割されてしまう。先に選択を外す。
	void RunTrimStep(UWG_RadNodzToolkit* Owner, int32 Step, const TArray<UMediaItem*>& KeepItems,
		const TArray<double>& Starts, const TArray<double>& Ends, const TArray<UMediaItem*>& DeleteItems);

	// 4段目：終了後の片を削除 → 新規テイクへレンダリング → ソースファイルのパス取得
	void RenderTrimmed(UWG_RadNodzToolkit* Owner, const TArray<UMediaItem*>& KeepItems, const TArray<UMediaItem*>& DeleteItems);

	// 5段目：IR 算出（解析用の接続で実行） → 6段目：IR トラックへ挿入
	void CalcImpulseResponse(UWG_RadNodzToolkit* Owner, const TArray<FString>& RecordedPaths);
	void InsertImpulseResponse(UWG_RadNodzToolkit* Owner, const FString& IrPath);

	static FString ErrorTitle();
};
