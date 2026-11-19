// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "TcpClient.h"
#include "Object/MediaOverlap.h"
#include <memory>
#include <string>
#include <vector>

class ReaProject;
class MediaItem;
class MediaItem_Take; 
class MediaTrack;

using namespace RADNODZ_client;

namespace RADNODZ_client::BRONZE
{
	using namespace Object;

	/****************** MediaItem *****************************/

	MediaItem* AZ_GetMediaItemSelect(TcpClient& tcpClient, ReaProject* projID, int itemID, int mediaType);
	MediaItem* AZ_GetMediaTrackItemSelect(TcpClient& tcpClient, ReaProject* projID, int trackID, int itemID, int mediaType);

	std::tuple<int, int> AZ_GetMediaTypeCount(TcpClient& tcpClient, ReaProject* projID, std::string mediaType);
	std::tuple<int, int> AZ_GetSelectedMediaTypeCount(TcpClient& tcpClient, ReaProject* projID, std::string mediaType);
	std::tuple<int, int> AZ_GetTrackItemMediaTypeCount(TcpClient& tcpClient, MediaTrack* track, std::string mediaType);
	std::tuple<int, int> AZ_GetTrackMediaTypeCount(TcpClient& tcpClient, ReaProject* projID, int trackID, std::string mediaType);
	std::tuple<int, int> AZ_GetSelectedTrackMediaTypeCount(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, std::string mediaType);

