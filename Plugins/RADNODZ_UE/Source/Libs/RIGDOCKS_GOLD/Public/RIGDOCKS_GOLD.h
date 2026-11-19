// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "TcpClient.h"
#include "Object/WwiseObject.h"
#include "Object/WwisePropertyList.h"
#include "Object/WwiseStateGroup.h"
#include "Object/ExcelSheetInfo.h"
#include "Object/GSheetInfo.h"
#include "Object/MiddlewareInfo.h"
#include "Object/RenderingOptions.h"
#include "Object/LoudnessRecord.h"
#include "Object/MIDDLESCANNER_Options.h"
#include "Object/TSPRecordingGroup.h"
#include <memory>
#include <string>
#include <tuple>
#include <vector>

class ReaProject;
class MediaItem;
class MediaItem_Take; 
class MediaTrack;
class XLDocument;
class XLWorksheet;
class GSheetConnection;

using namespace RADNODZ_client;

namespace RADNODZ_client::GOLD
{
	using namespace Object;

	/******************Wwise*****************************/

	bool AZ_Wwise_Connect(TcpClient& tcpClient, std::string address = "127.0.0.1", int port = 8080, int timeoutMs = 5000);
	void AZ_Wwise_Disconnect(TcpClient& tcpClient);
	std::string AZ_Wwise_GetProjectName(TcpClient& tcpClient);
	void AZ_Wwise_MoveObject(TcpClient& tcpClient, std::string itemID, std::string destinationID);
	void AZ_Wwise_SetName(TcpClient& tcpClient, std::string itemID, std::string name);
	std::tuple<std::vector<WwiseObject>, int> AZ_Wwise_GetSelectedObjectList(TcpClient& tcpClient);
	std::vector<WwiseObject> AZ_Wwise_GetAllActorAncestorList(TcpClient& tcpClient, std::string itemID, std::vector<std::string> excludeList = {});
	std::tuple<std::vector<WwiseObject>, int> AZ_Wwise_GetAllBusAncestorList(TcpClient& tcpClient, std::string itemID, std::vector<std::string> excludeList = {});
	std::tuple<std::vector<WwiseObject>, int> AZ_Wwise_GetUserAuxSend(TcpClient& tcpClient, std::string itemID);
	std::tuple<WwiseObject, std::vector<WwiseObject>, int> AZ_Wwise_GetSwitchSet(TcpClient& tcpClient, std::string switchGroupName);
	WwisePropertyList AZ_Wwise_GetPropertyList(TcpClient& tcpClient, std::string itemID, std::string platform = "");
	WwiseObject AZ_Wwise_CreateContainer(TcpClient& tcpClient, std::string parentItemID, int type, std::string name, bool isUnique);
	VarType AZ_Wwise_GetProperty(TcpClient& tcpClient, std::string itemID, std::string propertyName, std::string language = "", std::string platform = "");
	void AZ_Wwise_SetProperty(TcpClient& tcpClient, std::string itemID, std::string propertyName, VarType value, std::string platform = "");
	std::vector<WwiseObject> AZ_Wwise_SearchSoundItemFromName(TcpClient& tcpClient, std::string parentID, std::string name, bool isPartialMatch);
	std::vector<WwiseObject> AZ_Wwise_GetChildrenList(TcpClient& tcpClient, std::string itemID, bool isRecursive);
	double AZ_Wwise_GetTotalVolume(TcpClient& tcpClient, std::string itemID, std::string language, MIDDLESCANNER_Options options);
	WwiseObject AZ_Wwise_CreateSoundSFX(TcpClient& tcpClient, std::string parentItemID, std::string sourcePath = "", std::string itemName = "", std::string subFolder = "", bool isUnique = false);
	WwiseObject AZ_Wwise_CreateSoundVoice(TcpClient& tcpClient, std::string parentItemID, std::string sourcePath = "", std::string itemName = "", std::string language = "", std::string subFolder = "", bool isUnique = false);
	void AZ_Wwise_ImportAudioFile(TcpClient& tcpClient, std::string itemID, std::string sourcePath, std::string language = "", std::string subFolder = "");
	std::tuple<WwiseObject, std::vector<WwiseObject>> AZ_Wwise_GetStateSet(TcpClient& tcpClient, std::string stateGroupName, bool igroreNone = true);
	std::vector<WwiseStateGroup> AZ_Wwise_GetStateModificationFromFile(TcpClient& tcpClient, std::string itemID, std::string workUnitPath, std::vector<std::string> targetStateGroup = {}, std::vector<std::string> targetPropertyList = {});
	std::vector<WwiseStateGroup> AZ_Wwise_GetStateModification(TcpClient& tcpClient, std::string itemID, std::vector<std::string> targetStateGroup = {}, std::vector<std::string> targetPropertyList = {});
	void AZ_Wwise_AssignToSwitch(TcpClient& tcpClient, std::string assignedItemID, std::string switchOrStateID, bool idDefault = false);
	WwiseObject AZ_Wwise_CreateWorkUnit(TcpClient& tcpClient, std::string parentItemID = "", std::string name = "", bool isUnique = false);
	WwiseObject AZ_Wwise_CreateVirtualFolder(TcpClient& tcpClient, std::string parentItemID = "", std::string name = "", bool isUnique = false);
	WwiseObject AZ_Wwise_CreateActorMixer(TcpClient& tcpClient, std::string parentItemID = "", std::string name = "", bool isUnique = false);
	WwiseObject AZ_Wwise_CreateEvent(TcpClient& tcpClient, std::string parentItemID = "", std::string name = "", int actionType = 0, std::string targetItemID = "", bool isUnique = false);
	void AZ_Wwise_AddActionToEvent(TcpClient& tcpClient, std::string eventItemID, int actionType = 1, std::string targetItemID = "");
	WwiseObject AZ_Wwise_CreateObject(TcpClient& tcpClient, std::string parentItemID, std::string name, std::string objectType, bool isUnique = false);
	void AZ_Wwise_DeleteObject(TcpClient& tcpClient, std::string itemID);
	void AZ_Wwise_SetReference(TcpClient& tcpClient, std::string itemID, std::string propertyName, std::string refItemID, std::string platform = "");
	bool AZ_Wwise_ExistsObject(TcpClient& tcpClient, std::string itemPath);
	void AZ_Wwise_AddObjectToSoundBank(TcpClient& tcpClient, std::string soundbankID, std::string objectID, std::vector<int> filterList = {0, 1, 2});
	void AZ_Wwise_RemoveObjectFromSoundBank(TcpClient& tcpClient, std::string soundbankID, std::string objectID);
	std::vector<std::string> AZ_Wwise_GenerateSoundBank(TcpClient& tcpClient, std::vector<std::string> soundbankList, std::vector<std::string> platformList, std::vector<std::string> languageList);
	void AZ_Wwise_SaveProject(TcpClient& tcpClient);
	WwiseObject AZ_Wwise_CopyObject(TcpClient& tcpClient, std::string targetObjectID, std::string destObjectID, std::string destName = "", bool isUnique = false);
	void AZ_Wwise_SetRandomizer(TcpClient& tcpClient, std::string itemID, std::string propertyName, bool enabled = true, double* min = nullptr, double* max = nullptr, std::string platform = "");
	bool AZ_Wwise_HasChild(TcpClient& tcpClient, std::string parentID, std::string itemIDorName, bool isRecursive = false);

