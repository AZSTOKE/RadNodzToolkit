// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "ReaperClasses.h"
#include "RigdocksBronze_MediaItem_Build.generated.h"

UCLASS()
class RIGDOCKSBRONZEPLUGIN_API URigdocksBronze_MediaItem_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Build")
	static URigdocksRpcCall* AZ_GetMediaItemSelect_Build(UReaProject* proj, int itemID, int mediaType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Build")
	static URigdocksRpcCall* AZ_GetMediaTrackItemSelect_Build(UReaProject* proj, int trackID, int itemID, int mediaType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count|Build")
	static URigdocksRpcCall* AZ_GetMediaTypeCount_Build(UReaProject* proj, FString mediaType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaTypeCount_Build(UReaProject* proj, FString mediaType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count|Build")
	static URigdocksRpcCall* AZ_GetTrackItemMediaTypeCount_Build(UMediaTrack* track, FString mediaType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaTypeCount_Build(UReaProject* proj, int trackID, FString mediaType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaTypeCount_Build(UReaProject* proj, int selectTrackID, FString mediaType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth|Build")
	static URigdocksRpcCall* AZ_GetMediaItemBitRate_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth|Build")
	static URigdocksRpcCall* AZ_GetMediaBitRate_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaBitRate_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaBitRate_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaBitRate_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate|Build")
	static URigdocksRpcCall* AZ_GetMediaItemSampleRate_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate|Build")
	static URigdocksRpcCall* AZ_GetMediaSampleRate_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaSampleRate_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaSampleRate_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaSampleRate_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel|Build")
	static URigdocksRpcCall* AZ_GetMediaItemNumChannels_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel|Build")
	static URigdocksRpcCall* AZ_GetMediaNumChannels_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaNumChannels_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaNumChannels_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaNumChannels_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Build")
	static URigdocksRpcCall* AZ_GetMediaItemChannelMode_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Build")
	static URigdocksRpcCall* AZ_GetMediaChannelMode_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaChannelMode_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaChannelMode_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaChannelMode_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Build")
	static URigdocksRpcCall* AZ_SetMediaItemChannelMode_Build(UMediaItem* item, int setChannelMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Build")
	static URigdocksRpcCall* AZ_SetMediaChannelMode_Build(UReaProject* proj, int itemID, int setChannelMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaChannelMode_Build(UReaProject* proj, int selectItemID, int setChannelMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaChannelMode_Build(UReaProject* proj, int trackID, int trackItemID, int setChannelMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaChannelMode_Build(UReaProject* proj, int selectTrackID, int trackItemID, int setChannelMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Build")
	static URigdocksRpcCall* AZ_GetMediaItemName_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Build")
	static URigdocksRpcCall* AZ_GetMediaName_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaName_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaName_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaName_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Build")
	static URigdocksRpcCall* AZ_SetMediaItemName_Build(UMediaItem* item, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Build")
	static URigdocksRpcCall* AZ_SetMediaName_Build(UReaProject* proj, int itemID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaName_Build(UReaProject* proj, int selectItemID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaName_Build(UReaProject* proj, int trackID, int trackItemID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaName_Build(UReaProject* proj, int selectTrackID, int trackItemID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Build")
	static URigdocksRpcCall* AZ_GetMediaItemMute_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Build")
	static URigdocksRpcCall* AZ_GetMediaMute_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaMute_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaMute_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaMute_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Build")
	static URigdocksRpcCall* AZ_SetMediaItemMute_Build(UMediaItem* item, bool setType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Build")
	static URigdocksRpcCall* AZ_SetMediaMute_Build(UReaProject* proj, int itemID, bool setType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaMute_Build(UReaProject* proj, int selectItemID, bool setType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaMute_Build(UReaProject* proj, int trackID, int trackItemID, bool setType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaMute_Build(UReaProject* proj, int selectTrackID, int trackItemID, bool setType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_GetMediaItemStartTimeSeconds_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_GetMediaStartTimeSeconds_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaStartTimeSeconds_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaStartTimeSeconds_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaStartTimeSeconds_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_SetMediaItemStartTimeSeconds_Build(UMediaItem* item, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_SetMediaStartTimeSeconds_Build(UReaProject* proj, int itemID, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaStartTimeSeconds_Build(UReaProject* proj, int selectItemID, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaStartTimeSeconds_Build(UReaProject* proj, int trackID, int trackItemID, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaStartTimeSeconds_Build(UReaProject* proj, int selectTrackID, int trackItemID, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_GetMediaItemStartTimeFrame_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_GetMediaStartTimeFrame_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaStartTimeFrame_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaStartTimeFrame_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaStartTimeFrame_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_SetMediaItemStartTimeFrame_Build(UMediaItem* item, double setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_SetMediaStartTimeFrame_Build(UReaProject* proj, int itemID, double setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaStartTimeFrame_Build(UReaProject* proj, int selectItemID, double setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaStartTimeFrame_Build(UReaProject* proj, int trackID, int trackItemID, double setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaStartTimeFrame_Build(UReaProject* proj, int selectTrackID, int trackItemID, double setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_GetMediaItemEndTime_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_GetMediaEndTime_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaEndTime_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaEndTime_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaEndTime_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_GetMediaItemEndTimeFrame_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_GetMediaEndTimeFrame_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaEndTimeFrame_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaEndTimeFrame_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaEndTimeFrame_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length|Build")
	static URigdocksRpcCall* AZ_GetMediaItemLength_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length|Build")
	static URigdocksRpcCall* AZ_GetMediaLength_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaLength_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaLength_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaLength_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Build")
	static URigdocksRpcCall* AZ_GetMediaItemFade_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Build")
	static URigdocksRpcCall* AZ_GetMediaFade_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaFade_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaFade_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaFade_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Build")
	static URigdocksRpcCall* AZ_SetMediaItemFade_Build(UMediaItem* item, int inType, double inLen, int outType, double outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Build")
	static URigdocksRpcCall* AZ_SetMediaFade_Build(UReaProject* proj, int itemID, int inType, double inLen, int outType, double outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaFade_Build(UReaProject* proj, int selectItemID, int inType, double inLen, int outType, double outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaFade_Build(UReaProject* proj, int trackID, int trackItemID, int inType, double inLen, int outType, double outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaFade_Build(UReaProject* proj, int selectTrackID, int trackItemID, int inType, double inLen, int outType, double outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_GetMediaItemVolume_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_GetMediaVolume_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaVolume_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaVolume_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaVolume_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_SetMediaItemVolume_Build(UMediaItem* item, double volume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_SetMediaVolume_Build(UReaProject* proj, int itemID, double volume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaVolume_Build(UReaProject* proj, int itemID, double volume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaVolume_Build(UReaProject* proj, int trackID, int trackItemID, double volume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaVolume_Build(UReaProject* proj, int selectTrackID, int trackItemID, double volume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_SetMediaItemAddVolume_Build(UMediaItem* item, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_SetMediaAddVolume_Build(UReaProject* proj, int itemID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaAddVolume_Build(UReaProject* proj, int itemID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaAddVolume_Build(UReaProject* proj, int trackID, int trackItemID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaAddVolume_Build(UReaProject* proj, int selectTrackID, int trackItemID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_GetMediaItemPitch_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_GetMediaPitch_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaPitch_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaPitch_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaPitch_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_SetMediaItemPitch_Build(UMediaItem* item, double setPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_SetMediaPitch_Build(UReaProject* proj, int itemID, double setPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaPitch_Build(UReaProject* proj, int selectItemID, double setPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaPitch_Build(UReaProject* proj, int trackID, int trackItemID, double setPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaPitch_Build(UReaProject* proj, int selectTrackID, int trackItemID, double setPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_SetMediaItemAddPitch_Build(UMediaItem* item, double addPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_SetMediaAddPitch_Build(UReaProject* proj, int itemID, double addPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaAddPitch_Build(UReaProject* proj, int selectItemID, double addPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaAddPitch_Build(UReaProject* proj, int trackID, int trackItemID, double addPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaAddPitch_Build(UReaProject* proj, int selectTrackID, int trackItemID, double addPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Build")
	static URigdocksRpcCall* AZ_GetMediaItemRate_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Build")
	static URigdocksRpcCall* AZ_GetMediaRate_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaRate_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaRate_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaRate_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Build")
	static URigdocksRpcCall* AZ_SetMediaItemRate_Build(UMediaItem* item, double setRate);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Build")
	static URigdocksRpcCall* AZ_SetMediaRate_Build(UReaProject* proj, int itemID, double setRate);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaRate_Build(UReaProject* proj, int selectItemID, double setRate);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaRate_Build(UReaProject* proj, int trackID, int trackItemID, double setRate);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaRate_Build(UReaProject* proj, int selectTrackID, int trackItemID, double setRate);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Build")
	static URigdocksRpcCall* AZ_DeleteMediaItem_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Build")
	static URigdocksRpcCall* AZ_DeleteMedia_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Build")
	static URigdocksRpcCall* AZ_DeleteSelectedMedia_Build(UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Build")
	static URigdocksRpcCall* AZ_DeleteTrackMedia_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Build")
	static URigdocksRpcCall* AZ_DeleteSelectedTrackMedia_Build(UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Build")
	static URigdocksRpcCall* AZ_DeleteAllMedia_Build(UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Build")
	static URigdocksRpcCall* AZ_DeleteAllSelectedMedia_Build(UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Build")
	static URigdocksRpcCall* AZ_DeleteAllTrackMedia_Build(UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Build")
	static URigdocksRpcCall* AZ_DeleteAllSelectedTrackMedia_Build(UReaProject* proj, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select|Build")
	static URigdocksRpcCall* AZ_SetMediaSelected_Build(UReaProject* proj, int itemID, bool selectType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaSelected_Build(UReaProject* proj, int trackID, int trackItemID, bool selectType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select|Build")
	static URigdocksRpcCall* AZ_SetAllMediaSelected_Build(UReaProject* proj, bool selectType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move|Build")
	static URigdocksRpcCall* AZ_SetMediaItemMoveToTrack_Build(UMediaItem* item, int moveTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move|Build")
	static URigdocksRpcCall* AZ_SetMediaMoveToTrack_Build(UReaProject* proj, int itemID, int moveTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaMoveToTrack_Build(UReaProject* proj, int selectItemID, int moveTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaMoveToTrack_Build(UReaProject* proj, int trackID, int trackItemID, int moveTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaMoveToTrack_Build(UReaProject* proj, int selectTrackID, int trackItemID, int moveTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy|Build")
	static URigdocksRpcCall* AZ_SetMediaItemActiveTakeCopy_Build(UMediaItem* item, int setTrackID, double setPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy|Build")
	static URigdocksRpcCall* AZ_SetMediaActiveTakeCopy_Build(UReaProject* proj, int itemID, int setTrackID, double setPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaActiveTakeCopy_Build(UReaProject* proj, int selectItemID, int setTrackID, double setPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaActiveTakeCopy_Build(UReaProject* proj, int trackID, int trackItemID, int setTrackID, double setPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaActiveTakeCopy_Build(UReaProject* proj, int selectTrackID, int trackItemID, int setTrackID, double setPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Build")
	static URigdocksRpcCall* AZ_InsertMediaOutItem_Build(UReaProject* proj, FString filePath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Build")
	static URigdocksRpcCall* AZ_InsertMediaTrackItemSecond_Build(UMediaTrack* track, FString filePath, int startEndType, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Build")
	static URigdocksRpcCall* AZ_InsertMediaTrackIdSecond_Build(UReaProject* proj, int trackID, FString filePath, int startEndType, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Build")
	static URigdocksRpcCall* AZ_InsertMediaSelectedTrackIdSecond_Build(UReaProject* proj, int selectTrackID, FString filePath, int startEndType, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Build")
	static URigdocksRpcCall* AZ_InsertMediaTrackItemFrame_Build(UMediaTrack* track, FString filePath, int startEndType, int setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Build")
	static URigdocksRpcCall* AZ_InsertMediaTrackIdFrame_Build(UReaProject* proj, int trackID, FString filePath, int startEndType, int setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Build")
	static URigdocksRpcCall* AZ_InsertMediaSelectedTrackIdFrame_Build(UReaProject* proj, int selectTrackID, FString filePath, int startEndType, int setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Build")
	static URigdocksRpcCall* AZ_GetMediaItemList_Build(UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaItemList_Build(UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaItemList_Build(UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaItemList_Build(UReaProject* proj, int SelectedTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Build")
	static URigdocksRpcCall* AZ_GetMediaItemListInTrackItem_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select|Build")
	static URigdocksRpcCall* AZ_SelectMediaItems_Build(TArray<UMediaItem*> itemList, bool isSelected);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Build")
	static URigdocksRpcCall* AZ_InsertMediaItems_Build(UMediaTrack* track, TArray<FString> sourcePathList, double startPos, double interval, int timeUnitType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_InsertNoteItemToTrack_Build(UMediaTrack* track, double startSecond, double length, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_InsertNoteItemToTrackID_Build(UReaProject* proj, int trackID, double startSecond, double length, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_InsertNoteItemToSelectedTrackID_Build(UReaProject* proj, int selectTrackID, double startSecond, double length, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_InsertNoteItemToTrackFrame_Build(UMediaTrack* track, double startFrame, double lengthFrame, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_InsertNoteItemToTrackIDFrame_Build(UReaProject* proj, int trackID, double startFrame, double lengthFrame, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_InsertNoteItemToSelectedTrackIDFrame_Build(UReaProject* proj, int selectTrackID, double startFrame, double lengthFrame, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_GetMediaItemNotes_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_GetMediaIDNotes_Build(UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaIDNotes_Build(UReaProject* proj, int selectedItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_GetTrackMediaNotes_Build(UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackMediaNotes_Build(UReaProject* proj, int selectedTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_SetMediaItemNotes_Build(UMediaItem* item, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_SetMediaIDNotes_Build(UReaProject* proj, int itemID, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaIDNotes_Build(UReaProject* proj, int selectedItemID, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_SetTrackMediaNotes_Build(UReaProject* proj, int trackID, int trackItemID, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackMediaNotes_Build(UReaProject* proj, int selectedTrackID, int trackItemID, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Overlap|Build")
	static URigdocksRpcCall* AZ_GetSelectedMediaOverlapList_Build(UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split|Build")
	static URigdocksRpcCall* AZ_SplitMediaItem_Build(UMediaItem* item, double splitPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split|Build")
	static URigdocksRpcCall* AZ_SplitMediaId_Build(UReaProject* proj, int itemID, double splitPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split|Build")
	static URigdocksRpcCall* AZ_SplitSelectedMediaId_Build(UReaProject* proj, int selectedItemID, double splitPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split|Build")
	static URigdocksRpcCall* AZ_SplitTrackMedia_Build(UReaProject* proj, int trackID, int trackItemID, double splitPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split|Build")
	static URigdocksRpcCall* AZ_SplitSelectedTrackMedia_Build(UReaProject* proj, int selectedTrackID, int trackItemID, double splitPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Source|Build")
	static URigdocksRpcCall* AZ_GetMediaItemSourcePath_Build(UMediaItem* item);
};
