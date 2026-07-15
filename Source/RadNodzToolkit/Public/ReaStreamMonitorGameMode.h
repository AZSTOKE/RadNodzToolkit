// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReaStreamMonitorGameMode.generated.h"

/**
 * ReaStream 音声受信専用UIを起動時に表示するためのGameMode（Windows向け）。
 * 音声受信専用Mapの World Settings → GameMode Override に設定して使う。
 * 制御系（接続/録音/トラック操作）は一切持たない。
 */
UCLASS()
class RADNODZTOOLKIT_API AReaStreamMonitorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AReaStreamMonitorGameMode();
};
