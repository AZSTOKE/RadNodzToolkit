// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FMiddlewareInfo.h"
#include "Object/MiddlewareInfo.h"

using namespace RADNODZ_client::Object;

void FMiddlewareInfo::Initialize(MiddlewareInfo& obj)
{
	Item = NewObject<UMediaItem>();
	Item->SetPointer((void*)obj.Item);

	Volume = obj.Volume;
}

MiddlewareInfo FMiddlewareInfo::GetCppObj() const
{
	MiddlewareInfo obj;

	obj.Item = UMediaItem::Get(Item);
	obj.Volume = Volume;

	return obj;
}

bool UMiddlewareInfoFactory::FromJson(const FString& Json, FMiddlewareInfo& object)
{
	auto cppObjPtr = MiddlewareInfo::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UMiddlewareInfoFactory::FromJsonArray(const FString& Json, TArray<FMiddlewareInfo>& objectArray)
{
	auto cppObjArray = MiddlewareInfo::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FMiddlewareInfo obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UMiddlewareInfoFactory::ToJson(const FMiddlewareInfo& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UMiddlewareInfoFactory::ToJsonArray(const TArray<FMiddlewareInfo>& objectArray, int indent)
{
	std::vector<MiddlewareInfo> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = MiddlewareInfo::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
