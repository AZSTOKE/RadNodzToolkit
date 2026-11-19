// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_HANDAUTOMER_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

void URigdocksSilver_HANDAUTOMER_Parse::AZ_SetMediaItemHANDAUTOMER_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetMediaItemHANDAUTOMER_Parse(call->Get());
}

void URigdocksSilver_HANDAUTOMER_Parse::AZ_SetMediaIdHANDAUTOMER_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetMediaIdHANDAUTOMER_Parse(call->Get());
}

void URigdocksSilver_HANDAUTOMER_Parse::AZ_SetSelectedMediaIdHANDAUTOMER_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedMediaIdHANDAUTOMER_Parse(call->Get());
}

void URigdocksSilver_HANDAUTOMER_Parse::AZ_SetMediaItemListHANDAUTOMER_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetMediaItemListHANDAUTOMER_Parse(call->Get());
}
