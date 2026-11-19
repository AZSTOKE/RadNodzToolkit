// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "ReaperEnums.h"
#include "GoldClasses.h"

#include "RigdocksGold_P4_Build.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_P4_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4|Build")
	static URigdocksRpcCall* AZ_P4_Connect_Build(FString uri, FString userName, FString password);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4|Build")
	static URigdocksRpcCall* AZ_P4_Disconnect_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4|Build")
	static URigdocksRpcCall* AZ_P4_SyncFiles_Build(FString workspace, FString depotPath, int changeListID, FString extension);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4|Build")
	static URigdocksRpcCall* AZ_P4_ExportToDirectory_Build(FString workspace, FString toDir, FString depotPath, AZ_FileWriteMode fileWriteMode, int changeListID, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4|Build")
	static URigdocksRpcCall* AZ_P4_ExportAudioToProject_Build(FString workspace, FString depotPath, FString toDir, int changeListID, FString extension, bool toProjectDir, TArray<FString> exclusionList);
};
