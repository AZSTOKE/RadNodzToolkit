// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Device_Build.h"
#include "RIGDOCKS_SILVER_Build.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksSilver_Device_Build::AZ_GetLocalIpAddresse_Build()
{
	RpcCall c = SILVER::AZ_GetLocalIpAddresse_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Device_Build::AZ_GetDeviceName_Build()
{
	RpcCall c = SILVER::AZ_GetDeviceName_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Device_Build::AZ_CountInputChannel_Build()
{
	RpcCall c = SILVER::AZ_CountInputChannel_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Device_Build::AZ_GetInputChannelList_Build()
{
	RpcCall c = SILVER::AZ_GetInputChannelList_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Device_Build::AZ_CountOutputChannel_Build()
{
	RpcCall c = SILVER::AZ_CountOutputChannel_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Device_Build::AZ_GetOutputChannelList_Build()
{
	RpcCall c = SILVER::AZ_GetOutputChannelList_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
