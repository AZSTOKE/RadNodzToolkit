// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksGold_Wwise.h"
#include "RIGDOCKS_GOLD.h"

bool URigdocksGold_Wwise::AZ_Wwise_Connect(URadnodzClient* client, FString address, int port, int timeoutMs)
{
	return GOLD::AZ_Wwise_Connect(client->GetClient(), TCHAR_TO_UTF8(*address), port, timeoutMs);
}

void URigdocksGold_Wwise::AZ_Wwise_Disconnect(URadnodzClient* client)
{
	GOLD::AZ_Wwise_Disconnect(client->GetClient());
}

FString URigdocksGold_Wwise::AZ_Wwise_GetProjectName(URadnodzClient* client)
{
	std::string name = GOLD::AZ_Wwise_GetProjectName(client->GetClient());

	return UTF8_TO_TCHAR(name.c_str());
}

void URigdocksGold_Wwise::AZ_Wwise_MoveObject(URadnodzClient* client, FString itemID, FString destinationID)
{
	GOLD::AZ_Wwise_MoveObject(client->GetClient(), TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*destinationID));
}

void URigdocksGold_Wwise::AZ_Wwise_SetName(URadnodzClient* client, FString itemID, FString name)
{
	GOLD::AZ_Wwise_SetName(client->GetClient(), TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*name));
}

