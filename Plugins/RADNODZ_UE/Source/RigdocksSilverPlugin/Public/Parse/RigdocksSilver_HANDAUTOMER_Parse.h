// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "RigdocksSilver_HANDAUTOMER_Parse.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_HANDAUTOMER_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER|Parse")
	static void AZ_SetMediaItemHANDAUTOMER_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER|Parse")
	static void AZ_SetMediaIdHANDAUTOMER_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER|Parse")
	static void AZ_SetSelectedMediaIdHANDAUTOMER_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER|Parse")
	static void AZ_SetMediaItemListHANDAUTOMER_Parse(URigdocksRpcCall* call);
};
