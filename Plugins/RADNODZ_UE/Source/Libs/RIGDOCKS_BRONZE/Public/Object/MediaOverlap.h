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
	class MediaOverlap
	{
	public:

		MediaItem* Item;

		std::string Name;

		double StartTime;

		double EndTime;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<MediaOverlap>& objectArray, int indent = 4);

		static std::unique_ptr<MediaOverlap> FromJson(std::string jsonStr);

		static std::vector<MediaOverlap> FromJsonArray(std::string jsonStr);

	};
}
