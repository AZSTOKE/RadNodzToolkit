// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksBronze_MediaItem.h"
#include "RIGDOCKS_BRONZE.h"

UMediaItem* URigdocksBronze_MediaItem::AZ_GetMediaItemSelect(URadnodzClient* client, UReaProject* proj, int itemID, int mediaType)
{
	MediaItem* mediaItem = BRONZE::AZ_GetMediaItemSelect(client->GetClient(), UReaProject::Get(proj), itemID, mediaType);

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}

UMediaItem* URigdocksBronze_MediaItem::AZ_GetMediaTrackItemSelect(URadnodzClient* client, UReaProject* proj, int trackID, int itemID, int mediaType)
{
	MediaItem* mediaItem = BRONZE::AZ_GetMediaTrackItemSelect(client->GetClient(), UReaProject::Get(proj), trackID, itemID, mediaType);

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}

void URigdocksBronze_MediaItem::AZ_GetMediaTypeCount(URadnodzClient* client, UReaProject* proj, FString mediaType, int& allCount, int& typeCount)
{
	std::tie(allCount, typeCount) = BRONZE::AZ_GetMediaTypeCount(client->GetClient(), UReaProject::Get(proj), TCHAR_TO_UTF8(*mediaType));

	return;
}

void URigdocksBronze_MediaItem::AZ_GetSelectedMediaTypeCount(URadnodzClient* client, UReaProject* proj, FString mediaType, int& allCount, int& typeCount)
{
	std::tie(allCount, typeCount) = BRONZE::AZ_GetSelectedMediaTypeCount(client->GetClient(), UReaProject::Get(proj), TCHAR_TO_UTF8(*mediaType));

	return;
}

void URigdocksBronze_MediaItem::AZ_GetTrackItemMediaTypeCount(URadnodzClient* client, UMediaTrack* track, FString mediaType, int& allCount, int& typeCount)
{
	std::tie(allCount, typeCount) = BRONZE::AZ_GetTrackItemMediaTypeCount(client->GetClient(), UMediaTrack::Get(track), TCHAR_TO_UTF8(*mediaType));

	return;
}

void URigdocksBronze_MediaItem::AZ_GetTrackMediaTypeCount(URadnodzClient* client, UReaProject* proj, int trackID, FString mediaType, int& allCount, int& typeCount)
{
	std::tie(allCount, typeCount) = BRONZE::AZ_GetTrackMediaTypeCount(client->GetClient(), UReaProject::Get(proj), trackID, TCHAR_TO_UTF8(*mediaType));

	return;
}

void URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaTypeCount(URadnodzClient* client, UReaProject* proj, int selectTrackID, FString mediaType, int& allCount, int& typeCount)
{
	std::tie(allCount, typeCount) = BRONZE::AZ_GetSelectedTrackMediaTypeCount(client->GetClient(),  UReaProject::Get(proj), selectTrackID, TCHAR_TO_UTF8(*mediaType));

	return;
}

int URigdocksBronze_MediaItem::AZ_GetMediaItemBitRate(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemBitRate(client->GetClient(), UMediaItem::Get(item));
}

