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
#include "RigdocksSilver_RenderTake_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_RenderTake_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|RenderTake|Build")
	static URigdocksRpcCall* AZ_RenderMediaItemActiveTake_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|RenderTake|Build")
	static URigdocksRpcCall* AZ_RenderMediaIdActiveTake_Build(UReaProject* projID, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|RenderTake|Build")
	static URigdocksRpcCall* AZ_RenderSelectedMediaIdActiveTake_Build(UReaProject* projID, int selectItemID);
};
