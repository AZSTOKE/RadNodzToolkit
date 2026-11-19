// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksBronze_MediaItem_Parse.h"
#include "RIGDOCKS_BRONZE_Parse.h"
#include <string>
#include <vector>
#include <tuple>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemSelect_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_GetMediaItemSelect_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_GetMediaTrackItemSelect_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_GetMediaTrackItemSelect_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

void URigdocksBronze_MediaItem_Parse::AZ_GetMediaTypeCount_Parse(URigdocksRpcCall* call, int& allCount, int& typeCount)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(allCount, typeCount) = BRONZE::AZ_GetMediaTypeCount_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaTypeCount_Parse(URigdocksRpcCall* call, int& allCount, int& typeCount)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(allCount, typeCount) = BRONZE::AZ_GetSelectedMediaTypeCount_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_GetTrackItemMediaTypeCount_Parse(URigdocksRpcCall* call, int& allCount, int& typeCount)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(allCount, typeCount) = BRONZE::AZ_GetTrackItemMediaTypeCount_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaTypeCount_Parse(URigdocksRpcCall* call, int& allCount, int& typeCount)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(allCount, typeCount) = BRONZE::AZ_GetTrackMediaTypeCount_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaTypeCount_Parse(URigdocksRpcCall* call, int& allCount, int& typeCount)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(allCount, typeCount) = BRONZE::AZ_GetSelectedTrackMediaTypeCount_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemBitRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaItemBitRate_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaBitRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaBitRate_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaBitRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedMediaBitRate_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaBitRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetTrackMediaBitRate_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaBitRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaBitRate_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemSampleRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaItemSampleRate_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaSampleRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaSampleRate_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaSampleRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedMediaSampleRate_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaSampleRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetTrackMediaSampleRate_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaSampleRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaSampleRate_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemNumChannels_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaItemNumChannels_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaNumChannels_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaNumChannels_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaNumChannels_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedMediaNumChannels_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaNumChannels_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetTrackMediaNumChannels_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaNumChannels_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaNumChannels_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemChannelMode_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaItemChannelMode_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaChannelMode_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaChannelMode_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaChannelMode_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedMediaChannelMode_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaChannelMode_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetTrackMediaChannelMode_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaChannelMode_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaChannelMode_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemChannelMode_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemChannelMode_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaChannelMode_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaChannelMode_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaChannelMode_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaChannelMode_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaChannelMode_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaChannelMode_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaChannelMode_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaChannelMode_Parse(call->Get());
}

FString URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string s = BRONZE::AZ_GetMediaItemName_Parse(call->Get());
	return UTF8_TO_TCHAR(s.c_str());
}

FString URigdocksBronze_MediaItem_Parse::AZ_GetMediaName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string s = BRONZE::AZ_GetMediaName_Parse(call->Get());
	return UTF8_TO_TCHAR(s.c_str());
}

FString URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string s = BRONZE::AZ_GetSelectedMediaName_Parse(call->Get());
	return UTF8_TO_TCHAR(s.c_str());
}

FString URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string s = BRONZE::AZ_GetTrackMediaName_Parse(call->Get());
	return UTF8_TO_TCHAR(s.c_str());
}

FString URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string s = BRONZE::AZ_GetSelectedTrackMediaName_Parse(call->Get());
	return UTF8_TO_TCHAR(s.c_str());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemName_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaName_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaName_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaName_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaName_Parse(call->Get());
}

bool URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return BRONZE::AZ_GetMediaItemMute_Parse(call->Get());
}

bool URigdocksBronze_MediaItem_Parse::AZ_GetMediaMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return BRONZE::AZ_GetMediaMute_Parse(call->Get());
}

bool URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return BRONZE::AZ_GetSelectedMediaMute_Parse(call->Get());
}

bool URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return BRONZE::AZ_GetTrackMediaMute_Parse(call->Get());
}

