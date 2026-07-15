// Fill out your copyright notice in the Description page of Project Settings.

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
