// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FTrackDetail.h"
#include "DataClass/FTrackSendProperties.h"
#include "RigdocksSilver_Track.generated.h"

UENUM(BlueprintType)
enum class AZ_InputChannelType : uint8
{
	Audio = 0,
	Midi = 1,
};

// record monitoring (REAPER I_RECMON)
UENUM(BlueprintType)
enum class AZ_RecordMonitorMode : uint8
{
	Off = 0,
	On = 1,
	Auto = 2,	// monitor only while not playing (tape auto style)
};

UENUM(BlueprintType)
enum class AZ_TrackMeterMode : uint8
{
	StereoPeaks = 0,
	MultichannelPeaks = 2,
	StereoRMS = 4,
	CombinedRMS = 8,
	LufsM = 12,
	LufsS_ReadoutMax = 16,
	LufsS_ReadoutCurrent = 20,
};

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Track : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaTrack* AZ_GetTrackItemSelect(URadnodzClient* client, UReaProject* proj, int trackID, int trackType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackItemIndex(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaTrack* AZ_GetSelectedTrackFirstInfo(URadnodzClient* client, UReaProject* proj, int& trackId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaTrack* AZ_InsertTrackSearchUniqueOnly(URadnodzClient* client, UReaProject* proj, FString setTrackName, int& trackId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackCountSelect(URadnodzClient* client, UReaProject* proj, int trackType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetTrackItemName(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetTrackIdName(URadnodzClient* client, UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_GetSelectedTrackIdName(URadnodzClient* client, UReaProject* proj, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackItemDepth(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackIdDepth(URadnodzClient* client, UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedTrackIdDepth(URadnodzClient* client, UReaProject* proj, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemDepth(URadnodzClient* client, UMediaTrack* track, int setDepth);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdDepth(URadnodzClient* client, UReaProject* proj, int trackID, int setDepth);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdDepth(URadnodzClient* client, UReaProject* proj, int selectTrackID, int setDepth);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemName(URadnodzClient* client, UMediaTrack* track, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdName(URadnodzClient* client, UReaProject* proj, int trackID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdName(URadnodzClient* client, UReaProject* proj, int selectTrackID, FString setName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetTrackItemMute(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetTrackIdMute(URadnodzClient* client, UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetSelectedTrackIdMute(URadnodzClient* client, UReaProject* proj, int selecteTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemMute(URadnodzClient* client, UMediaTrack* track, bool setMute);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdMute(URadnodzClient* client, UReaProject* proj, int trackID, bool setMute);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdMute(URadnodzClient* client, UReaProject* proj, int selecteTrackID, bool setMute);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackAllMuteOFF(URadnodzClient* client, UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetTrackItemSolo(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetTrackIdSolo(URadnodzClient* client, UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetSelectedTrackIdSolo(URadnodzClient* client, UReaProject* proj, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemSolo(URadnodzClient* client, UMediaTrack* track, bool setSolo);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdSolo(URadnodzClient* client, UReaProject* proj, int trackID, bool setSolo);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdSolo(URadnodzClient* client, UReaProject* proj, int selectTrackID, bool setSolo);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackAllSoloOFF(URadnodzClient* client, UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackItemColor(URadnodzClient* client, UMediaTrack* track, int& R, int& G, int& B);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackIdColor(URadnodzClient* client, UReaProject* proj, int trackID, int& R, int& G, int& B);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedTrackIdColor(URadnodzClient* client, UReaProject* proj, int selectTrackID, int& R, int& G, int& B);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemColor(URadnodzClient* client, UMediaTrack* track, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdColor(URadnodzClient* client, UReaProject* proj, int trackID, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdColor(URadnodzClient* client, UReaProject* proj, int selectTrackID, int setColor);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemRGB(URadnodzClient* client, UMediaTrack* track, int setR, int setG, int setB);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdRGB(URadnodzClient* client, UReaProject* proj, int trackID, int setR, int setG, int setB);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdRGB(URadnodzClient* client, UReaProject* proj, int selectTrackID, int setR, int setG, int setB);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdSelected(URadnodzClient* client, UReaProject* proj, int trackID, bool setSelectType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemOnlySelected(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdOnlySelected(URadnodzClient* client, UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetTrackItemVolume(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetTrackIdVolume(URadnodzClient* client, UReaProject* proj, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetSelectedTrackIdVolume(URadnodzClient* client, UReaProject* proj, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemVolume(URadnodzClient* client, UMediaTrack* track, double setVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdVolume(URadnodzClient* client, UReaProject* proj, int trackID, double setVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdVolume(URadnodzClient* client, UReaProject* proj, int trackID, double setVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemAddVolume(URadnodzClient* client, UMediaTrack* track, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdAddVolume(URadnodzClient* client, UReaProject* proj, int trackID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdAddVolume(URadnodzClient* client, UReaProject* proj, int trackID, double addVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteTrackIdSelect(URadnodzClient* client, UReaProject* proj, int trackID, int trackType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<UMediaTrack*> AZ_GetTrackItemList(URadnodzClient* client, UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_isRecursive = "false"
			))
	static TArray<UMediaTrack*>  AZ_GetTrackItemChildList(URadnodzClient* client, UMediaTrack* track, bool isRecursive, int& trackCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_isRecursive = "false"
			))
	static TArray<UMediaTrack*> AZ_GetTrackIdChildList(URadnodzClient* client, UReaProject* project, int trackID, bool isRecursive, int& trackCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_isRecursive = "false"
			))
	static TArray<UMediaTrack*> AZ_GetSelectedTrackIdChildList(URadnodzClient* client, UReaProject* project, int selectTrackID, bool isRecursive, int& trackCount);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FTrackDetail> AZ_GetTrackListDetail(URadnodzClient* client, UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FTrackDetail> AZ_GetSelectedTrackListDetail(URadnodzClient* client, UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FTrackDetail> AZ_GetTrackDepthList(URadnodzClient* client, UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FTrackDetail> AZ_GetSamePerfectTrackNameList(URadnodzClient* client, UReaProject* project, FString _checkName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FTrackDetail> AZ_GetIncludePerfectTrackNameList(URadnodzClient* client, UReaProject* project, FString _checkName);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<UMediaTrack*> AZ_GetSelectedTrackItemList(URadnodzClient* client, UReaProject* project);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaTrack* AZ_InsertUniqueChildTrack(URadnodzClient* client, UMediaTrack* parentTrack, FString setName, int& trackId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaTrack* AZ_InsertTrackId(URadnodzClient* client, UReaProject* proj, int trackID, FString setName = "");

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UMediaTrack* AZ_InsertChildTrack(URadnodzClient* client, UMediaTrack* parentTrack, FString setName, int& trackId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetTrackItemRecording(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetTrackIdRecording(URadnodzClient* client, UReaProject* projID, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetSelectedTrackIdRecording(URadnodzClient* client, UReaProject* projID, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FTrackDetail> AZ_GetRecordingTrackList(URadnodzClient* client, UReaProject* projID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemRecording(URadnodzClient* client, UMediaTrack* track, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdRecording(URadnodzClient* client, UReaProject* projID, int trackID, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdRecording(URadnodzClient* client, UReaProject* projID, int selectTrackID, bool setRec);

	// record monitoring. Get returns Off when the call failed (check the error code)
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static AZ_RecordMonitorMode AZ_GetTrackItemRecordMonitoring(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static AZ_RecordMonitorMode AZ_GetTrackIdRecordMonitoring(URadnodzClient* client, UReaProject* projID, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static AZ_RecordMonitorMode AZ_GetSelectedTrackIdRecordMonitoring(URadnodzClient* client, UReaProject* projID, int selectTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemRecordMonitoring(URadnodzClient* client, UMediaTrack* track, AZ_RecordMonitorMode monitorMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdRecordMonitoring(URadnodzClient* client, UReaProject* projID, int trackID, AZ_RecordMonitorMode monitorMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdRecordMonitoring(URadnodzClient* client, UReaProject* projID, int selectTrackID, AZ_RecordMonitorMode monitorMode);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackAllRecording(URadnodzClient* client, UReaProject* projID, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemOnlyRecording(URadnodzClient* client, UMediaTrack* track, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdOnlyRecording(URadnodzClient* client, UReaProject* projID, int trackID, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdOnlyRecording(URadnodzClient* client, UReaProject* projID, int selectedTrackID, bool setRec);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetTrackItemCurrentPeak(URadnodzClient* client, UMediaTrack* track, int chIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetTrackItemCurrentPeakDB(URadnodzClient* client, UMediaTrack* track, int chIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetTrackItemHoldPeakDB(URadnodzClient* client, UMediaTrack* track, int chIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_ResetTrackItemHoldPeak(URadnodzClient* client, UMediaTrack* track, int chIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackItemNumChannels(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetTrackIdNumChannels(URadnodzClient* client, UReaProject* projID, int trackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_GetSelectedTrackIdNumChannels(URadnodzClient* client, UReaProject* projID, int selectedTrackID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemNumChannels(URadnodzClient* client, UMediaTrack* track, int numChs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdNumChannels(URadnodzClient* client, UReaProject* projID, int trackID, int numChs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdNumChannels(URadnodzClient* client, UReaProject* projID, int selectedTrackID, int numChs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetTrackItemInputChannel(URadnodzClient* client, UMediaTrack* track, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetTrackIdInputChannel(URadnodzClient* client, UReaProject* projID, int trackID, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetSelectedTrackIdInputChannel(URadnodzClient* client, UReaProject* projID, int selectedTrackID, int& firstChannelIndex, int& numChannels, AZ_InputChannelType& inputChType);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemInputChannel(URadnodzClient* client, UMediaTrack* track, int firstChannelIndex, int numChannels = 1, AZ_InputChannelType inputChType = AZ_InputChannelType::Audio);
	
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackIdInputChannel(URadnodzClient* client, UReaProject* projID, int trackID, int firstChannelIndex, int numChannels = 1, AZ_InputChannelType inputChType = AZ_InputChannelType::Audio);
	
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetSelectedTrackIdInputChannel(URadnodzClient* client, UReaProject* projID, int selectedTrackID, int firstChannelIndex, int numChannels = 1, AZ_InputChannelType inputChType = AZ_InputChannelType::Audio);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetTrackItemMeterMode(URadnodzClient* client, UMediaTrack* track, AZ_TrackMeterMode& meterMode, bool& enabled, bool& lufsChannels1And2Only);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemMeterMode(URadnodzClient* client, UMediaTrack* track, AZ_TrackMeterMode meterMode, bool enabled, bool lufsChannels1And2Only);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetTrackItemFxEnabled(URadnodzClient* client, UMediaTrack* track, int fxIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemFxEnabled(URadnodzClient* client, UMediaTrack* track, int fxIndex, bool enabled);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_GetTrackItemMainSend(URadnodzClient* client, UMediaTrack* track, bool& sendParent, int& firstChIndex, int& numChs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemMainSend(URadnodzClient* client, UMediaTrack* track, bool sendParent, int firstChIndex, int numChs);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FTrackDetail> AZ_GetTrackItemSendList(URadnodzClient* client, UMediaTrack* track);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_AddTrackItemSend(URadnodzClient* client, UMediaTrack* track, UMediaTrack* destTrack, bool isUnique = false);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FTrackSendProperties AZ_GetTrackItemSendProperties(URadnodzClient* client, UMediaTrack* track, int sendIndex);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_SetTrackItemSendProperties(URadnodzClient* client, UMediaTrack* track, int sendIndex, FTrackSendProperties props);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Track",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_DeleteTrackItemSend(URadnodzClient* client, UMediaTrack* track, int sendIndex);

};
