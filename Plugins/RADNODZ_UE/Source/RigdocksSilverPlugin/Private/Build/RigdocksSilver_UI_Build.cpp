// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_UI_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksSilver_UI_Build::AZ_ShowMessageBox_Ex_Build(FString messageStr, FString windowTitleStr, AZ_WindowType windowType)
{
	RpcCall c = SILVER::AZ_ShowMessageBox_Ex_Build(TCHAR_TO_UTF8(*messageStr), TCHAR_TO_UTF8(*windowTitleStr), (int)windowType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_UI_Build::AZ_GetUserInputList_Build(FString titleStr, int inputNum, FString captionStr, FString resultStr)
{
	RpcCall c = SILVER::AZ_GetUserInputList_Build(TCHAR_TO_UTF8(*titleStr), inputNum, TCHAR_TO_UTF8(*captionStr), TCHAR_TO_UTF8(*resultStr));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_UI_Build::AZ_ShowMessageBox_Info_Build(FString message, FString title, AZ_WindowType windowType)
{
	RpcCall c = SILVER::AZ_ShowMessageBox_Info_Build(TCHAR_TO_UTF8(*message), TCHAR_TO_UTF8(*title), (int)windowType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_UI_Build::AZ_ShowMessageBox_Warn_Build(FString message, FString title, AZ_WindowType windowType)
{
	RpcCall c = SILVER::AZ_ShowMessageBox_Warn_Build(TCHAR_TO_UTF8(*message), TCHAR_TO_UTF8(*title), (int)windowType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_UI_Build::AZ_ShowMessageBox_Error_Build(FString message, FString title, AZ_WindowType windowType)
{
	RpcCall c = SILVER::AZ_ShowMessageBox_Error_Build(TCHAR_TO_UTF8(*message), TCHAR_TO_UTF8(*title), (int)windowType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