bool URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return BRONZE::AZ_GetSelectedTrackMediaMute_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemMute_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaMute_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaMute_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaMute_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaMute_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaMute_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemStartTimeSeconds_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaItemStartTimeSeconds_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaStartTimeSeconds_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaStartTimeSeconds_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaStartTimeSeconds_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedMediaStartTimeSeconds_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaStartTimeSeconds_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetTrackMediaStartTimeSeconds_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaStartTimeSeconds_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaStartTimeSeconds_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemStartTimeSeconds_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemStartTimeSeconds_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaStartTimeSeconds_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaStartTimeSeconds_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaStartTimeSeconds_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaStartTimeSeconds_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaStartTimeSeconds_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaStartTimeSeconds_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaStartTimeSeconds_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaStartTimeSeconds_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemStartTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaItemStartTimeFrame_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaStartTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaStartTimeFrame_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaStartTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedMediaStartTimeFrame_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaStartTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetTrackMediaStartTimeFrame_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaStartTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaStartTimeFrame_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemStartTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemStartTimeFrame_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaStartTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaStartTimeFrame_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaStartTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaStartTimeFrame_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaStartTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaStartTimeFrame_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaStartTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaStartTimeFrame_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemEndTime_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaItemEndTime_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaEndTime_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaEndTime_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaEndTime_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedMediaEndTime_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaEndTime_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetTrackMediaEndTime_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaEndTime_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaEndTime_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemEndTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaItemEndTimeFrame_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetMediaEndTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetMediaEndTimeFrame_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaEndTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedMediaEndTimeFrame_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaEndTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetTrackMediaEndTimeFrame_Parse(call->Get());
}

int URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaEndTimeFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaEndTimeFrame_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemLength_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaItemLength_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaLength_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaLength_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaLength_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedMediaLength_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaLength_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetTrackMediaLength_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaLength_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaLength_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemFade_Parse(URigdocksRpcCall* call, int& inType, double& inLen, int& outType, double& outLen)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(inType, inLen, outType, outLen) = BRONZE::AZ_GetMediaItemFade_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_GetMediaFade_Parse(URigdocksRpcCall* call, int& inType, double& inLen, int& outType, double& outLen)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(inType, inLen, outType, outLen) = BRONZE::AZ_GetMediaFade_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaFade_Parse(URigdocksRpcCall* call, int& inType, double& inLen, int& outType, double& outLen)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(inType, inLen, outType, outLen) = BRONZE::AZ_GetSelectedMediaFade_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaFade_Parse(URigdocksRpcCall* call, int& inType, double& inLen, int& outType, double& outLen)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(inType, inLen, outType, outLen) = BRONZE::AZ_GetTrackMediaFade_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaFade_Parse(URigdocksRpcCall* call, int& inType, double& inLen, int& outType, double& outLen)
{
	if (call == nullptr)
	{
		return;
	}

	std::tie(inType, inLen, outType, outLen) = BRONZE::AZ_GetSelectedTrackMediaFade_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemFade_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemFade_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaFade_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaFade_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaFade_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaFade_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaFade_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaFade_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaFade_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaFade_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaItemVolume_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaVolume_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedMediaVolume_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetTrackMediaVolume_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaVolume_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemVolume_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaVolume_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaVolume_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaVolume_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaVolume_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemAddVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemAddVolume_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaAddVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaAddVolume_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaAddVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaAddVolume_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaAddVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaAddVolume_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaAddVolume_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaAddVolume_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaItemPitch_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaPitch_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedMediaPitch_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetTrackMediaPitch_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaPitch_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemPitch_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaPitch_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaPitch_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaPitch_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaPitch_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemAddPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemAddPitch_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaAddPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaAddPitch_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaAddPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaAddPitch_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaAddPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaAddPitch_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaAddPitch_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaAddPitch_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaItemRate_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetMediaRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetMediaRate_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedMediaRate_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetTrackMediaRate_Parse(call->Get());
}

double URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return BRONZE::AZ_GetSelectedTrackMediaRate_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemRate_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaRate_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaRate_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaRate_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaRate_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_DeleteMediaItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_DeleteMediaItem_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_DeleteMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_DeleteMedia_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_DeleteSelectedMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_DeleteSelectedMedia_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_DeleteTrackMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_DeleteTrackMedia_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_DeleteSelectedTrackMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_DeleteSelectedTrackMedia_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_DeleteAllMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_DeleteAllMedia_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_DeleteAllSelectedMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_DeleteAllSelectedMedia_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_DeleteAllTrackMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_DeleteAllTrackMedia_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_DeleteAllSelectedTrackMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_DeleteAllSelectedTrackMedia_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaSelected_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaSelected_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaSelected_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaSelected_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetAllMediaSelected_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetAllMediaSelected_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemMoveToTrack_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemMoveToTrack_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaMoveToTrack_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaMoveToTrack_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaMoveToTrack_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaMoveToTrack_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaMoveToTrack_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaMoveToTrack_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaMoveToTrack_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaMoveToTrack_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemActiveTakeCopy_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemActiveTakeCopy_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaActiveTakeCopy_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaActiveTakeCopy_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaActiveTakeCopy_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaActiveTakeCopy_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaActiveTakeCopy_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaActiveTakeCopy_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaActiveTakeCopy_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaActiveTakeCopy_Parse(call->Get());
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertMediaOutItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertMediaOutItem_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertMediaTrackItemSecond_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertMediaTrackItemSecond_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertMediaTrackIdSecond_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertMediaTrackIdSecond_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertMediaSelectedTrackIdSecond_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertMediaSelectedTrackIdSecond_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertMediaTrackItemFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertMediaTrackItemFrame_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertMediaTrackIdFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertMediaTrackIdFrame_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertMediaSelectedTrackIdFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertMediaSelectedTrackIdFrame_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

