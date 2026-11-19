// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FWwiseStateGroup.h"
#include "Object/WwiseStateGroup.h"

using namespace RADNODZ_client::Object;

void FWwiseState::Initialize(WwiseState& obj)
{
	ID = UTF8_TO_TCHAR(obj.ID.c_str());
	Name = UTF8_TO_TCHAR(obj.Name.c_str());

	CustomStateName = UTF8_TO_TCHAR(obj.CustomStateName.c_str());
	CustomStateID = UTF8_TO_TCHAR(obj.CustomStateID.c_str());
	ShortID = UTF8_TO_TCHAR(obj.ShortID.c_str());
	BypassEffect = obj.BypassEffect;
	BypassEffect0 = obj.BypassEffect0;
	BypassEffect1 = obj.BypassEffect1;
	BypassEffect2 = obj.BypassEffect2;
	BypassEffect3 = obj.BypassEffect3;
	GameAuxSendHPF = obj.GameAuxSendHPF;
	GameAuxSendLPF = obj.GameAuxSendLPF;
	GameAuxSendVolume = obj.GameAuxSendVolume;
	Highpass = obj.Highpass;
	InitialDelay = obj.InitialDelay;
	Lowpass = obj.Lowpass;
	MakeUpGain = obj.MakeUpGain;
	MidiTransposition = obj.MidiTransposition;
	MidiVelocityOffset = obj.MidiVelocityOffset;
	OutputBusHighpass = obj.OutputBusHighpass;
	OutputBusLowpass = obj.OutputBusLowpass;
	OutputBusVolume = obj.OutputBusVolume;
	Pitch = obj.Pitch;
	Priority = obj.Priority;
	ReflectionsVolume = obj.ReflectionsVolume;
	UserAuxSendHPF0 = obj.UserAuxSendHPF0;
	UserAuxSendHPF1 = obj.UserAuxSendHPF1;
	UserAuxSendHPF2 = obj.UserAuxSendHPF2;
	UserAuxSendHPF3 = obj.UserAuxSendHPF3;
	UserAuxSendLPF0 = obj.UserAuxSendLPF0;
	UserAuxSendLPF1 = obj.UserAuxSendLPF1;
	UserAuxSendLPF2 = obj.UserAuxSendLPF2;
	UserAuxSendLPF3 = obj.UserAuxSendLPF3;
	UserAuxSendVolume0 = obj.UserAuxSendVolume0;
	UserAuxSendVolume1 = obj.UserAuxSendVolume1;
	UserAuxSendVolume2 = obj.UserAuxSendVolume2;
	UserAuxSendVolume3 = obj.UserAuxSendVolume3;
	Volume = obj.Volume;

}

WwiseState FWwiseState::GetCppObj() const
{
	WwiseState obj;

	obj.ID = TCHAR_TO_UTF8(*ID);
	obj.Name = TCHAR_TO_UTF8(*Name);

	obj.CustomStateName = TCHAR_TO_UTF8(*CustomStateName);
	obj.CustomStateID = TCHAR_TO_UTF8(*CustomStateID);
	obj.ShortID = TCHAR_TO_UTF8(*ShortID);
	obj.BypassEffect = BypassEffect;
	obj.BypassEffect0 = BypassEffect0;
	obj.BypassEffect1 = BypassEffect1;
	obj.BypassEffect2 = BypassEffect2;
	obj.BypassEffect3 = BypassEffect3;
	obj.GameAuxSendHPF = GameAuxSendHPF;
	obj.GameAuxSendLPF = GameAuxSendLPF;
	obj.GameAuxSendVolume = GameAuxSendVolume;
	obj.Highpass = Highpass;
	obj.InitialDelay = InitialDelay;
	obj.Lowpass = Lowpass;
	obj.MakeUpGain = MakeUpGain;
	obj.MidiTransposition = MidiTransposition;
	obj.MidiVelocityOffset = MidiVelocityOffset;
	obj.OutputBusHighpass = OutputBusHighpass;
	obj.OutputBusLowpass = OutputBusLowpass;
	obj.OutputBusVolume = OutputBusVolume;
	obj.Pitch = Pitch;
	obj.Priority = Priority;
	obj.ReflectionsVolume = ReflectionsVolume;
	obj.UserAuxSendHPF0 = UserAuxSendHPF0;
	obj.UserAuxSendHPF1 = UserAuxSendHPF1;
	obj.UserAuxSendHPF2 = UserAuxSendHPF2;
	obj.UserAuxSendHPF3 = UserAuxSendHPF3;
	obj.UserAuxSendLPF0 = UserAuxSendLPF0;
	obj.UserAuxSendLPF1 = UserAuxSendLPF1;
	obj.UserAuxSendLPF2 = UserAuxSendLPF2;
	obj.UserAuxSendLPF3 = UserAuxSendLPF3;
	obj.UserAuxSendVolume0 = UserAuxSendVolume0;
	obj.UserAuxSendVolume1 = UserAuxSendVolume1;
	obj.UserAuxSendVolume2 = UserAuxSendVolume2;
	obj.UserAuxSendVolume3 = UserAuxSendVolume3;
	obj.Volume = Volume;

	return obj;
}

bool UWwiseStateFactory::FromJson(const FString& Json, FWwiseState& object)
{
	auto cppObjPtr = WwiseState::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UWwiseStateFactory::FromJsonArray(const FString& Json, TArray<FWwiseState>& objectArray)
{
	auto cppObjArray = WwiseState::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FWwiseState obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UWwiseStateFactory::ToJson(const FWwiseState& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UWwiseStateFactory::ToJsonArray(const TArray<FWwiseState>& objectArray, int indent)
{
	std::vector<WwiseState> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = WwiseState::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}

void FWwiseStateGroup::Initialize(WwiseStateGroup& obj)
{
	ID = UTF8_TO_TCHAR(obj.ID.c_str());
	Name = UTF8_TO_TCHAR(obj.Name.c_str());

	for(auto& state : obj.State)
	{
		FWwiseState uState;
		uState.Initialize(state);
		State.Add(uState);
	}
}

WwiseStateGroup FWwiseStateGroup::GetCppObj() const
{
	WwiseStateGroup obj;

	obj.ID = TCHAR_TO_UTF8(*ID);
	obj.Name = TCHAR_TO_UTF8(*Name);

	for(auto& state : State)
	{
		obj.State.push_back(state.GetCppObj());
	}
	return obj;
}

bool UWwiseStateGroupFactory::FromJson(const FString& Json, FWwiseStateGroup& object)
{
	auto cppObjPtr = WwiseStateGroup::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UWwiseStateGroupFactory::FromJsonArray(const FString& Json, TArray<FWwiseStateGroup>& objectArray)
{
	auto cppObjArray = WwiseStateGroup::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FWwiseStateGroup obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UWwiseStateGroupFactory::ToJson(const FWwiseStateGroup& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UWwiseStateGroupFactory::ToJsonArray(const TArray<FWwiseStateGroup>& objectArray, int indent)
{
	std::vector<WwiseStateGroup> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = WwiseStateGroup::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
