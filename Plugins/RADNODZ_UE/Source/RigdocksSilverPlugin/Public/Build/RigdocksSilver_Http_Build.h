// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "DataClass/FHttpResponse.h"
#include "RigdocksSilver_Http_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Http_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Build")
	static URigdocksRpcCall* AZ_Http_Get_Build(FString url, TMap<FString, FString> headers, TMap<FString, FString> queryParams);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Build")
	static URigdocksRpcCall* AZ_Http_Post_Build(FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Build")
	static URigdocksRpcCall* AZ_Http_Put_Build(FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Build")
	static URigdocksRpcCall* AZ_Http_Patch_Build(FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Build")
	static URigdocksRpcCall* AZ_Http_Delete_Build(FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Build")
	static URigdocksRpcCall* AZ_Http_Options_Build(FString url, TMap<FString, FString> headers);
};
