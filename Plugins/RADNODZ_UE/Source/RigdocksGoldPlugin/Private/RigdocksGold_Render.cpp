// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksGold_Render.h"
#include "RIGDOCKS_GOLD.h"

using namespace RADNODZ_client::Object;

void URigdocksGold_Render::AZ_RenderToAudioFile(URadnodzClient* client, UReaProject* project, FRenderingOptions options, TArray<FString>& primaryOutputList, TArray<FString>& secoundaryOutputList)
{
	RenderingOptions cppObj = options.GetCppObj();

	auto[primaryOutputListVec, secoundaryOutputListVec] = GOLD::AZ_RenderToAudioFile(client->GetClient(), UReaProject::Get(project), cppObj);

	primaryOutputList = TArray<FString>();
	for (const auto& output : primaryOutputListVec)
	{
		primaryOutputList.Add(UTF8_TO_TCHAR(output.c_str()));
	}

	secoundaryOutputList = TArray<FString>();
	for (const auto& output : secoundaryOutputListVec)
	{
		secoundaryOutputList.Add(UTF8_TO_TCHAR(output.c_str()));
	}
}

FRenderingOptions URigdocksGold_Render::AZ_GetRenderSetting(URadnodzClient* client, UReaProject* project)
{
	auto cppObj = GOLD::AZ_GetRenderSetting(client->GetClient(), UReaProject::Get(project));

	FRenderingOptions options;
	options.Initialize(cppObj);

	return options;
}

FRenderingOptions URigdocksGold_Render::AZ_GetRenderSettingFromProject(URadnodzClient* client, FString projectPath)
{
	auto cppObj = GOLD::AZ_GetRenderSettingFromProject(client->GetClient(), TCHAR_TO_UTF8(*projectPath));

	FRenderingOptions options;
	options.Initialize(cppObj);

	return options;
}

void URigdocksGold_Render::AZ_SetRenderSetting(URadnodzClient* client, UReaProject* project, FRenderingOptions options)
{
	RenderingOptions cppObj = options.GetCppObj();

	GOLD::AZ_SetRenderSetting(client->GetClient(), UReaProject::Get(project), cppObj);
}