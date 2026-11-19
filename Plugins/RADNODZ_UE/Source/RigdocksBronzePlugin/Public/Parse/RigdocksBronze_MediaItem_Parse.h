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
#include "DataClass/FMediaOverlap.h"
#include "RigdocksBronze_MediaItem_Parse.generated.h"

UCLASS()
class RIGDOCKSBRONZEPLUGIN_API URigdocksBronze_MediaItem_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Parse")
	static UMediaItem* AZ_GetMediaItemSelect_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Parse")
	static UMediaItem* AZ_GetMediaTrackItemSelect_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count|Parse")
	static void AZ_GetMediaTypeCount_Parse(URigdocksRpcCall* call, int& allCount, int& typeCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count|Parse")
	static void AZ_GetSelectedMediaTypeCount_Parse(URigdocksRpcCall* call, int& allCount, int& typeCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count|Parse")
	static void AZ_GetTrackItemMediaTypeCount_Parse(URigdocksRpcCall* call, int& allCount, int& typeCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count|Parse")
	static void AZ_GetTrackMediaTypeCount_Parse(URigdocksRpcCall* call, int& allCount, int& typeCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count|Parse")
	static void AZ_GetSelectedTrackMediaTypeCount_Parse(URigdocksRpcCall* call, int& allCount, int& typeCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth|Parse")
	static int AZ_GetMediaItemBitRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth|Parse")
	static int AZ_GetMediaBitRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth|Parse")
	static int AZ_GetSelectedMediaBitRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth|Parse")
	static int AZ_GetTrackMediaBitRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth|Parse")
	static int AZ_GetSelectedTrackMediaBitRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate|Parse")
	static int AZ_GetMediaItemSampleRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate|Parse")
	static int AZ_GetMediaSampleRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate|Parse")
	static int AZ_GetSelectedMediaSampleRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate|Parse")
	static int AZ_GetTrackMediaSampleRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate|Parse")
	static int AZ_GetSelectedTrackMediaSampleRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel|Parse")
	static int AZ_GetMediaItemNumChannels_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel|Parse")
	static int AZ_GetMediaNumChannels_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel|Parse")
	static int AZ_GetSelectedMediaNumChannels_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel|Parse")
	static int AZ_GetTrackMediaNumChannels_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel|Parse")
	static int AZ_GetSelectedTrackMediaNumChannels_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Parse")
	static int AZ_GetMediaItemChannelMode_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Parse")
	static int AZ_GetMediaChannelMode_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Parse")
	static int AZ_GetSelectedMediaChannelMode_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Parse")
	static int AZ_GetTrackMediaChannelMode_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Parse")
	static int AZ_GetSelectedTrackMediaChannelMode_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Parse")
	static void AZ_SetMediaItemChannelMode_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Parse")
	static void AZ_SetMediaChannelMode_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Parse")
	static void AZ_SetSelectedMediaChannelMode_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Parse")
	static void AZ_SetTrackMediaChannelMode_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode|Parse")
	static void AZ_SetSelectedTrackMediaChannelMode_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Parse")
	static FString AZ_GetMediaItemName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Parse")
	static FString AZ_GetMediaName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Parse")
	static FString AZ_GetSelectedMediaName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Parse")
	static FString AZ_GetTrackMediaName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Parse")
	static FString AZ_GetSelectedTrackMediaName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Parse")
	static void AZ_SetMediaItemName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Parse")
	static void AZ_SetMediaName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Parse")
	static void AZ_SetSelectedMediaName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Parse")
	static void AZ_SetTrackMediaName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name|Parse")
	static void AZ_SetSelectedTrackMediaName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Parse")
	static bool AZ_GetMediaItemMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Parse")
	static bool AZ_GetMediaMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Parse")
	static bool AZ_GetSelectedMediaMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Parse")
	static bool AZ_GetTrackMediaMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Parse")
	static bool AZ_GetSelectedTrackMediaMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Parse")
	static void AZ_SetMediaItemMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Parse")
	static void AZ_SetMediaMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Parse")
	static void AZ_SetSelectedMediaMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Parse")
	static void AZ_SetTrackMediaMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute|Parse")
	static void AZ_SetSelectedTrackMediaMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static double AZ_GetMediaItemStartTimeSeconds_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static double AZ_GetMediaStartTimeSeconds_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static double AZ_GetSelectedMediaStartTimeSeconds_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static double AZ_GetTrackMediaStartTimeSeconds_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static double AZ_GetSelectedTrackMediaStartTimeSeconds_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static void AZ_SetMediaItemStartTimeSeconds_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static void AZ_SetMediaStartTimeSeconds_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static void AZ_SetSelectedMediaStartTimeSeconds_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static void AZ_SetTrackMediaStartTimeSeconds_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static void AZ_SetSelectedTrackMediaStartTimeSeconds_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static int AZ_GetMediaItemStartTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static int AZ_GetMediaStartTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static int AZ_GetSelectedMediaStartTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static int AZ_GetTrackMediaStartTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static int AZ_GetSelectedTrackMediaStartTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static void AZ_SetMediaItemStartTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static void AZ_SetMediaStartTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static void AZ_SetSelectedMediaStartTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static void AZ_SetTrackMediaStartTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static void AZ_SetSelectedTrackMediaStartTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static double AZ_GetMediaItemEndTime_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static double AZ_GetMediaEndTime_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static double AZ_GetSelectedMediaEndTime_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static double AZ_GetTrackMediaEndTime_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time|Parse")
	static double AZ_GetSelectedTrackMediaEndTime_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static int AZ_GetMediaItemEndTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static int AZ_GetMediaEndTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static int AZ_GetSelectedMediaEndTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static int AZ_GetTrackMediaEndTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame|Parse")
	static int AZ_GetSelectedTrackMediaEndTimeFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length|Parse")
	static double AZ_GetMediaItemLength_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length|Parse")
	static double AZ_GetMediaLength_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length|Parse")
	static double AZ_GetSelectedMediaLength_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length|Parse")
	static double AZ_GetTrackMediaLength_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length|Parse")
	static double AZ_GetSelectedTrackMediaLength_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Parse")
	static void AZ_GetMediaItemFade_Parse(URigdocksRpcCall* call, int& inType, double& inLen, int& outType, double& outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Parse")
	static void AZ_GetMediaFade_Parse(URigdocksRpcCall* call, int& inType, double& inLen, int& outType, double& outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Parse")
	static void AZ_GetSelectedMediaFade_Parse(URigdocksRpcCall* call, int& inType, double& inLen, int& outType, double& outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Parse")
	static void AZ_GetTrackMediaFade_Parse(URigdocksRpcCall* call, int& inType, double& inLen, int& outType, double& outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Parse")
	static void AZ_GetSelectedTrackMediaFade_Parse(URigdocksRpcCall* call, int& inType, double& inLen, int& outType, double& outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Parse")
	static void AZ_SetMediaItemFade_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Parse")
	static void AZ_SetMediaFade_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Parse")
	static void AZ_SetSelectedMediaFade_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Parse")
	static void AZ_SetTrackMediaFade_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade|Parse")
	static void AZ_SetSelectedTrackMediaFade_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static double AZ_GetMediaItemVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static double AZ_GetMediaVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static double AZ_GetSelectedMediaVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static double AZ_GetTrackMediaVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static double AZ_GetSelectedTrackMediaVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static void AZ_SetMediaItemVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static void AZ_SetMediaVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static void AZ_SetSelectedMediaVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static void AZ_SetTrackMediaVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static void AZ_SetSelectedTrackMediaVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static void AZ_SetMediaItemAddVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static void AZ_SetMediaAddVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static void AZ_SetSelectedMediaAddVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static void AZ_SetTrackMediaAddVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume|Parse")
	static void AZ_SetSelectedTrackMediaAddVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static double AZ_GetMediaItemPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static double AZ_GetMediaPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static double AZ_GetSelectedMediaPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static double AZ_GetTrackMediaPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static double AZ_GetSelectedTrackMediaPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static void AZ_SetMediaItemPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static void AZ_SetMediaPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static void AZ_SetSelectedMediaPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static void AZ_SetTrackMediaPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static void AZ_SetSelectedTrackMediaPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static void AZ_SetMediaItemAddPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static void AZ_SetMediaAddPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static void AZ_SetSelectedMediaAddPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static void AZ_SetTrackMediaAddPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch|Parse")
	static void AZ_SetSelectedTrackMediaAddPitch_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Parse")
	static double AZ_GetMediaItemRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Parse")
	static double AZ_GetMediaRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Parse")
	static double AZ_GetSelectedMediaRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Parse")
	static double AZ_GetTrackMediaRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Parse")
	static double AZ_GetSelectedTrackMediaRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Parse")
	static void AZ_SetMediaItemRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Parse")
	static void AZ_SetMediaRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Parse")
	static void AZ_SetSelectedMediaRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Parse")
	static void AZ_SetTrackMediaRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate|Parse")
	static void AZ_SetSelectedTrackMediaRate_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Parse")
	static void AZ_DeleteMediaItem_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Parse")
	static void AZ_DeleteMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Parse")
	static void AZ_DeleteSelectedMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Parse")
	static void AZ_DeleteTrackMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Parse")
	static void AZ_DeleteSelectedTrackMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Parse")
	static void AZ_DeleteAllMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Parse")
	static void AZ_DeleteAllSelectedMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Parse")
	static void AZ_DeleteAllTrackMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete|Parse")
	static void AZ_DeleteAllSelectedTrackMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select|Parse")
	static void AZ_SetMediaSelected_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select|Parse")
	static void AZ_SetTrackMediaSelected_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select|Parse")
	static void AZ_SetAllMediaSelected_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move|Parse")
	static void AZ_SetMediaItemMoveToTrack_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move|Parse")
	static void AZ_SetMediaMoveToTrack_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move|Parse")
	static void AZ_SetSelectedMediaMoveToTrack_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move|Parse")
	static void AZ_SetTrackMediaMoveToTrack_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move|Parse")
	static void AZ_SetSelectedTrackMediaMoveToTrack_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy|Parse")
	static void AZ_SetMediaItemActiveTakeCopy_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy|Parse")
	static void AZ_SetMediaActiveTakeCopy_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy|Parse")
	static void AZ_SetSelectedMediaActiveTakeCopy_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy|Parse")
	static void AZ_SetTrackMediaActiveTakeCopy_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy|Parse")
	static void AZ_SetSelectedTrackMediaActiveTakeCopy_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Parse")
	static UMediaItem* AZ_InsertMediaOutItem_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Parse")
	static UMediaItem* AZ_InsertMediaTrackItemSecond_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Parse")
	static UMediaItem* AZ_InsertMediaTrackIdSecond_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Parse")
	static UMediaItem* AZ_InsertMediaSelectedTrackIdSecond_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Parse")
	static UMediaItem* AZ_InsertMediaTrackItemFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Parse")
	static UMediaItem* AZ_InsertMediaTrackIdFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Parse")
	static UMediaItem* AZ_InsertMediaSelectedTrackIdFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Parse")
	static TArray<UMediaItem*> AZ_GetMediaItemList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Parse")
	static TArray<UMediaItem*> AZ_GetSelectedMediaItemList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Parse")
	static TArray<UMediaItem*> AZ_GetTrackMediaItemList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Parse")
	static TArray<UMediaItem*> AZ_GetSelectedTrackMediaItemList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get|Parse")
	static TArray<UMediaItem*> AZ_GetMediaItemListInTrackItem_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select|Parse")
	static void AZ_SelectMediaItems_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert|Parse")
	static TArray<UMediaItem*> AZ_InsertMediaItems_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static UMediaItem* AZ_InsertNoteItemToTrack_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static UMediaItem* AZ_InsertNoteItemToTrackID_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static UMediaItem* AZ_InsertNoteItemToSelectedTrackID_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static UMediaItem* AZ_InsertNoteItemToTrackFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static UMediaItem* AZ_InsertNoteItemToTrackIDFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static UMediaItem* AZ_InsertNoteItemToSelectedTrackIDFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static FString AZ_GetMediaItemNotes_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static FString AZ_GetMediaIDNotes_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static FString AZ_GetSelectedMediaIDNotes_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static FString AZ_GetTrackMediaNotes_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static FString AZ_GetSelectedTrackMediaNotes_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static void AZ_SetMediaItemNotes_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static void AZ_SetMediaIDNotes_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static void AZ_SetSelectedMediaIDNotes_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static void AZ_SetTrackMediaNotes_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note|Parse")
	static void AZ_SetSelectedTrackMediaNotes_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Overlap|Parse")
	static TArray<FMediaOverlap> AZ_GetSelectedMediaOverlapList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split|Parse")
	static void AZ_SplitMediaItem_Parse(URigdocksRpcCall* call, UMediaItem*& leftItem, UMediaItem*& rightItem);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split|Parse")
	static void AZ_SplitMediaId_Parse(URigdocksRpcCall* call, UMediaItem*& leftItem, UMediaItem*& rightItem);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split|Parse")
	static void AZ_SplitSelectedMediaId_Parse(URigdocksRpcCall* call, UMediaItem*& leftItem, UMediaItem*& rightItem);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split|Parse")
	static void AZ_SplitTrackMedia_Parse(URigdocksRpcCall* call, UMediaItem*& leftItem, UMediaItem*& rightItem);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split|Parse")
	static void AZ_SplitSelectedTrackMedia_Parse(URigdocksRpcCall* call, UMediaItem*& leftItem, UMediaItem*& rightItem);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Source|Parse")
	static FString AZ_GetMediaItemSourcePath_Parse(URigdocksRpcCall* call);
};
