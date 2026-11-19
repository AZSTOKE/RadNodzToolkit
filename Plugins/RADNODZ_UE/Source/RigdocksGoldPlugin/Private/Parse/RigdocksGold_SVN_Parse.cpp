// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksGold_SVN_Parse.h"
#include "RIGDOCKS_GOLD_Parse.h"
#include <string>
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

void URigdocksGold_SVN_Parse::AZ_SVN_SetAuth_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_SVN_SetAuth_Parse(call->Get());
}

void URigdocksGold_SVN_Parse::AZ_SVN_CheckoutOrUpdate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_SVN_CheckoutOrUpdate_Parse(call->Get());
}

TArray<FString> URigdocksGold_SVN_Parse::AZ_SVN_Add_Parse(URigdocksRpcCall* call)
{
	TArray<FString> pathArray;
	if (call == nullptr)
	{
		return pathArray;
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_Add_Parse(call->Get());
	for (const std::string& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}
	return pathArray;
}

int URigdocksGold_SVN_Parse::AZ_SVN_Commit_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return GOLD::AZ_SVN_Commit_Parse(call->Get());
}

void URigdocksGold_SVN_Parse::AZ_SVN_Revert_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_SVN_Revert_Parse(call->Get());
}

TArray<FString> URigdocksGold_SVN_Parse::AZ_SVN_Export_Parse(URigdocksRpcCall* call)
{
	TArray<FString> pathArray;
	if (call == nullptr)
	{
		return pathArray;
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_Export_Parse(call->Get());
	for (const std::string& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}
	return pathArray;
}

TArray<FString> URigdocksGold_SVN_Parse::AZ_SVN_GetFileList_Parse(URigdocksRpcCall* call)
{
	TArray<FString> pathArray;
	if (call == nullptr)
	{
		return pathArray;
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_GetFileList_Parse(call->Get());
	for (const std::string& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}
	return pathArray;
}

TArray<FString> URigdocksGold_SVN_Parse::AZ_SVN_GetAddAndUpdateList_Parse(URigdocksRpcCall* call)
{
	TArray<FString> pathArray;
	if (call == nullptr)
	{
		return pathArray;
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_GetAddAndUpdateList_Parse(call->Get());
	for (const std::string& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}
	return pathArray;
}

FString URigdocksGold_SVN_Parse::AZ_SVN_GetRootUrl_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string path = GOLD::AZ_SVN_GetRootUrl_Parse(call->Get());
	return UTF8_TO_TCHAR(path.c_str());
}

TArray<FString> URigdocksGold_SVN_Parse::AZ_SVN_Export_CommitOnly_Parse(URigdocksRpcCall* call)
{
	TArray<FString> pathArray;
	if (call == nullptr)
	{
		return pathArray;
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_Export_CommitOnly_Parse(call->Get());
	for (const std::string& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}
	return pathArray;
}

TArray<FString> URigdocksGold_SVN_Parse::AZ_SVN_ExportAudioToProject_Parse(URigdocksRpcCall* call)
{
	TArray<FString> pathArray;
	if (call == nullptr)
	{
		return pathArray;
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_ExportAudioToProject_Parse(call->Get());
	for (const std::string& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}
	return pathArray;
}

TArray<FString> URigdocksGold_SVN_Parse::AZ_SVN_ExportAudioToProject_CommitOnly_Parse(URigdocksRpcCall* call)
{
	TArray<FString> pathArray;
	if (call == nullptr)
	{
		return pathArray;
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_ExportAudioToProject_CommitOnly_Parse(call->Get());
	for (const std::string& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}
	return pathArray;
}
