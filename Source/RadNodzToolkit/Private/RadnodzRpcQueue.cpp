// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "RadnodzRpcQueue.h"

#include "HAL/RunnableThread.h"
#include "HAL/PlatformProcess.h"

FRadnodzRpcQueue::FRadnodzRpcQueue(const TCHAR* InDebugName)
{
	WakeEvent = FPlatformProcess::GetSynchEventFromPool();

	Thread = FRunnableThread::Create(this, InDebugName, 0, TPri_Normal);
}

FRadnodzRpcQueue::~FRadnodzRpcQueue()
{
	Stop();

	if (Thread)
	{
		// 実行中のジョブが終わるまで待つ。
		// RPCは TcpClient 側の見張りタイムアウトと keepalive で必ず戻るため、
		// ここで永久に待たされることは無い（切断済みなら即座に戻る）。
		Thread->Kill(true);

		delete Thread;
		Thread = nullptr;
	}

	if (WakeEvent)
	{
		FPlatformProcess::ReturnSynchEventToPool(WakeEvent);
		WakeEvent = nullptr;
	}

	Clear();
}

void FRadnodzRpcQueue::Enqueue(TFunction<void()> Job)
{
	if (!Job)
	{
		return;
	}

	{
		FScopeLock Lock(&JobsLock);

		if (bStopRequested.Load())
		{
			return;
		}

		Jobs.Add(FJob{ NAME_None, MoveTemp(Job) });
	}

	if (WakeEvent)
	{
		WakeEvent->Trigger();
	}
}

bool FRadnodzRpcQueue::EnqueueUnique(FName Key, TFunction<void()> Job)
{
	if (!Job)
	{
		return false;
	}

	if (Key.IsNone())
	{
		Enqueue(MoveTemp(Job));

		return true;
	}

	{
		FScopeLock Lock(&JobsLock);

		if (bStopRequested.Load())
		{
			return false;
		}

		// 同じ用途のジョブが既に待っている＝前回ぶんがまだ流れていない。
		// 積み増さずに捨てる（次の周期でまた積まれる）。
		if (PendingKeys.Contains(Key))
		{
			return false;
		}

		PendingKeys.Add(Key);
		Jobs.Add(FJob{ Key, MoveTemp(Job) });
	}

	if (WakeEvent)
	{
		WakeEvent->Trigger();
	}

	return true;
}

void FRadnodzRpcQueue::Clear()
{
	FScopeLock Lock(&JobsLock);

	Jobs.Reset();
	PendingKeys.Reset();
}

int32 FRadnodzRpcQueue::NumPending() const
{
	FScopeLock Lock(&JobsLock);

	return Jobs.Num();
}

uint32 FRadnodzRpcQueue::Run()
{
	while (!bStopRequested.Load())
	{
		FJob Job;
		bool bHasJob = false;

		{
			FScopeLock Lock(&JobsLock);

			if (Jobs.Num() > 0)
			{
				Job = MoveTemp(Jobs[0]);
				Jobs.RemoveAt(0);

				// 実行を始めた時点でキーを外す。
				// こうしておくと「実行中の1件」と「待機中の1件」までは持てるので、
				// 周期処理が1回ぶん先に進む。
				if (!Job.Key.IsNone())
				{
					PendingKeys.Remove(Job.Key);
				}

				bHasJob = true;
			}
		}

		if (!bHasJob)
		{
			// 取りこぼしを避けるため、待ちきりにはしない。
			if (WakeEvent)
			{
				WakeEvent->Wait(FTimespan::FromMilliseconds(200));
			}

			continue;
		}

		bBusy.Store(true);

		if (Job.Body)
		{
			Job.Body();
		}

		bBusy.Store(false);
	}

	return 0;
}

void FRadnodzRpcQueue::Stop()
{
	{
		FScopeLock Lock(&JobsLock);

		bStopRequested.Store(true);

		Jobs.Reset();
		PendingKeys.Reset();
	}

	if (WakeEvent)
	{
		WakeEvent->Trigger();
	}
}
