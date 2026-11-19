// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Peak_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"

using namespace RADNODZ_client::Object;

TArray<FPeak> URigdocksSilver_Peak_Parse::AZ_GetMediaItemPeakList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FPeak>();
	}

	std::vector<Peak> objArray = SILVER::AZ_GetMediaItemPeakList_Parse(call->Get());

	TArray<FPeak> result;

	for (auto obj : objArray)
	{
		FPeak uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FPeak> URigdocksSilver_Peak_Parse::AZ_GetMediaIdPeakList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FPeak>();
	}

	std::vector<Peak> objArray = SILVER::AZ_GetMediaIdPeakList_Parse(call->Get());

	TArray<FPeak> result;

	for (auto obj : objArray)
	{
		FPeak uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FPeak> URigdocksSilver_Peak_Parse::AZ_GetSelectedMediaIdPeakList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FPeak>();
	}

	std::vector<Peak> objArray = SILVER::AZ_GetSelectedMediaIdPeakList_Parse(call->Get());

	TArray<FPeak> result;

	for (auto obj : objArray)
	{
		FPeak uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}
