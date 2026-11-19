// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FSection.h"
#include "RigdocksSilver_Loudness.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Loudness : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetMediaItemLoudnessMaxMomentary(URadnodzClient* client, UMediaItem* item, double& max_lufs_m, double& position);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetMediaIdLoudnessMaxMomentary(URadnodzClient* client, UReaProject* proj, int itemID, double& max_lufs_m, double& position);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_GetSelectedMediaIdLoudnessMaxMomentary(URadnodzClient* client, UReaProject* proj, int selectItemID, double& max_lufs_m, double& position);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	double AZ_SetMediaItemLoudnessMaxMomentary(URadnodzClient* client, UMediaItem* item, double setMaxMomentary);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	double AZ_SetMediaIdLoudnessMaxMomentary(URadnodzClient* client, UReaProject* proj, int itemID, double setMaxMomentary);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	double AZ_SetSelectedMediaIdLoudnessMaxMomentary(URadnodzClient* client, UReaProject* proj, int selectItemID, double setMaxMomentary);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness",
		meta = (
			BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_minSilentDulation = "0.5",
			CPP_Default_threshold = "-60"
			))
	static  TArray<FSection> AZ_GetSilentSectionListInMediaItem(URadnodzClient* client, UMediaItem* item, double minSilentDulation, double threshold);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness",
		meta = (
			BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_minSilentDulation = "0.5",
			CPP_Default_threshold = "-60"
			))
	static  TArray<FSection> AZ_GetAudioSectionListInMediaItem(URadnodzClient* client, UMediaItem* item, double maxSilentDulation, double threshold);

};
