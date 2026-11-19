// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_TALKTRON_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

TArray<FString> URigdocksSilver_TALKTRON_Parse::AZ_TALKTRON_InsertMediaItems_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FString>();
	}

	std::vector<std::string> unmatchedList = SILVER::AZ_TALKTRON_InsertMediaItems_Parse(call->Get());

	TArray<FString> result;
	for (const auto& text : unmatchedList)
	{
		result.Add(UTF8_TO_TCHAR(text.c_str()));
	}

	return result;
}
