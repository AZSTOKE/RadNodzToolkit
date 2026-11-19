// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FSection.h"
#include "Object/Section.h"

using namespace RADNODZ_client::Object;

void FSection::Initialize(Section& obj)
{
	StartTime = obj.StartTime;
	EndTime = obj.EndTime;
	Dulation = obj.Dulation;
}

Section FSection::GetCppObj() const
{
	Section obj;

	obj.StartTime = StartTime;
	obj.EndTime = EndTime;
	obj.Dulation = Dulation;

	return obj;
}

bool USectionFactory::FromJson(const FString& Json, FSection& object)
{
	auto cppObjPtr = Section::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool USectionFactory::FromJsonArray(const FString& Json, TArray<FSection>& objectArray)
{
	auto cppObjArray = Section::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FSection obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString USectionFactory::ToJson(const FSection& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString USectionFactory::ToJsonArray(const TArray<FSection>& objectArray, int indent)
{
	std::vector<Section> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = Section::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
