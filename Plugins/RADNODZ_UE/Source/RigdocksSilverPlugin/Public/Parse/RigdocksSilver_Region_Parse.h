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
#include "DataClass/FMarker.h"
#include "RigdocksSilver_Region_Parse.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Region_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static int AZ_GetRegionMarkerIndex_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static void AZ_GetRegionMarkerPosition_Parse(URigdocksRpcCall* call, double& startPosOut, double& endPosOut);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static FString AZ_GetRegionMarkerName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static int AZ_GetRegionMarkerColor_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static void AZ_SetRegionMarkerPosition_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static void AZ_SetRegionMarkerColor_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static void AZ_SetRegionMarkerName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static int AZ_AddRegionMarker_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static void AZ_DeleteAllRegion_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static FMarker AZ_GetRegionObject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static TArray<FMarker> AZ_GetRegionMarkerListMediaItem_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Parse")
	static FMarker AZ_AddRegionMarkerMediaItem_Parse(URigdocksRpcCall* call);
};
