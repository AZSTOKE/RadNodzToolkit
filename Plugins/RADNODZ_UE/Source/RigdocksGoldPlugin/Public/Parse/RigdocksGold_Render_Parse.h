// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "DataClass/FRenderingOptions.h"
#include "RigdocksGold_Render_Parse.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_Render_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render|Parse")
	static void AZ_RenderToAudioFile_Parse(URigdocksRpcCall* call, TArray<FString>& primaryOutputList, TArray<FString>& secoundaryOutputList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render|Parse")
	static FRenderingOptions AZ_GetRenderSetting_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render|Parse")
	static FRenderingOptions AZ_GetRenderSettingFromProject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render|Parse")
	static void AZ_SetRenderSetting_Parse(URigdocksRpcCall* call);
};
