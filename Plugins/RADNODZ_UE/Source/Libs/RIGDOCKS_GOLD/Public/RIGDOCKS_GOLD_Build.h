// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "RIGDOCKS_GOLD.h"
#include "RpcCall.h"

namespace RADNODZ_client::GOLD
{
	using namespace Object;

	/******************Wwise*****************************/
	RpcCall AZ_Wwise_Connect_Build(std::string address = "127.0.0.1", int port = 8080, int timeoutMs = 5000);
	RpcCall AZ_Wwise_Disconnect_Build();
	RpcCall AZ_Wwise_GetProjectName_Build();
	RpcCall AZ_Wwise_MoveObject_Build(std::string itemID, std::string destinationID);
	RpcCall AZ_Wwise_SetName_Build(std::string itemID, std::string name);
	RpcCall AZ_Wwise_GetSelectedObjectList_Build();
	RpcCall AZ_Wwise_GetAllActorAncestorList_Build(std::string itemID, std::vector<std::string> excludeList = {});
	RpcCall AZ_Wwise_GetAllBusAncestorList_Build(std::string itemID, std::vector<std::string> excludeList = {});
	RpcCall AZ_Wwise_GetUserAuxSend_Build(std::string itemID);
	RpcCall AZ_Wwise_GetSwitchSet_Build(std::string switchGroupName);
	RpcCall AZ_Wwise_GetPropertyList_Build(std::string itemID, std::string platform = "");
	RpcCall AZ_Wwise_CreateContainer_Build(std::string parentItemID, int type, std::string name, bool isUnique);
	RpcCall AZ_Wwise_GetProperty_Build(std::string itemID, std::string propertyName, std::string language = "", std::string platform = "");
	RpcCall AZ_Wwise_SetProperty_Build(std::string itemID, std::string propertyName, VarType value, std::string platform = "");
	RpcCall AZ_Wwise_SearchSoundItemFromName_Build(std::string parentID, std::string name, bool isPartialMatch);
	RpcCall AZ_Wwise_GetChildrenList_Build(std::string itemID, bool isRecursive);
	RpcCall AZ_Wwise_GetTotalVolume_Build(std::string itemID, std::string language, MIDDLESCANNER_Options options);
	RpcCall AZ_Wwise_CreateSoundSFX_Build(std::string parentItemID, std::string sourcePath = "", std::string itemName = "", std::string subFolder = "", bool isUnique = false);
	RpcCall AZ_Wwise_CreateSoundVoice_Build(std::string parentItemID, std::string sourcePath = "", std::string itemName = "", std::string language = "", std::string subFolder = "", bool isUnique = false);
	RpcCall AZ_Wwise_ImportAudioFile_Build(std::string itemID, std::string sourcePath, std::string language = "", std::string subFolder = "");
	RpcCall AZ_Wwise_GetStateSet_Build(std::string stateGroupName, bool igroreNone = true);
	RpcCall AZ_Wwise_GetStateModificationFromFile_Build(std::string itemID, std::string workUnitPath, std::vector<std::string> targetStateGroup = {}, std::vector<std::string> targetPropertyList = {});
	RpcCall AZ_Wwise_GetStateModification_Build(std::string itemID, std::vector<std::string> targetStateGroup = {}, std::vector<std::string> targetPropertyList = {});
	RpcCall AZ_Wwise_AssignToSwitch_Build(std::string assignedItemID, std::string switchOrStateID, bool idDefault = false);
	RpcCall AZ_Wwise_CreateWorkUnit_Build(std::string parentItemID = "", std::string name = "", bool isUnique = false);
	RpcCall AZ_Wwise_CreateVirtualFolder_Build(std::string parentItemID = "", std::string name = "", bool isUnique = false);
	RpcCall AZ_Wwise_CreateActorMixer_Build(std::string parentItemID = "", std::string name = "", bool isUnique = false);
	RpcCall AZ_Wwise_CreateEvent_Build(std::string parentItemID = "", std::string name = "", int actionType = 0, std::string targetItemID = "", bool isUnique = false);
	RpcCall AZ_Wwise_AddActionToEvent_Build(std::string eventItemID, int actionType = 1, std::string targetItemID = "");
	RpcCall AZ_Wwise_CreateObject_Build(std::string parentItemID, std::string name, std::string objectType, bool isUnique = false);
	RpcCall AZ_Wwise_DeleteObject_Build(std::string itemID);
	RpcCall AZ_Wwise_SetReference_Build(std::string itemID, std::string propertyName, std::string refItemID, std::string platform = "");
	RpcCall AZ_Wwise_ExistsObject_Build(std::string itemPath);
	RpcCall AZ_Wwise_AddObjectToSoundBank_Build(std::string soundbankID, std::string objectID, std::vector<int> filterList = {0, 1, 2});
	RpcCall AZ_Wwise_RemoveObjectFromSoundBank_Build(std::string soundbankID, std::string objectID);
	RpcCall AZ_Wwise_GenerateSoundBank_Build(std::vector<std::string> soundbankList, std::vector<std::string> platformList, std::vector<std::string> languageList);
	RpcCall AZ_Wwise_SaveProject_Build();
	RpcCall AZ_Wwise_CopyObject_Build(std::string targetObjectID, std::string destObjectID, std::string destName = "", bool isUnique = false);
	RpcCall AZ_Wwise_SetRandomizer_Build(std::string itemID, std::string propertyName, bool enabled = true, double* min = nullptr, double* max = nullptr, std::string platform = "");
	RpcCall AZ_Wwise_HasChild_Build(std::string parentID, std::string itemIDorName, bool isRecursive = false);

