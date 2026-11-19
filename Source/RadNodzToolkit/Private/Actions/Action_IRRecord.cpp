// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "Actions/Action_IRRecord.h"
#include "Actions/IRMeasurementSession.h"
#include "WG_RadNodzToolkit.h"
#include "ReaperLang.h"
#include "ReaperClasses.h"

#include "RadnodzRpcBatch.h"
#include "Build/RigdocksBronze_Cursor_Build.h"
#include "Parse/RigdocksBronze_Cursor_Parse.h"
#include "Build/RigdocksSilver_Project_Build.h"
#include "Parse/RigdocksSilver_Project_Parse.h"
#include "Build/RigdocksSilver_Track_Build.h"
#include "Containers/Ticker.h"
#include "HAL/PlatformTime.h"

FString UAction_IRRecord::GetDisplayName() const
{
	return ReaperLang::S(TEXT("IR測定"), TEXT("Record IR Measurement"));
}

FString UAction_IRRecord::ErrorTitle()
{
	return ReaperLang::S(TEXT("IR測定"), TEXT("Record IR Measurement"));
}

void UAction_IRRecord::Execute(UWG_RadNodzToolkit* Owner)
{
	if (!Owner) { return; }

	if (!Owner->GetConnectedClient())
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
		return;
	}

	const FIRMeasurementSession& Session = Owner->GetIRMeasurementSession();
	if (!Session.bValid || Session.ChannelTracks.Num() == 0)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(
			TEXT("先に「IR計測準備」を実行してください。"),
			TEXT("Run \"Prepare IR Measurement\" first.")));
		return;
	}

	const double StartPosition = FMath::Max(0.0, Session.TspStart - PreRollSec);
	StopPosition = Session.TspStart + Session.TspLength;
	DeadlineTime = FPlatformTime::Seconds() + (StopPosition - StartPosition) + TimeoutMarginSec;

	UReaProject* Proj = Owner->GetCurrentProject();

	// 停止 → 録音トラックを録音モード → TSP 開始 -0.5 秒へ移動 → 録音開始 を1往復で行う
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<URigdocksRpcCall*> Calls;
	auto Add = [&](URigdocksRpcCall* Call)
	{
		Calls.Add(Call);
		Batch->Add(Call);
	};

	Add(URigdocksBronze_Cursor_Build::AZ_StopPlaying_Build());   // 再生中だと録音開始が無視されるため
	for (UMediaTrack* Track : Session.ChannelTracks)
	{
		Add(URigdocksSilver_Track_Build::AZ_SetTrackItemRecording_Build(Track, true));
	}
	Add(URigdocksBronze_Cursor_Build::AZ_SetPlayCursorPosition_Build(Proj, /*startEndType=先頭基準*/0, StartPosition));
	Add(URigdocksBronze_Cursor_Build::AZ_StartRecording_Build());

	TWeakObjectPtr<UAction_IRRecord> WeakThis(this);
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	const bool bQueued = Owner->RunActionBatchAsync(Batch, /*bHeavy=*/false, [WeakThis, WeakOwner, Calls]()
	{
		UWG_RadNodzToolkit* O = WeakOwner.Get();
		if (!O) { return; }
		UAction_IRRecord* Self = WeakThis.Get();
		if (!Self) { O->EndActionBusy(); return; }

		FString Error;
		if (!CheckBatchResult(Calls, Error))
		{
			// 録音モードだけ入って録音が始まっていない可能性があるので、解除してから終える
			Self->StopRecording(O, /*bUserStopped=*/false);
			FinishWithError(O, ErrorTitle(), Error);
			return;
		}

		Self->SchedulePoll(O);
	});

	if (!bQueued)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
	}
}

void UAction_IRRecord::SchedulePoll(UWG_RadNodzToolkit* Owner)
{
	TWeakObjectPtr<UAction_IRRecord> WeakThis(this);
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([WeakThis, WeakOwner](float)
	{
		UWG_RadNodzToolkit* O = WeakOwner.Get();
		UAction_IRRecord* Self = WeakThis.Get();
		if (O && Self)
		{
			Self->Poll(O);
		}
		else if (O)
		{
			O->EndActionBusy();
		}
		return false;   // 1回だけ
	}), static_cast<float>(PollIntervalSec));
}

