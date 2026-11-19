// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "TcpClient.h"
#include "Object/Section.h"
#include "Object/TrackDetail.h"
#include "Object/Peak.h"
#include "Object/Marker.h"
#include "Object/PathInfo.h"
#include "Object/HttpResponse.h"
#include "Object/SlackMessage.h"
#include "Object/WhisperSegment.h"
#include "Object/TrackSendProperties.h"
#include <memory>
#include <string>
#include <vector>
#include <map>

class ReaProject;
class MediaItem;
class MediaItem_Take; 
class MediaTrack;
class TranscriptionContext;

using namespace RADNODZ_client;

namespace RADNODZ_client::SILVER
{
	using namespace Object;

	/****************** track *****************************/

	MediaTrack* AZ_GetTrackItemSelect(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackType);
	int AZ_GetTrackItemIndex(TcpClient& tcpClient, MediaTrack* track);
	std::tuple<MediaTrack*, int> AZ_GetSelectedTrackFirstInfo(TcpClient& tcpClient, ReaProject* projID);
	std::tuple<MediaTrack*, int> AZ_InsertTrackSearchUniqueOnly(TcpClient& tcpClient, ReaProject* projID, std::string setTrackName);
	int AZ_GetTrackCountSelect(TcpClient& tcpClient, ReaProject* projID, int trackType);

	std::string AZ_GetTrackItemName(TcpClient& tcpClient, MediaTrack* track);
	std::string AZ_GetTrackIdName(TcpClient& tcpClient, ReaProject* projID, int trackID);
	std::string AZ_GetSelectedTrackIdName(TcpClient& tcpClient, ReaProject* projID, int selectTrackID);

	int AZ_GetTrackItemDepth(TcpClient& tcpClient, MediaTrack* track);
	int AZ_GetTrackIdDepth(TcpClient& tcpClient, ReaProject* projID, int trackID);
	int AZ_GetSelectedTrackIdDepth(TcpClient& tcpClient, ReaProject* projID, int selectTrackID);

	void AZ_SetTrackItemDepth(TcpClient& tcpClient, MediaTrack* track, int setDepth);
	void AZ_SetTrackIdDepth(TcpClient& tcpClient, ReaProject* projID, int trackID, int setDepth);
	void AZ_SetSelectedTrackIdDepth(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int setDepth);

	void AZ_SetTrackItemName(TcpClient& tcpClient, MediaTrack* track, std::string setName);
	void AZ_SetTrackIdName(TcpClient& tcpClient, ReaProject* projID, int trackID, std::string setName);
	void AZ_SetSelectedTrackIdName(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, std::string setName);

	bool AZ_GetTrackItemMute(TcpClient& tcpClient, MediaTrack* track);
	bool AZ_GetTrackIdMute(TcpClient& tcpClient, ReaProject* projID, int trackID);
	bool AZ_GetSelectedTrackIdMute(TcpClient& tcpClient, ReaProject* projID, int selecteTrackID);

	void AZ_SetTrackItemMute(TcpClient& tcpClient, MediaTrack* track, bool setMute);
	void AZ_SetTrackIdMute(TcpClient& tcpClient, ReaProject* projID, int trackID, bool setMute);
	void AZ_SetSelectedTrackIdMute(TcpClient& tcpClient, ReaProject* projID, int selecteTrackID, bool setMute);

	void AZ_SetTrackAllMuteOFF(TcpClient& tcpClient, ReaProject* projID);
	bool AZ_GetTrackItemSolo(TcpClient& tcpClient, MediaTrack* track);
	bool AZ_GetTrackIdSolo(TcpClient& tcpClient, ReaProject* projID, int trackID);
	bool AZ_GetSelectedTrackIdSolo(TcpClient& tcpClient, ReaProject* projID, int selectTrackID);

	void AZ_SetTrackItemSolo(TcpClient& tcpClient, MediaTrack* track, bool setSolo);
	void AZ_SetTrackIdSolo(TcpClient& tcpClient, ReaProject* projID, int trackID, bool setSolo);
	void AZ_SetSelectedTrackIdSolo(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, bool setSolo);
	void AZ_SetTrackAllSoloOFF(TcpClient& tcpClient, ReaProject* projID);

