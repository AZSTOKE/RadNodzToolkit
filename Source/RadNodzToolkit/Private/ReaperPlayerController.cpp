// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#include "ReaperPlayerController.h"
#include "WG_RadNodzToolkit.h"
#include "Blueprint/UserWidget.h"

AReaperPlayerController::AReaperPlayerController()
{
	ControllerWidgetClass = UWG_RadNodzToolkit::StaticClass();
}

void AReaperPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<UUserWidget> WidgetClass = ControllerWidgetClass;
	if (!WidgetClass)
	{
		WidgetClass = UWG_RadNodzToolkit::StaticClass();
	}

	ControllerWidget = CreateWidget<UUserWidget>(this, WidgetClass);
	if (ControllerWidget)
	{
		ControllerWidget->AddToViewport();
	}

	// マウス操作できるようにする
	bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	if (ControllerWidget)
	{
		InputMode.SetWidgetToFocus(ControllerWidget->TakeWidget());
	}
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}
