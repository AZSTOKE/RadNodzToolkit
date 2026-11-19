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
	class MiddlewareInfo
	{
	public:

		MediaItem* Item = nullptr;

		double Volume = 0.0;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<MiddlewareInfo>& objectArray, int indent = 4);

		static std::unique_ptr<MiddlewareInfo> FromJson(std::string jsonStr);

		static std::vector<MiddlewareInfo> FromJsonArray(std::string jsonStr);

	};
}