	std::tuple<int, int, int, int> AZ_GetTrackItemColor(TcpClient& tcpClient, MediaTrack* track);
	std::tuple<int, int, int, int> AZ_GetTrackIdColor(TcpClient& tcpClient, ReaProject* projID, int trackID);
	std::tuple<int, int, int, int> AZ_GetSelectedTrackIdColor(TcpClient& tcpClient, ReaProject* projID, int selectTrackID);

	void AZ_SetTrackItemColor(TcpClient& tcpClient, MediaTrack* track, int setColor);
	void AZ_SetTrackIdColor(TcpClient& tcpClient, ReaProject* projID, int trackID, int setColor);
	void AZ_SetSelectedTrackIdColor(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int setColor);

	void AZ_SetTrackItemRGB(TcpClient& tcpClient, MediaTrack* track, int setR, int setG, int setB);
	void AZ_SetTrackIdRGB(TcpClient& tcpClient, ReaProject* projID, int trackID, int setR, int setG, int setB);
	void AZ_SetSelectedTrackIdRGB(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int setR, int setG, int setB);

	void AZ_SetTrackIdSelected(TcpClient& tcpClient, ReaProject* projID, int trackID, bool setSelectType);
	void AZ_SetTrackItemOnlySelected(TcpClient& tcpClient, MediaTrack* track);
	void AZ_SetTrackIdOnlySelected(TcpClient& tcpClient, ReaProject* projID, int trackID);

	double AZ_GetTrackItemVolume(TcpClient& tcpClient, MediaTrack* track);
	double AZ_GetTrackIdVolume(TcpClient& tcpClient, ReaProject* projID, int trackID);
	double AZ_GetSelectedTrackIdVolume(TcpClient& tcpClient, ReaProject* projID, int selectTrackID);

	void AZ_SetTrackItemVolume(TcpClient& tcpClient, MediaTrack* track, double setVolume);
	void AZ_SetTrackIdVolume(TcpClient& tcpClient, ReaProject* projID, int trackID, double setVolume);
	void AZ_SetSelectedTrackIdVolume(TcpClient& tcpClient, ReaProject* projID, int trackID, double setVolume);

	void AZ_SetTrackItemAddVolume(TcpClient& tcpClient, MediaTrack* track, double addVolume);
	void AZ_SetTrackIdAddVolume(TcpClient& tcpClient, ReaProject* projID, int trackID, double addVolume);
	void AZ_SetSelectedTrackIdAddVolume(TcpClient& tcpClient, ReaProject* projID, int trackID, double addVolume);

	void AZ_DeleteTrackIdSelect(TcpClient& tcpClient, ReaProject* projID, int trackID, int trackType);

	std::vector<MediaTrack*> AZ_GetTrackItemList(TcpClient& tcpClient, ReaProject* project);
	std::tuple<std::vector<MediaTrack*>, int> AZ_GetTrackItemChildList(TcpClient& tcpClient, MediaTrack* track, bool isRecursive = false);
	std::tuple<std::vector<MediaTrack*>, int> AZ_GetTrackIdChildList(TcpClient& tcpClient, ReaProject* project, int trackID, bool isRecursive = false);
	std::tuple<std::vector<MediaTrack*>, int> AZ_GetSelectedTrackIdChildList(TcpClient& tcpClient, ReaProject* project, int selectTrackID, bool isRecursive = false);
	std::vector<TrackDetail> AZ_GetTrackListDetail(TcpClient& tcpClient, ReaProject* proj);
	std::vector<TrackDetail> AZ_GetSelectedTrackListDetail(TcpClient& tcpClient, ReaProject* proj);
	std::vector<TrackDetail> AZ_GetTrackDepthList(TcpClient& tcpClient, ReaProject* project);
	std::vector<TrackDetail> AZ_GetSamePerfectTrackNameList(TcpClient& tcpClient, ReaProject* project, std::string checkName);
	std::vector<TrackDetail> AZ_GetIncludePerfectTrackNameList(TcpClient& tcpClient, ReaProject* project, std::string checkName);
	std::vector<MediaTrack*> AZ_GetSelectedTrackItemList(TcpClient& tcpClient, ReaProject* project);

