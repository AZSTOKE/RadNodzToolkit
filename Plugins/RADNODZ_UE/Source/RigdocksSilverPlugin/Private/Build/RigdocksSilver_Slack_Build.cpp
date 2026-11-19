// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Slack_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FSlackMessage.h"

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksSilver_Slack_Build::AZ_Slack_PostChannelMessage_Build(FString token, FString channel, FString message, TArray<FString> mentionList)
{
	std::vector<std::string> mentionVec;
	for (const auto& mention : mentionList)
	{
		mentionVec.push_back(TCHAR_TO_UTF8(*mention));
	}

	RpcCall c = SILVER::AZ_Slack_PostChannelMessage_Build(TCHAR_TO_UTF8(*token), TCHAR_TO_UTF8(*channel), TCHAR_TO_UTF8(*message), mentionVec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Slack_Build::AZ_Slack_PostDirectMessage_Build(FString token, FString toUser, FString message, bool useMension)
{
	RpcCall c = SILVER::AZ_Slack_PostDirectMessage_Build(TCHAR_TO_UTF8(*token), TCHAR_TO_UTF8(*toUser), TCHAR_TO_UTF8(*message), useMension);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Slack_Build::AZ_Slack_ReplayMessage_Build(FString token, FString channel, FString threadTimeStamp, FString message, TArray<FString> mentionList)
{
	std::vector<std::string> mentionVec;
	for (const auto& mention : mentionList)
	{
		mentionVec.push_back(TCHAR_TO_UTF8(*mention));
	}

	RpcCall c = SILVER::AZ_Slack_ReplayMessage_Build(TCHAR_TO_UTF8(*token), TCHAR_TO_UTF8(*channel), TCHAR_TO_UTF8(*threadTimeStamp), TCHAR_TO_UTF8(*message), mentionVec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
