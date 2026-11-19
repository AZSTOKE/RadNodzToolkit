// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Action_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksSilver_Action_Build::AZ_ActionMediaItemSelectKeep_Build(UMediaItem* item, int cmdID)
{
	RpcCall c = SILVER::AZ_ActionMediaItemSelectKeep_Build(UMediaItem::Get(item), cmdID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Action_Build::AZ_ActionMediaIdSelectKeep_Build(UReaProject* projID, int itemID, int cmdID)
{
	RpcCall c = SILVER::AZ_ActionMediaIdSelectKeep_Build(UReaProject::Get(projID), itemID, cmdID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Action_Build::AZ_ActionSelectedMediaIdSelectKeep_Build(UReaProject* projID, int selectItemID, int cmdID)
{
	RpcCall c = SILVER::AZ_ActionSelectedMediaIdSelectKeep_Build(UReaProject::Get(projID), selectItemID, cmdID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
