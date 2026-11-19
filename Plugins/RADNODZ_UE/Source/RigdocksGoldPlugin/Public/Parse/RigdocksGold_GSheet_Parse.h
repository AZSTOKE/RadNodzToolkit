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
#include "DataClass/FGSheetInfo.h"
#include "RigdocksGold_GSheet_Parse.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_GSheet_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static UGSheetConnection* AZ_GSheet_Connect_UserAccount_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static void AZ_GSheet_Disconnect_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static FGSheetInfo AZ_GSheet_AddWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static void AZ_GSheet_DeleteWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static void AZ_GSheet_RenameWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static FGSheetInfo AZ_GSheet_CopyWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static TArray<FGSheetInfo> AZ_GSheet_GetWorksheetList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static UVarType* AZ_GSheet_GetCellValue_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static UVarType* AZ_GSheet_GetCellValue_ByNumber_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static void AZ_GSheet_SetCellValue_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static void AZ_GSheet_SetCellValue_ByNumber_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static FGSheetInfo AZ_GSheet_SearchCellsInWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static FGSheetInfo AZ_GSheet_SearchRowsInWorksheet_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static TArray<FGSheetInfo> AZ_GSheet_SearchCellsInAllWorksheets_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|GSheet|Parse")
	static TArray<FGSheetInfo> AZ_GSheet_SearchRowsInAllWorksheets_Parse(URigdocksRpcCall* call);
};
