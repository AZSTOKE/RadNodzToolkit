// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Http_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FHttpResponse.h"

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksSilver_Http_Build::AZ_Http_Get_Build(FString url, TMap<FString, FString> headers, TMap<FString, FString> queryParams)
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

	RpcCall c = SILVER::AZ_Http_Get_Build(TCHAR_TO_UTF8(*url), headerMap, queryParamsMap);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Http_Build::AZ_Http_Post_Build(FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams)
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

	RpcCall c = SILVER::AZ_Http_Post_Build(TCHAR_TO_UTF8(*url), headerMap, TCHAR_TO_UTF8(*body), queryParamsMap);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Http_Build::AZ_Http_Put_Build(FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams)
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

	RpcCall c = SILVER::AZ_Http_Put_Build(TCHAR_TO_UTF8(*url), headerMap, TCHAR_TO_UTF8(*body), queryParamsMap);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Http_Build::AZ_Http_Patch_Build(FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams)
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

	RpcCall c = SILVER::AZ_Http_Patch_Build(TCHAR_TO_UTF8(*url), headerMap, TCHAR_TO_UTF8(*body), queryParamsMap);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Http_Build::AZ_Http_Delete_Build(FString url, TMap<FString, FString> headers, FString body, TMap<FString, FString> queryParams)
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

	RpcCall c = SILVER::AZ_Http_Delete_Build(TCHAR_TO_UTF8(*url), headerMap, TCHAR_TO_UTF8(*body), queryParamsMap);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Http_Build::AZ_Http_Options_Build(FString url, TMap<FString, FString> headers)
{
	std::map<std::string, std::string> headerMap;
	for (const TPair<FString, FString>& header : headers)
	{
		headerMap.emplace(TCHAR_TO_UTF8(*header.Key), TCHAR_TO_UTF8(*header.Value));
	}

	RpcCall c = SILVER::AZ_Http_Options_Build(TCHAR_TO_UTF8(*url), headerMap);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
