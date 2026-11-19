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
	class TrackDetail
	{
	public:

		std::string Name;

		MediaTrack* Track;

		double Volume;

		int Index;

		int Depth;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<TrackDetail>& objectArray, int indent = 4);

		static std::unique_ptr<TrackDetail> FromJson(std::string jsonStr);

		static std::vector<TrackDetail> FromJsonArray(std::string jsonStr);

	};
}
