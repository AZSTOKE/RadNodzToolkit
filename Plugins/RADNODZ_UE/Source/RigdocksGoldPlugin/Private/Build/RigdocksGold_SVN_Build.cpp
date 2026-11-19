// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksGold_SVN_Build.h"
#include "RIGDOCKS_GOLD_Build.h"
#include <string>
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_SetAuth_Build(FString userName, FString password)
{
	RpcCall c = GOLD::AZ_SVN_SetAuth_Build(TCHAR_TO_UTF8(*userName), TCHAR_TO_UTF8(*password));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_CheckoutOrUpdate_Build(FString repositoryUrl, FString workingCopyPath, int revision)
{
	RpcCall c = GOLD::AZ_SVN_CheckoutOrUpdate_Build(TCHAR_TO_UTF8(*repositoryUrl), TCHAR_TO_UTF8(*workingCopyPath), revision);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_Add_Build(FString localPath, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const FString& s : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*s));
	}

	RpcCall c = GOLD::AZ_SVN_Add_Build(TCHAR_TO_UTF8(*localPath), exclusionListVec, isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_Commit_Build(FString localPath, FString message, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const FString& s : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*s));
	}

	RpcCall c = GOLD::AZ_SVN_Commit_Build(TCHAR_TO_UTF8(*localPath), TCHAR_TO_UTF8(*message), exclusionListVec, isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_Revert_Build(FString localPath, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const FString& s : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*s));
	}

	RpcCall c = GOLD::AZ_SVN_Revert_Build(TCHAR_TO_UTF8(*localPath), exclusionListVec, isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_Export_Build(FString repositoryUrl, FString toDir, AZ_FileWriteMode mode, int revision, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const FString& s : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*s));
	}

	RpcCall c = GOLD::AZ_SVN_Export_Build(TCHAR_TO_UTF8(*repositoryUrl), TCHAR_TO_UTF8(*toDir), (int)mode, revision, exclusionListVec, isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_GetFileList_Build(FString repositoryUrl, int revision, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const FString& s : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*s));
	}

	RpcCall c = GOLD::AZ_SVN_GetFileList_Build(TCHAR_TO_UTF8(*repositoryUrl), revision, exclusionListVec, isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_GetAddAndUpdateList_Build(FString repositoryUrl, int revision, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const FString& s : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*s));
	}

	RpcCall c = GOLD::AZ_SVN_GetAddAndUpdateList_Build(TCHAR_TO_UTF8(*repositoryUrl), revision, exclusionListVec, isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_GetRootUrl_Build(FString repositoryUrl)
{
	RpcCall c = GOLD::AZ_SVN_GetRootUrl_Build(TCHAR_TO_UTF8(*repositoryUrl));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_Export_CommitOnly_Build(FString repositoryUrl, FString toDir, AZ_FileWriteMode mode, int revision, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const FString& s : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*s));
	}

	RpcCall c = GOLD::AZ_SVN_Export_CommitOnly_Build(TCHAR_TO_UTF8(*repositoryUrl), TCHAR_TO_UTF8(*toDir), (int)mode, revision, exclusionListVec, isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_ExportAudioToProject_Build(FString repositoryUrl, int revision, FString toDir, FString extension, bool toProjectDir)
{
	RpcCall c = GOLD::AZ_SVN_ExportAudioToProject_Build(TCHAR_TO_UTF8(*repositoryUrl), revision, TCHAR_TO_UTF8(*toDir), TCHAR_TO_UTF8(*extension), toProjectDir);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_SVN_Build::AZ_SVN_ExportAudioToProject_CommitOnly_Build(FString repositoryUrl, int revision, FString toDir, FString extension, bool toProjectDir)
{
	RpcCall c = GOLD::AZ_SVN_ExportAudioToProject_CommitOnly_Build(TCHAR_TO_UTF8(*repositoryUrl), revision, TCHAR_TO_UTF8(*toDir), TCHAR_TO_UTF8(*extension), toProjectDir);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
