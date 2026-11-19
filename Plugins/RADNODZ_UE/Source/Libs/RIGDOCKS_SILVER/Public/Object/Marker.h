// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include <string>
#include <memory>
#include <vector>

namespace RADNODZ_client::Object
{
	class Marker
	{
	public:

		int Index;

		int Number;

		double StartTime;

		double EndTime;

		int Color;

		std::string Name;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<Marker>& objectArray, int indent = 4);

		static std::unique_ptr<Marker> FromJson(std::string jsonStr);

		static std::vector<Marker> FromJsonArray(std::string jsonStr);

	};
}