	/******************LOUDMATCHIFY*****************************/
	RpcCall AZ_LOUDMATCHIFY_SetVolumeForMediaItem_Build(MediaItem* item, std::vector<LoudnessRecord> loudnessRecord, int volumeType, double additionalVolume);
	RpcCall AZ_LOUDMATCHIFY_SetVolumeForMediaId_Build(ReaProject* proj, int itemID, std::vector<LoudnessRecord> loudnessRecord, int volumeType, double additionalVolume);
	RpcCall AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId_Build(ReaProject* proj, int selectedItemID, std::vector<LoudnessRecord> loudnessRecord, int volumeType, double additionalVolume);
	RpcCall AZ_LOUDMATCHIFY_SetVolumeForMediaItemList_Build(std::vector<MediaItem*> itemList, std::vector<LoudnessRecord> loudnessRecord, int volumeType, std::vector<MiddlewareInfo> additionalVolumeList);

	/******************MIDDLESCANNER*****************************/
	RpcCall AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem_Build(MediaItem* item, std::string language, std::string parentID, MIDDLESCANNER_Options options);
	RpcCall AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId_Build(ReaProject* proj, int itemID, std::string language, std::string parentID, MIDDLESCANNER_Options options);
	RpcCall AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId_Build(ReaProject* proj, int selectedItemID, std::string language, std::string parentID, MIDDLESCANNER_Options options);
	RpcCall AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList_Build(std::vector<MediaItem*> itemList, std::string language, std::string parentID, MIDDLESCANNER_Options options);

