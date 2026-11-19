// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ReaperBackgroundKeepAlive.h"

#include "Engine/Engine.h"
#include "AudioDevice.h"
#include "Misc/CoreDelegates.h"
#include "HAL/RunnableThread.h"
#include "HAL/Runnable.h"
#include "HAL/Event.h"
#include "HAL/PlatformProcess.h"
#include "Templates/Atomic.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidEventManager.h"   // FAppEventManager::GameThreadTicker（Launchモジュール）
#endif

namespace
{
	TAtomic<bool> GKeepAliveActive { false };
	TAtomic<bool> GInBackground { false };
	TAtomic<double> GBackgroundEnterTime { 0.0 };

	// バックグラウンド中のメンテナンス関数（監視スレッドから呼ばれる）
	FCriticalSection GMaintenanceLock;
	TFunction<void()> GMaintenanceFunc;

	// メンテナンス実行中に前面復帰が走らないよう直列化するためのロック。
	// 前面復帰デリゲート（ゲームスレッド）が取得を試み、実行中バッチの完了を待つ。
	FCriticalSection GBatchLock;

	bool GDelegatesRegistered = false;

#if PLATFORM_ANDROID
	// 前面サービスの開始/停止（GameActivityのThunkをJNIで呼ぶ。ゲームスレッドから）
	void CallKeepAliveThunk(const char* MethodName)
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID Method = FJavaWrapper::FindMethod(
				Env, FJavaWrapper::GameActivityClassID, MethodName, "()V", false);
			if (Method)
			{
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
			}
		}
	}

	// エンジンがバックグラウンド移行時に一時停止した音声再生を再開する。
	// FMixerPlatformAndroid::ResumeContext は未サスペンド時は何もしないため、繰り返し呼んで安全。
	// OpenSLの操作はスレッドセーフなので監視スレッドから呼べる。
	void ResumeAudioDeviceIfSuspended()
	{
		if (GEngine)
		{
			FAudioDeviceHandle AudioDevice = GEngine->GetMainAudioDevice();
			if (AudioDevice.IsValid())
			{
				AudioDevice->ResumeContext();
			}
		}
	}

	/**
	 * バックグラウンド監視スレッド。
	 * 500ms周期で、(1)音声デバイス再開 (2)メンテナンス実行 (3)ゲームスレッドポンプ を行う。
	 */
	class FKeepAliveWatchdog final : public FRunnable
	{
	public:
		FKeepAliveWatchdog()
		{
			WakeEvent = FPlatformProcess::GetSynchEventFromPool();
			Thread = FRunnableThread::Create(this, TEXT("ReaperKeepAliveWatchdog"), 0, TPri_Normal);
		}

		virtual ~FKeepAliveWatchdog() override
		{
			bStop = true;
			WakeEvent->Trigger();
			if (Thread)
			{
				Thread->Kill(true);   // Join
				delete Thread;
				Thread = nullptr;
			}
			FPlatformProcess::ReturnSynchEventToPool(WakeEvent);
			WakeEvent = nullptr;
		}

		virtual uint32 Run() override
		{
			int32 PumpCounter = 0;
			while (!bStop)
			{
				WakeEvent->Wait(FTimespan::FromMilliseconds(500));
				if (bStop) break;
				if (!GKeepAliveActive.Load() || !GInBackground.Load())
				{
					PumpCounter = 0;
					continue;
				}

				// 移行直後はゲームスレッドがまだ動いている可能性があるため、
				// 完全に停止するまで（約1.5秒）待ってから作業を始める。
				// これによりRPCクライアント等へのアクセスがGTと重ならないことを保証する。
				const double SinceBg = FPlatformTime::Seconds() - GBackgroundEnterTime.Load();
				if (SinceBg < 1.5) continue;

				// (1) 音声再生の再開（エンジンのPauseAudioで止まったOpenSL出力を戻す）
				ResumeAudioDeviceIfSuspended();

				// (2) メンテナンス（在席ハートビート等）。前面復帰と直列化する。
				{
					FScopeLock BatchGuard(&GBatchLock);
					if (GInBackground.Load() && GKeepAliveActive.Load())
					{
						TFunction<void()> Func;
						{
							FScopeLock Lock(&GMaintenanceLock);
							Func = GMaintenanceFunc;
						}
						if (Func) { Func(); }
					}
				}

				// (3) ゲームスレッドを1フレームだけ動かす（約2.5秒ごと）。
				// 停止中に溜まったAsyncTask（新規参加者の再生セットアップ、
				// モニターのフォーマット変更追従など）を処理させる。
				// メンテナンスと同じスレッドで直列に行うためRPCが重なることはない。
				if (++PumpCounter >= 5)
				{
					PumpCounter = 0;
					if (GInBackground.Load() && GKeepAliveActive.Load())
					{
						FScopeLock BatchGuard(&GBatchLock);
						FAppEventManager::GameThreadTicker Ticker;
						FPlatformProcess::Sleep(0.25f);
						// Tickerのデストラクタでゲームスレッドは再び停止する
					}
				}
			}
			return 0;
		}

		virtual void Stop() override { bStop = true; if (WakeEvent) { WakeEvent->Trigger(); } }

	private:
		FRunnableThread* Thread = nullptr;
		FEvent* WakeEvent = nullptr;
		TAtomic<bool> bStop { false };
	};

	FKeepAliveWatchdog* GWatchdog = nullptr;

	void RegisterAppStateDelegates()
	{
		if (GDelegatesRegistered) return;
		GDelegatesRegistered = true;

		// バックグラウンド移行（ゲームスレッド上・この後エンジンが音声停止→GT停止する）
		FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddLambda([]()
		{
			GBackgroundEnterTime.Store(FPlatformTime::Seconds());
			GInBackground.Store(true);
		});

		// 前面復帰（ゲームスレッド上）。監視スレッドのバッチが実行中なら完了を待って
		// から通常動作へ戻る（RPCクライアント等へのアクセスを直列化するため）。
		FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddLambda([]()
		{
			GInBackground.Store(false);
			FScopeLock BatchGuard(&GBatchLock);   // 実行中バッチの完了待ち
		});

		// 終了時に監視スレッドを確実に畳む
		FCoreDelegates::OnPreExit.AddLambda([]()
		{
			if (GWatchdog) { delete GWatchdog; GWatchdog = nullptr; }
		});
	}
