// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Loudness.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

void URigdocksSilver_Loudness::AZ_GetMediaItemLoudnessMaxMomentary(URadnodzClient* client, UMediaItem* item, double& max_lufs_m, double& position)
{
	std::tie(max_lufs_m, position) = SILVER::AZ_GetMediaItemLoudnessMaxMomentary(client->GetClient(), UMediaItem::Get(item));

}

void URigdocksSilver_Loudness::AZ_GetMediaIdLoudnessMaxMomentary(URadnodzClient* client, UReaProject* proj, int itemID, double& max_lufs_m, double& position)
{
	std::tie(max_lufs_m, position) = SILVER::AZ_GetMediaIdLoudnessMaxMomentary(client->GetClient(), UReaProject::Get(proj), itemID);

}

void URigdocksSilver_Loudness::AZ_GetSelectedMediaIdLoudnessMaxMomentary(URadnodzClient* client, UReaProject* proj, int selectItemID, double& max_lufs_m, double& position)
{
	std::tie(max_lufs_m, position) = SILVER::AZ_GetSelectedMediaIdLoudnessMaxMomentary(client->GetClient(), UReaProject::Get(proj), selectItemID);

}

double URigdocksSilver_Loudness::AZ_SetMediaItemLoudnessMaxMomentary(URadnodzClient* client, UMediaItem* item, double setMaxMomentary)
{
	return SILVER::AZ_SetMediaItemLoudnessMaxMomentary(client->GetClient(), UMediaItem::Get(item), setMaxMomentary);

}

double URigdocksSilver_Loudness::AZ_SetMediaIdLoudnessMaxMomentary(URadnodzClient* client, UReaProject* proj, int itemID, double setMaxMomentary)
{
	return SILVER::AZ_SetMediaIdLoudnessMaxMomentary(client->GetClient(), UReaProject::Get(proj), itemID, setMaxMomentary);

}

double URigdocksSilver_Loudness::AZ_SetSelectedMediaIdLoudnessMaxMomentary(URadnodzClient* client, UReaProject* proj, int selectItemID, double setMaxMomentary)
{
	return SILVER::AZ_SetSelectedMediaIdLoudnessMaxMomentary(client->GetClient(), UReaProject::Get(proj), selectItemID, setMaxMomentary);

}

TArray<FSection> URigdocksSilver_Loudness::AZ_GetSilentSectionListInMediaItem(URadnodzClient* client, UMediaItem* item, double minSilentDulation, double threshold)
{

	std::vector<Section> objArray = SILVER::AZ_GetSilentSectionListInMediaItem(client->GetClient(), UMediaItem::Get(item), minSilentDulation, threshold);

	TArray<FSection> result;

	for (auto obj : objArray)
	{
		FSection uObj;

		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FSection> URigdocksSilver_Loudness::AZ_GetAudioSectionListInMediaItem(URadnodzClient* client, UMediaItem* item, double maxSilentDulation, double threshold)
{

	std::vector<Section> objArray = SILVER::AZ_GetAudioSectionListInMediaItem(client->GetClient(), UMediaItem::Get(item), maxSilentDulation, threshold);

	TArray<FSection> result;

	for (auto obj : objArray)
	{
		FSection uObj;

		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

