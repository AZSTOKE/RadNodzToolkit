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
	bool AZ_Wwise_Connect_Parse(const RpcCall& call);
	void AZ_Wwise_Disconnect_Parse(const RpcCall& call);
	std::string AZ_Wwise_GetProjectName_Parse(const RpcCall& call);
	void AZ_Wwise_MoveObject_Parse(const RpcCall& call);
	void AZ_Wwise_SetName_Parse(const RpcCall& call);
	std::tuple<std::vector<WwiseObject>, int> AZ_Wwise_GetSelectedObjectList_Parse(const RpcCall& call);
	std::vector<WwiseObject> AZ_Wwise_GetAllActorAncestorList_Parse(const RpcCall& call);
	std::tuple<std::vector<WwiseObject>, int> AZ_Wwise_GetAllBusAncestorList_Parse(const RpcCall& call);
	std::tuple<std::vector<WwiseObject>, int> AZ_Wwise_GetUserAuxSend_Parse(const RpcCall& call);
	std::tuple<WwiseObject, std::vector<WwiseObject>, int> AZ_Wwise_GetSwitchSet_Parse(const RpcCall& call);
	WwisePropertyList AZ_Wwise_GetPropertyList_Parse(const RpcCall& call);
	WwiseObject AZ_Wwise_CreateContainer_Parse(const RpcCall& call);
	VarType AZ_Wwise_GetProperty_Parse(const RpcCall& call);
	void AZ_Wwise_SetProperty_Parse(const RpcCall& call);
	std::vector<WwiseObject> AZ_Wwise_SearchSoundItemFromName_Parse(const RpcCall& call);
	std::vector<WwiseObject> AZ_Wwise_GetChildrenList_Parse(const RpcCall& call);
	double AZ_Wwise_GetTotalVolume_Parse(const RpcCall& call);
	WwiseObject AZ_Wwise_CreateSoundSFX_Parse(const RpcCall& call);
	WwiseObject AZ_Wwise_CreateSoundVoice_Parse(const RpcCall& call);
	void AZ_Wwise_ImportAudioFile_Parse(const RpcCall& call);
	std::tuple<WwiseObject, std::vector<WwiseObject>> AZ_Wwise_GetStateSet_Parse(const RpcCall& call);
	std::vector<WwiseStateGroup> AZ_Wwise_GetStateModificationFromFile_Parse(const RpcCall& call);
	std::vector<WwiseStateGroup> AZ_Wwise_GetStateModification_Parse(const RpcCall& call);
	void AZ_Wwise_AssignToSwitch_Parse(const RpcCall& call);
	WwiseObject AZ_Wwise_CreateWorkUnit_Parse(const RpcCall& call);
	WwiseObject AZ_Wwise_CreateVirtualFolder_Parse(const RpcCall& call);
	WwiseObject AZ_Wwise_CreateActorMixer_Parse(const RpcCall& call);
	WwiseObject AZ_Wwise_CreateEvent_Parse(const RpcCall& call);
	void AZ_Wwise_AddActionToEvent_Parse(const RpcCall& call);
	WwiseObject AZ_Wwise_CreateObject_Parse(const RpcCall& call);
	void AZ_Wwise_DeleteObject_Parse(const RpcCall& call);
	void AZ_Wwise_SetReference_Parse(const RpcCall& call);
	bool AZ_Wwise_ExistsObject_Parse(const RpcCall& call);
	void AZ_Wwise_AddObjectToSoundBank_Parse(const RpcCall& call);
	void AZ_Wwise_RemoveObjectFromSoundBank_Parse(const RpcCall& call);
	std::vector<std::string> AZ_Wwise_GenerateSoundBank_Parse(const RpcCall& call);
	void AZ_Wwise_SaveProject_Parse(const RpcCall& call);
	WwiseObject AZ_Wwise_CopyObject_Parse(const RpcCall& call);
	void AZ_Wwise_SetRandomizer_Parse(const RpcCall& call);
	bool AZ_Wwise_HasChild_Parse(const RpcCall& call);

	/******************LOUDMATCHIFY*****************************/
	void AZ_LOUDMATCHIFY_SetVolumeForMediaItem_Parse(const RpcCall& call);
	void AZ_LOUDMATCHIFY_SetVolumeForMediaId_Parse(const RpcCall& call);
	void AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId_Parse(const RpcCall& call);
	void AZ_LOUDMATCHIFY_SetVolumeForMediaItemList_Parse(const RpcCall& call);

	/******************MIDDLESCANNER*****************************/
	MiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItem_Parse(const RpcCall& call);
	MiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaId_Parse(const RpcCall& call);
	MiddlewareInfo AZ_MIDDLESCANNER_Wwise_GetVolumeForSelectedMediaId_Parse(const RpcCall& call);
	std::vector<MiddlewareInfo> AZ_MIDDLESCANNER_Wwise_GetVolumeForMediaItemList_Parse(const RpcCall& call);

	/******************Excel*****************************/
	XLDocument* AZ_Excel_OpenFile_Parse(const RpcCall& call);
	std::vector<ExcelSheetInfo> AZ_Excel_GetWorksheetList_Parse(const RpcCall& call);
	std::tuple<std::vector<std::string>, std::vector<std::string>> AZ_RenderToAudioFile_Parse(const RpcCall& call);
	void AZ_Excel_CloseFile_Parse(const RpcCall& call);
	XLWorksheet* AZ_Excel_OpenWorksheet_Parse(const RpcCall& call);
	VarType AZ_Excel_GetCellValue_Parse(const RpcCall& call);
	XLWorksheet* AZ_Excel_OpenWorksheet_ByNumber_Parse(const RpcCall& call);
	VarType AZ_Excel_GetCellValue_ByNumber_Parse(const RpcCall& call);
	void AZ_Excel_SetCellValue_Parse(const RpcCall& call);
	void AZ_Excel_SetCellValue_ByNumber_Parse(const RpcCall& call);
	void AZ_Excel_SaveDocument_Parse(const RpcCall& call);
	XLDocument* AZ_Excel_CreateDocument_Parse(const RpcCall& call);
	XLWorksheet* AZ_Excel_AddWorksheet_Parse(const RpcCall& call);
	void AZ_Excel_RenameWorksheet_Parse(const RpcCall& call);
	void AZ_Excel_DeleteWorksheet_Parse(const RpcCall& call);
	ExcelSheetInfo AZ_Excel_SearchCellsInWorksheet_Parse(const RpcCall& call);
	ExcelSheetInfo AZ_Excel_SearchRowsInWorksheet_Parse(const RpcCall& call);
	std::vector<ExcelSheetInfo> AZ_Excel_SearchCellsInAllWorksheets_Parse(const RpcCall& call);
	std::vector<ExcelSheetInfo> AZ_Excel_SearchRowsInAllWorksheets_Parse(const RpcCall& call);

	/******************Render*****************************/
	RenderingOptions AZ_GetRenderSetting_Parse(const RpcCall& call);
	RenderingOptions AZ_GetRenderSettingFromProject_Parse(const RpcCall& call);
	void AZ_SetRenderSetting_Parse(const RpcCall& call);

	/****************** P4 *****************************/
	bool AZ_P4_Connect_Parse(const RpcCall& call);
	void AZ_P4_Disconnect_Parse(const RpcCall& call);
	std::vector<std::string> AZ_P4_SyncFiles_Parse(const RpcCall& call);
	std::vector<std::string> AZ_P4_ExportToDirectory_Parse(const RpcCall& call);
	std::vector<std::string> AZ_P4_ExportAudioToProject_Parse(const RpcCall& call);

	/****************** SVN *****************************/
	void AZ_SVN_SetAuth_Parse(const RpcCall& call);
	void AZ_SVN_CheckoutOrUpdate_Parse(const RpcCall& call);
	std::vector<std::string> AZ_SVN_Add_Parse(const RpcCall& call);
	int AZ_SVN_Commit_Parse(const RpcCall& call);
	void AZ_SVN_Revert_Parse(const RpcCall& call);
	std::vector<std::string> AZ_SVN_Export_Parse(const RpcCall& call);
	std::vector<std::string> AZ_SVN_GetFileList_Parse(const RpcCall& call);
	std::vector<std::string> AZ_SVN_GetAddAndUpdateList_Parse(const RpcCall& call);
	std::string AZ_SVN_GetRootUrl_Parse(const RpcCall& call);
	std::vector<std::string> AZ_SVN_Export_CommitOnly_Parse(const RpcCall& call);
	std::vector<std::string> AZ_SVN_ExportAudioToProject_Parse(const RpcCall& call);
	std::vector<std::string> AZ_SVN_ExportAudioToProject_CommitOnly_Parse(const RpcCall& call);

	/****************** IR *****************************/
	std::tuple<double, std::string, std::string> AZ_GenerateLinearTSP_Parse(const RpcCall& call);
	std::tuple<double, std::string, std::string> AZ_GenerateLogTSP_Parse(const RpcCall& call);
	void AZ_CalcImpulseResponseFromLinearTSP_Parse(const RpcCall& call);
	void AZ_CalcImpulseResponseFromLogTSP_Parse(const RpcCall& call);

	/****************** GSheet *****************************/
	GSheetConnection* AZ_GSheet_Connect_UserAccount_Parse(const RpcCall& call);
	void AZ_GSheet_Disconnect_Parse(const RpcCall& call);
	GSheetInfo AZ_GSheet_AddWorksheet_Parse(const RpcCall& call);
	void AZ_GSheet_DeleteWorksheet_Parse(const RpcCall& call);
	void AZ_GSheet_RenameWorksheet_Parse(const RpcCall& call);
	GSheetInfo AZ_GSheet_CopyWorksheet_Parse(const RpcCall& call);
	std::vector<GSheetInfo> AZ_GSheet_GetWorksheetList_Parse(const RpcCall& call);
	VarType AZ_GSheet_GetCellValue_Parse(const RpcCall& call);
	VarType AZ_GSheet_GetCellValue_ByNumber_Parse(const RpcCall& call);
	void AZ_GSheet_SetCellValue_Parse(const RpcCall& call);
	void AZ_GSheet_SetCellValue_ByNumber_Parse(const RpcCall& call);
	GSheetInfo AZ_GSheet_SearchCellsInWorksheet_Parse(const RpcCall& call);
	GSheetInfo AZ_GSheet_SearchRowsInWorksheet_Parse(const RpcCall& call);
	std::vector<GSheetInfo> AZ_GSheet_SearchCellsInAllWorksheets_Parse(const RpcCall& call);
	std::vector<GSheetInfo> AZ_GSheet_SearchRowsInAllWorksheets_Parse(const RpcCall& call);

}
