// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Track.h"
#include "RIGDOCKS_SILVER.h"
#include "Object/TrackDetail.h"
#include "Object/TrackSendProperties.h"
#include "DataClass/FTrackDetail.h"

using namespace RADNODZ_client::Object;

UMediaTrack* URigdocksSilver_Track::AZ_GetTrackItemSelect(URadnodzClient* client, UReaProject* proj, int trackID, int trackType)
{
	MediaTrack* track = SILVER::AZ_GetTrackItemSelect(client->GetClient(), UReaProject::Get(proj), trackID, trackType);

	UMediaTrack* trackObject = NewObject<UMediaTrack>();
	trackObject->SetPointer((void*)track);

	return trackObject;
}

int URigdocksSilver_Track::AZ_GetTrackItemIndex(URadnodzClient* client, UMediaTrack* track)
{
	return SILVER::AZ_GetTrackItemIndex(client->GetClient(), UMediaTrack::Get(track));

}

UMediaTrack* URigdocksSilver_Track::AZ_GetSelectedTrackFirstInfo(URadnodzClient* client, UReaProject* proj, int& trackId)
{
	MediaTrack* track = nullptr;
	std::tie(track, trackId) = SILVER::AZ_GetSelectedTrackFirstInfo(client->GetClient(), UReaProject::Get(proj));

	UMediaTrack* trackObject = NewObject<UMediaTrack>();
	trackObject->SetPointer((void*)track);

	return trackObject;
}

UMediaTrack* URigdocksSilver_Track::AZ_InsertTrackSearchUniqueOnly(URadnodzClient* client, UReaProject* proj, FString setTrackName, int& trackId)
{
	MediaTrack* track = nullptr;
	std::tie(track, trackId) = SILVER::AZ_InsertTrackSearchUniqueOnly(client->GetClient(), UReaProject::Get(proj), TCHAR_TO_UTF8(*setTrackName));

	UMediaTrack* trackObject = NewObject<UMediaTrack>();
	trackObject->SetPointer((void*)track);

	return trackObject;
}

int URigdocksSilver_Track::AZ_GetTrackCountSelect(URadnodzClient* client, UReaProject* proj, int trackType)
{
	return SILVER::AZ_GetTrackCountSelect(client->GetClient(), UReaProject::Get(proj), trackType);

}

FString URigdocksSilver_Track::AZ_GetTrackItemName(URadnodzClient* client, UMediaTrack* track)
{
	std::string trackName = SILVER::AZ_GetTrackItemName(client->GetClient(), UMediaTrack::Get(track));

	return UTF8_TO_TCHAR(trackName.c_str());
}

FString URigdocksSilver_Track::AZ_GetTrackIdName(URadnodzClient* client, UReaProject* proj, int trackID)
{
	std::string trackName = SILVER::AZ_GetTrackIdName(client->GetClient(), UReaProject::Get(proj), trackID);

	return UTF8_TO_TCHAR(trackName.c_str());
}

FString URigdocksSilver_Track::AZ_GetSelectedTrackIdName(URadnodzClient* client, UReaProject* proj, int selectTrackID)
{
	std::string trackName = SILVER::AZ_GetSelectedTrackIdName(client->GetClient(), UReaProject::Get(proj), selectTrackID);

	return UTF8_TO_TCHAR(trackName.c_str());
}

int URigdocksSilver_Track::AZ_GetTrackItemDepth(URadnodzClient* client, UMediaTrack* track)
{
	return SILVER::AZ_GetTrackItemDepth(client->GetClient(), UMediaTrack::Get(track));

}

int URigdocksSilver_Track::AZ_GetTrackIdDepth(URadnodzClient* client, UReaProject* proj, int trackID)
{
	return SILVER::AZ_GetTrackIdDepth(client->GetClient(), UReaProject::Get(proj), trackID);

}

int URigdocksSilver_Track::AZ_GetSelectedTrackIdDepth(URadnodzClient* client, UReaProject* proj, int selectTrackID)
{
	return SILVER::AZ_GetSelectedTrackIdDepth(client->GetClient(), UReaProject::Get(proj), selectTrackID);

}

void URigdocksSilver_Track::AZ_SetTrackItemDepth(URadnodzClient* client, UMediaTrack* track, int setDepth)
{
	SILVER::AZ_SetTrackItemDepth(client->GetClient(), UMediaTrack::Get(track), setDepth);

}

