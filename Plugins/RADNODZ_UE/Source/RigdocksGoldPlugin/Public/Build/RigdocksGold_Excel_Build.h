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
#include "RigdocksGold_Excel_Build.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_Excel_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_OpenFile_Build(FString filePath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_GetWorksheetList_Build(UXLDocument* document, FString searchStr, bool isRegex, TArray<FString> excludeList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_CloseFile_Build(UXLDocument* excelDocumment);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_OpenWorksheet_Build(UXLDocument* excelDocumment, FString sheetName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_OpenWorksheet_ByNumber_Build(UXLDocument* excelDocumment, int sheetNumber);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_GetCellValue_Build(UXLWorksheet* excelSheet, FString cell);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_GetCellValue_ByNumber_Build(UXLWorksheet* excelSheet, int rowNumber, int columeNumber);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_SetCellValue_Build(UXLWorksheet* excelSheet, FString cell, UVarType* value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_SetCellValue_ByNumber_Build(UXLWorksheet* excelSheet, int rowNumber, int columeNumber, UVarType* value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_SaveDocument_Build(UXLDocument* excelDocumment, FString newPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_CreateDocument_Build(FString newPath, bool isOverwrite = true);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_AddWorksheet_Build(UXLDocument* excelDocumment, FString worksheetName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_RenameWorksheet_Build(UXLWorksheet* excelWorksheet, FString worksheetName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_DeleteWorksheet_Build(UXLWorksheet* excelWorksheet);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_SearchCellsInWorksheet_Build(UXLWorksheet* excelSheet, FString searchStr, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_SearchRowsInWorksheet_Build(UXLWorksheet* excelSheet, FString searchStr, FString targetCulume, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_SearchCellsInAllWorksheets_Build(UXLDocument* document, FString searchStr, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel|Build")
	static URigdocksRpcCall* AZ_Excel_SearchRowsInAllWorksheets_Build(UXLDocument* document, FString searchStr, FString targetCulume, FString range, bool isRegex);
};
