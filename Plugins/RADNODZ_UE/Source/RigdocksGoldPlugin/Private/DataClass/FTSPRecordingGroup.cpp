// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FTSPRecordingGroup.h"
#include "Object/TSPRecordingGroup.h"
#include "JsonObjectConverter.h"
#include "CoreUtility.h"

using namespace RADNODZ_client::Object;

namespace
{
	TArray<FString> ToFStringArray(const std::vector<std::string>& values)
	{
		TArray<FString> res;
		for (const std::string& v : values)
		{
			res.Add(UTF8_TO_TCHAR(v.c_str()));
		}
		return res;
	}

	std::vector<std::string> ToStdStringArray(const TArray<FString>& values)
	{
		std::vector<std::string> res;
		for (const FString& v : values)
		{
			res.push_back(TCHAR_TO_UTF8(*v));
		}
		return res;
	}
}

void FTSPRecordingGroup::Initialize(TSPRecordingGroup& obj)
{
	FilePaths = ToFStringArray(obj.FilePaths);
	ReferenceFilePaths = ToFStringArray(obj.ReferenceFilePaths);
	AutoAlign = obj.AutoAlign;
}

TSPRecordingGroup FTSPRecordingGroup::GetCppObj() const
{
	TSPRecordingGroup obj;

	obj.FilePaths = ToStdStringArray(FilePaths);
	obj.ReferenceFilePaths = ToStdStringArray(ReferenceFilePaths);
	obj.AutoAlign = AutoAlign;

	return obj;
}

bool UTSPRecordingGroupFactory::FromJson(const FString& Json, FTSPRecordingGroup& object)
{
	auto cppObjPtr = TSPRecordingGroup::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UTSPRecordingGroupFactory::FromJsonArray(const FString& Json, TArray<FTSPRecordingGroup>& objectArray)
{
	auto cppObjArray = TSPRecordingGroup::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FTSPRecordingGroup obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UTSPRecordingGroupFactory::ToJson(const FTSPRecordingGroup& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UTSPRecordingGroupFactory::ToJsonArray(const TArray<FTSPRecordingGroup>& objectArray, int indent)
{
	std::vector<TSPRecordingGroup> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = TSPRecordingGroup::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
