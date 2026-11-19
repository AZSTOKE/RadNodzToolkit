// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FWhisperSegment.h"
#include "RigdocksSilver_TRSC.generated.h"

class TranscriptionContext;

UCLASS(BlueprintType)
class RIGDOCKSSILVERPLUGIN_API UTranscriptionContext : public UReaperClass
{
public:
	GENERATED_BODY()

	inline static TranscriptionContext* Get(UTranscriptionContext* ptr)
	{
		return ptr ? (TranscriptionContext*)ptr->GetPointer() : nullptr;
	}

};

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_TRSC : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static UTranscriptionContext* AZ_TRSC_LoadModel(URadnodzClient* client, FString modelPath);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_TRSC_FullForMediaItem(URadnodzClient* client, UTranscriptionContext* ctx, UMediaItem* item, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_TRSC_FullForMediaID(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int mediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_TRSC_FullForSelectedMediaID(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int selectedMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_TRSC_FullForTrackMedia(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int trackID, int trackMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FString AZ_TRSC_FullForSelectedTrackMedia(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int selectedTrackID, int trackMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FWhisperSegment> AZ_TRSC_SegmentsForMediaItem(URadnodzClient* client, UTranscriptionContext* ctx, UMediaItem* item, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FWhisperSegment> AZ_TRSC_SegmentsForMediaID(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int mediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FWhisperSegment> AZ_TRSC_SegmentsForSelectedMediaID(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int selectedMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FWhisperSegment> AZ_TRSC_SegmentsForTrackMedia(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int trackID, int trackMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static TArray<FWhisperSegment> AZ_TRSC_SegmentsForSelectedTrackMedia(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int selectedTrackID, int trackMediaID, FString language);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|TRSC",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static void AZ_TRSC_ReleaseModel(URadnodzClient* client, UTranscriptionContext* ctx);

};