TArray<UMediaItem*> URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<UMediaItem*>();
	}

	std::vector<MediaItem*> objArray = BRONZE::AZ_GetMediaItemList_Parse(call->Get());

	TArray<UMediaItem*> result;
	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);
		result.Add(uObj);
	}
	return result;
}

TArray<UMediaItem*> URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaItemList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<UMediaItem*>();
	}

	std::vector<MediaItem*> objArray = BRONZE::AZ_GetSelectedMediaItemList_Parse(call->Get());

	TArray<UMediaItem*> result;
	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);
		result.Add(uObj);
	}
	return result;
}

TArray<UMediaItem*> URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaItemList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<UMediaItem*>();
	}

	std::vector<MediaItem*> objArray = BRONZE::AZ_GetTrackMediaItemList_Parse(call->Get());

	TArray<UMediaItem*> result;
	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);
		result.Add(uObj);
	}
	return result;
}

TArray<UMediaItem*> URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaItemList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<UMediaItem*>();
	}

	std::vector<MediaItem*> objArray = BRONZE::AZ_GetSelectedTrackMediaItemList_Parse(call->Get());

	TArray<UMediaItem*> result;
	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);
		result.Add(uObj);
	}
	return result;
}

TArray<UMediaItem*> URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemListInTrackItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<UMediaItem*>();
	}

	std::vector<MediaItem*> objArray = BRONZE::AZ_GetMediaItemListInTrackItem_Parse(call->Get());

	TArray<UMediaItem*> result;
	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);
		result.Add(uObj);
	}
	return result;
}

void URigdocksBronze_MediaItem_Parse::AZ_SelectMediaItems_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SelectMediaItems_Parse(call->Get());
}

TArray<UMediaItem*> URigdocksBronze_MediaItem_Parse::AZ_InsertMediaItems_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<UMediaItem*>();
	}

	std::vector<MediaItem*> objArray = BRONZE::AZ_InsertMediaItems_Parse(call->Get());

	TArray<UMediaItem*> result;
	for (auto obj : objArray)
	{
		UMediaItem* uObj = NewObject<UMediaItem>();
		uObj->SetPointer((void*)obj);
		result.Add(uObj);
	}
	return result;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertNoteItemToTrack_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertNoteItemToTrack_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertNoteItemToTrackID_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertNoteItemToTrackID_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertNoteItemToSelectedTrackID_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertNoteItemToSelectedTrackID_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertNoteItemToTrackFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertNoteItemToTrackFrame_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertNoteItemToTrackIDFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertNoteItemToTrackIDFrame_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

UMediaItem* URigdocksBronze_MediaItem_Parse::AZ_InsertNoteItemToSelectedTrackIDFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	MediaItem* p = BRONZE::AZ_InsertNoteItemToSelectedTrackIDFrame_Parse(call->Get());

	UMediaItem* obj = NewObject<UMediaItem>();
	obj->SetPointer((void*)p);
	return obj;
}

FString URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemNotes_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string s = BRONZE::AZ_GetMediaItemNotes_Parse(call->Get());
	return UTF8_TO_TCHAR(s.c_str());
}

FString URigdocksBronze_MediaItem_Parse::AZ_GetMediaIDNotes_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string s = BRONZE::AZ_GetMediaIDNotes_Parse(call->Get());
	return UTF8_TO_TCHAR(s.c_str());
}

FString URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaIDNotes_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string s = BRONZE::AZ_GetSelectedMediaIDNotes_Parse(call->Get());
	return UTF8_TO_TCHAR(s.c_str());
}

FString URigdocksBronze_MediaItem_Parse::AZ_GetTrackMediaNotes_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string s = BRONZE::AZ_GetTrackMediaNotes_Parse(call->Get());
	return UTF8_TO_TCHAR(s.c_str());
}

