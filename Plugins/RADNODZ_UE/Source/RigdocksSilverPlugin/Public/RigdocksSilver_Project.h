// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FSection.h"
#include "RigdocksSilver_Project.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Project : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_SetProjectInfoRecPath(URadnodzClient* client, UReaProject* projID, FString path);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetProjectInfoRecAddPath(URadnodzClient* client, UReaProject* projID, FString addPath, FString& brforePath, FString& afterPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetProjectPathFolder(URadnodzClient* client, FString folderName, int timeAddType, FString& brforePath, FString& afterPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetResoucePathFolder(URadnodzClient* client, FString folderName, int timeAddType, FString& brforePath, FString& afterPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetRepeat(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetRepeate(URadnodzClient* client, UReaProject* projID, bool repeat);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetPlayState(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetRecordState(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetPauseState(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetStopState(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetRecordingSampleRate(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetRecordingBitDepth(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetProjectName(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetProjectPath(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UReaProject* AZ_OpenProject(URadnodzClient* client, FString projectPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SaveProject(URadnodzClient* client, UReaProject* projID, FString dstPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_CloseProject(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SwitchProject(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UReaProject* AZ_GetProjectFromName(URadnodzClient* client, FString projectName);

	// Launch REAPER and wait until the RIGDOCKS extension is reachable.
	// If REAPER is already running, opens projectPath in a new tab (does nothing if empty).
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_LaunchReaper(URadnodzClient* client, FString projectPath = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_IsReaperRunning(URadnodzClient* client);

	// save = true: save changed projects then quit / false: terminate without saving
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Project",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_QuitReaper(URadnodzClient* client, bool save = true);

};
