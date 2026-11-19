// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FPathInfo.h"
#include "RigdocksSilver_File.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_File : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_WriteFile(URadnodzClient* client, FString directory, FString filename, FString text, int mode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_ReadFile(URadnodzClient* client, FString directory, FString filename);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FPathInfo AZ_GetPathInfo(URadnodzClient* client, FString file_path);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FString> AZ_GetFilePathList(URadnodzClient* client, FString folder_path, FString search_extension = "");

	// App data folder on the connected PC (parent of the RIGDOCKS Bridge folder)
	//   Windows : %APPDATA% / macOS : ~/Library/Application Support
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetAppDataPath(URadnodzClient* client);

	// RIGDOCKS Bridge folder on the connected PC
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|File",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetBridgeDataPath(URadnodzClient* client);

};
