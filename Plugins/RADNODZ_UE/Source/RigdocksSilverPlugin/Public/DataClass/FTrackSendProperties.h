// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReaperClasses.h"
#include "Object/TrackSendProperties.h"
#include "FTrackSendProperties.generated.h"

using namespace RADNODZ_client::Object;

UENUM(BlueprintType)
enum class AZ_TrackSendMode : uint8
{
	PostFader = 0,

	PreFaderPreFx = 1,

	PreFaderPostFx = 3,
};

USTRUCT(BlueprintType)
struct RIGDOCKSSILVERPLUGIN_API FTrackSendProperties
{
public:

	GENERATED_BODY()
	
	void Initialize(TrackSendProperties& obj);

	TrackSendProperties GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool Mute{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool setMute = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool Phase{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool setPhase = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool Mono{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool setMono = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	double Volume{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool setVolume = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	double VolDB{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool setVolDB = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	double Pan{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool setPan = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	AZ_TrackSendMode SendMode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool setSendMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	int Src_FirstChIndex{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool setSrc_FirstChIndex = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	int Src_NumChannels{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool setSrc_NumChannels = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	int Dst_FirstChIndex{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool setDst_FirstChIndex = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool Dst_MixToMono{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|TrackSendProperties")
	bool setDst_MixToMono = false;

};

UCLASS(BlueprintType)
class RIGDOCKSSILVERPLUGIN_API UTrackSendPropertiesFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TrackSendProperties")
	static bool FromJson(const FString& Json, FTrackSendProperties& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TrackSendProperties")
	static bool FromJsonArray(const FString& Json, TArray<FTrackSendProperties>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TrackSendProperties",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FTrackSendProperties& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|TrackSendProperties",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FTrackSendProperties>& objectArray, int indent = 4);

};
