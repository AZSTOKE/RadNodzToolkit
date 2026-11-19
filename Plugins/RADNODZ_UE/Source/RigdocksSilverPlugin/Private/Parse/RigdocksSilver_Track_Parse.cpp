// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Track_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include <string>

using namespace RADNODZ_client;

UMediaTrack* URigdocksSilver_Track_Parse::AZ_GetTrackItemSelect_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaTrack* track = SILVER::AZ_GetTrackItemSelect_Parse(call->Get());

	UMediaTrack* obj = NewObject<UMediaTrack>();
	obj->SetPointer((void*)track);
	return obj;
}

FString URigdocksSilver_Track_Parse::AZ_GetTrackItemName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string trackName = SILVER::AZ_GetTrackItemName_Parse(call->Get());
	return UTF8_TO_TCHAR(trackName.c_str());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemName_Parse(call->Get());
}

int URigdocksSilver_Track_Parse::AZ_GetTrackItemIndex_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetTrackItemIndex_Parse(call->Get());
}

UMediaTrack* URigdocksSilver_Track_Parse::AZ_GetSelectedTrackFirstInfo_Parse(URigdocksRpcCall* call, int& trackId)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaTrack* track = nullptr;
	std::tie(track, trackId) = SILVER::AZ_GetSelectedTrackFirstInfo_Parse(call->Get());

	UMediaTrack* trackObject = NewObject<UMediaTrack>();
	trackObject->SetPointer((void*)track);

	return trackObject;
}

UMediaTrack* URigdocksSilver_Track_Parse::AZ_InsertTrackSearchUniqueOnly_Parse(URigdocksRpcCall* call, int& trackId)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaTrack* track = nullptr;
	std::tie(track, trackId) = SILVER::AZ_InsertTrackSearchUniqueOnly_Parse(call->Get());

	UMediaTrack* trackObject = NewObject<UMediaTrack>();
	trackObject->SetPointer((void*)track);

	return trackObject;
}

int URigdocksSilver_Track_Parse::AZ_GetTrackCountSelect_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetTrackCountSelect_Parse(call->Get());
}

FString URigdocksSilver_Track_Parse::AZ_GetTrackIdName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string trackName = SILVER::AZ_GetTrackIdName_Parse(call->Get());
	return UTF8_TO_TCHAR(trackName.c_str());
}

FString URigdocksSilver_Track_Parse::AZ_GetSelectedTrackIdName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string trackName = SILVER::AZ_GetSelectedTrackIdName_Parse(call->Get());
	return UTF8_TO_TCHAR(trackName.c_str());
}

int URigdocksSilver_Track_Parse::AZ_GetTrackItemDepth_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetTrackItemDepth_Parse(call->Get());
}

int URigdocksSilver_Track_Parse::AZ_GetTrackIdDepth_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetTrackIdDepth_Parse(call->Get());
}

int URigdocksSilver_Track_Parse::AZ_GetSelectedTrackIdDepth_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetSelectedTrackIdDepth_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemDepth_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemDepth_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdDepth_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdDepth_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdDepth_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdDepth_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdName_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdName_Parse(call->Get());
}

bool URigdocksSilver_Track_Parse::AZ_GetTrackItemMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetTrackItemMute_Parse(call->Get());
}

bool URigdocksSilver_Track_Parse::AZ_GetTrackIdMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetTrackIdMute_Parse(call->Get());
}

bool URigdocksSilver_Track_Parse::AZ_GetSelectedTrackIdMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetSelectedTrackIdMute_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemMute_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdMute_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdMute_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackAllMuteOFF_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackAllMuteOFF_Parse(call->Get());
}

bool URigdocksSilver_Track_Parse::AZ_GetTrackItemSolo_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetTrackItemSolo_Parse(call->Get());
}

bool URigdocksSilver_Track_Parse::AZ_GetTrackIdSolo_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetTrackIdSolo_Parse(call->Get());
}

bool URigdocksSilver_Track_Parse::AZ_GetSelectedTrackIdSolo_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetSelectedTrackIdSolo_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemSolo_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemSolo_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdSolo_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdSolo_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdSolo_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdSolo_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackAllSoloOFF_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackAllSoloOFF_Parse(call->Get());
}

int URigdocksSilver_Track_Parse::AZ_GetTrackItemColor_Parse(URigdocksRpcCall* call, int& R, int& G, int& B)
{
	if (call == nullptr)
	{
		return 0;
	}

	int color = 0;
	std::tie(color, R, G, B) = SILVER::AZ_GetTrackItemColor_Parse(call->Get());

	return color;
}

