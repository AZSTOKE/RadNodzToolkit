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
#include "RigdocksSilver_Region_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Region_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_GetRegionMarkerIndex_Build(UReaProject* projID, int regionId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_GetRegionMarkerPosition_Build(UReaProject* projID, int regionId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_GetRegionMarkerName_Build(UReaProject* projID, int regionId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_GetRegionMarkerColor_Build(UReaProject* projID, int regionId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_SetRegionMarkerPosition_Build(UReaProject* projID, int regionId, double setStartPos, double setEndPos);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_SetRegionMarkerColor_Build(UReaProject* projID, int regionId, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_SetRegionMarkerName_Build(UReaProject* projID, int regionId, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_AddRegionMarker_Build(UReaProject* projID, double setStartPos, double setEndPos, FString setName, int setRegionIndex, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_DeleteAllRegion_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_GetRegionObject_Build(UReaProject* proj, int regionId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_GetRegionMarkerListMediaItem_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region|Build")
	static URigdocksRpcCall* AZ_AddRegionMarkerMediaItem_Build(UMediaItem* item, FString regionName, int setColor);
};
