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
#include "RigdocksGold_SVN_Parse.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_SVN_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static void AZ_SVN_SetAuth_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static void AZ_SVN_CheckoutOrUpdate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static TArray<FString> AZ_SVN_Add_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static int AZ_SVN_Commit_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static void AZ_SVN_Revert_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static TArray<FString> AZ_SVN_Export_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static TArray<FString> AZ_SVN_GetFileList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static TArray<FString> AZ_SVN_GetAddAndUpdateList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static FString AZ_SVN_GetRootUrl_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static TArray<FString> AZ_SVN_Export_CommitOnly_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static TArray<FString> AZ_SVN_ExportAudioToProject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Parse")
	static TArray<FString> AZ_SVN_ExportAudioToProject_CommitOnly_Parse(URigdocksRpcCall* call);
};
