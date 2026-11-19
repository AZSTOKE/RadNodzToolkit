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
#include "RigdocksSilver_Marker.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Marker : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FMarker AZ_GetMarkerObject(URadnodzClient* client, UReaProject* proj, int markerId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FMarker> AZ_GetMarkerListMediaItem(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMarkerPosition(URadnodzClient* client, UReaProject* proj, int markerId, double setStartPos);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMarkerColor(URadnodzClient* client, UReaProject* proj, int markerId, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMarkerName(URadnodzClient* client, UReaProject* proj, int markerId, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FMarker AZ_AddMarker(URadnodzClient* client, UReaProject* proj, double setStartPos, FString setName, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteAllMarker(URadnodzClient* client, UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FMarker AZ_AddMarkerMediaItem(URadnodzClient* client, UMediaItem* item, FString markerName, int setColor);

};
