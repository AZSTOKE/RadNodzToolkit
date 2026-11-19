// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksGold_LOUDMATCHIFY_Parse.h"
#include "RIGDOCKS_GOLD_Parse.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

void URigdocksGold_LOUDMATCHIFY_Parse::AZ_LOUDMATCHIFY_SetVolumeForMediaItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_LOUDMATCHIFY_SetVolumeForMediaItem_Parse(call->Get());
}

void URigdocksGold_LOUDMATCHIFY_Parse::AZ_LOUDMATCHIFY_SetVolumeForMediaId_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_LOUDMATCHIFY_SetVolumeForMediaId_Parse(call->Get());
}

void URigdocksGold_LOUDMATCHIFY_Parse::AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId_Parse(call->Get());
}

void URigdocksGold_LOUDMATCHIFY_Parse::AZ_LOUDMATCHIFY_SetVolumeForMediaItemList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_LOUDMATCHIFY_SetVolumeForMediaItemList_Parse(call->Get());
}
