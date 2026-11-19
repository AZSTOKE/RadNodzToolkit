// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FLoudnessRecord.h"
#include "Object/LoudnessRecord.h"
#include "JsonObjectConverter.h"
#include "CoreUtility.h"

using namespace RADNODZ_client::Object;

void FLoudnessRecord::Initialize(LoudnessRecord& obj)
{
	Name = UTF8_TO_TCHAR(obj.Name.c_str());
	Loudness = obj.Loudness;
}

LoudnessRecord FLoudnessRecord::GetCppObj() const
{
	LoudnessRecord obj;

	obj.Name = TCHAR_TO_UTF8(*Name);
	obj.Loudness = Loudness;

	return obj;
}

bool ULoudnessRecordFactory::FromJson(const FString& Json, FLoudnessRecord& object)
{
	auto cppObjPtr = LoudnessRecord::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool ULoudnessRecordFactory::FromJsonArray(const FString& Json, TArray<FLoudnessRecord>& objectArray)
{
	auto cppObjArray = LoudnessRecord::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FLoudnessRecord obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString ULoudnessRecordFactory::ToJson(const FLoudnessRecord& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString ULoudnessRecordFactory::ToJsonArray(const TArray<FLoudnessRecord>& objectArray, int indent)
{
	std::vector<LoudnessRecord> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = LoudnessRecord::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
