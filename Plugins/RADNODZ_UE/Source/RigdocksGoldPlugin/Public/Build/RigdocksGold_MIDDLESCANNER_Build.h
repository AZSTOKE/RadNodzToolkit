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
#include "GoldClasses.h"
#include "DataClass/FMIDDLESCANNER_Options.h"
#include "RigdocksGold_MIDDLESCANNER_Build.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_MIDDLESCANNER_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER|Build")
	static URigdocksRpcCall* AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem_Build(UMediaItem* item, FString language, FString parentID, FMIDDLESCANNER_Options options);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER|Build")
	static URigdocksRpcCall* AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId_Build(UReaProject* proj, int itemID, FString language, FString parentID, FMIDDLESCANNER_Options options);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER|Build")
	static URigdocksRpcCall* AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId_Build(UReaProject* proj, int selectedItemID, FString language, FString parentID, FMIDDLESCANNER_Options options);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER|Build")
	static URigdocksRpcCall* AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList_Build(TArray<UMediaItem*> itemList, FString language, FString parentID, FMIDDLESCANNER_Options options);
};
