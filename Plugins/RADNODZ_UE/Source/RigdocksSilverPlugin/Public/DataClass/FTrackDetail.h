// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReaperClasses.h"
#include "Object/TrackDetail.h"
#include "FTrackDetail.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSSILVERPLUGIN_API FTrackDetail
{
public:

	GENERATED_BODY()
	
	void Initialize(TrackDetail& obj);

	TrackDetail GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackDetail")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackDetail")
	UMediaTrack* Track{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackDetail")
	double Volume{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackDetail")
	int Index{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackDetail")
	int Depth{};

};

UCLASS(BlueprintType)
class RIGDOCKSSILVERPLUGIN_API UTrackDetailFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TrackDetail")
	static bool FromJson(const FString& Json, FTrackDetail& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TrackDetail")
	static bool FromJsonArray(const FString& Json, TArray<FTrackDetail>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TrackDetail",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FTrackDetail& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TrackDetail",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FTrackDetail>& objectArray, int indent = 4);

};
