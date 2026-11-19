// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_HANDAUTOMER_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksSilver_HANDAUTOMER_Build::AZ_SetMediaItemHANDAUTOMER_Build(UMediaItem* item, int ratioType)
{
	RpcCall c = SILVER::AZ_SetMediaItemHANDAUTOMER_Build(UMediaItem::Get(item), ratioType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_HANDAUTOMER_Build::AZ_SetMediaIdHANDAUTOMER_Build(UReaProject* projID, int itemID, int ratioType)
{
	RpcCall c = SILVER::AZ_SetMediaIdHANDAUTOMER_Build(UReaProject::Get(projID), itemID, ratioType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_HANDAUTOMER_Build::AZ_SetSelectedMediaIdHANDAUTOMER_Build(UReaProject* projID, int selectItemID, int ratioType)
{
	RpcCall c = SILVER::AZ_SetSelectedMediaIdHANDAUTOMER_Build(UReaProject::Get(projID), selectItemID, ratioType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_HANDAUTOMER_Build::AZ_SetMediaItemListHANDAUTOMER_Build(TArray<UMediaItem*> itemList, int ratioType)
{
	std::vector<MediaItem*> nativeItemList;
	for (UMediaItem* item : itemList)
	{
		nativeItemList.push_back(UMediaItem::Get(item));
	}

	RpcCall c = SILVER::AZ_SetMediaItemListHANDAUTOMER_Build(nativeItemList, ratioType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
