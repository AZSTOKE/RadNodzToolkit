// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "GoldClasses.h"
#include "DataClass/FRenderingOptions.h"
#include "RigdocksGold_Render.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_Render : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_RenderToAudioFile(URadnodzClient* client, UReaProject* project, FRenderingOptions options, TArray<FString>& primaryOutputList, TArray<FString>& secoundaryOutputList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FRenderingOptions AZ_GetRenderSetting(URadnodzClient* client, UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FRenderingOptions AZ_GetRenderSettingFromProject(URadnodzClient* client, FString projectPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_SetRenderSetting(URadnodzClient* client, UReaProject* project, FRenderingOptions options);

};
