// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "Actions/Action_IRMeasurementPrepare.h"
#include "Actions/IRMeasurementSession.h"
#include "Actions/WG_IRMeasurementDialog.h"
#include "WG_RadNodzToolkit.h"
#include "ReaperLang.h"
#include "ReaperClasses.h"

#include "RadnodzRpcBatch.h"
#include "RigdocksSilver_Track.h"                 // AZ_RecordMonitorMode / AZ_InputChannelType
#include "Build/RigdocksSilver_Project_Build.h"
#include "Parse/RigdocksSilver_Project_Parse.h"
#include "Build/RigdocksSilver_Track_Build.h"
#include "Parse/RigdocksSilver_Track_Parse.h"
#include "Build/RigdocksSilver_Path_Build.h"
#include "Build/RigdocksSilver_Region_Build.h"
#include "Build/RigdocksBronze_MediaItem_Build.h"
#include "Parse/RigdocksBronze_MediaItem_Parse.h"
#include "Build/RigdocksGold_IR_Build.h"
#include "Parse/RigdocksGold_IR_Parse.h"

FString UAction_IRMeasurementPrepare::GetDisplayName() const
{
	return ReaperLang::S(TEXT("IR計測準備"), TEXT("Prepare IR Measurement"));
}

FString UAction_IRMeasurementPrepare::ErrorTitle()
{
	return ReaperLang::S(TEXT("IR計測準備"), TEXT("Prepare IR Measurement"));
}

void UAction_IRMeasurementPrepare::Execute(UWG_RadNodzToolkit* Owner)
{
	if (!Owner) { return; }

	if (!Owner->GetConnectedClient())
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
		return;
	}

	UWG_IRMeasurementDialog* Dlg = CreateWidget<UWG_IRMeasurementDialog>(Owner, UWG_IRMeasurementDialog::StaticClass());
	if (!Dlg)
	{
		Owner->EndActionBusy();
		return;
	}

	TWeakObjectPtr<UAction_IRMeasurementPrepare> WeakThis(this);
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	Dlg->Setup(
		[WeakThis, WeakOwner](int32 RepeatCount, int32 NumChannels, bool bLogTSP, double IntervalSec)
		{
			UWG_RadNodzToolkit* O = WeakOwner.Get();
			if (!O) { return; }
			if (UAction_IRMeasurementPrepare* Self = WeakThis.Get())
			{
				Self->StartPrepare(O, RepeatCount, NumChannels, bLogTSP, IntervalSec);
			}
			else
			{
				O->EndActionBusy();
			}
		},
		[WeakOwner]()
		{
			// キャンセル：何もせず終了
			if (UWG_RadNodzToolkit* O = WeakOwner.Get()) { O->EndActionBusy(); }
		});
	Dlg->AddToViewport(100);
}