	std::tuple<MediaTrack*, int> AZ_InsertUniqueChildTrack(TcpClient& tcpClient, MediaTrack* parentTrack, std::string setName);
	MediaTrack* AZ_InsertTrackId(TcpClient& tcpClient, ReaProject* project, int trackId, std::string setName = "");
	std::tuple<MediaTrack*, int> AZ_InsertChildTrack(TcpClient& tcpClient, MediaTrack* parentTrack, std::string setName);

	bool AZ_GetTrackItemRecording(TcpClient& tcpClient, MediaTrack* track);
	bool AZ_GetTrackIdRecording(TcpClient& tcpClient, ReaProject* projID, int trackID);
	bool AZ_GetSelectedTrackIdRecording(TcpClient& tcpClient, ReaProject* projID, int selectTrackID);

	std::vector<TrackDetail> AZ_GetRecordingTrackList(TcpClient& tcpClient, ReaProject* projID);
	void AZ_SetTrackItemRecording(TcpClient& tcpClient, MediaTrack* track, bool setRec);
	void AZ_SetTrackIdRecording(TcpClient& tcpClient, ReaProject* projID, int trackID, bool setRec);
	void AZ_SetSelectedTrackIdRecording(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, bool setRec);

	// record monitoring: 0 = Off / 1 = On / 2 = Auto (not when playing). Get returns -1 on failure
	int AZ_GetTrackItemRecordMonitoring(TcpClient& tcpClient, MediaTrack* track);
	int AZ_GetTrackIdRecordMonitoring(TcpClient& tcpClient, ReaProject* projID, int trackID);
	int AZ_GetSelectedTrackIdRecordMonitoring(TcpClient& tcpClient, ReaProject* projID, int selectTrackID);
	void AZ_SetTrackItemRecordMonitoring(TcpClient& tcpClient, MediaTrack* track, int monitorMode);
	void AZ_SetTrackIdRecordMonitoring(TcpClient& tcpClient, ReaProject* projID, int trackID, int monitorMode);
	void AZ_SetSelectedTrackIdRecordMonitoring(TcpClient& tcpClient, ReaProject* projID, int selectTrackID, int monitorMode);
	void AZ_SetTrackAllRecording(TcpClient& tcpClient, ReaProject* projID, bool setRec);
	void AZ_SetTrackItemOnlyRecording(TcpClient& tcpClient, MediaTrack* track, bool setRec);
	void AZ_SetTrackIdOnlyRecording(TcpClient& tcpClient, ReaProject* projID, int trackID, bool setRec);
	void AZ_SetSelectedTrackIdOnlyRecording(TcpClient& tcpClient, ReaProject* projID, int selectedTrackID, bool setRec);
	
	double AZ_GetTrackItemCurrentPeak(TcpClient & tcpClient, MediaTrack * track, int chIndex);
	double AZ_GetTrackItemCurrentPeakDB(TcpClient& tcpClient, MediaTrack* track, int chIndex);
	double AZ_GetTrackItemHoldPeakDB(TcpClient& tcpClient, MediaTrack* track, int chIndex);
	void   AZ_ResetTrackItemHoldPeak(TcpClient& tcpClient, MediaTrack* track, int chIndex);	
	
	int AZ_GetTrackItemNumChannels(TcpClient& tcpClient, MediaTrack* track);
	int AZ_GetTrackIdNumChannels(TcpClient& tcpClient, ReaProject* projID, int trackID);
	int AZ_GetSelectedTrackIdNumChannels(TcpClient& tcpClient, ReaProject* projID, int selectedTrackID);

