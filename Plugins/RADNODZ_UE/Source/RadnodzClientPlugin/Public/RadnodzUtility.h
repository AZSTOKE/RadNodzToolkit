// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzUtility.generated.h"

UCLASS(BlueprintType)
class RADNODZCLIENTPLUGIN_API URadnodzUtility : public UObject
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Utility")
	static FString AZ_GetLocalIP();
};