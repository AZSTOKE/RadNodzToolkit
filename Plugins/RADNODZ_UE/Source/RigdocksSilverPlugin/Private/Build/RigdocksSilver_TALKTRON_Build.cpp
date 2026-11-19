// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_TALKTRON_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksSilver_TALKTRON_Build::AZ_TALKTRON_InsertMediaItems_Build(FString subjectLangPath, FString baseLangPath, double startPos, double mediaInterval, int startEndType, bool isVolMatch, bool isHANDAUTOMER, int HANDAUTOMER_ratioType, TArray<FString> trackNameList)
{
	std::vector<std::string> textList;
	for (const auto& str : trackNameList)
	{
		textList.push_back(TCHAR_TO_UTF8(*str));
	}

	RpcCall c = SILVER::AZ_TALKTRON_InsertMediaItems_Build(TCHAR_TO_UTF8(*subjectLangPath), TCHAR_TO_UTF8(*baseLangPath), startPos, mediaInterval, startEndType, isVolMatch, isHANDAUTOMER, HANDAUTOMER_ratioType, textList);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
