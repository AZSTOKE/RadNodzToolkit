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
	class Peak
	{
	public:

		double Time;

		double MaxPeak;

		double MinPeak;

		double Frequency;

		double Pos;

		double PointVol;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<Peak>& objectArray, int indent = 4);

		static std::unique_ptr<Peak> FromJson(std::string jsonStr);

		static std::vector<Peak> FromJsonArray(std::string jsonStr);

	};
}
