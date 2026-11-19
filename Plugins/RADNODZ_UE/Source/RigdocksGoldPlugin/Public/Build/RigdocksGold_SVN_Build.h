// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "ReaperEnums.h"
#include "GoldClasses.h"
#include "RigdocksGold_SVN_Build.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_SVN_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_SetAuth_Build(FString userName, FString password);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_CheckoutOrUpdate_Build(FString repositoryUrl, FString workingCopyPath, int revision);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_Add_Build(FString localPath, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_Commit_Build(FString localPath, FString message, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_Revert_Build(FString localPath, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_Export_Build(FString repositoryUrl, FString toDir, AZ_FileWriteMode mode, int revision, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_GetFileList_Build(FString repositoryUrl, int revision, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_GetAddAndUpdateList_Build(FString repositoryUrl, int revision, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_GetRootUrl_Build(FString repositoryUrl);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_Export_CommitOnly_Build(FString repositoryUrl, FString toDir, AZ_FileWriteMode mode, int revision, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_ExportAudioToProject_Build(FString repositoryUrl, int revision, FString toDir, FString extension, bool toProjectDir);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN|Build")
	static URigdocksRpcCall* AZ_SVN_ExportAudioToProject_CommitOnly_Build(FString repositoryUrl, int revision, FString toDir, FString extension, bool toProjectDir);
};
