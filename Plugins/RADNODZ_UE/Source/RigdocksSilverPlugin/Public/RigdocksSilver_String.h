// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "RigdocksSilver_String.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_String : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_CheckIncludeString(URadnodzClient* client, FString textString, FString checkString);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_CheckSameString(URadnodzClient* client, FString textString, FString checkString);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_CheckIncludePerfectString(URadnodzClient* client, FString textString, FString checkString);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_ReplaceSearchString(URadnodzClient* client, FString textString, FString searchString, FString replaceString);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_DeleteBlankString(URadnodzClient* client, FString textString);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_FormatNumDigit(URadnodzClient* client, int num, int digit);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FString> AZ_GetStringSplitList(URadnodzClient* client, FString baseStr, FString splitStr);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_JoinArray(URadnodzClient* client, TArray<FString> stringArray, FString separator);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_NormalizeText(URadnodzClient* client, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_CalcStringSimilarity(URadnodzClient* client, FString targetText, FString sourceText);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|String",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_CalcStringPartialSimilarity(URadnodzClient* client, FString targetText, FString sourceText);

};
