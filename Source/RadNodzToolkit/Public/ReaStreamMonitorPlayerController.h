// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ReaStreamMonitorPlayerController.generated.h"

class UUserWidget;

/**
 * 起動(Play)時に ReaStream 音声受信専用UIを画面へ表示するプレイヤーコントローラー。
 * AReaStreamMonitorGameMode から使用される（Windows向けの音声受信専用Map用）。
 */
UCLASS()
class RADNODZTOOLKIT_API AReaStreamMonitorPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AReaStreamMonitorPlayerController();

	/** 表示するウィジェット（未指定ならC++の UWG_ReaStreamMonitor）。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReaStream")
	TSubclassOf<UUserWidget> MonitorWidgetClass;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> MonitorWidget;
};
