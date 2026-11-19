// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "define.h"
#include <memory>
#include <vector>
#include <map>

namespace RADNODZ_client::Object
{
	class WwiseObject
	{
	public:

		std::string ID;

		std::string Name;

		std::string Path;

		std::map<std::string, VarType> Properties;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<WwiseObject>& objectArray, int indent = 4);

		static std::unique_ptr<WwiseObject> FromJson(std::string jsonStr);

		static std::vector<WwiseObject> FromJsonArray(std::string jsonStr);

	};
}
