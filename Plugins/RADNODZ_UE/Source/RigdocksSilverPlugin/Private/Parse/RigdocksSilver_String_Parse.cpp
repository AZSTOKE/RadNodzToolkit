// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_String_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include <string>
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

bool URigdocksSilver_String_Parse::AZ_CheckIncludeString_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_CheckIncludeString_Parse(call->Get());
}

bool URigdocksSilver_String_Parse::AZ_CheckSameString_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_CheckSameString_Parse(call->Get());
}

bool URigdocksSilver_String_Parse::AZ_CheckIncludePerfectString_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_CheckIncludePerfectString_Parse(call->Get());
}

FString URigdocksSilver_String_Parse::AZ_ReplaceSearchString_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string text = SILVER::AZ_ReplaceSearchString_Parse(call->Get());
	return UTF8_TO_TCHAR(text.c_str());
}

FString URigdocksSilver_String_Parse::AZ_DeleteBlankString_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string text = SILVER::AZ_DeleteBlankString_Parse(call->Get());
	return UTF8_TO_TCHAR(text.c_str());
}

FString URigdocksSilver_String_Parse::AZ_FormatNumDigit_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string text = SILVER::AZ_FormatNumDigit_Parse(call->Get());
	return UTF8_TO_TCHAR(text.c_str());
}

TArray<FString> URigdocksSilver_String_Parse::AZ_GetStringSplitList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FString>();
	}

	std::vector<std::string> textList = SILVER::AZ_GetStringSplitList_Parse(call->Get());

	TArray<FString> result;
	for (const auto& text : textList)
	{
		result.Add(UTF8_TO_TCHAR(text.c_str()));
	}

	return result;
}

FString URigdocksSilver_String_Parse::AZ_JoinArray_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string text = SILVER::AZ_JoinArray_Parse(call->Get());
	return UTF8_TO_TCHAR(text.c_str());
}

FString URigdocksSilver_String_Parse::AZ_NormalizeText_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string textStr = SILVER::AZ_NormalizeText_Parse(call->Get());
	return UTF8_TO_TCHAR(textStr.c_str());
}

int URigdocksSilver_String_Parse::AZ_CalcStringSimilarity_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_CalcStringSimilarity_Parse(call->Get());
}

int URigdocksSilver_String_Parse::AZ_CalcStringPartialSimilarity_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_CalcStringPartialSimilarity_Parse(call->Get());
}
