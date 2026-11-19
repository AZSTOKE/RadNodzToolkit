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
	class WwiseState
	{
	public:

		std::string ID;

		std::string Name;

		std::string CustomStateName;

		std::string CustomStateID;

		std::string ShortID;

		bool BypassEffect;

		bool BypassEffect0;

		bool BypassEffect1;

		bool BypassEffect2;

		bool BypassEffect3;

		int16_t GameAuxSendHPF;

		int16_t GameAuxSendLPF;

		double GameAuxSendVolume;

		int16_t Highpass;

		double InitialDelay;

		int16_t Lowpass;

		double MakeUpGain;

		int MidiTransposition;

		int MidiVelocityOffset;

		int16_t OutputBusHighpass;

		int16_t OutputBusLowpass;

		double OutputBusVolume;

		int Pitch;

		int16_t Priority;

		double ReflectionsVolume;

		int16_t UserAuxSendHPF0;

		int16_t UserAuxSendHPF1;

		int16_t UserAuxSendHPF2;

		int16_t UserAuxSendHPF3;

		int16_t UserAuxSendLPF0;

		int16_t UserAuxSendLPF1;

		int16_t UserAuxSendLPF2;

		int16_t UserAuxSendLPF3;

		double UserAuxSendVolume0;

		double UserAuxSendVolume1;

		double UserAuxSendVolume2;

		double UserAuxSendVolume3;

		double Volume;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<WwiseState>& objectArray, int indent = 4);

		static std::unique_ptr<WwiseState> FromJson(std::string jsonStr);

		static std::vector<WwiseState> FromJsonArray(std::string jsonStr);
	};

	class WwiseStateGroup
	{
	public:

		std::string ID;

		std::string Name;

		std::vector<WwiseState> State;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<WwiseStateGroup>& objectArray, int indent = 4);

		static std::unique_ptr<WwiseStateGroup> FromJson(std::string jsonStr);

		static std::vector<WwiseStateGroup> FromJsonArray(std::string jsonStr);

	};
}
