// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Path_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksSilver_Path_Build::AZ_SetPathFolder_Build(FString folderPath, FString folderName, int timeAddType)
{
	RpcCall c = SILVER::AZ_SetPathFolder_Build(TCHAR_TO_UTF8(*folderPath), TCHAR_TO_UTF8(*folderName), timeAddType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Path_Build::AZ_SetCopyFile_Build(FString copyFilePath, FString pasteFilePath)
{
	RpcCall c = SILVER::AZ_SetCopyFile_Build(TCHAR_TO_UTF8(*copyFilePath), TCHAR_TO_UTF8(*pasteFilePath));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
