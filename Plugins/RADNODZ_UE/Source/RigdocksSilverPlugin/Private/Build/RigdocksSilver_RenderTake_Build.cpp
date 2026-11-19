// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_RenderTake_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksSilver_RenderTake_Build::AZ_RenderMediaItemActiveTake_Build(UMediaItem* item)
{
	RpcCall c = SILVER::AZ_RenderMediaItemActiveTake_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_RenderTake_Build::AZ_RenderMediaIdActiveTake_Build(UReaProject* projID, int itemID)
{
	RpcCall c = SILVER::AZ_RenderMediaIdActiveTake_Build(UReaProject::Get(projID), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_RenderTake_Build::AZ_RenderSelectedMediaIdActiveTake_Build(UReaProject* projID, int selectItemID)
{
	RpcCall c = SILVER::AZ_RenderSelectedMediaIdActiveTake_Build(UReaProject::Get(projID), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
