// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_File_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"

using namespace RADNODZ_client::Object;

void URigdocksSilver_File_Parse::AZ_WriteFile_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_WriteFile_Parse(call->Get());
}

FString URigdocksSilver_File_Parse::AZ_ReadFile_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string text = SILVER::AZ_ReadFile_Parse(call->Get());

	return UTF8_TO_TCHAR(text.c_str());
}

FString URigdocksSilver_File_Parse::AZ_GetAppDataPath_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string path = SILVER::AZ_GetAppDataPath_Parse(call->Get());

	return UTF8_TO_TCHAR(path.c_str());
}

FString URigdocksSilver_File_Parse::AZ_GetBridgeDataPath_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string path = SILVER::AZ_GetBridgeDataPath_Parse(call->Get());

	return UTF8_TO_TCHAR(path.c_str());
}

FPathInfo URigdocksSilver_File_Parse::AZ_GetPathInfo_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FPathInfo();
	}

	PathInfo obj = SILVER::AZ_GetPathInfo_Parse(call->Get());

	FPathInfo pathInfoObject;
	pathInfoObject.Initialize(obj);

	return pathInfoObject;
}

TArray<FString> URigdocksSilver_File_Parse::AZ_GetFilePathList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FString>();
	}

	std::vector<std::string> pathList = SILVER::AZ_GetFilePathList_Parse(call->Get());

	TArray<FString> result;

	for (auto path : pathList)
	{
		result.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return result;
}
