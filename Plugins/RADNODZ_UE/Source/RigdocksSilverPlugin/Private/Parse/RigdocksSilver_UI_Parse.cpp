// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_UI_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

bool URigdocksSilver_UI_Parse::AZ_ShowMessageBox_Ex_Parse(URigdocksRpcCall* call, AZ_ButtonID& pushedButtonId)
{
	if (call == nullptr)
	{
		return false;
	}

	auto [result, pushedButtonId_int] = SILVER::AZ_ShowMessageBox_Ex_Parse(call->Get());

	pushedButtonId = (AZ_ButtonID)pushedButtonId_int;

	return result;
}

bool URigdocksSilver_UI_Parse::AZ_GetUserInputList_Parse(URigdocksRpcCall* call, TArray<FString>& inputList)
{
	if (call == nullptr)
	{
		return false;
	}

	auto [result, inputListVec] = SILVER::AZ_GetUserInputList_Parse(call->Get());

	inputList = TArray<FString>();

	for (auto input : inputListVec)
	{
		inputList.Add(UTF8_TO_TCHAR(input.c_str()));
	}

	return result;
}

bool URigdocksSilver_UI_Parse::AZ_ShowMessageBox_Info_Parse(URigdocksRpcCall* call, AZ_ButtonID& pushedButtonId)
{
	if (call == nullptr)
	{
		return false;
	}

	auto [result, pushedButtonId_int] = SILVER::AZ_ShowMessageBox_Info_Parse(call->Get());

	pushedButtonId = (AZ_ButtonID)pushedButtonId_int;

	return result;
}

bool URigdocksSilver_UI_Parse::AZ_ShowMessageBox_Warn_Parse(URigdocksRpcCall* call, AZ_ButtonID& pushedButtonId)
{
	if (call == nullptr)
	{
		return false;
	}

	auto [result, pushedButtonId_int] = SILVER::AZ_ShowMessageBox_Warn_Parse(call->Get());

	pushedButtonId = (AZ_ButtonID)pushedButtonId_int;

	return result;
}

bool URigdocksSilver_UI_Parse::AZ_ShowMessageBox_Error_Parse(URigdocksRpcCall* call, AZ_ButtonID& pushedButtonId)
{
	if (call == nullptr)
	{
		return false;
	}

	auto [result, pushedButtonId_int] = SILVER::AZ_ShowMessageBox_Error_Parse(call->Get());

	pushedButtonId = (AZ_ButtonID)pushedButtonId_int;

	return result;
}
