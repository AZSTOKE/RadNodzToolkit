// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReaperClasses.h"
#include "Object/MiddlewareInfo.h"
#include "FMiddlewareInfo.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FMiddlewareInfo
{
public:

	GENERATED_BODY()
	
	void Initialize(MiddlewareInfo& obj);

	MiddlewareInfo GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|MiddlewareInfo")
	UMediaItem* Item{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|MiddlewareInfo")
	double Volume{};

};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UMiddlewareInfoFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MiddlewareInfo")
	static bool FromJson(const FString& Json, FMiddlewareInfo& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MiddlewareInfo")
	static bool FromJsonArray(const FString& Json, TArray<FMiddlewareInfo>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MiddlewareInfo",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FMiddlewareInfo& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|MiddlewareInfo",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FMiddlewareInfo>& objectArray, int indent = 4);

};
