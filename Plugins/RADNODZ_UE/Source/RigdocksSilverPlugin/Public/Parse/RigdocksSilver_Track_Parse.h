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
#include "DataClass/FTrackDetail.h"
#include "DataClass/FTrackSendProperties.h"
#include "RigdocksSilver_Track_Parse.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Track_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static UMediaTrack* AZ_GetTrackItemSelect_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static FString AZ_GetTrackItemName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_GetTrackItemIndex_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static UMediaTrack* AZ_GetSelectedTrackFirstInfo_Parse(URigdocksRpcCall* call, int& trackId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static UMediaTrack* AZ_InsertTrackSearchUniqueOnly_Parse(URigdocksRpcCall* call, int& trackId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_GetTrackCountSelect_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static FString AZ_GetTrackIdName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static FString AZ_GetSelectedTrackIdName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_GetTrackItemDepth_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_GetTrackIdDepth_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_GetSelectedTrackIdDepth_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemDepth_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdDepth_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdDepth_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdName_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static bool AZ_GetTrackItemMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static bool AZ_GetTrackIdMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static bool AZ_GetSelectedTrackIdMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdMute_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackAllMuteOFF_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static bool AZ_GetTrackItemSolo_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static bool AZ_GetTrackIdSolo_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static bool AZ_GetSelectedTrackIdSolo_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemSolo_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdSolo_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdSolo_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackAllSoloOFF_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_GetTrackItemColor_Parse(URigdocksRpcCall* call, int& R, int& G, int& B);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_GetTrackIdColor_Parse(URigdocksRpcCall* call, int& R, int& G, int& B);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_GetSelectedTrackIdColor_Parse(URigdocksRpcCall* call, int& R, int& G, int& B);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemColor_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdColor_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdColor_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemRGB_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdRGB_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdRGB_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdSelected_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemOnlySelected_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdOnlySelected_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static double AZ_GetTrackItemVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static double AZ_GetTrackIdVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static double AZ_GetSelectedTrackIdVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemAddVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdAddVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdAddVolume_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_DeleteTrackIdSelect_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<UMediaTrack*> AZ_GetTrackItemList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<UMediaTrack*> AZ_GetTrackItemChildList_Parse(URigdocksRpcCall* call, int& trackCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<UMediaTrack*> AZ_GetTrackIdChildList_Parse(URigdocksRpcCall* call, int& trackCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<UMediaTrack*> AZ_GetSelectedTrackIdChildList_Parse(URigdocksRpcCall* call, int& trackCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<FTrackDetail> AZ_GetTrackListDetail_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<FTrackDetail> AZ_GetSelectedTrackListDetail_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<FTrackDetail> AZ_GetTrackDepthList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<FTrackDetail> AZ_GetSamePerfectTrackNameList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<FTrackDetail> AZ_GetIncludePerfectTrackNameList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<UMediaTrack*> AZ_GetSelectedTrackItemList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static UMediaTrack* AZ_InsertUniqueChildTrack_Parse(URigdocksRpcCall* call, int& trackId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static UMediaTrack* AZ_InsertTrackId_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static UMediaTrack* AZ_InsertChildTrack_Parse(URigdocksRpcCall* call, int& trackId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static bool AZ_GetTrackItemRecording_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static bool AZ_GetTrackIdRecording_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static bool AZ_GetSelectedTrackIdRecording_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<FTrackDetail> AZ_GetRecordingTrackList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemRecording_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdRecording_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdRecording_Parse(URigdocksRpcCall* call);

	// returns Off when the call failed (check the error code)
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static AZ_RecordMonitorMode AZ_GetTrackItemRecordMonitoring_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static AZ_RecordMonitorMode AZ_GetTrackIdRecordMonitoring_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static AZ_RecordMonitorMode AZ_GetSelectedTrackIdRecordMonitoring_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemRecordMonitoring_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdRecordMonitoring_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdRecordMonitoring_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackAllRecording_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemOnlyRecording_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdOnlyRecording_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdOnlyRecording_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static double AZ_GetTrackItemCurrentPeak_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static double AZ_GetTrackItemCurrentPeakDB_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static double AZ_GetTrackItemHoldPeakDB_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_ResetTrackItemHoldPeak_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_GetTrackItemNumChannels_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_GetTrackIdNumChannels_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_GetSelectedTrackIdNumChannels_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemNumChannels_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdNumChannels_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdNumChannels_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_GetTrackItemInputChannel_Parse(URigdocksRpcCall* call, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_GetTrackIdInputChannel_Parse(URigdocksRpcCall* call, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_GetSelectedTrackIdInputChannel_Parse(URigdocksRpcCall* call, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemInputChannel_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackIdInputChannel_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetSelectedTrackIdInputChannel_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_GetTrackItemMeterMode_Parse(URigdocksRpcCall* call, AZ_TrackMeterMode& meterMode, bool& enabled, bool& lufsChannels1And2Only);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemMeterMode_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static bool AZ_GetTrackItemFxEnabled_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemFxEnabled_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_GetTrackItemMainSend_Parse(URigdocksRpcCall* call, bool& sendParent, int& firstChIndex, int& numChs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemMainSend_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static TArray<FTrackDetail> AZ_GetTrackItemSendList_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static int AZ_AddTrackItemSend_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static FTrackSendProperties AZ_GetTrackItemSendProperties_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_SetTrackItemSendProperties_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track|Parse")
	static void AZ_DeleteTrackItemSend_Parse(URigdocksRpcCall* call);
};
