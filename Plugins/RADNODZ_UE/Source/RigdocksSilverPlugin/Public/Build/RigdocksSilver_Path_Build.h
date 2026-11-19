// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "RigdocksSilver_Path_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Path_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Path|Build")
	static URigdocksRpcCall* AZ_SetPathFolder_Build(FString folderPath, FString folderName, int timeAddType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Path|Build")
	static URigdocksRpcCall* AZ_SetCopyFile_Build(FString copyFilePath, FString pasteFilePath);
};
