// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Region_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FMarker.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

int URigdocksSilver_Region_Parse::AZ_GetRegionMarkerIndex_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetRegionMarkerIndex_Parse(call->Get());
}

void URigdocksSilver_Region_Parse::AZ_GetRegionMarkerPosition_Parse(URigdocksRpcCall* call, double& startPosOut, double& endPosOut)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(startPosOut, endPosOut) = SILVER::AZ_GetRegionMarkerPosition_Parse(call->Get());
}

FString URigdocksSilver_Region_Parse::AZ_GetRegionMarkerName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string name = SILVER::AZ_GetRegionMarkerName_Parse(call->Get());
	return UTF8_TO_TCHAR(name.c_str());
}

int URigdocksSilver_Region_Parse::AZ_GetRegionMarkerColor_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetRegionMarkerColor_Parse(call->Get());
}

void URigdocksSilver_Region_Parse::AZ_SetRegionMarkerPosition_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetRegionMarkerPosition_Parse(call->Get());
}

void URigdocksSilver_Region_Parse::AZ_SetRegionMarkerColor_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetRegionMarkerColor_Parse(call->Get());
}

void URigdocksSilver_Region_Parse::AZ_SetRegionMarkerName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetRegionMarkerName_Parse(call->Get());
}

int URigdocksSilver_Region_Parse::AZ_AddRegionMarker_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_AddRegionMarker_Parse(call->Get());
}

void URigdocksSilver_Region_Parse::AZ_DeleteAllRegion_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_DeleteAllRegion_Parse(call->Get());
}

FMarker URigdocksSilver_Region_Parse::AZ_GetRegionObject_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FMarker();
	}

    RADNODZ_client::Object::Marker marker = SILVER::AZ_GetRegionObject_Parse(call->Get());

	FMarker markerObject;
	markerObject.Initialize(marker);
	return markerObject;
}

TArray<FMarker> URigdocksSilver_Region_Parse::AZ_GetRegionMarkerListMediaItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FMarker>();
	}

	std::vector<RADNODZ_client::Object::Marker> markerList = SILVER::AZ_GetRegionMarkerListMediaItem_Parse(call->Get());

	TArray<FMarker> markerObjectList;
	for (auto& marker : markerList)
	{
		FMarker markerObject;
		markerObject.Initialize(marker);
		markerObjectList.Add(markerObject);
	}

	return markerObjectList;
}

FMarker URigdocksSilver_Region_Parse::AZ_AddRegionMarkerMediaItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FMarker();
	}

    RADNODZ_client::Object::Marker marker = SILVER::AZ_AddRegionMarkerMediaItem_Parse(call->Get());

	FMarker markerObject;
	markerObject.Initialize(marker);
	return markerObject;
}
