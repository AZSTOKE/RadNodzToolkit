// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "RigdocksSilver_TALKTRON_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_TALKTRON_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TALKTRON|Build")
	static URigdocksRpcCall* AZ_TALKTRON_InsertMediaItems_Build(FString subjectLangPath, FString baseLangPath, double startPos, double mediaInterval, int startEndType, bool isVolMatch, bool isHANDAUTOMER, int HANDAUTOMER_ratioType, TArray<FString> trackNameList);
};
