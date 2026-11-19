// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FMarker.h"
#include "RigdocksSilver_Region.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Region : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetRegionMarkerIndex(URadnodzClient* client, UReaProject* projID, int regionId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetRegionMarkerPosition(URadnodzClient* client, UReaProject* projID, int regionId, double& startPosOut, double& endPosOut);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetRegionMarkerName(URadnodzClient* client, UReaProject* projID, int regionId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetRegionMarkerColor(URadnodzClient* client, UReaProject* projID, int regionId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetRegionMarkerPosition(URadnodzClient* client, UReaProject* projID, int regionId, double setStartPos, double setEndPos);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetRegionMarkerColor(URadnodzClient* client, UReaProject* projID, int regionId, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetRegionMarkerName(URadnodzClient* client, UReaProject* projID, int regionId, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_AddRegionMarker(URadnodzClient* client, UReaProject* projID, double setStartPos, double setEndPos, FString setName, int setRegionIndex, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteAllRegion(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FMarker AZ_GetRegionObject(URadnodzClient* client, UReaProject* proj, int regionId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FMarker> AZ_GetRegionMarkerListMediaItem(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Region",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FMarker AZ_AddRegionMarkerMediaItem(URadnodzClient* client, UMediaItem* item, FString regionName, int setColor);

};
