// Fill out your copyright notice in the Description page of Project Settings.

#include "ReaStreamMonitorPlayerController.h"
#include "WG_ReaStreamMonitor.h"
#include "Blueprint/UserWidget.h"

AReaStreamMonitorPlayerController::AReaStreamMonitorPlayerController()
{
	MonitorWidgetClass = UWG_ReaStreamMonitor::StaticClass();
}

void AReaStreamMonitorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<UUserWidget> WidgetClass = MonitorWidgetClass;
	if (!WidgetClass)
	{
		WidgetClass = UWG_ReaStreamMonitor::StaticClass();
	}

	MonitorWidget = CreateWidget<UUserWidget>(this, WidgetClass);
	if (MonitorWidget)
	{
		MonitorWidget->AddToViewport();
	}

	// マウス操作できるようにする
	bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	if (MonitorWidget)
	{
		InputMode.SetWidgetToFocus(MonitorWidget->TakeWidget());
	}
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}
