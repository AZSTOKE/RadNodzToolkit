// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ReaperPlayerController.generated.h"

class UUserWidget;

/**
 * 起動(Play)時にReaperコントローラーUIを画面へ表示するプレイヤーコントローラー。
 * AReaperGameMode から使用される。
 */
UCLASS()
class RADNODZTOOLKIT_API AReaperPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AReaperPlayerController();

	/** 表示するウィジェット（未指定ならC++の UWG_RadNodzToolkit）。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaper")
	TSubclassOf<UUserWidget> ControllerWidgetClass;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> ControllerWidget;
};
