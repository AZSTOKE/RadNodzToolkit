// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksGold_SVN.h"
#include "RIGDOCKS_GOLD.h"

void  URigdocksGold_SVN::AZ_SVN_SetAuth(URadnodzClient* client, FString userName, FString password)
{
    GOLD::AZ_SVN_SetAuth(client->GetClient(), TCHAR_TO_UTF8(*userName), TCHAR_TO_UTF8(*password));
}

void  URigdocksGold_SVN::AZ_SVN_CheckoutOrUpdate(URadnodzClient* client, FString repositoryUrl, FString workingCopyPath, int revision)
{
    GOLD::AZ_SVN_CheckoutOrUpdate(client->GetClient(), TCHAR_TO_UTF8(*repositoryUrl), TCHAR_TO_UTF8(*workingCopyPath), revision);
}

TArray<FString>  URigdocksGold_SVN::AZ_SVN_Add(URadnodzClient* client, FString localPath, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const auto& exclusion : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*exclusion));
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_Add(client->GetClient(), TCHAR_TO_UTF8(*localPath), exclusionListVec, isRegex);

	TArray<FString> pathArray;

	for (const auto& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return pathArray;
}

int  URigdocksGold_SVN::AZ_SVN_Commit(URadnodzClient* client, FString localPath, FString message, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const auto& exclusion : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*exclusion));
	}

	return GOLD::AZ_SVN_Commit(client->GetClient(), TCHAR_TO_UTF8(*localPath), TCHAR_TO_UTF8(*message), exclusionListVec, isRegex);
}

void URigdocksGold_SVN::AZ_SVN_Revert(URadnodzClient* client, FString localPath, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const auto& exclusion : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*exclusion));
	}

	GOLD::AZ_SVN_Revert(client->GetClient(), TCHAR_TO_UTF8(*localPath), exclusionListVec, isRegex);
}

TArray<FString> URigdocksGold_SVN::AZ_SVN_Export(URadnodzClient* client, FString repositoryUrl, FString toDir, AZ_FileWriteMode mode, int revision, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const auto& exclusion : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*exclusion));
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_Export(client->GetClient(), TCHAR_TO_UTF8(*repositoryUrl), TCHAR_TO_UTF8(*toDir), (int)mode, revision, exclusionListVec, isRegex);

	TArray<FString> pathArray;

	for (const auto& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return pathArray;
}

TArray<FString> URigdocksGold_SVN::AZ_SVN_GetFileList(URadnodzClient* client, FString repositoryUrl, int revision, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const auto& exclusion : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*exclusion));
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_GetFileList(client->GetClient(), TCHAR_TO_UTF8(*repositoryUrl), revision, exclusionListVec, isRegex);

	TArray<FString> pathArray;

	for (const auto& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return pathArray;
}

TArray<FString> URigdocksGold_SVN::AZ_SVN_GetAddAndUpdateList(URadnodzClient* client, FString repositoryUrl, int revision, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const auto& exclusion : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*exclusion));
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_GetAddAndUpdateList(client->GetClient(), TCHAR_TO_UTF8(*repositoryUrl), revision, exclusionListVec, isRegex);

	TArray<FString> pathArray;

	for (const auto& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return pathArray;
}

FString URigdocksGold_SVN::AZ_SVN_GetRootUrl(URadnodzClient* client, FString repositoryUrl)
{

	std::string path = GOLD::AZ_SVN_GetRootUrl(client->GetClient(), TCHAR_TO_UTF8(*repositoryUrl));

	return UTF8_TO_TCHAR(path.c_str());
}

TArray<FString> URigdocksGold_SVN::AZ_SVN_Export_CommitOnly(URadnodzClient* client, FString repositoryUrl, FString toDir, AZ_FileWriteMode mode, int revision, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const auto& exclusion : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*exclusion));
	}

	std::vector<std::string> pathList = GOLD::AZ_SVN_Export_CommitOnly(client->GetClient(), TCHAR_TO_UTF8(*repositoryUrl), TCHAR_TO_UTF8(*toDir), (int)mode, revision, exclusionListVec, isRegex);

	TArray<FString> pathArray;

	for (const auto& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return pathArray;
}

TArray<FString> URigdocksGold_SVN::AZ_SVN_ExportAudioToProject(URadnodzClient* client, FString repositoryUrl, int revision, FString toDir, FString extension, bool toProjectDir)
{
	std::vector<std::string> pathList = GOLD::AZ_SVN_ExportAudioToProject(client->GetClient(), TCHAR_TO_UTF8(*repositoryUrl), revision, TCHAR_TO_UTF8(*toDir), TCHAR_TO_UTF8(*extension), toProjectDir);

	TArray<FString> pathArray;

	for (const auto& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return pathArray;
}

TArray<FString> URigdocksGold_SVN::AZ_SVN_ExportAudioToProject_CommitOnly(URadnodzClient* client, FString repositoryUrl, int revision, FString toDir, FString extension, bool toProjectDir)
{
	std::vector<std::string> pathList = GOLD::AZ_SVN_ExportAudioToProject_CommitOnly(client->GetClient(), TCHAR_TO_UTF8(*repositoryUrl), revision, TCHAR_TO_UTF8(*toDir), TCHAR_TO_UTF8(*extension), toProjectDir);

	TArray<FString> pathArray;

	for (const auto& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return pathArray;
}
