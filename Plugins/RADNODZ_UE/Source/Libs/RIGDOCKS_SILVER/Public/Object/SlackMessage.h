// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "define.h"
#include <memory>
#include <vector>

namespace RADNODZ_client::Object
{
	class SlackMessage
	{
	public:

		bool Result;

		std::string Channel;

		std::string TimeStamp;

		std::string ThreadTimeStamp;

		std::string Message;

		std::string Error;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<SlackMessage>& objectArray, int indent = 4);

		static std::unique_ptr<SlackMessage> FromJson(std::string jsonStr);

		static std::vector<SlackMessage> FromJsonArray(std::string jsonStr);

	};
}
