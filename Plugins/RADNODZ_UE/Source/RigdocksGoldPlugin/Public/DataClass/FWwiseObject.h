// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReaperClasses.h"
#include "Object/WwiseObject.h"
#include "FWwiseObject.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FWwiseObject
{
public:

	GENERATED_BODY()
	
	void Initialize(WwiseObject& obj);

	WwiseObject GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseObject")
	FString ID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseObject")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseObject")
	FString Path{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseObject")
	TMap<FString, UVarType*> Properties{};

};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UWwiseObjectFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseObject")
	static bool FromJson(const FString& Json, FWwiseObject& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseObject")
	static bool FromJsonArray(const FString& Json, TArray<FWwiseObject>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseObject",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FWwiseObject& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseObject",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FWwiseObject>& objectArray, int indent = 4);

};
