// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_TALKTRON.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

TArray<FString> URigdocksSilver_TALKTRON::AZ_TALKTRON_InsertMediaItems(URadnodzClient* client, FString subjectLangPath, FString baseLangPath, double startPos, double mediaInterval, int startEndType, bool isVolMatch, bool isHANDAUTOMER, int HANDAUTOMER_ratioType, TArray<FString> trackNameList)
{
	std::vector<std::string> textList;
	for (const auto& str : trackNameList)
	{
		textList.push_back(TCHAR_TO_UTF8(*str));
	}

	std::vector<std::string> unmatchedList = SILVER::AZ_TALKTRON_InsertMediaItems(client->GetClient(), TCHAR_TO_UTF8(*subjectLangPath), TCHAR_TO_UTF8(*baseLangPath), startPos, mediaInterval, startEndType, isVolMatch, isHANDAUTOMER, HANDAUTOMER_ratioType, textList);

	TArray<FString> result;
	for (const auto& text : unmatchedList)
	{
		result.Add(UTF8_TO_TCHAR(text.c_str()));
	}

	return result;
}

