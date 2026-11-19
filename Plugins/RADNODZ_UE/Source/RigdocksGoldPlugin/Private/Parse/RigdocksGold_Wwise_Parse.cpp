// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksGold_Wwise_Parse.h"
#include "RIGDOCKS_GOLD_Parse.h"
#include <string>
#include <vector>
#include <tuple>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

bool URigdocksGold_Wwise_Parse::AZ_Wwise_Connect_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return GOLD::AZ_Wwise_Connect_Parse(call->Get());
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_Disconnect_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_Disconnect_Parse(call->Get());
}

FString URigdocksGold_Wwise_Parse::AZ_Wwise_GetProjectName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string name = GOLD::AZ_Wwise_GetProjectName_Parse(call->Get());

	return UTF8_TO_TCHAR(name.c_str());
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_MoveObject_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_MoveObject_Parse(call->Get());
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_SetName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_SetName_Parse(call->Get());
}

TArray<FWwiseObject> URigdocksGold_Wwise_Parse::AZ_Wwise_GetSelectedObjectList_Parse(URigdocksRpcCall* call, int& count)
{
	TArray<FWwiseObject> result;
	count = 0;
	if (call == nullptr)
	{
		return result;
	}

	auto [objArray, tmpCount] = GOLD::AZ_Wwise_GetSelectedObjectList_Parse(call->Get());
	count = tmpCount;

	for (WwiseObject& obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);
		result.Add(uObj);
	}

	return result;
}

TArray<FWwiseObject> URigdocksGold_Wwise_Parse::AZ_Wwise_GetAllActorAncestorList_Parse(URigdocksRpcCall* call)
{
	TArray<FWwiseObject> result;
	if (call == nullptr)
	{
		return result;
	}

	std::vector<WwiseObject> objArray = GOLD::AZ_Wwise_GetAllActorAncestorList_Parse(call->Get());

	for (WwiseObject& obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);
		result.Add(uObj);
	}

	return result;
}

TArray<FWwiseObject> URigdocksGold_Wwise_Parse::AZ_Wwise_GetAllBusAncestorList_Parse(URigdocksRpcCall* call, int& count)
{
	TArray<FWwiseObject> result;
	count = 0;
	if (call == nullptr)
	{
		return result;
	}

	auto [objArray, tmpCount] = GOLD::AZ_Wwise_GetAllBusAncestorList_Parse(call->Get());
	count = tmpCount;

	for (WwiseObject& obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);
		result.Add(uObj);
	}

	return result;
}

TArray<FWwiseObject> URigdocksGold_Wwise_Parse::AZ_Wwise_GetUserAuxSend_Parse(URigdocksRpcCall* call, int& count)
{
	TArray<FWwiseObject> result;
	count = 0;
	if (call == nullptr)
	{
		return result;
	}

	auto [objArray, tmpCount] = GOLD::AZ_Wwise_GetUserAuxSend_Parse(call->Get());
	count = tmpCount;

	for (WwiseObject& obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);
		result.Add(uObj);
	}

	return result;
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_GetSwitchSet_Parse(URigdocksRpcCall* call, FWwiseObject& switchGroup, TArray<FWwiseObject>& switcheList, int& switchCount)
{
	switchGroup = FWwiseObject();
	switcheList = TArray<FWwiseObject>();
	switchCount = 0;
	if (call == nullptr)
	{
		return;
	}

	auto [switchGroupObj, switcheListVec, tmpSwitchCount] = GOLD::AZ_Wwise_GetSwitchSet_Parse(call->Get());
	switchCount = tmpSwitchCount;

	switchGroup.Initialize(switchGroupObj);

	for (WwiseObject& obj : switcheListVec)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);
		switcheList.Add(uObj);
	}
}

FWwisePropertyList URigdocksGold_Wwise_Parse::AZ_Wwise_GetPropertyList_Parse(URigdocksRpcCall* call)
{
	FWwisePropertyList uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	WwisePropertyList obj = GOLD::AZ_Wwise_GetPropertyList_Parse(call->Get());
	uObj.Initialize(obj);

	return uObj;
}

