// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FWhisperSegment.h"
#include "Object/WhisperSegment.h"

using namespace RADNODZ_client::Object;

void FWhisperSegment::Initialize(WhisperSegment& obj)
{
	StartTime = obj.StartTime;
	EndTime = obj.EndTime;
	Text = UTF8_TO_TCHAR(obj.Text.c_str());
}

WhisperSegment FWhisperSegment::GetCppObj() const
{
	WhisperSegment obj;

	obj.StartTime = StartTime;
	obj.EndTime = EndTime;
	obj.Text = TCHAR_TO_UTF8(*Text);

	return obj;
}

bool UWhisperSegmentFactory::FromJson(const FString& Json, FWhisperSegment& object)
{
	auto cppObjPtr = WhisperSegment::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UWhisperSegmentFactory::FromJsonArray(const FString& Json, TArray<FWhisperSegment>& objectArray)
{
	auto cppObjArray = WhisperSegment::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FWhisperSegment obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UWhisperSegmentFactory::ToJson(const FWhisperSegment& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UWhisperSegmentFactory::ToJsonArray(const TArray<FWhisperSegment>& objectArray, int indent)
{
	std::vector<WhisperSegment> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = WhisperSegment::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
