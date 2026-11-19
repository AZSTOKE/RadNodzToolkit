// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "RigdocksSilver_RenderTake_Parse.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_RenderTake_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|RenderTake|Parse")
	static void AZ_RenderMediaItemActiveTake_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|RenderTake|Parse")
	static void AZ_RenderMediaIdActiveTake_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|RenderTake|Parse")
	static void AZ_RenderSelectedMediaIdActiveTake_Parse(URigdocksRpcCall* call);
};
