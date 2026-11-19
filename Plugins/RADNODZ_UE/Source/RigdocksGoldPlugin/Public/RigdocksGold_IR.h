// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "DataClass/FTSPRecordingGroup.h"
#include "RigdocksGold_IR.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_IR : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_sampleRate = "48000",
			CPP_Default_sweepDuration = "5.0",
			CPP_Default_silenceDuration = "5.0",
			CPP_Default_repeatCount = "1"
			))
	static 	void AZ_GenerateLinearTSP(URadnodzClient* client, FString outputPath, int sampleRate, double sweepDuration, double silenceDuration, int repeatCount, double& duration, FString& wavPath, FString& tspInfoPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_sampleRate = "48000",
			CPP_Default_sweepDuration = "10.0",
			CPP_Default_silenceDuration = "5.0",
			CPP_Default_repeatCount = "1",
			CPP_Default_startFreq = "20.0",
			CPP_Default_endFreq = "0.0"
			))
	static 	void AZ_GenerateLogTSP(URadnodzClient* client, FString outputPath, int sampleRate, double sweepDuration, double silenceDuration, int repeatCount, double startFreq, double endFreq, double& duration, FString& wavPath, FString& tspInfoPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_irLength = "0.0",
			CPP_Default_trimStart = "true",
			CPP_Default_normalize = "true"
			))
	static 	void AZ_CalcImpulseResponseFromLinearTSP(URadnodzClient* client, FString tspInfoPath, TArray<FTSPRecordingGroup> recordingGroups, FString outputPath, double irLength, bool trimStart, bool normalize);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_irLength = "0.0",
			CPP_Default_trimStart = "true",
			CPP_Default_normalize = "true"
			))
	static 	void AZ_CalcImpulseResponseFromLogTSP(URadnodzClient* client, FString tspInfoPath, TArray<FTSPRecordingGroup> recordingGroups, FString outputPath, double irLength, bool trimStart, bool normalize);

};
