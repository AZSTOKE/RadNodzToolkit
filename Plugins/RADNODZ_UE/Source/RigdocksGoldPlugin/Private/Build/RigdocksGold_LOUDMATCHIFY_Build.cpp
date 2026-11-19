// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksGold_LOUDMATCHIFY_Build.h"
#include "RIGDOCKS_GOLD_Build.h"
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksGold_LOUDMATCHIFY_Build::AZ_LOUDMATCHIFY_SetVolumeForMediaItem_Build(UMediaItem* item, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume)
{
	std::vector<LoudnessRecord> loudnessRecordVector;
	for (FLoudnessRecord record : loudnessRecord)
	{
		loudnessRecordVector.push_back(record.GetCppObj());
	}

	RpcCall c = GOLD::AZ_LOUDMATCHIFY_SetVolumeForMediaItem_Build(UMediaItem::Get(item), loudnessRecordVector, (int)volumeType, additionalVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_LOUDMATCHIFY_Build::AZ_LOUDMATCHIFY_SetVolumeForMediaId_Build(UReaProject* proj, int itemID, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume)
{
	std::vector<LoudnessRecord> loudnessRecordVector;
	for (FLoudnessRecord record : loudnessRecord)
	{
		loudnessRecordVector.push_back(record.GetCppObj());
	}

	RpcCall c = GOLD::AZ_LOUDMATCHIFY_SetVolumeForMediaId_Build(UReaProject::Get(proj), itemID, loudnessRecordVector, (int)volumeType, additionalVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_LOUDMATCHIFY_Build::AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId_Build(UReaProject* proj, int selectedItemID, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume)
{
	std::vector<LoudnessRecord> loudnessRecordVector;
	for (FLoudnessRecord record : loudnessRecord)
	{
		loudnessRecordVector.push_back(record.GetCppObj());
	}

	RpcCall c = GOLD::AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId_Build(UReaProject::Get(proj), selectedItemID, loudnessRecordVector, (int)volumeType, additionalVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_LOUDMATCHIFY_Build::AZ_LOUDMATCHIFY_SetVolumeForMediaItemList_Build(TArray<UMediaItem*> itemList, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, TArray<FMiddlewareInfo> additionalVolumeList)
{
	std::vector<MediaItem*> mediaItemVector;
	for (UMediaItem* item : itemList)
	{
		mediaItemVector.push_back(UMediaItem::Get(item));
	}

	std::vector<LoudnessRecord> loudnessRecordVector;
	for (FLoudnessRecord record : loudnessRecord)
	{
		loudnessRecordVector.push_back(record.GetCppObj());
	}

	std::vector<MiddlewareInfo> additionalVolumeVector;
	for (FMiddlewareInfo info : additionalVolumeList)
	{
		additionalVolumeVector.push_back(info.GetCppObj());
	}

	RpcCall c = GOLD::AZ_LOUDMATCHIFY_SetVolumeForMediaItemList_Build(mediaItemVector, loudnessRecordVector, (int)volumeType, additionalVolumeVector);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
