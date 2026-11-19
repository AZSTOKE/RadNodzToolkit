// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Action.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

 	void URigdocksSilver_Action::AZ_ActionMediaItemSelectKeep(URadnodzClient* client, UMediaItem* item, int cmdID)
    {
        SILVER::AZ_ActionMediaItemSelectKeep(client->GetClient(), UMediaItem::Get(item), cmdID);

    }

 	void URigdocksSilver_Action::AZ_ActionMediaIdSelectKeep(URadnodzClient* client, UReaProject* projID, int itemID, int cmdID)
    {
        SILVER::AZ_ActionMediaIdSelectKeep(client->GetClient(), UReaProject::Get(projID), itemID, cmdID);

    }

 	void URigdocksSilver_Action::AZ_ActionSelectedMediaIdSelectKeep(URadnodzClient* client, UReaProject* projID, int selectItemID, int cmdID)
    {
        SILVER::AZ_ActionSelectedMediaIdSelectKeep(client->GetClient(), UReaProject::Get(projID), selectItemID, cmdID);

    }