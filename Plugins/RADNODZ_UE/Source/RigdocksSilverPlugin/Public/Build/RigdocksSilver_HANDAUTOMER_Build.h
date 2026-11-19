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
#include "RigdocksSilver_HANDAUTOMER_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_HANDAUTOMER_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER|Build")
	static URigdocksRpcCall* AZ_SetMediaItemHANDAUTOMER_Build(UMediaItem* item, int ratioType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER|Build")
	static URigdocksRpcCall* AZ_SetMediaIdHANDAUTOMER_Build(UReaProject* projID, int itemID, int ratioType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaIdHANDAUTOMER_Build(UReaProject* projID, int selectItemID, int ratioType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|HANDAUTOMER|Build")
	static URigdocksRpcCall* AZ_SetMediaItemListHANDAUTOMER_Build(TArray<UMediaItem*> itemList, int ratioType);
};