	int AZ_GetMediaItemBitRate(TcpClient& tcpClient, MediaItem* item);
	int AZ_GetMediaBitRate(TcpClient& tcpClient, ReaProject* projID, int itemID);
	int AZ_GetSelectedMediaBitRate(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	int AZ_GetTrackMediaBitRate(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	int AZ_GetSelectedTrackMediaBitRate(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	int AZ_GetMediaItemSampleRate(TcpClient& tcpClient, MediaItem* item);
	int AZ_GetMediaSampleRate(TcpClient& tcpClient, ReaProject* projID, int itemID);
	int AZ_GetSelectedMediaSampleRate(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	int AZ_GetTrackMediaSampleRate(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	int AZ_GetSelectedTrackMediaSampleRate(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	int AZ_GetMediaItemNumChannels(TcpClient& tcpClient, MediaItem* item);
	int AZ_GetMediaNumChannels(TcpClient& tcpClient, ReaProject* projID, int itemID);
	int AZ_GetSelectedMediaNumChannels(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	int AZ_GetTrackMediaNumChannels(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	int AZ_GetSelectedTrackMediaNumChannels(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	int AZ_GetMediaItemChannelMode(TcpClient& tcpClient, MediaItem* item);
	int AZ_GetMediaChannelMode(TcpClient& tcpClient, ReaProject* projID, int itemID);
	int AZ_GetSelectedMediaChannelMode(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	int AZ_GetTrackMediaChannelMode(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	int AZ_GetSelectedTrackMediaChannelMode(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	void AZ_SetMediaItemChannelMode(TcpClient& tcpClient, MediaItem* item, int setChannelMode);
	void AZ_SetMediaChannelMode(TcpClient& tcpClient, ReaProject* projID, int itemID, int setChannelMode);
	void AZ_SetSelectedMediaChannelMode(TcpClient& tcpClient, ReaProject* projID, int selectItemID, int setChannelMode);
	void AZ_SetTrackMediaChannelMode(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, int setChannelMode);
	void AZ_SetSelectedTrackMediaChannelMode(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, int setChannelMode);

	std::string AZ_GetMediaItemName(TcpClient& tcpClient, MediaItem* item);
	std::string AZ_GetMediaName(TcpClient& tcpClient, ReaProject* projID, int itemID);
	std::string AZ_GetSelectedMediaName(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	std::string AZ_GetTrackMediaName(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	std::string AZ_GetSelectedTrackMediaName(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	void AZ_SetMediaItemName(TcpClient& tcpClient, MediaItem* item, std::string setName);
	void AZ_SetMediaName(TcpClient& tcpClient, ReaProject* projID, int itemID, std::string setName);
	void AZ_SetSelectedMediaName(TcpClient& tcpClient, ReaProject* projID, int selectItemID, std::string setName);
	void AZ_SetTrackMediaName(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, std::string setName);
	void AZ_SetSelectedTrackMediaName(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, std::string setName);

	bool AZ_GetMediaItemMute(TcpClient& tcpClient, MediaItem* item);
	bool AZ_GetMediaMute(TcpClient& tcpClient, ReaProject* projID, int itemID);
	bool AZ_GetSelectedMediaMute(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	bool AZ_GetTrackMediaMute(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	bool AZ_GetSelectedTrackMediaMute(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	void AZ_SetMediaItemMute(TcpClient& tcpClient, MediaItem* item, bool setMute);
	void AZ_SetMediaMute(TcpClient& tcpClient, ReaProject* projID, int itemID, bool setMute);
	void AZ_SetSelectedMediaMute(TcpClient& tcpClient, ReaProject* projID, int selectItemID, bool setMute);
	void AZ_SetTrackMediaMute(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, bool setMute);
	void AZ_SetSelectedTrackMediaMute(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, bool setMute);

	double AZ_GetMediaItemStartTimeSeconds(TcpClient& tcpClient, MediaItem* item);
	double AZ_GetMediaStartTimeSeconds(TcpClient& tcpClient, ReaProject* projID, int itemID);
	double AZ_GetSelectedMediaStartTimeSeconds(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	double AZ_GetTrackMediaStartTimeSeconds(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	double AZ_GetSelectedTrackMediaStartTimeSeconds(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	int AZ_GetMediaItemStartTimeFrame(TcpClient& tcpClient, MediaItem* item);
	int AZ_GetMediaStartTimeFrame(TcpClient& tcpClient, ReaProject* projID, int itemID);
	int AZ_GetSelectedMediaStartTimeFrame(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	int AZ_GetTrackMediaStartTimeFrame(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	int AZ_GetSelectedTrackMediaStartTimeFrame(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	void AZ_SetMediaItemStartTimeSeconds(TcpClient& tcpClient, MediaItem* item, double setSecond);
	void AZ_SetMediaStartTimeSeconds(TcpClient& tcpClient, ReaProject* projID, int itemID, double setSecond);
	void AZ_SetSelectedMediaStartTimeSeconds(TcpClient& tcpClient, ReaProject* projID, int selectItemID, double setSecond);
	void AZ_SetTrackMediaStartTimeSeconds(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, double setSecond);
	void AZ_SetSelectedTrackMediaStartTimeSeconds(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, double setSecond);

	void AZ_SetMediaItemStartTimeFrame(TcpClient& tcpClient, MediaItem* item, double setFrame);
	void AZ_SetMediaStartTimeFrame(TcpClient& tcpClient, ReaProject* projID, int itemID, double setFrame);
	void AZ_SetSelectedMediaStartTimeFrame(TcpClient& tcpClient, ReaProject* projID, int selectItemID, double setFrame);
	void AZ_SetTrackMediaStartTimeFrame(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, double setFrame);
	void AZ_SetSelectedTrackMediaStartTimeFrame(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, double setFrame);

	double AZ_GetMediaItemEndTime(TcpClient& tcpClient, MediaItem* item);
	double AZ_GetMediaEndTime(TcpClient& tcpClient, ReaProject* projID, int itemID);
	double AZ_GetSelectedMediaEndTime(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	double AZ_GetTrackMediaEndTime(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	double AZ_GetSelectedTrackMediaEndTime(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	int AZ_GetMediaItemEndTimeFrame(TcpClient& tcpClient, MediaItem* item);
	int AZ_GetMediaEndTimeFrame(TcpClient& tcpClient, ReaProject* projID, int itemID);
	int AZ_GetSelectedMediaEndTimeFrame(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	int AZ_GetTrackMediaEndTimeFrame(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	int AZ_GetSelectedTrackMediaEndTimeFrame(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	double AZ_GetMediaItemLength(TcpClient& tcpClient, MediaItem* item);
	double AZ_GetMediaLength(TcpClient& tcpClient, ReaProject* projID, int itemID);
	double AZ_GetSelectedMediaLength(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	double AZ_GetTrackMediaLength(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	double AZ_GetSelectedTrackMediaLength(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	std::tuple<int, double, int, double> AZ_GetMediaItemFade(TcpClient& tcpClient, MediaItem* item);
	std::tuple<int, double, int, double> AZ_GetMediaFade(TcpClient& tcpClient, ReaProject* projID, int itemID);
	std::tuple<int, double, int, double> AZ_GetSelectedMediaFade(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	std::tuple<int, double, int, double> AZ_GetTrackMediaFade(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	std::tuple<int, double, int, double> AZ_GetSelectedTrackMediaFade(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	void AZ_SetMediaItemFade(TcpClient& tcpClient, MediaItem* item, int inType, double inLen, int outType, double outLen);
	void AZ_SetMediaFade(TcpClient& tcpClient, ReaProject* projID, int itemID, int inType, double inLen, int outType, double outLen);
	void AZ_SetSelectedMediaFade(TcpClient& tcpClient, ReaProject* projID, int selectItemID, int inType, double inLen, int outType, double outLen);
	void AZ_SetTrackMediaFade(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, int inType, double inLen, int outType, double outLen);
	void AZ_SetSelectedTrackMediaFade(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, int inType, double inLen, int outType, double outLen);

	double AZ_GetMediaItemVolume(TcpClient& tcpClient, MediaItem* item);
	double AZ_GetMediaVolume(TcpClient& tcpClient, ReaProject* projID, int itemID);
	double AZ_GetSelectedMediaVolume(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	double AZ_GetTrackMediaVolume(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	double AZ_GetSelectedTrackMediaVolume(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	void AZ_SetMediaItemVolume(TcpClient& tcpClient, MediaItem* item, double volume);
	void AZ_SetMediaVolume(TcpClient& tcpClient, ReaProject* projID, int itemID, double volume);
	void AZ_SetSelectedMediaVolume(TcpClient& tcpClient, ReaProject* projID, int itemID, double volume);
	void AZ_SetTrackMediaVolume(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, double volume);
	void AZ_SetSelectedTrackMediaVolume(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, double volume);

	void AZ_SetMediaItemAddVolume(TcpClient& tcpClient, MediaItem* item, double addVolume);
	void AZ_SetMediaAddVolume(TcpClient& tcpClient, ReaProject* projID, int itemID, double addVolume);
	void AZ_SetSelectedMediaAddVolume(TcpClient& tcpClient, ReaProject* projID, int itemID, double addVolume);
	void AZ_SetTrackMediaAddVolume(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, double addVolume);
	void AZ_SetSelectedTrackMediaAddVolume(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, double addVolume);

	double AZ_GetMediaItemPitch(TcpClient& tcpClient, MediaItem* item);
	double AZ_GetMediaPitch(TcpClient& tcpClient, ReaProject* projID, int itemID);
	double AZ_GetSelectedMediaPitch(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	double AZ_GetTrackMediaPitch(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	double AZ_GetSelectedTrackMediaPitch(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	void AZ_SetMediaItemPitch(TcpClient& tcpClient, MediaItem* item, double setPitch);
	void AZ_SetMediaPitch(TcpClient& tcpClient, ReaProject* projID, int itemID, double setPitch);
	void AZ_SetSelectedMediaPitch(TcpClient& tcpClient, ReaProject* projID, int selectItemID, double setPitch);
	void AZ_SetTrackMediaPitch(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, double setPitch);
	void AZ_SetSelectedTrackMediaPitch(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, double setPitch);

	void AZ_SetMediaItemAddPitch(TcpClient& tcpClient, MediaItem* item, double addPitch);
	void AZ_SetMediaAddPitch(TcpClient& tcpClient, ReaProject* projID, int itemID, double addPitch);
	void AZ_SetSelectedMediaAddPitch(TcpClient& tcpClient, ReaProject* projID, int selectItemID, double addPitch);
	void AZ_SetTrackMediaAddPitch(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, double addPitch);
	void AZ_SetSelectedTrackMediaAddPitch(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, double addPitch);

	double AZ_GetMediaItemRate(TcpClient& tcpClient, MediaItem* item);
	double AZ_GetMediaRate(TcpClient& tcpClient, ReaProject* projID, int itemID);
	double AZ_GetSelectedMediaRate(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	double AZ_GetTrackMediaRate(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	double AZ_GetSelectedTrackMediaRate(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	void AZ_SetMediaItemRate(TcpClient& tcpClient, MediaItem* item, double setRate);
	void AZ_SetMediaRate(TcpClient& tcpClient, ReaProject* projID, int itemID, double setRate);
	void AZ_SetSelectedMediaRate(TcpClient& tcpClient, ReaProject* projID, int selectItemID, double setRate);
	void AZ_SetTrackMediaRate(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, double setRate);
	void AZ_SetSelectedTrackMediaRate(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, double setRate);

	void AZ_DeleteMediaItem(TcpClient& tcpClient, MediaItem* item);
	void AZ_DeleteMedia(TcpClient& tcpClient, ReaProject* projID, int itemID);
	void AZ_DeleteSelectedMedia(TcpClient& tcpClient, ReaProject* projID, int selectItemID);
	void AZ_DeleteTrackMedia(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID);
	void AZ_DeleteSelectedTrackMedia(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID);

	void AZ_DeleteAllMedia(TcpClient& tcpClient, ReaProject* projID);
	void AZ_DeleteAllSelectedMedia(TcpClient& tcpClient, ReaProject* projID);
	void AZ_DeleteAllTrackMedia(TcpClient& tcpClient, ReaProject* projID, int trackID);
	void AZ_DeleteAllSelectedTrackMedia(TcpClient& tcpClient, ReaProject* projID, int selectTrackID);

	void AZ_SetMediaSelected(TcpClient& tcpClient, ReaProject* projID, int itemID, bool selectType);
	void AZ_SetTrackMediaSelected(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, bool selectType);
	void AZ_SetAllMediaSelected(TcpClient& tcpClient, ReaProject* projID, bool selectType);

	void AZ_SetMediaItemMoveToTrack(TcpClient& tcpClient, MediaItem* item, int moveTrackID);
	void AZ_SetMediaMoveToTrack(TcpClient& tcpClient, ReaProject* projID, int itemID, int moveTrackID);
	void AZ_SetSelectedMediaMoveToTrack(TcpClient& tcpClient, ReaProject* projID, int selectItemID, int moveTrackID);
	void AZ_SetTrackMediaMoveToTrack(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, int moveTrackID);
	void AZ_SetSelectedTrackMediaMoveToTrack(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, int moveTrackID);

	void AZ_SetMediaItemActiveTakeCopy(TcpClient& tcpClient, MediaItem* item, int setTrackID, double setPosition);
	void AZ_SetMediaActiveTakeCopy(TcpClient& tcpClient, ReaProject* projID, int itemID, int setTrackID, double setPosition);
	void AZ_SetSelectedMediaActiveTakeCopy(TcpClient& tcpClient, ReaProject* projID, int selectItemID, int setTrackID, double setPosition);
	void AZ_SetTrackMediaActiveTakeCopy(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackItemID, int setTrackID, double setPosition);
	void AZ_SetSelectedTrackMediaActiveTakeCopy(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int trackItemID, int setTrackID, double setPosition);

	MediaItem* AZ_InsertMediaOutItem(TcpClient& tcpClient, ReaProject* projID, std::string filePath);
	MediaItem* AZ_InsertMediaTrackItemSecond(TcpClient& tcpClient, MediaTrack* track, std::string filePath, int startEndType, double setSecond);
	MediaItem* AZ_InsertMediaTrackIdSecond(TcpClient& tcpClient, ReaProject* projID, int trackID, std::string filePath, int startEndType, double setSecond);
	MediaItem* AZ_InsertMediaSelectedTrackIdSecond(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, std::string filePath, int startEndType, double setSecond);
	MediaItem* AZ_InsertMediaTrackItemFrame(TcpClient& tcpClient, MediaTrack* track, std::string filePath, int startEndType, int setFrame);
	MediaItem* AZ_InsertMediaTrackIdFrame(TcpClient& tcpClient, ReaProject* projID, int trackID, std::string filePath, int startEndType, int setFrame);
	MediaItem* AZ_InsertMediaSelectedTrackIdFrame(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, std::string filePath, int startEndType, int setFrame);

	std::vector<MediaItem*> AZ_GetMediaItemList(TcpClient& tcpClient, ReaProject* proj);
	std::vector<MediaItem*> AZ_GetSelectedMediaItemList(TcpClient& tcpClient, ReaProject* proj);
	std::vector<MediaItem*> AZ_GetTrackMediaItemList(TcpClient& tcpClient, ReaProject* proj, int trackID);
	std::vector<MediaItem*> AZ_GetSelectedTrackMediaItemList(TcpClient& tcpClient, ReaProject* proj, int SelectedTrackID);
	std::vector<MediaItem*> AZ_GetMediaItemListInTrackItem(TcpClient& tcpClient, MediaTrack* track);
	void AZ_SelectMediaItems(TcpClient& tcpClient, std::vector<MediaItem*> itemList,  bool isSelected);
	std::vector<MediaItem*> AZ_InsertMediaItems(TcpClient& tcpClient, MediaTrack* track, std::vector<std::string> sourcePathList, double startPos, double interval, int timeUnitType);

	MediaItem* AZ_InsertNoteItemToTrack(TcpClient& tcpClient, MediaTrack* track, double startSecond, double length, std::string text);
	MediaItem* AZ_InsertNoteItemToTrackID(TcpClient& tcpClient, ReaProject* proj, int trackID, double startSecond, double length, std::string text);
	MediaItem* AZ_InsertNoteItemToSelectedTrackID(TcpClient& tcpClient, ReaProject* proj, int selectTrackID, double startSecond, double length, std::string text);
	MediaItem* AZ_InsertNoteItemToTrackFrame(TcpClient& tcpClient, MediaTrack* track, double startFrame, double lengthFrame, std::string text);
	MediaItem* AZ_InsertNoteItemToTrackIDFrame(TcpClient& tcpClient, ReaProject* proj, int trackID, double startFrame, double lengthFrame, std::string text);
	MediaItem* AZ_InsertNoteItemToSelectedTrackIDFrame(TcpClient& tcpClient, ReaProject* proj, int selectTrackID, double startFrame, double lengthFrame, std::string text);
	
	std::string AZ_GetMediaItemNotes(TcpClient& tcpClient, MediaItem* item);
	std::string AZ_GetMediaIDNotes(TcpClient& tcpClient, ReaProject* proj, int itemID);
	std::string AZ_GetSelectedMediaIDNotes(TcpClient& tcpClient, ReaProject* proj, int selectedItemID);
	std::string AZ_GetTrackMediaNotes(TcpClient& tcpClient, ReaProject* proj, int trackID, int trackItemID);
	std::string AZ_GetSelectedTrackMediaNotes(TcpClient& tcpClient, ReaProject* proj, int selectedTrackID, int trackItemID);
	void AZ_SetMediaItemNotes(TcpClient& tcpClient, MediaItem* item, std::string text);
	void AZ_SetMediaIDNotes(TcpClient& tcpClient, ReaProject* proj, int itemID, std::string text);
	void AZ_SetSelectedMediaIDNotes(TcpClient& tcpClient, ReaProject* proj, int selectedItemID, std::string text);
	void AZ_SetTrackMediaNotes(TcpClient& tcpClient, ReaProject* proj, int trackID, int trackItemID, std::string text);
	void AZ_SetSelectedTrackMediaNotes(TcpClient& tcpClient, ReaProject* proj, int selectedTrackID, int trackItemID, std::string text);
	
	std::vector<MediaOverlap> AZ_GetSelectedMediaOverlapList(TcpClient& tcpClient, ReaProject* proj);
	std::tuple<MediaItem*, MediaItem*> AZ_SplitMediaItem(TcpClient& tcpClient, MediaItem* item, double splitPosition);
	std::tuple<MediaItem*, MediaItem*> AZ_SplitMediaId(TcpClient& tcpClient, ReaProject* proj, int itemID, double splitPosition);
	std::tuple<MediaItem*, MediaItem*> AZ_SplitSelectedMediaId(TcpClient& tcpClient, ReaProject* proj, int selectedItemID, double splitPosition);
	std::tuple<MediaItem*, MediaItem*> AZ_SplitTrackMedia(TcpClient& tcpClient, ReaProject* proj, int trackID, int trackItemID, double splitPosition);
	std::tuple<MediaItem*, MediaItem*> AZ_SplitSelectedTrackMedia(TcpClient& tcpClient, ReaProject* proj, int selectedTrackID, int trackItemID, double splitPosition);

	std::string AZ_GetMediaItemSourcePath(TcpClient& tcpClient, MediaItem* item);

	/****************** Cursor *****************************/

	double AZ_GetPlayCursorPosition(TcpClient& tcpClient, ReaProject* projID);
	double AZ_SetPlayCursorPosition(TcpClient& tcpClient, ReaProject* projID, int startEndType, double setPos);
	int AZ_SetPlayCursorPositionFrame(TcpClient& tcpClient, ReaProject* projID, int startEndType, int setPosFrame);

	double AZ_StartPlaying(TcpClient& tcpClient);
	double AZ_StartRecording(TcpClient& tcpClient);
	double AZ_StopPlaying(TcpClient& tcpClient);
	double AZ_PausePlaying(TcpClient& tcpClient);

	double AZ_SetPlayCursorPositionAdd(TcpClient& tcpClient, ReaProject* projID, double addPos);
	int AZ_SetPlayCursorPositionAddFrame(TcpClient& tcpClient, ReaProject* projID, int addPosFrame);

	double AZ_SetPlayCursorStart(TcpClient& tcpClient);
	double AZ_SetPlayCursorEnd(TcpClient& tcpClient);

}
