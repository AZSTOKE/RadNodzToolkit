// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FPeak.h"
#include "Object/Peak.h"

using namespace RADNODZ_client::Object;

void FPeak::Initialize(Peak& obj)
{
	Time = obj.Time;
	MaxPeak = obj.MaxPeak;
	MinPeak = obj.MinPeak;
	Frequency = obj.Frequency;
	Pos = obj.Pos;
	PointVol = obj.PointVol;
}

Peak FPeak::GetCppObj() const
{
	Peak obj;

	obj.Time = Time;
	obj.MaxPeak = MaxPeak;
	obj.MinPeak = MinPeak;
	obj.Frequency = Frequency;
	obj.Pos = Pos;
	obj.PointVol = PointVol;

	return obj;
}

bool UPeakFactory::FromJson(const FString& Json, FPeak& object)
{
	auto cppObjPtr = Peak::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UPeakFactory::FromJsonArray(const FString& Json, TArray<FPeak>& objectArray)
{
	auto cppObjArray = Peak::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FPeak obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UPeakFactory::ToJson(const FPeak& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UPeakFactory::ToJsonArray(const TArray<FPeak>& objectArray, int indent)
{
	std::vector<Peak> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = Peak::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
