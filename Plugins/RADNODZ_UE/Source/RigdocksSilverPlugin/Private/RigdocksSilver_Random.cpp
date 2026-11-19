// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Random.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

int URigdocksSilver_Random::AZ_GetRandomNumInteger(URadnodzClient* client, int low, int high)
{
	return SILVER::AZ_GetRandomNumInteger(client->GetClient(), low, high);
}

double URigdocksSilver_Random::AZ_GetRandomNumDouble(URadnodzClient* client, double low, double high, int doubleType)
{
	return SILVER::AZ_GetRandomNumDouble(client->GetClient(), low, high, doubleType);
}

void URigdocksSilver_Random::AZ_GetRandomColor(URadnodzClient* client, int& color, int& R, int& G, int& B)
{
	std::tie(color, R, G, B) = SILVER::AZ_GetRandomColor(client->GetClient());
}