int URigdocksSilver_Track_Parse::AZ_GetTrackIdColor_Parse(URigdocksRpcCall* call, int& R, int& G, int& B)
{
	if (call == nullptr)
	{
		return 0;
	}

	int color = 0;
	std::tie(color, R, G, B) = SILVER::AZ_GetTrackIdColor_Parse(call->Get());

	return color;
}

int URigdocksSilver_Track_Parse::AZ_GetSelectedTrackIdColor_Parse(URigdocksRpcCall* call, int& R, int& G, int& B)
{
	if (call == nullptr)
	{
		return 0;
	}

	int color = 0;
	std::tie(color, R, G, B) = SILVER::AZ_GetSelectedTrackIdColor_Parse(call->Get());

	return color;
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemColor_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemColor_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdColor_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdColor_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdColor_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdColor_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemRGB_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemRGB_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdRGB_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdRGB_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdRGB_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdRGB_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdSelected_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdSelected_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemOnlySelected_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemOnlySelected_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdOnlySelected_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdOnlySelected_Parse(call->Get());
}

double URigdocksSilver_Track_Parse::AZ_GetTrackItemVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return SILVER::AZ_GetTrackItemVolume_Parse(call->Get());
}

double URigdocksSilver_Track_Parse::AZ_GetTrackIdVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return SILVER::AZ_GetTrackIdVolume_Parse(call->Get());
}

double URigdocksSilver_Track_Parse::AZ_GetSelectedTrackIdVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return SILVER::AZ_GetSelectedTrackIdVolume_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemVolume_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdVolume_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdVolume_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemAddVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemAddVolume_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdAddVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdAddVolume_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdAddVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdAddVolume_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_DeleteTrackIdSelect_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_DeleteTrackIdSelect_Parse(call->Get());
}

TArray<UMediaTrack*> URigdocksSilver_Track_Parse::AZ_GetTrackItemList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<UMediaTrack*>();
	}

	std::vector<MediaTrack*> trackList = SILVER::AZ_GetTrackItemList_Parse(call->Get());

	TArray<UMediaTrack*> result;

	for (auto obj : trackList)
	{
		UMediaTrack* uObj = NewObject<UMediaTrack>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}

TArray<UMediaTrack*> URigdocksSilver_Track_Parse::AZ_GetTrackItemChildList_Parse(URigdocksRpcCall* call, int& trackCount)
{
	if (call == nullptr)
	{
		return TArray<UMediaTrack*>();
	}

	auto [trackList, count] = SILVER::AZ_GetTrackItemChildList_Parse(call->Get());
	trackCount = count;

	TArray<UMediaTrack*> result;

	for (auto obj : trackList)
	{
		UMediaTrack* uObj = NewObject<UMediaTrack>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}

TArray<UMediaTrack*> URigdocksSilver_Track_Parse::AZ_GetTrackIdChildList_Parse(URigdocksRpcCall* call, int& trackCount)
{
	if (call == nullptr)
	{
		return TArray<UMediaTrack*>();
	}

	auto [trackList, count] = SILVER::AZ_GetTrackIdChildList_Parse(call->Get());
	trackCount = count;

	TArray<UMediaTrack*> result;

	for (auto obj : trackList)
	{
		UMediaTrack* uObj = NewObject<UMediaTrack>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}

TArray<UMediaTrack*> URigdocksSilver_Track_Parse::AZ_GetSelectedTrackIdChildList_Parse(URigdocksRpcCall* call, int& trackCount)
{
	if (call == nullptr)
	{
		return TArray<UMediaTrack*>();
	}

	auto [trackList, count] = SILVER::AZ_GetSelectedTrackIdChildList_Parse(call->Get());
	trackCount = count;

	TArray<UMediaTrack*> result;

	for (auto obj : trackList)
	{
		UMediaTrack* uObj = NewObject<UMediaTrack>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FTrackDetail> URigdocksSilver_Track_Parse::AZ_GetTrackListDetail_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FTrackDetail>();
	}

	std::vector<TrackDetail> objArray = SILVER::AZ_GetTrackListDetail_Parse(call->Get());

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FTrackDetail> URigdocksSilver_Track_Parse::AZ_GetSelectedTrackListDetail_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FTrackDetail>();
	}

	std::vector<TrackDetail> objArray = SILVER::AZ_GetSelectedTrackListDetail_Parse(call->Get());

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FTrackDetail> URigdocksSilver_Track_Parse::AZ_GetTrackDepthList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FTrackDetail>();
	}

	std::vector<TrackDetail> objArray = SILVER::AZ_GetTrackDepthList_Parse(call->Get());

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FTrackDetail> URigdocksSilver_Track_Parse::AZ_GetSamePerfectTrackNameList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FTrackDetail>();
	}

	std::vector<TrackDetail> objArray = SILVER::AZ_GetSamePerfectTrackNameList_Parse(call->Get());

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FTrackDetail> URigdocksSilver_Track_Parse::AZ_GetIncludePerfectTrackNameList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FTrackDetail>();
	}

	std::vector<TrackDetail> objArray = SILVER::AZ_GetIncludePerfectTrackNameList_Parse(call->Get());

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<UMediaTrack*> URigdocksSilver_Track_Parse::AZ_GetSelectedTrackItemList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<UMediaTrack*>();
	}

	std::vector<MediaTrack*> trackList = SILVER::AZ_GetSelectedTrackItemList_Parse(call->Get());

	TArray<UMediaTrack*> result;

	for (auto obj : trackList)
	{
		UMediaTrack* uObj = NewObject<UMediaTrack>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}

