// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/PathInfo.h"
#include "FPathInfo.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSSILVERPLUGIN_API FPathInfo
{
public:

	GENERATED_BODY()

	void Initialize(PathInfo& obj);

	PathInfo GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|PathInfo")
	FString ParentPath{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|PathInfo")
	FString ParentName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|PathInfo")
	FString FilePath{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|PathInfo")
	FString FileName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|PathInfo")
	FString FileNameWithoutExtension{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|PathInfo")
	FString Extension{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|PathInfo")
	FString Type{};

};

UCLASS(BlueprintType)
class RIGDOCKSSILVERPLUGIN_API UPathInfoFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|PathInfo")
	static bool FromJson(const FString& Json, FPathInfo& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|PathInfo")
	static bool FromJsonArray(const FString& Json, TArray<FPathInfo>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|PathInfo",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FPathInfo& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|PathInfo",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FPathInfo>& objectArray, int indent = 4);

};
