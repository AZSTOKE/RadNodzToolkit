// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksGold_Wwise_Build.h"
#include "RIGDOCKS_GOLD_Build.h"
#include <string>
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_Connect_Build(FString address, int port, int timeoutMs)
{
	RpcCall c = GOLD::AZ_Wwise_Connect_Build(TCHAR_TO_UTF8(*address), port, timeoutMs);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_Disconnect_Build()
{
	RpcCall c = GOLD::AZ_Wwise_Disconnect_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetProjectName_Build()
{
	RpcCall c = GOLD::AZ_Wwise_GetProjectName_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_MoveObject_Build(FString itemID, FString destinationID)
{
	RpcCall c = GOLD::AZ_Wwise_MoveObject_Build(TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*destinationID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_SetName_Build(FString itemID, FString name)
{
	RpcCall c = GOLD::AZ_Wwise_SetName_Build(TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*name));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetSelectedObjectList_Build()
{
	RpcCall c = GOLD::AZ_Wwise_GetSelectedObjectList_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetAllActorAncestorList_Build(FString itemID, TArray<FString> excludeList)
{
	std::vector<std::string> excludeListStr;
	for (const FString& str : excludeList)
	{
		excludeListStr.push_back(TCHAR_TO_UTF8(*str));
	}

	RpcCall c = GOLD::AZ_Wwise_GetAllActorAncestorList_Build(TCHAR_TO_UTF8(*itemID), excludeListStr);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetAllBusAncestorList_Build(FString itemID, TArray<FString> excludeList)
{
	std::vector<std::string> excludeListStr;
	for (const FString& str : excludeList)
	{
		excludeListStr.push_back(TCHAR_TO_UTF8(*str));
	}

	RpcCall c = GOLD::AZ_Wwise_GetAllBusAncestorList_Build(TCHAR_TO_UTF8(*itemID), excludeListStr);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetUserAuxSend_Build(FString itemID)
{
	RpcCall c = GOLD::AZ_Wwise_GetUserAuxSend_Build(TCHAR_TO_UTF8(*itemID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetSwitchSet_Build(FString switchGroupName)
{
	RpcCall c = GOLD::AZ_Wwise_GetSwitchSet_Build(TCHAR_TO_UTF8(*switchGroupName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetPropertyList_Build(FString itemID, FString platform)
{
	RpcCall c = GOLD::AZ_Wwise_GetPropertyList_Build(TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*platform));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_CreateContainer_Build(FString parentItemID, AZ_Wwise_ContainerType type, FString name, bool isUnique)
{
	RpcCall c = GOLD::AZ_Wwise_CreateContainer_Build(TCHAR_TO_UTF8(*parentItemID), (int)type, TCHAR_TO_UTF8(*name), isUnique);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetProperty_Build(FString itemID, FString propertyName, FString language, FString platform)
{
	RpcCall c = GOLD::AZ_Wwise_GetProperty_Build(TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*propertyName), TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*platform));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_SetProperty_Build(FString itemID, FString propertyName, UVarType* value, FString platform)
{
	RpcCall c = GOLD::AZ_Wwise_SetProperty_Build(TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*propertyName), value->GetCppObj(), TCHAR_TO_UTF8(*platform));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_SearchSoundItemFromName_Build(FString parentID, FString name, bool isPartialMatch)
{
	RpcCall c = GOLD::AZ_Wwise_SearchSoundItemFromName_Build(TCHAR_TO_UTF8(*parentID), TCHAR_TO_UTF8(*name), isPartialMatch);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetChildrenList_Build(FString itemID, bool isRecursive)
{
	RpcCall c = GOLD::AZ_Wwise_GetChildrenList_Build(TCHAR_TO_UTF8(*itemID), isRecursive);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetTotalVolume_Build(FString itemID, FString language, FMIDDLESCANNER_Options options)
{
	MIDDLESCANNER_Options cppOptions = options.GetCppObj();

	RpcCall c = GOLD::AZ_Wwise_GetTotalVolume_Build(TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*language), cppOptions);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_CreateSoundSFX_Build(FString parentItemID, FString sourcePath, FString itemName, FString subFolder, bool isUnique)
{
	RpcCall c = GOLD::AZ_Wwise_CreateSoundSFX_Build(TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*sourcePath), TCHAR_TO_UTF8(*itemName), TCHAR_TO_UTF8(*subFolder), isUnique);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_CreateSoundVoice_Build(FString parentItemID, FString sourcePath, FString itemName, FString language, FString subFolder, bool isUnique)
{
	RpcCall c = GOLD::AZ_Wwise_CreateSoundVoice_Build(TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*sourcePath), TCHAR_TO_UTF8(*itemName), TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*subFolder), isUnique);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_ImportAudioFile_Build(FString itemID, FString sourcePath, FString language, FString subFolder)
{
	RpcCall c = GOLD::AZ_Wwise_ImportAudioFile_Build(TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*sourcePath), TCHAR_TO_UTF8(*language), TCHAR_TO_UTF8(*subFolder));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetStateSet_Build(FString stateGroupName, bool igroreNone)
{
	RpcCall c = GOLD::AZ_Wwise_GetStateSet_Build(TCHAR_TO_UTF8(*stateGroupName), igroreNone);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetStateModificationFromFile_Build(FString itemID, FString workUnitPath, TArray<FString> targetStateGroup, TArray<FString> targetPropertyList)
{
	std::vector<std::string> targetStateGroupVec;
	for (const FString& str : targetStateGroup)
	{
		targetStateGroupVec.push_back(TCHAR_TO_UTF8(*str));
	}

	std::vector<std::string> targetPropertyListVec;
	for (const FString& str : targetPropertyList)
	{
		targetPropertyListVec.push_back(TCHAR_TO_UTF8(*str));
	}

	RpcCall c = GOLD::AZ_Wwise_GetStateModificationFromFile_Build(TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*workUnitPath), targetStateGroupVec, targetPropertyListVec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GetStateModification_Build(FString itemID, TArray<FString> targetStateGroup, TArray<FString> targetPropertyList)
{
	std::vector<std::string> targetStateGroupVec;
	for (const FString& str : targetStateGroup)
	{
		targetStateGroupVec.push_back(TCHAR_TO_UTF8(*str));
	}

	std::vector<std::string> targetPropertyListVec;
	for (const FString& str : targetPropertyList)
	{
		targetPropertyListVec.push_back(TCHAR_TO_UTF8(*str));
	}

	RpcCall c = GOLD::AZ_Wwise_GetStateModification_Build(TCHAR_TO_UTF8(*itemID), targetStateGroupVec, targetPropertyListVec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_AssignToSwitch_Build(FString assignedItemID, FString switchOrStateID, bool idDefault)
{
	RpcCall c = GOLD::AZ_Wwise_AssignToSwitch_Build(TCHAR_TO_UTF8(*assignedItemID), TCHAR_TO_UTF8(*switchOrStateID), idDefault);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_CreateWorkUnit_Build(FString parentItemID, FString name, bool isUnique)
{
	RpcCall c = GOLD::AZ_Wwise_CreateWorkUnit_Build(TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*name), isUnique);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_CreateVirtualFolder_Build(FString parentItemID, FString name, bool isUnique)
{
	RpcCall c = GOLD::AZ_Wwise_CreateVirtualFolder_Build(TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*name), isUnique);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_CreateActorMixer_Build(FString parentItemID, FString name, bool isUnique)
{
	RpcCall c = GOLD::AZ_Wwise_CreateActorMixer_Build(TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*name), isUnique);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_CreateEvent_Build(FString parentItemID, FString name, int actionType, FString targetItemID, bool isUnique)
{
	RpcCall c = GOLD::AZ_Wwise_CreateEvent_Build(TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*name), actionType, TCHAR_TO_UTF8(*targetItemID), isUnique);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_AddActionToEvent_Build(FString eventItemID, int actionType, FString targetItemID)
{
	RpcCall c = GOLD::AZ_Wwise_AddActionToEvent_Build(TCHAR_TO_UTF8(*eventItemID), actionType, TCHAR_TO_UTF8(*targetItemID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_CreateObject_Build(FString parentItemID, FString name, FString objectType, bool isUnique)
{
	RpcCall c = GOLD::AZ_Wwise_CreateObject_Build(TCHAR_TO_UTF8(*parentItemID), TCHAR_TO_UTF8(*name), TCHAR_TO_UTF8(*objectType), isUnique);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_DeleteObject_Build(FString itemID)
{
	RpcCall c = GOLD::AZ_Wwise_DeleteObject_Build(TCHAR_TO_UTF8(*itemID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_SetReference_Build(FString itemID, FString propertyName, FString refItemID, FString platform)
{
	RpcCall c = GOLD::AZ_Wwise_SetReference_Build(TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*propertyName), TCHAR_TO_UTF8(*refItemID), TCHAR_TO_UTF8(*platform));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_ExistsObject_Build(FString itemPath)
{
	RpcCall c = GOLD::AZ_Wwise_ExistsObject_Build(TCHAR_TO_UTF8(*itemPath));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_AddObjectToSoundBank_Build(FString soundbankID, FString objectID, TArray<AZ_Wwise_FilterType> filterList)
{
	std::vector<int> filterListVec;
	for (AZ_Wwise_FilterType filter : filterList)
	{
		filterListVec.push_back((int)filter);
	}

	RpcCall c = GOLD::AZ_Wwise_AddObjectToSoundBank_Build(TCHAR_TO_UTF8(*soundbankID), TCHAR_TO_UTF8(*objectID), filterListVec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_RemoveObjectFromSoundBank_Build(FString soundbankID, FString objectID)
{
	RpcCall c = GOLD::AZ_Wwise_RemoveObjectFromSoundBank_Build(TCHAR_TO_UTF8(*soundbankID), TCHAR_TO_UTF8(*objectID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_GenerateSoundBank_Build(TArray<FString> soundbankList, TArray<FString> platformListJson, TArray<FString> languageListJson)
{
	std::vector<std::string> soundbankListVec;
	for (const FString& str : soundbankList)
	{
		soundbankListVec.push_back(TCHAR_TO_UTF8(*str));
	}

	std::vector<std::string> platformListVec;
	for (const FString& str : platformListJson)
	{
		platformListVec.push_back(TCHAR_TO_UTF8(*str));
	}

	std::vector<std::string> languageListVec;
	for (const FString& str : languageListJson)
	{
		languageListVec.push_back(TCHAR_TO_UTF8(*str));
	}

	RpcCall c = GOLD::AZ_Wwise_GenerateSoundBank_Build(soundbankListVec, platformListVec, languageListVec);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_SaveProject_Build()
{
	RpcCall c = GOLD::AZ_Wwise_SaveProject_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_CopyObject_Build(FString targetObjectID, FString destObjectID, FString destName, bool isUnique)
{
	RpcCall c = GOLD::AZ_Wwise_CopyObject_Build(TCHAR_TO_UTF8(*targetObjectID), TCHAR_TO_UTF8(*destObjectID), TCHAR_TO_UTF8(*destName), isUnique);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_SetRandomizer_Build(FString itemID, FString propertyName, bool enabled, bool bHasmin, double min, bool bHasmax, double max, FString platform)
{
	double* minPtr = bHasmin ? &min : nullptr;
	double* maxPtr = bHasmax ? &max : nullptr;

	RpcCall c = GOLD::AZ_Wwise_SetRandomizer_Build(TCHAR_TO_UTF8(*itemID), TCHAR_TO_UTF8(*propertyName), enabled, minPtr, maxPtr, TCHAR_TO_UTF8(*platform));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Wwise_Build::AZ_Wwise_HasChild_Build(FString parentID, FString itemIDorName, bool isRecursive)
{
	RpcCall c = GOLD::AZ_Wwise_HasChild_Build(TCHAR_TO_UTF8(*parentID), TCHAR_TO_UTF8(*itemIDorName), isRecursive);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
