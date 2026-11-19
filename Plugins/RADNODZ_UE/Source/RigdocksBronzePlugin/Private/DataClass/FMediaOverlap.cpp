// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FMediaOverlap.h"
#include "Object/MediaOverlap.h"
#include "JsonObjectConverter.h"

using namespace RADNODZ_client::Object;

void FMediaOverlap::Initialize(MediaOverlap& obj)
{
	Item = NewObject<UMediaItem>();
	Item->SetPointer((void*)obj.Item);

	Name = UTF8_TO_TCHAR(obj.Name.c_str());
	StartTime = obj.StartTime;
	EndTime = obj.EndTime;
}

MediaOverlap FMediaOverlap::GetCppObj() const
{
	MediaOverlap obj;

	obj.StartTime = StartTime;
	obj.EndTime = EndTime;
	obj.Name = TCHAR_TO_UTF8(*Name);
	obj.Item = UMediaItem::Get(Item);

	return obj;
}

bool UMediaOverlapFactory::FromJson(const FString& Json, FMediaOverlap& object)
{
	auto cppObjPtr = MediaOverlap::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UMediaOverlapFactory::FromJsonArray(const FString& Json, TArray<FMediaOverlap>& objectArray)
{
	auto cppObjArray = MediaOverlap::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FMediaOverlap obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UMediaOverlapFactory::ToJson(const FMediaOverlap& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UMediaOverlapFactory::ToJsonArray(const TArray<FMediaOverlap>& objectArray, int indent)
{
	std::vector<MediaOverlap> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = MediaOverlap::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