TArray<FWwiseObject> URigdocksGold_Wwise::AZ_Wwise_GetSelectedObjectList(URadnodzClient* client, int& count)
{

	auto[objArray, tmpCount] = GOLD::AZ_Wwise_GetSelectedObjectList(client->GetClient());
	count = tmpCount;

	TArray<FWwiseObject> result;

	for (auto obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FWwiseObject> URigdocksGold_Wwise::AZ_Wwise_GetAllActorAncestorList(URadnodzClient* client, FString itemID, TArray<FString> excludeList)
{
	std::vector<std::string > excludeListStr;
	for (auto str : excludeList)
	{
		excludeListStr.push_back(TCHAR_TO_UTF8(*str));
	}

	auto objArray = GOLD::AZ_Wwise_GetAllActorAncestorList(client->GetClient(), TCHAR_TO_UTF8(*itemID), excludeListStr);

	TArray<FWwiseObject> result;

	for (auto obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FWwiseObject> URigdocksGold_Wwise::AZ_Wwise_GetAllBusAncestorList(URadnodzClient* client, FString itemID, TArray<FString> excludeList, int& count)
{
	std::vector<std::string > excludeListStr;
	for (auto str : excludeList)
	{
		excludeListStr.push_back(TCHAR_TO_UTF8(*str));
	}

	auto [objArray, tmpCount] = GOLD::AZ_Wwise_GetAllBusAncestorList(client->GetClient(), TCHAR_TO_UTF8(*itemID), excludeListStr);
	count = tmpCount;

	TArray<FWwiseObject> result;

	for (auto obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FWwiseObject> URigdocksGold_Wwise::AZ_Wwise_GetUserAuxSend(URadnodzClient* client, FString itemID, int& count)
{

	auto [objArray, tmpCount] = GOLD::AZ_Wwise_GetUserAuxSend(client->GetClient(), TCHAR_TO_UTF8(*itemID));
	count = tmpCount;

	TArray<FWwiseObject> result;

	for (auto obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

void URigdocksGold_Wwise::AZ_Wwise_GetSwitchSet(URadnodzClient* client, FString switchGroupName, FWwiseObject& switchGroup, TArray<FWwiseObject>& switcheList, int& switchCount)
{

	auto [switchGroupObj, switcheListVec, tmpSwitchCount] = GOLD::AZ_Wwise_GetSwitchSet(client->GetClient(), TCHAR_TO_UTF8(*switchGroupName));
	switchCount = tmpSwitchCount;

	switcheList = TArray<FWwiseObject>();

	switchGroup = FWwiseObject();
	switchGroup.Initialize(switchGroupObj);

	for (auto obj : switcheListVec)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);

		switcheList.Add(uObj);
	}

}

FWwisePropertyList URigdocksGold_Wwise::AZ_Wwise_GetPropertyList(URadnodzClient* client, FString itemID, FString platform)
{

	auto obj = GOLD::AZ_Wwise_GetPropertyList(client->GetClient(), TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*platform));

	FWwisePropertyList uObj;
	uObj.Initialize(obj);

	return uObj;
}

FWwiseObject URigdocksGold_Wwise::AZ_Wwise_CreateContainer(URadnodzClient* client, FString parentItemID, AZ_Wwise_ContainerType type, FString name, bool isUnique)
{

	auto obj = GOLD::AZ_Wwise_CreateContainer(client->GetClient(), TCHAR_TO_UTF8(*parentItemID), (int)type, TCHAR_TO_UTF8(*name), isUnique);

	FWwiseObject uObj;
	uObj.Initialize(obj);

	return uObj;
}

	
UVarType* URigdocksGold_Wwise::AZ_Wwise_GetProperty(URadnodzClient* client, FString itemID, FString propertyName, FString language, FString platform)
{
	VarType value = GOLD::AZ_Wwise_GetProperty(client->GetClient(), TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*propertyName), TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*platform));

	UVarType* valueObj = NewObject<UVarType>();
	valueObj->Initialze(value);

	return valueObj;
}
	
void URigdocksGold_Wwise::AZ_Wwise_SetProperty(URadnodzClient* client, FString itemID, FString propertyName, UVarType* value, FString platform)
{
	GOLD::AZ_Wwise_SetProperty(client->GetClient(), TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*propertyName), value->GetCppObj(), TCHAR_TO_UTF8(*platform));
}

	
TArray<FWwiseObject> URigdocksGold_Wwise::AZ_Wwise_SearchSoundItemFromName(URadnodzClient* client, FString parentID, FString name, bool isPartialMatch)
{

	auto objArray = GOLD::AZ_Wwise_SearchSoundItemFromName(client->GetClient(), TCHAR_TO_UTF8(*parentID), TCHAR_TO_UTF8(*name), isPartialMatch);

	TArray<FWwiseObject> result;

	for (auto obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FWwiseObject> URigdocksGold_Wwise::AZ_Wwise_GetChildrenList(URadnodzClient* client, FString itemID, bool isRecursive)
{

	auto objArray = GOLD::AZ_Wwise_GetChildrenList(client->GetClient(), TCHAR_TO_UTF8(*itemID), isRecursive);

	TArray<FWwiseObject> result;

	for (auto obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

double URigdocksGold_Wwise::AZ_Wwise_GetTotalVolume(URadnodzClient* client, FString itemID, FString language, FMIDDLESCANNER_Options options)
{
	return GOLD::AZ_Wwise_GetTotalVolume(client->GetClient(), TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*language), options.GetCppObj());
}

FWwiseObject URigdocksGold_Wwise::AZ_Wwise_CreateSoundSFX(URadnodzClient* client, FString parentItemID, FString sourcePath, FString itemName, FString subFolder, bool isUnique)
{

	auto obj = GOLD::AZ_Wwise_CreateSoundSFX(client->GetClient(), TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*sourcePath), TCHAR_TO_UTF8(*itemName), TCHAR_TO_UTF8(*subFolder), isUnique);

	FWwiseObject uObj;
	uObj.Initialize(obj);

	return uObj;
}

FWwiseObject URigdocksGold_Wwise::AZ_Wwise_CreateSoundVoice(URadnodzClient* client, FString parentItemID, FString sourcePath, FString itemName, FString language, FString subFolder, bool isUnique)
{

	auto obj = GOLD::AZ_Wwise_CreateSoundVoice(client->GetClient(), TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*sourcePath), TCHAR_TO_UTF8(*itemName), TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*subFolder), isUnique);

	FWwiseObject uObj;
	uObj.Initialize(obj);

	return uObj;
}

void URigdocksGold_Wwise::AZ_Wwise_ImportAudioFile(URadnodzClient* client, FString itemID, FString sourcePath, FString language, FString subFolder)
{

	GOLD::AZ_Wwise_ImportAudioFile(client->GetClient(), TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*sourcePath), TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*subFolder));

}

void URigdocksGold_Wwise::AZ_Wwise_GetStateSet(URadnodzClient* client, FString stateGroupName, bool igroreNone, FWwiseObject& stateGroup, TArray<FWwiseObject>& stateList)
{

	auto [stateGroupObj, stateListVec] = GOLD::AZ_Wwise_GetStateSet(client->GetClient(), TCHAR_TO_UTF8(*stateGroupName));

	stateGroup = FWwiseObject();
	stateGroup.Initialize(stateGroupObj);

	stateList = TArray<FWwiseObject>();

	for (auto obj : stateListVec)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);

		stateList.Add(uObj);
	}

}