void UAction_IRRecord::Poll(UWG_RadNodzToolkit* Owner)
{
	UReaProject* Proj = Owner->GetCurrentProject();

	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	URigdocksRpcCall* CallPos = URigdocksBronze_Cursor_Build::AZ_GetPlayCursorPosition_Build(Proj);
	URigdocksRpcCall* CallRec = URigdocksSilver_Project_Build::AZ_GetRecordState_Build(Proj);
	Batch->Add(CallPos);
	Batch->Add(CallRec);

	TWeakObjectPtr<UAction_IRRecord> WeakThis(this);
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	const bool bQueued = Owner->RunActionBatchAsync(Batch, /*bHeavy=*/false, [WeakThis, WeakOwner, CallPos, CallRec]()
	{
		UWG_RadNodzToolkit* O = WeakOwner.Get();
		if (!O) { return; }
		UAction_IRRecord* Self = WeakThis.Get();
		if (!Self) { O->EndActionBusy(); return; }

		FString Error;
		if (!CheckBatchResult({ CallPos, CallRec }, Error))
		{
			Self->StopRecording(O, /*bUserStopped=*/false);
			FinishWithError(O, ErrorTitle(), Error);
			return;
		}

		// 録音が途中で止められた（REAPER 側で停止された等）
		if (!URigdocksSilver_Project_Parse::AZ_GetRecordState_Parse(CallRec))
		{
			Self->StopRecording(O, /*bUserStopped=*/true);
			return;
		}

		// TSP の終了位置を超えていれば停止命令を送る
		const double Position = URigdocksBronze_Cursor_Parse::AZ_GetPlayCursorPosition_Parse(CallPos);
		if (Position > Self->StopPosition)
		{
			Self->StopRecording(O, /*bUserStopped=*/false);
			return;
		}

		if (FPlatformTime::Seconds() > Self->DeadlineTime)
		{
			Self->StopRecording(O, /*bUserStopped=*/false);
			FinishWithError(O, ErrorTitle(), ReaperLang::S(
				TEXT("録音が予定の時間内に終了位置へ到達しなかったため停止しました。"),
				TEXT("Recording was stopped because it did not reach the end position in time.")));
			return;
		}

		Self->SchedulePoll(O);
	});

	if (!bQueued)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
	}
}

void UAction_IRRecord::StopRecording(UWG_RadNodzToolkit* Owner, bool bUserStopped)
{
	const FIRMeasurementSession& Session = Owner->GetIRMeasurementSession();

	// 停止 → 録音トラックの録音モードを解除
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<URigdocksRpcCall*> Calls;
	auto Add = [&](URigdocksRpcCall* Call)
	{
		Calls.Add(Call);
		Batch->Add(Call);
	};

	Add(URigdocksBronze_Cursor_Build::AZ_StopPlaying_Build());
	for (UMediaTrack* Track : Session.ChannelTracks)
	{
		Add(URigdocksSilver_Track_Build::AZ_SetTrackItemRecording_Build(Track, false));
	}

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	Owner->RunActionBatchAsync(Batch, /*bHeavy=*/false, [WeakOwner, Calls, bUserStopped]()
	{
		UWG_RadNodzToolkit* O = WeakOwner.Get();
		if (!O) { return; }

		O->FetchTracks();

		// エラー終了の途中で呼ばれた場合は、呼び出し側が FinishWithError でダイアログ表示と
		// 実行中の解除を済ませている。ここでは正常終了・途中停止のときだけ完了を知らせる。
		if (!O->IsActionBusy()) { return; }

		FString Error;
		if (!CheckBatchResult(Calls, Error))
		{
			FinishWithError(O, ErrorTitle(), Error);
			return;
		}

		O->ShowActionInfo(ErrorTitle(), bUserStopped
			? ReaperLang::S(
				TEXT("録音が途中で停止されました。TSP の範囲を録り切れていない場合は、もう一度「IR測定」を実行してください。"),
				TEXT("Recording was stopped midway. If the TSP range was not fully recorded, run \"Record IR Measurement\" again."))
			: ReaperLang::S(
				TEXT("録音が完了しました。「IR算出」を実行して IR を算出してください。"),
				TEXT("Recording finished. Run \"Calculate IR\" to calculate the IR.")));
		O->EndActionBusy();
	});
}
