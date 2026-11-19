// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Marker.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FMarker.h"

using namespace RADNODZ_client::Object;

FMarker URigdocksSilver_Marker::AZ_GetMarkerObject(URadnodzClient* client, UReaProject* proj, int markerId)
{
	RADNODZ_client::Object::Marker marker = SILVER::AZ_GetMarkerObject(client->GetClient(), UReaProject::Get(proj), markerId);

	FMarker markerObject;
	markerObject.Initialize(marker);

	return markerObject;
}

TArray<FMarker> URigdocksSilver_Marker::AZ_GetMarkerListMediaItem(URadnodzClient* client, UMediaItem* item)
{
	std::vector<RADNODZ_client::Object::Marker> markerList = SILVER::AZ_GetMarkerListMediaItem(client->GetClient(), UMediaItem::Get(item));

	TArray<FMarker> markerObjectList;

	for (auto& marker : markerList) {
		FMarker markerObject;
		markerObject.Initialize(marker);

		markerObjectList.Add(markerObject);
	}

	return markerObjectList;
}

void URigdocksSilver_Marker::AZ_SetMarkerPosition(URadnodzClient* client, UReaProject* proj, int markerId, double setStartPos)
{
	SILVER::AZ_SetMarkerPosition(client->GetClient(), UReaProject::Get(proj), markerId, setStartPos);
}

void URigdocksSilver_Marker::AZ_SetMarkerColor(URadnodzClient* client, UReaProject* proj, int markerId, int setColor)
{
	SILVER::AZ_SetMarkerColor(client->GetClient(), UReaProject::Get(proj), markerId, setColor);
}

void URigdocksSilver_Marker::AZ_SetMarkerName(URadnodzClient* client, UReaProject* proj, int markerId, FString setName)
{
	SILVER::AZ_SetMarkerName(client->GetClient(), UReaProject::Get(proj), markerId, TCHAR_TO_UTF8(*setName));
}

FMarker URigdocksSilver_Marker::AZ_AddMarker(URadnodzClient* client, UReaProject* proj, double setStartPos, FString setName, int setColor)
{
	RADNODZ_client::Object::Marker marker = SILVER::AZ_AddMarker(client->GetClient(), UReaProject::Get(proj), setStartPos, TCHAR_TO_UTF8(*setName), setColor);

	FMarker markerObject;
	markerObject.Initialize(marker);

	return markerObject;
}

void URigdocksSilver_Marker::AZ_DeleteAllMarker(URadnodzClient* client, UReaProject* proj)
{
	SILVER::AZ_DeleteAllMarker(client->GetClient(), UReaProject::Get(proj));
}

FMarker URigdocksSilver_Marker::AZ_AddMarkerMediaItem(URadnodzClient* client, UMediaItem* item, FString markerName, int setColor)
{
	RADNODZ_client::Object::Marker marker = SILVER::AZ_AddMarkerMediaItem(client->GetClient(), UMediaItem::Get(item), TCHAR_TO_UTF8(*markerName), setColor);

	FMarker markerObject;
	markerObject.Initialize(marker);

	return markerObject;
}