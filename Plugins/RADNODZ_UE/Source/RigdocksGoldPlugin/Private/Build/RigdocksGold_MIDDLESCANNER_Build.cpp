// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksGold_MIDDLESCANNER_Build.h"
#include "RIGDOCKS_GOLD_Build.h"
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksGold_MIDDLESCANNER_Build::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem_Build(UMediaItem* item, FString language, FString parentID, FMIDDLESCANNER_Options options)
{
	RpcCall c = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem_Build(UMediaItem::Get(item), TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*parentID), options.GetCppObj());

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_MIDDLESCANNER_Build::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId_Build(UReaProject* proj, int itemID, FString language, FString parentID, FMIDDLESCANNER_Options options)
{
	RpcCall c = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId_Build(UReaProject::Get(proj), itemID, TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*parentID), options.GetCppObj());

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_MIDDLESCANNER_Build::AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId_Build(UReaProject* proj, int selectedItemID, FString language, FString parentID, FMIDDLESCANNER_Options options)
{
	RpcCall c = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId_Build(UReaProject::Get(proj), selectedItemID, TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*parentID), options.GetCppObj());

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_MIDDLESCANNER_Build::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList_Build(TArray<UMediaItem*> itemList, FString language, FString parentID, FMIDDLESCANNER_Options options)
{
	std::vector<MediaItem*> mediaItemVector;
	for (UMediaItem* item : itemList)
	{
		mediaItemVector.push_back(UMediaItem::Get(item));
	}

	RpcCall c = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList_Build(mediaItemVector, TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*parentID), options.GetCppObj());

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
