// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "Actions/Action_IRMeasurement.h"
#include "Actions/IRMeasurementSession.h"
#include "WG_RadNodzToolkit.h"
#include "ReaperLang.h"
#include "ReaperClasses.h"

#include "RadnodzRpcBatch.h"
#include "Build/RigdocksBronze_MediaItem_Build.h"
#include "Parse/RigdocksBronze_MediaItem_Parse.h"
#include "Build/RigdocksSilver_RenderTake_Build.h"
#include "Build/RigdocksGold_IR_Build.h"
#include "DataClass/FTSPRecordingGroup.h"

FString UAction_IRMeasurement::GetDisplayName() const
{
	return ReaperLang::S(TEXT("IR算出"), TEXT("Calculate IR"));
}

FString UAction_IRMeasurement::ErrorTitle()
{
	return ReaperLang::S(TEXT("IR算出"), TEXT("Calculate IR"));
}

void UAction_IRMeasurement::Execute(UWG_RadNodzToolkit* Owner)
{
	if (!Owner) { return; }

	if (!Owner->GetConnectedClient())
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
		return;
	}

	const FIRMeasurementSession& Session = Owner->GetIRMeasurementSession();
	if (!Session.bValid || Session.ChannelTracks.Num() == 0 || !Session.IrTrack)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(
			TEXT("先に「IR計測準備」を実行してください。"),
			TEXT("Run \"Prepare IR Measurement\" first.")));
		return;
	}

	// 1段目：各 ch トラックのアイテム一覧を取得する
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<URigdocksRpcCall*> Calls;
	for (UMediaTrack* Track : Session.ChannelTracks)
	{
		URigdocksRpcCall* C = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemListInTrackItem_Build(Track);
		Calls.Add(C);
		Batch->Add(C);
	}

	TWeakObjectPtr<UAction_IRMeasurement> WeakThis(this);
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	const bool bQueued = Owner->RunActionBatchAsync(Batch, /*bHeavy=*/false, [WeakThis, WeakOwner, Calls]()
	{
		UWG_RadNodzToolkit* O = WeakOwner.Get();
		if (!O) { return; }
		UAction_IRMeasurement* Self = WeakThis.Get();
		if (!Self) { O->EndActionBusy(); return; }

		FString Error;
		if (!CheckBatchResult(Calls, Error))
		{
			FinishWithError(O, ErrorTitle(), Error);
			return;
		}

		TArray<TArray<UMediaItem*>> ItemsPerChannel;
		for (URigdocksRpcCall* C : Calls)
		{
			ItemsPerChannel.Add(URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemListInTrackItem_Parse(C));
		}

		Self->FetchItemRanges(O, ItemsPerChannel);
	});

	if (!bQueued)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
	}
}

