// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "define.h"
#include <memory>
#include <string>
#include <vector>

namespace RADNODZ_client::Object
{
	// Recording group for the IR APIs (AZ_CalcImpulseResponseFrom*TSP).
	//   - FilePaths          : takes recorded under the same condition (averaged per channel)
	//   - ReferenceFilePaths : loopback recording (mono) of each take, same order as FilePaths.
	//                          Empty = no latency correction.
	//   - AutoAlign          : align each take / repeat to the first take by its direct sound before
	//                          averaging (default true). Ignored when ReferenceFilePaths is given.
	// Groups are concatenated channel-wise in the order they are passed.
	class TSPRecordingGroup
	{
	public:

		std::vector<std::string> FilePaths;

		std::vector<std::string> ReferenceFilePaths;

		bool AutoAlign = true;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<TSPRecordingGroup>& objectArray, int indent = 4);

		static std::unique_ptr<TSPRecordingGroup> FromJson(std::string jsonStr);

		static std::vector<TSPRecordingGroup> FromJsonArray(std::string jsonStr);

	};
}
