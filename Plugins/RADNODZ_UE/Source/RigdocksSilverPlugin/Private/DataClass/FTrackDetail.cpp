// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FTrackDetail.h"
#include "Object/TrackDetail.h"

using namespace RADNODZ_client::Object;

void FTrackDetail::Initialize(TrackDetail& obj)
{
	Name = UTF8_TO_TCHAR(obj.Name.c_str());

	Track = NewObject<UMediaTrack>();
	Track->SetPointer((void*)obj.Track);

	Volume = obj.Volume;
	Index = obj.Index;
	Depth = obj.Depth;

}

TrackDetail FTrackDetail::GetCppObj() const
{
	TrackDetail obj;

	obj.Name = TCHAR_TO_UTF8(*Name);

	obj.Track = UMediaTrack::Get(Track);

	obj.Volume = Volume;
	obj.Index = Index;
	obj.Depth = Depth;

	return obj;
}

bool UTrackDetailFactory::FromJson(const FString& Json, FTrackDetail& object)
{
	auto cppObjPtr = TrackDetail::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UTrackDetailFactory::FromJsonArray(const FString& Json, TArray<FTrackDetail>& objectArray)
{
	auto cppObjArray = TrackDetail::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FTrackDetail obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UTrackDetailFactory::ToJson(const FTrackDetail& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UTrackDetailFactory::ToJsonArray(const TArray<FTrackDetail>& objectArray, int indent)
{
	std::vector<TrackDetail> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = TrackDetail::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
