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
#include "RigdocksSilver_Http_Parse.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Http_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Parse")
	static bool AZ_Http_Get_Parse(URigdocksRpcCall* call, FHttpResponse& response);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Parse")
	static bool AZ_Http_Post_Parse(URigdocksRpcCall* call, FHttpResponse& response);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Parse")
	static bool AZ_Http_Put_Parse(URigdocksRpcCall* call, FHttpResponse& response);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Parse")
	static bool AZ_Http_Patch_Parse(URigdocksRpcCall* call, FHttpResponse& response);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Parse")
	static bool AZ_Http_Delete_Parse(URigdocksRpcCall* call, FHttpResponse& response);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Http|Parse")
	static bool AZ_Http_Options_Parse(URigdocksRpcCall* call, FHttpResponse& response);
};
