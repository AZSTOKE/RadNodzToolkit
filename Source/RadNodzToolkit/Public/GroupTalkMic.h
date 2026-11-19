// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"

/**
 * マイク送信チェック（Android）。
 * 送信と同じ取り込み（送信ゲイン込み）を UDP 送信せずに自分のイヤホンへ折り返し再生し、
 * 相手に届く音を確認する。非Androidでは何もしない（IsPreviewRunning=false）。
 */
class RADNODZTOOLKIT_API FGroupTalkMic
{
public:
	/** 折り返し再生を開始する。Gain=送信ゲイン倍率。要イヤホン。 */
	static void StartPreview(float Gain);

	/** 停止する。 */
	static void Stop();

	/** 送信ゲイン倍率を更新する。 */
	static void SetGain(float Gain);

	/** 折り返し再生中かどうか。 */
	static bool IsPreviewRunning();
};
