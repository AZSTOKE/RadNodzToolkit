// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Envelope.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

 	void URigdocksSilver_Envelope::AZ_SetMediaItemEnvelopeAllOFF(URadnodzClient* client, UMediaItem* item)
    {
        SILVER::AZ_SetMediaItemEnvelopeAllOFF(client->GetClient(), UMediaItem::Get(item));

    }

 	void URigdocksSilver_Envelope::AZ_SetMediaIdEnvelopeAllOFF(URadnodzClient* client, UReaProject* projID, int itemID)
    {
        SILVER::AZ_SetMediaIdEnvelopeAllOFF(client->GetClient(), UReaProject::Get(projID), itemID);

    }

 	void URigdocksSilver_Envelope::AZ_SetSelectedMediaIdEnvelopeAllOFF(URadnodzClient* client, UReaProject* projID, int selectItemID)
    {
        SILVER::AZ_SetSelectedMediaIdEnvelopeAllOFF(client->GetClient(), UReaProject::Get(projID), selectItemID);

    }

