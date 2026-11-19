// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReaperClasses.h"
#include "Object/WwiseStateGroup.h"
#include "FWwiseStateGroup.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FWwiseState
{
public:

	GENERATED_BODY()
	
	void Initialize(WwiseState& obj);

	WwiseState GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	FString ID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	FString CustomStateName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	FString CustomStateID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	FString ShortID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	bool BypassEffect{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	bool BypassEffect0{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	bool BypassEffect1{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	bool BypassEffect2{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	bool BypassEffect3{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  GameAuxSendHPF{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  GameAuxSendLPF{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	double GameAuxSendVolume{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  Highpass{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	double InitialDelay{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  Lowpass{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	double MakeUpGain{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int MidiTransposition{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int MidiVelocityOffset{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  OutputBusHighpass{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  OutputBusLowpass{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	double OutputBusVolume{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int Pitch{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  Priority{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	double ReflectionsVolume{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  UserAuxSendHPF0{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  UserAuxSendHPF1{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  UserAuxSendHPF2{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  UserAuxSendHPF3{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  UserAuxSendLPF0{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  UserAuxSendLPF1{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  UserAuxSendLPF2{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	int  UserAuxSendLPF3{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	double UserAuxSendVolume0{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	double UserAuxSendVolume1{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	double UserAuxSendVolume2{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	double UserAuxSendVolume3{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseState")
	double Volume{};

};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UWwiseStateFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseState")
	static bool FromJson(const FString& Json, FWwiseState& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseState")
	static bool FromJsonArray(const FString& Json, TArray<FWwiseState>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseState",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FWwiseState& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseState",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FWwiseState>& objectArray, int indent = 4);

};

USTRUCT(BlueprintType)
struct RIGDOCKSGOLDPLUGIN_API FWwiseStateGroup
{
public:

	GENERATED_BODY()

	void Initialize(WwiseStateGroup& obj);

	WwiseStateGroup GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseStateGroup")
	FString ID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseStateGroup")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|WwiseStateGroup")
	TArray<FWwiseState> State{};
};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UWwiseStateGroupFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseStateGroup")
	static bool FromJson(const FString& Json, FWwiseStateGroup& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseStateGroup")
	static bool FromJsonArray(const FString& Json, TArray<FWwiseStateGroup>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseStateGroup",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FWwiseStateGroup& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|WwiseStateGroup",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FWwiseStateGroup>& objectArray, int indent = 4);

};
