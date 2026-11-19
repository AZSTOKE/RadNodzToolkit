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
#include "RigdocksSilver_Project_Parse.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Project_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static FString AZ_SetProjectInfoRecPath_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static void AZ_SetProjectInfoRecAddPath_Parse(URigdocksRpcCall* call, FString& brforePath, FString& afterPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static void AZ_SetProjectPathFolder_Parse(URigdocksRpcCall* call, FString& brforePath, FString& afterPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static void AZ_SetResoucePathFolder_Parse(URigdocksRpcCall* call, FString& brforePath, FString& afterPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static bool AZ_GetRepeat_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static void AZ_SetRepeate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static bool AZ_GetPlayState_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static bool AZ_GetRecordState_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static bool AZ_GetPauseState_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static bool AZ_GetStopState_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static double AZ_GetRecordingSampleRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static int AZ_GetRecordingBitDepth_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static FString AZ_GetProjectName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static FString AZ_GetProjectPath_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static UReaProject* AZ_OpenProject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static void AZ_SaveProject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static void AZ_CloseProject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static void AZ_SwitchProject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static UReaProject* AZ_GetProjectFromName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static void AZ_LaunchReaper_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static bool AZ_IsReaperRunning_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project|Parse")
	static void AZ_QuitReaper_Parse(URigdocksRpcCall* call);
};
