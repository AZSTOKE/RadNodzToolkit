// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ReaperGameMode.h"
#include "ReaperPlayerController.h"

AReaperGameMode::AReaperGameMode()
{
	PlayerControllerClass = AReaperPlayerController::StaticClass();
}