	void AZ_SetTrackItemNumChannels(TcpClient& tcpClient, MediaTrack* track, int numChs);
	void AZ_SetTrackIdNumChannels(TcpClient& tcpClient, ReaProject* projID, int trackID, int numChs);
	void AZ_SetSelectedTrackIdNumChannels(TcpClient& tcpClient, ReaProject* projID, int selectedTrackID, int numChs);

	std::tuple<int, int, int> AZ_GetTrackItemInputChannel(TcpClient& tcpClient, MediaTrack* track);
	std::tuple<int, int, int> AZ_GetTrackIdInputChannel(TcpClient& tcpClient, ReaProject* projID, int trackID);
	std::tuple<int, int, int> AZ_GetSelectedTrackIdInputChannel(TcpClient& tcpClient, ReaProject* projID, int selectedTrackID);

	void AZ_SetTrackItemInputChannel(TcpClient& tcpClient, MediaTrack* track, int firstChannelIndex, int numChannels, int inputChType = 0);
	void AZ_SetTrackIdInputChannel(TcpClient& tcpClient, ReaProject* projID, int trackID, int firstChannelIndex, int numChannels, int inputChType = 0);
	void AZ_SetSelectedTrackIdInputChannel(TcpClient& tcpClient, ReaProject* projID, int selectedTrackID, int firstChannelIndex, int numChannels, int inputChType = 0);

	std::tuple<int, bool, bool> AZ_GetTrackItemMeterMode(TcpClient& tcpClient, MediaTrack* track);
	void AZ_SetTrackItemMeterMode(TcpClient& tcpClient, MediaTrack* track, int meterMode, bool enabled, bool lufsChannels1And2Only);

	bool AZ_GetTrackItemFxEnabled(TcpClient& tcpClient, MediaTrack* track, int fxIndex);
	void AZ_SetTrackItemFxEnabled(TcpClient& tcpClient, MediaTrack* track, int fxIndex, bool enabled);

	std::tuple<bool, int, int> AZ_GetTrackItemMainSend(TcpClient& tcpClient, MediaTrack* track);
	void AZ_SetTrackItemMainSend(TcpClient& tcpClient, MediaTrack* track, bool sendParent, int firstChIndex, int numChs);
	std::vector<TrackDetail> AZ_GetTrackItemSendList(TcpClient& tcpClient, MediaTrack* track);
	int AZ_AddTrackItemSend(TcpClient& tcpClient, MediaTrack* track, MediaTrack* destTrack, bool isUnique = false);
	TrackSendProperties AZ_GetTrackItemSendProperties(TcpClient& tcpClient, MediaTrack* track, int sendIndex);
	void AZ_SetTrackItemSendProperties(TcpClient& tcpClient, MediaTrack* track, int sendIndex, TrackSendProperties props);
	void AZ_DeleteTrackItemSend(TcpClient& tcpClient, MediaTrack* track, int sendIndex);

	/****************** peak *****************************/

	std::vector<Peak> AZ_GetMediaItemPeakList(TcpClient& tcpClient, MediaItem* item);
	std::vector<Peak> AZ_GetMediaIdPeakList(TcpClient& tcpClient, ReaProject* project, int itemID);
	std::vector<Peak> AZ_GetSelectedMediaIdPeakList(TcpClient& tcpClient, ReaProject* project, int selectItemID);

	/****************** loudness *****************************/

	std::tuple<double, double> AZ_GetMediaItemLoudnessMaxMomentary(TcpClient& tcpClient, MediaItem* item);
	std::tuple<double, double> AZ_GetMediaIdLoudnessMaxMomentary(TcpClient& tcpClient, ReaProject* proj, int itemID);
	std::tuple<double, double> AZ_GetSelectedMediaIdLoudnessMaxMomentary(TcpClient& tcpClient, ReaProject* proj, int selectItemID);
	double AZ_SetMediaItemLoudnessMaxMomentary(TcpClient& tcpClient, MediaItem* item, double setMaxMomentary);
	double AZ_SetMediaIdLoudnessMaxMomentary(TcpClient& tcpClient, ReaProject* proj, int itemID, double setMaxMomentary);
	double AZ_SetSelectedMediaIdLoudnessMaxMomentary(TcpClient& tcpClient, ReaProject* proj, int selectItemID, double setMaxMomentary);
	std::vector<Section> AZ_GetSilentSectionListInMediaItem(TcpClient& tcpClient, MediaItem* item, double minSilentDulation = 0.5, double threshold = -60);
	std::vector<Section> AZ_GetAudioSectionListInMediaItem(TcpClient& tcpClient, MediaItem* item, double maxSilentDulation = 0.5, double threshold = -60);

