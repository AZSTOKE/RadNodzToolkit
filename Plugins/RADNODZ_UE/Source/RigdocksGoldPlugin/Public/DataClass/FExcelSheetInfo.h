// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReaperClasses.h"
#include "GoldClasses.h"
#include "Object/ExcelSheetInfo.h"
#include "FExcelSheetInfo.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FExcelRow
{
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|ExcelSheetInfo")
	TMap<FString, FString> Cells{};
};

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FExcelSheetInfo
{
public:

	GENERATED_BODY()
	
	void Initialize(ExcelSheetInfo& obj);

	ExcelSheetInfo GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|ExcelSheetInfo")
	UXLWorksheet* Worksheet{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|ExcelSheetInfo")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|ExcelSheetInfo")
	TMap<FString, FExcelRow> Cells{};
};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UExcelSheetInfoFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|ExcelSheetInfo")
	static bool FromJson(const FString& Json, FExcelSheetInfo& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|ExcelSheetInfo")
	static bool FromJsonArray(const FString& Json, TArray<FExcelSheetInfo>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|ExcelSheetInfo",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FExcelSheetInfo& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|ExcelSheetInfo",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FExcelSheetInfo>& objectArray, int indent = 4);

};
