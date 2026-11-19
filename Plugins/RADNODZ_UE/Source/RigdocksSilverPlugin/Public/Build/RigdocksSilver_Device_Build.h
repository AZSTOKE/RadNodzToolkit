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
#include "RigdocksSilver_Device_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Device_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device|Build")
	static URigdocksRpcCall* AZ_GetLocalIpAddresse_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device|Build")
	static URigdocksRpcCall* AZ_GetDeviceName_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device|Build")
	static URigdocksRpcCall* AZ_CountInputChannel_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device|Build")
	static URigdocksRpcCall* AZ_GetInputChannelList_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device|Build")
	static URigdocksRpcCall* AZ_CountOutputChannel_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device|Build")
	static URigdocksRpcCall* AZ_GetOutputChannelList_Build();
};
