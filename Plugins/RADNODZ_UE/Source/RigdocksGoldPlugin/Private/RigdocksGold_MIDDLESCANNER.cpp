// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksGold_MIDDLESCANNER.h"
#include "RIGDOCKS_GOLD.h"

FMiddlewareInfo URigdocksGold_MIDDLESCANNER::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem(URadnodzClient* client, UMediaItem* item, FString language, FString parentID, FMIDDLESCANNER_Options options)
{
	auto obj = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem(client->GetClient(), UMediaItem::Get(item), TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*parentID), options.GetCppObj());

	FMiddlewareInfo uObj;
	uObj.Initialize(obj);

	return uObj;
}

FMiddlewareInfo URigdocksGold_MIDDLESCANNER::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId(URadnodzClient* client, UReaProject* proj, int itemID, FString language, FString parentID, FMIDDLESCANNER_Options options)
{
	auto obj = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId(client->GetClient(), UReaProject::Get(proj), itemID, TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*parentID), options.GetCppObj());

	FMiddlewareInfo uObj;
	uObj.Initialize(obj);

	return uObj;
}

FMiddlewareInfo URigdocksGold_MIDDLESCANNER::AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId(URadnodzClient* client, UReaProject* proj, int selectedItemID, FString language, FString parentID, FMIDDLESCANNER_Options options)
{
	auto obj = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId(client->GetClient(), UReaProject::Get(proj), selectedItemID, TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*parentID), options.GetCppObj());

	FMiddlewareInfo uObj;
	uObj.Initialize(obj);

	return uObj;
}

TArray<FMiddlewareInfo> URigdocksGold_MIDDLESCANNER::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList(URadnodzClient* client, TArray<UMediaItem*> itemList, FString language, FString parentID, FMIDDLESCANNER_Options options)
{
	std::vector<MediaItem*> mediaItemVector;
	for (UMediaItem* item : itemList)
	{
		mediaItemVector.push_back(UMediaItem::Get(item));
	}

	auto objList = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList(client->GetClient(), mediaItemVector, TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*parentID), options.GetCppObj());

	TArray<FMiddlewareInfo> uObjList;

	for (auto obj : objList)
	{
		FMiddlewareInfo uObj;
		uObj.Initialize(obj);
		uObjList.Add(uObj);
	}

	return uObjList;
}

