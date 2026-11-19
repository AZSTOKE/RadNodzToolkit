// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Path.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

FString URigdocksSilver_Path::AZ_SetPathFolder(URadnodzClient* client, FString folderPath, FString folderName, int timeAddType)
{
    std::string  path = SILVER::AZ_SetPathFolder(client->GetClient(), TCHAR_TO_UTF8(*folderPath), TCHAR_TO_UTF8(*folderName), timeAddType);

	return UTF8_TO_TCHAR(path.c_str());
}

void URigdocksSilver_Path::AZ_SetCopyFile(URadnodzClient* client, FString copyFilePath, FString pasteFilePath)
{
	SILVER::AZ_SetCopyFile(client->GetClient(), TCHAR_TO_UTF8(*copyFilePath), TCHAR_TO_UTF8(*pasteFilePath));
}

