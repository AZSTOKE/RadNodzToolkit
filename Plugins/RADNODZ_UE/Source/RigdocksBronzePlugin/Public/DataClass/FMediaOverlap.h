// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/MediaOverlap.h"
#include "ReaperClasses.h"
#include "FMediaOverlap.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSBRONZEPLUGIN_API FMediaOverlap
{
public:

	GENERATED_BODY()
	
	void Initialize(MediaOverlap& obj);

	MediaOverlap GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|MediaOverlap")
	UMediaItem* Item{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|MediaOverlap")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|MediaOverlap")
	double StartTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|MediaOverlap")
	double EndTime{};

};

UCLASS(BlueprintType)
class RIGDOCKSBRONZEPLUGIN_API UMediaOverlapFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MediaOverlap")
	static bool FromJson(const FString& Json, FMediaOverlap& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MediaOverlap")
	static bool FromJsonArray(const FString& Json, TArray<FMediaOverlap>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MediaOverlap",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FMediaOverlap& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MediaOverlap",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FMediaOverlap>& objectArray, int indent = 4);

};
