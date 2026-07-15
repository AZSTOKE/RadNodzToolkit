// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



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
