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
#include "DataClass/FRenderingOptions.h"
#include "RigdocksGold_Render_Build.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_Render_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render|Build")
	static URigdocksRpcCall* AZ_RenderToAudioFile_Build(UReaProject* project, FRenderingOptions options);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render|Build")
	static URigdocksRpcCall* AZ_GetRenderSetting_Build(UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render|Build")
	static URigdocksRpcCall* AZ_GetRenderSettingFromProject_Build(FString projectPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Render|Build")
	static URigdocksRpcCall* AZ_SetRenderSetting_Build(UReaProject* project, FRenderingOptions options);
};
