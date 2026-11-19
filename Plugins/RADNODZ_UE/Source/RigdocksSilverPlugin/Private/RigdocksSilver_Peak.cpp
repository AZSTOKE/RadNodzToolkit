// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Peak.h"
#include "RIGDOCKS_SILVER.h"

using namespace RADNODZ_client::Object;

TArray<FPeak> URigdocksSilver_Peak::AZ_GetMediaItemPeakList(URadnodzClient* client, UMediaItem* item)
{

	std::vector<Peak> objArray = SILVER::AZ_GetMediaItemPeakList(client->GetClient(), UMediaItem::Get(item));

	TArray<FPeak> result;

	for (auto obj : objArray)
	{
		FPeak uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FPeak> URigdocksSilver_Peak::AZ_GetMediaIdPeakList(URadnodzClient* client, UReaProject* project, int itemID)
{

	std::vector<Peak> objArray = SILVER::AZ_GetMediaIdPeakList(client->GetClient(), UReaProject::Get(project), itemID);

	TArray<FPeak> result;

	for (auto obj : objArray)
	{
		FPeak uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FPeak> URigdocksSilver_Peak::AZ_GetSelectedMediaIdPeakList(URadnodzClient* client, UReaProject* project, int selectItemID)
{

	std::vector<Peak> objArray = SILVER::AZ_GetSelectedMediaIdPeakList(client->GetClient(), UReaProject::Get(project), selectItemID);

	TArray<FPeak> result;

	for (auto obj : objArray)
	{
		FPeak uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}