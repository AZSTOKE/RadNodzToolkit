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
#include "RigdocksSilver_Random.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Random : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Random",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetRandomNumInteger(URadnodzClient* client, int low, int high);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Random",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetRandomNumDouble(URadnodzClient* client, double low, double high, int doubleType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Random",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetRandomColor(URadnodzClient* client, int& color, int& R, int& G, int& B);

};
