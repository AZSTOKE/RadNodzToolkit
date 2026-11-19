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
#include "RigdocksSilver_Track.h"
#include "DataClass/FTrackSendProperties.h"
#include "RigdocksSilver_Track_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Track_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemSelect_Build(UReaProject* proj, int trackID, int trackType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemName_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemName_Build(UMediaTrack* track, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemIndex_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackFirstInfo_Build(UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_InsertTrackSearchUniqueOnly_Build(UReaProject* proj, FString setTrackName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackCountSelect_Build(UReaProject* proj, int trackType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackIdName_Build(UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackIdName_Build(UReaProject* proj, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemDepth_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackIdDepth_Build(UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackIdDepth_Build(UReaProject* proj, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemDepth_Build(UMediaTrack* track, int setDepth);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdDepth_Build(UReaProject* proj, int trackID, int setDepth);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdDepth_Build(UReaProject* proj, int selectTrackID, int setDepth);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdName_Build(UReaProject* proj, int trackID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdName_Build(UReaProject* proj, int selectTrackID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemMute_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackIdMute_Build(UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackIdMute_Build(UReaProject* proj, int selecteTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemMute_Build(UMediaTrack* track, bool setMute);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdMute_Build(UReaProject* proj, int trackID, bool setMute);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdMute_Build(UReaProject* proj, int selecteTrackID, bool setMute);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackAllMuteOFF_Build(UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemSolo_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackIdSolo_Build(UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackIdSolo_Build(UReaProject* proj, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemSolo_Build(UMediaTrack* track, bool setSolo);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdSolo_Build(UReaProject* proj, int trackID, bool setSolo);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdSolo_Build(UReaProject* proj, int selectTrackID, bool setSolo);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackAllSoloOFF_Build(UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemColor_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackIdColor_Build(UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackIdColor_Build(UReaProject* proj, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemColor_Build(UMediaTrack* track, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdColor_Build(UReaProject* proj, int trackID, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdColor_Build(UReaProject* proj, int selectTrackID, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemRGB_Build(UMediaTrack* track, int setR, int setG, int setB);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdRGB_Build(UReaProject* proj, int trackID, int setR, int setG, int setB);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdRGB_Build(UReaProject* proj, int selectTrackID, int setR, int setG, int setB);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdSelected_Build(UReaProject* proj, int trackID, bool setSelectType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemOnlySelected_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdOnlySelected_Build(UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemVolume_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackIdVolume_Build(UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackIdVolume_Build(UReaProject* proj, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemVolume_Build(UMediaTrack* track, double setVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdVolume_Build(UReaProject* proj, int trackID, double setVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdVolume_Build(UReaProject* proj, int trackID, double setVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemAddVolume_Build(UMediaTrack* track, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdAddVolume_Build(UReaProject* proj, int trackID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdAddVolume_Build(UReaProject* proj, int trackID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_DeleteTrackIdSelect_Build(UReaProject* proj, int trackID, int trackType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemList_Build(UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemChildList_Build(UMediaTrack* track, bool isRecursive);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackIdChildList_Build(UReaProject* project, int trackID, bool isRecursive);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackIdChildList_Build(UReaProject* project, int selectTrackID, bool isRecursive);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackListDetail_Build(UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackListDetail_Build(UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackDepthList_Build(UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSamePerfectTrackNameList_Build(UReaProject* project, FString _checkName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetIncludePerfectTrackNameList_Build(UReaProject* project, FString _checkName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackItemList_Build(UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_InsertUniqueChildTrack_Build(UMediaTrack* parentTrack, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_InsertTrackId_Build(UReaProject* proj, int trackID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_InsertChildTrack_Build(UMediaTrack* parentTrack, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemRecording_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackIdRecording_Build(UReaProject* projID, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackIdRecording_Build(UReaProject* projID, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetRecordingTrackList_Build(UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemRecording_Build(UMediaTrack* track, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdRecording_Build(UReaProject* projID, int trackID, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdRecording_Build(UReaProject* projID, int selectTrackID, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemRecordMonitoring_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackIdRecordMonitoring_Build(UReaProject* projID, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackIdRecordMonitoring_Build(UReaProject* projID, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemRecordMonitoring_Build(UMediaTrack* track, AZ_RecordMonitorMode monitorMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdRecordMonitoring_Build(UReaProject* projID, int trackID, AZ_RecordMonitorMode monitorMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdRecordMonitoring_Build(UReaProject* projID, int selectTrackID, AZ_RecordMonitorMode monitorMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackAllRecording_Build(UReaProject* projID, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemOnlyRecording_Build(UMediaTrack* track, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdOnlyRecording_Build(UReaProject* projID, int trackID, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdOnlyRecording_Build(UReaProject* projID, int selectedTrackID, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemCurrentPeak_Build(UMediaTrack* track, int chIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemCurrentPeakDB_Build(UMediaTrack* track, int chIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemHoldPeakDB_Build(UMediaTrack* track, int chIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_ResetTrackItemHoldPeak_Build(UMediaTrack* track, int chIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemNumChannels_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackIdNumChannels_Build(UReaProject* projID, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackIdNumChannels_Build(UReaProject* projID, int selectedTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemNumChannels_Build(UMediaTrack* track, int numChs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdNumChannels_Build(UReaProject* projID, int trackID, int numChs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdNumChannels_Build(UReaProject* projID, int selectedTrackID, int numChs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemInputChannel_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackIdInputChannel_Build(UReaProject* projID, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetSelectedTrackIdInputChannel_Build(UReaProject* projID, int selectedTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemInputChannel_Build(UMediaTrack* track, int firstChannelIndex, int numChannels = 1, AZ_InputChannelType inputChType = AZ_InputChannelType::Audio);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackIdInputChannel_Build(UReaProject* projID, int trackID, int firstChannelIndex, int numChannels = 1, AZ_InputChannelType inputChType = AZ_InputChannelType::Audio);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetSelectedTrackIdInputChannel_Build(UReaProject* projID, int selectedTrackID, int firstChannelIndex, int numChannels = 1, AZ_InputChannelType inputChType = AZ_InputChannelType::Audio);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemMeterMode_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemMeterMode_Build(UMediaTrack* track, AZ_TrackMeterMode meterMode, bool enabled, bool lufsChannels1And2Only);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemFxEnabled_Build(UMediaTrack* track, int fxIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemFxEnabled_Build(UMediaTrack* track, int fxIndex, bool enabled);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemMainSend_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemMainSend_Build(UMediaTrack* track, bool sendParent, int firstChIndex, int numChs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemSendList_Build(UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_AddTrackItemSend_Build(UMediaTrack* track, UMediaTrack* destTrack, bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_GetTrackItemSendProperties_Build(UMediaTrack* track, int sendIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_SetTrackItemSendProperties_Build(UMediaTrack* track, int sendIndex, FTrackSendProperties props);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Build")
	static URigdocksRpcCall* AZ_DeleteTrackItemSend_Build(UMediaTrack* track, int sendIndex);
};
