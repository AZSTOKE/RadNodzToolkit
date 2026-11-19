// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksGold_MIDDLESCANNER_Parse.h"
#include "RIGDOCKS_GOLD_Parse.h"
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

FMiddlewareInfo URigdocksGold_MIDDLESCANNER_Parse::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem_Parse(URigdocksRpcCall* call)
{
	FMiddlewareInfo uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	auto obj = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem_Parse(call->Get());
	uObj.Initialize(obj);
	return uObj;
}

FMiddlewareInfo URigdocksGold_MIDDLESCANNER_Parse::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId_Parse(URigdocksRpcCall* call)
{
	FMiddlewareInfo uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	auto obj = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId_Parse(call->Get());
	uObj.Initialize(obj);
	return uObj;
}

FMiddlewareInfo URigdocksGold_MIDDLESCANNER_Parse::AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId_Parse(URigdocksRpcCall* call)
{
	FMiddlewareInfo uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	auto obj = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId_Parse(call->Get());
	uObj.Initialize(obj);
	return uObj;
}

TArray<FMiddlewareInfo> URigdocksGold_MIDDLESCANNER_Parse::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList_Parse(URigdocksRpcCall* call)
{
	TArray<FMiddlewareInfo> uObjList;
	if (call == nullptr)
	{
		return uObjList;
	}

	auto objList = GOLD::AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList_Parse(call->Get());
	for (auto obj : objList)
	{
		FMiddlewareInfo uObj;
		uObj.Initialize(obj);
		uObjList.Add(uObj);
	}
	return uObjList;
}