void URigdocksSilver_Track::AZ_SetTrackIdDepth(URadnodzClient* client, UReaProject* proj, int trackID, int setDepth)
{
	SILVER::AZ_SetTrackIdDepth(client->GetClient(), UReaProject::Get(proj), trackID, setDepth);

}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdDepth(URadnodzClient* client, UReaProject* proj, int selectTrackID, int setDepth)
{
	SILVER::AZ_SetSelectedTrackIdDepth(client->GetClient(), UReaProject::Get(proj), selectTrackID, setDepth);

}

void URigdocksSilver_Track::AZ_SetTrackItemName(URadnodzClient* client, UMediaTrack* track, FString setName)
{
	SILVER::AZ_SetTrackItemName(client->GetClient(), UMediaTrack::Get(track), TCHAR_TO_UTF8(*setName));

}

void URigdocksSilver_Track::AZ_SetTrackIdName(URadnodzClient* client, UReaProject* proj, int trackID, FString setName)
{
	SILVER::AZ_SetTrackIdName(client->GetClient(), UReaProject::Get(proj), trackID, TCHAR_TO_UTF8(*setName));

}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdName(URadnodzClient* client, UReaProject* proj, int selectTrackID, FString setName)
{
	SILVER::AZ_SetSelectedTrackIdName(client->GetClient(), UReaProject::Get(proj), selectTrackID, TCHAR_TO_UTF8(*setName));

}

bool URigdocksSilver_Track::AZ_GetTrackItemMute(URadnodzClient* client, UMediaTrack* track)
{
	return SILVER::AZ_GetTrackItemMute(client->GetClient(), UMediaTrack::Get(track));

}

bool URigdocksSilver_Track::AZ_GetTrackIdMute(URadnodzClient* client, UReaProject* proj, int trackID)
{
	return SILVER::AZ_GetTrackIdMute(client->GetClient(), UReaProject::Get(proj), trackID);

}

bool URigdocksSilver_Track::AZ_GetSelectedTrackIdMute(URadnodzClient* client, UReaProject* proj, int selecteTrackID)
{
	return SILVER::AZ_GetSelectedTrackIdMute(client->GetClient(), UReaProject::Get(proj), selecteTrackID);

}

void URigdocksSilver_Track::AZ_SetTrackItemMute(URadnodzClient* client, UMediaTrack* track, bool setMute)
{
	SILVER::AZ_SetTrackItemMute(client->GetClient(), UMediaTrack::Get(track), setMute);

}

void URigdocksSilver_Track::AZ_SetTrackIdMute(URadnodzClient* client, UReaProject* proj, int trackID, bool setMute)
{
	SILVER::AZ_SetTrackIdMute(client->GetClient(), UReaProject::Get(proj), trackID, setMute);

}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdMute(URadnodzClient* client, UReaProject* proj, int selecteTrackID, bool setMute)
{
	SILVER::AZ_SetSelectedTrackIdMute(client->GetClient(), UReaProject::Get(proj), selecteTrackID, setMute);

}

void URigdocksSilver_Track::AZ_SetTrackAllMuteOFF(URadnodzClient* client, UReaProject* proj)
{
	SILVER::AZ_SetTrackAllMuteOFF(client->GetClient(), UReaProject::Get(proj));

}

bool URigdocksSilver_Track::AZ_GetTrackItemSolo(URadnodzClient* client, UMediaTrack* track)
{
	return SILVER::AZ_GetTrackItemSolo(client->GetClient(), UMediaTrack::Get(track));

}

bool URigdocksSilver_Track::AZ_GetTrackIdSolo(URadnodzClient* client, UReaProject* proj, int trackID)
{
	return SILVER::AZ_GetTrackIdSolo(client->GetClient(), UReaProject::Get(proj), trackID);

}

bool URigdocksSilver_Track::AZ_GetSelectedTrackIdSolo(URadnodzClient* client, UReaProject* proj, int selecteTrackID)
{
	return SILVER::AZ_GetSelectedTrackIdSolo(client->GetClient(), UReaProject::Get(proj), selecteTrackID);

}

void URigdocksSilver_Track::AZ_SetTrackItemSolo(URadnodzClient* client, UMediaTrack* track, bool setSolo)
{
	SILVER::AZ_SetTrackItemSolo(client->GetClient(), UMediaTrack::Get(track), setSolo);

}

