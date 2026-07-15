// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#include "ReaStreamMonitorGameMode.h"
#include "ReaStreamMonitorPlayerController.h"

AReaStreamMonitorGameMode::AReaStreamMonitorGameMode()
{
	PlayerControllerClass = AReaStreamMonitorPlayerController::StaticClass();
}
