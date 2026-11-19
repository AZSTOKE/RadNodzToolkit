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
#include "RigdocksSilver_Marker_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Marker_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker|Build")
	static URigdocksRpcCall* AZ_GetMarkerObject_Build(UReaProject* proj, int markerId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker|Build")
	static URigdocksRpcCall* AZ_GetMarkerListMediaItem_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker|Build")
	static URigdocksRpcCall* AZ_SetMarkerPosition_Build(UReaProject* proj, int markerId, double setStartPos);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker|Build")
	static URigdocksRpcCall* AZ_SetMarkerColor_Build(UReaProject* proj, int markerId, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker|Build")
	static URigdocksRpcCall* AZ_SetMarkerName_Build(UReaProject* proj, int markerId, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker|Build")
	static URigdocksRpcCall* AZ_AddMarker_Build(UReaProject* proj, double setStartPos, FString setName, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker|Build")
	static URigdocksRpcCall* AZ_DeleteAllMarker_Build(UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Marker|Build")
	static URigdocksRpcCall* AZ_AddMarkerMediaItem_Build(UMediaItem* item, FString markerName, int setColor);
};
