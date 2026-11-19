// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Templates/Atomic.h"
#include "Templates/Function.h"

/**
 * REAPER への同期RPCを1本のスレッドで順番に流すためのキュー。
 *
 * RIGDOCKS のRPCは「送って、応答が返るまで待つ」同期通信で、電波が悪いと
 * 数秒〜数十秒返らない。これをゲームスレッドで呼ぶとUIがそのまま固まる
 * （Androidでは入力に5秒応答しないとANRになる）。
 *
 * そこで RPC はすべてこのキューへ積み、専用スレッドで実行する。
 * ゲームスレッドは「積む」だけなので絶対にブロックしない。
 * UIの更新は各ジョブの中から AsyncTask(GameThread) で戻して行う。
 *
 * 1本のスレッドで順番に実行するため、同じ接続を同時に使うことも無い。
 *
 * 用途ごとに別インスタンスを持たせる（= 接続を分ける）ことを想定している。
 *   ・制御用 … 操作・一覧取得・メーター・ハートビート（短い往復）
 *   ・解析用 … ラウドネス計測など、1回で数十秒かかるもの
 * 解析用を分けておかないと、計測中は操作系のRPCが全部その後ろに並ぶ。
 */
class FRadnodzRpcQueue final : public FRunnable
{
public:
	explicit FRadnodzRpcQueue(const TCHAR* InDebugName);

	virtual ~FRadnodzRpcQueue() override;

	/** ジョブを積む（ゲームスレッドから呼ぶ。ブロックしない）。 */
	void Enqueue(TFunction<void()> Job);

	/**
	 * キー付きでジョブを積む。同じキーのジョブが未実行で残っていれば捨てる。
	 *
	 * 周期処理（メーター取得など）に使う。回線が遅いときに毎ティック積むと
	 * ジョブが雪だるま式に溜まり、回線が戻った瞬間に古い要求が一気に流れて
	 * しまうため、「1件だけ待たせる」形にする。
	 *
	 * @return 積んだら true / 同じキーが待機中で捨てたら false
	 */
	bool EnqueueUnique(FName Key, TFunction<void()> Job);

	/** 未実行のジョブを破棄する（切断時など）。実行中のジョブは止められない。 */
	void Clear();

	/** 待機中のジョブ数。 */
	int32 NumPending() const;

	/** 現在ジョブを実行中か。 */
	bool IsBusy() const { return bBusy.Load(); }

	// FRunnable
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	struct FJob
	{
		FName Key;
		TFunction<void()> Body;
	};

	/** 先頭から順に実行する待ち行列。 */
	TArray<FJob> Jobs;

	/** Jobs / PendingKeys の保護。 */
	mutable FCriticalSection JobsLock;

	/** 待機中のキー（EnqueueUnique の重複判定用）。 */
	TSet<FName> PendingKeys;

	FEvent* WakeEvent = nullptr;

	FRunnableThread* Thread = nullptr;

	TAtomic<bool> bStopRequested{ false };

	TAtomic<bool> bBusy{ false };
};
