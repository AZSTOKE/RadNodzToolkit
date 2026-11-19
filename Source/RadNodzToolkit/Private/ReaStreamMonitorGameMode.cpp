// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ReaStreamMonitorGameMode.h"
#include "ReaStreamMonitorPlayerController.h"

AReaStreamMonitorGameMode::AReaStreamMonitorGameMode()
{
	PlayerControllerClass = AReaStreamMonitorPlayerController::StaticClass();
}
