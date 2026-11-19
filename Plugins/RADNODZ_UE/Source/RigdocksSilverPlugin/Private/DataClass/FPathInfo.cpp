// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FPathInfo.h"
#include "Object/PathInfo.h"

using namespace RADNODZ_client::Object;

void FPathInfo::Initialize(PathInfo& obj)
{
	ParentPath = UTF8_TO_TCHAR(obj.ParentPath.c_str());
	ParentName = UTF8_TO_TCHAR(obj.ParentName.c_str());
	FilePath = UTF8_TO_TCHAR(obj.FilePath.c_str());
	FileName = UTF8_TO_TCHAR(obj.FileName.c_str());
	FileNameWithoutExtension = UTF8_TO_TCHAR(obj.FileNameWithoutExtension.c_str());
	Extension = UTF8_TO_TCHAR(obj.Extension.c_str());
	Type = UTF8_TO_TCHAR(obj.Type.c_str());

}

PathInfo FPathInfo::GetCppObj() const
{
	PathInfo obj;

	obj.ParentPath = TCHAR_TO_UTF8(*ParentPath);
	obj.ParentName = TCHAR_TO_UTF8(*ParentName);
	obj.FilePath = TCHAR_TO_UTF8(*FilePath);
	obj.FileName = TCHAR_TO_UTF8(*FileName);
	obj.FileNameWithoutExtension = TCHAR_TO_UTF8(*FileNameWithoutExtension);
	obj.Extension = TCHAR_TO_UTF8(*Extension);
	obj.Type = TCHAR_TO_UTF8(*Type);

	return obj;
}

bool UPathInfoFactory::FromJson(const FString& Json, FPathInfo& object)
{
	auto cppObjPtr = PathInfo::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UPathInfoFactory::FromJsonArray(const FString& Json, TArray<FPathInfo>& objectArray)
{
	auto cppObjArray = PathInfo::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FPathInfo obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UPathInfoFactory::ToJson(const FPathInfo& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UPathInfoFactory::ToJsonArray(const TArray<FPathInfo>& objectArray, int indent)
{
	std::vector<PathInfo> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = PathInfo::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