	/****************** プロジェクト *****************************/

	std::string AZ_SetProjectInfoRecPath(TcpClient& tcpClient, ReaProject* projID, std::string path);
	std::tuple<std::string, std::string> AZ_SetProjectInfoRecAddPath(TcpClient& tcpClient, ReaProject* projID, std::string addPath);
	std::tuple<std::string, std::string> AZ_SetProjectPathFolder(TcpClient& tcpClient, std::string folderName, int timeAddType);
	std::tuple<std::string, std::string> AZ_SetResoucePathFolder(TcpClient& tcpClient, std::string folderName, int timeAddType);
	bool AZ_GetRepeat(TcpClient& tcpClient, ReaProject* projID);
	void AZ_SetRepeate(TcpClient& tcpClient, ReaProject* projID, bool repeat);
	bool AZ_GetPlayState(TcpClient& tcpClient, ReaProject* projID);
	bool AZ_GetRecordState(TcpClient& tcpClient, ReaProject* projID);
	bool AZ_GetPauseState(TcpClient& tcpClient, ReaProject* projID);
	bool AZ_GetStopState(TcpClient& tcpClient, ReaProject* projID);
	std::string AZ_GetProjectName(TcpClient& tcpClient, ReaProject* projID);
	std::string AZ_GetProjectPath(TcpClient& tcpClient, ReaProject* projID);
	ReaProject* AZ_OpenProject(TcpClient& tcpClient, std::string projectPath);
	void AZ_SaveProject(TcpClient& tcpClient, ReaProject* projID, std::string dstPath);
	void AZ_CloseProject(TcpClient& tcpClient, ReaProject* projID);
	void AZ_SwitchProject(TcpClient& tcpClient, ReaProject* projID);
	ReaProject* AZ_GetProjectFromName(TcpClient& tcpClient, std::string projectName);

	// REAPER process: launch (waits until the RIGDOCKS extension is reachable) / is running / quit.
	// LaunchReaper on a running REAPER opens projectPath in a new tab (does nothing if empty).
	// QuitReaper: save = true saves changed projects then quits / false terminates without saving.
	void AZ_LaunchReaper(TcpClient& tcpClient, std::string projectPath = "");
	bool AZ_IsReaperRunning(TcpClient& tcpClient);
	void AZ_QuitReaper(TcpClient& tcpClient, bool save = true);

	/****************** device *****************************/

	std::vector<std::string> AZ_GetLocalIpAddresse(TcpClient& tcpClient);
	std::string AZ_GetDeviceName(TcpClient& tcpClient);
	double AZ_GetRecordingSampleRate(TcpClient& tcpClient, ReaProject* projID);
	int AZ_GetRecordingBitDepth(TcpClient& tcpClient, ReaProject* projID);
	int AZ_CountInputChannel(TcpClient& tcpClient);
	std::vector<std::string> AZ_GetInputChannelList(TcpClient& tcpClient);
	int AZ_CountOutputChannel(TcpClient& tcpClient);
	std::vector<std::string> AZ_GetOutputChannelList(TcpClient& tcpClient);

	/****************** リージョン*****************************/

