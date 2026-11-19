// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_String_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include <string>
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksSilver_String_Build::AZ_CheckIncludeString_Build(FString textString, FString checkString)
{
	RpcCall c = SILVER::AZ_CheckIncludeString_Build(TCHAR_TO_UTF8(*textString), TCHAR_TO_UTF8(*checkString));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_String_Build::AZ_CheckSameString_Build(FString textString, FString checkString)
{
	RpcCall c = SILVER::AZ_CheckSameString_Build(TCHAR_TO_UTF8(*textString), TCHAR_TO_UTF8(*checkString));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_String_Build::AZ_CheckIncludePerfectString_Build(FString textString, FString checkString)
{
	RpcCall c = SILVER::AZ_CheckIncludePerfectString_Build(TCHAR_TO_UTF8(*textString), TCHAR_TO_UTF8(*checkString));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_String_Build::AZ_ReplaceSearchString_Build(FString textString, FString searchString, FString replaceString)
{
	RpcCall c = SILVER::AZ_ReplaceSearchString_Build(TCHAR_TO_UTF8(*textString), TCHAR_TO_UTF8(*searchString), TCHAR_TO_UTF8(*replaceString));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_String_Build::AZ_DeleteBlankString_Build(FString textString)
{
	RpcCall c = SILVER::AZ_DeleteBlankString_Build(TCHAR_TO_UTF8(*textString));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_String_Build::AZ_FormatNumDigit_Build(int num, int digit)
{
	RpcCall c = SILVER::AZ_FormatNumDigit_Build(num, digit);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_String_Build::AZ_GetStringSplitList_Build(FString baseStr, FString splitStr)
{
	RpcCall c = SILVER::AZ_GetStringSplitList_Build(TCHAR_TO_UTF8(*baseStr), TCHAR_TO_UTF8(*splitStr));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_String_Build::AZ_JoinArray_Build(TArray<FString> stringArray, FString separator)
{
	std::vector<std::string> textList;
	for (const auto& str : stringArray)
	{
		textList.push_back(TCHAR_TO_UTF8(*str));
	}

	RpcCall c = SILVER::AZ_JoinArray_Build(textList, TCHAR_TO_UTF8(*separator));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_String_Build::AZ_NormalizeText_Build(FString text)
{
	RpcCall c = SILVER::AZ_NormalizeText_Build(TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_String_Build::AZ_CalcStringSimilarity_Build(FString targetText, FString sourceText)
{
	RpcCall c = SILVER::AZ_CalcStringSimilarity_Build(TCHAR_TO_UTF8(*targetText), TCHAR_TO_UTF8(*sourceText));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_String_Build::AZ_CalcStringPartialSimilarity_Build(FString targetText, FString sourceText)
{
	RpcCall c = SILVER::AZ_CalcStringPartialSimilarity_Build(TCHAR_TO_UTF8(*targetText), TCHAR_TO_UTF8(*sourceText));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
