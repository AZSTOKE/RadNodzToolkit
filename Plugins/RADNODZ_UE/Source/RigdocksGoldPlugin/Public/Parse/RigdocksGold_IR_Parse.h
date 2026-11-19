// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "RigdocksGold_IR_Parse.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_IR_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR|Parse")
	static void AZ_GenerateLinearTSP_Parse(URigdocksRpcCall* call, double& duration, FString& wavPath, FString& tspInfoPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR|Parse")
	static void AZ_GenerateLogTSP_Parse(URigdocksRpcCall* call, double& duration, FString& wavPath, FString& tspInfoPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR|Parse")
	static void AZ_CalcImpulseResponseFromLinearTSP_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|IR|Parse")
	static void AZ_CalcImpulseResponseFromLogTSP_Parse(URigdocksRpcCall* call);
};