	/******************Excel*****************************/
	RpcCall AZ_Excel_OpenFile_Build(std::string filePath);
	RpcCall AZ_Excel_GetWorksheetList_Build(XLDocument* document, std::string searchStr = "", bool isRegex = false, std::vector<std::string> excludeList = {});
	RpcCall AZ_RenderToAudioFile_Build(ReaProject* project, RenderingOptions option);
	RpcCall AZ_Excel_CloseFile_Build(XLDocument* excelDocumment = nullptr);
	RpcCall AZ_Excel_OpenWorksheet_Build(XLDocument* excelDocumment, std::string sheetName);
	RpcCall AZ_Excel_GetCellValue_Build(XLWorksheet* excelSheet, std::string cell);
	RpcCall AZ_Excel_OpenWorksheet_ByNumber_Build(XLDocument* excelDocumment, int sheetNumber);
	RpcCall AZ_Excel_GetCellValue_ByNumber_Build(XLWorksheet* excelSheet, int rowNumber, int columeNumber);
	RpcCall AZ_Excel_SetCellValue_Build(XLWorksheet* excelSheet, std::string cell, VarType value);
	RpcCall AZ_Excel_SetCellValue_ByNumber_Build(XLWorksheet* excelSheet, int rowNumber, int columeNumber, VarType value);
	RpcCall AZ_Excel_SaveDocument_Build(XLDocument* excelDocumment, std::string newPath = "");
	RpcCall AZ_Excel_CreateDocument_Build(std::string newPath, bool isOverwrite = true);
	RpcCall AZ_Excel_AddWorksheet_Build(XLDocument* excelDocumment, std::string worksheetName = "");
	RpcCall AZ_Excel_RenameWorksheet_Build(XLWorksheet* excelWorksheet, std::string worksheetName);
	RpcCall AZ_Excel_DeleteWorksheet_Build(XLWorksheet* excelWorksheet);
	RpcCall AZ_Excel_SearchCellsInWorksheet_Build(XLWorksheet* excelSheet, std::string searchStr, std::string range = "", bool isRegex = false);
	RpcCall AZ_Excel_SearchRowsInWorksheet_Build(XLWorksheet* excelSheet, std::string searchStr, std::string targetCulume, std::string range = "", bool isRegex = false);
	RpcCall AZ_Excel_SearchCellsInAllWorksheets_Build(XLDocument* document, std::string searchStr, std::string range = "", bool isRegex = false);
	RpcCall AZ_Excel_SearchRowsInAllWorksheets_Build(XLDocument* document, std::string searchStr, std::string targetCulume, std::string range = "", bool isRegex = false);

	/******************Render*****************************/
	RpcCall AZ_GetRenderSetting_Build(ReaProject* project);
	RpcCall AZ_GetRenderSettingFromProject_Build(std::string projectPath);
	RpcCall AZ_SetRenderSetting_Build(ReaProject* project, RenderingOptions option);

