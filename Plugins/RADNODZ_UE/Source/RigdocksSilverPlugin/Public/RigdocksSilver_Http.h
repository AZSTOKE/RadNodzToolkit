// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FHttpResponse.h"
#include "RigdocksSilver_Http.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Http : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_Http_Get(URadnodzClient* client, FString url, TMap<FString, FString> headers, TMap<FString, FString> queryParams, FHttpResponse& response);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_Http_Post(URadnodzClient* client, FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams, FHttpResponse& response);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_Http_Put(URadnodzClient* client, FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams, FHttpResponse& response);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_Http_Patch(URadnodzClient* client, FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams, FHttpResponse& response);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_Http_Delete(URadnodzClient* client, FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams, FHttpResponse& response);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_Http_Options(URadnodzClient* client, FString url, TMap<FString, FString> headers, FHttpResponse& response);

};
