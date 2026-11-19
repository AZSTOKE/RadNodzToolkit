// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReaperClasses.h"
#include "GoldClasses.h"
#include "Object/GSheetInfo.h"
#include "FGSheetInfo.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FCellItem
{
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|GSheetInfo")
	int32 RowNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|GSheetInfo")
	int32 ColNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|GSheetInfo")
	FString CellAddress{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|GSheetInfo")
	FString Value{};
};

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FCellRow
{
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|GSheetInfo")
	int32 RowNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|GSheetInfo")
	TMap<FString, FCellItem> Cells{};
};

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FGSheetInfo
{
public:

	GENERATED_BODY()

	void Initialize(GSheetInfo& obj);

	GSheetInfo GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|GSheetInfo")
	int32 Id = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|GSheetInfo")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|GSheetInfo")
	int32 Index = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|GSheetInfo")
	TArray<FCellRow> Cells{};
};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UGSheetInfoFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|GSheetInfo")
	static bool FromJson(const FString& Json, FGSheetInfo& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|GSheetInfo")
	static bool FromJsonArray(const FString& Json, TArray<FGSheetInfo>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|GSheetInfo",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FGSheetInfo& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|GSheetInfo",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FGSheetInfo>& objectArray, int indent = 4);

};
