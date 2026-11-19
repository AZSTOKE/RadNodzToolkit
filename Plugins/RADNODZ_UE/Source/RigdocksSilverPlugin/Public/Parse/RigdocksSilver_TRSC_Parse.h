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
#include "RigdocksSilver_TRSC.h"
#include "DataClass/FWhisperSegment.h"
#include "RigdocksSilver_TRSC_Parse.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_TRSC_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static UTranscriptionContext* AZ_TRSC_LoadModel_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static FString AZ_TRSC_FullForMediaItem_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static FString AZ_TRSC_FullForMediaID_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static FString AZ_TRSC_FullForSelectedMediaID_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static FString AZ_TRSC_FullForTrackMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static FString AZ_TRSC_FullForSelectedTrackMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static TArray<FWhisperSegment> AZ_TRSC_SegmentsForMediaItem_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static TArray<FWhisperSegment> AZ_TRSC_SegmentsForMediaID_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static TArray<FWhisperSegment> AZ_TRSC_SegmentsForSelectedMediaID_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static TArray<FWhisperSegment> AZ_TRSC_SegmentsForTrackMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static TArray<FWhisperSegment> AZ_TRSC_SegmentsForSelectedTrackMedia_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Parse")
	static void AZ_TRSC_ReleaseModel_Parse(URigdocksRpcCall* call);
};
