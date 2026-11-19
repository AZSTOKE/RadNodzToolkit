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
#include "RigdocksGold_P4_Parse.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_P4_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4|Parse")
	static bool AZ_P4_Connect_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4|Parse")
	static void AZ_P4_Disconnect_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4|Parse")
	static TArray<FString> AZ_P4_SyncFiles_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4|Parse")
	static TArray<FString> AZ_P4_ExportToDirectory_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4|Parse")
	static TArray<FString> AZ_P4_ExportAudioToProject_Parse(URigdocksRpcCall* call);
};
