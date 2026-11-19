// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_RenderTake_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

void URigdocksSilver_RenderTake_Parse::AZ_RenderMediaItemActiveTake_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_RenderMediaItemActiveTake_Parse(call->Get());
}

void URigdocksSilver_RenderTake_Parse::AZ_RenderMediaIdActiveTake_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_RenderMediaIdActiveTake_Parse(call->Get());
}

void URigdocksSilver_RenderTake_Parse::AZ_RenderSelectedMediaIdActiveTake_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_RenderSelectedMediaIdActiveTake_Parse(call->Get());
}
