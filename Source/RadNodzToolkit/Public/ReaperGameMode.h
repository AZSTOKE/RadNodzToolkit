// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReaperGameMode.generated.h"

/**
 * ReaperコントローラーUIを起動時に表示するためのGameMode。
 * プロジェクト既定GameMode(DefaultEngine.ini)に設定して使う。
 */
UCLASS()
class RADNODZTOOLKIT_API AReaperGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AReaperGameMode();
};
