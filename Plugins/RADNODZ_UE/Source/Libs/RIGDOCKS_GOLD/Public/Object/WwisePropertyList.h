// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "define.h"
#include <string>
#include <memory>
#include <vector>
#include <map>

namespace RADNODZ_client::Object
{
	class WwisePropertyList
	{
	public:

		std::string ID;

		std::string Name;

		std::string Path;

		double Volume;

		int Pitch;

		int Lowpass;

		int Highpass;

		double MakeUpGain;

		double OutputBusVolume;

		int OutputBusLowpass;

		int OutputBusHighpass;

		double InitialDelay;

		bool IsStreamingEnabled;

		bool IsNonCachable;

		bool IsZeroLatency;

		int PreFetchLength;

		double VolumeOffset;

		double BusVolume;

		std::string OutputBusID;

		std::string OutputBusName;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<WwisePropertyList>& objectArray, int indent = 4);

		static std::unique_ptr<WwisePropertyList> FromJson(std::string jsonStr);

		static std::vector<WwisePropertyList> FromJsonArray(std::string jsonStr);

	};
}