	/******************LOUDMATCHIFY*****************************/

	void  AZ_LOUDMATCHIFY_SetVolumeForMediaItem(TcpClient& tcpClient, MediaItem* item, std::vector<LoudnessRecord> loudnessRecord, int volumeType, double additionalVolume);
	void  AZ_LOUDMATCHIFY_SetVolumeForMediaId(TcpClient& tcpClient, ReaProject* proj, int itemID, std::vector<LoudnessRecord> loudnessRecord, int volumeType, double additionalVolume);
	void  AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId(TcpClient& tcpClient, ReaProject* proj, int selectedItemID, std::vector<LoudnessRecord> loudnessRecord, int volumeType, double additionalVolume);
	void  AZ_LOUDMATCHIFY_SetVolumeForMediaItemList(TcpClient& tcpClient, std::vector<MediaItem*> itemList, std::vector<LoudnessRecord> loudnessRecord, int volumeType, std::vector<MiddlewareInfo> additionalVolumeList);

	/******************MIDDLESCANNER*****************************/

	MiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem(TcpClient& tcpClient, MediaItem* item, std::string language, std::string parentID, MIDDLESCANNER_Options options);
	MiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId(TcpClient& tcpClient, ReaProject* proj, int itemID, std::string language, std::string parentID, MIDDLESCANNER_Options options);
	MiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId(TcpClient& tcpClient, ReaProject* proj, int selectedItemID, std::string language, std::string parentID, MIDDLESCANNER_Options options);
	std::vector<MiddlewareInfo> AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList(TcpClient& tcpClient, std::vector<MediaItem*> itemList, std::string language, std::string parentID, MIDDLESCANNER_Options options);

	/******************Excel*****************************/

