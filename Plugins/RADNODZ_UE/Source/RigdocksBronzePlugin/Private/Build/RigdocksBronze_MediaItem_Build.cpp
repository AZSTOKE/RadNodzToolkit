// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksBronze_MediaItem_Build.h"
#include "RIGDOCKS_BRONZE_Build.h"
#include <string>
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemSelect_Build(UReaProject* proj, int itemID, int mediaType)
{
	RpcCall c = BRONZE::AZ_GetMediaItemSelect_Build(UReaProject::Get(proj), itemID, mediaType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaTrackItemSelect_Build(UReaProject* proj, int trackID, int itemID, int mediaType)
{
	RpcCall c = BRONZE::AZ_GetMediaTrackItemSelect_Build(UReaProject::Get(proj), trackID, itemID, mediaType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaTypeCount_Build(UReaProject* proj, FString mediaType)
{
	RpcCall c = BRONZE::AZ_GetMediaTypeCount_Build(UReaProject::Get(proj), TCHAR_TO_UTF8(*mediaType));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaTypeCount_Build(UReaProject* proj, FString mediaType)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaTypeCount_Build(UReaProject::Get(proj), TCHAR_TO_UTF8(*mediaType));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackItemMediaTypeCount_Build(UMediaTrack* track, FString mediaType)
{
	RpcCall c = BRONZE::AZ_GetTrackItemMediaTypeCount_Build(UMediaTrack::Get(track), TCHAR_TO_UTF8(*mediaType));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaTypeCount_Build(UReaProject* proj, int trackID, FString mediaType)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaTypeCount_Build(UReaProject::Get(proj), trackID, TCHAR_TO_UTF8(*mediaType));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaTypeCount_Build(UReaProject* proj, int selectTrackID, FString mediaType)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaTypeCount_Build(UReaProject::Get(proj), selectTrackID, TCHAR_TO_UTF8(*mediaType));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemBitRate_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemBitRate_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaBitRate_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaBitRate_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaBitRate_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaBitRate_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaBitRate_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaBitRate_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaBitRate_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaBitRate_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemSampleRate_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemSampleRate_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaSampleRate_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaSampleRate_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaSampleRate_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaSampleRate_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaSampleRate_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaSampleRate_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaSampleRate_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaSampleRate_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemNumChannels_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemNumChannels_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaNumChannels_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaNumChannels_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaNumChannels_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaNumChannels_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaNumChannels_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaNumChannels_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaNumChannels_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaNumChannels_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemChannelMode_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemChannelMode_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaChannelMode_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaChannelMode_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaChannelMode_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaChannelMode_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaChannelMode_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaChannelMode_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaChannelMode_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaChannelMode_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemChannelMode_Build(UMediaItem* item, int setChannelMode)
{
	RpcCall c = BRONZE::AZ_SetMediaItemChannelMode_Build(UMediaItem::Get(item), setChannelMode);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaChannelMode_Build(UReaProject* proj, int itemID, int setChannelMode)
{
	RpcCall c = BRONZE::AZ_SetMediaChannelMode_Build(UReaProject::Get(proj), itemID, setChannelMode);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaChannelMode_Build(UReaProject* proj, int selectItemID, int setChannelMode)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaChannelMode_Build(UReaProject::Get(proj), selectItemID, setChannelMode);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaChannelMode_Build(UReaProject* proj, int trackID, int trackItemID, int setChannelMode)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaChannelMode_Build(UReaProject::Get(proj), trackID, trackItemID, setChannelMode);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaChannelMode_Build(UReaProject* proj, int selectTrackID, int trackItemID, int setChannelMode)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaChannelMode_Build(UReaProject::Get(proj), selectTrackID, trackItemID, setChannelMode);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemName_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemName_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaName_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaName_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaName_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaName_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaName_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaName_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaName_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaName_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemName_Build(UMediaItem* item, FString setName)
{
	RpcCall c = BRONZE::AZ_SetMediaItemName_Build(UMediaItem::Get(item), TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaName_Build(UReaProject* proj, int itemID, FString setName)
{
	RpcCall c = BRONZE::AZ_SetMediaName_Build(UReaProject::Get(proj), itemID, TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaName_Build(UReaProject* proj, int selectItemID, FString setName)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaName_Build(UReaProject::Get(proj), selectItemID, TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaName_Build(UReaProject* proj, int trackID, int trackItemID, FString setName)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaName_Build(UReaProject::Get(proj), trackID, trackItemID, TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaName_Build(UReaProject* proj, int selectTrackID, int trackItemID, FString setName)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaName_Build(UReaProject::Get(proj), selectTrackID, trackItemID, TCHAR_TO_UTF8(*setName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemMute_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemMute_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaMute_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaMute_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaMute_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaMute_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaMute_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaMute_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaMute_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaMute_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemMute_Build(UMediaItem* item, bool setMute)
{
	RpcCall c = BRONZE::AZ_SetMediaItemMute_Build(UMediaItem::Get(item), setMute);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaMute_Build(UReaProject* proj, int itemID, bool setMute)
{
	RpcCall c = BRONZE::AZ_SetMediaMute_Build(UReaProject::Get(proj), itemID, setMute);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaMute_Build(UReaProject* proj, int selectItemID, bool setMute)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaMute_Build(UReaProject::Get(proj), selectItemID, setMute);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaMute_Build(UReaProject* proj, int trackID, int trackItemID, bool setMute)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaMute_Build(UReaProject::Get(proj), trackID, trackItemID, setMute);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaMute_Build(UReaProject* proj, int selectTrackID, int trackItemID, bool setType)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaMute_Build(UReaProject::Get(proj), selectTrackID, trackItemID, setType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemStartTimeSeconds_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemStartTimeSeconds_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaStartTimeSeconds_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaStartTimeSeconds_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaStartTimeSeconds_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaStartTimeSeconds_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaStartTimeSeconds_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaStartTimeSeconds_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaStartTimeSeconds_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaStartTimeSeconds_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemStartTimeSeconds_Build(UMediaItem* item, double setSecond)
{
	RpcCall c = BRONZE::AZ_SetMediaItemStartTimeSeconds_Build(UMediaItem::Get(item), setSecond);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaStartTimeSeconds_Build(UReaProject* proj, int itemID, double setSecond)
{
	RpcCall c = BRONZE::AZ_SetMediaStartTimeSeconds_Build(UReaProject::Get(proj), itemID, setSecond);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaStartTimeSeconds_Build(UReaProject* proj, int selectItemID, double setSecond)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaStartTimeSeconds_Build(UReaProject::Get(proj), selectItemID, setSecond);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaStartTimeSeconds_Build(UReaProject* proj, int trackID, int trackItemID, double setSecond)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaStartTimeSeconds_Build(UReaProject::Get(proj), trackID, trackItemID, setSecond);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaStartTimeSeconds_Build(UReaProject* proj, int selectTrackID, int trackItemID, double setSecond)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaStartTimeSeconds_Build(UReaProject::Get(proj), selectTrackID, trackItemID, setSecond);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemStartTimeFrame_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemStartTimeFrame_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaStartTimeFrame_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaStartTimeFrame_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaStartTimeFrame_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaStartTimeFrame_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaStartTimeFrame_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaStartTimeFrame_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaStartTimeFrame_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaStartTimeFrame_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemStartTimeFrame_Build(UMediaItem* item, double setFrame)
{
	RpcCall c = BRONZE::AZ_SetMediaItemStartTimeFrame_Build(UMediaItem::Get(item), setFrame);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaStartTimeFrame_Build(UReaProject* proj, int itemID, double setFrame)
{
	RpcCall c = BRONZE::AZ_SetMediaStartTimeFrame_Build(UReaProject::Get(proj), itemID, setFrame);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaStartTimeFrame_Build(UReaProject* proj, int selectItemID, double setFrame)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaStartTimeFrame_Build(UReaProject::Get(proj), selectItemID, setFrame);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaStartTimeFrame_Build(UReaProject* proj, int trackID, int trackItemID, double setFrame)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaStartTimeFrame_Build(UReaProject::Get(proj), trackID, trackItemID, setFrame);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaStartTimeFrame_Build(UReaProject* proj, int selectTrackID, int trackItemID, double setFrame)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaStartTimeFrame_Build(UReaProject::Get(proj), selectTrackID, trackItemID, setFrame);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemEndTime_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemEndTime_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaEndTime_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaEndTime_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaEndTime_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaEndTime_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaEndTime_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaEndTime_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaEndTime_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaEndTime_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemEndTimeFrame_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemEndTimeFrame_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaEndTimeFrame_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaEndTimeFrame_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaEndTimeFrame_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaEndTimeFrame_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaEndTimeFrame_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaEndTimeFrame_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaEndTimeFrame_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaEndTimeFrame_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemLength_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemLength_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaLength_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaLength_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaLength_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaLength_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaLength_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaLength_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaLength_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaLength_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemFade_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemFade_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaFade_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaFade_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaFade_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaFade_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaFade_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaFade_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaFade_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaFade_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemFade_Build(UMediaItem* item, int inType, double inLen, int outType, double outLen)
{
	RpcCall c = BRONZE::AZ_SetMediaItemFade_Build(UMediaItem::Get(item), inType, inLen, outType, outLen);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaFade_Build(UReaProject* proj, int itemID, int inType, double inLen, int outType, double outLen)
{
	RpcCall c = BRONZE::AZ_SetMediaFade_Build(UReaProject::Get(proj), itemID, inType, inLen, outType, outLen);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaFade_Build(UReaProject* proj, int selectItemID, int inType, double inLen, int outType, double outLen)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaFade_Build(UReaProject::Get(proj), selectItemID, inType, inLen, outType, outLen);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaFade_Build(UReaProject* proj, int trackID, int trackItemID, int inType, double inLen, int outType, double outLen)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaFade_Build(UReaProject::Get(proj), trackID, trackItemID, inType, inLen, outType, outLen);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaFade_Build(UReaProject* proj, int selectTrackID, int trackItemID, int inType, double inLen, int outType, double outLen)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaFade_Build(UReaProject::Get(proj), selectTrackID, trackItemID, inType, inLen, outType, outLen);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemVolume_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemVolume_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaVolume_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaVolume_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaVolume_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaVolume_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaVolume_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaVolume_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaVolume_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaVolume_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemVolume_Build(UMediaItem* item, double volume)
{
	RpcCall c = BRONZE::AZ_SetMediaItemVolume_Build(UMediaItem::Get(item), volume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaVolume_Build(UReaProject* proj, int itemID, double volume)
{
	RpcCall c = BRONZE::AZ_SetMediaVolume_Build(UReaProject::Get(proj), itemID, volume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaVolume_Build(UReaProject* proj, int itemID, double volume)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaVolume_Build(UReaProject::Get(proj), itemID, volume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaVolume_Build(UReaProject* proj, int trackID, int trackItemID, double volume)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaVolume_Build(UReaProject::Get(proj), trackID, trackItemID, volume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaVolume_Build(UReaProject* proj, int selectTrackID, int trackItemID, double volume)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaVolume_Build(UReaProject::Get(proj), selectTrackID, trackItemID, volume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemAddVolume_Build(UMediaItem* item, double addVolume)
{
	RpcCall c = BRONZE::AZ_SetMediaItemAddVolume_Build(UMediaItem::Get(item), addVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaAddVolume_Build(UReaProject* proj, int itemID, double addVolume)
{
	RpcCall c = BRONZE::AZ_SetMediaAddVolume_Build(UReaProject::Get(proj), itemID, addVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaAddVolume_Build(UReaProject* proj, int itemID, double addVolume)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaAddVolume_Build(UReaProject::Get(proj), itemID, addVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaAddVolume_Build(UReaProject* proj, int trackID, int trackItemID, double addVolume)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaAddVolume_Build(UReaProject::Get(proj), trackID, trackItemID, addVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaAddVolume_Build(UReaProject* proj, int selectTrackID, int trackItemID, double addVolume)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaAddVolume_Build(UReaProject::Get(proj), selectTrackID, trackItemID, addVolume);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemPitch_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemPitch_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaPitch_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaPitch_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaPitch_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaPitch_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaPitch_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaPitch_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaPitch_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaPitch_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemPitch_Build(UMediaItem* item, double setPitch)
{
	RpcCall c = BRONZE::AZ_SetMediaItemPitch_Build(UMediaItem::Get(item), setPitch);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaPitch_Build(UReaProject* proj, int itemID, double setPitch)
{
	RpcCall c = BRONZE::AZ_SetMediaPitch_Build(UReaProject::Get(proj), itemID, setPitch);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaPitch_Build(UReaProject* proj, int selectItemID, double setPitch)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaPitch_Build(UReaProject::Get(proj), selectItemID, setPitch);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaPitch_Build(UReaProject* proj, int trackID, int trackItemID, double setPitch)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaPitch_Build(UReaProject::Get(proj), trackID, trackItemID, setPitch);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaPitch_Build(UReaProject* proj, int selectTrackID, int trackItemID, double setPitch)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaPitch_Build(UReaProject::Get(proj), selectTrackID, trackItemID, setPitch);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemAddPitch_Build(UMediaItem* item, double addPitch)
{
	RpcCall c = BRONZE::AZ_SetMediaItemAddPitch_Build(UMediaItem::Get(item), addPitch);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaAddPitch_Build(UReaProject* proj, int itemID, double addPitch)
{
	RpcCall c = BRONZE::AZ_SetMediaAddPitch_Build(UReaProject::Get(proj), itemID, addPitch);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaAddPitch_Build(UReaProject* proj, int selectItemID, double addPitch)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaAddPitch_Build(UReaProject::Get(proj), selectItemID, addPitch);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaAddPitch_Build(UReaProject* proj, int trackID, int trackItemID, double addPitch)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaAddPitch_Build(UReaProject::Get(proj), trackID, trackItemID, addPitch);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaAddPitch_Build(UReaProject* proj, int selectTrackID, int trackItemID, double addPitch)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaAddPitch_Build(UReaProject::Get(proj), selectTrackID, trackItemID, addPitch);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemRate_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemRate_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaRate_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaRate_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaRate_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaRate_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaRate_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaRate_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaRate_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaRate_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemRate_Build(UMediaItem* item, double setRate)
{
	RpcCall c = BRONZE::AZ_SetMediaItemRate_Build(UMediaItem::Get(item), setRate);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaRate_Build(UReaProject* proj, int itemID, double setRate)
{
	RpcCall c = BRONZE::AZ_SetMediaRate_Build(UReaProject::Get(proj), itemID, setRate);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaRate_Build(UReaProject* proj, int selectItemID, double setRate)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaRate_Build(UReaProject::Get(proj), selectItemID, setRate);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaRate_Build(UReaProject* proj, int trackID, int trackItemID, double setRate)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaRate_Build(UReaProject::Get(proj), trackID, trackItemID, setRate);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaRate_Build(UReaProject* proj, int selectTrackID, int trackItemID, double setRate)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaRate_Build(UReaProject::Get(proj), selectTrackID, trackItemID, setRate);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_DeleteMediaItem_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_DeleteMediaItem_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_DeleteMedia_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_DeleteMedia_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_DeleteSelectedMedia_Build(UReaProject* proj, int selectItemID)
{
	RpcCall c = BRONZE::AZ_DeleteSelectedMedia_Build(UReaProject::Get(proj), selectItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_DeleteTrackMedia_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_DeleteTrackMedia_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_DeleteSelectedTrackMedia_Build(UReaProject* proj, int selectTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_DeleteSelectedTrackMedia_Build(UReaProject::Get(proj), selectTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_DeleteAllMedia_Build(UReaProject* proj)
{
	RpcCall c = BRONZE::AZ_DeleteAllMedia_Build(UReaProject::Get(proj));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_DeleteAllSelectedMedia_Build(UReaProject* proj)
{
	RpcCall c = BRONZE::AZ_DeleteAllSelectedMedia_Build(UReaProject::Get(proj));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_DeleteAllTrackMedia_Build(UReaProject* proj, int trackID)
{
	RpcCall c = BRONZE::AZ_DeleteAllTrackMedia_Build(UReaProject::Get(proj), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_DeleteAllSelectedTrackMedia_Build(UReaProject* proj, int selectTrackID)
{
	RpcCall c = BRONZE::AZ_DeleteAllSelectedTrackMedia_Build(UReaProject::Get(proj), selectTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaSelected_Build(UReaProject* proj, int itemID, bool selectType)
{
	RpcCall c = BRONZE::AZ_SetMediaSelected_Build(UReaProject::Get(proj), itemID, selectType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaSelected_Build(UReaProject* proj, int trackID, int trackItemID, bool selectType)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaSelected_Build(UReaProject::Get(proj), trackID, trackItemID, selectType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetAllMediaSelected_Build(UReaProject* proj, bool selectType)
{
	RpcCall c = BRONZE::AZ_SetAllMediaSelected_Build(UReaProject::Get(proj), selectType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemMoveToTrack_Build(UMediaItem* item, int moveTrackID)
{
	RpcCall c = BRONZE::AZ_SetMediaItemMoveToTrack_Build(UMediaItem::Get(item), moveTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaMoveToTrack_Build(UReaProject* proj, int itemID, int moveTrackID)
{
	RpcCall c = BRONZE::AZ_SetMediaMoveToTrack_Build(UReaProject::Get(proj), itemID, moveTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaMoveToTrack_Build(UReaProject* proj, int selectItemID, int moveTrackID)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaMoveToTrack_Build(UReaProject::Get(proj), selectItemID, moveTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaMoveToTrack_Build(UReaProject* proj, int trackID, int trackItemID, int moveTrackID)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaMoveToTrack_Build(UReaProject::Get(proj), trackID, trackItemID, moveTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaMoveToTrack_Build(UReaProject* proj, int selectTrackID, int trackItemID, int moveTrackID)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaMoveToTrack_Build(UReaProject::Get(proj), selectTrackID, trackItemID, moveTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemActiveTakeCopy_Build(UMediaItem* item, int setTrackID, double setPosition)
{
	RpcCall c = BRONZE::AZ_SetMediaItemActiveTakeCopy_Build(UMediaItem::Get(item), setTrackID, setPosition);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaActiveTakeCopy_Build(UReaProject* proj, int itemID, int setTrackID, double setPosition)
{
	RpcCall c = BRONZE::AZ_SetMediaActiveTakeCopy_Build(UReaProject::Get(proj), itemID, setTrackID, setPosition);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaActiveTakeCopy_Build(UReaProject* proj, int selectItemID, int setTrackID, double setPosition)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaActiveTakeCopy_Build(UReaProject::Get(proj), selectItemID, setTrackID, setPosition);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaActiveTakeCopy_Build(UReaProject* proj, int trackID, int trackItemID, int setTrackID, double setPosition)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaActiveTakeCopy_Build(UReaProject::Get(proj), trackID, trackItemID, setTrackID, setPosition);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaActiveTakeCopy_Build(UReaProject* proj, int selectTrackID, int trackItemID, int setTrackID, double setPosition)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaActiveTakeCopy_Build(UReaProject::Get(proj), selectTrackID, trackItemID, setTrackID, setPosition);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertMediaOutItem_Build(UReaProject* proj, FString filePath)
{
	RpcCall c = BRONZE::AZ_InsertMediaOutItem_Build(UReaProject::Get(proj), TCHAR_TO_UTF8(*filePath));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertMediaTrackItemSecond_Build(UMediaTrack* track, FString filePath, int startEndType, double setSecond)
{
	RpcCall c = BRONZE::AZ_InsertMediaTrackItemSecond_Build(UMediaTrack::Get(track), TCHAR_TO_UTF8(*filePath), startEndType, setSecond);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertMediaTrackIdSecond_Build(UReaProject* proj, int trackID, FString filePath, int startEndType, double setSecond)
{
	RpcCall c = BRONZE::AZ_InsertMediaTrackIdSecond_Build(UReaProject::Get(proj), trackID, TCHAR_TO_UTF8(*filePath), startEndType, setSecond);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertMediaSelectedTrackIdSecond_Build(UReaProject* proj, int selectTrackID, FString filePath, int startEndType, double setSecond)
{
	RpcCall c = BRONZE::AZ_InsertMediaSelectedTrackIdSecond_Build(UReaProject::Get(proj), selectTrackID, TCHAR_TO_UTF8(*filePath), startEndType, setSecond);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertMediaTrackItemFrame_Build(UMediaTrack* track, FString filePath, int startEndType, int setFrame)
{
	RpcCall c = BRONZE::AZ_InsertMediaTrackItemFrame_Build(UMediaTrack::Get(track), TCHAR_TO_UTF8(*filePath), startEndType, setFrame);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertMediaTrackIdFrame_Build(UReaProject* proj, int trackID, FString filePath, int startEndType, int setFrame)
{
	RpcCall c = BRONZE::AZ_InsertMediaTrackIdFrame_Build(UReaProject::Get(proj), trackID, TCHAR_TO_UTF8(*filePath), startEndType, setFrame);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertMediaSelectedTrackIdFrame_Build(UReaProject* proj, int selectTrackID, FString filePath, int startEndType, int setFrame)
{
	RpcCall c = BRONZE::AZ_InsertMediaSelectedTrackIdFrame_Build(UReaProject::Get(proj), selectTrackID, TCHAR_TO_UTF8(*filePath), startEndType, setFrame);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemList_Build(UReaProject* proj)
{
	RpcCall c = BRONZE::AZ_GetMediaItemList_Build(UReaProject::Get(proj));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaItemList_Build(UReaProject* proj)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaItemList_Build(UReaProject::Get(proj));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaItemList_Build(UReaProject* proj, int trackID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaItemList_Build(UReaProject::Get(proj), trackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaItemList_Build(UReaProject* proj, int SelectedTrackID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaItemList_Build(UReaProject::Get(proj), SelectedTrackID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemListInTrackItem_Build(UMediaTrack* track)
{
	RpcCall c = BRONZE::AZ_GetMediaItemListInTrackItem_Build(UMediaTrack::Get(track));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SelectMediaItems_Build(TArray<UMediaItem*> itemList, bool isSelected)
{
	std::vector<MediaItem*> objArray;
	for (auto uObj : itemList)
	{
		objArray.push_back((MediaItem*)uObj->GetPointer());
	}

	RpcCall c = BRONZE::AZ_SelectMediaItems_Build(objArray, isSelected);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertMediaItems_Build(UMediaTrack* track, TArray<FString> sourcePathList, double startPos, double interval, int timeUnitType)
{
	std::vector<std::string> pathArray;
	for (auto str : sourcePathList)
	{
		pathArray.push_back(TCHAR_TO_UTF8(*str));
	}

	RpcCall c = BRONZE::AZ_InsertMediaItems_Build(UMediaTrack::Get(track), pathArray, startPos, interval, timeUnitType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertNoteItemToTrack_Build(UMediaTrack* track, double startSecond, double length, FString text)
{
	RpcCall c = BRONZE::AZ_InsertNoteItemToTrack_Build(UMediaTrack::Get(track), startSecond, length, TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertNoteItemToTrackID_Build(UReaProject* proj, int trackID, double startSecond, double length, FString text)
{
	RpcCall c = BRONZE::AZ_InsertNoteItemToTrackID_Build(UReaProject::Get(proj), trackID, startSecond, length, TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertNoteItemToSelectedTrackID_Build(UReaProject* proj, int selectTrackID, double startSecond, double length, FString text)
{
	RpcCall c = BRONZE::AZ_InsertNoteItemToSelectedTrackID_Build(UReaProject::Get(proj), selectTrackID, startSecond, length, TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertNoteItemToTrackFrame_Build(UMediaTrack* track, double startFrame, double lengthFrame, FString text)
{
	RpcCall c = BRONZE::AZ_InsertNoteItemToTrackFrame_Build(UMediaTrack::Get(track), startFrame, lengthFrame, TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertNoteItemToTrackIDFrame_Build(UReaProject* proj, int trackID, double startFrame, double lengthFrame, FString text)
{
	RpcCall c = BRONZE::AZ_InsertNoteItemToTrackIDFrame_Build(UReaProject::Get(proj), trackID, startFrame, lengthFrame, TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_InsertNoteItemToSelectedTrackIDFrame_Build(UReaProject* proj, int selectTrackID, double startFrame, double lengthFrame, FString text)
{
	RpcCall c = BRONZE::AZ_InsertNoteItemToSelectedTrackIDFrame_Build(UReaProject::Get(proj), selectTrackID, startFrame, lengthFrame, TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemNotes_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemNotes_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaIDNotes_Build(UReaProject* proj, int itemID)
{
	RpcCall c = BRONZE::AZ_GetMediaIDNotes_Build(UReaProject::Get(proj), itemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaIDNotes_Build(UReaProject* proj, int selectedItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaIDNotes_Build(UReaProject::Get(proj), selectedItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetTrackMediaNotes_Build(UReaProject* proj, int trackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetTrackMediaNotes_Build(UReaProject::Get(proj), trackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedTrackMediaNotes_Build(UReaProject* proj, int selectedTrackID, int trackItemID)
{
	RpcCall c = BRONZE::AZ_GetSelectedTrackMediaNotes_Build(UReaProject::Get(proj), selectedTrackID, trackItemID);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaItemNotes_Build(UMediaItem* item, FString text)
{
	RpcCall c = BRONZE::AZ_SetMediaItemNotes_Build(UMediaItem::Get(item), TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetMediaIDNotes_Build(UReaProject* proj, int itemID, FString text)
{
	RpcCall c = BRONZE::AZ_SetMediaIDNotes_Build(UReaProject::Get(proj), itemID, TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedMediaIDNotes_Build(UReaProject* proj, int selectedItemID, FString text)
{
	RpcCall c = BRONZE::AZ_SetSelectedMediaIDNotes_Build(UReaProject::Get(proj), selectedItemID, TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetTrackMediaNotes_Build(UReaProject* proj, int trackID, int trackItemID, FString text)
{
	RpcCall c = BRONZE::AZ_SetTrackMediaNotes_Build(UReaProject::Get(proj), trackID, trackItemID, TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SetSelectedTrackMediaNotes_Build(UReaProject* proj, int selectedTrackID, int trackItemID, FString text)
{
	RpcCall c = BRONZE::AZ_SetSelectedTrackMediaNotes_Build(UReaProject::Get(proj), selectedTrackID, trackItemID, TCHAR_TO_UTF8(*text));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetSelectedMediaOverlapList_Build(UReaProject* proj)
{
	RpcCall c = BRONZE::AZ_GetSelectedMediaOverlapList_Build(UReaProject::Get(proj));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SplitMediaItem_Build(UMediaItem* item, double splitPosition)
{
	RpcCall c = BRONZE::AZ_SplitMediaItem_Build(UMediaItem::Get(item), splitPosition);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SplitMediaId_Build(UReaProject* proj, int itemID, double splitPosition)
{
	RpcCall c = BRONZE::AZ_SplitMediaId_Build(UReaProject::Get(proj), itemID, splitPosition);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SplitSelectedMediaId_Build(UReaProject* proj, int selectedItemID, double splitPosition)
{
	RpcCall c = BRONZE::AZ_SplitSelectedMediaId_Build(UReaProject::Get(proj), selectedItemID, splitPosition);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SplitTrackMedia_Build(UReaProject* proj, int trackID, int trackItemID, double splitPosition)
{
	RpcCall c = BRONZE::AZ_SplitTrackMedia_Build(UReaProject::Get(proj), trackID, trackItemID, splitPosition);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_SplitSelectedTrackMedia_Build(UReaProject* proj, int selectedTrackID, int trackItemID, double splitPosition)
{
	RpcCall c = BRONZE::AZ_SplitSelectedTrackMedia_Build(UReaProject::Get(proj), selectedTrackID, trackItemID, splitPosition);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_MediaItem_Build::AZ_GetMediaItemSourcePath_Build(UMediaItem* item)
{
	RpcCall c = BRONZE::AZ_GetMediaItemSourcePath_Build(UMediaItem::Get(item));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}