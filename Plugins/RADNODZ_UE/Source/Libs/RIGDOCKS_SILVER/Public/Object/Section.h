// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include <memory>
#include <vector>
#include <string>

namespace RADNODZ_client::Object
{
	class Section
	{
	public:

		double StartTime;

		double EndTime;

		double Dulation;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<Section>& objectArray, int indent = 4);

		static std::unique_ptr<Section> FromJson(std::string jsonStr);

		static std::vector<Section> FromJsonArray(std::string jsonStr);

	};
}
