// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HAL/ThreadSafeBool.h"
#include "Common/UdpSocketReceiver.h"   // FArrayReaderPtr / FUdpSocketReceiver / FIPv4Endpoint
#include "ReaperMonitorOutputMode.h"
#include "MonitorOutputProcessor.h"
#include "ReaStreamReceiver.generated.h"

class USoundWaveProcedural;
class UAudioComponent;
class FSocket;

/**
 * Reaper の ReaStream（VST/JSFX）から UDP で送られてくる音声を受信して再生する。
 *
 * **ReaStream 専用。** PARAKNOT のモニターは UParaknotReceiver が受け持つ。
 * 出力方法（ステレオ / 立体音響 / そのまま）の変換は両者共通で、
 * FMonitorOutputProcessor が行う。
 *
 * パケット形式（リトルエンディアン・音声）:
 *   0  : char[4]  'M','R','S','R'
 *   4  : int32    packetsize
 *   8  : char[32] identifier
 *   40 : uint8    channel count
 *   41 : int32    sample rate
 *   45 : int16    block length
 *   47-: float[]  サンプル（プレーナ＝非インターリーブ）
 *
 * Reaper側: マスタートラックに ReaStream を挿し、Send モードで
 *           この端末のIP・ポート(58710) を指定する。
 */
UCLASS()
class RADNODZTOOLKIT_API UReaStreamReceiver : public UObject
{
	GENERATED_BODY()

public:

	/** 受信を開始する。WorldContextObject は再生用ワールドの取得に使う。 */
	bool Start(UObject* WorldContextObject, int32 InPort = 58710);

	/** 受信を停止してリソースを解放する。 */
	void Stop();

	/** 受信中かどうか。 */
	bool IsActive() const { return bActive; }

	/**
	 * 最後に有効な音声パケットを受信した時刻（FPlatformTime::Seconds基準）。
	 * 0=未受信。受信中インジケータ（点滅）判定に使う。受信スレッドから書くためアトミック。
	 */
	double GetLastAudioRecvTime() const { return LastAudioRecvTime.Load(); }

	/**
	 * 出力設定（方法・立体音響・HRTF・ダウンミックス係数）をまとめて設定する。
	 * **開始前に呼ぶこと。** 再生中に変える場合は再起動が必要。
	 */
	void SetOutputSettings(const FMonitorOutputSettings& InSettings) { OutputSettings = InSettings; }

	/**
	 * 出力方法（ステレオ / 立体音響 / そのまま）を設定する（既定: Stereo）。
	 * 開始前に呼ぶこと。再生中に変える場合は再起動が必要。
	 */
	void SetOutputMode(EMonitorOutputMode InMode) { OutputSettings.Mode = InMode; }

	/** 現在の出力方法を返す。 */
	EMonitorOutputMode GetOutputMode() const { return OutputSettings.Mode; }

	/**
	 * 立体音響(バイノーラル)のパラメータを設定する。開始前に呼ぶこと（変更時は再起動）。
	 * @param InItdMs        最大ITD(ミリ秒, 0=ITDなし)
	 * @param InShadowAlpha  遠い耳の頭部減衰LPF係数(0〜1, 1=減衰なし/小さいほどこもる)
	 * @param InOrder        ch順マッピング
	 */
	void SetBinauralParams(float InItdMs, float InShadowAlpha, EBinauralChannelOrder InOrder)
	{
		OutputSettings.BinItdMs = InItdMs;
		OutputSettings.BinShadowAlpha = InShadowAlpha;
		OutputSettings.ChannelOrder = InOrder;
	}

	/** 受信バッファ秒数（遅延上限）。大きいほど音飛びに強いが遅延が増える。 */
	void SetBufferSeconds(float InSeconds) { BufferSeconds = FMath::Clamp(InSeconds, 0.1f, 5.0f); }