void UAction_IRMeasurementPrepare::StartPrepare(UWG_RadNodzToolkit* Owner, int32 RepeatCount, int32 NumChannels, bool bLogTSP, double IntervalSec)
{
	UReaProject* Proj = Owner->GetCurrentProject();

	// 1段目：出力先（プロジェクトの録音フォルダ）・録音サンプルレート・トラック数を取得する
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	URigdocksRpcCall* CallPath  = URigdocksSilver_Project_Build::AZ_GetProjectPath_Build(Proj);
	URigdocksRpcCall* CallRate  = URigdocksSilver_Project_Build::AZ_GetRecordingSampleRate_Build(Proj);
	URigdocksRpcCall* CallCount = URigdocksSilver_Track_Build::AZ_GetTrackCountSelect_Build(Proj, /*trackType=全トラック*/0);
	Batch->Add(CallPath);
	Batch->Add(CallRate);
	Batch->Add(CallCount);

	TWeakObjectPtr<UAction_IRMeasurementPrepare> WeakThis(this);
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);
	const TArray<URigdocksRpcCall*> Calls = { CallPath, CallRate, CallCount };

	const bool bQueued = Owner->RunActionBatchAsync(Batch, /*bHeavy=*/false,
		[WeakThis, WeakOwner, Calls, CallPath, CallRate, CallCount, RepeatCount, NumChannels, bLogTSP, IntervalSec]()
		{
			UWG_RadNodzToolkit* O = WeakOwner.Get();
			if (!O) { return; }
			UAction_IRMeasurementPrepare* Self = WeakThis.Get();
			if (!Self) { O->EndActionBusy(); return; }

			FString Error;
			if (!CheckBatchResult(Calls, Error))
			{
				FinishWithError(O, ErrorTitle(), Error);
				return;
			}

			const FString ProjectPath = URigdocksSilver_Project_Parse::AZ_GetProjectPath_Parse(CallPath);
			if (ProjectPath.IsEmpty())
			{
				FinishWithError(O, ErrorTitle(), ReaperLang::S(
					TEXT("プロジェクトのフォルダを取得できませんでした。"),
					TEXT("Could not get the project folder.")));
				return;
			}

			// 録音サンプルレートが取れない（0）場合は 48kHz で生成する
			const double RateD = URigdocksSilver_Project_Parse::AZ_GetRecordingSampleRate_Parse(CallRate);
			const int32 SampleRate = RateD > 0.0 ? FMath::RoundToInt(RateD) : FallbackSampleRate;

			const int32 TrackCount = FMath::Max(0, URigdocksSilver_Track_Parse::AZ_GetTrackCountSelect_Parse(CallCount));

			UE_LOG(LogTemp, Log, TEXT("[Action] IR prepare: projectPath=%s sampleRate=%d trackCount=%d interval=%.1fs"),
				*ProjectPath, SampleRate, TrackCount, IntervalSec);

			Self->RunPrepareBatch(O, RepeatCount, NumChannels, bLogTSP, IntervalSec, ProjectPath, SampleRate, TrackCount);
		});

	if (!bQueued)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
	}
}

