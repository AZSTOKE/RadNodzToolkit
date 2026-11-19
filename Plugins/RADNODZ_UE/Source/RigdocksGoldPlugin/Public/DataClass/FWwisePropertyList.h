// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReaperClasses.h"
#include "Object/WwisePropertyList.h"
#include "FWwisePropertyList.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FWwisePropertyList
{
public:

	GENERATED_BODY()
	
	void Initialize(WwisePropertyList& obj);

	WwisePropertyList GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	FString ID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	FString Path{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	double Volume{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	int Pitch{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	int Lowpass{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	int Highpass{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	double MakeUpGain{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	double OutputBusVolume{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	int OutputBusLowpass{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	int OutputBusHighpass{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	double InitialDelay{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	bool IsStreamingEnabled{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	bool IsNonCachable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	bool IsZeroLatency{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	int PreFetchLength{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	double VolumeOffset{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	double BusVolume{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	FString OutputBusID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwisePropertyList")
	FString OutputBusName{};

};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UWwisePropertyListFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwisePropertyList")
	static bool FromJson(const FString& Json, FWwisePropertyList& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwisePropertyList")
	static bool FromJsonArray(const FString& Json, TArray<FWwisePropertyList>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwisePropertyList",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FWwisePropertyList& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwisePropertyList",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FWwisePropertyList>& objectArray, int indent = 4);

};
