// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Random_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

int URigdocksSilver_Random_Parse::AZ_GetRandomNumInteger_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetRandomNumInteger_Parse(call->Get());
}

double URigdocksSilver_Random_Parse::AZ_GetRandomNumDouble_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return SILVER::AZ_GetRandomNumDouble_Parse(call->Get());
}

void URigdocksSilver_Random_Parse::AZ_GetRandomColor_Parse(URigdocksRpcCall* call, int& color, int& R, int& G, int& B)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(color, R, G, B) = SILVER::AZ_GetRandomColor_Parse(call->Get());
}
