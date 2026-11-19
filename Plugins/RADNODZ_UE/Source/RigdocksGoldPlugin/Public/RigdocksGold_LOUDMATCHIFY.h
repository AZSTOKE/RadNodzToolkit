// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "GoldClasses.h"
#include "DataClass/FLoudnessRecord.h"
#include "DataClass/FMiddlewareInfo.h"
#include "RigdocksGold_LOUDMATCHIFY.generated.h"

UENUM(BlueprintType)
enum class LOUDMATCHIFY_VolumeType : uint8
{
	Other = 0,
	Momentary = 1,
	RMS = 2,
	Peak = 3,
	Vol = 4,
};

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_LOUDMATCHIFY : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_volumeType = "Momentary"
			))
	static 	void  AZ_LOUDMATCHIFY_SetVolumeForMediaItem(URadnodzClient* client, UMediaItem* item, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_volumeType = "Momentary"
			))
	static 	void  AZ_LOUDMATCHIFY_SetVolumeForMediaId(URadnodzClient* client, UReaProject* proj, int itemID, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client",
			CPP_Default_volumeType = "Momentary"
			))
	static 	void  AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId(URadnodzClient* client, UReaProject* proj, int selectedItemID, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY",
		meta = (AutoCreateRefTerm = "additionalVolumeList",
			CPP_Default_volumeType = "Momentary"
			))
	static 	void  AZ_LOUDMATCHIFY_SetVolumeForMediaItemList(URadnodzClient* client, TArray<UMediaItem*> itemList, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, TArray<FMiddlewareInfo> additionalVolumeList);

};