void UAction_IRMeasurement::FetchItemRanges(UWG_RadNodzToolkit* Owner, const TArray<TArray<UMediaItem*>>& ItemsPerChannel)
{
	// 2段目：全アイテムの位置・長さを1往復で取得する
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<URigdocksRpcCall*> Calls;

	// 平坦化した一覧（ch 番号・アイテム・位置/長さの呼び出し）
	TArray<int32>             ItemChannel;
	TArray<UMediaItem*>       Items;
	TArray<URigdocksRpcCall*> StartCalls;
	TArray<URigdocksRpcCall*> LengthCalls;
	TArray<UObject*>          KeepAlive;   // Parse で作られた一時 UObject（GC から守る）

	for (int32 Ch = 0; Ch < ItemsPerChannel.Num(); ++Ch)
	{
		for (UMediaItem* Item : ItemsPerChannel[Ch])
		{
			if (!Item) { continue; }

			URigdocksRpcCall* CallStart = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemStartTimeSeconds_Build(Item);
			URigdocksRpcCall* CallLen   = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemLength_Build(Item);
			Batch->Add(CallStart);
			Batch->Add(CallLen);
			Calls.Add(CallStart);
			Calls.Add(CallLen);

			ItemChannel.Add(Ch);
			Items.Add(Item);
			StartCalls.Add(CallStart);
			LengthCalls.Add(CallLen);
			KeepAlive.Add(Item);
		}
	}

	if (Batch->Num() == 0)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(
			TEXT("「ch. n」トラックに録音がありません。「IR測定」で録音してから実行してください。"),
			TEXT("No recordings on the \"ch. n\" tracks. Record with \"Record IR Measurement\" first.")));
		return;
	}

	const int32 NumChannels = ItemsPerChannel.Num();
	TWeakObjectPtr<UAction_IRMeasurement> WeakThis(this);
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	const bool bQueued = Owner->RunActionBatchAsync(Batch, /*bHeavy=*/false,
		[WeakThis, WeakOwner, Calls, ItemChannel, Items, StartCalls, LengthCalls, NumChannels]()
		{
			UWG_RadNodzToolkit* O = WeakOwner.Get();
			if (!O) { return; }
			UAction_IRMeasurement* Self = WeakThis.Get();
			if (!Self) { O->EndActionBusy(); return; }

			FString Error;
			if (!CheckBatchResult(Calls, Error))
			{
				FinishWithError(O, ErrorTitle(), Error);
				return;
			}

			const FIRMeasurementSession& Session = O->GetIRMeasurementSession();
			const double TspStart = Session.TspStart;
			const double TspEnd   = Session.TspStart + Session.TspLength;

			// ch ごとに、TSP の範囲を丸ごと含む録音アイテムを選ぶ。
			// 複数あれば一覧の後ろ（後から置かれたもの）を使う。
			TArray<UMediaItem*> Targets;
			TArray<double>      Starts;
			TArray<double>      Lengths;
			Targets.Init(nullptr, NumChannels);
			Starts.Init(0.0, NumChannels);
			Lengths.Init(0.0, NumChannels);

			for (int32 i = 0; i < Items.Num(); ++i)
			{
				const double Start = URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemStartTimeSeconds_Parse(StartCalls[i]);
				const double Len   = URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemLength_Parse(LengthCalls[i]);
				if (Start <= TspStart + PositionEpsilon && Start + Len >= TspEnd - PositionEpsilon)
				{
					const int32 Ch = ItemChannel[i];
					Targets[Ch] = Items[i];
					Starts[Ch]  = Start;
					Lengths[Ch] = Len;
				}
			}

			TArray<FString> Missing;
			for (int32 Ch = 0; Ch < NumChannels; ++Ch)
			{
				if (!Targets[Ch]) { Missing.Add(FString::Printf(TEXT("ch. %d"), Ch + 1)); }
			}
			if (Missing.Num() > 0)
			{
				FinishWithError(O, ErrorTitle(), FString::Printf(TEXT("%s\n%s"),
					*ReaperLang::S(
						TEXT("TSP の範囲を含む録音が見つからないトラックがあります。"),
						TEXT("Some tracks have no recording that covers the TSP range.")),
					*FString::Join(Missing, TEXT(", "))));
				return;
			}

			TArray<double> Ends;
			for (int32 Ch = 0; Ch < NumChannels; ++Ch) { Ends.Add(Starts[Ch] + Lengths[Ch]); }

			Self->RunTrimStep(O, /*Step=*/0, Targets, Starts, Ends, {});
		}, KeepAlive);

	if (!bQueued)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
	}
}

