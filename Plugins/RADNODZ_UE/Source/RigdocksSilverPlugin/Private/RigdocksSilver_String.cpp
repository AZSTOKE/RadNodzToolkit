// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_String.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

bool URigdocksSilver_String::AZ_CheckIncludeString(URadnodzClient* client, FString textString, FString checkString)
{
    return SILVER::AZ_CheckIncludeString(client->GetClient(), TCHAR_TO_UTF8(*textString), TCHAR_TO_UTF8(*checkString));
}

bool URigdocksSilver_String::AZ_CheckSameString(URadnodzClient* client, FString textString, FString checkString)
{
    return SILVER::AZ_CheckSameString(client->GetClient(), TCHAR_TO_UTF8(*textString), TCHAR_TO_UTF8(*checkString));
}

bool URigdocksSilver_String::AZ_CheckIncludePerfectString(URadnodzClient* client, FString textString, FString checkString)
{
    return SILVER::AZ_CheckIncludePerfectString(client->GetClient(), TCHAR_TO_UTF8(*textString), TCHAR_TO_UTF8(*checkString));
}

FString URigdocksSilver_String::AZ_ReplaceSearchString(URadnodzClient* client, FString textString, FString searchString, FString replaceString)
{
    std::string text = SILVER::AZ_ReplaceSearchString(client->GetClient(), TCHAR_TO_UTF8(*textString), TCHAR_TO_UTF8(*searchString), TCHAR_TO_UTF8(*replaceString));

	return UTF8_TO_TCHAR(text.c_str());
}

FString URigdocksSilver_String::AZ_DeleteBlankString(URadnodzClient* client, FString textString)
{
    std::string text = SILVER::AZ_DeleteBlankString(client->GetClient(), TCHAR_TO_UTF8(*textString));

    return UTF8_TO_TCHAR(text.c_str());
}

FString URigdocksSilver_String::AZ_FormatNumDigit(URadnodzClient* client, int num, int digit)
{
    std::string text = SILVER::AZ_FormatNumDigit(client->GetClient(), num, digit);

    return UTF8_TO_TCHAR(text.c_str());
}

TArray<FString> URigdocksSilver_String::AZ_GetStringSplitList(URadnodzClient* client, FString baseStr, FString splitStr)
{
    std::vector<std::string> textList = SILVER::AZ_GetStringSplitList(client->GetClient(), TCHAR_TO_UTF8(*baseStr), TCHAR_TO_UTF8(*splitStr));

	TArray<FString> result;
	for (const auto& text : textList)
	{
		result.Add(UTF8_TO_TCHAR(text.c_str()));
	}

    return result;
}

FString URigdocksSilver_String::AZ_JoinArray(URadnodzClient* client, TArray<FString> stringArray, FString separator)
{
	std::vector<std::string> textList;
	for (const auto& str : stringArray)
	{
		textList.push_back(TCHAR_TO_UTF8(*str));
	}

    std::string text = SILVER::AZ_JoinArray(client->GetClient(), textList, TCHAR_TO_UTF8(*separator));

    return UTF8_TO_TCHAR(text.c_str());
}

FString URigdocksSilver_String::AZ_NormalizeText(URadnodzClient* client, FString text)
{
    std::string textStr = SILVER::AZ_NormalizeText(client->GetClient(), TCHAR_TO_UTF8(*text));

    return UTF8_TO_TCHAR(textStr.c_str());
}

int URigdocksSilver_String::AZ_CalcStringSimilarity(URadnodzClient* client, FString targetText, FString sourceText)
{
    return SILVER::AZ_CalcStringSimilarity(client->GetClient(), TCHAR_TO_UTF8(*targetText), TCHAR_TO_UTF8(*sourceText));
}

int URigdocksSilver_String::AZ_CalcStringPartialSimilarity(URadnodzClient* client, FString targetText, FString sourceText)
{
    return SILVER::AZ_CalcStringPartialSimilarity(client->GetClient(), TCHAR_TO_UTF8(*targetText), TCHAR_TO_UTF8(*sourceText));
}

