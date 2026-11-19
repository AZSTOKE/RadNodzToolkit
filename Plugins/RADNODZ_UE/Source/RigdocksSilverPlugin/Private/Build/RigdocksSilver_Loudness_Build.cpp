// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Loudness_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksSilver_Loudness_Build::AZ_GetMediaItemLoudnessMaxMomentary_Build(UMediaItem* item)
{
	RpcCall c = SILVER::AZ_GetMediaItemLoudnessMaxMomentary_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Loudness_Build::AZ_GetMediaIdLoudnessMaxMomentary_Build(UReaProject* proj, int itemID)
{
	RpcCall c = SILVER::AZ_GetMediaIdLoudnessMaxMomentary_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Loudness_Build::AZ_GetSelectedMediaIdLoudnessMaxMomentary_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = SILVER::AZ_GetSelectedMediaIdLoudnessMaxMomentary_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Loudness_Build::AZ_SetMediaItemLoudnessMaxMomentary_Build(UMediaItem* item, double setMaxMomentary)
{
	RpcCall c = SILVER::AZ_SetMediaItemLoudnessMaxMomentary_Build(UMediaItem::Get(item), setMaxMomentary);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Loudness_Build::AZ_SetMediaIdLoudnessMaxMomentary_Build(UReaProject* proj, int itemID, double setMaxMomentary)
{
	RpcCall c = SILVER::AZ_SetMediaIdLoudnessMaxMomentary_Build(UReaProject::Get(proj), itemID, setMaxMomentary);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Loudness_Build::AZ_SetSelectedMediaIdLoudnessMaxMomentary_Build(UReaProject* proj, int selectItemID, double setMaxMomentary)
{
	RpcCall c = SILVER::AZ_SetSelectedMediaIdLoudnessMaxMomentary_Build(UReaProject::Get(proj), selectItemID, setMaxMomentary);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Loudness_Build::AZ_GetSilentSectionListInMediaItem_Build(UMediaItem* item, double minSilentDulation, double threshold)
{
	RpcCall c = SILVER::AZ_GetSilentSectionListInMediaItem_Build(UMediaItem::Get(item), minSilentDulation, threshold);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Loudness_Build::AZ_GetAudioSectionListInMediaItem_Build(UMediaItem* item, double maxSilentDulation, double threshold)
{
	RpcCall c = SILVER::AZ_GetAudioSectionListInMediaItem_Build(UMediaItem::Get(item), maxSilentDulation, threshold);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
