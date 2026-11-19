// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "RigdocksSilver_Path.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Path : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Path",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_SetPathFolder(URadnodzClient* client, FString folderPath, FString folderName, int timeAddType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Path",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetCopyFile(URadnodzClient* client, FString copyFilePath, FString pasteFilePath);

};
