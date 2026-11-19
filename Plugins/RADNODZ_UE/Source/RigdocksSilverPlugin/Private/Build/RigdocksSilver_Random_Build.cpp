// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Random_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksSilver_Random_Build::AZ_GetRandomNumInteger_Build(int low, int high)
{
	RpcCall c = SILVER::AZ_GetRandomNumInteger_Build(low, high);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Random_Build::AZ_GetRandomNumDouble_Build(double low, double high, int doubleType)
{
	RpcCall c = SILVER::AZ_GetRandomNumDouble_Build(low, high, doubleType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Random_Build::AZ_GetRandomColor_Build()
{
	RpcCall c = SILVER::AZ_GetRandomColor_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
