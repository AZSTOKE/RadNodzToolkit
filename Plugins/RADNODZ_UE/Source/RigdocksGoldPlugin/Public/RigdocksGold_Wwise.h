// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "GoldClasses.h"
#include "DataClass/FWwiseObject.h"
#include "DataClass/FWwisePropertyList.h"
#include "DataClass/FWwiseStateGroup.h"
#include "DataClass/FMIDDLESCANNER_Options.h"
#include "RigdocksGold_Wwise.generated.h"

UENUM(BlueprintType)
enum class AZ_Wwise_ContainerType : uint8
{
	Random = 0,
	Sequence = 1,
	Switch = 2,
	Blend = 3,
};

UENUM(BlueprintType)
enum class AZ_Wwise_FilterType : uint8
{
	Events = 0,
	Structures = 1,
	Media = 2
};

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_Wwise : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_address = "127.0.0.1",
			CPP_Default_port = "8080",
			CPP_Default_timeoutMs = "5000"
			))
	static 	bool AZ_Wwise_Connect(URadnodzClient* client, FString address, int port, int timeoutMs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_Disconnect(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FString AZ_Wwise_GetProjectName(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_MoveObject(URadnodzClient* client, FString itemID, FString destinationID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_SetName(URadnodzClient* client, FString itemID, FString name);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FWwiseObject> AZ_Wwise_GetSelectedObjectList(URadnodzClient* client, int& count);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FWwiseObject> AZ_Wwise_GetAllActorAncestorList(URadnodzClient* client, FString itemID, TArray<FString> excludeList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FWwiseObject> AZ_Wwise_GetAllBusAncestorList(URadnodzClient* client, FString itemID, TArray<FString> excludeList, int& count);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FWwiseObject> AZ_Wwise_GetUserAuxSend(URadnodzClient* client, FString itemID, int& count);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_GetSwitchSet(URadnodzClient* client, FString switchGroupName, FWwiseObject& switchGroup, TArray<FWwiseObject>& switcheList, int& switchCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FWwisePropertyList AZ_Wwise_GetPropertyList(URadnodzClient* client, FString itemID, FString platform = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FWwiseObject AZ_Wwise_CreateContainer(URadnodzClient* client, FString parentItemID, AZ_Wwise_ContainerType type, FString name, bool isUnique);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
	meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	UVarType* AZ_Wwise_GetProperty(URadnodzClient* client, FString itemID, FString propertyName, FString language = "", FString platform = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_SetProperty(URadnodzClient* client, FString itemID, FString propertyName, UVarType* value, FString platform = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FWwiseObject> AZ_Wwise_SearchSoundItemFromName(URadnodzClient* client, FString parentID, FString name, bool isPartialMatch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FWwiseObject> AZ_Wwise_GetChildrenList(URadnodzClient* client, FString itemID, bool isRecursive);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	double AZ_Wwise_GetTotalVolume(URadnodzClient* client, FString itemID, FString language, FMIDDLESCANNER_Options options);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FWwiseObject AZ_Wwise_CreateSoundSFX(URadnodzClient* client, FString parentItemID, FString sourcePath = "", FString itemName = "", FString subFolder = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FWwiseObject AZ_Wwise_CreateSoundVoice(URadnodzClient* client, FString parentItemID, FString sourcePath = "", FString itemName = "", FString language = "", FString subFolder = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_ImportAudioFile(URadnodzClient* client, FString itemID, FString sourcePath, FString language = "", FString subFolder = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_GetStateSet(URadnodzClient* client, FString stateGroupName, bool igroreNone, FWwiseObject& stateGroup, TArray<FWwiseObject>& stateList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FWwiseStateGroup> AZ_Wwise_GetStateModificationFromFile(URadnodzClient* client, FString itemID, FString workUnitPath, TArray<FString> targetStateGroup, TArray<FString> targetPropertyList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FWwiseStateGroup> AZ_Wwise_GetStateModification(URadnodzClient* client, FString itemID, TArray<FString> targetStateGroup, TArray<FString> targetPropertyList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_AssignToSwitch(URadnodzClient* client, FString assignedItemID, FString switchOrStateID, bool idDefault = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FWwiseObject AZ_Wwise_CreateWorkUnit(URadnodzClient* client, FString parentItemID = "", FString name = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FWwiseObject AZ_Wwise_CreateVirtualFolder(URadnodzClient* client, FString parentItemID = "", FString name = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FWwiseObject AZ_Wwise_CreateActorMixer(URadnodzClient* client, FString parentItemID = "", FString name = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FWwiseObject AZ_Wwise_CreateEvent(URadnodzClient* client, FString parentItemID = "", FString name = "", int actionType = 0, FString targetItemID = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_AddActionToEvent(URadnodzClient* client, FString eventItemID, int actionType = 1, FString targetItemID = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FWwiseObject AZ_Wwise_CreateObject(URadnodzClient* client, FString parentItemID, FString name, FString objectType, bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_DeleteObject(URadnodzClient* client, FString itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_SetReference(URadnodzClient* client, FString itemID, FString propertyName, FString refItemID, FString platform = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	bool AZ_Wwise_ExistsObject(URadnodzClient* client, FString itemPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_filterList = "(Events, Structures, Media)"
			))
	static 	void AZ_Wwise_AddObjectToSoundBank(URadnodzClient* client, FString soundbankID, FString objectID, TArray<AZ_Wwise_FilterType> filterList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
	meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_RemoveObjectFromSoundBank(URadnodzClient* client, FString soundbankID, FString objectID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
	meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	TArray<FString> AZ_Wwise_GenerateSoundBank(URadnodzClient* client, TArray<FString> soundbankList, TArray<FString> platformListJson, TArray<FString> languageListJson);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
	meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_SaveProject(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	FWwiseObject AZ_Wwise_CopyObject(URadnodzClient* client, FString targetObjectID, FString destObjectID, FString destName = "", bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_Wwise_SetRandomizer(URadnodzClient* client, FString itemID, FString propertyName, bool enabled = true, bool bHasmin = false, double min = 0, bool bHasmax = false, double max = 0, FString platform = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|Wwise",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	bool AZ_Wwise_HasChild(URadnodzClient* client, FString parentID, FString itemIDorName, bool isRecursive = false);

};
