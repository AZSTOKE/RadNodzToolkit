// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksGold_IR_Build.h"
#include "RIGDOCKS_GOLD_Build.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

namespace
{
	std::vector<TSPRecordingGroup> IRBuild_ToCppGroups(const TArray<FTSPRecordingGroup>& recordingGroups)
	{
		std::vector<TSPRecordingGroup> groups;
		for (const FTSPRecordingGroup& group : recordingGroups)
		{
			groups.push_back(group.GetCppObj());
		}
		return groups;
	}
}

URigdocksRpcCall* URigdocksGold_IR_Build::AZ_GenerateLinearTSP_Build(FString outputPath, int sampleRate, double sweepDuration, double silenceDuration, int repeatCount)
{
	RpcCall c = GOLD::AZ_GenerateLinearTSP_Build(TCHAR_TO_UTF8(*outputPath), sampleRate, sweepDuration, silenceDuration, repeatCount);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_IR_Build::AZ_GenerateLogTSP_Build(FString outputPath, int sampleRate, double sweepDuration, double silenceDuration, int repeatCount, double startFreq, double endFreq)
{
	RpcCall c = GOLD::AZ_GenerateLogTSP_Build(TCHAR_TO_UTF8(*outputPath), sampleRate, sweepDuration, silenceDuration, repeatCount, startFreq, endFreq);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_IR_Build::AZ_CalcImpulseResponseFromLinearTSP_Build(FString tspInfoPath, TArray<FTSPRecordingGroup> recordingGroups, FString outputPath, double irLength, bool trimStart, bool normalize)
{
	RpcCall c = GOLD::AZ_CalcImpulseResponseFromLinearTSP_Build(TCHAR_TO_UTF8(*tspInfoPath), IRBuild_ToCppGroups(recordingGroups), TCHAR_TO_UTF8(*outputPath), irLength, trimStart, normalize);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_IR_Build::AZ_CalcImpulseResponseFromLogTSP_Build(FString tspInfoPath, TArray<FTSPRecordingGroup> recordingGroups, FString outputPath, double irLength, bool trimStart, bool normalize)
{
	RpcCall c = GOLD::AZ_CalcImpulseResponseFromLogTSP_Build(TCHAR_TO_UTF8(*tspInfoPath), IRBuild_ToCppGroups(recordingGroups), TCHAR_TO_UTF8(*outputPath), irLength, trimStart, normalize);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
