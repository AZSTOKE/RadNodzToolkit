// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/Peak.h"
#include "FPeak.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSSILVERPLUGIN_API FPeak
{
public:

	GENERATED_BODY()
	
	void Initialize(Peak& obj);

	Peak GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Peak")
	double Time{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Peak")
	double MaxPeak{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Peak")
	double MinPeak{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Peak")
	double Frequency{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Peak")
	double Pos{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Peak")
	double PointVol{};

};

UCLASS(BlueprintType)
class RIGDOCKSSILVERPLUGIN_API UPeakFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Peak")
	static bool FromJson(const FString& Json, FPeak& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Peak")
	static bool FromJsonArray(const FString& Json, TArray<FPeak>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Peak",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FPeak& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Peak",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FPeak>& objectArray, int indent = 4);

};
