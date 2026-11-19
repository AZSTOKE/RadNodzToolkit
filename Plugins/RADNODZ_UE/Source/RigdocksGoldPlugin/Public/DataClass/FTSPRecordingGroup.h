// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/TSPRecordingGroup.h"
#include "FTSPRecordingGroup.generated.h"

using namespace RADNODZ_client::Object;

// Recording group for AZ_CalcImpulseResponseFrom*TSP.
//   FilePaths          : takes recorded under the same condition (averaged per channel)
//   ReferenceFilePaths : loopback recording (mono) of each take, same order as FilePaths.
//                        Empty = no latency correction.
//   AutoAlign          : align each take / repeat to the first take by its direct sound before
//                        averaging (default true). Ignored when ReferenceFilePaths is given.
USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FTSPRecordingGroup
{
public:

	GENERATED_BODY()

	void Initialize(TSPRecordingGroup& obj);

	TSPRecordingGroup GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TSPRecordingGroup")
	TArray<FString> FilePaths{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TSPRecordingGroup")
	TArray<FString> ReferenceFilePaths{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TSPRecordingGroup")
	bool AutoAlign = true;

};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UTSPRecordingGroupFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TSPRecordingGroup")
	static bool FromJson(const FString& Json, FTSPRecordingGroup& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TSPRecordingGroup")
	static bool FromJsonArray(const FString& Json, TArray<FTSPRecordingGroup>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TSPRecordingGroup",
		meta = (AdvancedDisplay = "indent")
		)
	static FString ToJson(const FTSPRecordingGroup& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TSPRecordingGroup",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FTSPRecordingGroup>& objectArray, int indent = 4);

};
