// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Marker_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FMarker.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksSilver_Marker_Build::AZ_GetMarkerObject_Build(UReaProject* proj, int markerId)
{
	RpcCall c = SILVER::AZ_GetMarkerObject_Build(UReaProject::Get(proj), markerId);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Marker_Build::AZ_GetMarkerListMediaItem_Build(UMediaItem* item)
{
	RpcCall c = SILVER::AZ_GetMarkerListMediaItem_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Marker_Build::AZ_SetMarkerPosition_Build(UReaProject* proj, int markerId, double setStartPos)
{
	RpcCall c = SILVER::AZ_SetMarkerPosition_Build(UReaProject::Get(proj), markerId, setStartPos);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Marker_Build::AZ_SetMarkerColor_Build(UReaProject* proj, int markerId, int setColor)
{
	RpcCall c = SILVER::AZ_SetMarkerColor_Build(UReaProject::Get(proj), markerId, setColor);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Marker_Build::AZ_SetMarkerName_Build(UReaProject* proj, int markerId, FString setName)
{
	RpcCall c = SILVER::AZ_SetMarkerName_Build(UReaProject::Get(proj), markerId, TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Marker_Build::AZ_AddMarker_Build(UReaProject* proj, double setStartPos, FString setName, int setColor)
{
	RpcCall c = SILVER::AZ_AddMarker_Build(UReaProject::Get(proj), setStartPos, TCHAR_TO_UTF8(*setName), setColor);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Marker_Build::AZ_DeleteAllMarker_Build(UReaProject* proj)
{
	RpcCall c = SILVER::AZ_DeleteAllMarker_Build(UReaProject::Get(proj));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Marker_Build::AZ_AddMarkerMediaItem_Build(UMediaItem* item, FString markerName, int setColor)
{
	RpcCall c = SILVER::AZ_AddMarkerMediaItem_Build(UMediaItem::Get(item), TCHAR_TO_UTF8(*markerName), setColor);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