	XLDocument* AZ_Excel_OpenFile(TcpClient& tcpClient, std::string filePath);
	void AZ_Excel_CloseFile(TcpClient& tcpClient, XLDocument* excelDocumment = nullptr);
	XLWorksheet* AZ_Excel_OpenWorksheet(TcpClient& tcpClient, XLDocument* excelDocumment, std::string sheetName);
	XLWorksheet* AZ_Excel_OpenWorksheet_ByNumber(TcpClient& tcpClient, XLDocument* excelDocumment, int sheetNumber);
	VarType AZ_Excel_GetCellValue(TcpClient& tcpClient, XLWorksheet* excelSheet, std::string cell);
	VarType AZ_Excel_GetCellValue_ByNumber(TcpClient& tcpClient, XLWorksheet* excelSheet, int rowNumber, int columeNumber);
	void AZ_Excel_SetCellValue(TcpClient& tcpClient, XLWorksheet* excelSheet, std::string cell, VarType value);
	void AZ_Excel_SetCellValue_ByNumber(TcpClient& tcpClient, XLWorksheet* excelSheet, int rowNumber, int columeNumber, VarType value);
	void AZ_Excel_SaveDocument(TcpClient& tcpClient, XLDocument* excelDocumment, std::string newPath = "");
	XLDocument* AZ_Excel_CreateDocument(TcpClient& tcpClient, std::string newPath, bool isOverwrite = true);
	XLWorksheet* AZ_Excel_AddWorksheet(TcpClient& tcpClient, XLDocument* excelDocumment, std::string worksheetName = "");
	void AZ_Excel_RenameWorksheet(TcpClient& tcpClient, XLWorksheet* excelWorksheet, std::string worksheetName);
	void AZ_Excel_DeleteWorksheet(TcpClient& tcpClient, XLWorksheet* excelWorksheet);
	std::vector<ExcelSheetInfo> AZ_Excel_GetWorksheetList(TcpClient& tcpClient, XLDocument* document, std::string searchStr = "", bool isRegex = false, std::vector<std::string> excludeList = {});
	ExcelSheetInfo AZ_Excel_SearchCellsInWorksheet(TcpClient& tcpClient, XLWorksheet* excelSheet, std::string searchStr, std::string range = "", bool isRegex = false);
	ExcelSheetInfo AZ_Excel_SearchRowsInWorksheet(TcpClient& tcpClient, XLWorksheet* excelSheet, std::string searchStr, std::string targetCulume, std::string range = "", bool isRegex = false);
	std::vector<ExcelSheetInfo> AZ_Excel_SearchCellsInAllWorksheets(TcpClient& tcpClient, XLDocument* document, std::string searchStr, std::string range = "", bool isRegex = false);
	std::vector<ExcelSheetInfo> AZ_Excel_SearchRowsInAllWorksheets(TcpClient& tcpClient, XLDocument* document, std::string searchStr, std::string targetCulume, std::string range = "", bool isRegex = false);

	/******************Render*****************************/

	std::tuple<std::vector<std::string>, std::vector<std::string>> AZ_RenderToAudioFile(TcpClient& tcpClient, ReaProject* project, RenderingOptions option);
	RenderingOptions AZ_GetRenderSetting(TcpClient& tcpClient, ReaProject* project);
	RenderingOptions AZ_GetRenderSettingFromProject(TcpClient& tcpClient, std::string projectPath);
	void AZ_SetRenderSetting(TcpClient& tcpClient, ReaProject* project, RenderingOptions options);

	/****************** P4 *****************************/

