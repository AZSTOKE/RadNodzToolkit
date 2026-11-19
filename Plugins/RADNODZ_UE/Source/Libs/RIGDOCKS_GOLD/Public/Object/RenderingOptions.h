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
	class RenderingOptions
	{
	public:

		std::string OutputDir;

		std::string FileName;

		int RenderingTarget;

		int RenderingRangeType;

		double StartPos;

		double EndPos;

		double SamplingRate;

		int Channels;

		double TailMs;

		bool IsTail;

		std::string Format;

		std::string Format2;

		int RenderingSetting;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<RenderingOptions>& objectArray, int indent = 4);

		static std::unique_ptr<RenderingOptions> FromJson(std::string jsonStr);

		static std::vector<RenderingOptions> FromJsonArray(std::string jsonStr);

	};
}
