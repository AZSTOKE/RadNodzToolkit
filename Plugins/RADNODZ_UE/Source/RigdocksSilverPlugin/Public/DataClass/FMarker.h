// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/Marker.h"
#include "FMarker.generated.h"

USTRUCT(BlueprintType)
struct RIGDOCKSSILVERPLUGIN_API FMarker
{
public:

	GENERATED_BODY()

	void Initialize(RADNODZ_client::Object::Marker& obj);

	RADNODZ_client::Object::Marker GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Marker")
	int Index{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Marker")
	int Number{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Marker")
	double StartTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Marker")
	double EndTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Marker")
	int Color{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|Marker")
	FString Name{};

};

UCLASS(BlueprintType)
class RIGDOCKSSILVERPLUGIN_API UMarkerFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Marker")
	static bool FromJson(const FString& Json, FMarker& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Marker")
	static bool FromJsonArray(const FString& Json, TArray<FMarker>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Marker",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FMarker& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|Marker",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FMarker>& objectArray, int indent = 4);

};