	bool  AZ_P4_Connect(TcpClient& tcpClient, std::string uri, std::string userName, std::string password);
	void  AZ_P4_Disconnect(TcpClient& tcpClient);
	std::vector<std::string> AZ_P4_SyncFiles(TcpClient& tcpClient, std::string workspace, std::string depotPath, int changeListID = -1, std::string extension = "");
	std::vector<std::string> AZ_P4_ExportToDirectory(TcpClient& tcpClient, std::string workspace, std::string toDir, std::string depotPath, int fileWriteMode = 0, int changeListID = -1, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	std::vector<std::string> AZ_P4_ExportAudioToProject(TcpClient& tcpClient, std::string workspace, std::string depotPath, std::string toDir = "", int changeListID = -1, std::string extension = "wav", bool toProjectDir = true, std::vector<std::string> exclusionList = {});

	/****************** SVN *****************************/

	void  AZ_SVN_SetAuth(TcpClient& tcpClient, std::string userName, std::string password);
	void  AZ_SVN_CheckoutOrUpdate(TcpClient& tcpClient, std::string repositoryUrl, std::string workingCopyPath, int revision = -1);
	std::vector<std::string>  AZ_SVN_Add(TcpClient& tcpClient, std::string localPath, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	int  AZ_SVN_Commit(TcpClient& tcpClient, std::string localPath, std::string message, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	void AZ_SVN_Revert(TcpClient& tcpClient, std::string localPath, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	std::vector<std::string> AZ_SVN_Export(TcpClient& tcpClient, std::string repositoryUrl, std::string toDir, int mode = 0, int revision = -1, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	std::vector<std::string> AZ_SVN_GetFileList(TcpClient& tcpClient, std::string repositoryUrl, int revision = -1, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	std::vector<std::string> AZ_SVN_GetAddAndUpdateList(TcpClient& tcpClient, std::string repositoryUrl, int revision = -1, std::vector<std::string> exclusionList = {},  bool isRegex = true);
	std::string AZ_SVN_GetRootUrl(TcpClient& tcpClient, std::string repositoryUrl);
	std::vector<std::string> AZ_SVN_Export_CommitOnly(TcpClient& tcpClient, std::string repositoryUrl, std::string toDir, int mode = 0, int revision = -1, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	std::vector<std::string> AZ_SVN_ExportAudioToProject(TcpClient& tcpClient, std::string repositoryUrl, int revision, std::string toDir = "", std::string extension = "wav", bool toProjectDir = true);
	std::vector<std::string> AZ_SVN_ExportAudioToProject_CommitOnly(TcpClient& tcpClient, std::string repositoryUrl, int revision, std::string toDir = "", std::string extension = "wav", bool toProjectDir = true);

	/****************** IR *****************************/

	// returns (signal duration [sec], TSP signal wav path, TSP info json path)
	std::tuple<double, std::string, std::string> AZ_GenerateLinearTSP(TcpClient& tcpClient, std::string outputPath, int sampleRate = 48000, double sweepDuration = 5.0, double silenceDuration = 5.0, int repeatCount = 1);
	std::tuple<double, std::string, std::string> AZ_GenerateLogTSP(TcpClient& tcpClient, std::string outputPath, int sampleRate = 48000, double sweepDuration = 10.0, double silenceDuration = 5.0, int repeatCount = 1, double startFreq = 20.0, double endFreq = 0.0);
	void AZ_CalcImpulseResponseFromLinearTSP(TcpClient& tcpClient, std::string tspInfoPath, std::vector<TSPRecordingGroup> recordingGroups, std::string outputPath, double irLength = 0.0, bool trimStart = true, bool normalize = true);
	void AZ_CalcImpulseResponseFromLogTSP(TcpClient& tcpClient, std::string tspInfoPath, std::vector<TSPRecordingGroup> recordingGroups, std::string outputPath, double irLength = 0.0, bool trimStart = true, bool normalize = true);

	/****************** Googl SpreadSheet *****************************/

	GSheetConnection* AZ_GSheet_Connect_UserAccount(TcpClient& tcpClient, std::string spreadSheetId, std::string clientId = "");
	void AZ_GSheet_Disconnect(TcpClient& tcpClient, GSheetConnection* connection);
	GSheetInfo AZ_GSheet_AddWorksheet(TcpClient& tcpClient, GSheetConnection* connection, std::string sheetName = "");
	void AZ_GSheet_DeleteWorksheet(TcpClient& tcpClient, GSheetConnection* connection, int sheetId);
	void AZ_GSheet_RenameWorksheet(TcpClient& tcpClient, GSheetConnection* connection, int sheetId, std::string newSheetName);
	GSheetInfo AZ_GSheet_CopyWorksheet(TcpClient& tcpClient, GSheetConnection* connection, int sheetId, std::string newSheetName = "", bool isNextSheet = true);
	std::vector<GSheetInfo> AZ_GSheet_GetWorksheetList(TcpClient& tcpClient, GSheetConnection* connection, std::string searchStr = "", bool isRegex = false, std::vector<std::string> excludeList = std::vector<std::string>());
	VarType AZ_GSheet_GetCellValue(TcpClient& tcpClient, GSheetConnection* connection, int sheetId, std::string cell);
	VarType AZ_GSheet_GetCellValue_ByNumber(TcpClient& tcpClient, GSheetConnection* connection, int sheetId, int rowNumber, int columnNumber);
	void AZ_GSheet_SetCellValue(TcpClient& tcpClient, GSheetConnection* connection, int sheetId, std::string cell, VarType value);
	void AZ_GSheet_SetCellValue_ByNumber(TcpClient& tcpClient, GSheetConnection* connection, int sheetId, int rowNumber, int columnNumber, VarType value);
	GSheetInfo AZ_GSheet_SearchCellsInWorksheet(TcpClient& tcpClient, GSheetConnection* connection, int sheetId, std::string searchStr, std::string range = "", bool isRegex = false);
	GSheetInfo AZ_GSheet_SearchRowsInWorksheet(TcpClient& tcpClient, GSheetConnection* connection, int sheetId, std::string searchStr, std::string targetColumn, std::string range = "", bool isRegex = false);
	std::vector<GSheetInfo> AZ_GSheet_SearchCellsInAllWorksheets(TcpClient& tcpClient, GSheetConnection* connection, std::string searchStr, std::string range = "", bool isRegex = false);
	std::vector<GSheetInfo> AZ_GSheet_SearchRowsInAllWorksheets(TcpClient& tcpClient, GSheetConnection* connection, std::string searchStr, std::string targetColumn, std::string range = "", bool isRegex = false);

}
