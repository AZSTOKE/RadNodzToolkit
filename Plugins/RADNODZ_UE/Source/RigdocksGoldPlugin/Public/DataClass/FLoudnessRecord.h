// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/LoudnessRecord.h"
#include "FLoudnessRecord.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FLoudnessRecord
{
public:

	GENERATED_BODY()
	
	void Initialize(LoudnessRecord& obj);

	LoudnessRecord GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|LoudnessRecord")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|LoudnessRecord")
	double Loudness{};

};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API ULoudnessRecordFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|LoudnessRecord")
	static bool FromJson(const FString& Json, FLoudnessRecord& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|LoudnessRecord")
	static bool FromJsonArray(const FString& Json, TArray<FLoudnessRecord>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|LoudnessRecord",
		meta = (AdvancedDisplay = "indent")
		)
	static FString ToJson(const FLoudnessRecord& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|LoudnessRecord",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FLoudnessRecord>& objectArray, int indent = 4);

};
