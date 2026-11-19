// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Http.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FHttpResponse.h"

using namespace RADNODZ_client::Object;

bool URigdocksSilver_Http::AZ_Http_Get(URadnodzClient* client, FString url, TMap<FString, FString> headers, TMap<FString, FString> queryParams, FHttpResponse& response)
{
	std::map<std::string, std::string> headerMap;
	for (const TPair<FString, FString>& header : headers)
	{
		headerMap.emplace(TCHAR_TO_UTF8(*header.Key), TCHAR_TO_UTF8(*header.Value));
	}

	std::map<std::string, std::string> queryParamsMap;
	for (const TPair<FString, FString>& query : queryParams)
	{
		queryParamsMap.emplace(TCHAR_TO_UTF8(*query.Key), TCHAR_TO_UTF8(*query.Value));
	}

    auto[res, httpResponse] = SILVER::AZ_Http_Get(client->GetClient(), TCHAR_TO_UTF8(*url), headerMap, queryParamsMap);

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}

bool URigdocksSilver_Http::AZ_Http_Post(URadnodzClient* client, FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams, FHttpResponse& response)
{
	std::map<std::string, std::string> headerMap;
	for (const TPair<FString, FString>& header : headers)
	{
		headerMap.emplace(TCHAR_TO_UTF8(*header.Key), TCHAR_TO_UTF8(*header.Value));
	}

	std::map<std::string, std::string> queryParamsMap;
	for (const TPair<FString, FString>& query : queryParams)
	{
		queryParamsMap.emplace(TCHAR_TO_UTF8(*query.Key), TCHAR_TO_UTF8(*query.Value));
	}

	auto [res, httpResponse] = SILVER::AZ_Http_Post(client->GetClient(), TCHAR_TO_UTF8(*url), headerMap, TCHAR_TO_UTF8(*body), queryParamsMap);

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}

bool URigdocksSilver_Http::AZ_Http_Put(URadnodzClient* client, FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams, FHttpResponse& response)
{
	std::map<std::string, std::string> headerMap;
	for (const TPair<FString, FString>& header : headers)
	{
		headerMap.emplace(TCHAR_TO_UTF8(*header.Key), TCHAR_TO_UTF8(*header.Value));
	}

	std::map<std::string, std::string> queryParamsMap;
	for (const TPair<FString, FString>& query : queryParams)
	{
		queryParamsMap.emplace(TCHAR_TO_UTF8(*query.Key), TCHAR_TO_UTF8(*query.Value));
	}

	auto [res, httpResponse] = SILVER::AZ_Http_Put(client->GetClient(), TCHAR_TO_UTF8(*url), headerMap, TCHAR_TO_UTF8(*body), queryParamsMap);

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}

bool URigdocksSilver_Http::AZ_Http_Patch(URadnodzClient* client, FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams, FHttpResponse& response)
{
	std::map<std::string, std::string> headerMap;
	for (const TPair<FString, FString>& header : headers)
	{
		headerMap.emplace(TCHAR_TO_UTF8(*header.Key), TCHAR_TO_UTF8(*header.Value));
	}

	std::map<std::string, std::string> queryParamsMap;
	for (const TPair<FString, FString>& query : queryParams)
	{
		queryParamsMap.emplace(TCHAR_TO_UTF8(*query.Key), TCHAR_TO_UTF8(*query.Value));
	}

	auto [res, httpResponse] = SILVER::AZ_Http_Patch(client->GetClient(), TCHAR_TO_UTF8(*url), headerMap, TCHAR_TO_UTF8(*body), queryParamsMap);

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}

bool URigdocksSilver_Http::AZ_Http_Delete(URadnodzClient* client, FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams, FHttpResponse& response)
{
	std::map<std::string, std::string> headerMap;
	for (const TPair<FString, FString>& header : headers)
	{
		headerMap.emplace(TCHAR_TO_UTF8(*header.Key), TCHAR_TO_UTF8(*header.Value));
	}

	std::map<std::string, std::string> queryParamsMap;
	for (const TPair<FString, FString>& query : queryParams)
	{
		queryParamsMap.emplace(TCHAR_TO_UTF8(*query.Key), TCHAR_TO_UTF8(*query.Value));
	}

	auto [res, httpResponse] = SILVER::AZ_Http_Delete(client->GetClient(), TCHAR_TO_UTF8(*url), headerMap, TCHAR_TO_UTF8(*body), queryParamsMap);

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}

bool URigdocksSilver_Http::AZ_Http_Options(URadnodzClient* client, FString url, TMap<FString, FString> headers, FHttpResponse& response)
{
	std::map<std::string, std::string> headerMap;
	for (const TPair<FString, FString>& header : headers)
	{
		headerMap.emplace(TCHAR_TO_UTF8(*header.Key), TCHAR_TO_UTF8(*header.Value));
	}

	auto [res, httpResponse] = SILVER::AZ_Http_Options(client->GetClient(), TCHAR_TO_UTF8(*url), headerMap);

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}