	/**
	 * 再生音量（線形ゲイン倍率）を設定する。端末側でReaperからの受信音を増減するのに使う。
	 * 再生中でも即時反映される（AudioComponentのVolumeMultiplierへ適用）。
	 * @param InGain 線形倍率（1.0=素通し）。0〜8にクランプ（±約18dB相当）。
	 */
	void SetPlaybackGain(float InGain);

	/**
	 * 許可する送信元IP（固定IP）を設定する。指定IP以外からのパケットは破棄する。
	 * 空文字 or 解析失敗のときは制限なし（全許可）。受信中に変更しても即時反映される。
	 */
	void SetAllowedSenderIP(const FString& InIP)
	{
		FIPv4Address Addr;
		if (!InIP.IsEmpty() && FIPv4Address::Parse(InIP, Addr))
		{
			AllowedSenderAddr.Store(Addr.Value);   // 指定IPのみ受信
		}
		else
		{
			AllowedSenderAddr.Store(0);            // 0 = 制限なし（全許可）
		}
	}

	virtual void BeginDestroy() override;

private:

	// UDP受信スレッドから呼ばれる（ゲームスレッドではない）
	void HandleData(const FArrayReaderPtr& Data, const FIPv4Endpoint& Endpoint);

	// プレーナ float を出力設定に従って変換し、再生キューへ積む（UDP受信スレッド）。
	void ProcessPlanarAudio(const float* Plane, int32 PerChannel, int32 NumChannels);

	// 実際の再生セットアップ（ゲームスレッドで実行）
	void SetupPlayback(int32 InChannels, int32 InSampleRate);

	UPROPERTY(Transient) TObjectPtr<USoundWaveProcedural> SoundWave;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent>      AudioComponent;
	UPROPERTY(Transient) TWeakObjectPtr<UObject>          WorldContext;

	// 出力設定（ゲームスレッドで設定し、SetupPlayback で Output へ渡す）。
	FMonitorOutputSettings OutputSettings;

	// 出力の変換。**SetupPlayback（ゲームスレッド）で作り、ProcessPlanarAudio（受信
	// スレッド）で使う。** 作り直す間は bReady を落として受信側に触らせない。
	FMonitorOutputProcessor Output;

	// 変換結果の作業バッファ（受信スレッド専用）。
	TArray<float> OutputScratch;

	FSocket* Socket = nullptr;
	FUdpSocketReceiver* Receiver = nullptr;

	int32 Port = 58710;

	// ストリームのフォーマット（最初に受信した形式を採用）
	int32 StreamChannels = 0;     // 受信側のch数
	int32 StreamSampleRate = 0;
	int32 OutputChannels = 0;     // 実際に再生するch数（ステレオ/立体音響は2、そのままは受信ch数）

	// 受信バッファ秒数（遅延上限）。既定1.0秒。受信スレッドから読むためアトミック。
	TAtomic<float> BufferSeconds { 1.0f };

	// 再生音量（線形ゲイン倍率）。1.0=素通し。ゲームスレッドから設定し、再生時にAudioComponentへ適用する。
	float PlaybackGain = 1.0f;

	// 許可する送信元IPv4アドレス（uint32）。0=制限なし（全許可）。受信スレッドから読むためアトミック。
	TAtomic<uint32> AllowedSenderAddr { 0 };

	// 現在のストリームとして「ロック」している送信元（IP+ポート）。0=未ロック。
	// 同じポートへ複数の送信元（例：Reaperの別トラックのReaStream Send）が同時に送ってくると
	// パケットが混ざって音が割れるため、最初に音声を送ってきた送信元だけを受け付ける。
	TAtomic<uint32> LockedSenderAddr { 0 };
	TAtomic<int32>  LockedSenderPort { 0 };

	// 最後に有効な音声パケットを受信した時刻（秒, FPlatformTime基準）。0=未受信。
	TAtomic<double> LastAudioRecvTime { 0.0 };

	FThreadSafeBool bActive = false;
	FThreadSafeBool bReady = false;            // 再生準備完了
	FThreadSafeBool bSetupDispatched = false;  // セットアップ依頼済み
};
