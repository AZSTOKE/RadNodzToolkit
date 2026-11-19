// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Device.h"
#include "RIGDOCKS_SILVER.h"

using namespace RADNODZ_client::Object;

TArray<FString> URigdocksSilver_Device::AZ_GetLocalIpAddresse(URadnodzClient* client)
{
	std::vector<std::string> objArray = SILVER::AZ_GetLocalIpAddresse(client->GetClient());

	TArray<FString> result;

	for (auto obj : objArray)
	{
		FString str = UTF8_TO_TCHAR(obj.c_str());

		result.Add(str);
	}

	return result;
}

FString URigdocksSilver_Device::AZ_GetDeviceName(URadnodzClient* client)
{
	std::string nameStr = SILVER::AZ_GetDeviceName(client->GetClient());

	return UTF8_TO_TCHAR(nameStr.c_str());
}

int URigdocksSilver_Device::AZ_CountInputChannel(URadnodzClient* client)
{
	return SILVER::AZ_CountInputChannel(client->GetClient());
}

TArray<FString> URigdocksSilver_Device::AZ_GetInputChannelList(URadnodzClient* client)
{
	std::vector<std::string> objArray = SILVER::AZ_GetInputChannelList(client->GetClient());

	TArray<FString> result;

	for (auto obj : objArray)
	{
		FString str = UTF8_TO_TCHAR(obj.c_str());

		result.Add(str);
	}

	return result;
}

int URigdocksSilver_Device::AZ_CountOutputChannel(URadnodzClient* client)
{
	return SILVER::AZ_CountOutputChannel(client->GetClient());
}

TArray<FString> URigdocksSilver_Device::AZ_GetOutputChannelList(URadnodzClient* client)
{
	std::vector<std::string> objArray = SILVER::AZ_GetOutputChannelList(client->GetClient());

	TArray<FString> result;

	for (auto obj : objArray)
	{
		FString str = UTF8_TO_TCHAR(obj.c_str());

		result.Add(str);
	}

	return result;
}