void UAction_IRMeasurement::RunTrimStep(UWG_RadNodzToolkit* Owner, int32 Step, const TArray<UMediaItem*>& KeepItems,
	const TArray<double>& Starts, const TArray<double>& Ends, const TArray<UMediaItem*>& DeleteItems)
{
	const FIRMeasurementSession& Session = Owner->GetIRMeasurementSession();
	const double TspStart = Session.TspStart;
	const double TspEnd   = Session.TspStart + Session.TspLength;

	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<URigdocksRpcCall*> Calls;
	auto Add = [&](URigdocksRpcCall* Call)
	{
		Calls.Add(Call);
		Batch->Add(Call);
	};

	if (Step == 0)
	{
		// 分割が他の ch のアイテムへ波及しないよう、先に全アイテムの選択を外す
		Add(URigdocksBronze_MediaItem_Build::AZ_SetAllMediaSelected_Build(Owner->GetCurrentProject(), false));
	}

	// 前の段で切り離した不要な片を削除する
	for (UMediaItem* Item : DeleteItems)
	{
		Add(URigdocksBronze_MediaItem_Build::AZ_DeleteMediaItem_Build(Item));
	}

	// ch ごとの分割（不要な ch は nullptr）。分割位置はアイテム先頭からの秒。
	TArray<URigdocksRpcCall*> SplitCalls;
	SplitCalls.Init(nullptr, KeepItems.Num());
	for (int32 Ch = 0; Ch < KeepItems.Num(); ++Ch)
	{
		if (Step == 0 && Starts[Ch] < TspStart - PositionEpsilon)
		{
			SplitCalls[Ch] = URigdocksBronze_MediaItem_Build::AZ_SplitMediaItem_Build(KeepItems[Ch], TspStart - Starts[Ch]);
		}
		else if (Step == 1 && Ends[Ch] > TspEnd + PositionEpsilon)
		{
			SplitCalls[Ch] = URigdocksBronze_MediaItem_Build::AZ_SplitMediaItem_Build(KeepItems[Ch], TspEnd - Starts[Ch]);
		}
		if (SplitCalls[Ch]) { Add(SplitCalls[Ch]); }
	}

	TArray<UObject*> KeepAlive;
	for (UMediaItem* Item : KeepItems)   { KeepAlive.Add(Item); }
	for (UMediaItem* Item : DeleteItems) { KeepAlive.Add(Item); }

	TWeakObjectPtr<UAction_IRMeasurement> WeakThis(this);
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	const bool bQueued = Owner->RunActionBatchAsync(Batch, /*bHeavy=*/false,
		[WeakThis, WeakOwner, Step, Calls, SplitCalls, KeepItems, Starts, Ends, TspStart, TspEnd]()
		{
			UWG_RadNodzToolkit* O = WeakOwner.Get();
			if (!O) { return; }
			UAction_IRMeasurement* Self = WeakThis.Get();
			if (!Self) { O->EndActionBusy(); return; }

			FString Error;
			if (!CheckBatchResult(Calls, Error))
			{
				O->FetchTracks();
				FinishWithError(O, ErrorTitle(), Error);
				return;
			}

			// 分割結果を取り出す：開始側（Step 0）は右を残して左を削除、終了側（Step 1）は左を残して右を削除
			TArray<UMediaItem*> NextKeep    = KeepItems;
			TArray<double>      NextStarts  = Starts;
			TArray<double>      NextEnds    = Ends;
			TArray<UMediaItem*> NextDelete;
			for (int32 Ch = 0; Ch < SplitCalls.Num(); ++Ch)
			{
				if (!SplitCalls[Ch]) { continue; }

				UMediaItem* Left  = nullptr;
				UMediaItem* Right = nullptr;
				URigdocksBronze_MediaItem_Parse::AZ_SplitMediaItem_Parse(SplitCalls[Ch], Left, Right);
				if (!Left || !Right)
				{
					O->FetchTracks();
					FinishWithError(O, ErrorTitle(), FString::Printf(TEXT("%s (ch. %d)"),
						*ReaperLang::S(TEXT("録音アイテムの分割に失敗しました。"), TEXT("Failed to split the recorded item.")), Ch + 1));
					return;
				}

				if (Step == 0)
				{
					NextKeep[Ch]   = Right;
					NextStarts[Ch] = TspStart;
					NextDelete.Add(Left);
				}
				else
				{
					NextKeep[Ch]   = Left;
					NextEnds[Ch]   = TspEnd;
					NextDelete.Add(Right);
				}
			}

			if (Step == 0)
			{
				Self->RunTrimStep(O, 1, NextKeep, NextStarts, NextEnds, NextDelete);
			}
			else
			{
				Self->RenderTrimmed(O, NextKeep, NextDelete);
			}
		}, KeepAlive);

	if (!bQueued)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
	}
}

void UAction_IRMeasurement::RenderTrimmed(UWG_RadNodzToolkit* Owner, const TArray<UMediaItem*>& KeepItems, const TArray<UMediaItem*>& DeleteItems)
{
	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	TArray<URigdocksRpcCall*> Calls;
	TArray<URigdocksRpcCall*> PathCalls;
	auto Add = [&](URigdocksRpcCall* Call)
	{
		Calls.Add(Call);
		Batch->Add(Call);
	};

	// TSP 終了より後ろの片を削除する
	for (UMediaItem* Item : DeleteItems)
	{
		Add(URigdocksBronze_MediaItem_Build::AZ_DeleteMediaItem_Build(Item));
	}

	for (UMediaItem* Item : KeepItems)
	{
		// 新規テイクにレンダリング（トリミング後の範囲だけのファイルになる）
		Add(URigdocksSilver_RenderTake_Build::AZ_RenderMediaItemActiveTake_Build(Item));

		// レンダリングで作られたテイク（アクティブテイク）のファイルパス
		URigdocksRpcCall* Path = URigdocksBronze_MediaItem_Build::AZ_GetMediaItemSourcePath_Build(Item);
		Add(Path);
		PathCalls.Add(Path);
	}

	TArray<UObject*> KeepAlive;
	for (UMediaItem* Item : KeepItems)   { KeepAlive.Add(Item); }
	for (UMediaItem* Item : DeleteItems) { KeepAlive.Add(Item); }

	TWeakObjectPtr<UAction_IRMeasurement> WeakThis(this);
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	const bool bQueued = Owner->RunActionBatchAsync(Batch, /*bHeavy=*/false, [WeakThis, WeakOwner, Calls, PathCalls]()
	{
		UWG_RadNodzToolkit* O = WeakOwner.Get();
		if (!O) { return; }
		UAction_IRMeasurement* Self = WeakThis.Get();
		if (!Self) { O->EndActionBusy(); return; }

		FString Error;
		if (!CheckBatchResult(Calls, Error))
		{
			O->FetchTracks();
			FinishWithError(O, ErrorTitle(), Error);
			return;
		}

		TArray<FString> RecordedPaths;
		for (URigdocksRpcCall* C : PathCalls)
		{
			RecordedPaths.Add(URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemSourcePath_Parse(C));
		}

		Self->CalcImpulseResponse(O, RecordedPaths);
	}, KeepAlive);

	if (!bQueued)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
	}
}

