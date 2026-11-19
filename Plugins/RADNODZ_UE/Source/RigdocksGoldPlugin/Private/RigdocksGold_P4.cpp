// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksGold_P4.h"

#include "RIGDOCKS_GOLD.h"

bool  URigdocksGold_P4::AZ_P4_Connect(URadnodzClient* client, FString uri, FString userName, FString password)
{
	return GOLD::AZ_P4_Connect(client->GetClient(), TCHAR_TO_UTF8(*uri), TCHAR_TO_UTF8(*userName), TCHAR_TO_UTF8(*password));
}

void  URigdocksGold_P4::AZ_P4_Disconnect(URadnodzClient* client)
{
	GOLD::AZ_P4_Disconnect(client->GetClient());
}

TArray<FString> URigdocksGold_P4::AZ_P4_SyncFiles(URadnodzClient* client, FString workspace, FString depotPath, int revision, FString extension)
{
	std::vector<std::string> pathList = GOLD::AZ_P4_SyncFiles(client->GetClient(), TCHAR_TO_UTF8(*workspace), TCHAR_TO_UTF8(*depotPath), revision, TCHAR_TO_UTF8(*extension));

	TArray<FString> pathArray;

	for (const auto& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return pathArray;
}

TArray<FString> URigdocksGold_P4::AZ_P4_ExportToDirectory(URadnodzClient* client, FString workspace, FString toDir, FString depotPath, AZ_FileWriteMode fileWriteMode, int revision, TArray<FString> exclusionList, bool isRegex)
{
	std::vector<std::string> exclusionListVec;
	for (const auto& exclusion : exclusionList) {
		exclusionListVec.push_back(TCHAR_TO_UTF8(*exclusion));
	}

	std::vector<std::string> pathList =  GOLD::AZ_P4_ExportToDirectory(client->GetClient(), TCHAR_TO_UTF8(*workspace), TCHAR_TO_UTF8(*toDir), TCHAR_TO_UTF8(*depotPath), (int)fileWriteMode, revision, exclusionListVec, isRegex);

	TArray<FString> pathArray;

	for (const auto& path : pathList) {
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return pathArray;
}

TArray<FString> URigdocksGold_P4::AZ_P4_ExportAudioToProject(URadnodzClient* client, FString workspace, FString depotPath, FString toDir, int revision, FString extension, bool toProjectDir, TArray<FString> exclusionList)
{
	std::vector<std::string> exclusionListVec;
	for (const auto& exclusion : exclusionList)
	{
		exclusionListVec.push_back(TCHAR_TO_UTF8(*exclusion));
	}

	std::vector<std::string> pathList = GOLD::AZ_P4_ExportAudioToProject(client->GetClient(), TCHAR_TO_UTF8(*workspace), TCHAR_TO_UTF8(*depotPath), TCHAR_TO_UTF8(*toDir), revision, TCHAR_TO_UTF8(*extension), toProjectDir, exclusionListVec);

	TArray<FString> pathArray;

	for (const auto& path : pathList)
	{
		pathArray.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return pathArray;
}

