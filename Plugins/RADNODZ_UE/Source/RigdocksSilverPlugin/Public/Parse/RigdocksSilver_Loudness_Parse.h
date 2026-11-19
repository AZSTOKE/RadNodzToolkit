// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "ReaperClasses.h"
#include "DataClass/FSection.h"
#include "RigdocksSilver_Loudness_Parse.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Loudness_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Parse")
	static void AZ_GetMediaItemLoudnessMaxMomentary_Parse(URigdocksRpcCall* call, double& max_lufs_m, double& position);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Parse")
	static void AZ_GetMediaIdLoudnessMaxMomentary_Parse(URigdocksRpcCall* call, double& max_lufs_m, double& position);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Parse")
	static void AZ_GetSelectedMediaIdLoudnessMaxMomentary_Parse(URigdocksRpcCall* call, double& max_lufs_m, double& position);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Parse")
	static double AZ_SetMediaItemLoudnessMaxMomentary_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Parse")
	static double AZ_SetMediaIdLoudnessMaxMomentary_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Parse")
	static double AZ_SetSelectedMediaIdLoudnessMaxMomentary_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Parse")
	static TArray<FSection> AZ_GetSilentSectionListInMediaItem_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Parse")
	static TArray<FSection> AZ_GetAudioSectionListInMediaItem_Parse(URigdocksRpcCall* call);
};
