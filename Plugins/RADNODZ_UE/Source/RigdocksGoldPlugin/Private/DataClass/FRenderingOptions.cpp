// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FRenderingOptions.h"
#include "Object/RenderingOptions.h"

using namespace RADNODZ_client::Object;

void FRenderingOptions::Initialize(RenderingOptions& obj)
{
	OutputDir = UTF8_TO_TCHAR(obj.OutputDir.c_str());
	FileName = UTF8_TO_TCHAR(obj.FileName.c_str());
	RenderingTarget = obj.RenderingTarget;
	RenderingRangeType = obj.RenderingRangeType;
	StartPos = obj.StartPos;
	EndPos = obj.EndPos;
	SamplingRate = obj.SamplingRate;
	Channels = obj.Channels;
	TailMs = obj.TailMs;
	IsTail = obj.IsTail;
	Format = UTF8_TO_TCHAR(obj.Format.c_str());
	Format2 = UTF8_TO_TCHAR(obj.Format2.c_str());
	RenderingSetting = obj.RenderingSetting;
}

RenderingOptions FRenderingOptions::GetCppObj() const
{
	RenderingOptions obj;

	obj.OutputDir = TCHAR_TO_UTF8(*OutputDir);
	obj.FileName = TCHAR_TO_UTF8(*FileName);
	obj.RenderingTarget = RenderingTarget;
	obj.RenderingRangeType = RenderingRangeType;
	obj.StartPos = StartPos;
	obj.EndPos = EndPos;
	obj.SamplingRate = SamplingRate;
	obj.Channels = Channels;
	obj.TailMs = TailMs;
	obj.IsTail = IsTail;
	obj.Format = TCHAR_TO_UTF8(*Format);
	obj.Format2 = TCHAR_TO_UTF8(*Format2);
	obj.RenderingSetting = RenderingSetting;

	return obj;
}

bool URenderingOptionsFactory::FromJson(const FString& Json, FRenderingOptions& object)
{
	auto cppObjPtr = RenderingOptions::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool URenderingOptionsFactory::FromJsonArray(const FString& Json, TArray<FRenderingOptions>& objectArray)
{
	auto cppObjArray = RenderingOptions::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FRenderingOptions obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString URenderingOptionsFactory::ToJson(const FRenderingOptions& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString URenderingOptionsFactory::ToJsonArray(const TArray<FRenderingOptions>& objectArray, int indent)
{
	std::vector<RenderingOptions> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = RenderingOptions::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
