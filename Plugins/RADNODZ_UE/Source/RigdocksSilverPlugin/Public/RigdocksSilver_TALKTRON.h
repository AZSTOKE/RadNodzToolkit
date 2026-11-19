// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FSection.h"
#include "RigdocksSilver_TALKTRON.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_TALKTRON : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TALKTRON",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FString> AZ_TALKTRON_InsertMediaItems(URadnodzClient* client, FString subjectLangPath, FString baseLangPath, double startPos, double mediaInterval, int startEndType, bool isVolMatch, bool isHANDAUTOMER, int HANDAUTOMER_ratioType, TArray<FString> trackNameList);

};
