// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FPeak.h"
#include "RigdocksSilver_Peak.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Peak : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Peak",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FPeak> AZ_GetMediaItemPeakList(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Peak",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FPeak> AZ_GetMediaIdPeakList(URadnodzClient* client, UReaProject* project, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Peak",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FPeak> AZ_GetSelectedMediaIdPeakList(URadnodzClient* client, UReaProject* project, int selectItemID);

};