void UAction_IRMeasurementPrepare::RunPrepareBatch(UWG_RadNodzToolkit* Owner, int32 RepeatCount, int32 NumChannels, bool bLogTSP, double IntervalSec,
	const FString& ProjectPath, int32 SampleRate, int32 TrackCount)
{
	UReaProject* Proj = Owner->GetCurrentProject();

	// 出力先：<プロジェクトの録音フォルダ>/IR_<日時>/TSP.wav
	FString Parent = ProjectPath;
	while (Parent.EndsWith(TEXT("/")) || Parent.EndsWith(TEXT("\\"))) { Parent.LeftChopInline(1); }
	const FString FolderName = FString::Printf(TEXT("IR_%s"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
	const FString OutputFolder = Parent + TEXT("/") + FolderName;
	const FString TspOutputPath = OutputFolder + TEXT("/TSP.wav");

	// 2段目：すべてを1往復のバッチにまとめる。生成したトラック/アイテムは
	// BindRef で同一バッチ内の戻り値を後続の呼び出しへ渡す（失敗した時点で以降は実行されない）。
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<URigdocksRpcCall*> Calls;
	auto Add = [&](URigdocksRpcCall* Call) -> int32
	{
		Calls.Add(Call);
		return Batch->Add(Call);
	};

	// 出力フォルダを作る（TSP 生成 API はフォルダを作らないため）
	Add(URigdocksSilver_Path_Build::AZ_SetPathFolder_Build(Parent, FolderName, /*timeAddType=付与なし*/0));

	// TSP 信号を生成（戻り値: 総長, WAVパス, jsonパス）
	URigdocksRpcCall* CallGen = bLogTSP
		? URigdocksGold_IR_Build::AZ_GenerateLogTSP_Build(TspOutputPath, SampleRate, LogSweepSec, IntervalSec, RepeatCount, LogStartFreq, LogEndFreq)
		: URigdocksGold_IR_Build::AZ_GenerateLinearTSP_Build(TspOutputPath, SampleRate, LinearSweepSec, IntervalSec, RepeatCount);
	const int32 GenIdx = Add(CallGen);

	// 親トラック「IR Measurement」を末尾に追加。
	// 末尾追加時に REAPER が直前の開フォルダ配下へ入れてしまうため、最上位(階層0)に強制する。
	URigdocksRpcCall* CallParent = URigdocksSilver_Track_Build::AZ_InsertTrackId_Build(Proj, TrackCount, TEXT("IR Measurement"));
	const int32 ParentIdx = Add(CallParent);
	{
		URigdocksRpcCall* C = URigdocksSilver_Track_Build::AZ_SetTrackItemDepth_Build(nullptr, 0);
		C->BindRef(0, ParentIdx, 0);
		Add(C);
	}

	// 子トラックを親の配下に追加するヘルパー（戻り値: 追加呼び出しのバッチ内番号）
	auto AddChild = [&](const FString& Name, URigdocksRpcCall*& OutCall) -> int32
	{
		OutCall = URigdocksSilver_Track_Build::AZ_InsertChildTrack_Build(nullptr, Name);
		OutCall->BindRef(0, ParentIdx, 0);
		return Add(OutCall);
	};

	// 「TSP」（再生用）
	URigdocksRpcCall* CallTspTrack = nullptr;
	const int32 TspTrackIdx = AddChild(TEXT("TSP"), CallTspTrack);

	// 「ch. 1」～「ch. N」（録音用）：入力 n 番（モノラル）・録音時モニタリング OFF
	// （録音モードは「IR測定」が録音の間だけ入れる）
	TArray<URigdocksRpcCall*> CallChTracks;
	for (int32 i = 0; i < NumChannels; ++i)
	{
		URigdocksRpcCall* CallCh = nullptr;
		const int32 ChIdx = AddChild(FString::Printf(TEXT("ch. %d"), i + 1), CallCh);
		CallChTracks.Add(CallCh);

		URigdocksRpcCall* CallInput = URigdocksSilver_Track_Build::AZ_SetTrackItemInputChannel_Build(
			nullptr, /*firstChannelIndex=*/i, /*numChannels=*/1, AZ_InputChannelType::Audio);
		CallInput->BindRef(0, ChIdx, 0);
		Add(CallInput);

		URigdocksRpcCall* CallMon = URigdocksSilver_Track_Build::AZ_SetTrackItemRecordMonitoring_Build(nullptr, AZ_RecordMonitorMode::Off);
		CallMon->BindRef(0, ChIdx, 0);
		Add(CallMon);
	}

	// 「IR」（算出した IR の置き場所）。IR は ch 数分のチャンネルを持つので、トラックもそれに合わせる（偶数）。
	URigdocksRpcCall* CallIrTrack = nullptr;
	const int32 IrTrackIdx = AddChild(TEXT("IR"), CallIrTrack);
	if (NumChannels > 2)
	{
		URigdocksRpcCall* C = URigdocksSilver_Track_Build::AZ_SetTrackItemNumChannels_Build(nullptr, NumChannels + (NumChannels % 2));
		C->BindRef(0, IrTrackIdx, 0);
		Add(C);
	}

	// TSP 信号を「TSP」トラックのプロジェクト末尾＋1秒に挿入（startEndType≠0 ＝ プロジェクト末尾基準）
	URigdocksRpcCall* CallItem = URigdocksBronze_MediaItem_Build::AZ_InsertMediaTrackItemSecond_Build(
		nullptr, FString(), /*startEndType=末尾基準*/1, InsertOffsetFromEndSec);
	CallItem->BindRef(0, TspTrackIdx, 0);
	CallItem->BindRef(1, GenIdx, 1);   // 生成した WAV の絶対パス
	const int32 ItemIdx = Add(CallItem);

	// アイテムの範囲にリージョン「TSP」を作る
	{
		URigdocksRpcCall* C = URigdocksSilver_Region_Build::AZ_AddRegionMarkerMediaItem_Build(nullptr, TEXT("TSP"), /*setColor=*/0);
		C->BindRef(0, ItemIdx, 0);
		Add(C);
	}

	// 「IR算出」で録音を切り出すため、TSP アイテムの実際の位置と長さを取っておく
	URigdocksRpcCall* CallStart = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemStartTimeSeconds_Build(nullptr);
	CallStart->BindRef(0, ItemIdx, 0);
	Add(CallStart);
	URigdocksRpcCall* CallLen = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemLength_Build(nullptr);
	CallLen->BindRef(0, ItemIdx, 0);
	Add(CallLen);

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	const bool bQueued = Owner->RunActionBatchAsync(Batch, /*bHeavy=*/false,
		[WeakOwner, Calls, CallGen, CallParent, CallTspTrack, CallChTracks, CallIrTrack, CallItem, CallStart, CallLen,
		 bLogTSP, RepeatCount, OutputFolder]()
		{
			UWG_RadNodzToolkit* O = WeakOwner.Get();
			if (!O) { return; }

			FString Error;
			if (!CheckBatchResult(Calls, Error))
			{
				// 途中まで作られたトラックが残っている可能性があるので一覧は取り直す
				O->FetchTracks();
				FinishWithError(O, ErrorTitle(), Error);
				return;
			}

			FIRMeasurementSession Session;
			Session.bLogTSP      = bLogTSP;
			Session.RepeatCount  = RepeatCount;
			Session.OutputFolder = OutputFolder;

			double Duration = 0.0;
			if (bLogTSP)
			{
				URigdocksGold_IR_Parse::AZ_GenerateLogTSP_Parse(CallGen, Duration, Session.TspWavPath, Session.TspInfoPath);
			}
			else
			{
				URigdocksGold_IR_Parse::AZ_GenerateLinearTSP_Parse(CallGen, Duration, Session.TspWavPath, Session.TspInfoPath);
			}

			int32 TrackId = -1;
			Session.ParentTrack = URigdocksSilver_Track_Parse::AZ_InsertTrackId_Parse(CallParent);
			Session.TspTrack    = URigdocksSilver_Track_Parse::AZ_InsertChildTrack_Parse(CallTspTrack, TrackId);
			for (URigdocksRpcCall* C : CallChTracks)
			{
				Session.ChannelTracks.Add(URigdocksSilver_Track_Parse::AZ_InsertChildTrack_Parse(C, TrackId));
			}
			Session.IrTrack   = URigdocksSilver_Track_Parse::AZ_InsertChildTrack_Parse(CallIrTrack, TrackId);
			Session.TspItem   = URigdocksBronze_MediaItem_Parse::AZ_InsertMediaTrackItemSecond_Parse(CallItem);
			Session.TspStart  = URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemStartTimeSeconds_Parse(CallStart);
			Session.TspLength = URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemLength_Parse(CallLen);
			Session.bValid    = Session.TspItem != nullptr && Session.TspLength > 0.0 && !Session.TspInfoPath.IsEmpty();

			O->GetIRMeasurementSession() = Session;
			O->FetchTracks();

			if (!Session.bValid)
			{
				FinishWithError(O, ErrorTitle(), ReaperLang::S(
					TEXT("TSP 信号の挿入結果を取得できませんでした。"),
					TEXT("Could not get the inserted TSP signal.")));
				return;
			}

			const FString Msg = FString::Printf(TEXT("%s\n\n%s: %.3f s / %.3f s\n%s"),
				*ReaperLang::S(
					TEXT("IR計測の準備が完了しました。「IR測定」で録音し、録音後に「IR算出」を実行してください。"),
					TEXT("IR measurement is ready. Record with \"Record IR Measurement\", then run \"Calculate IR\".")),
				*ReaperLang::S(TEXT("TSP 位置 / 長さ"), TEXT("TSP position / length")),
				Session.TspStart, Session.TspLength,
				*Session.TspWavPath);
			O->ShowActionInfo(ErrorTitle(), Msg);
			O->EndActionBusy();
		});

	if (!bQueued)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
	}
}
