// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Region_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FMarker.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_GetRegionMarkerIndex_Build(UReaProject* projID, int regionId)
{
	RpcCall c = SILVER::AZ_GetRegionMarkerIndex_Build(UReaProject::Get(projID), regionId);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_GetRegionMarkerPosition_Build(UReaProject* projID, int regionId)
{
	RpcCall c = SILVER::AZ_GetRegionMarkerPosition_Build(UReaProject::Get(projID), regionId);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_GetRegionMarkerName_Build(UReaProject* projID, int regionId)
{
	RpcCall c = SILVER::AZ_GetRegionMarkerName_Build(UReaProject::Get(projID), regionId);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_GetRegionMarkerColor_Build(UReaProject* projID, int regionId)
{
	RpcCall c = SILVER::AZ_GetRegionMarkerColor_Build(UReaProject::Get(projID), regionId);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_SetRegionMarkerPosition_Build(UReaProject* projID, int regionId, double setStartPos, double setEndPos)
{
	RpcCall c = SILVER::AZ_SetRegionMarkerPosition_Build(UReaProject::Get(projID), regionId, setStartPos, setEndPos);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_SetRegionMarkerColor_Build(UReaProject* projID, int regionId, int setColor)
{
	RpcCall c = SILVER::AZ_SetRegionMarkerColor_Build(UReaProject::Get(projID), regionId, setColor);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_SetRegionMarkerName_Build(UReaProject* projID, int regionId, FString setName)
{
	RpcCall c = SILVER::AZ_SetRegionMarkerName_Build(UReaProject::Get(projID), regionId, TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_AddRegionMarker_Build(UReaProject* projID, double setStartPos, double setEndPos, FString setName, int setRegionIndex, int setColor)
{
	RpcCall c = SILVER::AZ_AddRegionMarker_Build(UReaProject::Get(projID), setStartPos, setEndPos, TCHAR_TO_UTF8(*setName), setRegionIndex, setColor);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_DeleteAllRegion_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_DeleteAllRegion_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_GetRegionObject_Build(UReaProject* proj, int regionId)
{
	RpcCall c = SILVER::AZ_GetRegionObject_Build(UReaProject::Get(proj), regionId);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_GetRegionMarkerListMediaItem_Build(UMediaItem* item)
{
	RpcCall c = SILVER::AZ_GetRegionMarkerListMediaItem_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Region_Build::AZ_AddRegionMarkerMediaItem_Build(UMediaItem* item, FString regionName, int setColor)
{
	RpcCall c = SILVER::AZ_AddRegionMarkerMediaItem_Build(UMediaItem::Get(item), TCHAR_TO_UTF8(*regionName), setColor);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
