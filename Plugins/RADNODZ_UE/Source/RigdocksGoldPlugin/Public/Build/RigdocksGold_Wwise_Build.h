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
#include "RigdocksGold_Wwise.h"
#include "DataClass/FWwiseObject.h"
#include "DataClass/FWwisePropertyList.h"
#include "DataClass/FWwiseStateGroup.h"
#include "DataClass/FMIDDLESCANNER_Options.h"
#include "RigdocksGold_Wwise_Build.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_Wwise_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_Connect_Build(FString address, int port, int timeoutMs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_Disconnect_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetProjectName_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_MoveObject_Build(FString itemID, FString destinationID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_SetName_Build(FString itemID, FString name);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetSelectedObjectList_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetAllActorAncestorList_Build(FString itemID, TArray<FString> excludeList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetAllBusAncestorList_Build(FString itemID, TArray<FString> excludeList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetUserAuxSend_Build(FString itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetSwitchSet_Build(FString switchGroupName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetPropertyList_Build(FString itemID, FString platform = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_CreateContainer_Build(FString parentItemID, AZ_Wwise_ContainerType type, FString name, bool isUnique);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetProperty_Build(FString itemID, FString propertyName, FString language = "", FString platform = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_SetProperty_Build(FString itemID, FString propertyName, UVarType* value, FString platform = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_SearchSoundItemFromName_Build(FString parentID, FString name, bool isPartialMatch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetChildrenList_Build(FString itemID, bool isRecursive);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetTotalVolume_Build(FString itemID, FString language, FMIDDLESCANNER_Options options);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_CreateSoundSFX_Build(FString parentItemID, FString sourcePath = "", FString itemName = "", FString subFolder = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_CreateSoundVoice_Build(FString parentItemID, FString sourcePath = "", FString itemName = "", FString language = "", FString subFolder = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_ImportAudioFile_Build(FString itemID, FString sourcePath, FString language = "", FString subFolder = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetStateSet_Build(FString stateGroupName, bool igroreNone = true);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetStateModificationFromFile_Build(FString itemID, FString workUnitPath, TArray<FString> targetStateGroup, TArray<FString> targetPropertyList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GetStateModification_Build(FString itemID, TArray<FString> targetStateGroup, TArray<FString> targetPropertyList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_AssignToSwitch_Build(FString assignedItemID, FString switchOrStateID, bool idDefault = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_CreateWorkUnit_Build(FString parentItemID = "", FString name = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_CreateVirtualFolder_Build(FString parentItemID = "", FString name = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_CreateActorMixer_Build(FString parentItemID = "", FString name = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_CreateEvent_Build(FString parentItemID = "", FString name = "", int actionType = 0, FString targetItemID = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_AddActionToEvent_Build(FString eventItemID, int actionType = 1, FString targetItemID = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_CreateObject_Build(FString parentItemID, FString name, FString objectType, bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_DeleteObject_Build(FString itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_SetReference_Build(FString itemID, FString propertyName, FString refItemID, FString platform = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_ExistsObject_Build(FString itemPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_AddObjectToSoundBank_Build(FString soundbankID, FString objectID, TArray<AZ_Wwise_FilterType> filterList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_RemoveObjectFromSoundBank_Build(FString soundbankID, FString objectID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_GenerateSoundBank_Build(TArray<FString> soundbankList, TArray<FString> platformListJson, TArray<FString> languageListJson);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_SaveProject_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_CopyObject_Build(FString targetObjectID, FString destObjectID, FString destName = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_SetRandomizer_Build(FString itemID, FString propertyName, bool enabled = true, bool bHasmin = false, double min = 0, bool bHasmax = false, double max = 0, FString platform = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise|Build")
	static URigdocksRpcCall* AZ_Wwise_HasChild_Build(FString parentID, FString itemIDorName, bool isRecursive = false);
};
