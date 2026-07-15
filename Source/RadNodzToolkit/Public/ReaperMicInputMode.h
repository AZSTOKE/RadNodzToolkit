// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReaperMicInputMode.generated.h"

/**
 * マイク送信に使う入力経路。
 *
 * 注意（プラットフォーム差）:
 *   - Android: UEのオーディオキャプチャ(Oboe)は入力デバイスをOS既定で固定するため、
 *     Bluetoothマイクへ切り替えるには AudioManager の Bluetooth SCO を制御する必要がある。
 *     本enumに応じて UReaStreamSender が SCO の ON/OFF を行う。
 *   - iOS / PC: 既定では「本体マイク(OS既定)」として動作。
 *     将来 iOS で Bluetooth(HFP) を選べるようにする場合は AVAudioSession 側のルーティング実装を追加する。
 */
UENUM(BlueprintType)
enum class EMicInputMode : uint8
{
	/** 本体内蔵マイク（OS既定のルート。Bluetooth SCOは使わない）。 */
	Builtin    UMETA(DisplayName = "Built-in Mic"),

	/** Bluetooth接続マイク（AndroidではBluetooth SCOを有効化する）。 */
	Bluetooth  UMETA(DisplayName = "Bluetooth Mic"),
};