void UAction_IRMeasurement::CalcImpulseResponse(UWG_RadNodzToolkit* Owner, const TArray<FString>& RecordedPaths)
{
	const FIRMeasurementSession& Session = Owner->GetIRMeasurementSession();

	// ch ごとにモノラルで録っているので、グループを ch ごとに分けて多チャンネルの IR にまとめる
	TArray<FTSPRecordingGroup> Groups;
	for (const FString& Path : RecordedPaths)
	{
		FTSPRecordingGroup Group;
		Group.FilePaths.Add(Path);
		Groups.Add(Group);
	}

	const FString IrPath = FString::Printf(TEXT("%s/IR_%s.wav"),
		*Session.OutputFolder, *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));

	URigdocksRpcCall* Call = Session.bLogTSP
		? URigdocksGold_IR_Build::AZ_CalcImpulseResponseFromLogTSP_Build(Session.TspInfoPath, Groups, IrPath, /*irLength=*/0.0, /*trimStart=*/true, /*normalize=*/true)
		: URigdocksGold_IR_Build::AZ_CalcImpulseResponseFromLinearTSP_Build(Session.TspInfoPath, Groups, IrPath, /*irLength=*/0.0, /*trimStart=*/true, /*normalize=*/true);

	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	Batch->Add(Call);

	TWeakObjectPtr<UAction_IRMeasurement> WeakThis(this);
	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	// 算出は数秒かかることがあるので、操作系を待たせないよう解析用の接続で実行する
	const bool bQueued = Owner->RunActionBatchAsync(Batch, /*bHeavy=*/true, [WeakThis, WeakOwner, Call, IrPath]()
	{
		UWG_RadNodzToolkit* O = WeakOwner.Get();
		if (!O) { return; }
		UAction_IRMeasurement* Self = WeakThis.Get();
		if (!Self) { O->EndActionBusy(); return; }

		FString Error;
		if (!CheckBatchResult({ Call }, Error))
		{
			O->FetchTracks();
			FinishWithError(O, ErrorTitle(), Error);
			return;
		}

		Self->InsertImpulseResponse(O, IrPath);
	});

	if (!bQueued)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
	}
}

void UAction_IRMeasurement::InsertImpulseResponse(UWG_RadNodzToolkit* Owner, const FString& IrPath)
{
	const FIRMeasurementSession& Session = Owner->GetIRMeasurementSession();

	// 5段目：IR を「IR」トラックの TSP 開始位置へ挿入（startEndType=0 ＝ プロジェクト先頭からの絶対位置）
	URigdocksRpcCall* Call = URigdocksBronze_MediaItem_Build::AZ_InsertMediaTrackItemSecond_Build(
		Session.IrTrack, IrPath, /*startEndType=*/0, Session.TspStart);

	URadnodzRpcBatch* Batch = URadnodzRpcBatch::CreateRadnodzRpcBatch();
	Batch->Add(Call);

	TWeakObjectPtr<UWG_RadNodzToolkit> WeakOwner(Owner);

	const bool bQueued = Owner->RunActionBatchAsync(Batch, /*bHeavy=*/false, [WeakOwner, Call, IrPath]()
	{
		UWG_RadNodzToolkit* O = WeakOwner.Get();
		if (!O) { return; }

		O->FetchTracks();

		FString Error;
		if (!CheckBatchResult({ Call }, Error))
		{
			FinishWithError(O, ErrorTitle(), Error);
			return;
		}

		O->ShowActionInfo(ErrorTitle(), FString::Printf(TEXT("%s\n\n%s"),
			*ReaperLang::S(TEXT("IR を算出して「IR」トラックに挿入しました。"), TEXT("The IR was calculated and inserted on the \"IR\" track.")),
			*IrPath));
		O->EndActionBusy();
	});

	if (!bQueued)
	{
		FinishWithError(Owner, ErrorTitle(), ReaperLang::S(TEXT("REAPER に接続していません。"), TEXT("Not connected to REAPER.")));
	}
}
