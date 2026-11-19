// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Action_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

void URigdocksSilver_Action_Parse::AZ_ActionMediaItemSelectKeep_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_ActionMediaItemSelectKeep_Parse(call->Get());
}

void URigdocksSilver_Action_Parse::AZ_ActionMediaIdSelectKeep_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_ActionMediaIdSelectKeep_Parse(call->Get());
}

void URigdocksSilver_Action_Parse::AZ_ActionSelectedMediaIdSelectKeep_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_ActionSelectedMediaIdSelectKeep_Parse(call->Get());
}
