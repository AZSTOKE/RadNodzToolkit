// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksGold_IR.h"
#include "RIGDOCKS_GOLD.h"

using namespace RADNODZ_client::Object;

namespace
{
	std::vector<TSPRecordingGroup> ToCppGroups(const TArray<FTSPRecordingGroup>& recordingGroups)
	{
		std::vector<TSPRecordingGroup> groups;
		for (const FTSPRecordingGroup& group : recordingGroups)
		{
			groups.push_back(group.GetCppObj());
		}
		return groups;
	}
}

void URigdocksGold_IR::AZ_GenerateLinearTSP(URadnodzClient* client, FString outputPath, int sampleRate, double sweepDuration, double silenceDuration, int repeatCount, double& duration, FString& wavPath, FString& tspInfoPath)
{
	auto [durationSec, signalPath, infoPath] = GOLD::AZ_GenerateLinearTSP(client->GetClient(), TCHAR_TO_UTF8(*outputPath), sampleRate, sweepDuration, silenceDuration, repeatCount);

	duration = durationSec;
	wavPath = UTF8_TO_TCHAR(signalPath.c_str());
	tspInfoPath = UTF8_TO_TCHAR(infoPath.c_str());
}

void URigdocksGold_IR::AZ_GenerateLogTSP(URadnodzClient* client, FString outputPath, int sampleRate, double sweepDuration, double silenceDuration, int repeatCount, double startFreq, double endFreq, double& duration, FString& wavPath, FString& tspInfoPath)
{
	auto [durationSec, signalPath, infoPath] = GOLD::AZ_GenerateLogTSP(client->GetClient(), TCHAR_TO_UTF8(*outputPath), sampleRate, sweepDuration, silenceDuration, repeatCount, startFreq, endFreq);

	duration = durationSec;
	wavPath = UTF8_TO_TCHAR(signalPath.c_str());
	tspInfoPath = UTF8_TO_TCHAR(infoPath.c_str());
}

void URigdocksGold_IR::AZ_CalcImpulseResponseFromLinearTSP(URadnodzClient* client, FString tspInfoPath, TArray<FTSPRecordingGroup> recordingGroups, FString outputPath, double irLength, bool trimStart, bool normalize)
{
	GOLD::AZ_CalcImpulseResponseFromLinearTSP(client->GetClient(), TCHAR_TO_UTF8(*tspInfoPath), ToCppGroups(recordingGroups), TCHAR_TO_UTF8(*outputPath), irLength, trimStart, normalize);
}

void URigdocksGold_IR::AZ_CalcImpulseResponseFromLogTSP(URadnodzClient* client, FString tspInfoPath, TArray<FTSPRecordingGroup> recordingGroups, FString outputPath, double irLength, bool trimStart, bool normalize)
{
	GOLD::AZ_CalcImpulseResponseFromLogTSP(client->GetClient(), TCHAR_TO_UTF8(*tspInfoPath), ToCppGroups(recordingGroups), TCHAR_TO_UTF8(*outputPath), irLength, trimStart, normalize);
}