void URigdocksSilver_Track::AZ_SetTrackIdSolo(URadnodzClient* client, UReaProject* proj, int trackID, bool setSolo)
{
	SILVER::AZ_SetTrackIdSolo(client->GetClient(), UReaProject::Get(proj), trackID, setSolo);

}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdSolo(URadnodzClient* client, UReaProject* proj, int selecteTrackID, bool setSolo)
{
	SILVER::AZ_SetSelectedTrackIdSolo(client->GetClient(), UReaProject::Get(proj), selecteTrackID, setSolo);

}

void URigdocksSilver_Track::AZ_SetTrackAllSoloOFF(URadnodzClient* client, UReaProject* proj)
{
	SILVER::AZ_SetTrackAllSoloOFF(client->GetClient(), UReaProject::Get(proj));

}

int URigdocksSilver_Track::AZ_GetTrackItemColor(URadnodzClient* client, UMediaTrack* track, int& R, int& G, int& B)
{
	int color = 0;
	std::tie(color, R, G, B) = SILVER::AZ_GetTrackItemColor(client->GetClient(), UMediaTrack::Get(track));

	return color;

}

int URigdocksSilver_Track::AZ_GetTrackIdColor(URadnodzClient* client, UReaProject* proj, int trackID, int& R, int& G, int& B)
{
	int color = 0;
	std::tie(color, R, G, B) = SILVER::AZ_GetTrackIdColor(client->GetClient(), UReaProject::Get(proj), trackID);

	return color;

}

int URigdocksSilver_Track::AZ_GetSelectedTrackIdColor(URadnodzClient* client, UReaProject* proj, int selectTrackID, int& R, int& G, int& B)
{
	int color = 0;
	std::tie(color, R, G, B) = SILVER::AZ_GetSelectedTrackIdColor(client->GetClient(), UReaProject::Get(proj), selectTrackID);

	return color;
}

void URigdocksSilver_Track::AZ_SetTrackItemColor(URadnodzClient* client, UMediaTrack* track, int setColor)
{
	SILVER::AZ_SetTrackItemColor(client->GetClient(), UMediaTrack::Get(track), setColor);

}

void URigdocksSilver_Track::AZ_SetTrackIdColor(URadnodzClient* client, UReaProject* proj, int trackID, int setColor)
{
	SILVER::AZ_SetTrackIdColor(client->GetClient(), UReaProject::Get(proj), trackID, setColor);

}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdColor(URadnodzClient* client, UReaProject* proj, int selectTrackID, int setColor)
{
	SILVER::AZ_SetSelectedTrackIdColor(client->GetClient(), UReaProject::Get(proj), selectTrackID, setColor);

}

void URigdocksSilver_Track::AZ_SetTrackItemRGB(URadnodzClient* client, UMediaTrack* track, int setR, int setG, int setB)
{
	SILVER::AZ_SetTrackItemRGB(client->GetClient(), UMediaTrack::Get(track), setR, setG, setB);

}

void URigdocksSilver_Track::AZ_SetTrackIdRGB(URadnodzClient* client, UReaProject* proj, int trackID, int setR, int setG, int setB)
{
	SILVER::AZ_SetTrackIdRGB(client->GetClient(), UReaProject::Get(proj), trackID, setR, setG, setB);

}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdRGB(URadnodzClient* client, UReaProject* proj, int selectTrackID, int setR, int setG, int setB)
{
	SILVER::AZ_SetSelectedTrackIdRGB(client->GetClient(), UReaProject::Get(proj), selectTrackID, setR, setG, setB);

}

void URigdocksSilver_Track::AZ_SetTrackIdSelected(URadnodzClient* client, UReaProject* proj, int trackID, bool setSelectType)
{
	SILVER::AZ_SetTrackIdSelected(client->GetClient(), UReaProject::Get(proj), trackID, setSelectType);

}

void URigdocksSilver_Track::AZ_SetTrackItemOnlySelected(URadnodzClient* client, UMediaTrack* track)
{
	SILVER::AZ_SetTrackItemOnlySelected(client->GetClient(), UMediaTrack::Get(track));

}

void URigdocksSilver_Track::AZ_SetTrackIdOnlySelected(URadnodzClient* client, UReaProject* proj, int trackID)
{
	SILVER::AZ_SetTrackIdOnlySelected(client->GetClient(), UReaProject::Get(proj), trackID);

}

double URigdocksSilver_Track::AZ_GetTrackItemVolume(URadnodzClient* client, UMediaTrack* track)
{
	return SILVER::AZ_GetTrackItemVolume(client->GetClient(), UMediaTrack::Get(track));

}

double URigdocksSilver_Track::AZ_GetTrackIdVolume(URadnodzClient* client, UReaProject* proj, int trackID)
{
	return SILVER::AZ_GetTrackIdVolume(client->GetClient(), UReaProject::Get(proj), trackID);

}

