// Fill out your copyright notice in the Description page of Project Settings.

#include "ReaStreamMonitorGameMode.h"
#include "ReaStreamMonitorPlayerController.h"

AReaStreamMonitorGameMode::AReaStreamMonitorGameMode()
{
	PlayerControllerClass = AReaStreamMonitorPlayerController::StaticClass();
}
