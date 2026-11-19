// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/Section.h"
#include "FSection.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSSILVERPLUGIN_API FSection
{
public:

	GENERATED_BODY()
	
	void Initialize(Section& obj);

	Section GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Section")
	double StartTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Section")
	double EndTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Section")
	double Dulation{};

};

UCLASS(BlueprintType)
class RIGDOCKSSILVERPLUGIN_API USectionFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Section")
	static bool FromJson(const FString& Json, FSection& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Section")
	static bool FromJsonArray(const FString& Json, TArray<FSection>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Section",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FSection& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Section",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FSection>& objectArray, int indent = 4);

};
