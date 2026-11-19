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
#include "DataClass/FPeak.h"
#include "RigdocksSilver_Peak_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Peak_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Peak|Build")
	static URigdocksRpcCall* AZ_GetMediaItemPeakList_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Peak|Build")
	static URigdocksRpcCall* AZ_GetMediaIdPeakList_Build(UReaProject* project, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Peak|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaIdPeakList_Build(UReaProject* project, int selectItemID);
};