	int AZ_GetRegionMarkerIndex(TcpClient& tcpClient, ReaProject* projID, int regionId);
	std::tuple<double, double> AZ_GetRegionMarkerPosition(TcpClient& tcpClient, ReaProject* projID, int regionId);
	std::string AZ_GetRegionMarkerName(TcpClient& tcpClient, ReaProject* projID, int regionId);
	int AZ_GetRegionMarkerColor(TcpClient& tcpClient, ReaProject* projID, int regionId);
	void AZ_SetRegionMarkerPosition(TcpClient& tcpClient, ReaProject* projID, int regionId, double setStartPos, double setEndPos);
	void AZ_SetRegionMarkerColor(TcpClient& tcpClient, ReaProject* projID, int regionId, int setColor);
	void AZ_SetRegionMarkerName(TcpClient& tcpClient, ReaProject* projID, int regionId, std::string setName);
	int AZ_AddRegionMarker(TcpClient& tcpClient, ReaProject* projID, double setStartPos, double setEndPos, std::string setName, int setRegionIndex, int setColor);
	void AZ_DeleteAllRegion(TcpClient& tcpClient, ReaProject* projID);
	Marker AZ_GetRegionObject(TcpClient& tcpClient, ReaProject* proj, int regionId);
	std::vector<Marker> AZ_GetRegionMarkerListMediaItem(TcpClient& tcpClient, MediaItem* item);
	Marker AZ_AddRegionMarkerMediaItem(TcpClient& tcpClient, MediaItem* item, std::string regionName, int setColor);

	/****************** marker *****************************/

	Marker AZ_GetMarkerObject(TcpClient& tcpClient, ReaProject* proj, int markerId);
	std::vector<Marker> AZ_GetMarkerListMediaItem(TcpClient& tcpClient, MediaItem* item);
	void AZ_SetMarkerPosition(TcpClient& tcpClient, ReaProject* proj, int markerId, double setStartPos);
	void AZ_SetMarkerColor(TcpClient& tcpClient, ReaProject* proj, int markerId, int setColor);
	void AZ_SetMarkerName(TcpClient& tcpClient, ReaProject* proj, int markerId, std::string setName);
	Marker AZ_AddMarker(TcpClient& tcpClient, ReaProject* proj, double setStartPos, std::string setName, int setColor);
	void AZ_DeleteAllMarker(TcpClient& tcpClient, ReaProject* proj);
	Marker AZ_AddMarkerMediaItem(TcpClient& tcpClient, MediaItem* item, std::string markerName, int setColor);

	/****************** Action *****************************/

	void AZ_ActionMediaItemSelectKeep(TcpClient& tcpClient, MediaItem* item, int cmdID);
	void AZ_ActionMediaIdSelectKeep(TcpClient& tcpClient, ReaProject* projID, int itemID, int cmdID);
	void AZ_ActionSelectedMediaIdSelectKeep(TcpClient& tcpClient, ReaProject* projID, int selectItemID, int cmdID);

	/****************** Render Take *****************************/

	void AZ_RenderMediaItemActiveTake(TcpClient& tcpClient, MediaItem* item);
	void AZ_RenderMediaIdActiveTake(TcpClient& tcpClient, ReaProject* projID, int itemID);
	void AZ_RenderSelectedMediaIdActiveTake(TcpClient& tcpClient, ReaProject* projID, int selectItemID);

	/****************** Envelope *****************************/

	void AZ_SetMediaItemEnvelopeAllOFF(TcpClient& tcpClient, MediaItem* item);
	void AZ_SetMediaIdEnvelopeAllOFF(TcpClient& tcpClient, ReaProject* projID, int itemID);
	void AZ_SetSelectedMediaIdEnvelopeAllOFF(TcpClient& tcpClient, ReaProject* projID, int selectItemID);

	/****************** path *****************************/

	std::string AZ_SetPathFolder(TcpClient& tcpClient, std::string folderPath, std::string folderName, int timeAddType);
	void AZ_SetCopyFile(TcpClient& tcpClient, std::string copyFilePath, std::string pasteFilePath);

	/****************** ファイル *****************************/

	void AZ_WriteFile(TcpClient& tcpClient, std::string directory, std::string filename, std::string text, int mode);
	std::string AZ_ReadFile(TcpClient& tcpClient, std::string directory, std::string filename);
	PathInfo AZ_GetPathInfo(TcpClient& tcpClient, std::string file_path);
	std::vector<std::string> AZ_GetFilePathList(TcpClient& tcpClient, std::string folder_path, std::string search_extension = "");