FWwiseObject URigdocksGold_Wwise_Parse::AZ_Wwise_CreateContainer_Parse(URigdocksRpcCall* call)
{
	FWwiseObject uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	WwiseObject obj = GOLD::AZ_Wwise_CreateContainer_Parse(call->Get());
	uObj.Initialize(obj);

	return uObj;
}

UVarType* URigdocksGold_Wwise_Parse::AZ_Wwise_GetProperty_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	VarType value = GOLD::AZ_Wwise_GetProperty_Parse(call->Get());

	UVarType* valueObj = NewObject<UVarType>();
	valueObj->Initialze(value);

	return valueObj;
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_SetProperty_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_SetProperty_Parse(call->Get());
}

TArray<FWwiseObject> URigdocksGold_Wwise_Parse::AZ_Wwise_SearchSoundItemFromName_Parse(URigdocksRpcCall* call)
{
	TArray<FWwiseObject> result;
	if (call == nullptr)
	{
		return result;
	}

	std::vector<WwiseObject> objArray = GOLD::AZ_Wwise_SearchSoundItemFromName_Parse(call->Get());

	for (WwiseObject& obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);
		result.Add(uObj);
	}

	return result;
}

TArray<FWwiseObject> URigdocksGold_Wwise_Parse::AZ_Wwise_GetChildrenList_Parse(URigdocksRpcCall* call)
{
	TArray<FWwiseObject> result;
	if (call == nullptr)
	{
		return result;
	}

	std::vector<WwiseObject> objArray = GOLD::AZ_Wwise_GetChildrenList_Parse(call->Get());

	for (WwiseObject& obj : objArray)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);
		result.Add(uObj);
	}

	return result;
}

double URigdocksGold_Wwise_Parse::AZ_Wwise_GetTotalVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return GOLD::AZ_Wwise_GetTotalVolume_Parse(call->Get());
}

FWwiseObject URigdocksGold_Wwise_Parse::AZ_Wwise_CreateSoundSFX_Parse(URigdocksRpcCall* call)
{
	FWwiseObject uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	WwiseObject obj = GOLD::AZ_Wwise_CreateSoundSFX_Parse(call->Get());
	uObj.Initialize(obj);

	return uObj;
}

