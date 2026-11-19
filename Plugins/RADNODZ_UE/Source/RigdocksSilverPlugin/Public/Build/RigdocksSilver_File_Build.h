// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "DataClass/FPathInfo.h"
#include "RigdocksSilver_File_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_File_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File|Build")
	static URigdocksRpcCall* AZ_WriteFile_Build(FString directory, FString filename, FString text, int mode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File|Build")
	static URigdocksRpcCall* AZ_ReadFile_Build(FString directory, FString filename);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File|Build")
	static URigdocksRpcCall* AZ_GetPathInfo_Build(FString file_path);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File|Build")
	static URigdocksRpcCall* AZ_GetFilePathList_Build(FString folder_path, FString search_extension = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File|Build")
	static URigdocksRpcCall* AZ_GetAppDataPath_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File|Build")
	static URigdocksRpcCall* AZ_GetBridgeDataPath_Build();
};