	// 接続先 PC のアプリ用データフォルダ (RIGDOCKS Bridge フォルダの親)
	//   Windows : %APPDATA% / macOS : ~/Library/Application Support
	std::string AZ_GetAppDataPath(TcpClient& tcpClient);

	// 接続先 PC の RIGDOCKS Bridge フォルダ
	std::string AZ_GetBridgeDataPath(TcpClient& tcpClient);

	/******************string*****************************/

	bool AZ_CheckIncludeString(TcpClient& tcpClient, std::string textString, std::string checkString);
	bool AZ_CheckSameString(TcpClient& tcpClient, std::string textString, std::string checkString);
	bool AZ_CheckIncludePerfectString(TcpClient& tcpClient, std::string textString, std::string checkString);
	std::string AZ_ReplaceSearchString(TcpClient& tcpClient, std::string textString, std::string searchString, std::string replaceString);
	std::string AZ_DeleteBlankString(TcpClient& tcpClient, std::string textString);
	std::string AZ_FormatNumDigit(TcpClient& tcpClient, int num, int digit);
	std::vector<std::string> AZ_GetStringSplitList(TcpClient& tcpClient, std::string baseStr, std::string splitStr);
	std::string AZ_JoinArray(TcpClient& tcpClient, std::vector<std::string> stringArray, std::string separator);
	std::string AZ_NormalizeText(TcpClient& tcpClient, std::string text);
	int AZ_CalcStringSimilarity(TcpClient& tcpClient, std::string targetText, std::string sourceText);
	int AZ_CalcStringPartialSimilarity(TcpClient& tcpClient, std::string targetText, std::string sourceText);

	/****************** ランダム *****************************/

	int AZ_GetRandomNumInteger(TcpClient& tcpClient, int low, int high);
	double AZ_GetRandomNumDouble(TcpClient& tcpClient, double low, double high, int doubleType);
	std::tuple<int, int, int, int> AZ_GetRandomColor(TcpClient& tcpClient);

	/****************** UI *****************************/

	std::tuple<bool, int> AZ_ShowMessageBox_Ex(TcpClient& tcpClient, std::string messageStr, std::string windowTitleStr, int windowType);
	std::tuple<bool, std::vector<std::string>> AZ_GetUserInputList(TcpClient& tcpClient, std::string titleStr, int inputNum, std::string captionStr, std::string resultStr);
	std::tuple<bool, int> AZ_ShowMessageBox_Info(TcpClient& tcpClient, std::string message, std::string title, int windowType);
	std::tuple<bool, int> AZ_ShowMessageBox_Warn(TcpClient& tcpClient, std::string message, std::string title, int windowType);
	std::tuple<bool, int> AZ_ShowMessageBox_Error(TcpClient& tcpClient, std::string message, std::string title, int windowType);

	/******************Http*****************************/

	std::tuple<bool, HttpResponse> AZ_Http_Get(TcpClient& tcpClient, std::string url, std::map<std::string, std::string> headers, std::map<std::string, std::string> queryParams);
	std::tuple<bool, HttpResponse> AZ_Http_Post(TcpClient& tcpClient, std::string url, std::map<std::string, std::string> headers, std::string body, std::map<std::string, std::string> queryParams);
	std::tuple<bool, HttpResponse> AZ_Http_Put(TcpClient& tcpClient, std::string url, std::map<std::string, std::string> headers, std::string body, std::map<std::string, std::string> queryParams);
	std::tuple<bool, HttpResponse> AZ_Http_Patch(TcpClient& tcpClient, std::string url, std::map<std::string, std::string> headers, std::string body, std::map<std::string, std::string> queryParams);
	std::tuple<bool, HttpResponse> AZ_Http_Delete(TcpClient& tcpClient, std::string url, std::map<std::string, std::string> headers, std::string body, std::map<std::string, std::string> queryParams);
	std::tuple<bool, HttpResponse> AZ_Http_Options(TcpClient& tcpClient, std::string url, std::map<std::string, std::string> headers);