double URigdocksSilver_Track::AZ_GetSelectedTrackIdVolume(URadnodzClient* client, UReaProject* proj, int selectTrackID)
{
	return SILVER::AZ_GetSelectedTrackIdVolume(client->GetClient(), UReaProject::Get(proj), selectTrackID);

}

void URigdocksSilver_Track::AZ_SetTrackItemVolume(URadnodzClient* client, UMediaTrack* track, double setVolume)
{
	SILVER::AZ_SetTrackItemVolume(client->GetClient(), UMediaTrack::Get(track), setVolume);

}

void URigdocksSilver_Track::AZ_SetTrackIdVolume(URadnodzClient* client, UReaProject* proj, int trackID, double setVolume)
{
	SILVER::AZ_SetTrackIdVolume(client->GetClient(), UReaProject::Get(proj), trackID, setVolume);

}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdVolume(URadnodzClient* client, UReaProject* proj, int trackID, double setVolume)
{
	SILVER::AZ_SetSelectedTrackIdVolume(client->GetClient(), UReaProject::Get(proj), trackID, setVolume);

}

void URigdocksSilver_Track::AZ_SetTrackItemAddVolume(URadnodzClient* client, UMediaTrack* track, double addVolume)
{
	SILVER::AZ_SetTrackItemAddVolume(client->GetClient(), UMediaTrack::Get(track), addVolume);

}

void URigdocksSilver_Track::AZ_SetTrackIdAddVolume(URadnodzClient* client, UReaProject* proj, int trackID, double addVolume)
{
	SILVER::AZ_SetTrackIdAddVolume(client->GetClient(), UReaProject::Get(proj), trackID, addVolume);

}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdAddVolume(URadnodzClient* client, UReaProject* proj, int trackID, double addVolume)
{
	SILVER::AZ_SetSelectedTrackIdAddVolume(client->GetClient(), UReaProject::Get(proj), trackID, addVolume);

}

void URigdocksSilver_Track::AZ_DeleteTrackIdSelect(URadnodzClient* client, UReaProject* proj, int trackID, int trackType)
{
	SILVER::AZ_DeleteTrackIdSelect(client->GetClient(), UReaProject::Get(proj), trackID, trackType);

}

