// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FTrackSendProperties.h"
#include "Object/TrackSendProperties.h"

using namespace RADNODZ_client::Object;

void FTrackSendProperties::Initialize(TrackSendProperties& obj)
{

	Mute = obj.Mute;

	Phase = obj.Phase;

	Mono = obj.Mono;

	Volume = obj.Volume;

	VolDB = obj.VolDB;

	Pan = obj.Pan;

	SendMode = (AZ_TrackSendMode)obj.SendMode;

	Src_FirstChIndex = obj.Src_FirstChIndex;

	Src_NumChannels = obj.Src_NumChannels;

	Dst_FirstChIndex = obj.Dst_FirstChIndex;

	Dst_MixToMono = obj.Dst_MixToMono;

}

TrackSendProperties FTrackSendProperties::GetCppObj() const
{
	TrackSendProperties obj;

	obj.Mute = Mute;

	obj.Phase = Phase;

	obj.Mono = Mono;

	obj.Volume = Volume;

	obj.VolDB = VolDB;

	obj.Pan = Pan;

	obj.SendMode = (int)SendMode;

	obj.Src_FirstChIndex = Src_FirstChIndex;

	obj.Src_NumChannels = Src_NumChannels;

	obj.Dst_FirstChIndex = Dst_FirstChIndex;

	obj.Dst_MixToMono = Dst_MixToMono;

	obj.setMute = setMute;

	obj.setPhase = setPhase;

	obj.setMono = setMono;

	obj.setVolume = setVolume;

	obj.setVolDB = setVolDB;

	obj.setPan = setPan;

	obj.setSendMode = setSendMode;

	obj.setSrc_FirstChIndex = setSrc_FirstChIndex;

	obj.setSrc_NumChannels = setSrc_NumChannels;

	obj.setDst_FirstChIndex = setDst_FirstChIndex;

	obj.setDst_MixToMono = setDst_MixToMono;

	return obj;
}

bool UTrackSendPropertiesFactory::FromJson(const FString& Json, FTrackSendProperties& object)
{
	auto cppObjPtr = TrackSendProperties::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UTrackSendPropertiesFactory::FromJsonArray(const FString& Json, TArray<FTrackSendProperties>& objectArray)
{
	auto cppObjArray = TrackSendProperties::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FTrackSendProperties obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UTrackSendPropertiesFactory::ToJson(const FTrackSendProperties& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UTrackSendPropertiesFactory::ToJsonArray(const TArray<FTrackSendProperties>& objectArray, int indent)
{
	std::vector<TrackSendProperties> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = TrackSendProperties::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
