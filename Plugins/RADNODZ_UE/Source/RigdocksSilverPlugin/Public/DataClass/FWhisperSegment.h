// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/WhisperSegment.h"
#include "FWhisperSegment.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSSILVERPLUGIN_API FWhisperSegment
{
public:

	GENERATED_BODY()

	WhisperSegment GetCppObj() const;

	void Initialize(WhisperSegment& obj);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WhisperSegment")
	double StartTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WhisperSegment")
	double EndTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WhisperSegment")
	FString Text{};

};

UCLASS(BlueprintType)
class RIGDOCKSSILVERPLUGIN_API UWhisperSegmentFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WhisperSegment")
	static bool FromJson(const FString& Json, FWhisperSegment& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WhisperSegment")
	static bool FromJsonArray(const FString& Json, TArray<FWhisperSegment>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WhisperSegment",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FWhisperSegment& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WhisperSegment",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FWhisperSegment>& objectArray, int indent = 4);

};
