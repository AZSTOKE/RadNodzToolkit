// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Http_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FHttpResponse.h"

using namespace RADNODZ_client::Object;

bool URigdocksSilver_Http_Parse::AZ_Http_Get_Parse(URigdocksRpcCall* call, FHttpResponse& response)
{
	if (call == nullptr)
	{
		return false;
	}

	auto [res, httpResponse] = SILVER::AZ_Http_Get_Parse(call->Get());

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}

bool URigdocksSilver_Http_Parse::AZ_Http_Post_Parse(URigdocksRpcCall* call, FHttpResponse& response)
{
	if (call == nullptr)
	{
		return false;
	}

	auto [res, httpResponse] = SILVER::AZ_Http_Post_Parse(call->Get());

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}

bool URigdocksSilver_Http_Parse::AZ_Http_Put_Parse(URigdocksRpcCall* call, FHttpResponse& response)
{
	if (call == nullptr)
	{
		return false;
	}

	auto [res, httpResponse] = SILVER::AZ_Http_Put_Parse(call->Get());

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}

bool URigdocksSilver_Http_Parse::AZ_Http_Patch_Parse(URigdocksRpcCall* call, FHttpResponse& response)
{
	if (call == nullptr)
	{
		return false;
	}

	auto [res, httpResponse] = SILVER::AZ_Http_Patch_Parse(call->Get());

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}

bool URigdocksSilver_Http_Parse::AZ_Http_Delete_Parse(URigdocksRpcCall* call, FHttpResponse& response)
{
	if (call == nullptr)
	{
		return false;
	}

	auto [res, httpResponse] = SILVER::AZ_Http_Delete_Parse(call->Get());

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}

bool URigdocksSilver_Http_Parse::AZ_Http_Options_Parse(URigdocksRpcCall* call, FHttpResponse& response)
{
	if (call == nullptr)
	{
		return false;
	}

	auto [res, httpResponse] = SILVER::AZ_Http_Options_Parse(call->Get());

	response = FHttpResponse();
	response.Initialize(httpResponse);

	return res;
}
