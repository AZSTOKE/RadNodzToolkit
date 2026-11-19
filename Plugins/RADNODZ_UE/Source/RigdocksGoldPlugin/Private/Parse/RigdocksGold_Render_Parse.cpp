// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksGold_Render_Parse.h"
#include "RIGDOCKS_GOLD_Parse.h"
#include <string>
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

void URigdocksGold_Render_Parse::AZ_RenderToAudioFile_Parse(URigdocksRpcCall* call, TArray<FString>& primaryOutputList, TArray<FString>& secoundaryOutputList)
{
	primaryOutputList.Reset();
	secoundaryOutputList.Reset();
	if (call == nullptr)
	{
		return;
	}

	auto [primaryVec, secoundaryVec] = GOLD::AZ_RenderToAudioFile_Parse(call->Get());

	for (const std::string& o : primaryVec)
	{
		primaryOutputList.Add(UTF8_TO_TCHAR(o.c_str()));
	}
	for (const std::string& o : secoundaryVec)
	{
		secoundaryOutputList.Add(UTF8_TO_TCHAR(o.c_str()));
	}
}

FRenderingOptions URigdocksGold_Render_Parse::AZ_GetRenderSetting_Parse(URigdocksRpcCall* call)
{
	FRenderingOptions options;
	if (call == nullptr)
	{
		return options;
	}

	auto cppObj = GOLD::AZ_GetRenderSetting_Parse(call->Get());
	options.Initialize(cppObj);
	return options;
}

FRenderingOptions URigdocksGold_Render_Parse::AZ_GetRenderSettingFromProject_Parse(URigdocksRpcCall* call)
{
	FRenderingOptions options;
	if (call == nullptr)
	{
		return options;
	}

	auto cppObj = GOLD::AZ_GetRenderSettingFromProject_Parse(call->Get());
	options.Initialize(cppObj);
	return options;
}

void URigdocksGold_Render_Parse::AZ_SetRenderSetting_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_SetRenderSetting_Parse(call->Get());
}
