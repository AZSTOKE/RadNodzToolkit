// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "Actions/RadNodzAction.h"
#include "Action_IRRecord.generated.h"

/**
 * 「IR測定」アクション（「IR計測準備」の後、「IR算出」の前に実行する）。
 *
 *   1. 録音トラック「ch. 1」～「ch. N」を録音モードにする。
 *   2. TSP の開始 -0.5 秒から録音を始め、TSP の終了位置を過ぎたら止める。
 *   3. 録音トラックの録音モードを解除する。
 *
 * 停止位置を指定して録音する API は無いため、録音中の再生位置を1秒ごとに取得し、
 * TSP の終了位置を超えていれば停止命令を送る（止まるのが遅れるだけで、録音が短くなることはない）。
 */
UCLASS()
class RADNODZTOOLKIT_API UAction_IRRecord : public URadNodzAction
{
	GENERATED_BODY()

public:

	virtual FString GetDisplayName() const override;
	virtual void Execute(UWG_RadNodzToolkit* Owner) override;

	// TSP の手前に付ける録音の余白（秒）
	static constexpr double PreRollSec = 0.5;

	// 再生位置の確認間隔（秒）
	static constexpr double PollIntervalSec = 1.0;

	// 想定の録音時間を超えてもこの秒数だけ終わらなければ打ち切る（再生位置が進まない等の異常時）
	static constexpr double TimeoutMarginSec = 30.0;

private:

	// 1秒後に再生位置を確認する（終了位置を過ぎていれば停止、まだなら再度予約）
	void SchedulePoll(UWG_RadNodzToolkit* Owner);
	void Poll(UWG_RadNodzToolkit* Owner);

	// 録音を止めて録音モードを解除する。bUserStopped＝録音が途中で止められていた
	void StopRecording(UWG_RadNodzToolkit* Owner, bool bUserStopped);

	static FString ErrorTitle();

	double StopPosition = 0.0;     // 録音を止める再生位置（TSP の終了位置）
	double DeadlineTime = 0.0;     // この時刻（FPlatformTime::Seconds）を過ぎたら打ち切る
};
