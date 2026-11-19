// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Region.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FMarker.h"

using namespace RADNODZ_client::Object;

int URigdocksSilver_Region::AZ_GetRegionMarkerIndex(URadnodzClient* client, UReaProject* projID, int regionId)
{
	return SILVER::AZ_GetRegionMarkerIndex(client->GetClient(), UReaProject::Get(projID), regionId);

}

void URigdocksSilver_Region::AZ_GetRegionMarkerPosition(URadnodzClient* client, UReaProject* projID, int regionId, double& startPosOut, double& endPosOut)
{
	std::tie(startPosOut, endPosOut) = SILVER::AZ_GetRegionMarkerPosition(client->GetClient(), UReaProject::Get(projID), regionId);

}

FString URigdocksSilver_Region::AZ_GetRegionMarkerName(URadnodzClient* client, UReaProject* projID, int regionId)
{
	 std::string name = SILVER::AZ_GetRegionMarkerName(client->GetClient(), UReaProject::Get(projID), regionId);

	 return UTF8_TO_TCHAR(name.c_str());

}

int URigdocksSilver_Region::AZ_GetRegionMarkerColor(URadnodzClient* client, UReaProject* projID, int regionId)
{
	return SILVER::AZ_GetRegionMarkerColor(client->GetClient(), UReaProject::Get(projID), regionId);

}

void URigdocksSilver_Region::AZ_SetRegionMarkerPosition(URadnodzClient* client, UReaProject* projID, int regionId, double setStartPos, double setEndPos)
{
	SILVER::AZ_SetRegionMarkerPosition(client->GetClient(), UReaProject::Get(projID), regionId, setStartPos, setEndPos);
}

void URigdocksSilver_Region::AZ_SetRegionMarkerColor(URadnodzClient* client, UReaProject* projID, int regionId, int setColor)
{
	SILVER::AZ_SetRegionMarkerColor(client->GetClient(), UReaProject::Get(projID), regionId, setColor);
}

void URigdocksSilver_Region::AZ_SetRegionMarkerName(URadnodzClient* client, UReaProject* projID, int regionId, FString setName)
{
	SILVER::AZ_SetRegionMarkerName(client->GetClient(), UReaProject::Get(projID), regionId, TCHAR_TO_UTF8(*setName));
}

int URigdocksSilver_Region::AZ_AddRegionMarker(URadnodzClient* client, UReaProject* projID, double setStartPos, double setEndPos, FString setName, int setRegionIndex, int setColor)
{
	return SILVER::AZ_AddRegionMarker(client->GetClient(), UReaProject::Get(projID), setStartPos, setEndPos, TCHAR_TO_UTF8(*setName), setRegionIndex, setColor);

}

void URigdocksSilver_Region::AZ_DeleteAllRegion(URadnodzClient* client, UReaProject* projID)
{
	SILVER::AZ_DeleteAllRegion(client->GetClient(), UReaProject::Get(projID));
}

FMarker URigdocksSilver_Region::AZ_GetRegionObject(URadnodzClient* client, UReaProject* proj, int regionId)
{
    RADNODZ_client::Object::Marker marker = SILVER::AZ_GetRegionObject(client->GetClient(), UReaProject::Get(proj), regionId);

	FMarker markerObject;
	markerObject.Initialize(marker);

	return markerObject;
}

TArray<FMarker> URigdocksSilver_Region::AZ_GetRegionMarkerListMediaItem(URadnodzClient* client, UMediaItem* item)
{
	std::vector<RADNODZ_client::Object::Marker> markerList = SILVER::AZ_GetRegionMarkerListMediaItem(client->GetClient(), UMediaItem::Get(item));

	TArray<FMarker> markerObjectList;

	for (auto& marker : markerList)
	{
		FMarker markerObject;
		markerObject.Initialize(marker);
		markerObjectList.Add(markerObject);
	}

	return markerObjectList;
}

FMarker URigdocksSilver_Region::AZ_AddRegionMarkerMediaItem(URadnodzClient* client, UMediaItem* item, FString regionName, int setColor)
{
    RADNODZ_client::Object::Marker marker = SILVER::AZ_AddRegionMarkerMediaItem(client->GetClient(), UMediaItem::Get(item), TCHAR_TO_UTF8(*regionName), setColor);

	FMarker markerObject;
	markerObject.Initialize(marker);

	return markerObject;
}
