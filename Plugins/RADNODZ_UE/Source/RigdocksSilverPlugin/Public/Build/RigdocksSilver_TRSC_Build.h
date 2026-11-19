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
#include "RigdocksSilver_TRSC_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_TRSC_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_LoadModel_Build(FString modelPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_FullForMediaItem_Build(UTranscriptionContext* ctx, UMediaItem* item, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_FullForMediaID_Build(UTranscriptionContext* ctx, UReaProject* proj, int mediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_FullForSelectedMediaID_Build(UTranscriptionContext* ctx, UReaProject* proj, int selectedMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_FullForTrackMedia_Build(UTranscriptionContext* ctx, UReaProject* proj, int trackID, int trackMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_FullForSelectedTrackMedia_Build(UTranscriptionContext* ctx, UReaProject* proj, int selectedTrackID, int trackMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_SegmentsForMediaItem_Build(UTranscriptionContext* ctx, UMediaItem* item, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_SegmentsForMediaID_Build(UTranscriptionContext* ctx, UReaProject* proj, int mediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_SegmentsForSelectedMediaID_Build(UTranscriptionContext* ctx, UReaProject* proj, int selectedMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_SegmentsForTrackMedia_Build(UTranscriptionContext* ctx, UReaProject* proj, int trackID, int trackMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_SegmentsForSelectedTrackMedia_Build(UTranscriptionContext* ctx, UReaProject* proj, int selectedTrackID, int trackMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC|Build")
	static URigdocksRpcCall* AZ_TRSC_ReleaseModel_Build(UTranscriptionContext* ctx);
};
