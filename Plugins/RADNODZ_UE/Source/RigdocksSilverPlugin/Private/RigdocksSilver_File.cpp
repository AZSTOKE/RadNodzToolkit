// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_File.h"
#include "RIGDOCKS_SILVER.h"

using namespace RADNODZ_client::Object;

void URigdocksSilver_File::AZ_WriteFile(URadnodzClient* client, FString directory, FString filename, FString text, int mode)
{
    SILVER::AZ_WriteFile(client->GetClient(), TCHAR_TO_UTF8(*directory), TCHAR_TO_UTF8(*filename), TCHAR_TO_UTF8(*text), mode);

}

FString URigdocksSilver_File::AZ_ReadFile(URadnodzClient* client, FString directory, FString filename)
{
    std::string text = SILVER::AZ_ReadFile(client->GetClient(), TCHAR_TO_UTF8(*directory), TCHAR_TO_UTF8(*filename));

	return UTF8_TO_TCHAR(text.c_str());

}

FString URigdocksSilver_File::AZ_GetAppDataPath(URadnodzClient* client)
{
    std::string path = SILVER::AZ_GetAppDataPath(client->GetClient());

	return UTF8_TO_TCHAR(path.c_str());
}

FString URigdocksSilver_File::AZ_GetBridgeDataPath(URadnodzClient* client)
{
    std::string path = SILVER::AZ_GetBridgeDataPath(client->GetClient());

	return UTF8_TO_TCHAR(path.c_str());
}

FPathInfo URigdocksSilver_File::AZ_GetPathInfo(URadnodzClient* client, FString file_path)
{
    PathInfo obj = SILVER::AZ_GetPathInfo(client->GetClient(), TCHAR_TO_UTF8(*file_path));

	FPathInfo pathInfoObject;
	pathInfoObject.Initialize(obj);

	return pathInfoObject;
}

TArray<FString> URigdocksSilver_File::AZ_GetFilePathList(URadnodzClient* client, FString folder_path, FString search_extension)
{
	std::vector<std::string> pathList = SILVER::AZ_GetFilePathList(client->GetClient(), TCHAR_TO_UTF8(*folder_path), TCHAR_TO_UTF8(*search_extension));

	TArray<FString> result;

	for (auto path : pathList)
	{
		 result.Add(UTF8_TO_TCHAR(path.c_str()));
	}

	return result;

}

