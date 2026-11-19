// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FMarker.h"
#include "Object/Marker.h"

using namespace RADNODZ_client::Object;

void FMarker::Initialize(RADNODZ_client::Object::Marker& obj)
{
	Index = obj.Index;
	Number = obj.Number;
	StartTime = obj.StartTime;
	EndTime = obj.EndTime;
	Color = obj.Color;
	Name = UTF8_TO_TCHAR(obj.Name.c_str());

}

RADNODZ_client::Object::Marker FMarker::GetCppObj() const
{
	RADNODZ_client::Object::Marker obj;

	obj.Index = Index;
	obj.Number = Number;
	obj.StartTime = StartTime;
	obj.EndTime = EndTime;
	obj.Color = Color;
	obj.Name = TCHAR_TO_UTF8(*Name);

	return obj;
}

bool UMarkerFactory::FromJson(const FString& Json, FMarker& object)
{
	auto cppObjPtr = RADNODZ_client::Object::Marker::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UMarkerFactory::FromJsonArray(const FString& Json, TArray<FMarker>& objectArray)
{
	auto cppObjArray = RADNODZ_client::Object::Marker::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FMarker obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UMarkerFactory::ToJson(const FMarker& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UMarkerFactory::ToJsonArray(const TArray<FMarker>& objectArray, int indent)
{
	std::vector<RADNODZ_client::Object::Marker> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = RADNODZ_client::Object::Marker::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
