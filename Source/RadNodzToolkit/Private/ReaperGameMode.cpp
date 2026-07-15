// Fill out your copyright notice in the Description page of Project Settings.

#include "ReaperGameMode.h"
#include "ReaperPlayerController.h"

AReaperGameMode::AReaperGameMode()
{
	PlayerControllerClass = AReaperPlayerController::StaticClass();
}
