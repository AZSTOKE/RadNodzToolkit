// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FMediaOverlap.h"
#include "RigdocksBronze_MediaItem.generated.h"

UCLASS()
class RIGDOCKSBRONZEPLUGIN_API URigdocksBronze_MediaItem : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_GetMediaItemSelect(URadnodzClient* client, UReaProject* proj, int itemID, int mediaType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_GetMediaTrackItemSelect(URadnodzClient* client, UReaProject* proj, int trackID, int itemID, int mediaType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetMediaTypeCount(URadnodzClient* client, UReaProject* proj, FString mediaType, int& allCount, int& typeCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetSelectedMediaTypeCount(URadnodzClient* client, UReaProject* proj, FString mediaType, int& allCount, int& typeCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetTrackItemMediaTypeCount(URadnodzClient* client, UMediaTrack* track, FString mediaType, int& allCount, int& typeCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetTrackMediaTypeCount(URadnodzClient* client, UReaProject* proj, int trackID, FString mediaType, int& allCount, int& typeCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Count",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetSelectedTrackMediaTypeCount(URadnodzClient* client, UReaProject* proj, int selectTrackID, FString mediaType, int& allCount, int& typeCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaItemBitRate(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaBitRate(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedMediaBitRate(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackMediaBitRate(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|PitDepth",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedTrackMediaBitRate(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaItemSampleRate(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaSampleRate(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedMediaSampleRate(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackMediaSampleRate(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|SamplingRate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedTrackMediaSampleRate(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaItemNumChannels(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaNumChannels(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedMediaNumChannels(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackMediaNumChannels(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|NumChannel",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedTrackMediaNumChannels(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaItemChannelMode(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaChannelMode(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedMediaChannelMode(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackMediaChannelMode(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedTrackMediaChannelMode(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemChannelMode(URadnodzClient* client, UMediaItem* item, int setChannelMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaChannelMode(URadnodzClient* client, UReaProject* proj, int itemID, int setChannelMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaChannelMode(URadnodzClient* client, UReaProject* proj, int selectItemID, int setChannelMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaChannelMode(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, int setChannelMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|ChannelMode",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaChannelMode(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, int setChannelMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetMediaItemName(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetMediaName(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetSelectedMediaName(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetTrackMediaName(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetSelectedTrackMediaName(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemName(URadnodzClient* client, UMediaItem* item, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaName(URadnodzClient* client, UReaProject* proj, int itemID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaName(URadnodzClient* client, UReaProject* proj, int selectItemID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaName(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Name",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaName(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetMediaItemMute(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetMediaMute(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetSelectedMediaMute(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetTrackMediaMute(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetSelectedTrackMediaMute(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemMute(URadnodzClient* client, UMediaItem* item, bool setMute);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaMute(URadnodzClient* client, UReaProject* proj, int itemID, bool setMute);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaMute(URadnodzClient* client, UReaProject* proj, int selectItemID, bool setMute);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaMute(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, bool setMute);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Mute",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaMute(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, bool setMute);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaItemStartTimeSeconds(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetTrackMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedTrackMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemStartTimeSeconds(URadnodzClient* client, UMediaItem* item, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int itemID, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int selectItemID, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaItemStartTimeFrame(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedTrackMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemStartTimeFrame(URadnodzClient* client, UMediaItem* item, double setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int itemID, double setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int selectItemID, double setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaItemEndTime(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaEndTime(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedMediaEndTime(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetTrackMediaEndTime(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Time",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedTrackMediaEndTime(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaItemEndTimeFrame(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetMediaEndTimeFrame(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedMediaEndTimeFrame(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackMediaEndTimeFrame(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Frame",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedTrackMediaEndTimeFrame(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaItemLength(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaLength(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedMediaLength(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetTrackMediaLength(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Length",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedTrackMediaLength(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetMediaItemFade(URadnodzClient* client, UMediaItem* item, int& inType, double& inLen, int& outType, double& outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetMediaFade(URadnodzClient* client, UReaProject* proj, int itemID, int& inType, double& inLen, int& outType, double& outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetSelectedMediaFade(URadnodzClient* client, UReaProject* proj, int selectItemID, int& inType, double& inLen, int& outType, double& outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetTrackMediaFade(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, int& inType, double& inLen, int& outType, double& outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetSelectedTrackMediaFade(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, int& inType, double& inLen, int& outType, double& outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemFade(URadnodzClient* client, UMediaItem* item, int inType, double inLen, int outType, double outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaFade(URadnodzClient* client, UReaProject* proj, int itemID, int inType, double inLen, int outType, double outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaFade(URadnodzClient* client, UReaProject* proj, int selectItemID, int inType, double inLen, int outType, double outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	void AZ_SetTrackMediaFade(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, int inType, double inLen, int outType, double outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Fade",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaFade(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, int inType, double inLen, int outType, double outLen);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaItemVolume(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaVolume(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedMediaVolume(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetTrackMediaVolume(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedTrackMediaVolume(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemVolume(URadnodzClient* client, UMediaItem* item, double volume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaVolume(URadnodzClient* client, UReaProject* proj, int itemID, double volume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaVolume(URadnodzClient* client, UReaProject* proj, int itemID, double volume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaVolume(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double volume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaVolume(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double volume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemAddVolume(URadnodzClient* client, UMediaItem* item, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaAddVolume(URadnodzClient* client, UReaProject* proj, int itemID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaAddVolume(URadnodzClient* client, UReaProject* proj, int itemID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaAddVolume(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Volume",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaAddVolume(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaItemPitch(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaPitch(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedMediaPitch(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetTrackMediaPitch(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedTrackMediaPitch(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemPitch(URadnodzClient* client, UMediaItem* item, double setPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaPitch(URadnodzClient* client, UReaProject* proj, int itemID, double setPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaPitch(URadnodzClient* client, UReaProject* proj, int selectItemID, double setPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaPitch(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double setPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaPitch(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double setPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemAddPitch(URadnodzClient* client, UMediaItem* item, double addPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaAddPitch(URadnodzClient* client, UReaProject* proj, int itemID, double addPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaAddPitch(URadnodzClient* client, UReaProject* proj, int selectItemID, double addPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaAddPitch(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double addPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Pitch",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaAddPitch(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double addPitch);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaItemRate(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetMediaRate(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedMediaRate(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetTrackMediaRate(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedTrackMediaRate(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemRate(URadnodzClient* client, UMediaItem* item, double setRate);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaRate(URadnodzClient* client, UReaProject* proj, int itemID, double setRate);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaRate(URadnodzClient* client, UReaProject* proj, int selectItemID, double setRate);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaRate(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double setRate);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Rate",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaRate(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double setRate);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteMediaItem(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteMedia(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteSelectedMedia(URadnodzClient* client, UReaProject* proj, int selectItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteTrackMedia(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteSelectedTrackMedia(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteAllMedia(URadnodzClient* client, UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteAllSelectedMedia(URadnodzClient* client, UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteAllTrackMedia(URadnodzClient* client, UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Delete",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteAllSelectedTrackMedia(URadnodzClient* client, UReaProject* proj, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaSelected(URadnodzClient* client, UReaProject* proj, int itemID, bool selectType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaSelected(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, bool selectType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetAllMediaSelected(URadnodzClient* client, UReaProject* proj, bool selectType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemMoveToTrack(URadnodzClient* client, UMediaItem* item, int moveTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaMoveToTrack(URadnodzClient* client, UReaProject* proj, int itemID, int moveTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaMoveToTrack(URadnodzClient* client, UReaProject* proj, int selectItemID, int moveTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaMoveToTrack(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, int moveTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Move",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaMoveToTrack(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, int moveTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemActiveTakeCopy(URadnodzClient* client, UMediaItem* item, int setTrackID, double setPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaActiveTakeCopy(URadnodzClient* client, UReaProject* proj, int itemID, int setTrackID, double setPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaActiveTakeCopy(URadnodzClient* client, UReaProject* proj, int selectItemID, int setTrackID, double setPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaActiveTakeCopy(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, int setTrackID, double setPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Copy",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaActiveTakeCopy(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, int setTrackID, double setPosition);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertMediaOutItem(URadnodzClient* client, UReaProject* proj, FString filePath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertMediaTrackItemSecond(URadnodzClient* client, UMediaTrack* track, FString filePath, int startEndType, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertMediaTrackIdSecond(URadnodzClient* client, UReaProject* proj, int trackID, FString filePath, int startEndType, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertMediaSelectedTrackIdSecond(URadnodzClient* client, UReaProject* proj, int selectTrackID, FString filePath, int startEndType, double setSecond);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertMediaTrackItemFrame(URadnodzClient* client, UMediaTrack* track, FString filePath, int startEndType, int setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertMediaTrackIdFrame(URadnodzClient* client, UReaProject* proj, int trackID, FString filePath, int startEndType, int setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertMediaSelectedTrackIdFrame(URadnodzClient* client, UReaProject* proj, int selectTrackID, FString filePath, int startEndType, int setFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<UMediaItem*> AZ_GetMediaItemList(URadnodzClient* client, UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<UMediaItem*> AZ_GetSelectedMediaItemList(URadnodzClient* client, UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<UMediaItem*> AZ_GetTrackMediaItemList(URadnodzClient* client, UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<UMediaItem*> AZ_GetSelectedTrackMediaItemList(URadnodzClient* client, UReaProject* proj, int SelectedTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Get",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<UMediaItem*> AZ_GetMediaItemListInTrackItem(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Select",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SelectMediaItems(URadnodzClient* client, TArray<UMediaItem*> itemList, bool isSelected);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Insert",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<UMediaItem*> AZ_InsertMediaItems(URadnodzClient* client, UMediaTrack* track, TArray<FString> sourcePathList, double startPos, double interval, int timeUnitType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertNoteItemToTrack(URadnodzClient* client, UMediaTrack* track, double startSecond, double length, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertNoteItemToTrackID(URadnodzClient* client, UReaProject* proj, int trackID, double startSecond, double length, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertNoteItemToSelectedTrackID(URadnodzClient* client, UReaProject* proj, int selectTrackID, double startSecond, double length, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertNoteItemToTrackFrame(URadnodzClient* client, UMediaTrack* track, double startFrame, double lengthFrame, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertNoteItemToTrackIDFrame(URadnodzClient* client, UReaProject* proj, int trackID, double startFrame, double lengthFrame, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaItem* AZ_InsertNoteItemToSelectedTrackIDFrame(URadnodzClient* client, UReaProject* proj, int selectTrackID, double startFrame, double lengthFrame, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetMediaItemNotes(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetMediaIDNotes(URadnodzClient* client, UReaProject* proj, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetSelectedMediaIDNotes(URadnodzClient* client, UReaProject* proj, int selectedItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetTrackMediaNotes(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetSelectedTrackMediaNotes(URadnodzClient* client, UReaProject* proj, int selectedTrackID, int trackItemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaItemNotes(URadnodzClient* client, UMediaItem* item, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetMediaIDNotes(URadnodzClient* client, UReaProject* proj, int itemID, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedMediaIDNotes(URadnodzClient* client, UReaProject* proj, int selectedItemID, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackMediaNotes(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Note",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackMediaNotes(URadnodzClient* client, UReaProject* proj, int selectedTrackID, int trackItemID, FString text);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Overlap",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FMediaOverlap> AZ_GetSelectedMediaOverlapList(URadnodzClient* client, UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SplitMediaItem(URadnodzClient* client, UMediaItem* item, double splitPosition, UMediaItem*& leftItem, UMediaItem*& rightItem);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SplitMediaId(URadnodzClient* client, UReaProject* proj, int itemID, double splitPosition, UMediaItem*& leftItem, UMediaItem*& rightItem);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SplitSelectedMediaId(URadnodzClient* client, UReaProject* proj, int selectedItemID, double splitPosition, UMediaItem*& leftItem, UMediaItem*& rightItem);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SplitTrackMedia(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double splitPosition, UMediaItem*& leftItem, UMediaItem*& rightItem);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Split",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SplitSelectedTrackMedia(URadnodzClient* client, UReaProject* proj, int selectedTrackID, int trackItemID, double splitPosition, UMediaItem*& leftItem, UMediaItem*& rightItem);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|MediaItem|Source",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetMediaItemSourcePath(URadnodzClient* client, UMediaItem* item);

};