	/****************** slack *****************************/

	SlackMessage AZ_Slack_PostChannelMessage(TcpClient& tcpClient, std::string token, std::string channel, std::string message, std::vector<std::string> mentionList);
	SlackMessage AZ_Slack_PostDirectMessage(TcpClient& tcpClient, std::string token, std::string toUser, std::string message, bool useMension);
	SlackMessage AZ_Slack_ReplayMessage(TcpClient& tcpClient, std::string token, std::string channel, std::string threadTimeStamp, std::string message, std::vector<std::string> mentionList);

	/******************HANDAUTOMER*****************************/

	void AZ_SetMediaItemHANDAUTOMER(TcpClient& tcpClient, MediaItem* item, int ratioType);
	void AZ_SetMediaIdHANDAUTOMER(TcpClient& tcpClient, ReaProject* projID, int itemID, int ratioType);
	void AZ_SetSelectedMediaIdHANDAUTOMER(TcpClient& tcpClient, ReaProject* projID, int selectItemID, int ratioType);
	void AZ_SetMediaItemListHANDAUTOMER(TcpClient& tcpClient, std::vector<MediaItem*> itemListJson, int ratioType);

	/******************TALKTRON*****************************/

	std::vector<std::string> AZ_TALKTRON_InsertMediaItems(TcpClient& tcpClient, std::string subjectLangPath, std::string baseLangPath, double startPos, double mediaInterval, int startEndType, bool isVolMatch, bool isHANDAUTOMER, int HANDAUTOMER_ratioType, std::vector<std::string> trackNameList);

	/******************文字起こし*****************************/

	TranscriptionContext* AZ_TRSC_LoadModel(TcpClient& tcpClient, std::string modelPath);
	std::string AZ_TRSC_FullForMediaItem(TcpClient& tcpClient, TranscriptionContext* ctx, MediaItem* item, std::string language);
	std::string AZ_TRSC_FullForMediaID(TcpClient& tcpClient, TranscriptionContext* ctx, ReaProject* proj, int mediaID, std::string language);
	std::string AZ_TRSC_FullForSelectedMediaID(TcpClient& tcpClient, TranscriptionContext* ctx, ReaProject* proj, int selectedMediaID, std::string language);
	std::string AZ_TRSC_FullForTrackMedia(TcpClient& tcpClient, TranscriptionContext* ctx, ReaProject* proj, int trackID, int trackMediaID, std::string language);
	std::string AZ_TRSC_FullForSelectedTrackMedia(TcpClient& tcpClient, TranscriptionContext* ctx, ReaProject* proj, int selectedTrackID, int trackMediaID, std::string language);
	std::vector<WhisperSegment> AZ_TRSC_SegmentsForMediaItem(TcpClient& tcpClient, TranscriptionContext* ctx, MediaItem* item, std::string language);
	std::vector<WhisperSegment> AZ_TRSC_SegmentsForMediaID(TcpClient& tcpClient, TranscriptionContext* ctx, ReaProject* proj, int mediaID, std::string language);
	std::vector<WhisperSegment> AZ_TRSC_SegmentsForSelectedMediaID(TcpClient& tcpClient, TranscriptionContext* ctx, ReaProject* proj, int selectedMediaID, std::string language);
	std::vector<WhisperSegment> AZ_TRSC_SegmentsForTrackMedia(TcpClient& tcpClient, TranscriptionContext* ctx, ReaProject* proj, int trackID, int trackMediaID, std::string language);
	std::vector<WhisperSegment> AZ_TRSC_SegmentsForSelectedTrackMedia(TcpClient& tcpClient, TranscriptionContext* ctx, ReaProject* proj, int selectedTrackID, int trackMediaID, std::string language);
	void AZ_TRSC_ReleaseModel(TcpClient& tcpClient, TranscriptionContext* ctx);

}
