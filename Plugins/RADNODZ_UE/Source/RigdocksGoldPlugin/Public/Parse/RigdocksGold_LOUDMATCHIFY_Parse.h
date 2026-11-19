// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "GoldClasses.h"
#include "RigdocksGold_LOUDMATCHIFY_Parse.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_LOUDMATCHIFY_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY|Parse")
	static void AZ_LOUDMATCHIFY_SetVolumeForMediaItem_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY|Parse")
	static void AZ_LOUDMATCHIFY_SetVolumeForMediaId_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY|Parse")
	static void AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY|Parse")
	static void AZ_LOUDMATCHIFY_SetVolumeForMediaItemList_Parse(URigdocksRpcCall* call);
};
