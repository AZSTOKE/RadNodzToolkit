// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "GoldClasses.h"
#include "DataClass/FExcelSheetInfo.h"
#include "RigdocksGold_Excel.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_Excel : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  UXLDocument* AZ_Excel_OpenFile(URadnodzClient* client, FString filePath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  void AZ_Excel_CloseFile(URadnodzClient* client, UXLDocument* excelDocumment);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  UXLWorksheet* AZ_Excel_OpenWorksheet(URadnodzClient* client, UXLDocument* excelDocumment, FString sheetName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	UXLWorksheet* AZ_Excel_OpenWorksheet_ByNumber(URadnodzClient* client, UXLDocument* excelDocumment, int sheetNumber);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  UVarType* AZ_Excel_GetCellValue(URadnodzClient* client, UXLWorksheet* excelSheet, FString cell);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	UVarType* AZ_Excel_GetCellValue_ByNumber(URadnodzClient* client, UXLWorksheet* excelSheet, int rowNumber, int columeNumber);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Excel_SetCellValue(URadnodzClient* client, UXLWorksheet* excelSheet, FString cell, UVarType* valueJson);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Excel_SetCellValue_ByNumber(URadnodzClient* client, UXLWorksheet* excelSheet, int rowNumber, int columeNumber, UVarType* value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Excel_SaveDocument(URadnodzClient* client, UXLDocument* excelDocumment, FString newPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	UXLDocument* AZ_Excel_CreateDocument(URadnodzClient* client, FString newPath, bool isOverwrite = true);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	UXLWorksheet* AZ_Excel_AddWorksheet(URadnodzClient* client, UXLDocument* excelDocumment, FString worksheetName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Excel_RenameWorksheet(URadnodzClient* client, UXLWorksheet* excelWorksheet, FString worksheetName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Excel_DeleteWorksheet(URadnodzClient* client, UXLWorksheet* excelWorksheet);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FExcelSheetInfo> AZ_Excel_GetWorksheetList(URadnodzClient* client, UXLDocument* document, FString searchStr, bool isRegex, TArray<FString> excludeList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FExcelSheetInfo AZ_Excel_SearchCellsInWorksheet(URadnodzClient* client, UXLWorksheet* excelSheet, FString searchStr, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FExcelSheetInfo AZ_Excel_SearchRowsInWorksheet(URadnodzClient* client, UXLWorksheet* excelSheet, FString searchStr, FString targetCulume, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FExcelSheetInfo> AZ_Excel_SearchCellsInAllWorksheets(URadnodzClient* client, UXLDocument* document, FString searchStr, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Excel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FExcelSheetInfo> AZ_Excel_SearchRowsInAllWorksheets(URadnodzClient* client, UXLDocument* document, FString searchStr, FString targetCulume, FString range, bool isRegex);

};
