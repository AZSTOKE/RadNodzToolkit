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
#include "RigdocksSilver_Loudness_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Loudness_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Build")
	static URigdocksRpcCall* AZ_GetMediaItemLoudnessMaxMomentary_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Build")
	static URigdocksRpcCall* AZ_GetMediaIdLoudnessMaxMomentary_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaIdLoudnessMaxMomentary_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Build")
	static URigdocksRpcCall* AZ_SetMediaItemLoudnessMaxMomentary_Build(UMediaItem* item, double setMaxMomentary);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Build")
	static URigdocksRpcCall* AZ_SetMediaIdLoudnessMaxMomentary_Build(UReaProject* proj, int itemID, double setMaxMomentary);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaIdLoudnessMaxMomentary_Build(UReaProject* proj, int selectItemID, double setMaxMomentary);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Build",
		meta = (CPP_Default_minSilentDulation = "0.5", CPP_Default_threshold = "-60"))
	static URigdocksRpcCall* AZ_GetSilentSectionListInMediaItem_Build(UMediaItem* item, double minSilentDulation, double threshold);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Loudness|Build",
		meta = (CPP_Default_maxSilentDulation = "0.5", CPP_Default_threshold = "-60"))
	static URigdocksRpcCall* AZ_GetAudioSectionListInMediaItem_Build(UMediaItem* item, double maxSilentDulation, double threshold);
};
