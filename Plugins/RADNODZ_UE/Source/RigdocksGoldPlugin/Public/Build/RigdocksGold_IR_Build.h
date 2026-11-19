// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "DataClass/FTSPRecordingGroup.h"
#include "RigdocksGold_IR_Build.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_IR_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR|Build",
		meta = (CPP_Default_sampleRate = "48000",
			CPP_Default_sweepDuration = "5.0",
			CPP_Default_silenceDuration = "5.0",
			CPP_Default_repeatCount = "1"
			))
	static URigdocksRpcCall* AZ_GenerateLinearTSP_Build(FString outputPath, int sampleRate, double sweepDuration, double silenceDuration, int repeatCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR|Build",
		meta = (CPP_Default_sampleRate = "48000",
			CPP_Default_sweepDuration = "10.0",
			CPP_Default_silenceDuration = "5.0",
			CPP_Default_repeatCount = "1",
			CPP_Default_startFreq = "20.0",
			CPP_Default_endFreq = "0.0"
			))
	static URigdocksRpcCall* AZ_GenerateLogTSP_Build(FString outputPath, int sampleRate, double sweepDuration, double silenceDuration, int repeatCount, double startFreq, double endFreq);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR|Build",
		meta = (CPP_Default_irLength = "0.0",
			CPP_Default_trimStart = "true",
			CPP_Default_normalize = "true"
			))
	static URigdocksRpcCall* AZ_CalcImpulseResponseFromLinearTSP_Build(FString tspInfoPath, TArray<FTSPRecordingGroup> recordingGroups, FString outputPath, double irLength, bool trimStart, bool normalize);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR|Build",
		meta = (CPP_Default_irLength = "0.0",
			CPP_Default_trimStart = "true",
			CPP_Default_normalize = "true"
			))
	static URigdocksRpcCall* AZ_CalcImpulseResponseFromLogTSP_Build(FString tspInfoPath, TArray<FTSPRecordingGroup> recordingGroups, FString outputPath, double irLength, bool trimStart, bool normalize);
};
