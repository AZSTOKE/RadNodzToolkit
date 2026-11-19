// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Envelope_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

void URigdocksSilver_Envelope_Parse::AZ_SetMediaItemEnvelopeAllOFF_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetMediaItemEnvelopeAllOFF_Parse(call->Get());
}

void URigdocksSilver_Envelope_Parse::AZ_SetMediaIdEnvelopeAllOFF_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetMediaIdEnvelopeAllOFF_Parse(call->Get());
}

void URigdocksSilver_Envelope_Parse::AZ_SetSelectedMediaIdEnvelopeAllOFF_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetSelectedMediaIdEnvelopeAllOFF_Parse(call->Get());
}
