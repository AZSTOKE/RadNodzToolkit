// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Slack_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FSlackMessage.h"

using namespace RADNODZ_client::Object;

FSlackMessage URigdocksSilver_Slack_Parse::AZ_Slack_PostChannelMessage_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FSlackMessage();
	}

	SlackMessage slackMsg = SILVER::AZ_Slack_PostChannelMessage_Parse(call->Get());

	FSlackMessage slackMsgObject;
	slackMsgObject.Initialize(slackMsg);

	return slackMsgObject;
}

FSlackMessage URigdocksSilver_Slack_Parse::AZ_Slack_PostDirectMessage_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FSlackMessage();
	}

	SlackMessage slackMsg = SILVER::AZ_Slack_PostDirectMessage_Parse(call->Get());

	FSlackMessage slackMsgObject;
	slackMsgObject.Initialize(slackMsg);

	return slackMsgObject;
}

FSlackMessage URigdocksSilver_Slack_Parse::AZ_Slack_ReplayMessage_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FSlackMessage();
	}

	SlackMessage slackMsg = SILVER::AZ_Slack_ReplayMessage_Parse(call->Get());

	FSlackMessage slackMsgObject;
	slackMsgObject.Initialize(slackMsg);

	return slackMsgObject;
}
