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
#include "DataClass/FGSheetInfo.h"
#include "RigdocksGold_GSheet.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_GSheet : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  UGSheetConnection* AZ_GSheet_Connect_UserAccount(URadnodzClient* client, FString spreadSheetId, FString clientId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  void AZ_GSheet_Disconnect(URadnodzClient* client, UGSheetConnection* connection);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  FGSheetInfo AZ_GSheet_AddWorksheet(URadnodzClient* client, UGSheetConnection* connection, FString sheetName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  void AZ_GSheet_DeleteWorksheet(URadnodzClient* client, UGSheetConnection* connection, int sheetId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  void AZ_GSheet_RenameWorksheet(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString newSheetName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  FGSheetInfo AZ_GSheet_CopyWorksheet(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString newSheetName, bool isNextSheet = true);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  TArray<FGSheetInfo> AZ_GSheet_GetWorksheetList(URadnodzClient* client, UGSheetConnection* connection, FString searchStr, bool isRegex, TArray<FString> excludeList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  UVarType* AZ_GSheet_GetCellValue(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString cell);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  UVarType* AZ_GSheet_GetCellValue_ByNumber(URadnodzClient* client, UGSheetConnection* connection, int sheetId, int rowNumber, int columnNumber);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  void AZ_GSheet_SetCellValue(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString cell, UVarType* value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  void AZ_GSheet_SetCellValue_ByNumber(URadnodzClient* client, UGSheetConnection* connection, int sheetId, int rowNumber, int columnNumber, UVarType* value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  FGSheetInfo AZ_GSheet_SearchCellsInWorksheet(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString searchStr, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  FGSheetInfo AZ_GSheet_SearchRowsInWorksheet(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString searchStr, FString targetColumn, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  TArray<FGSheetInfo> AZ_GSheet_SearchCellsInAllWorksheets(URadnodzClient* client, UGSheetConnection* connection, FString searchStr, FString range, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static  TArray<FGSheetInfo> AZ_GSheet_SearchRowsInAllWorksheets(URadnodzClient* client, UGSheetConnection* connection, FString searchStr, FString targetColumn, FString range, bool isRegex);

};
