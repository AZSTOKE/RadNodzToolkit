// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Device_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

TArray<FString> URigdocksSilver_Device_Parse::AZ_GetLocalIpAddresse_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FString>();
	}

	std::vector<std::string> objArray = SILVER::AZ_GetLocalIpAddresse_Parse(call->Get());

	TArray<FString> result;
	for (auto obj : objArray)
	{
		result.Add(UTF8_TO_TCHAR(obj.c_str()));
	}

	return result;
}

FString URigdocksSilver_Device_Parse::AZ_GetDeviceName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string nameStr = SILVER::AZ_GetDeviceName_Parse(call->Get());
	return UTF8_TO_TCHAR(nameStr.c_str());
}

int URigdocksSilver_Device_Parse::AZ_CountInputChannel_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_CountInputChannel_Parse(call->Get());
}

TArray<FString> URigdocksSilver_Device_Parse::AZ_GetInputChannelList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FString>();
	}

	std::vector<std::string> objArray = SILVER::AZ_GetInputChannelList_Parse(call->Get());

	TArray<FString> result;
	for (auto obj : objArray)
	{
		result.Add(UTF8_TO_TCHAR(obj.c_str()));
	}

	return result;
}

int URigdocksSilver_Device_Parse::AZ_CountOutputChannel_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_CountOutputChannel_Parse(call->Get());
}

TArray<FString> URigdocksSilver_Device_Parse::AZ_GetOutputChannelList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FString>();
	}

	std::vector<std::string> objArray = SILVER::AZ_GetOutputChannelList_Parse(call->Get());

	TArray<FString> result;
	for (auto obj : objArray)
	{
		result.Add(UTF8_TO_TCHAR(obj.c_str()));
	}

	return result;
}
