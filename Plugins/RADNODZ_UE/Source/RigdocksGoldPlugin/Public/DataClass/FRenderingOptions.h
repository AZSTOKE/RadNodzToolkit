// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/RenderingOptions.h"
#include "FRenderingOptions.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FRenderingOptions
{
public:

	GENERATED_BODY()
	
	void Initialize(RenderingOptions& obj);

	RenderingOptions GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	FString OutputDir{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	FString FileName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	int RenderingTarget{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	int RenderingRangeType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	double StartPos{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	double EndPos{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	double SamplingRate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	int Channels{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	double TailMs{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	bool IsTail{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	FString Format{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	FString Format2{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|RenderingOptions")
	int RenderingSetting{};

};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API URenderingOptionsFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|RenderingOptions")
	static bool FromJson(const FString& Json, FRenderingOptions& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|RenderingOptions")
	static bool FromJsonArray(const FString& Json, TArray<FRenderingOptions>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|RenderingOptions",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FRenderingOptions& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|RenderingOptions",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FRenderingOptions>& objectArray, int indent = 4);

};
