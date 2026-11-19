// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "define.h"
#include <vector>
#include <string>
#include <memory>

namespace RADNODZ_client::Object
{
	class BusItem
	{
	public:

		std::string Name;

		double Volume = 0.0;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<BusItem>& objectArray, int indent = 4);

		static std::unique_ptr<BusItem> FromJson(std::string jsonStr);

		static std::vector<BusItem> FromJsonArray(std::string jsonStr);

	};

	class MIDDLESCANNER_Options
	{
	public:

		std::vector<std::string> ExcludeBusList;

		std::vector<std::string> ExcludeActorList;

		std::vector<BusItem> AddBusList;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<MIDDLESCANNER_Options>& objectArray, int indent = 4);

		static std::unique_ptr<MIDDLESCANNER_Options> FromJson(std::string jsonStr);

		static std::vector<MIDDLESCANNER_Options> FromJsonArray(std::string jsonStr);

	};
}
