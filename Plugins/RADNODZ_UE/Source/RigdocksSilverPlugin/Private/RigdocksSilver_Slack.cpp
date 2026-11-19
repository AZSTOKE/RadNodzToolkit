// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Slack.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSlackMessage.h"

using namespace RADNODZ_client::Object;

FSlackMessage URigdocksSilver_Slack::AZ_Slack_PostChannelMessage(URadnodzClient* client, FString token, FString channel, FString message, TArray<FString> mentionList)
{
	std::vector<std::string> mentionVec;
	for (const auto& mention : mentionList) {
		mentionVec.push_back(TCHAR_TO_UTF8(*mention));
	}

	SlackMessage slackMsg = SILVER::AZ_Slack_PostChannelMessage(client->GetClient(), TCHAR_TO_UTF8(*token), TCHAR_TO_UTF8(*channel), TCHAR_TO_UTF8(*message), mentionVec);

	FSlackMessage slackMsgObject;
	slackMsgObject.Initialize(slackMsg);

	return slackMsgObject;
}

FSlackMessage URigdocksSilver_Slack::AZ_Slack_PostDirectMessage(URadnodzClient* client, FString token, FString toUser, FString message, bool useMension)
{
	SlackMessage slackMsg = SILVER::AZ_Slack_PostDirectMessage(client->GetClient(), TCHAR_TO_UTF8(*token), TCHAR_TO_UTF8(*toUser), TCHAR_TO_UTF8(*message), useMension);

	FSlackMessage slackMsgObject;
	slackMsgObject.Initialize(slackMsg);

	return slackMsgObject;
}

FSlackMessage URigdocksSilver_Slack::AZ_Slack_ReplayMessage(URadnodzClient* client, FString token, FString channel, FString threadTimeStamp, FString message, TArray<FString> mentionList)
{
	std::vector<std::string> mentionVec;
	for (const auto& mention : mentionList)
	{
		mentionVec.push_back(TCHAR_TO_UTF8(*mention));
	}

	SlackMessage slackMsg = SILVER::AZ_Slack_ReplayMessage(client->GetClient(), TCHAR_TO_UTF8(*token), TCHAR_TO_UTF8(*channel), TCHAR_TO_UTF8(*threadTimeStamp), TCHAR_TO_UTF8(*message), mentionVec);

	FSlackMessage slackMsgObject;
	slackMsgObject.Initialize(slackMsg);

	return slackMsgObject;
}