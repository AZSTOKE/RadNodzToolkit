// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "ReaperClasses.h"
#include "DataClass/FSection.h"
#include "RigdocksSilver_Project_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Project_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_SetProjectInfoRecPath_Build(UReaProject* projID, FString path);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_SetProjectInfoRecAddPath_Build(UReaProject* projID, FString addPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_SetProjectPathFolder_Build(FString folderName, int timeAddType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_SetResoucePathFolder_Build(FString folderName, int timeAddType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_GetRepeat_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_SetRepeate_Build(UReaProject* projID, bool repeat);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_GetPlayState_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_GetRecordState_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_GetPauseState_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_GetStopState_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_GetRecordingSampleRate_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_GetRecordingBitDepth_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_GetProjectName_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_GetProjectPath_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_OpenProject_Build(FString projectPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_SaveProject_Build(UReaProject* projID, FString dstPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_CloseProject_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_SwitchProject_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_GetProjectFromName_Build(FString projectName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_LaunchReaper_Build(FString projectPath = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_IsReaperRunning_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Build")
	static URigdocksRpcCall* AZ_QuitReaper_Build(bool save = true);
};
