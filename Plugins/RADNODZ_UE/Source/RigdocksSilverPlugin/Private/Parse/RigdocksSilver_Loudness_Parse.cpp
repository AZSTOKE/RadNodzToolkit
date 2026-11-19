// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Loudness_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

void URigdocksSilver_Loudness_Parse::AZ_GetMediaItemLoudnessMaxMomentary_Parse(URigdocksRpcCall* call, double& max_lufs_m, double& position)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(max_lufs_m, position) = SILVER::AZ_GetMediaItemLoudnessMaxMomentary_Parse(call->Get());
}

void URigdocksSilver_Loudness_Parse::AZ_GetMediaIdLoudnessMaxMomentary_Parse(URigdocksRpcCall* call, double& max_lufs_m, double& position)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(max_lufs_m, position) = SILVER::AZ_GetMediaIdLoudnessMaxMomentary_Parse(call->Get());
}

void URigdocksSilver_Loudness_Parse::AZ_GetSelectedMediaIdLoudnessMaxMomentary_Parse(URigdocksRpcCall* call, double& max_lufs_m, double& position)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(max_lufs_m, position) = SILVER::AZ_GetSelectedMediaIdLoudnessMaxMomentary_Parse(call->Get());
}

double URigdocksSilver_Loudness_Parse::AZ_SetMediaItemLoudnessMaxMomentary_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return SILVER::AZ_SetMediaItemLoudnessMaxMomentary_Parse(call->Get());
}

double URigdocksSilver_Loudness_Parse::AZ_SetMediaIdLoudnessMaxMomentary_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return SILVER::AZ_SetMediaIdLoudnessMaxMomentary_Parse(call->Get());
}

double URigdocksSilver_Loudness_Parse::AZ_SetSelectedMediaIdLoudnessMaxMomentary_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return SILVER::AZ_SetSelectedMediaIdLoudnessMaxMomentary_Parse(call->Get());
}

TArray<FSection> URigdocksSilver_Loudness_Parse::AZ_GetSilentSectionListInMediaItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FSection>();
	}

	std::vector<Section> objArray = SILVER::AZ_GetSilentSectionListInMediaItem_Parse(call->Get());

	TArray<FSection> result;
	for (auto obj : objArray)
	{
		FSection uObj;
		uObj.Initialize(obj);
		result.Add(uObj);
	}

	return result;
}

TArray<FSection> URigdocksSilver_Loudness_Parse::AZ_GetAudioSectionListInMediaItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FSection>();
	}

	std::vector<Section> objArray = SILVER::AZ_GetAudioSectionListInMediaItem_Parse(call->Get());

	TArray<FSection> result;
	for (auto obj : objArray)
	{
		FSection uObj;
		uObj.Initialize(obj);
		result.Add(uObj);
	}

	return result;
}
