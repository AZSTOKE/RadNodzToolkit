// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksGold_P4_Build.h"
#include "RIGDOCKS_GOLD_Build.h"
#include <string>
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksGold_P4_Build::AZ_P4_Connect_Build(FString uri, FString userName, FString password)
{
	RpcCall c = GOLD::AZ_P4_Connect_Build(TCHAR_TO_UTF8(*uri), TCHAR_TO_UTF8(*userName), TCHAR_TO_UTF8(*password));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_P4_Build::AZ_P4_Disconnect_Build()
{
	RpcCall c = GOLD::AZ_P4_Disconnect_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_P4_Build::AZ_P4_SyncFiles_Build(FString workspace, FString depotPath, int changeListID, FString extension)
{
	RpcCall c = GOLD::AZ_P4_SyncFiles_Build(TCHAR_TO_UTF8(*workspace), TCHAR_TO_UTF8(*depotPath), changeListID, TCHAR_TO_UTF8(*extension));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_P4_Build::AZ_P4_ExportToDirectory_Build(FString workspace, FString toDir, FString depotPath, AZ_FileWriteMode fileWriteMode, int changeListID, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const FString& s : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*s));
	}

	RpcCall c = GOLD::AZ_P4_ExportToDirectory_Build(TCHAR_TO_UTF8(*workspace), TCHAR_TO_UTF8(*toDir), TCHAR_TO_UTF8(*depotPath), (int)fileWriteMode, changeListID, exclusionListVec, isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_P4_Build::AZ_P4_ExportAudioToProject_Build(FString workspace, FString depotPath, FString toDir, int changeListID, FString extension, bool toProjectDir, TArray<FString> exclusionList)
{
	std::vector<std::string> exclusionListVec;
	for (const FString& s : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*s));
	}

	RpcCall c = GOLD::AZ_P4_ExportAudioToProject_Build(TCHAR_TO_UTF8(*workspace), TCHAR_TO_UTF8(*depotPath), TCHAR_TO_UTF8(*toDir), changeListID, TCHAR_TO_UTF8(*extension), toProjectDir, exclusionListVec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