TArray<FWwiseStateGroup> URigdocksGold_Wwise::AZ_Wwise_GetStateModificationFromFile(URadnodzClient* client, FString itemID, FString workUnitPath, TArray<FString> targetStateGroup, TArray<FString> targetPropertyList)
{
	std::vector<std::string> targetStateGroupVec;
	for (auto str : targetStateGroup)
	{
		targetStateGroupVec.push_back(TCHAR_TO_UTF8(*str));
	}

	std::vector<std::string> targetPropertyListVec;
	for (auto str : targetPropertyList)
	{
		targetPropertyListVec.push_back(TCHAR_TO_UTF8(*str));
	}

	auto objArray = GOLD::AZ_Wwise_GetStateModificationFromFile(client->GetClient(), TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*workUnitPath), targetStateGroupVec, targetPropertyListVec);

	TArray<FWwiseStateGroup> result;

	for (auto obj : objArray)
	{
		FWwiseStateGroup uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

TArray<FWwiseStateGroup> URigdocksGold_Wwise::AZ_Wwise_GetStateModification(URadnodzClient* client, FString itemID, TArray<FString> targetStateGroup, TArray<FString> targetPropertyList)
{
	std::vector<std::string> targetStateGroupVec;
	for (auto str : targetStateGroup)
	{
		targetStateGroupVec.push_back(TCHAR_TO_UTF8(*str));
	}

	std::vector<std::string> targetPropertyListVec;
	for (auto str : targetPropertyList)
	{
		targetPropertyListVec.push_back(TCHAR_TO_UTF8(*str));
	}

	auto objArray = GOLD::AZ_Wwise_GetStateModification(client->GetClient(), TCHAR_TO_UTF8(*itemID), targetStateGroupVec, targetPropertyListVec);

	TArray<FWwiseStateGroup> result;

	for (auto obj : objArray)
	{
		FWwiseStateGroup uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;
}

void URigdocksGold_Wwise::AZ_Wwise_AssignToSwitch(URadnodzClient* client, FString assignedItemID, FString switchOrStateID, bool idDefault)
{

	GOLD::AZ_Wwise_AssignToSwitch(client->GetClient(), TCHAR_TO_UTF8(*assignedItemID), TCHAR_TO_UTF8(*switchOrStateID), idDefault);

}

FWwiseObject URigdocksGold_Wwise::AZ_Wwise_CreateWorkUnit(URadnodzClient* client, FString parentItemID, FString name, bool isUnique)
{

	auto obj = GOLD::AZ_Wwise_CreateWorkUnit(client->GetClient(), TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*name), isUnique);

	FWwiseObject uObj;
	uObj.Initialize(obj);

	return uObj;
}

FWwiseObject URigdocksGold_Wwise::AZ_Wwise_CreateVirtualFolder(URadnodzClient* client, FString parentItemID, FString name, bool isUnique)
{

	auto obj = GOLD::AZ_Wwise_CreateVirtualFolder(client->GetClient(), TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*name), isUnique);

	FWwiseObject uObj;
	uObj.Initialize(obj);

	return uObj;
}

FWwiseObject URigdocksGold_Wwise::AZ_Wwise_CreateActorMixer(URadnodzClient* client, FString parentItemID, FString name, bool isUnique)
{

	auto obj = GOLD::AZ_Wwise_CreateActorMixer(client->GetClient(), TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*name), isUnique);

	FWwiseObject uObj;
	uObj.Initialize(obj);

	return uObj;
}

FWwiseObject URigdocksGold_Wwise::AZ_Wwise_CreateEvent(URadnodzClient* client, FString parentItemID, FString name, int actionType, FString targetItemID, bool isUnique)
{

	auto obj = GOLD::AZ_Wwise_CreateEvent(client->GetClient(), TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*name), actionType, TCHAR_TO_UTF8(*targetItemID), isUnique);

	FWwiseObject uObj;
	uObj.Initialize(obj);

	return uObj;
}

void URigdocksGold_Wwise::AZ_Wwise_AddActionToEvent(URadnodzClient* client, FString eventItemID, int actionType, FString targetItemID)
{

	GOLD::AZ_Wwise_AddActionToEvent(client->GetClient(), TCHAR_TO_UTF8(*eventItemID), actionType, TCHAR_TO_UTF8(*targetItemID));

}

FWwiseObject URigdocksGold_Wwise::AZ_Wwise_CreateObject(URadnodzClient* client, FString parentItemID, FString name, FString objectType, bool isUnique)
{

	auto obj = GOLD::AZ_Wwise_CreateObject(client->GetClient(), TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*name), TCHAR_TO_UTF8(*objectType), isUnique);

	FWwiseObject uObj;
	uObj.Initialize(obj);

	return uObj;
}

