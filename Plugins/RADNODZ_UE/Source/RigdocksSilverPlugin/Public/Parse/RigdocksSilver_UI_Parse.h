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
#include "RigdocksSilver_UI_Parse.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_UI_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI|Parse")
	static bool AZ_ShowMessageBox_Ex_Parse(URigdocksRpcCall* call, AZ_ButtonID& pushedButtonId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI|Parse")
	static bool AZ_GetUserInputList_Parse(URigdocksRpcCall* call, TArray<FString>& inputList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI|Parse")
	static bool AZ_ShowMessageBox_Info_Parse(URigdocksRpcCall* call, AZ_ButtonID& pushedButtonId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI|Parse")
	static bool AZ_ShowMessageBox_Warn_Parse(URigdocksRpcCall* call, AZ_ButtonID& pushedButtonId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI|Parse")
	static bool AZ_ShowMessageBox_Error_Parse(URigdocksRpcCall* call, AZ_ButtonID& pushedButtonId);
};
