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
#include "GoldClasses.h"
#include "DataClass/FLoudnessRecord.h"
#include "DataClass/FMiddlewareInfo.h"
#include "RigdocksGold_LOUDMATCHIFY.h"
#include "RigdocksGold_LOUDMATCHIFY_Build.generated.h"

UCLASS()
class RIGDOCKSGOLDPLUGIN_API URigdocksGold_LOUDMATCHIFY_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY|Build",
		meta = (CPP_Default_volumeType = "Momentary"))
	static URigdocksRpcCall* AZ_LOUDMATCHIFY_SetVolumeForMediaItem_Build(UMediaItem* item, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY|Build",
		meta = (CPP_Default_volumeType = "Momentary"))
	static URigdocksRpcCall* AZ_LOUDMATCHIFY_SetVolumeForMediaId_Build(UReaProject* proj, int itemID, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY|Build",
		meta = (CPP_Default_volumeType = "Momentary"))
	static URigdocksRpcCall* AZ_LOUDMATCHIFY_SetVolumeForSelectedMediaId_Build(UReaProject* proj, int selectedItemID, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, double additionalVolume);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|GOLD|LOUDMATCHIFY|Build",
		meta = (AutoCreateRefTerm = "additionalVolumeList", CPP_Default_volumeType = "Momentary"))
	static URigdocksRpcCall* AZ_LOUDMATCHIFY_SetVolumeForMediaItemList_Build(TArray<UMediaItem*> itemList, TArray<FLoudnessRecord> loudnessRecord, LOUDMATCHIFY_VolumeType volumeType, TArray<FMiddlewareInfo> additionalVolumeList);
};
