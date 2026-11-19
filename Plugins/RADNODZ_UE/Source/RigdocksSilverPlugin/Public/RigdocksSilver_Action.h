// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "RigdocksSilver_Action.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Action : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Action",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_ActionMediaItemSelectKeep(URadnodzClient* client, UMediaItem* item, int cmdID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Action",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_ActionMediaIdSelectKeep(URadnodzClient* client, UReaProject* projID, int itemID, int cmdID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Action",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_ActionSelectedMediaIdSelectKeep(URadnodzClient* client, UReaProject* projID, int selectItemID, int cmdID);
};
