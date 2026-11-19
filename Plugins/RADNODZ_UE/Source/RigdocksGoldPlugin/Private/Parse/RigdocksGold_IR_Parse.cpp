// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksGold_IR_Parse.h"
#include "RIGDOCKS_GOLD_Parse.h"
#include <string>

using namespace RADNODZ_client;

void URigdocksGold_IR_Parse::AZ_GenerateLinearTSP_Parse(URigdocksRpcCall* call, double& duration, FString& wavPath, FString& tspInfoPath)
{
	duration = 0.0;
	wavPath.Reset();
	tspInfoPath.Reset();
	if (call == nullptr)
	{
		return;
	}

	auto [durationSec, signalPath, infoPath] = GOLD::AZ_GenerateLinearTSP_Parse(call->Get());

	duration = durationSec;
	wavPath = UTF8_TO_TCHAR(signalPath.c_str());
	tspInfoPath = UTF8_TO_TCHAR(infoPath.c_str());
}

void URigdocksGold_IR_Parse::AZ_GenerateLogTSP_Parse(URigdocksRpcCall* call, double& duration, FString& wavPath, FString& tspInfoPath)
{
	duration = 0.0;
	wavPath.Reset();
	tspInfoPath.Reset();
	if (call == nullptr)
	{
		return;
	}

	auto [durationSec, signalPath, infoPath] = GOLD::AZ_GenerateLogTSP_Parse(call->Get());

	duration = durationSec;
	wavPath = UTF8_TO_TCHAR(signalPath.c_str());
	tspInfoPath = UTF8_TO_TCHAR(infoPath.c_str());
}

void URigdocksGold_IR_Parse::AZ_CalcImpulseResponseFromLinearTSP_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_CalcImpulseResponseFromLinearTSP_Parse(call->Get());
}

void URigdocksGold_IR_Parse::AZ_CalcImpulseResponseFromLogTSP_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_CalcImpulseResponseFromLogTSP_Parse(call->Get());
}
