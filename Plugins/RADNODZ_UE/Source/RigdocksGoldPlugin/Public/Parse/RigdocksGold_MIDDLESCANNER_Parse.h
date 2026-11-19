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
#include "DataClass/FMiddlewareInfo.h"
#include "RigdocksGold_MIDDLESCANNER_Parse.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_MIDDLESCANNER_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER|Parse")
	static FMiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER|Parse")
	static FMiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER|Parse")
	static FMiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER|Parse")
	static TArray<FMiddlewareInfo> AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList_Parse(URigdocksRpcCall* call);
};
