// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/MIDDLESCANNER_Options.h"
#include "FMIDDLESCANNER_Options.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FBusItem
{
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|MIDDLESCANNER_Options")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|MIDDLESCANNER_Options")
	double Volume = 0.0;

};

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FMIDDLESCANNER_Options
{
public:

	GENERATED_BODY()

	void Initialize(MIDDLESCANNER_Options& obj);

	MIDDLESCANNER_Options GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|MIDDLESCANNER_Options")
	TArray<FString> ExcludeBusList{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|MIDDLESCANNER_Options")
	TArray<FString> ExcludeActorList{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|MIDDLESCANNER_Options")
	TArray<FBusItem> AddBusList{};

};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UMIDDLESCANNER_OptionsFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MIDDLESCANNER_Options")
	static bool FromJson(const FString& Json, FMIDDLESCANNER_Options& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MIDDLESCANNER_Options")
	static bool FromJsonArray(const FString& Json, TArray<FMIDDLESCANNER_Options>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MIDDLESCANNER_Options",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FMIDDLESCANNER_Options& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MIDDLESCANNER_Options",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FMIDDLESCANNER_Options>& objectArray, int indent = 4);

};
