// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "ReaperEnums.h"
#include "GoldClasses.h"
#include "RigdocksGold_SVN.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_SVN : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void  AZ_SVN_SetAuth(URadnodzClient* client, FString userName, FString password);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_revision = "-1"
			))
	static 	void  AZ_SVN_CheckoutOrUpdate(URadnodzClient* client, FString repositoryUrl, FString workingCopyPath, int revision);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_isRegex = "true"
			))
	static 	TArray<FString>  AZ_SVN_Add(URadnodzClient* client, FString localPath, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_isRegex = "true"
			))
	static 	int  AZ_SVN_Commit(URadnodzClient* client, FString localPath, FString message, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_isRegex = "true"
			))
	static 	void AZ_SVN_Revert(URadnodzClient* client, FString localPath, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_mode = "Overwrite",
			CPP_Default_revision = "-1",
			CPP_Default_isRegex = "true"
			))
	static 	TArray<FString> AZ_SVN_Export(URadnodzClient* client, FString repositoryUrl, FString toDir, AZ_FileWriteMode mode, int revision, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_revision = "-1",
			CPP_Default_isRegex = "true"
			))
	static 	TArray<FString> AZ_SVN_GetFileList(URadnodzClient* client, FString repositoryUrl, int revision, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_revision = "-1",
			CPP_Default_isRegex = "true"
			))
	static 	TArray<FString> AZ_SVN_GetAddAndUpdateList(URadnodzClient* client, FString repositoryUrl, int revision, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FString AZ_SVN_GetRootUrl(URadnodzClient* client, FString repositoryUrl);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_mode = "Overwrite",
			CPP_Default_revision = "-1",
			CPP_Default_isRegex = "true"
			))
	static 	TArray<FString> AZ_SVN_Export_CommitOnly(URadnodzClient* client, FString repositoryUrl, FString toDir, AZ_FileWriteMode mode, int revision, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_revision = "-1",
			CPP_Default_extension = "wav",
			CPP_Default_isRegex = "true"
			))
	static 	TArray<FString> AZ_SVN_ExportAudioToProject(URadnodzClient* client, FString repositoryUrl, int revision, FString toDir, FString extension, bool toProjectDir);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|SVN",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_revision = "-1",
			CPP_Default_extension = "wav",
			CPP_Default_isRegex = "true"
			))
	static 	TArray<FString> AZ_SVN_ExportAudioToProject_CommitOnly(URadnodzClient* client, FString repositoryUrl, int revision, FString toDir, FString extension, bool toProjectDir);
};
