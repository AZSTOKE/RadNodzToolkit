// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "GoldClasses.h"
#include "DataClass/FMiddlewareInfo.h"
#include "DataClass/FMIDDLESCANNER_Options.h"
#include "RigdocksGold_MIDDLESCANNER.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_MIDDLESCANNER : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FMiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem(URadnodzClient* client, UMediaItem* item, FString language, FString parentID, FMIDDLESCANNER_Options options);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FMiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId(URadnodzClient* client, UReaProject* proj, int itemID, FString language, FString parentID, FMIDDLESCANNER_Options options);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FMiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId(URadnodzClient* client, UReaProject* proj, int selectedItemID, FString language, FString parentID, FMIDDLESCANNER_Options options);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|MIDDLESCANNER",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FMiddlewareInfo> AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList(URadnodzClient* client, TArray<UMediaItem*> itemList, FString language, FString parentID, FMIDDLESCANNER_Options options);

};
