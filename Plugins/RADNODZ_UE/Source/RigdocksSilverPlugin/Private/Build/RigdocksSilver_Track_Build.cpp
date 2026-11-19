// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Track_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include <string>

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemSelect_Build(UReaProject* proj, int trackID, int trackType)
{
	RpcCall c = SILVER::AZ_GetTrackItemSelect_Build(UReaProject::Get(proj), trackID, trackType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemName_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemName_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemName_Build(UMediaTrack* track, FString setName)
{
	RpcCall c = SILVER::AZ_SetTrackItemName_Build(UMediaTrack::Get(track), TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemIndex_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemIndex_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackFirstInfo_Build(UReaProject* proj)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackFirstInfo_Build(UReaProject::Get(proj));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_InsertTrackSearchUniqueOnly_Build(UReaProject* proj, FString setTrackName)
{
	RpcCall c = SILVER::AZ_InsertTrackSearchUniqueOnly_Build(UReaProject::Get(proj), TCHAR_TO_UTF8(*setTrackName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackCountSelect_Build(UReaProject* proj, int trackType)
{
	RpcCall c = SILVER::AZ_GetTrackCountSelect_Build(UReaProject::Get(proj), trackType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackIdName_Build(UReaProject* proj, int trackID)
{
	RpcCall c = SILVER::AZ_GetTrackIdName_Build(UReaProject::Get(proj), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackIdName_Build(UReaProject* proj, int selectTrackID)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackIdName_Build(UReaProject::Get(proj), selectTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemDepth_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemDepth_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackIdDepth_Build(UReaProject* proj, int trackID)
{
	RpcCall c = SILVER::AZ_GetTrackIdDepth_Build(UReaProject::Get(proj), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackIdDepth_Build(UReaProject* proj, int selectTrackID)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackIdDepth_Build(UReaProject::Get(proj), selectTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemDepth_Build(UMediaTrack* track, int setDepth)
{
	RpcCall c = SILVER::AZ_SetTrackItemDepth_Build(UMediaTrack::Get(track), setDepth);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdDepth_Build(UReaProject* proj, int trackID, int setDepth)
{
	RpcCall c = SILVER::AZ_SetTrackIdDepth_Build(UReaProject::Get(proj), trackID, setDepth);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdDepth_Build(UReaProject* proj, int selectTrackID, int setDepth)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdDepth_Build(UReaProject::Get(proj), selectTrackID, setDepth);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdName_Build(UReaProject* proj, int trackID, FString setName)
{
	RpcCall c = SILVER::AZ_SetTrackIdName_Build(UReaProject::Get(proj), trackID, TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdName_Build(UReaProject* proj, int selectTrackID, FString setName)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdName_Build(UReaProject::Get(proj), selectTrackID, TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemMute_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemMute_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackIdMute_Build(UReaProject* proj, int trackID)
{
	RpcCall c = SILVER::AZ_GetTrackIdMute_Build(UReaProject::Get(proj), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackIdMute_Build(UReaProject* proj, int selecteTrackID)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackIdMute_Build(UReaProject::Get(proj), selecteTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemMute_Build(UMediaTrack* track, bool setMute)
{
	RpcCall c = SILVER::AZ_SetTrackItemMute_Build(UMediaTrack::Get(track), setMute);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdMute_Build(UReaProject* proj, int trackID, bool setMute)
{
	RpcCall c = SILVER::AZ_SetTrackIdMute_Build(UReaProject::Get(proj), trackID, setMute);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdMute_Build(UReaProject* proj, int selecteTrackID, bool setMute)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdMute_Build(UReaProject::Get(proj), selecteTrackID, setMute);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackAllMuteOFF_Build(UReaProject* proj)
{
	RpcCall c = SILVER::AZ_SetTrackAllMuteOFF_Build(UReaProject::Get(proj));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemSolo_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemSolo_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackIdSolo_Build(UReaProject* proj, int trackID)
{
	RpcCall c = SILVER::AZ_GetTrackIdSolo_Build(UReaProject::Get(proj), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackIdSolo_Build(UReaProject* proj, int selectTrackID)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackIdSolo_Build(UReaProject::Get(proj), selectTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemSolo_Build(UMediaTrack* track, bool setSolo)
{
	RpcCall c = SILVER::AZ_SetTrackItemSolo_Build(UMediaTrack::Get(track), setSolo);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdSolo_Build(UReaProject* proj, int trackID, bool setSolo)
{
	RpcCall c = SILVER::AZ_SetTrackIdSolo_Build(UReaProject::Get(proj), trackID, setSolo);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdSolo_Build(UReaProject* proj, int selectTrackID, bool setSolo)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdSolo_Build(UReaProject::Get(proj), selectTrackID, setSolo);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackAllSoloOFF_Build(UReaProject* proj)
{
	RpcCall c = SILVER::AZ_SetTrackAllSoloOFF_Build(UReaProject::Get(proj));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemColor_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemColor_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackIdColor_Build(UReaProject* proj, int trackID)
{
	RpcCall c = SILVER::AZ_GetTrackIdColor_Build(UReaProject::Get(proj), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackIdColor_Build(UReaProject* proj, int selectTrackID)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackIdColor_Build(UReaProject::Get(proj), selectTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemColor_Build(UMediaTrack* track, int setColor)
{
	RpcCall c = SILVER::AZ_SetTrackItemColor_Build(UMediaTrack::Get(track), setColor);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdColor_Build(UReaProject* proj, int trackID, int setColor)
{
	RpcCall c = SILVER::AZ_SetTrackIdColor_Build(UReaProject::Get(proj), trackID, setColor);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdColor_Build(UReaProject* proj, int selectTrackID, int setColor)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdColor_Build(UReaProject::Get(proj), selectTrackID, setColor);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemRGB_Build(UMediaTrack* track, int setR, int setG, int setB)
{
	RpcCall c = SILVER::AZ_SetTrackItemRGB_Build(UMediaTrack::Get(track), setR, setG, setB);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdRGB_Build(UReaProject* proj, int trackID, int setR, int setG, int setB)
{
	RpcCall c = SILVER::AZ_SetTrackIdRGB_Build(UReaProject::Get(proj), trackID, setR, setG, setB);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdRGB_Build(UReaProject* proj, int selectTrackID, int setR, int setG, int setB)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdRGB_Build(UReaProject::Get(proj), selectTrackID, setR, setG, setB);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdSelected_Build(UReaProject* proj, int trackID, bool setSelectType)
{
	RpcCall c = SILVER::AZ_SetTrackIdSelected_Build(UReaProject::Get(proj), trackID, setSelectType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemOnlySelected_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_SetTrackItemOnlySelected_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdOnlySelected_Build(UReaProject* proj, int trackID)
{
	RpcCall c = SILVER::AZ_SetTrackIdOnlySelected_Build(UReaProject::Get(proj), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemVolume_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemVolume_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackIdVolume_Build(UReaProject* proj, int trackID)
{
	RpcCall c = SILVER::AZ_GetTrackIdVolume_Build(UReaProject::Get(proj), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackIdVolume_Build(UReaProject* proj, int selectTrackID)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackIdVolume_Build(UReaProject::Get(proj), selectTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemVolume_Build(UMediaTrack* track, double setVolume)
{
	RpcCall c = SILVER::AZ_SetTrackItemVolume_Build(UMediaTrack::Get(track), setVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdVolume_Build(UReaProject* proj, int trackID, double setVolume)
{
	RpcCall c = SILVER::AZ_SetTrackIdVolume_Build(UReaProject::Get(proj), trackID, setVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdVolume_Build(UReaProject* proj, int trackID, double setVolume)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdVolume_Build(UReaProject::Get(proj), trackID, setVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemAddVolume_Build(UMediaTrack* track, double addVolume)
{
	RpcCall c = SILVER::AZ_SetTrackItemAddVolume_Build(UMediaTrack::Get(track), addVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdAddVolume_Build(UReaProject* proj, int trackID, double addVolume)
{
	RpcCall c = SILVER::AZ_SetTrackIdAddVolume_Build(UReaProject::Get(proj), trackID, addVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdAddVolume_Build(UReaProject* proj, int trackID, double addVolume)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdAddVolume_Build(UReaProject::Get(proj), trackID, addVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_DeleteTrackIdSelect_Build(UReaProject* proj, int trackID, int trackType)
{
	RpcCall c = SILVER::AZ_DeleteTrackIdSelect_Build(UReaProject::Get(proj), trackID, trackType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemList_Build(UReaProject* project)
{
	RpcCall c = SILVER::AZ_GetTrackItemList_Build(UReaProject::Get(project));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemChildList_Build(UMediaTrack* track, bool isRecursive)
{
	RpcCall c = SILVER::AZ_GetTrackItemChildList_Build(UMediaTrack::Get(track), isRecursive);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackIdChildList_Build(UReaProject* project, int trackID, bool isRecursive)
{
	RpcCall c = SILVER::AZ_GetTrackIdChildList_Build(UReaProject::Get(project), trackID, isRecursive);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackIdChildList_Build(UReaProject* project, int selectTrackID, bool isRecursive)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackIdChildList_Build(UReaProject::Get(project), selectTrackID, isRecursive);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackListDetail_Build(UReaProject* project)
{
	RpcCall c = SILVER::AZ_GetTrackListDetail_Build(UReaProject::Get(project));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackListDetail_Build(UReaProject* project)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackListDetail_Build(UReaProject::Get(project));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackDepthList_Build(UReaProject* project)
{
	RpcCall c = SILVER::AZ_GetTrackDepthList_Build(UReaProject::Get(project));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSamePerfectTrackNameList_Build(UReaProject* project, FString _checkName)
{
	RpcCall c = SILVER::AZ_GetSamePerfectTrackNameList_Build(UReaProject::Get(project), TCHAR_TO_UTF8(*_checkName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetIncludePerfectTrackNameList_Build(UReaProject* project, FString _checkName)
{
	RpcCall c = SILVER::AZ_GetIncludePerfectTrackNameList_Build(UReaProject::Get(project), TCHAR_TO_UTF8(*_checkName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackItemList_Build(UReaProject* project)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackItemList_Build(UReaProject::Get(project));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_InsertUniqueChildTrack_Build(UMediaTrack* parentTrack, FString setName)
{
	RpcCall c = SILVER::AZ_InsertUniqueChildTrack_Build(UMediaTrack::Get(parentTrack), TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_InsertTrackId_Build(UReaProject* proj, int trackID, FString setName)
{
	RpcCall c = SILVER::AZ_InsertTrackId_Build(UReaProject::Get(proj), trackID, TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_InsertChildTrack_Build(UMediaTrack* parentTrack, FString setName)
{
	RpcCall c = SILVER::AZ_InsertChildTrack_Build(UMediaTrack::Get(parentTrack), TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemRecording_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemRecording_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackIdRecording_Build(UReaProject* projID, int trackID)
{
	RpcCall c = SILVER::AZ_GetTrackIdRecording_Build(UReaProject::Get(projID), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackIdRecording_Build(UReaProject* projID, int selectTrackID)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackIdRecording_Build(UReaProject::Get(projID), selectTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetRecordingTrackList_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_GetRecordingTrackList_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemRecording_Build(UMediaTrack* track, bool setRec)
{
	RpcCall c = SILVER::AZ_SetTrackItemRecording_Build(UMediaTrack::Get(track), setRec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdRecording_Build(UReaProject* projID, int trackID, bool setRec)
{
	RpcCall c = SILVER::AZ_SetTrackIdRecording_Build(UReaProject::Get(projID), trackID, setRec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdRecording_Build(UReaProject* projID, int selectTrackID, bool setRec)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdRecording_Build(UReaProject::Get(projID), selectTrackID, setRec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemRecordMonitoring_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemRecordMonitoring_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackIdRecordMonitoring_Build(UReaProject* projID, int trackID)
{
	RpcCall c = SILVER::AZ_GetTrackIdRecordMonitoring_Build(UReaProject::Get(projID), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackIdRecordMonitoring_Build(UReaProject* projID, int selectTrackID)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackIdRecordMonitoring_Build(UReaProject::Get(projID), selectTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemRecordMonitoring_Build(UMediaTrack* track, AZ_RecordMonitorMode monitorMode)
{
	RpcCall c = SILVER::AZ_SetTrackItemRecordMonitoring_Build(UMediaTrack::Get(track), (int)monitorMode);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdRecordMonitoring_Build(UReaProject* projID, int trackID, AZ_RecordMonitorMode monitorMode)
{
	RpcCall c = SILVER::AZ_SetTrackIdRecordMonitoring_Build(UReaProject::Get(projID), trackID, (int)monitorMode);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdRecordMonitoring_Build(UReaProject* projID, int selectTrackID, AZ_RecordMonitorMode monitorMode)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdRecordMonitoring_Build(UReaProject::Get(projID), selectTrackID, (int)monitorMode);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackAllRecording_Build(UReaProject* projID, bool setRec)
{
	RpcCall c = SILVER::AZ_SetTrackAllRecording_Build(UReaProject::Get(projID), setRec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemOnlyRecording_Build(UMediaTrack* track, bool setRec)
{
	RpcCall c = SILVER::AZ_SetTrackItemOnlyRecording_Build(UMediaTrack::Get(track), setRec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdOnlyRecording_Build(UReaProject* projID, int trackID, bool setRec)
{
	RpcCall c = SILVER::AZ_SetTrackIdOnlyRecording_Build(UReaProject::Get(projID), trackID, setRec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdOnlyRecording_Build(UReaProject* projID, int selectedTrackID, bool setRec)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdOnlyRecording_Build(UReaProject::Get(projID), selectedTrackID, setRec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemCurrentPeak_Build(UMediaTrack* track, int chIndex)
{
	RpcCall c = SILVER::AZ_GetTrackItemCurrentPeak_Build(UMediaTrack::Get(track), chIndex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemCurrentPeakDB_Build(UMediaTrack* track, int chIndex)
{
	RpcCall c = SILVER::AZ_GetTrackItemCurrentPeakDB_Build(UMediaTrack::Get(track), chIndex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemHoldPeakDB_Build(UMediaTrack* track, int chIndex)
{
	RpcCall c = SILVER::AZ_GetTrackItemHoldPeakDB_Build(UMediaTrack::Get(track), chIndex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_ResetTrackItemHoldPeak_Build(UMediaTrack* track, int chIndex)
{
	RpcCall c = SILVER::AZ_ResetTrackItemHoldPeak_Build(UMediaTrack::Get(track), chIndex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemNumChannels_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemNumChannels_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackIdNumChannels_Build(UReaProject* projID, int trackID)
{
	RpcCall c = SILVER::AZ_GetTrackIdNumChannels_Build(UReaProject::Get(projID), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackIdNumChannels_Build(UReaProject* projID, int selectedTrackID)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackIdNumChannels_Build(UReaProject::Get(projID), selectedTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemNumChannels_Build(UMediaTrack* track, int numChs)
{
	RpcCall c = SILVER::AZ_SetTrackItemNumChannels_Build(UMediaTrack::Get(track), numChs);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdNumChannels_Build(UReaProject* projID, int trackID, int numChs)
{
	RpcCall c = SILVER::AZ_SetTrackIdNumChannels_Build(UReaProject::Get(projID), trackID, numChs);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdNumChannels_Build(UReaProject* projID, int selectedTrackID, int numChs)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdNumChannels_Build(UReaProject::Get(projID), selectedTrackID, numChs);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemInputChannel_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemInputChannel_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackIdInputChannel_Build(UReaProject* projID, int trackID)
{
	RpcCall c = SILVER::AZ_GetTrackIdInputChannel_Build(UReaProject::Get(projID), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetSelectedTrackIdInputChannel_Build(UReaProject* projID, int selectedTrackID)
{
	RpcCall c = SILVER::AZ_GetSelectedTrackIdInputChannel_Build(UReaProject::Get(projID), selectedTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemInputChannel_Build(UMediaTrack* track, int firstChannelIndex, int numChannels, AZ_InputChannelType inputChType)
{
	RpcCall c = SILVER::AZ_SetTrackItemInputChannel_Build(UMediaTrack::Get(track), firstChannelIndex, numChannels, (int)inputChType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackIdInputChannel_Build(UReaProject* projID, int trackID, int firstChannelIndex, int numChannels, AZ_InputChannelType inputChType)
{
	RpcCall c = SILVER::AZ_SetTrackIdInputChannel_Build(UReaProject::Get(projID), trackID, firstChannelIndex, numChannels, (int)inputChType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetSelectedTrackIdInputChannel_Build(UReaProject* projID, int selectedTrackID, int firstChannelIndex, int numChannels, AZ_InputChannelType inputChType)
{
	RpcCall c = SILVER::AZ_SetSelectedTrackIdInputChannel_Build(UReaProject::Get(projID), selectedTrackID, firstChannelIndex, numChannels, (int)inputChType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemMeterMode_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemMeterMode_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemMeterMode_Build(UMediaTrack* track, AZ_TrackMeterMode meterMode, bool enabled, bool lufsChannels1And2Only)
{
	RpcCall c = SILVER::AZ_SetTrackItemMeterMode_Build(UMediaTrack::Get(track), (int)meterMode, enabled, lufsChannels1And2Only);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemFxEnabled_Build(UMediaTrack* track, int fxIndex)
{
	RpcCall c = SILVER::AZ_GetTrackItemFxEnabled_Build(UMediaTrack::Get(track), fxIndex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemFxEnabled_Build(UMediaTrack* track, int fxIndex, bool enabled)
{
	RpcCall c = SILVER::AZ_SetTrackItemFxEnabled_Build(UMediaTrack::Get(track), fxIndex, enabled);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemMainSend_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemMainSend_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemMainSend_Build(UMediaTrack* track, bool sendParent, int firstChIndex, int numChs)
{
	RpcCall c = SILVER::AZ_SetTrackItemMainSend_Build(UMediaTrack::Get(track), sendParent, firstChIndex, numChs);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemSendList_Build(UMediaTrack* track)
{
	RpcCall c = SILVER::AZ_GetTrackItemSendList_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_AddTrackItemSend_Build(UMediaTrack* track, UMediaTrack* destTrack, bool isUnique)
{
	RpcCall c = SILVER::AZ_AddTrackItemSend_Build(UMediaTrack::Get(track), UMediaTrack::Get(destTrack), isUnique);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_GetTrackItemSendProperties_Build(UMediaTrack* track, int sendIndex)
{
	RpcCall c = SILVER::AZ_GetTrackItemSendProperties_Build(UMediaTrack::Get(track), sendIndex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_SetTrackItemSendProperties_Build(UMediaTrack* track, int sendIndex, FTrackSendProperties props)
{
	RpcCall c = SILVER::AZ_SetTrackItemSendProperties_Build(UMediaTrack::Get(track), sendIndex, props.GetCppObj());

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Track_Build::AZ_DeleteTrackItemSend_Build(UMediaTrack* track, int sendIndex)
{
	RpcCall c = SILVER::AZ_DeleteTrackItemSend_Build(UMediaTrack::Get(track), sendIndex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
