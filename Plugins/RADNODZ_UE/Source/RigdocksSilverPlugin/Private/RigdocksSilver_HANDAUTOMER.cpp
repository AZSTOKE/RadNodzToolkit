// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_HANDAUTOMER.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

void URigdocksSilver_HANDAUTOMER::AZ_SetMediaItemHANDAUTOMER(URadnodzClient* client, UMediaItem* item, int ratioType)
{
    SILVER::AZ_SetMediaItemHANDAUTOMER(client->GetClient(), UMediaItem::Get(item), ratioType);

}

void URigdocksSilver_HANDAUTOMER::AZ_SetMediaIdHANDAUTOMER(URadnodzClient* client, UReaProject* projID, int itemID, int ratioType)
{
    SILVER::AZ_SetMediaIdHANDAUTOMER(client->GetClient(), UReaProject::Get(projID), itemID, ratioType);

}

void URigdocksSilver_HANDAUTOMER::AZ_SetSelectedMediaIdHANDAUTOMER(URadnodzClient* client, UReaProject* projID, int selectItemID, int ratioType)
{
    SILVER::AZ_SetSelectedMediaIdHANDAUTOMER(client->GetClient(), UReaProject::Get(projID), selectItemID, ratioType);

}

void URigdocksSilver_HANDAUTOMER::AZ_SetMediaItemListHANDAUTOMER(URadnodzClient* client, TArray<UMediaItem*> itemList, int ratioType)
{
	std::vector<MediaItem*> nativeItemList;
	for (UMediaItem* item : itemList)
	{
		nativeItemList.push_back(UMediaItem::Get(item));
	}

    SILVER::AZ_SetMediaItemListHANDAUTOMER(client->GetClient(), nativeItemList, ratioType);

}