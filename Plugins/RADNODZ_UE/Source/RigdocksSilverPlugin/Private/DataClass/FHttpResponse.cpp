// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FHttpResponse.h"
#include "Object/HttpResponse.h"

using namespace RADNODZ_client::Object;

void FHttpResponse::Initialize(HttpResponse& obj)
{
	Body = UTF8_TO_TCHAR(obj.Body.c_str());
	Headers = TMap<FString, FString>();

	for (const auto& header : obj.Headers)
	{
		Headers.Add(UTF8_TO_TCHAR(header.first.c_str()), FString(header.second.c_str()));
	}

	StatusCode = obj.StatusCode;
}

HttpResponse FHttpResponse::GetCppObj() const
{
	HttpResponse obj;

	obj.Body = TCHAR_TO_UTF8(*Body);
	obj.StatusCode = StatusCode;

	for (const auto& header : Headers)
	{
		obj.Headers.emplace(TCHAR_TO_UTF8(*header.Key), TCHAR_TO_UTF8(*header.Value));
	}

	return obj;
}

bool UHttpResponseFactory::FromJson(const FString& Json, FHttpResponse& object)
{
	auto cppObjPtr = HttpResponse::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UHttpResponseFactory::FromJsonArray(const FString& Json, TArray<FHttpResponse>& objectArray)
{
	auto cppObjArray = HttpResponse::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FHttpResponse obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UHttpResponseFactory::ToJson(const FHttpResponse& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UHttpResponseFactory::ToJsonArray(const TArray<FHttpResponse>& objectArray, int indent)
{
	std::vector<HttpResponse> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = HttpResponse::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
