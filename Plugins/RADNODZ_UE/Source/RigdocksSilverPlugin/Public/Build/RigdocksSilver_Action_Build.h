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
#include "RigdocksSilver_Action_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Action_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Action|Build")
	static URigdocksRpcCall* AZ_ActionMediaItemSelectKeep_Build(UMediaItem* item, int cmdID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Action|Build")
	static URigdocksRpcCall* AZ_ActionMediaIdSelectKeep_Build(UReaProject* projID, int itemID, int cmdID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Action|Build")
	static URigdocksRpcCall* AZ_ActionSelectedMediaIdSelectKeep_Build(UReaProject* projID, int selectItemID, int cmdID);
};