UMediaTrack* URigdocksSilver_Track_Parse::AZ_InsertUniqueChildTrack_Parse(URigdocksRpcCall* call, int& trackId)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaTrack* track = nullptr;
	std::tie(track, trackId) = SILVER::AZ_InsertUniqueChildTrack_Parse(call->Get());

	UMediaTrack* trackObj = NewObject<UMediaTrack>();
	trackObj->SetPointer((void*)track);

	return trackObj;
}

UMediaTrack* URigdocksSilver_Track_Parse::AZ_InsertTrackId_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaTrack* track = SILVER::AZ_InsertTrackId_Parse(call->Get());

	UMediaTrack* trackObj = NewObject<UMediaTrack>();
	trackObj->SetPointer((void*)track);

	return trackObj;
}

UMediaTrack* URigdocksSilver_Track_Parse::AZ_InsertChildTrack_Parse(URigdocksRpcCall* call, int& trackId)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaTrack* track = nullptr;
	std::tie(track, trackId) = SILVER::AZ_InsertChildTrack_Parse(call->Get());

	UMediaTrack* trackObj = NewObject<UMediaTrack>();
	trackObj->SetPointer((void*)track);

	return trackObj;
}

bool URigdocksSilver_Track_Parse::AZ_GetTrackItemRecording_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetTrackItemRecording_Parse(call->Get());
}

bool URigdocksSilver_Track_Parse::AZ_GetTrackIdRecording_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetTrackIdRecording_Parse(call->Get());
}

bool URigdocksSilver_Track_Parse::AZ_GetSelectedTrackIdRecording_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetSelectedTrackIdRecording_Parse(call->Get());
}

TArray<FTrackDetail> URigdocksSilver_Track_Parse::AZ_GetRecordingTrackList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FTrackDetail>();
	}

	std::vector<TrackDetail> objArray = SILVER::AZ_GetRecordingTrackList_Parse(call->Get());

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemRecording_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemRecording_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdRecording_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdRecording_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdRecording_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdRecording_Parse(call->Get());
}

// -1 (failure) and unknown values are returned as Off
static AZ_RecordMonitorMode TrackParse_ToRecordMonitorMode(int value)
{
	return (value >= 0 && value <= 2) ? (AZ_RecordMonitorMode)value : AZ_RecordMonitorMode::Off;
}

AZ_RecordMonitorMode URigdocksSilver_Track_Parse::AZ_GetTrackItemRecordMonitoring_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return AZ_RecordMonitorMode::Off;
	}

	return TrackParse_ToRecordMonitorMode(SILVER::AZ_GetTrackItemRecordMonitoring_Parse(call->Get()));
}

AZ_RecordMonitorMode URigdocksSilver_Track_Parse::AZ_GetTrackIdRecordMonitoring_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return AZ_RecordMonitorMode::Off;
	}

	return TrackParse_ToRecordMonitorMode(SILVER::AZ_GetTrackIdRecordMonitoring_Parse(call->Get()));
}

AZ_RecordMonitorMode URigdocksSilver_Track_Parse::AZ_GetSelectedTrackIdRecordMonitoring_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return AZ_RecordMonitorMode::Off;
	}

	return TrackParse_ToRecordMonitorMode(SILVER::AZ_GetSelectedTrackIdRecordMonitoring_Parse(call->Get()));
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemRecordMonitoring_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemRecordMonitoring_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdRecordMonitoring_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdRecordMonitoring_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdRecordMonitoring_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdRecordMonitoring_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackAllRecording_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackAllRecording_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemOnlyRecording_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemOnlyRecording_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdOnlyRecording_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdOnlyRecording_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdOnlyRecording_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdOnlyRecording_Parse(call->Get());
}

double URigdocksSilver_Track_Parse::AZ_GetTrackItemCurrentPeak_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return SILVER::AZ_GetTrackItemCurrentPeak_Parse(call->Get());
}

