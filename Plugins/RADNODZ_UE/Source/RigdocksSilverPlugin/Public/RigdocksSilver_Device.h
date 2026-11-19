// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "RigdocksSilver_Device.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Device : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FString> AZ_GetLocalIpAddresse(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetDeviceName(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_CountInputChannel(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FString> AZ_GetInputChannelList(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_CountOutputChannel(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Device",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FString> AZ_GetOutputChannelList(URadnodzClient* client);

};
