// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FMIDDLESCANNER_Options.h"
#include "Object/MIDDLESCANNER_Options.h"

using namespace RADNODZ_client::Object;

void FMIDDLESCANNER_Options::Initialize(MIDDLESCANNER_Options& obj)
{
	ExcludeBusList = TArray<FString>();
	for (const std::string& str : obj.ExcludeBusList)
	{
		ExcludeBusList.Add(UTF8_TO_TCHAR(str.c_str()));
	}

	ExcludeActorList = TArray<FString>();
	for (const std::string& str : obj.ExcludeActorList)
	{
		ExcludeActorList.Add(UTF8_TO_TCHAR(str.c_str()));
	}

	AddBusList = TArray<FBusItem>();
	for (const BusItem& busItem : obj.AddBusList)
	{
		FBusItem item;
		item.Name = UTF8_TO_TCHAR(busItem.Name.c_str());
		item.Volume = busItem.Volume;
		AddBusList.Add(item);
	}
}

MIDDLESCANNER_Options FMIDDLESCANNER_Options::GetCppObj() const
{
	MIDDLESCANNER_Options obj;

	obj.ExcludeBusList = std::vector<std::string>();
	for (const FString& str : ExcludeBusList)
	{
		obj.ExcludeBusList.push_back(TCHAR_TO_UTF8(*str));
	}

	obj.ExcludeActorList = std::vector<std::string>();
	for (const FString& str : ExcludeActorList)
	{
		obj.ExcludeActorList.push_back(TCHAR_TO_UTF8(*str));
	}

	obj.AddBusList = std::vector<BusItem>();
	for (const FBusItem& item : AddBusList)
	{
		BusItem busItem;
		busItem.Name = TCHAR_TO_UTF8(*item.Name);
		busItem.Volume = item.Volume;
		obj.AddBusList.push_back(busItem);
	}

	return obj;
}

bool UMIDDLESCANNER_OptionsFactory::FromJson(const FString& Json, FMIDDLESCANNER_Options& object)
{
	auto cppObjPtr = MIDDLESCANNER_Options::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UMIDDLESCANNER_OptionsFactory::FromJsonArray(const FString& Json, TArray<FMIDDLESCANNER_Options>& objectArray)
{
	auto cppObjArray = MIDDLESCANNER_Options::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FMIDDLESCANNER_Options obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UMIDDLESCANNER_OptionsFactory::ToJson(const FMIDDLESCANNER_Options& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UMIDDLESCANNER_OptionsFactory::ToJsonArray(const TArray<FMIDDLESCANNER_Options>& objectArray, int indent)
{
	std::vector<MIDDLESCANNER_Options> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = MIDDLESCANNER_Options::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