double URigdocksSilver_Track_Parse::AZ_GetTrackItemCurrentPeakDB_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return SILVER::AZ_GetTrackItemCurrentPeakDB_Parse(call->Get());
}

double URigdocksSilver_Track_Parse::AZ_GetTrackItemHoldPeakDB_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return SILVER::AZ_GetTrackItemHoldPeakDB_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_ResetTrackItemHoldPeak_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_ResetTrackItemHoldPeak_Parse(call->Get());
}

int URigdocksSilver_Track_Parse::AZ_GetTrackItemNumChannels_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetTrackItemNumChannels_Parse(call->Get());
}

int URigdocksSilver_Track_Parse::AZ_GetTrackIdNumChannels_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetTrackIdNumChannels_Parse(call->Get());
}

int URigdocksSilver_Track_Parse::AZ_GetSelectedTrackIdNumChannels_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetSelectedTrackIdNumChannels_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemNumChannels_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemNumChannels_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdNumChannels_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdNumChannels_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdNumChannels_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdNumChannels_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_GetTrackItemInputChannel_Parse(URigdocksRpcCall* call, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType)
{
	if (call == nullptr)
	{
		return;
	}

	int _inputChType = 0;
	std::tie(firstChannelIndex, numChannels, _inputChType) = SILVER::AZ_GetTrackItemInputChannel_Parse(call->Get());

	inputChType = (AZ_InputChannelType)_inputChType;
}

void URigdocksSilver_Track_Parse::AZ_GetTrackIdInputChannel_Parse(URigdocksRpcCall* call, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType)
{
	if (call == nullptr)
	{
		return;
	}

	int _inputChType = 0;
	std::tie(firstChannelIndex, numChannels, _inputChType) = SILVER::AZ_GetTrackIdInputChannel_Parse(call->Get());

	inputChType = (AZ_InputChannelType)_inputChType;
}

void URigdocksSilver_Track_Parse::AZ_GetSelectedTrackIdInputChannel_Parse(URigdocksRpcCall* call, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType)
{
	if (call == nullptr)
	{
		return;
	}

	int _inputChType = 0;
	std::tie(firstChannelIndex, numChannels, _inputChType) = SILVER::AZ_GetSelectedTrackIdInputChannel_Parse(call->Get());

	inputChType = (AZ_InputChannelType)_inputChType;
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemInputChannel_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemInputChannel_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackIdInputChannel_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackIdInputChannel_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetSelectedTrackIdInputChannel_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedTrackIdInputChannel_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_GetTrackItemMeterMode_Parse(URigdocksRpcCall* call, AZ_TrackMeterMode& meterMode, bool& enabled, bool& lufsChannels1And2Only)
{
	if (call == nullptr)
	{
		return;
	}

	int _meterMode = 0;
	std::tie(_meterMode, enabled, lufsChannels1And2Only) = SILVER::AZ_GetTrackItemMeterMode_Parse(call->Get());

	meterMode = (AZ_TrackMeterMode)_meterMode;
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemMeterMode_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemMeterMode_Parse(call->Get());
}

bool URigdocksSilver_Track_Parse::AZ_GetTrackItemFxEnabled_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetTrackItemFxEnabled_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemFxEnabled_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemFxEnabled_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_GetTrackItemMainSend_Parse(URigdocksRpcCall* call, bool& sendParent, int& firstChIndex, int& numChs)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(sendParent, firstChIndex, numChs) = SILVER::AZ_GetTrackItemMainSend_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemMainSend_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemMainSend_Parse(call->Get());
}

TArray<FTrackDetail> URigdocksSilver_Track_Parse::AZ_GetTrackItemSendList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FTrackDetail>();
	}

	std::vector<TrackDetail> objArray = SILVER::AZ_GetTrackItemSendList_Parse(call->Get());

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

int URigdocksSilver_Track_Parse::AZ_AddTrackItemSend_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_AddTrackItemSend_Parse(call->Get());
}

FTrackSendProperties URigdocksSilver_Track_Parse::AZ_GetTrackItemSendProperties_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FTrackSendProperties();
	}

	TrackSendProperties obj = SILVER::AZ_GetTrackItemSendProperties_Parse(call->Get());

	FTrackSendProperties uObj;
	uObj.Initialize(obj);

	return uObj;
}

void URigdocksSilver_Track_Parse::AZ_SetTrackItemSendProperties_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetTrackItemSendProperties_Parse(call->Get());
}

void URigdocksSilver_Track_Parse::AZ_DeleteTrackItemSend_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_DeleteTrackItemSend_Parse(call->Get());
}
