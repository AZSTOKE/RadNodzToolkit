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
#include "RigdocksSilver_String_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_String_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String|Build")
	static URigdocksRpcCall* AZ_CheckIncludeString_Build(FString textString, FString checkString);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String|Build")
	static URigdocksRpcCall* AZ_CheckSameString_Build(FString textString, FString checkString);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String|Build")
	static URigdocksRpcCall* AZ_CheckIncludePerfectString_Build(FString textString, FString checkString);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String|Build")
	static URigdocksRpcCall* AZ_ReplaceSearchString_Build(FString textString, FString searchString, FString replaceString);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String|Build")
	static URigdocksRpcCall* AZ_DeleteBlankString_Build(FString textString);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String|Build")
	static URigdocksRpcCall* AZ_FormatNumDigit_Build(int num, int digit);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String|Build")
	static URigdocksRpcCall* AZ_GetStringSplitList_Build(FString baseStr, FString splitStr);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String|Build")
	static URigdocksRpcCall* AZ_JoinArray_Build(TArray<FString> stringArray, FString separator);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String|Build")
	static URigdocksRpcCall* AZ_NormalizeText_Build(FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String|Build")
	static URigdocksRpcCall* AZ_CalcStringSimilarity_Build(FString targetText, FString sourceText);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String|Build")
	static URigdocksRpcCall* AZ_CalcStringPartialSimilarity_Build(FString targetText, FString sourceText);
};
