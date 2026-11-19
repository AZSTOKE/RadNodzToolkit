// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "Modules/ModuleManager.h"

PARAKNOTAUDIO_API DECLARE_LOG_CATEGORY_EXTERN(LogParaknotAudio, Log, All);

class FParaknotAudioModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
