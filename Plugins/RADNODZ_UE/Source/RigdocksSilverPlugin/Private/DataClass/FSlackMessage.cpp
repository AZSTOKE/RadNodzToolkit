// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FSlackMessage.h"
#include "Object/SlackMessage.h"

using namespace RADNODZ_client::Object;

void FSlackMessage::Initialize(SlackMessage& obj)
{
	Channel = UTF8_TO_TCHAR(obj.Channel.c_str());
	TimeStamp = UTF8_TO_TCHAR(obj.TimeStamp.c_str());
	ThreadTimeStamp = UTF8_TO_TCHAR(obj.ThreadTimeStamp.c_str());
	Message = UTF8_TO_TCHAR(obj.Message.c_str());
	Error = UTF8_TO_TCHAR(obj.Error.c_str());

}

SlackMessage FSlackMessage::GetCppObj() const
{
	SlackMessage obj;

	obj.Channel = TCHAR_TO_UTF8(*Channel);
	obj.TimeStamp = TCHAR_TO_UTF8(*TimeStamp);
	obj.ThreadTimeStamp = TCHAR_TO_UTF8(*ThreadTimeStamp);
	obj.Message = TCHAR_TO_UTF8(*Message);
	obj.Error = TCHAR_TO_UTF8(*Error);

	return obj;
}

bool USlackMessageFactory::FromJson(const FString& Json, FSlackMessage& object)
{
	auto cppObjPtr = SlackMessage::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool USlackMessageFactory::FromJsonArray(const FString& Json, TArray<FSlackMessage>& objectArray)
{
	auto cppObjArray = SlackMessage::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FSlackMessage obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString USlackMessageFactory::ToJson(const FSlackMessage& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString USlackMessageFactory::ToJsonArray(const TArray<FSlackMessage>& objectArray, int indent)
{
	std::vector<SlackMessage> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = SlackMessage::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
