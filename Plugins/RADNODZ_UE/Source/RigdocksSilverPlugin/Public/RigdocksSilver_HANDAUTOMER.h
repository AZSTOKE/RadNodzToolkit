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
#include "RigdocksSilver_HANDAUTOMER.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_HANDAUTOMER : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemHANDAUTOMER(URadnodzClient* client, UMediaItem* item, int ratioType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaIdHANDAUTOMER(URadnodzClient* client, UReaProject* projID, int itemID, int ratioType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaIdHANDAUTOMER(URadnodzClient* client, UReaProject* projID, int selectItemID, int ratioType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemListHANDAUTOMER(URadnodzClient* client, TArray<UMediaItem*> itemList, int ratioType);

};
