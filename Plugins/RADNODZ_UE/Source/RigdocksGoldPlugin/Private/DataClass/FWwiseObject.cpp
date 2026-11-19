// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FWwiseObject.h"
#include "Object/WwiseObject.h"

using namespace RADNODZ_client::Object;

void FWwiseObject::Initialize(WwiseObject& obj)
{
	ID = UTF8_TO_TCHAR(obj.ID.c_str());
	Name = UTF8_TO_TCHAR(obj.Name.c_str());
	Path = UTF8_TO_TCHAR(obj.Path.c_str());

	Properties = TMap<FString, UVarType*>();
	for (const auto& item : obj.Properties)
	{
		UVarType* uVar = NewObject<UVarType>();
		VarType cppVar = item.second;
		uVar->Initialze(cppVar);

		Properties.Add(UTF8_TO_TCHAR(item.first.c_str()), uVar);
	}

}

WwiseObject FWwiseObject::GetCppObj() const
{
	WwiseObject obj;

	obj.ID = TCHAR_TO_UTF8(*ID);
	obj.Name = TCHAR_TO_UTF8(*Name);
	obj.Path = TCHAR_TO_UTF8(*Path);

	obj.Properties = std::map<std::string, VarType>();
	for (const auto& item : Properties)
	{
		FString key = item.Key;
		UVarType* uVar = item.Value;
		VarType cppVar = uVar->GetCppObj();
		obj.Properties.emplace(TCHAR_TO_UTF8(*key), cppVar);
	}

	return obj;
}

bool UWwiseObjectFactory::FromJson(const FString& Json, FWwiseObject& object)
{
	auto cppObjPtr = WwiseObject::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UWwiseObjectFactory::FromJsonArray(const FString& Json, TArray<FWwiseObject>& objectArray)
{
	auto cppObjArray = WwiseObject::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FWwiseObject obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UWwiseObjectFactory::ToJson(const FWwiseObject& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UWwiseObjectFactory::ToJsonArray(const TArray<FWwiseObject>& objectArray, int indent)
{
	std::vector<WwiseObject> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = WwiseObject::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