TArray<UMediaTrack*> URigdocksSilver_Track::AZ_GetTrackItemList(URadnodzClient* client, UReaProject* project)
{
	std::vector<MediaTrack*> trackList = SILVER::AZ_GetTrackItemList(client->GetClient(), UReaProject::Get(project));

	TArray<UMediaTrack*> result;

	for (auto obj : trackList)
	{
		UMediaTrack* uObj = NewObject<UMediaTrack>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}

TArray<UMediaTrack*>  URigdocksSilver_Track::AZ_GetTrackItemChildList(URadnodzClient* client, UMediaTrack* track, bool isRecursive, int& trackCount)
{
	auto[trackList, count] = SILVER::AZ_GetTrackItemChildList(client->GetClient(), UMediaTrack::Get(track), isRecursive);
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

TArray<UMediaTrack*> URigdocksSilver_Track::AZ_GetTrackIdChildList(URadnodzClient* client, UReaProject* project, int trackID, bool isRecursive, int& trackCount)
{
	auto [trackList, count] = SILVER::AZ_GetTrackIdChildList(client->GetClient(), UReaProject::Get(project), trackID, isRecursive);
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

TArray<UMediaTrack*> URigdocksSilver_Track::AZ_GetSelectedTrackIdChildList(URadnodzClient* client, UReaProject* project, int selectTrackID, bool isRecursive, int& trackCount)
{
	auto [trackList, count] = SILVER::AZ_GetSelectedTrackIdChildList(client->GetClient(), UReaProject::Get(project), selectTrackID, isRecursive);
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

TArray<FTrackDetail> URigdocksSilver_Track::AZ_GetTrackListDetail(URadnodzClient* client, UReaProject* project)
{
	std::vector<TrackDetail> objArray = SILVER::AZ_GetTrackListDetail(client->GetClient(), UReaProject::Get(project));

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FTrackDetail> URigdocksSilver_Track::AZ_GetSelectedTrackListDetail(URadnodzClient* client, UReaProject* project)
{
	std::vector<TrackDetail> objArray = SILVER::AZ_GetSelectedTrackListDetail(client->GetClient(), UReaProject::Get(project));

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FTrackDetail> URigdocksSilver_Track::AZ_GetTrackDepthList(URadnodzClient* client, UReaProject* project)
{
	std::vector<TrackDetail> objArray = SILVER::AZ_GetTrackDepthList(client->GetClient(), UReaProject::Get(project));

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FTrackDetail> URigdocksSilver_Track::AZ_GetSamePerfectTrackNameList(URadnodzClient* client, UReaProject* project, FString _checkName)
{
	std::vector<TrackDetail> objArray = SILVER::AZ_GetSamePerfectTrackNameList(client->GetClient(), UReaProject::Get(project), TCHAR_TO_UTF8(*_checkName));

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FTrackDetail> URigdocksSilver_Track::AZ_GetIncludePerfectTrackNameList(URadnodzClient* client, UReaProject* project, FString _checkName)
{
	std::vector<TrackDetail> objArray = SILVER::AZ_GetIncludePerfectTrackNameList(client->GetClient(), UReaProject::Get(project), TCHAR_TO_UTF8(*_checkName));

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<UMediaTrack*> URigdocksSilver_Track::AZ_GetSelectedTrackItemList(URadnodzClient* client, UReaProject* project)
{
	std::vector<MediaTrack*> trackList = SILVER::AZ_GetSelectedTrackItemList(client->GetClient(), UReaProject::Get(project));

	TArray<UMediaTrack*> result;

	for (auto obj : trackList)
	{
		UMediaTrack* uObj = NewObject<UMediaTrack>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}
	return result;
}

UMediaTrack* URigdocksSilver_Track::AZ_InsertUniqueChildTrack(URadnodzClient* client, UMediaTrack* parentTrack, FString setName, int& trackId)
{
	MediaTrack* track = nullptr;
	std::tie(track, trackId) = SILVER::AZ_InsertUniqueChildTrack(client->GetClient(), UMediaTrack::Get(parentTrack), TCHAR_TO_UTF8(*setName));

	UMediaTrack* trackObj = NewObject<UMediaTrack>();
	trackObj->SetPointer((void*)track);

	return trackObj;
}

UMediaTrack* URigdocksSilver_Track::AZ_InsertTrackId(URadnodzClient* client, UReaProject* proj, int trackID, FString setName)
{
	MediaTrack* track = SILVER::AZ_InsertTrackId(client->GetClient(), UReaProject::Get(proj), trackID, TCHAR_TO_UTF8(*setName));

	UMediaTrack* trackObj = NewObject<UMediaTrack>();
	trackObj->SetPointer((void*)track);

	return trackObj;
}

UMediaTrack* URigdocksSilver_Track::AZ_InsertChildTrack(URadnodzClient* client, UMediaTrack* parentTrack, FString setName, int& trackId)
{
	MediaTrack* track = nullptr;
	
	std::tie(track, trackId) = SILVER::AZ_InsertChildTrack(client->GetClient(), UMediaTrack::Get(parentTrack), TCHAR_TO_UTF8(*setName));

	UMediaTrack* trackObj = NewObject<UMediaTrack>();
	trackObj->SetPointer((void*)track);

	return trackObj;
}

bool URigdocksSilver_Track::AZ_GetTrackItemRecording(URadnodzClient* client, UMediaTrack* track)
{
	return SILVER::AZ_GetTrackItemRecording(client->GetClient(), UMediaTrack::Get(track));
}

bool URigdocksSilver_Track::AZ_GetTrackIdRecording(URadnodzClient* client, UReaProject* projID, int trackID)
{
	return SILVER::AZ_GetTrackIdRecording(client->GetClient(), UReaProject::Get(projID), trackID);
}

bool URigdocksSilver_Track::AZ_GetSelectedTrackIdRecording(URadnodzClient* client, UReaProject* projID, int selectTrackID)
{
	return SILVER::AZ_GetSelectedTrackIdRecording(client->GetClient(), UReaProject::Get(projID), selectTrackID);
}

TArray<FTrackDetail> URigdocksSilver_Track::AZ_GetRecordingTrackList(URadnodzClient* client, UReaProject* projID)
{
	std::vector<TrackDetail> objArray = SILVER::AZ_GetRecordingTrackList(client->GetClient(), UReaProject::Get(projID));

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

void URigdocksSilver_Track::AZ_SetTrackItemRecording(URadnodzClient* client, UMediaTrack* track, bool setRec)
{
	SILVER::AZ_SetTrackItemRecording(client->GetClient(), UMediaTrack::Get(track), setRec);
}

void URigdocksSilver_Track::AZ_SetTrackIdRecording(URadnodzClient* client, UReaProject* projID, int trackID, bool setRec)
{
	SILVER::AZ_SetTrackIdRecording(client->GetClient(), UReaProject::Get(projID), trackID, setRec);
}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdRecording(URadnodzClient* client, UReaProject* projID, int selectTrackID, bool setRec)
{
	SILVER::AZ_SetSelectedTrackIdRecording(client->GetClient(), UReaProject::Get(projID), selectTrackID, setRec);
}

// -1 (failure) and unknown values are returned as Off
static AZ_RecordMonitorMode ToRecordMonitorMode(int value)
{
	return (value >= 0 && value <= 2) ? (AZ_RecordMonitorMode)value : AZ_RecordMonitorMode::Off;
}

AZ_RecordMonitorMode URigdocksSilver_Track::AZ_GetTrackItemRecordMonitoring(URadnodzClient* client, UMediaTrack* track)
{
	return ToRecordMonitorMode(SILVER::AZ_GetTrackItemRecordMonitoring(client->GetClient(), UMediaTrack::Get(track)));
}

AZ_RecordMonitorMode URigdocksSilver_Track::AZ_GetTrackIdRecordMonitoring(URadnodzClient* client, UReaProject* projID, int trackID)
{
	return ToRecordMonitorMode(SILVER::AZ_GetTrackIdRecordMonitoring(client->GetClient(), UReaProject::Get(projID), trackID));
}

AZ_RecordMonitorMode URigdocksSilver_Track::AZ_GetSelectedTrackIdRecordMonitoring(URadnodzClient* client, UReaProject* projID, int selectTrackID)
{
	return ToRecordMonitorMode(SILVER::AZ_GetSelectedTrackIdRecordMonitoring(client->GetClient(), UReaProject::Get(projID), selectTrackID));
}

void URigdocksSilver_Track::AZ_SetTrackItemRecordMonitoring(URadnodzClient* client, UMediaTrack* track, AZ_RecordMonitorMode monitorMode)
{
	SILVER::AZ_SetTrackItemRecordMonitoring(client->GetClient(), UMediaTrack::Get(track), (int)monitorMode);
}

void URigdocksSilver_Track::AZ_SetTrackIdRecordMonitoring(URadnodzClient* client, UReaProject* projID, int trackID, AZ_RecordMonitorMode monitorMode)
{
	SILVER::AZ_SetTrackIdRecordMonitoring(client->GetClient(), UReaProject::Get(projID), trackID, (int)monitorMode);
}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdRecordMonitoring(URadnodzClient* client, UReaProject* projID, int selectTrackID, AZ_RecordMonitorMode monitorMode)
{
	SILVER::AZ_SetSelectedTrackIdRecordMonitoring(client->GetClient(), UReaProject::Get(projID), selectTrackID, (int)monitorMode);
}

void URigdocksSilver_Track::AZ_SetTrackAllRecording(URadnodzClient* client, UReaProject* projID, bool setRec)
{
	SILVER::AZ_SetTrackAllRecording(client->GetClient(), UReaProject::Get(projID), setRec);
}

void URigdocksSilver_Track::AZ_SetTrackItemOnlyRecording(URadnodzClient* client, UMediaTrack* track, bool setRec)
{
	SILVER::AZ_SetTrackItemOnlyRecording(client->GetClient(), UMediaTrack::Get(track), setRec);
}

void URigdocksSilver_Track::AZ_SetTrackIdOnlyRecording(URadnodzClient* client, UReaProject* projID, int trackID, bool setRec)
{
	SILVER::AZ_SetTrackIdOnlyRecording(client->GetClient(), UReaProject::Get(projID), trackID, setRec);
}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdOnlyRecording(URadnodzClient* client, UReaProject* projID, int selectedTrackID, bool setRec)
{
	SILVER::AZ_SetSelectedTrackIdOnlyRecording(client->GetClient(), UReaProject::Get(projID), selectedTrackID, setRec);
}

double URigdocksSilver_Track::AZ_GetTrackItemCurrentPeak(URadnodzClient* client, UMediaTrack* track, int chIndex)
{
	return SILVER::AZ_GetTrackItemCurrentPeak(client->GetClient(), UMediaTrack::Get(track), chIndex);
}

double URigdocksSilver_Track::AZ_GetTrackItemCurrentPeakDB(URadnodzClient* client, UMediaTrack* track, int chIndex)
{
	return SILVER::AZ_GetTrackItemCurrentPeakDB(client->GetClient(), UMediaTrack::Get(track), chIndex);
}

double URigdocksSilver_Track::AZ_GetTrackItemHoldPeakDB(URadnodzClient* client, UMediaTrack* track, int chIndex)
{
	return SILVER::AZ_GetTrackItemHoldPeakDB(client->GetClient(), UMediaTrack::Get(track), chIndex);
}

void URigdocksSilver_Track::AZ_ResetTrackItemHoldPeak(URadnodzClient* client, UMediaTrack* track, int chIndex)
{
	SILVER::AZ_ResetTrackItemHoldPeak(client->GetClient(), UMediaTrack::Get(track), chIndex);
}

int URigdocksSilver_Track::AZ_GetTrackItemNumChannels(URadnodzClient* client, UMediaTrack* track)
{
	return SILVER::AZ_GetTrackItemNumChannels(client->GetClient(), UMediaTrack::Get(track));
}

int URigdocksSilver_Track::AZ_GetTrackIdNumChannels(URadnodzClient* client, UReaProject* projID, int trackID)
{
	return SILVER::AZ_GetTrackIdNumChannels(client->GetClient(), UReaProject::Get(projID), trackID);
}

int URigdocksSilver_Track::AZ_GetSelectedTrackIdNumChannels(URadnodzClient* client, UReaProject* projID, int selectedTrackID)
{
	return SILVER::AZ_GetSelectedTrackIdNumChannels(client->GetClient(), UReaProject::Get(projID), selectedTrackID);
}

void URigdocksSilver_Track::AZ_SetTrackItemNumChannels(URadnodzClient* client, UMediaTrack* track, int numChs)
{
     SILVER::AZ_SetTrackItemNumChannels(client->GetClient(), UMediaTrack::Get(track), numChs);
}

void URigdocksSilver_Track::AZ_SetTrackIdNumChannels(URadnodzClient* client, UReaProject* projID, int trackID, int numChs)
{
	SILVER::AZ_SetTrackIdNumChannels(client->GetClient(), UReaProject::Get(projID), trackID, numChs);
}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdNumChannels(URadnodzClient* client, UReaProject* projID, int selectedTrackID, int numChs)
{
	SILVER::AZ_SetSelectedTrackIdNumChannels(client->GetClient(), UReaProject::Get(projID), selectedTrackID, numChs);
}

void URigdocksSilver_Track::AZ_GetTrackItemInputChannel(URadnodzClient* client, UMediaTrack* track, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType)
{
	int _inputChType = 0;
	std::tie(firstChannelIndex, numChannels, _inputChType)  = SILVER::AZ_GetTrackItemInputChannel(client->GetClient(), UMediaTrack::Get(track));

	inputChType = (AZ_InputChannelType)_inputChType;
}

void URigdocksSilver_Track::AZ_GetTrackIdInputChannel(URadnodzClient* client, UReaProject* projID, int trackID, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType)
{
	int _inputChType = 0;
	std::tie(firstChannelIndex, numChannels, _inputChType) = SILVER::AZ_GetTrackIdInputChannel(client->GetClient(), UReaProject::Get(projID), trackID);

	inputChType = (AZ_InputChannelType)_inputChType;
}

void URigdocksSilver_Track::AZ_GetSelectedTrackIdInputChannel(URadnodzClient* client, UReaProject* projID, int selectedTrackID, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType)
{
	int _inputChType = 0;
	std::tie(firstChannelIndex, numChannels, _inputChType) = SILVER::AZ_GetSelectedTrackIdInputChannel(client->GetClient(), UReaProject::Get(projID), selectedTrackID);

	inputChType = (AZ_InputChannelType)_inputChType;
}

void URigdocksSilver_Track::AZ_SetTrackItemInputChannel(URadnodzClient* client, UMediaTrack* track, int firstChannelIndex, int numChannels, AZ_InputChannelType inputChType)
{
	SILVER::AZ_SetTrackItemInputChannel(client->GetClient(), UMediaTrack::Get(track), firstChannelIndex, numChannels, (int)inputChType);
}

void URigdocksSilver_Track::AZ_SetTrackIdInputChannel(URadnodzClient* client, UReaProject* projID, int trackID, int firstChannelIndex, int numChannels, AZ_InputChannelType inputChType)
{
	SILVER::AZ_SetTrackIdInputChannel(client->GetClient(), UReaProject::Get(projID), trackID, firstChannelIndex, numChannels, (int)inputChType);
}

void URigdocksSilver_Track::AZ_SetSelectedTrackIdInputChannel(URadnodzClient* client, UReaProject* projID, int selectedTrackID, int firstChannelIndex, int numChannels, AZ_InputChannelType inputChType)
{
	SILVER::AZ_SetSelectedTrackIdInputChannel(client->GetClient(), UReaProject::Get(projID), selectedTrackID, firstChannelIndex, numChannels, (int)inputChType);
}

void URigdocksSilver_Track::AZ_GetTrackItemMeterMode(URadnodzClient* client, UMediaTrack* track, AZ_TrackMeterMode& meterMode, bool& enabled, bool& lufsChannels1And2Only)
{
	int _meterMode = 0;
	std::tie(_meterMode, enabled, lufsChannels1And2Only) = SILVER::AZ_GetTrackItemMeterMode(client->GetClient(), UMediaTrack::Get(track));

	meterMode = (AZ_TrackMeterMode)_meterMode;
}

void URigdocksSilver_Track::AZ_SetTrackItemMeterMode(URadnodzClient* client, UMediaTrack* track, AZ_TrackMeterMode meterMode, bool enabled, bool lufsChannels1And2Only)
{
	SILVER::AZ_SetTrackItemMeterMode(client->GetClient(), UMediaTrack::Get(track), (int)meterMode, enabled, lufsChannels1And2Only);
}

bool URigdocksSilver_Track::AZ_GetTrackItemFxEnabled(URadnodzClient* client, UMediaTrack* track, int fxIndex)
{
	return SILVER::AZ_GetTrackItemFxEnabled(client->GetClient(), UMediaTrack::Get(track), fxIndex);
}

void URigdocksSilver_Track::AZ_SetTrackItemFxEnabled(URadnodzClient* client, UMediaTrack* track, int fxIndex, bool enabled)
{
	SILVER::AZ_SetTrackItemFxEnabled(client->GetClient(), UMediaTrack::Get(track), fxIndex, enabled);
}

void URigdocksSilver_Track::AZ_GetTrackItemMainSend(URadnodzClient* client, UMediaTrack* track, bool& sendParent, int& firstChIndex, int& numChs)
{
	std::tie(sendParent, firstChIndex, numChs) = SILVER::AZ_GetTrackItemMainSend(client->GetClient(), UMediaTrack::Get(track));
}

void URigdocksSilver_Track::AZ_SetTrackItemMainSend(URadnodzClient* client, UMediaTrack* track, bool sendParent, int firstChIndex, int numChs)
{
	SILVER::AZ_SetTrackItemMainSend(client->GetClient(), UMediaTrack::Get(track), sendParent, firstChIndex, numChs);
}

TArray<FTrackDetail> URigdocksSilver_Track::AZ_GetTrackItemSendList(URadnodzClient* client, UMediaTrack* track)
{
	std::vector<TrackDetail> objArray = SILVER::AZ_GetTrackItemSendList(client->GetClient(), UMediaTrack::Get(track));

	TArray<FTrackDetail> result;

	for (auto obj : objArray)
	{
		FTrackDetail uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

int URigdocksSilver_Track::AZ_AddTrackItemSend(URadnodzClient* client, UMediaTrack* track, UMediaTrack* destTrack, bool isUnique)
{
	return SILVER::AZ_AddTrackItemSend(client->GetClient(), UMediaTrack::Get(track), UMediaTrack::Get(destTrack), isUnique);
}

FTrackSendProperties URigdocksSilver_Track::AZ_GetTrackItemSendProperties(URadnodzClient* client, UMediaTrack* track, int sendIndex)
{
	TrackSendProperties obj = SILVER::AZ_GetTrackItemSendProperties(client->GetClient(), UMediaTrack::Get(track), sendIndex);
		
	FTrackSendProperties uObj;
	uObj.Initialize(obj);

	return uObj;
}

void URigdocksSilver_Track::AZ_SetTrackItemSendProperties(URadnodzClient* client, UMediaTrack* track, int sendIndex, FTrackSendProperties props)
{

	SILVER::AZ_SetTrackItemSendProperties(client->GetClient(), UMediaTrack::Get(track), sendIndex, props.GetCppObj());
}

void URigdocksSilver_Track::AZ_DeleteTrackItemSend(URadnodzClient* client, UMediaTrack* track, int sendIndex)
{
	SILVER::AZ_DeleteTrackItemSend(client->GetClient(), UMediaTrack::Get(track), sendIndex);
}
