// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FWwisePropertyList.h"
#include "Object/WwisePropertyList.h"

using namespace RADNODZ_client::Object;

void FWwisePropertyList::Initialize(WwisePropertyList& obj)
{
	ID = UTF8_TO_TCHAR(obj.ID.c_str());
	Name = UTF8_TO_TCHAR(obj.Name.c_str());
	Path = UTF8_TO_TCHAR(obj.Path.c_str());

	Volume = obj.Volume;
	Pitch = obj.Pitch;
	Lowpass = obj.Lowpass;
	Highpass = obj.Highpass;
	MakeUpGain = obj.MakeUpGain;
	OutputBusVolume = obj.OutputBusVolume;
	OutputBusLowpass = obj.OutputBusLowpass;
	OutputBusHighpass = obj.OutputBusHighpass;
	InitialDelay = obj.InitialDelay;
	IsStreamingEnabled = obj.IsStreamingEnabled;
	IsNonCachable = obj.IsNonCachable;
	IsZeroLatency = obj.IsZeroLatency;
	PreFetchLength = obj.PreFetchLength;
	VolumeOffset = obj.VolumeOffset;
	BusVolume = obj.BusVolume;
	OutputBusID = UTF8_TO_TCHAR(obj.OutputBusID.c_str());
	OutputBusName = UTF8_TO_TCHAR(obj.OutputBusName.c_str());

}

WwisePropertyList FWwisePropertyList::GetCppObj() const
{
	WwisePropertyList obj;

	obj.ID = TCHAR_TO_UTF8(*ID);
	obj.Name = TCHAR_TO_UTF8(*Name);
	obj.Path = TCHAR_TO_UTF8(*Path);

	obj.Volume = Volume;
	obj.Pitch = Pitch;
	obj.Lowpass = Lowpass;
	obj.Highpass = Highpass;
	obj.MakeUpGain = MakeUpGain;
	obj.OutputBusVolume = OutputBusVolume;
	obj.OutputBusLowpass = OutputBusLowpass;
	obj.OutputBusHighpass = OutputBusHighpass;
	obj.InitialDelay = InitialDelay;
	obj.IsStreamingEnabled = IsStreamingEnabled;
	obj.IsNonCachable = IsNonCachable;
	obj.IsZeroLatency = IsZeroLatency;
	obj.PreFetchLength = PreFetchLength;
	obj.VolumeOffset = VolumeOffset;
	obj.BusVolume = BusVolume;
	obj.OutputBusID = TCHAR_TO_UTF8(*OutputBusID);	
	obj.OutputBusName = TCHAR_TO_UTF8(*OutputBusName);

	return obj;
}

bool UWwisePropertyListFactory::FromJson(const FString& Json, FWwisePropertyList& object)
{
	auto cppObjPtr = WwisePropertyList::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UWwisePropertyListFactory::FromJsonArray(const FString& Json, TArray<FWwisePropertyList>& objectArray)
{
	auto cppObjArray = WwisePropertyList::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FWwisePropertyList obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UWwisePropertyListFactory::ToJson(const FWwisePropertyList& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UWwisePropertyListFactory::ToJsonArray(const TArray<FWwisePropertyList>& objectArray, int indent)
{
	std::vector<WwisePropertyList> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = WwisePropertyList::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
