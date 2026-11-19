// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "GoldClasses.h"
#include "DataClass/FExcelSheetInfo.h"
#include "RigdocksGold_Excel_Parse.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_Excel_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static UXLDocument* AZ_Excel_OpenFile_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static TArray<FExcelSheetInfo> AZ_Excel_GetWorksheetList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static void AZ_Excel_CloseFile_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static UXLWorksheet* AZ_Excel_OpenWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static UXLWorksheet* AZ_Excel_OpenWorksheet_ByNumber_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static UVarType* AZ_Excel_GetCellValue_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static UVarType* AZ_Excel_GetCellValue_ByNumber_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static void AZ_Excel_SetCellValue_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static void AZ_Excel_SetCellValue_ByNumber_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static void AZ_Excel_SaveDocument_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static UXLDocument* AZ_Excel_CreateDocument_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static UXLWorksheet* AZ_Excel_AddWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static void AZ_Excel_RenameWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static void AZ_Excel_DeleteWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static FExcelSheetInfo AZ_Excel_SearchCellsInWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static FExcelSheetInfo AZ_Excel_SearchRowsInWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static TArray<FExcelSheetInfo> AZ_Excel_SearchCellsInAllWorksheets_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Parse")
	static TArray<FExcelSheetInfo> AZ_Excel_SearchRowsInAllWorksheets_Parse(URigdocksRpcCall* call);
};
