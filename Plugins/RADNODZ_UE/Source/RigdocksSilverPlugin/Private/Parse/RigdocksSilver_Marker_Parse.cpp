// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Marker_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FMarker.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

FMarker URigdocksSilver_Marker_Parse::AZ_GetMarkerObject_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FMarker();
	}

	RADNODZ_client::Object::Marker marker = SILVER::AZ_GetMarkerObject_Parse(call->Get());

	FMarker markerObject;
	markerObject.Initialize(marker);
	return markerObject;
}

TArray<FMarker> URigdocksSilver_Marker_Parse::AZ_GetMarkerListMediaItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FMarker>();
	}

	std::vector<RADNODZ_client::Object::Marker> markerList = SILVER::AZ_GetMarkerListMediaItem_Parse(call->Get());

	TArray<FMarker> markerObjectList;
	for (auto& marker : markerList)
	{
		FMarker markerObject;
		markerObject.Initialize(marker);
		markerObjectList.Add(markerObject);
	}

	return markerObjectList;
}

void URigdocksSilver_Marker_Parse::AZ_SetMarkerPosition_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetMarkerPosition_Parse(call->Get());
}

void URigdocksSilver_Marker_Parse::AZ_SetMarkerColor_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetMarkerColor_Parse(call->Get());
}

void URigdocksSilver_Marker_Parse::AZ_SetMarkerName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetMarkerName_Parse(call->Get());
}

FMarker URigdocksSilver_Marker_Parse::AZ_AddMarker_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FMarker();
	}

	RADNODZ_client::Object::Marker marker = SILVER::AZ_AddMarker_Parse(call->Get());

	FMarker markerObject;
	markerObject.Initialize(marker);
	return markerObject;
}

void URigdocksSilver_Marker_Parse::AZ_DeleteAllMarker_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_DeleteAllMarker_Parse(call->Get());
}

FMarker URigdocksSilver_Marker_Parse::AZ_AddMarkerMediaItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FMarker();
	}

	RADNODZ_client::Object::Marker marker = SILVER::AZ_AddMarkerMediaItem_Parse(call->Get());

	FMarker markerObject;
	markerObject.Initialize(marker);
	return markerObject;
}
