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
	class TrackSendProperties
	{
	public:

		bool Mute;

		bool Phase;

		bool Mono;

		double Volume;

		double VolDB;

		double Pan;

		int SendMode;

		int Src_FirstChIndex;

		int Src_NumChannels;

		int Dst_FirstChIndex;

		bool Dst_MixToMono;


		// Settin Flags
		bool setMute = false;

		bool setPhase = false;

		bool setMono = false;

		bool setVolume = false;

		bool setVolDB = false;

		bool setPan = false;

		bool setSendMode = false;

		bool setSrc_FirstChIndex = false;

		bool setSrc_NumChannels = false;

		bool setDst_FirstChIndex = false;

		bool setDst_MixToMono = false;


		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<TrackSendProperties>& objectArray, int indent = 4);

		static std::unique_ptr<TrackSendProperties> FromJson(std::string jsonStr);

		static std::vector<TrackSendProperties> FromJsonArray(std::string jsonStr);

	};
}
