// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_RenderTake.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

void URigdocksSilver_RenderTake::AZ_RenderMediaItemActiveTake(URadnodzClient* client, UMediaItem* item)
{
	SILVER::AZ_RenderMediaItemActiveTake(client->GetClient(), UMediaItem::Get(item));
}

void URigdocksSilver_RenderTake::AZ_RenderMediaIdActiveTake(URadnodzClient* client, UReaProject* projID, int itemID)
{
	SILVER::AZ_RenderMediaIdActiveTake(client->GetClient(), UReaProject::Get(projID), itemID);
}

void URigdocksSilver_RenderTake::AZ_RenderSelectedMediaIdActiveTake(URadnodzClient* client, UReaProject* projID, int selectItemID)
{
	SILVER::AZ_RenderSelectedMediaIdActiveTake(client->GetClient(), UReaProject::Get(projID), selectItemID);
}

