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
#include "RigdocksGold_GSheet_Build.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_GSheet_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_Connect_UserAccount_Build(FString spreadSheetId, FString clientId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_Disconnect_Build(UGSheetConnection* connection);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_AddWorksheet_Build(UGSheetConnection* connection, FString sheetName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_DeleteWorksheet_Build(UGSheetConnection* connection, int sheetId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_RenameWorksheet_Build(UGSheetConnection* connection, int sheetId, FString newSheetName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_CopyWorksheet_Build(UGSheetConnection* connection, int sheetId, FString newSheetName, bool isNextSheet = true);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_GetWorksheetList_Build(UGSheetConnection* connection, FString searchStr, bool isRegex, TArray<FString> excludeList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_GetCellValue_Build(UGSheetConnection* connection, int sheetId, FString cell);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_GetCellValue_ByNumber_Build(UGSheetConnection* connection, int sheetId, int rowNumber, int columnNumber);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_SetCellValue_Build(UGSheetConnection* connection, int sheetId, FString cell, UVarType* value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_SetCellValue_ByNumber_Build(UGSheetConnection* connection, int sheetId, int rowNumber, int columnNumber, UVarType* value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_SearchCellsInWorksheet_Build(UGSheetConnection* connection, int sheetId, FString searchStr, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_SearchRowsInWorksheet_Build(UGSheetConnection* connection, int sheetId, FString searchStr, FString targetColumn, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_SearchCellsInAllWorksheets_Build(UGSheetConnection* connection, FString searchStr, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Build")
	static URigdocksRpcCall* AZ_GSheet_SearchRowsInAllWorksheets_Build(UGSheetConnection* connection, FString searchStr, FString targetColumn, FString range, bool isRegex);
};
