// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksGold_P4_Parse.h"
#include "RIGDOCKS_GOLD_Parse.h"
#include <string>
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

bool URigdocksGold_P4_Parse::AZ_P4_Connect_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return GOLD::AZ_P4_Connect_Parse(call->Get());
}

void URigdocksGold_P4_Parse::AZ_P4_Disconnect_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_P4_Disconnect_Parse(call->Get());
}

TArray<FString> URigdocksGold_P4_Parse::AZ_P4_SyncFiles_Parse(URigdocksRpcCall* call)
{
	TArray<FString> pathArray;
	if (call == nullptr)
	{
		return pathArray;
	}

	std::vector<std::string> pathList = GOLD::AZ_P4_SyncFiles_Parse(call->Get());
	for (const std::string& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}
	return pathArray;
}

TArray<FString> URigdocksGold_P4_Parse::AZ_P4_ExportToDirectory_Parse(URigdocksRpcCall* call)
{
	TArray<FString> pathArray;
	if (call == nullptr)
	{
		return pathArray;
	}

	std::vector<std::string> pathList = GOLD::AZ_P4_ExportToDirectory_Parse(call->Get());
	for (const std::string& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}
	return pathArray;
}

TArray<FString> URigdocksGold_P4_Parse::AZ_P4_ExportAudioToProject_Parse(URigdocksRpcCall* call)
{
	TArray<FString> pathArray;
	if (call == nullptr)
	{
		return pathArray;
	}

	std::vector<std::string> pathList = GOLD::AZ_P4_ExportAudioToProject_Parse(call->Get());
	for (const std::string& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}
	return pathArray;
}