FWwiseObject URigdocksGold_Wwise_Parse::AZ_Wwise_CreateSoundVoice_Parse(URigdocksRpcCall* call)
{
	FWwiseObject uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	WwiseObject obj = GOLD::AZ_Wwise_CreateSoundVoice_Parse(call->Get());
	uObj.Initialize(obj);

	return uObj;
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_ImportAudioFile_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_ImportAudioFile_Parse(call->Get());
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_GetStateSet_Parse(URigdocksRpcCall* call, FWwiseObject& stateGroup, TArray<FWwiseObject>& stateList)
{
	stateGroup = FWwiseObject();
	stateList = TArray<FWwiseObject>();
	if (call == nullptr)
	{
		return;
	}

	auto [stateGroupObj, stateListVec] = GOLD::AZ_Wwise_GetStateSet_Parse(call->Get());

	stateGroup.Initialize(stateGroupObj);

	for (WwiseObject& obj : stateListVec)
	{
		FWwiseObject uObj;
		uObj.Initialize(obj);
		stateList.Add(uObj);
	}
}

TArray<FWwiseStateGroup> URigdocksGold_Wwise_Parse::AZ_Wwise_GetStateModificationFromFile_Parse(URigdocksRpcCall* call)
{
	TArray<FWwiseStateGroup> result;
	if (call == nullptr)
	{
		return result;
	}

	std::vector<WwiseStateGroup> objArray = GOLD::AZ_Wwise_GetStateModificationFromFile_Parse(call->Get());

	for (WwiseStateGroup& obj : objArray)
	{
		FWwiseStateGroup uObj;
		uObj.Initialize(obj);
		result.Add(uObj);
	}

	return result;
}

TArray<FWwiseStateGroup> URigdocksGold_Wwise_Parse::AZ_Wwise_GetStateModification_Parse(URigdocksRpcCall* call)
{
	TArray<FWwiseStateGroup> result;
	if (call == nullptr)
	{
		return result;
	}

	std::vector<WwiseStateGroup> objArray = GOLD::AZ_Wwise_GetStateModification_Parse(call->Get());

	for (WwiseStateGroup& obj : objArray)
	{
		FWwiseStateGroup uObj;
		uObj.Initialize(obj);
		result.Add(uObj);
	}

	return result;
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_AssignToSwitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_AssignToSwitch_Parse(call->Get());
}

FWwiseObject URigdocksGold_Wwise_Parse::AZ_Wwise_CreateWorkUnit_Parse(URigdocksRpcCall* call)
{
	FWwiseObject uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	WwiseObject obj = GOLD::AZ_Wwise_CreateWorkUnit_Parse(call->Get());
	uObj.Initialize(obj);

	return uObj;
}

FWwiseObject URigdocksGold_Wwise_Parse::AZ_Wwise_CreateVirtualFolder_Parse(URigdocksRpcCall* call)
{
	FWwiseObject uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	WwiseObject obj = GOLD::AZ_Wwise_CreateVirtualFolder_Parse(call->Get());
	uObj.Initialize(obj);

	return uObj;
}

FWwiseObject URigdocksGold_Wwise_Parse::AZ_Wwise_CreateActorMixer_Parse(URigdocksRpcCall* call)
{
	FWwiseObject uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	WwiseObject obj = GOLD::AZ_Wwise_CreateActorMixer_Parse(call->Get());
	uObj.Initialize(obj);

	return uObj;
}

FWwiseObject URigdocksGold_Wwise_Parse::AZ_Wwise_CreateEvent_Parse(URigdocksRpcCall* call)
{
	FWwiseObject uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	WwiseObject obj = GOLD::AZ_Wwise_CreateEvent_Parse(call->Get());
	uObj.Initialize(obj);

	return uObj;
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_AddActionToEvent_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_AddActionToEvent_Parse(call->Get());
}

FWwiseObject URigdocksGold_Wwise_Parse::AZ_Wwise_CreateObject_Parse(URigdocksRpcCall* call)
{
	FWwiseObject uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	WwiseObject obj = GOLD::AZ_Wwise_CreateObject_Parse(call->Get());
	uObj.Initialize(obj);

	return uObj;
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_DeleteObject_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_DeleteObject_Parse(call->Get());
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_SetReference_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_SetReference_Parse(call->Get());
}

bool URigdocksGold_Wwise_Parse::AZ_Wwise_ExistsObject_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return GOLD::AZ_Wwise_ExistsObject_Parse(call->Get());
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_AddObjectToSoundBank_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_AddObjectToSoundBank_Parse(call->Get());
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_RemoveObjectFromSoundBank_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_RemoveObjectFromSoundBank_Parse(call->Get());
}

TArray<FString> URigdocksGold_Wwise_Parse::AZ_Wwise_GenerateSoundBank_Parse(URigdocksRpcCall* call)
{
	TArray<FString> result;
	if (call == nullptr)
	{
		return result;
	}

	std::vector<std::string> array = GOLD::AZ_Wwise_GenerateSoundBank_Parse(call->Get());

	for (const std::string& str : array)
	{
		result.Add(UTF8_TO_TCHAR(str.c_str()));
	}

	return result;
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_SaveProject_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_SaveProject_Parse(call->Get());
}

FWwiseObject URigdocksGold_Wwise_Parse::AZ_Wwise_CopyObject_Parse(URigdocksRpcCall* call)
{
	FWwiseObject uObj;
	if (call == nullptr)
	{
		return uObj;
	}

	WwiseObject obj = GOLD::AZ_Wwise_CopyObject_Parse(call->Get());
	uObj.Initialize(obj);

	return uObj;
}

void URigdocksGold_Wwise_Parse::AZ_Wwise_SetRandomizer_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Wwise_SetRandomizer_Parse(call->Get());
}

bool URigdocksGold_Wwise_Parse::AZ_Wwise_HasChild_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return GOLD::AZ_Wwise_HasChild_Parse(call->Get());
}
