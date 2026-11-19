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
	class WhisperSegment
	{
	public:

		double StartTime;

		double EndTime;

		std::string Text;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<WhisperSegment>& objectArray, int indent = 4);

		static std::unique_ptr<WhisperSegment> FromJson(std::string jsonStr);

		static std::vector<WhisperSegment> FromJsonArray(std::string jsonStr);

	};
}
