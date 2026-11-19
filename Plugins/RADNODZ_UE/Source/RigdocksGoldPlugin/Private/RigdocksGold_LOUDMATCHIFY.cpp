// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksGold_LOUDMATCHIFY.h"
#include "RIGDOCKS_GOLD.h"

using namespace RADNODZ_client::Object;

void  URigdocksGold_LOUDMATCHIFY::AZ_LOUDMATCHIFY_SetVolumeForMediaItem(URadnodzClient* client, UMediaItem* item, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume)
{
	std::vector<LoudnessRecord> loudnessRecordVector;
	for (FLoudnessRecord record : loudnessRecord) 
	{
		loudnessRecordVector.push_back(record.GetCppObj());
	}

	GOLD::AZ_LOUDMATCHIFY_SetVolumeForMediaItem(client->GetClient(), UMediaItem::Get(item), loudnessRecordVector, (int)volumeType, additionalVolume);
}

void  URigdocksGold_LOUDMATCHIFY::AZ_LOUDMATCHIFY_SetVolumeForMediaId(URadnodzClient* client, UReaProject* proj, int itemID, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume)
{
	std::vector<LoudnessRecord> loudnessRecordVector;
	for (FLoudnessRecord record : loudnessRecord)
	{
		loudnessRecordVector.push_back(record.GetCppObj());
	}

	GOLD::AZ_LOUDMATCHIFY_SetVolumeForMediaId(client->GetClient(), UReaProject::Get(proj), itemID, loudnessRecordVector, (int)volumeType, additionalVolume);
}

void  URigdocksGold_LOUDMATCHIFY::AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId(URadnodzClient* client, UReaProject* proj, int selectedItemID, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume)
{
	std::vector<LoudnessRecord> loudnessRecordVector;
	for (FLoudnessRecord record : loudnessRecord)
	{
		loudnessRecordVector.push_back(record.GetCppObj());
	}

	GOLD::AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId(client->GetClient(), UReaProject::Get(proj), selectedItemID, loudnessRecordVector, (int)volumeType, additionalVolume);
}

void  URigdocksGold_LOUDMATCHIFY::AZ_LOUDMATCHIFY_SetVolumeForMediaItemList(URadnodzClient* client, TArray<UMediaItem*> itemList, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, TArray<FMiddlewareInfo> additionalVolumeList)
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

	GOLD::AZ_LOUDMATCHIFY_SetVolumeForMediaItemList(client->GetClient(), mediaItemVector, loudnessRecordVector, (int)volumeType, additionalVolumeVector);
}