FString URigdocksBronze_MediaItem_Parse::AZ_GetSelectedTrackMediaNotes_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string s = BRONZE::AZ_GetSelectedTrackMediaNotes_Parse(call->Get());
	return UTF8_TO_TCHAR(s.c_str());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaItemNotes_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaItemNotes_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetMediaIDNotes_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetMediaIDNotes_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedMediaIDNotes_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedMediaIDNotes_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetTrackMediaNotes_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetTrackMediaNotes_Parse(call->Get());
}

void URigdocksBronze_MediaItem_Parse::AZ_SetSelectedTrackMediaNotes_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	BRONZE::AZ_SetSelectedTrackMediaNotes_Parse(call->Get());
}

TArray<FMediaOverlap> URigdocksBronze_MediaItem_Parse::AZ_GetSelectedMediaOverlapList_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FMediaOverlap>();
	}

	std::vector<RADNODZ_client::Object::MediaOverlap> objArray = BRONZE::AZ_GetSelectedMediaOverlapList_Parse(call->Get());

	TArray<FMediaOverlap> result;
	for (auto obj : objArray)
	{
		FMediaOverlap uObj;
		uObj.Initialize(obj);
		result.Add(uObj);
	}
	return result;
}

void URigdocksBronze_MediaItem_Parse::AZ_SplitMediaItem_Parse(URigdocksRpcCall* call, UMediaItem*& leftItem, UMediaItem*& rightItem)
{
	if (call == nullptr)
	{
		return;
	}

	auto [oLeft, oRight] = BRONZE::AZ_SplitMediaItem_Parse(call->Get());

	leftItem = NewObject<UMediaItem>();
	leftItem->SetPointer((void*)oLeft);

	rightItem = NewObject<UMediaItem>();
	rightItem->SetPointer((void*)oRight);
}

void URigdocksBronze_MediaItem_Parse::AZ_SplitMediaId_Parse(URigdocksRpcCall* call, UMediaItem*& leftItem, UMediaItem*& rightItem)
{
	if (call == nullptr)
	{
		return;
	}

	auto [oLeft, oRight] = BRONZE::AZ_SplitMediaId_Parse(call->Get());

	leftItem = NewObject<UMediaItem>();
	leftItem->SetPointer((void*)oLeft);

	rightItem = NewObject<UMediaItem>();
	rightItem->SetPointer((void*)oRight);
}

void URigdocksBronze_MediaItem_Parse::AZ_SplitSelectedMediaId_Parse(URigdocksRpcCall* call, UMediaItem*& leftItem, UMediaItem*& rightItem)
{
	if (call == nullptr)
	{
		return;
	}

	auto [oLeft, oRight] = BRONZE::AZ_SplitSelectedMediaId_Parse(call->Get());

	leftItem = NewObject<UMediaItem>();
	leftItem->SetPointer((void*)oLeft);

	rightItem = NewObject<UMediaItem>();
	rightItem->SetPointer((void*)oRight);
}

void URigdocksBronze_MediaItem_Parse::AZ_SplitTrackMedia_Parse(URigdocksRpcCall* call, UMediaItem*& leftItem, UMediaItem*& rightItem)
{
	if (call == nullptr)
	{
		return;
	}

	auto [oLeft, oRight] = BRONZE::AZ_SplitTrackMedia_Parse(call->Get());

	leftItem = NewObject<UMediaItem>();
	leftItem->SetPointer((void*)oLeft);

	rightItem = NewObject<UMediaItem>();
	rightItem->SetPointer((void*)oRight);
}

void URigdocksBronze_MediaItem_Parse::AZ_SplitSelectedTrackMedia_Parse(URigdocksRpcCall* call, UMediaItem*& leftItem, UMediaItem*& rightItem)
{
	if (call == nullptr)
	{
		return;
	}

	auto [oLeft, oRight] = BRONZE::AZ_SplitSelectedTrackMedia_Parse(call->Get());

	leftItem = NewObject<UMediaItem>();
	leftItem->SetPointer((void*)oLeft);

	rightItem = NewObject<UMediaItem>();
	rightItem->SetPointer((void*)oRight);
}

FString URigdocksBronze_MediaItem_Parse::AZ_GetMediaItemSourcePath_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string s = BRONZE::AZ_GetMediaItemSourcePath_Parse(call->Get());
	return UTF8_TO_TCHAR(s.c_str());
}
