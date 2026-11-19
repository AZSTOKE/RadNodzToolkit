// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "RigdocksSilver_Random_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Random_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Random|Build")
	static URigdocksRpcCall* AZ_GetRandomNumInteger_Build(int low, int high);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Random|Build")
	static URigdocksRpcCall* AZ_GetRandomNumDouble_Build(double low, double high, int doubleType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Random|Build")
	static URigdocksRpcCall* AZ_GetRandomColor_Build();
};