	/****************** P4 *****************************/
	RpcCall AZ_P4_Connect_Build(std::string uri, std::string userName, std::string password);
	RpcCall AZ_P4_Disconnect_Build();
	RpcCall AZ_P4_SyncFiles_Build(std::string workspace, std::string depotPath, int revision = -1, std::string extension = "");
	RpcCall AZ_P4_ExportToDirectory_Build(std::string workspace, std::string toDir, std::string depotPath, int fileWriteMode = 0, int revision = -1, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	RpcCall AZ_P4_ExportAudioToProject_Build(std::string workspace, std::string depotPath, std::string toDir = "", int revision = -1, std::string extension = "wav", bool toProjectDir = true, std::vector<std::string> exclusionList = {});

	/****************** SVN *****************************/
	RpcCall AZ_SVN_SetAuth_Build(std::string userName, std::string password);
	RpcCall AZ_SVN_CheckoutOrUpdate_Build(std::string repositoryUrl, std::string workingCopyPath, int revision = -1);
	RpcCall AZ_SVN_Add_Build(std::string localPath, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	RpcCall AZ_SVN_Commit_Build(std::string localPath, std::string message, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	RpcCall AZ_SVN_Revert_Build(std::string localPath, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	RpcCall AZ_SVN_Export_Build(std::string repositoryUrl, std::string toDir, int mode = 0, int revision = -1, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	RpcCall AZ_SVN_GetFileList_Build(std::string repositoryUrl, int revision = -1, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	RpcCall AZ_SVN_GetAddAndUpdateList_Build(std::string repositoryUrl, int revision = -1, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	RpcCall AZ_SVN_GetRootUrl_Build(std::string repositoryUrl);
	RpcCall AZ_SVN_Export_CommitOnly_Build(std::string repositoryUrl, std::string toDir, int mode = 0, int revision = -1, std::vector<std::string> exclusionList = {}, bool isRegex = true);
	RpcCall AZ_SVN_ExportAudioToProject_Build(std::string repositoryUrl, int revision, std::string toDir = "", std::string extension = "wav", bool toProjectDir = true);
	RpcCall AZ_SVN_ExportAudioToProject_CommitOnly_Build(std::string repositoryUrl, int revision, std::string toDir = "", std::string extension = "wav", bool toProjectDir = true);

	/****************** IR *****************************/
	RpcCall AZ_GenerateLinearTSP_Build(std::string outputPath, int sampleRate = 48000, double sweepDuration = 5.0, double silenceDuration = 5.0, int repeatCount = 1);
	RpcCall AZ_GenerateLogTSP_Build(std::string outputPath, int sampleRate = 48000, double sweepDuration = 10.0, double silenceDuration = 5.0, int repeatCount = 1, double startFreq = 20.0, double endFreq = 0.0);
	RpcCall AZ_CalcImpulseResponseFromLinearTSP_Build(std::string tspInfoPath, std::vector<TSPRecordingGroup> recordingGroups, std::string outputPath, double irLength = 0.0, bool trimStart = true, bool normalize = true);
	RpcCall AZ_CalcImpulseResponseFromLogTSP_Build(std::string tspInfoPath, std::vector<TSPRecordingGroup> recordingGroups, std::string outputPath, double irLength = 0.0, bool trimStart = true, bool normalize = true);

	/****************** GSheet *****************************/
	RpcCall AZ_GSheet_Connect_UserAccount_Build(std::string spreadSheetId, std::string clientId = "");
	RpcCall AZ_GSheet_Disconnect_Build(GSheetConnection* connection);
	RpcCall AZ_GSheet_AddWorksheet_Build(GSheetConnection* connection, std::string sheetName = "");
	RpcCall AZ_GSheet_DeleteWorksheet_Build(GSheetConnection* connection, int sheetId);
	RpcCall AZ_GSheet_RenameWorksheet_Build(GSheetConnection* connection, int sheetId, std::string newSheetName);
	RpcCall AZ_GSheet_CopyWorksheet_Build(GSheetConnection* connection, int sheetId, std::string newSheetName = "", bool isNextSheet = true);
	RpcCall AZ_GSheet_GetWorksheetList_Build(GSheetConnection* connection, std::string searchStr = "", bool isRegex = false, std::vector<std::string> excludeList = std::vector<std::string>());
	RpcCall AZ_GSheet_GetCellValue_Build(GSheetConnection* connection, int sheetId, std::string cell);
	RpcCall AZ_GSheet_GetCellValue_ByNumber_Build(GSheetConnection* connection, int sheetId, int rowNumber, int columnNumber);
	RpcCall AZ_GSheet_SetCellValue_Build(GSheetConnection* connection, int sheetId, std::string cell, VarType value);
	RpcCall AZ_GSheet_SetCellValue_ByNumber_Build(GSheetConnection* connection, int sheetId, int rowNumber, int columnNumber, VarType value);
	RpcCall AZ_GSheet_SearchCellsInWorksheet_Build(GSheetConnection* connection, int sheetId, std::string searchStr, std::string range = "", bool isRegex = false);
	RpcCall AZ_GSheet_SearchRowsInWorksheet_Build(GSheetConnection* connection, int sheetId, std::string searchStr, std::string targetColumn, std::string range = "", bool isRegex = false);
	RpcCall AZ_GSheet_SearchCellsInAllWorksheets_Build(GSheetConnection* connection, std::string searchStr, std::string range = "", bool isRegex = false);
	RpcCall AZ_GSheet_SearchRowsInAllWorksheets_Build(GSheetConnection* connection, std::string searchStr, std::string targetColumn, std::string range = "", bool isRegex = false);

}
