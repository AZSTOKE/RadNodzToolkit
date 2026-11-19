// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksGold_Render_Build.h"
#include "RIGDOCKS_GOLD_Build.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksGold_Render_Build::AZ_RenderToAudioFile_Build(UReaProject* project, FRenderingOptions options)
{
	RenderingOptions cppObj = options.GetCppObj();

	RpcCall c = GOLD::AZ_RenderToAudioFile_Build(UReaProject::Get(project), cppObj);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Render_Build::AZ_GetRenderSetting_Build(UReaProject* project)
{
	RpcCall c = GOLD::AZ_GetRenderSetting_Build(UReaProject::Get(project));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Render_Build::AZ_GetRenderSettingFromProject_Build(FString projectPath)
{
	RpcCall c = GOLD::AZ_GetRenderSettingFromProject_Build(TCHAR_TO_UTF8(*projectPath));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Render_Build::AZ_SetRenderSetting_Build(UReaProject* project, FRenderingOptions options)
{
	RenderingOptions cppObj = options.GetCppObj();

	RpcCall c = GOLD::AZ_SetRenderSetting_Build(UReaProject::Get(project), cppObj);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
