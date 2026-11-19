// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "RigdocksSilver_UI.h"
#include "RigdocksSilver_UI_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_UI_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI|Build")
	static URigdocksRpcCall* AZ_ShowMessageBox_Ex_Build(FString messageStr, FString windowTitleStr, AZ_WindowType windowType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI|Build")
	static URigdocksRpcCall* AZ_GetUserInputList_Build(FString titleStr, int inputNum, FString captionStr, FString resultStr);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI|Build")
	static URigdocksRpcCall* AZ_ShowMessageBox_Info_Build(FString message, FString title, AZ_WindowType windowType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI|Build")
	static URigdocksRpcCall* AZ_ShowMessageBox_Warn_Build(FString message, FString title, AZ_WindowType windowType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI|Build")
	static URigdocksRpcCall* AZ_ShowMessageBox_Error_Build(FString message, FString title, AZ_WindowType windowType);
};