void URigdocksGold_Wwise::AZ_Wwise_DeleteObject(URadnodzClient* client, FString itemID)
{

	GOLD::AZ_Wwise_DeleteObject(client->GetClient(), TCHAR_TO_UTF8(*itemID));

}

void URigdocksGold_Wwise::AZ_Wwise_SetReference(URadnodzClient* client, FString itemID, FString propertyName, FString refItemID, FString platform)
{

	GOLD::AZ_Wwise_SetReference(client->GetClient(), TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*propertyName), TCHAR_TO_UTF8(*refItemID), TCHAR_TO_UTF8(*platform));

}

bool URigdocksGold_Wwise::AZ_Wwise_ExistsObject(URadnodzClient* client, FString itemPath)
{

	return GOLD::AZ_Wwise_ExistsObject(client->GetClient(), TCHAR_TO_UTF8(*itemPath));

}

void URigdocksGold_Wwise::AZ_Wwise_AddObjectToSoundBank(URadnodzClient* client, FString soundbankID, FString objectID, TArray<AZ_Wwise_FilterType> filterList)
{

	std::vector<int> filterListVec;
	for (auto filter : filterList)
	{
		filterListVec.push_back((int)filter);
	}

	GOLD::AZ_Wwise_AddObjectToSoundBank(client->GetClient(), TCHAR_TO_UTF8(*soundbankID), TCHAR_TO_UTF8(*objectID), filterListVec);

}

void URigdocksGold_Wwise::AZ_Wwise_RemoveObjectFromSoundBank(URadnodzClient* client, FString soundbankID, FString objectID)
{

	GOLD::AZ_Wwise_RemoveObjectFromSoundBank(client->GetClient(), TCHAR_TO_UTF8(*soundbankID), TCHAR_TO_UTF8(*objectID));

}

TArray<FString> URigdocksGold_Wwise::AZ_Wwise_GenerateSoundBank(URadnodzClient* client, TArray<FString> soundbankList, TArray<FString> platformListJson, TArray<FString> languageListJson)
{
	std::vector<std::string> soundbankListVec;
	for (auto str : soundbankList)
	{
		soundbankListVec.push_back(TCHAR_TO_UTF8(*str));
	}

	std::vector<std::string> platformListVec;
	for (auto str : platformListJson)
	{
		platformListVec.push_back(TCHAR_TO_UTF8(*str));
	}

	std::vector<std::string> languageListVec;
	for (auto str : languageListJson)
	{
		languageListVec.push_back(TCHAR_TO_UTF8(*str));
	}

	auto array = GOLD::AZ_Wwise_GenerateSoundBank(client->GetClient(), soundbankListVec, platformListVec, languageListVec);

	TArray<FString> result;
	for (auto str : array)
	{
		result.Add(UTF8_TO_TCHAR(str.c_str()));
	}

	return result;
}

	
void URigdocksGold_Wwise::AZ_Wwise_SaveProject(URadnodzClient* client)
{

	GOLD::AZ_Wwise_SaveProject(client->GetClient());

}

FWwiseObject URigdocksGold_Wwise::AZ_Wwise_CopyObject(URadnodzClient* client, FString targetObjectID, FString destObjectID, FString destName, bool isUnique)
{

	auto obj = GOLD::AZ_Wwise_CopyObject(client->GetClient(), TCHAR_TO_UTF8(*targetObjectID), TCHAR_TO_UTF8(*destObjectID), TCHAR_TO_UTF8(*destName), isUnique);

	FWwiseObject uObj;
	uObj.Initialize(obj);

	return uObj;
}

void URigdocksGold_Wwise::AZ_Wwise_SetRandomizer(URadnodzClient* client, FString itemID, FString propertyName, bool enabled, bool bHasmin, double min, bool bHasmax, double max, FString platform)
{
	double* minPtr = bHasmin ? &min : nullptr;
	double* maxPtr = bHasmax ? &max : nullptr;

	GOLD::AZ_Wwise_SetRandomizer(client->GetClient(), TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*propertyName), enabled, minPtr, maxPtr, TCHAR_TO_UTF8(*platform));

}

bool URigdocksGold_Wwise::AZ_Wwise_HasChild(URadnodzClient* client, FString parentID, FString itemIDorName, bool isRecursive)
{

	return GOLD::AZ_Wwise_HasChild(client->GetClient(), TCHAR_TO_UTF8(*parentID), TCHAR_TO_UTF8(*itemIDorName), isRecursive);

}