int URigdocksBronze_MediaItem::AZ_GetMediaBitRate(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaBitRate(client->GetClient(), UReaProject::Get(proj), itemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedMediaBitRate(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaBitRate(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

int URigdocksBronze_MediaItem::AZ_GetTrackMediaBitRate(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaBitRate(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaBitRate(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaBitRate(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
int URigdocksBronze_MediaItem::AZ_GetMediaItemSampleRate(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemSampleRate(client->GetClient(), UMediaItem::Get(item));
}

int URigdocksBronze_MediaItem::AZ_GetMediaSampleRate(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaSampleRate(client->GetClient(), UReaProject::Get(proj), itemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedMediaSampleRate(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaSampleRate(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

int URigdocksBronze_MediaItem::AZ_GetTrackMediaSampleRate(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaSampleRate(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaSampleRate(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaSampleRate(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
int URigdocksBronze_MediaItem::AZ_GetMediaItemNumChannels(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemNumChannels(client->GetClient(), UMediaItem::Get(item));
}

int URigdocksBronze_MediaItem::AZ_GetMediaNumChannels(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaNumChannels(client->GetClient(), UReaProject::Get(proj), itemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedMediaNumChannels(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaNumChannels(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

int URigdocksBronze_MediaItem::AZ_GetTrackMediaNumChannels(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaNumChannels(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaNumChannels(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaNumChannels(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
int URigdocksBronze_MediaItem::AZ_GetMediaItemChannelMode(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemChannelMode(client->GetClient(), UMediaItem::Get(item));
}

int URigdocksBronze_MediaItem::AZ_GetMediaChannelMode(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaChannelMode(client->GetClient(), UReaProject::Get(proj), itemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedMediaChannelMode(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaChannelMode(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

int URigdocksBronze_MediaItem::AZ_GetTrackMediaChannelMode(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaChannelMode(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaChannelMode(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaChannelMode(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemChannelMode(URadnodzClient* client, UMediaItem* item, int setChannelMode)
{
	BRONZE::AZ_SetMediaItemChannelMode(client->GetClient(), UMediaItem::Get(item), setChannelMode);
}

void URigdocksBronze_MediaItem::AZ_SetMediaChannelMode(URadnodzClient* client, UReaProject* proj, int itemID, int setChannelMode)
{
	BRONZE::AZ_SetMediaChannelMode(client->GetClient(), UReaProject::Get(proj), itemID, setChannelMode);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaChannelMode(URadnodzClient* client, UReaProject* proj, int selectItemID, int setChannelMode)
{
	BRONZE::AZ_SetSelectedMediaChannelMode(client->GetClient(), UReaProject::Get(proj), selectItemID, setChannelMode);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaChannelMode(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, int setChannelMode)
{
	BRONZE::AZ_SetTrackMediaChannelMode(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, setChannelMode);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaChannelMode(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, int setChannelMode)
{
	BRONZE::AZ_SetSelectedTrackMediaChannelMode(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, setChannelMode);
}
FString URigdocksBronze_MediaItem::AZ_GetMediaItemName(URadnodzClient* client, UMediaItem* item)
{
	std::string itemName = BRONZE::AZ_GetMediaItemName(client->GetClient(), UMediaItem::Get(item));

	return  UTF8_TO_TCHAR(itemName.c_str());
}

FString URigdocksBronze_MediaItem::AZ_GetMediaName(URadnodzClient* client, UReaProject* proj, int itemID)
{
	std::string itemName = BRONZE::AZ_GetMediaName(client->GetClient(), UReaProject::Get(proj), itemID);

	return  UTF8_TO_TCHAR(itemName.c_str());
}

FString URigdocksBronze_MediaItem::AZ_GetSelectedMediaName(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	std::string itemName = BRONZE::AZ_GetSelectedMediaName(client->GetClient(), UReaProject::Get(proj), selectItemID);

	return  UTF8_TO_TCHAR(itemName.c_str());
}

FString URigdocksBronze_MediaItem::AZ_GetTrackMediaName(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	std::string itemName = BRONZE::AZ_GetTrackMediaName(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);

	return  UTF8_TO_TCHAR(itemName.c_str());
}

FString URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaName(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	std::string itemName = BRONZE::AZ_GetSelectedTrackMediaName(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);

	return  UTF8_TO_TCHAR(itemName.c_str());
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemName(URadnodzClient* client, UMediaItem* item, FString setName)
{
	BRONZE::AZ_SetMediaItemName(client->GetClient(), UMediaItem::Get(item), TCHAR_TO_UTF8(*setName));
}

void URigdocksBronze_MediaItem::AZ_SetMediaName(URadnodzClient* client, UReaProject* proj, int itemID, FString setName)
{
	BRONZE::AZ_SetMediaName(client->GetClient(), UReaProject::Get(proj), itemID, TCHAR_TO_UTF8(*setName));
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaName(URadnodzClient* client, UReaProject* proj, int selectItemID, FString setName)
{
	BRONZE::AZ_SetSelectedMediaName(client->GetClient(), UReaProject::Get(proj), selectItemID, TCHAR_TO_UTF8(*setName));
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaName(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, FString setName)
{
	BRONZE::AZ_SetTrackMediaName(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, TCHAR_TO_UTF8(*setName));
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaName(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, FString setName)
{
	BRONZE::AZ_SetSelectedTrackMediaName(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, TCHAR_TO_UTF8(*setName));
}
bool URigdocksBronze_MediaItem::AZ_GetMediaItemMute(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemMute(client->GetClient(), UMediaItem::Get(item));
}

bool URigdocksBronze_MediaItem::AZ_GetMediaMute(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaMute(client->GetClient(), UReaProject::Get(proj), itemID);
}

bool URigdocksBronze_MediaItem::AZ_GetSelectedMediaMute(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaMute(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

bool URigdocksBronze_MediaItem::AZ_GetTrackMediaMute(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaMute(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

bool URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaMute(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaMute(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemMute(URadnodzClient* client, UMediaItem* item, bool setMute)
{
	BRONZE::AZ_SetMediaItemMute(client->GetClient(), UMediaItem::Get(item), setMute);
}

void URigdocksBronze_MediaItem::AZ_SetMediaMute(URadnodzClient* client, UReaProject* proj, int itemID, bool setMute)
{
	BRONZE::AZ_SetMediaMute(client->GetClient(), UReaProject::Get(proj), itemID, setMute);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaMute(URadnodzClient* client, UReaProject* proj, int selectItemID, bool setMute)
{
	BRONZE::AZ_SetSelectedMediaMute(client->GetClient(), UReaProject::Get(proj), selectItemID, setMute);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaMute(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, bool setMute)
{
	BRONZE::AZ_SetTrackMediaMute(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, setMute);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaMute(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, bool setMute)
{
	BRONZE::AZ_SetSelectedTrackMediaMute(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, setMute);
}
double URigdocksBronze_MediaItem::AZ_GetMediaItemStartTimeSeconds(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemStartTimeSeconds(client->GetClient(), UMediaItem::Get(item));
}

double URigdocksBronze_MediaItem::AZ_GetMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaStartTimeSeconds(client->GetClient(), UReaProject::Get(proj), itemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaStartTimeSeconds(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

double URigdocksBronze_MediaItem::AZ_GetTrackMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaStartTimeSeconds(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaStartTimeSeconds(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemStartTimeSeconds(URadnodzClient* client, UMediaItem* item, double setStartTimeSeconds)
{
	BRONZE::AZ_SetMediaItemStartTimeSeconds(client->GetClient(), UMediaItem::Get(item), setStartTimeSeconds);
}

void URigdocksBronze_MediaItem::AZ_SetMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int itemID, double setStartTimeSeconds)
{
	BRONZE::AZ_SetMediaStartTimeSeconds(client->GetClient(), UReaProject::Get(proj), itemID, setStartTimeSeconds);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int selectItemID, double setStartTimeSeconds)
{
	BRONZE::AZ_SetSelectedMediaStartTimeSeconds(client->GetClient(), UReaProject::Get(proj), selectItemID, setStartTimeSeconds);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double setStartTimeSeconds)
{
	BRONZE::AZ_SetTrackMediaStartTimeSeconds(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, setStartTimeSeconds);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaStartTimeSeconds(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double setStartTimeSeconds)
{
	BRONZE::AZ_SetSelectedTrackMediaStartTimeSeconds(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, setStartTimeSeconds);
}
int URigdocksBronze_MediaItem::AZ_GetMediaItemStartTimeFrame(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemStartTimeFrame(client->GetClient(), UMediaItem::Get(item));
}

int URigdocksBronze_MediaItem::AZ_GetMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaStartTimeFrame(client->GetClient(), UReaProject::Get(proj), itemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaStartTimeFrame(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

int URigdocksBronze_MediaItem::AZ_GetTrackMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaStartTimeFrame(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaStartTimeFrame(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemStartTimeFrame(URadnodzClient* client, UMediaItem* item, double setStartTimeFrame)
{
	BRONZE::AZ_SetMediaItemStartTimeFrame(client->GetClient(), UMediaItem::Get(item), setStartTimeFrame);
}

void URigdocksBronze_MediaItem::AZ_SetMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int itemID, double setStartTimeFrame)
{
	BRONZE::AZ_SetMediaStartTimeFrame(client->GetClient(), UReaProject::Get(proj), itemID, setStartTimeFrame);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int selectItemID, double setStartTimeFrame)
{
	BRONZE::AZ_SetSelectedMediaStartTimeFrame(client->GetClient(), UReaProject::Get(proj), selectItemID, setStartTimeFrame);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double setStartTimeFrame)
{
	BRONZE::AZ_SetTrackMediaStartTimeFrame(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, setStartTimeFrame);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaStartTimeFrame(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double setStartTimeFrame)
{
	BRONZE::AZ_SetSelectedTrackMediaStartTimeFrame(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, setStartTimeFrame);
}
double URigdocksBronze_MediaItem::AZ_GetMediaItemEndTime(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemEndTime(client->GetClient(), UMediaItem::Get(item));
}

double URigdocksBronze_MediaItem::AZ_GetMediaEndTime(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaEndTime(client->GetClient(), UReaProject::Get(proj), itemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedMediaEndTime(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaEndTime(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

double URigdocksBronze_MediaItem::AZ_GetTrackMediaEndTime(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaEndTime(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaEndTime(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaEndTime(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
int URigdocksBronze_MediaItem::AZ_GetMediaItemEndTimeFrame(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemEndTimeFrame(client->GetClient(), UMediaItem::Get(item));
}

int URigdocksBronze_MediaItem::AZ_GetMediaEndTimeFrame(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaEndTimeFrame(client->GetClient(), UReaProject::Get(proj), itemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedMediaEndTimeFrame(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaEndTimeFrame(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

int URigdocksBronze_MediaItem::AZ_GetTrackMediaEndTimeFrame(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaEndTimeFrame(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

int URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaEndTimeFrame(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaEndTimeFrame(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
double URigdocksBronze_MediaItem::AZ_GetMediaItemLength(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemLength(client->GetClient(), UMediaItem::Get(item));
}

double URigdocksBronze_MediaItem::AZ_GetMediaLength(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaLength(client->GetClient(), UReaProject::Get(proj), itemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedMediaLength(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaLength(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

double URigdocksBronze_MediaItem::AZ_GetTrackMediaLength(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaLength(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaLength(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaLength(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
void URigdocksBronze_MediaItem::AZ_GetMediaItemFade(URadnodzClient* client, UMediaItem* item, int& inType, double& inLen, int& outType, double& outLen)
{
	std::tie(inType, inLen, outType, outLen) = BRONZE::AZ_GetMediaItemFade(client->GetClient(), UMediaItem::Get(item));
}

void URigdocksBronze_MediaItem::AZ_GetMediaFade(URadnodzClient* client, UReaProject* proj, int itemID, int& inType, double& inLen, int& outType, double& outLen)
{
	std::tie(inType, inLen, outType, outLen) = BRONZE::AZ_GetMediaFade(client->GetClient(), UReaProject::Get(proj), itemID);
}
void URigdocksBronze_MediaItem::AZ_GetSelectedMediaFade(URadnodzClient* client, UReaProject* proj, int selectItemID, int& inType, double& inLen, int& outType, double& outLen)
{
	std::tie(inType, inLen, outType, outLen) = BRONZE::AZ_GetSelectedMediaFade(client->GetClient(), UReaProject::Get(proj), selectItemID);
}
void URigdocksBronze_MediaItem::AZ_GetTrackMediaFade(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, int& inType, double& inLen, int& outType, double& outLen)
{
	std::tie(inType, inLen, outType, outLen) = BRONZE::AZ_GetTrackMediaFade(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}
void URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaFade(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, int& inType, double& inLen, int& outType, double& outLen)
{
	std::tie(inType, inLen, outType, outLen) = BRONZE::AZ_GetSelectedTrackMediaFade(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemFade(URadnodzClient* client, UMediaItem* item, int inType, double inLen, int outType, double outLen)
{
	BRONZE::AZ_SetMediaItemFade(client->GetClient(), UMediaItem::Get(item), inType, inLen, outType, outLen);
}

void URigdocksBronze_MediaItem::AZ_SetMediaFade(URadnodzClient* client, UReaProject* proj, int itemID, int inType, double inLen, int outType, double outLen)
{
	BRONZE::AZ_SetMediaFade(client->GetClient(), UReaProject::Get(proj), itemID, inType, inLen, outType, outLen);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaFade(URadnodzClient* client, UReaProject* proj, int selectItemID, int inType, double inLen, int outType, double outLen)
{
	BRONZE::AZ_SetSelectedMediaFade(client->GetClient(), UReaProject::Get(proj), selectItemID, inType, inLen, outType, outLen);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaFade(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, int inType, double inLen, int outType, double outLen)
{
	BRONZE::AZ_SetTrackMediaFade(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, inType, inLen, outType, outLen);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaFade(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, int inType, double inLen, int outType, double outLen)
{
	BRONZE::AZ_SetSelectedTrackMediaFade(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, inType, inLen, outType, outLen);
}
double URigdocksBronze_MediaItem::AZ_GetMediaItemVolume(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemVolume(client->GetClient(), UMediaItem::Get(item));
}

double URigdocksBronze_MediaItem::AZ_GetMediaVolume(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaVolume(client->GetClient(), UReaProject::Get(proj), itemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedMediaVolume(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaVolume(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

double URigdocksBronze_MediaItem::AZ_GetTrackMediaVolume(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaVolume(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaVolume(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaVolume(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemVolume(URadnodzClient* client, UMediaItem* item, double setVolume)
{
	BRONZE::AZ_SetMediaItemVolume(client->GetClient(), UMediaItem::Get(item), setVolume);
}

void URigdocksBronze_MediaItem::AZ_SetMediaVolume(URadnodzClient* client, UReaProject* proj, int itemID, double setVolume)
{
	BRONZE::AZ_SetMediaVolume(client->GetClient(), UReaProject::Get(proj), itemID, setVolume);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaVolume(URadnodzClient* client, UReaProject* proj, int selectItemID, double setVolume)
{
	BRONZE::AZ_SetSelectedMediaVolume(client->GetClient(), UReaProject::Get(proj), selectItemID, setVolume);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaVolume(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double setVolume)
{
	BRONZE::AZ_SetTrackMediaVolume(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, setVolume);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaVolume(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double setVolume)
{
	BRONZE::AZ_SetSelectedTrackMediaVolume(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, setVolume);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemAddVolume(URadnodzClient* client, UMediaItem* item, double addVolume)
{
	BRONZE::AZ_SetMediaItemAddVolume(client->GetClient(), UMediaItem::Get(item), addVolume);
}

void URigdocksBronze_MediaItem::AZ_SetMediaAddVolume(URadnodzClient* client, UReaProject* proj, int itemID, double addVolume)
{
	BRONZE::AZ_SetMediaAddVolume(client->GetClient(), UReaProject::Get(proj), itemID, addVolume);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaAddVolume(URadnodzClient* client, UReaProject* proj, int selectItemID, double addVolume)
{
	BRONZE::AZ_SetSelectedMediaAddVolume(client->GetClient(), UReaProject::Get(proj), selectItemID, addVolume);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaAddVolume(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double addVolume)
{
	BRONZE::AZ_SetTrackMediaAddVolume(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, addVolume);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaAddVolume(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double addVolume)
{
	BRONZE::AZ_SetSelectedTrackMediaAddVolume(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, addVolume);
}
double URigdocksBronze_MediaItem::AZ_GetMediaItemPitch(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemPitch(client->GetClient(), UMediaItem::Get(item));
}

double URigdocksBronze_MediaItem::AZ_GetMediaPitch(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaPitch(client->GetClient(), UReaProject::Get(proj), itemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedMediaPitch(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaPitch(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

double URigdocksBronze_MediaItem::AZ_GetTrackMediaPitch(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaPitch(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaPitch(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaPitch(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemPitch(URadnodzClient* client, UMediaItem* item, double setPitch)
{
	BRONZE::AZ_SetMediaItemPitch(client->GetClient(), UMediaItem::Get(item), setPitch);
}

void URigdocksBronze_MediaItem::AZ_SetMediaPitch(URadnodzClient* client, UReaProject* proj, int itemID, double setPitch)
{
	BRONZE::AZ_SetMediaPitch(client->GetClient(), UReaProject::Get(proj), itemID, setPitch);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaPitch(URadnodzClient* client, UReaProject* proj, int selectItemID, double setPitch)
{
	BRONZE::AZ_SetSelectedMediaPitch(client->GetClient(), UReaProject::Get(proj), selectItemID, setPitch);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaPitch(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double setPitch)
{
	BRONZE::AZ_SetTrackMediaPitch(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, setPitch);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaPitch(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double setPitch)
{
	BRONZE::AZ_SetSelectedTrackMediaPitch(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, setPitch);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemAddPitch(URadnodzClient* client, UMediaItem* item, double addPitch)
{
	BRONZE::AZ_SetMediaItemAddPitch(client->GetClient(), UMediaItem::Get(item), addPitch);
}

void URigdocksBronze_MediaItem::AZ_SetMediaAddPitch(URadnodzClient* client, UReaProject* proj, int itemID, double addPitch)
{
	BRONZE::AZ_SetMediaAddPitch(client->GetClient(), UReaProject::Get(proj), itemID, addPitch);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaAddPitch(URadnodzClient* client, UReaProject* proj, int selectItemID, double addPitch)
{
	BRONZE::AZ_SetSelectedMediaAddPitch(client->GetClient(), UReaProject::Get(proj), selectItemID, addPitch);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaAddPitch(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double addPitch)
{
	BRONZE::AZ_SetTrackMediaAddPitch(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, addPitch);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaAddPitch(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double addPitch)
{
	BRONZE::AZ_SetSelectedTrackMediaAddPitch(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, addPitch);
}
double URigdocksBronze_MediaItem::AZ_GetMediaItemRate(URadnodzClient* client, UMediaItem* item)
{
	return BRONZE::AZ_GetMediaItemRate(client->GetClient(), UMediaItem::Get(item));
}

double URigdocksBronze_MediaItem::AZ_GetMediaRate(URadnodzClient* client, UReaProject* proj, int itemID)
{
	return BRONZE::AZ_GetMediaRate(client->GetClient(), UReaProject::Get(proj), itemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedMediaRate(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	return BRONZE::AZ_GetSelectedMediaRate(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

double URigdocksBronze_MediaItem::AZ_GetTrackMediaRate(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	return BRONZE::AZ_GetTrackMediaRate(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

double URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaRate(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	return BRONZE::AZ_GetSelectedTrackMediaRate(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemRate(URadnodzClient* client, UMediaItem* item, double setRate)
{
	BRONZE::AZ_SetMediaItemRate(client->GetClient(), UMediaItem::Get(item), setRate);
}

void URigdocksBronze_MediaItem::AZ_SetMediaRate(URadnodzClient* client, UReaProject* proj, int itemID, double setRate)
{
	BRONZE::AZ_SetMediaRate(client->GetClient(), UReaProject::Get(proj), itemID, setRate);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaRate(URadnodzClient* client, UReaProject* proj, int selectItemID, double setRate)
{
	BRONZE::AZ_SetSelectedMediaRate(client->GetClient(), UReaProject::Get(proj), selectItemID, setRate);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaRate(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double setRate)
{
	BRONZE::AZ_SetTrackMediaRate(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, setRate);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaRate(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, double setRate)
{
	BRONZE::AZ_SetSelectedTrackMediaRate(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, setRate);
}
void URigdocksBronze_MediaItem::AZ_DeleteMediaItem(URadnodzClient* client, UMediaItem* item)
{
	BRONZE::AZ_DeleteMediaItem(client->GetClient(), UMediaItem::Get(item));
}

void URigdocksBronze_MediaItem::AZ_DeleteMedia(URadnodzClient* client, UReaProject* proj, int itemID)
{
	BRONZE::AZ_DeleteMedia(client->GetClient(), UReaProject::Get(proj), itemID);
}

void URigdocksBronze_MediaItem::AZ_DeleteSelectedMedia(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	BRONZE::AZ_DeleteSelectedMedia(client->GetClient(), UReaProject::Get(proj), selectItemID);
}

void URigdocksBronze_MediaItem::AZ_DeleteTrackMedia(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	BRONZE::AZ_DeleteTrackMedia(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);
}

void URigdocksBronze_MediaItem::AZ_DeleteSelectedTrackMedia(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	BRONZE::AZ_DeleteSelectedTrackMedia(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);
}
void URigdocksBronze_MediaItem::AZ_DeleteAllMedia(URadnodzClient* client, UReaProject* proj)
{
	BRONZE::AZ_DeleteAllMedia(client->GetClient(), UReaProject::Get(proj));
}

void URigdocksBronze_MediaItem::AZ_DeleteAllSelectedMedia(URadnodzClient* client, UReaProject* proj)
{
	BRONZE::AZ_DeleteAllSelectedMedia(client->GetClient(), UReaProject::Get(proj));
}

void URigdocksBronze_MediaItem::AZ_DeleteAllTrackMedia(URadnodzClient* client, UReaProject* proj, int trackID)
{
	BRONZE::AZ_DeleteAllTrackMedia(client->GetClient(), UReaProject::Get(proj), trackID);
}

void URigdocksBronze_MediaItem::AZ_DeleteAllSelectedTrackMedia(URadnodzClient* client, UReaProject* proj, int selectTrackID)
{
	BRONZE::AZ_DeleteAllSelectedTrackMedia(client->GetClient(), UReaProject::Get(proj), selectTrackID);
}
void URigdocksBronze_MediaItem::AZ_SetMediaSelected(URadnodzClient* client, UReaProject* proj, int itemID, bool selectType)
{
	BRONZE::AZ_SetMediaSelected(client->GetClient(), UReaProject::Get(proj), itemID, selectType);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaSelected(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, bool selectType)
{
	BRONZE::AZ_SetTrackMediaSelected(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID,  selectType);
}

void URigdocksBronze_MediaItem::AZ_SetAllMediaSelected(URadnodzClient* client, UReaProject* proj, bool selectType)
{
	BRONZE::AZ_SetAllMediaSelected(client->GetClient(), UReaProject::Get(proj), selectType);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemMoveToTrack(URadnodzClient* client, UMediaItem* item, int moveTrackID)
{
	BRONZE::AZ_SetMediaItemMoveToTrack(client->GetClient(), UMediaItem::Get(item), moveTrackID);
}

void URigdocksBronze_MediaItem::AZ_SetMediaMoveToTrack(URadnodzClient* client, UReaProject* proj, int itemID, int moveTrackID)
{
	BRONZE::AZ_SetMediaMoveToTrack(client->GetClient(), UReaProject::Get(proj), itemID, moveTrackID);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaMoveToTrack(URadnodzClient* client, UReaProject* proj, int selectItemID, int moveTrackID)
{
	BRONZE::AZ_SetSelectedMediaMoveToTrack(client->GetClient(), UReaProject::Get(proj), selectItemID, moveTrackID);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaMoveToTrack(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, int moveTrackID)
{
	BRONZE::AZ_SetTrackMediaMoveToTrack(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, moveTrackID);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaMoveToTrack(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, int moveTrackID)
{
	BRONZE::AZ_SetSelectedTrackMediaMoveToTrack(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, moveTrackID);
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemActiveTakeCopy(URadnodzClient* client, UMediaItem* item, int setTrackID, double setPosition)
{
	BRONZE::AZ_SetMediaItemActiveTakeCopy(client->GetClient(), UMediaItem::Get(item), setTrackID, setPosition);
}

void URigdocksBronze_MediaItem::AZ_SetMediaActiveTakeCopy(URadnodzClient* client, UReaProject* proj, int itemID, int setTrackID, double setPosition)
{
	BRONZE::AZ_SetMediaActiveTakeCopy(client->GetClient(), UReaProject::Get(proj), itemID, setTrackID, setPosition);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaActiveTakeCopy(URadnodzClient* client, UReaProject* proj, int selectItemID, int setTrackID, double setPosition)
{
	BRONZE::AZ_SetSelectedMediaActiveTakeCopy(client->GetClient(), UReaProject::Get(proj), selectItemID, setTrackID, setPosition);
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaActiveTakeCopy(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, int setTrackID, double setPosition)
{
	BRONZE::AZ_SetTrackMediaActiveTakeCopy(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, setTrackID, setPosition);
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaActiveTakeCopy(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, int setTrackID, double setPosition)
{
	BRONZE::AZ_SetSelectedTrackMediaActiveTakeCopy(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, setTrackID, setPosition);
}
UMediaItem* URigdocksBronze_MediaItem::AZ_InsertMediaOutItem(URadnodzClient* client, UReaProject* proj, FString filePath)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertMediaOutItem(client->GetClient(), UReaProject::Get(proj), TCHAR_TO_UTF8(*filePath));

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}

UMediaItem* URigdocksBronze_MediaItem::AZ_InsertMediaTrackItemSecond(URadnodzClient* client, UMediaTrack* track, FString filePath, int startEndType, double setSecond)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertMediaTrackItemSecond(client->GetClient(), UMediaTrack::Get(track), TCHAR_TO_UTF8(*filePath), startEndType, setSecond);

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}
UMediaItem* URigdocksBronze_MediaItem::AZ_InsertMediaTrackIdSecond(URadnodzClient* client, UReaProject* proj, int trackID, FString filePath, int startEndType, double setSecond)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertMediaTrackIdSecond(client->GetClient(), UReaProject::Get(proj), trackID, TCHAR_TO_UTF8(*filePath), startEndType, setSecond);

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}
UMediaItem* URigdocksBronze_MediaItem::AZ_InsertMediaSelectedTrackIdSecond(URadnodzClient* client, UReaProject* proj, int selectTrackID, FString filePath, int startEndType, double setSecond)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertMediaSelectedTrackIdSecond(client->GetClient(), UReaProject::Get(proj), selectTrackID, TCHAR_TO_UTF8(*filePath), startEndType, setSecond);

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}
UMediaItem* URigdocksBronze_MediaItem::AZ_InsertMediaTrackItemFrame(URadnodzClient* client, UMediaTrack* track, FString filePath, int startEndType, int setFrame)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertMediaTrackItemFrame(client->GetClient(), UMediaTrack::Get(track), TCHAR_TO_UTF8(*filePath), startEndType, setFrame);

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}
UMediaItem* URigdocksBronze_MediaItem::AZ_InsertMediaTrackIdFrame(URadnodzClient* client, UReaProject* proj, int trackID, FString filePath, int startEndType, int setFrame)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertMediaTrackIdFrame(client->GetClient(), UReaProject::Get(proj), trackID, TCHAR_TO_UTF8(*filePath), startEndType, setFrame);

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}
UMediaItem* URigdocksBronze_MediaItem::AZ_InsertMediaSelectedTrackIdFrame(URadnodzClient* client, UReaProject* proj, int selectTrackID, FString filePath, int startEndType, int setFrame)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertMediaSelectedTrackIdFrame(client->GetClient(), UReaProject::Get(proj), selectTrackID, TCHAR_TO_UTF8(*filePath), startEndType, setFrame);

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}
TArray<UMediaItem*> URigdocksBronze_MediaItem::AZ_GetMediaItemList(URadnodzClient* client, UReaProject* proj)
{
	std::vector<MediaItem*> objArray = BRONZE::AZ_GetMediaItemList(client->GetClient(), UReaProject::Get(proj));

	TArray<UMediaItem*> result;

	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}

TArray<UMediaItem*> URigdocksBronze_MediaItem::AZ_GetSelectedMediaItemList(URadnodzClient* client, UReaProject* proj)
{
	std::vector<MediaItem*> objArray = BRONZE::AZ_GetSelectedMediaItemList(client->GetClient(), UReaProject::Get(proj));

	TArray<UMediaItem*> result;

	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}

TArray<UMediaItem*> URigdocksBronze_MediaItem::AZ_GetTrackMediaItemList(URadnodzClient* client, UReaProject* proj, int trackID)
{
	std::vector<MediaItem*> objArray = BRONZE::AZ_GetTrackMediaItemList(client->GetClient(), UReaProject::Get(proj), trackID);

	TArray<UMediaItem*> result;

	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}

TArray<UMediaItem*> URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaItemList(URadnodzClient* client, UReaProject* proj, int SelectedTrackID)
{
	std::vector<MediaItem*> objArray = BRONZE::AZ_GetSelectedTrackMediaItemList(client->GetClient(), UReaProject::Get(proj), SelectedTrackID);

	TArray<UMediaItem*> result;

	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}

TArray<UMediaItem*> URigdocksBronze_MediaItem::AZ_GetMediaItemListInTrackItem(URadnodzClient* client, UMediaTrack* track)
{
	std::vector<MediaItem*> objArray = BRONZE::AZ_GetMediaItemListInTrackItem(client->GetClient(), UMediaTrack::Get(track));

	TArray<UMediaItem*> result;

	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}
void URigdocksBronze_MediaItem::AZ_SelectMediaItems(URadnodzClient* client, TArray<UMediaItem*> itemList, bool isSelected)
{
	std::vector<MediaItem*> objArray;
	
	for(auto uObj : itemList)
	{
		objArray.push_back((MediaItem*)uObj->GetPointer());
	}

	BRONZE::AZ_SelectMediaItems(client->GetClient(), objArray, isSelected);
}

TArray<UMediaItem*> URigdocksBronze_MediaItem::AZ_InsertMediaItems(URadnodzClient* client, UMediaTrack* track, TArray<FString> sourcePathList, double startPos, double interval, int timeUnitType)
{
	std::vector<std::string> pathArray;

	for (auto str : sourcePathList)
	{
		pathArray.push_back(TCHAR_TO_UTF8(*str));
	}

	std::vector<MediaItem*> objArray = BRONZE::AZ_InsertMediaItems(client->GetClient(), UMediaTrack::Get(track), pathArray, startPos, interval, timeUnitType);

	TArray<UMediaItem*> result;

	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);

		result.Add(uObj);
	}

	return result;
}
UMediaItem* URigdocksBronze_MediaItem::AZ_InsertNoteItemToTrack(URadnodzClient* client, UMediaTrack* track, double startSecond, double length, FString text)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertNoteItemToTrack(client->GetClient(), UMediaTrack::Get(track), startSecond, length, TCHAR_TO_UTF8(*text));

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}
UMediaItem* URigdocksBronze_MediaItem::AZ_InsertNoteItemToTrackID(URadnodzClient* client, UReaProject* proj, int trackID, double startSecond, double length, FString text)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertNoteItemToTrackID(client->GetClient(), UReaProject::Get(proj), trackID, startSecond, length, TCHAR_TO_UTF8(*text));

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}
UMediaItem* URigdocksBronze_MediaItem::AZ_InsertNoteItemToSelectedTrackID(URadnodzClient* client, UReaProject* proj, int selectTrackID, double startSecond, double length, FString text)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertNoteItemToSelectedTrackID(client->GetClient(), UReaProject::Get(proj), selectTrackID, startSecond, length, TCHAR_TO_UTF8(*text));

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}
UMediaItem* URigdocksBronze_MediaItem::AZ_InsertNoteItemToTrackFrame(URadnodzClient* client, UMediaTrack* track, double startFrame, double lengthFrame, FString text)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertNoteItemToTrackFrame(client->GetClient(), UMediaTrack::Get(track), startFrame, lengthFrame, TCHAR_TO_UTF8(*text));

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}

UMediaItem* URigdocksBronze_MediaItem::AZ_InsertNoteItemToTrackIDFrame(URadnodzClient* client, UReaProject* proj, int trackID, double startFrame, double lengthFrame, FString text)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertNoteItemToTrackIDFrame(client->GetClient(), UReaProject::Get(proj), trackID, startFrame, lengthFrame, TCHAR_TO_UTF8(*text));

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}

UMediaItem* URigdocksBronze_MediaItem::AZ_InsertNoteItemToSelectedTrackIDFrame(URadnodzClient* client, UReaProject* proj, int selectTrackID, double startFrame, double lengthFrame, FString text)
{
	MediaItem* mediaItem = BRONZE::AZ_InsertNoteItemToSelectedTrackIDFrame(client->GetClient(), UReaProject::Get(proj), selectTrackID, startFrame, lengthFrame, TCHAR_TO_UTF8(*text));

	UMediaItem* mediaItemObject = NewObject<UMediaItem>();
	mediaItemObject->SetPointer((void*)mediaItem);

	return mediaItemObject;
}
FString URigdocksBronze_MediaItem::AZ_GetMediaItemNotes(URadnodzClient* client, UMediaItem* item)
{
	std::string itemNotes = BRONZE::AZ_GetMediaItemNotes(client->GetClient(), UMediaItem::Get(item));

	return  UTF8_TO_TCHAR(itemNotes.c_str());
}

FString URigdocksBronze_MediaItem::AZ_GetMediaIDNotes(URadnodzClient* client, UReaProject* proj, int itemID)
{
	std::string itemNotes = BRONZE::AZ_GetMediaIDNotes(client->GetClient(), UReaProject::Get(proj), itemID);

	return  UTF8_TO_TCHAR(itemNotes.c_str());
}

FString URigdocksBronze_MediaItem::AZ_GetSelectedMediaIDNotes(URadnodzClient* client, UReaProject* proj, int selectItemID)
{
	std::string itemNotes = BRONZE::AZ_GetSelectedMediaIDNotes(client->GetClient(), UReaProject::Get(proj), selectItemID);

	return  UTF8_TO_TCHAR(itemNotes.c_str());
}

FString URigdocksBronze_MediaItem::AZ_GetTrackMediaNotes(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID)
{
	std::string itemNotes = BRONZE::AZ_GetTrackMediaNotes(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID);

	return  UTF8_TO_TCHAR(itemNotes.c_str());
}

FString URigdocksBronze_MediaItem::AZ_GetSelectedTrackMediaNotes(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID)
{
	std::string itemNotes = BRONZE::AZ_GetSelectedTrackMediaNotes(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID);

	return  UTF8_TO_TCHAR(itemNotes.c_str());
}
void URigdocksBronze_MediaItem::AZ_SetMediaItemNotes(URadnodzClient* client, UMediaItem* item, FString setNotes)
{
	BRONZE::AZ_SetMediaItemNotes(client->GetClient(), UMediaItem::Get(item), TCHAR_TO_UTF8(*setNotes));
}

void URigdocksBronze_MediaItem::AZ_SetMediaIDNotes(URadnodzClient* client, UReaProject* proj, int itemID, FString setNotes)
{
	BRONZE::AZ_SetMediaIDNotes(client->GetClient(), UReaProject::Get(proj), itemID, TCHAR_TO_UTF8(*setNotes));
}

void URigdocksBronze_MediaItem::AZ_SetSelectedMediaIDNotes(URadnodzClient* client, UReaProject* proj, int selectItemID, FString setNotes)
{
	BRONZE::AZ_SetSelectedMediaIDNotes(client->GetClient(), UReaProject::Get(proj), selectItemID, TCHAR_TO_UTF8(*setNotes));
}

void URigdocksBronze_MediaItem::AZ_SetTrackMediaNotes(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, FString setNotes)
{
	BRONZE::AZ_SetTrackMediaNotes(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, TCHAR_TO_UTF8(*setNotes));
}

void URigdocksBronze_MediaItem::AZ_SetSelectedTrackMediaNotes(URadnodzClient* client, UReaProject* proj, int selectTrackID, int trackItemID, FString setNotes)
{
	BRONZE::AZ_SetSelectedTrackMediaNotes(client->GetClient(), UReaProject::Get(proj), selectTrackID, trackItemID, TCHAR_TO_UTF8(*setNotes));
}
TArray<FMediaOverlap> URigdocksBronze_MediaItem::AZ_GetSelectedMediaOverlapList(URadnodzClient* client, UReaProject* proj)
{
	std::vector<RADNODZ_client::Object::MediaOverlap> objArray =  BRONZE::AZ_GetSelectedMediaOverlapList(client->GetClient(), UReaProject::Get(proj));

	TArray<FMediaOverlap> result;

	for (auto obj : objArray)
	{
		FMediaOverlap uObj;
		uObj.Initialize(obj);

		result.Add(uObj);
	}

	return result;

}

void URigdocksBronze_MediaItem::AZ_SplitMediaItem(URadnodzClient* client, UMediaItem* item, double splitPosition, UMediaItem*& leftItem, UMediaItem*& rightItem)
{
	auto [oLeft, oRight] = BRONZE::AZ_SplitMediaItem(client->GetClient(), UMediaItem::Get(item), splitPosition);

	leftItem = NewObject<UMediaItem>();
	leftItem->SetPointer((void*)oLeft);

	rightItem = NewObject<UMediaItem>();
	rightItem->SetPointer((void*)oRight);
}

void URigdocksBronze_MediaItem::AZ_SplitMediaId(URadnodzClient* client, UReaProject* proj, int itemID, double splitPosition, UMediaItem*& leftItem, UMediaItem*& rightItem)
{
	auto [oLeft, oRight] = BRONZE::AZ_SplitMediaId(client->GetClient(), UReaProject::Get(proj), itemID, splitPosition);

	leftItem = NewObject<UMediaItem>();
	leftItem->SetPointer((void*)oLeft);

	rightItem = NewObject<UMediaItem>();
	rightItem->SetPointer((void*)oRight);
}

void URigdocksBronze_MediaItem::AZ_SplitSelectedMediaId(URadnodzClient* client, UReaProject* proj, int selectedItemID, double splitPosition, UMediaItem*& leftItem, UMediaItem*& rightItem)
{
	auto [oLeft, oRight] = BRONZE::AZ_SplitSelectedMediaId(client->GetClient(), UReaProject::Get(proj), selectedItemID, splitPosition);

	leftItem = NewObject<UMediaItem>();
	leftItem->SetPointer((void*)oLeft);

	rightItem = NewObject<UMediaItem>();
	rightItem->SetPointer((void*)oRight);
}

void URigdocksBronze_MediaItem::AZ_SplitTrackMedia(URadnodzClient* client, UReaProject* proj, int trackID, int trackItemID, double splitPosition, UMediaItem*& leftItem, UMediaItem*& rightItem)
{
	auto [oLeft, oRight] = BRONZE::AZ_SplitTrackMedia(client->GetClient(), UReaProject::Get(proj), trackID, trackItemID, splitPosition);

	leftItem = NewObject<UMediaItem>();
	leftItem->SetPointer((void*)oLeft);

	rightItem = NewObject<UMediaItem>();
	rightItem->SetPointer((void*)oRight);
}

void URigdocksBronze_MediaItem::AZ_SplitSelectedTrackMedia(URadnodzClient* client, UReaProject* proj, int selectedTrackID, int trackItemID, double splitPosition, UMediaItem*& leftItem, UMediaItem*& rightItem)
{
	auto [oLeft, oRight] = BRONZE::AZ_SplitSelectedTrackMedia(client->GetClient(), UReaProject::Get(proj), selectedTrackID, trackItemID, splitPosition);

	leftItem = NewObject<UMediaItem>();
	leftItem->SetPointer((void*)oLeft);

	rightItem = NewObject<UMediaItem>();
	rightItem->SetPointer((void*)oRight);
}

FString URigdocksBronze_MediaItem::AZ_GetMediaItemSourcePath(URadnodzClient* client, UMediaItem* item)
{
	std::string sourcePath = BRONZE::AZ_GetMediaItemSourcePath(client->GetClient(), UMediaItem::Get(item));

	return  UTF8_TO_TCHAR(sourcePath.c_str());
}
