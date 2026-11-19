// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_UI.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

bool URigdocksSilver_UI::AZ_ShowMessageBox_Ex(URadnodzClient* client, FString messageStr, FString windowTitleStr, AZ_WindowType windowType, AZ_ButtonID& pushedButtonId)
{
	auto[result, pushedButtonId_int] = SILVER::AZ_ShowMessageBox_Ex(client->GetClient(), TCHAR_TO_UTF8(*messageStr), TCHAR_TO_UTF8(*windowTitleStr), (int)windowType);

	pushedButtonId = (AZ_ButtonID)pushedButtonId_int;
	
	return result;
}

bool URigdocksSilver_UI::AZ_GetUserInputList(URadnodzClient* client, FString titleStr, int inputNum, FString captionStr, FString resultStr, TArray<FString>& inputList)
{
	auto[result, inputListVec] = SILVER::AZ_GetUserInputList(client->GetClient(), TCHAR_TO_UTF8(*titleStr), inputNum, TCHAR_TO_UTF8(*captionStr), TCHAR_TO_UTF8(*resultStr));

	inputList = TArray<FString>();

	for (auto input : inputListVec)
	{
		inputList.Add(UTF8_TO_TCHAR(input.c_str()));
	}

	return result;
}

bool URigdocksSilver_UI::AZ_ShowMessageBox_Info(URadnodzClient* client, FString message, FString title, AZ_WindowType windowType, AZ_ButtonID& pushedButtonId)
{
	auto [result, pushedButtonId_int] = SILVER::AZ_ShowMessageBox_Info(client->GetClient(), TCHAR_TO_UTF8(*message), TCHAR_TO_UTF8(*title), (int)windowType);

	pushedButtonId = (AZ_ButtonID)pushedButtonId_int;

	return result;
}

bool URigdocksSilver_UI::AZ_ShowMessageBox_Warn(URadnodzClient* client, FString message, FString title, AZ_WindowType windowType, AZ_ButtonID& pushedButtonId)
{
	auto [result, pushedButtonId_int] = SILVER::AZ_ShowMessageBox_Warn(client->GetClient(), TCHAR_TO_UTF8(*message), TCHAR_TO_UTF8(*title), (int)windowType);

	pushedButtonId = (AZ_ButtonID)pushedButtonId_int;

	return result;
}

bool URigdocksSilver_UI::AZ_ShowMessageBox_Error(URadnodzClient* client, FString message, FString title, AZ_WindowType windowType, AZ_ButtonID& pushedButtonId)
{
	auto [result, pushedButtonId_int] = SILVER::AZ_ShowMessageBox_Error(client->GetClient(), TCHAR_TO_UTF8(*message), TCHAR_TO_UTF8(*title), (int)windowType);

	pushedButtonId = (AZ_ButtonID)pushedButtonId_int;

	return result;
}
