// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_File_Build.h"
#include "RIGDOCKS_SILVER_Build.h"

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksSilver_File_Build::AZ_WriteFile_Build(FString directory, FString filename, FString text, int mode)
{
	RpcCall c = SILVER::AZ_WriteFile_Build(TCHAR_TO_UTF8(*directory), TCHAR_TO_UTF8(*filename), TCHAR_TO_UTF8(*text), mode);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_File_Build::AZ_ReadFile_Build(FString directory, FString filename)
{
	RpcCall c = SILVER::AZ_ReadFile_Build(TCHAR_TO_UTF8(*directory), TCHAR_TO_UTF8(*filename));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_File_Build::AZ_GetAppDataPath_Build()
{
	RpcCall c = SILVER::AZ_GetAppDataPath_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_File_Build::AZ_GetBridgeDataPath_Build()
{
	RpcCall c = SILVER::AZ_GetBridgeDataPath_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_File_Build::AZ_GetPathInfo_Build(FString file_path)
{
	RpcCall c = SILVER::AZ_GetPathInfo_Build(TCHAR_TO_UTF8(*file_path));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_File_Build::AZ_GetFilePathList_Build(FString folder_path, FString search_extension)
{
	RpcCall c = SILVER::AZ_GetFilePathList_Build(TCHAR_TO_UTF8(*folder_path), TCHAR_TO_UTF8(*search_extension));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
