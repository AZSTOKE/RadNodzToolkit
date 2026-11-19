// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Peak_Build.h"
#include "RIGDOCKS_SILVER_Build.h"

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksSilver_Peak_Build::AZ_GetMediaItemPeakList_Build(UMediaItem* item)
{
	RpcCall c = SILVER::AZ_GetMediaItemPeakList_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Peak_Build::AZ_GetMediaIdPeakList_Build(UReaProject* project, int itemID)
{
	RpcCall c = SILVER::AZ_GetMediaIdPeakList_Build(UReaProject::Get(project), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Peak_Build::AZ_GetSelectedMediaIdPeakList_Build(UReaProject* project, int selectItemID)
{
	RpcCall c = SILVER::AZ_GetSelectedMediaIdPeakList_Build(UReaProject::Get(project), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