#endif // PLATFORM_ANDROID
}

void FReaperKeepAlive::SetActive(bool bInActive)
{
#if PLATFORM_ANDROID
	check(IsInGameThread());
	RegisterAppStateDelegates();

	const bool bWasActive = GKeepAliveActive.Load();
	if (bWasActive == bInActive) return;
	GKeepAliveActive.Store(bInActive);

	if (bInActive)
	{
		CallKeepAliveThunk("AndroidThunkJava_StartKeepAliveService");
		if (!GWatchdog) { GWatchdog = new FKeepAliveWatchdog(); }
		UE_LOG(LogTemp, Log, TEXT("[RIGDRED KeepAlive] activated (foreground service + watchdog)"));
	}
	else
	{
		if (GWatchdog) { delete GWatchdog; GWatchdog = nullptr; }
		CallKeepAliveThunk("AndroidThunkJava_StopKeepAliveService");
		UE_LOG(LogTemp, Log, TEXT("[RIGDRED KeepAlive] deactivated"));
	}
#endif
}

void FReaperKeepAlive::SetBackgroundMaintenance(TFunction<void()> InFunc)
{
#if PLATFORM_ANDROID
	FScopeLock Lock(&GMaintenanceLock);
	GMaintenanceFunc = MoveTemp(InFunc);
#endif
}

bool FReaperKeepAlive::IsInBackground()
{
#if PLATFORM_ANDROID
	return GInBackground.Load();
#else
	return false;
#endif
}
