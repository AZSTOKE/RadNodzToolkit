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
#include "DataClass/FWwiseObject.h"
#include "DataClass/FWwisePropertyList.h"
#include "DataClass/FWwiseStateGroup.h"
#include "RigdocksGold_Wwise_Parse.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_Wwise_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static bool AZ_Wwise_Connect_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_Disconnect_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static FString AZ_Wwise_GetProjectName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_MoveObject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_SetName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static TArray<FWwiseObject> AZ_Wwise_GetSelectedObjectList_Parse(URigdocksRpcCall* call, int& count);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static TArray<FWwiseObject> AZ_Wwise_GetAllActorAncestorList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static TArray<FWwiseObject> AZ_Wwise_GetAllBusAncestorList_Parse(URigdocksRpcCall* call, int& count);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static TArray<FWwiseObject> AZ_Wwise_GetUserAuxSend_Parse(URigdocksRpcCall* call, int& count);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_GetSwitchSet_Parse(URigdocksRpcCall* call, FWwiseObject& switchGroup, TArray<FWwiseObject>& switcheList, int& switchCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static FWwisePropertyList AZ_Wwise_GetPropertyList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static FWwiseObject AZ_Wwise_CreateContainer_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static UVarType* AZ_Wwise_GetProperty_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_SetProperty_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static TArray<FWwiseObject> AZ_Wwise_SearchSoundItemFromName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static TArray<FWwiseObject> AZ_Wwise_GetChildrenList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static double AZ_Wwise_GetTotalVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static FWwiseObject AZ_Wwise_CreateSoundSFX_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static FWwiseObject AZ_Wwise_CreateSoundVoice_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_ImportAudioFile_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_GetStateSet_Parse(URigdocksRpcCall* call, FWwiseObject& stateGroup, TArray<FWwiseObject>& stateList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static TArray<FWwiseStateGroup> AZ_Wwise_GetStateModificationFromFile_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static TArray<FWwiseStateGroup> AZ_Wwise_GetStateModification_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_AssignToSwitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static FWwiseObject AZ_Wwise_CreateWorkUnit_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static FWwiseObject AZ_Wwise_CreateVirtualFolder_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static FWwiseObject AZ_Wwise_CreateActorMixer_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static FWwiseObject AZ_Wwise_CreateEvent_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_AddActionToEvent_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static FWwiseObject AZ_Wwise_CreateObject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_DeleteObject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_SetReference_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static bool AZ_Wwise_ExistsObject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_AddObjectToSoundBank_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_RemoveObjectFromSoundBank_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static TArray<FString> AZ_Wwise_GenerateSoundBank_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_SaveProject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static FWwiseObject AZ_Wwise_CopyObject_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static void AZ_Wwise_SetRandomizer_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Parse")
	static bool AZ_Wwise_HasChild_Parse(URigdocksRpcCall* call);
};
