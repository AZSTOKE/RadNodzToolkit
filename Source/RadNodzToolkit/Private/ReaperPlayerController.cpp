// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ReaperPlayerController.h"
#include "WG_RadNodzToolkit.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"

AReaperPlayerController::AReaperPlayerController()
{
	ControllerWidgetClass = UWG_RadNodzToolkit::StaticClass();
}

void AReaperPlayerController::BeginPlay()
{
	Super::BeginPlay();

#if !WITH_EDITOR
	// パッケージ版はクック済みの既定がボーダーレス全画面（FullscreenMode=1）で起動する。
	// デスクトップのショートカットは起動引数 -windowed で回避しているが、ZIPを解凍して素のexeを
	// 直接起動すると引数が付かず全画面になってしまう。ここで起動時に必ずウィンドウモードへ切り替え、
	// exeを直接叩いても（＝ショートカット無しで配布しても）リサイズ可能なウィンドウで立ち上げる。
	// ※iniの既定値はクック済みビルドでは無視されるため、実行時にコードで適用する必要がある。
	if (GEngine)
	{
		if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
		{
			if (Settings->GetFullscreenMode() != EWindowMode::Windowed)
			{
				Settings->SetFullscreenMode(EWindowMode::Windowed);
				// 全画面既定から切り替える初回は、ほどよいウィンドウサイズにする（デスクトップより大きくしない）。
				const FIntPoint Desktop = Settings->GetDesktopResolution();
				const int32 WinW = FMath::Min(1600, Desktop.X);
				const int32 WinH = FMath::Min(900, Desktop.Y);
				Settings->SetScreenResolution(FIntPoint(WinW, WinH));
				Settings->ApplyResolutionSettings(false);
			}
		}
	}
#endif

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
