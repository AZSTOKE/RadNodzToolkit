// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Path_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

FString URigdocksSilver_Path_Parse::AZ_SetPathFolder_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string path = SILVER::AZ_SetPathFolder_Parse(call->Get());

	return UTF8_TO_TCHAR(path.c_str());
}

void URigdocksSilver_Path_Parse::AZ_SetCopyFile_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetCopyFile_Parse(call->Get());
}
