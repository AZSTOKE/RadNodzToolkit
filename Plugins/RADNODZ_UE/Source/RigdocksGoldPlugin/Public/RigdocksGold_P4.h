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
#include "RigdocksGold_P4.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_P4 : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	bool  AZ_P4_Connect(URadnodzClient* client, FString uri, FString userName, FString password);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void  AZ_P4_Disconnect(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_changeListID = "-1"
			))
	static 	TArray<FString> AZ_P4_SyncFiles(URadnodzClient* client, FString workspace, FString depotPath, int changeListID, FString extension);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_fileWriteMode = "Overwrite",
			CPP_Default_changeListID = "-1"
			))
	static 	TArray<FString> AZ_P4_ExportToDirectory(URadnodzClient* client, FString workspace, FString toDir, FString depotPath, AZ_FileWriteMode fileWriteMode, int changeListID, TArray<FString> exclusionList, bool isRegex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|P4",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_changeListID = "-1",
			CPP_Default_extension = "wav",
			CPP_Default_toProjectDir = "true"
			))
	static 	TArray<FString> AZ_P4_ExportAudioToProject(URadnodzClient* client, FString workspace, FString depotPath, FString toDir, int changeListID, FString extension, bool toProjectDir, TArray<FString> exclusionList);

};
