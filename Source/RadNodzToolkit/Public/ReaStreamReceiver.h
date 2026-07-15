// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HAL/ThreadSafeBool.h"
#include "Common/UdpSocketReceiver.h"   // FArrayReaderPtr / FUdpSocketReceiver / FIPv4Endpoint
#include "ReaperMonitorOutputMode.h"
#include "ReaStreamReceiver.generated.h"

class USoundWaveProcedural;
class UAudioComponent;
class FSocket;

/**
 * Reaper の ReaStream（VST/JSFX）から UDP で送られてくる音声を受信して再生する。
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

	UReaStreamReceiver();

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
	 * 出力方法（ステレオ / 立体音響 / そのまま）を設定する（既定: Stereo）。
	 * 開始前に呼ぶこと。再生中に変える場合は再起動が必要。
	 */
	void SetOutputMode(EMonitorOutputMode InMode) { OutputMode = InMode; }

	/** 現在の出力方法を返す。 */
	EMonitorOutputMode GetOutputMode() const { return OutputMode.Load(); }

	/**
	 * 立体音響(バイノーラル)のパラメータを設定する。開始前に呼ぶこと（変更時は再起動）。
	 * @param InItdMs        最大ITD(ミリ秒, 0=ITDなし)
	 * @param InShadowAlpha  遠い耳の頭部減衰LPF係数(0〜1, 1=減衰なし/小さいほどこもる)
	 * @param InOrder        ch順マッピング
	 */
	void SetBinauralParams(float InItdMs, float InShadowAlpha, EBinauralChannelOrder InOrder)
	{
		BinItdMs = InItdMs;
		BinShadowAlpha = InShadowAlpha;
		BinChannelOrder = InOrder;
	}

	/** Customマッピング時の各ch配置（方位角[度]・距離[0.2〜2.0]）を設定する。開始前に呼ぶこと。 */
	void SetCustomLayout(const TArray<float>& InAzimuths, const TArray<float>& InDistances)
	{
		BinCustomAzimuths = InAzimuths;
		BinCustomDistances = InDistances;
	}

	/**
	 * 立体音響で使うHRTFプロファイル名を設定する（開始前に呼ぶ。変更時は再起動）。
	 * 空 or 未登録名のときは内蔵の簡易バイノーラルにフォールバックする。
	 */
	void SetHrtfProfile(const FString& InName) { HrtfProfileName = InName; }

	/** 受信バッファ秒数（遅延上限）。大きいほど音飛びに強いが遅延が増える。 */
	void SetBufferSeconds(float InSeconds) { BufferSeconds = FMath::Clamp(InSeconds, 0.1f, 5.0f); }

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

	/** ダウンミックス係数の最大ch数（標準レイアウト L R C LFE Ls Rs Lb Rb）。 */
	static constexpr int32 MaxDownmixChannels = 8;

	/**
	 * ダウンミックス係数を受信chごとに設定する。
	 * 標準レイアウト（L R C LFE Ls Rs Lb Rb）の各chをL/Rへ混ぜる量を、先頭から最大8ch分指定する。
	 */
	void SetDownmixCoeffs(const TArray<float>& InGains)
	{
		for (int32 i = 0; i < MaxDownmixChannels; ++i)
		{
			if (InGains.IsValidIndex(i))
			{
				DmGains[i].Store(FMath::Clamp(InGains[i], 0.0f, 1.5f));
			}
		}
	}

	virtual void BeginDestroy() override;

private:

	// UDP受信スレッドから呼ばれる（ゲームスレッドではない）
	void HandleData(const FArrayReaderPtr& Data, const FIPv4Endpoint& Endpoint);

	// 実際の再生セットアップ（ゲームスレッドで実行）
	void SetupPlayback(int32 InChannels, int32 InSampleRate);

	// 立体音響(アプリ内バイノーラル)用：ステレオ音源を用意し係数を計算する（ゲームスレッド）
	void SetupSpatialPlayback(int32 InChannels, int32 InSampleRate);

	// N(プレーナ)chサンプルをステレオ(インターリーブ)へダウンミックスする
	void DownmixToStereoInterleaved(const float* Plane, int32 PerChannel, int32 InChannels,
		TArray<int16>& OutInterleaved) const;

	// chごとのバイノーラル係数（ILD/ITD/頭部減衰）を計算し、遅延・フィルタ状態を確保する
	void BuildBinauralCoeffs(int32 InChannels, int32 InSampleRate);

	// HRTFプロファイルが選択されていれば、chごとの最近傍HRIRと畳み込みバッファを用意する
	// （成功時 bHrtfActive=true。未選択/未登録なら false で簡易バイノーラルにフォールバック）
	void BuildHrtfCoeffs(int32 InChannels, int32 InSampleRate);

	// N(プレーナ)chサンプルをHRTF畳み込みしてステレオ(インターリーブ)へ合成する
	void HrtfMixToStereoInterleaved(const float* Plane, int32 PerChannel, int32 InChannels,
		TArray<int16>& OutInterleaved);

	// N(プレーナ)chサンプルをバイノーラル処理してステレオ(インターリーブ)へ合成する
	void BinauralMixToStereoInterleaved(const float* Plane, int32 PerChannel, int32 InChannels,
		TArray<int16>& OutInterleaved);

	// 受信ch数・ch順マッピングごとの各chの方位角（度・前方0°/右が正）を返す
	static void GetChannelAzimuths(int32 InChannels, EBinauralChannelOrder Order, TArray<float>& OutAzimuthsDeg);

	UPROPERTY(Transient) TObjectPtr<USoundWaveProcedural> SoundWave;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent>      AudioComponent;
	UPROPERTY(Transient) TWeakObjectPtr<UObject>          WorldContext;

	// 立体音響(バイノーラル)用の係数・状態（chごと）
	TArray<bool>          SpNearIsLeft;    // 近い耳が左か
	TArray<float>         SpNearGain;      // 近い耳のゲイン
	TArray<float>         SpFarGain;       // 遠い耳のゲイン
	TArray<int32>         SpDelaySamples;  // 遠い耳の遅延(サンプル, ITD)
	TArray<float>         SpLpfAlpha;      // 遠い耳の頭部減衰LPF係数(0〜1)
	TArray<TArray<float>> SpDelayBuf;      // 遠い耳の遅延リングバッファ
	TArray<int32>         SpDelayWrite;    // リングバッファ書込み位置
	TArray<float>         SpLpfPrev;       // LPFの直前出力

	// 立体音響パラメータ（開始前に設定。既定=中）
	float                 BinItdMs = 0.65f;                                  // 最大ITD(ms)
	float                 BinShadowAlpha = 0.35f;                            // 頭部減衰LPF(|pan|=1時)
	EBinauralChannelOrder BinChannelOrder = EBinauralChannelOrder::ReaperITU;
	TArray<float>         BinCustomAzimuths;   // Custom時の各ch方位角(度)
	TArray<float>         BinCustomDistances;  // Custom時の各ch距離(0.2〜2.0)

	// chごとの距離ゲイン（距離→音量）
	TArray<float>         SpDistGain;

	// 前後キュー：後方chの音色を暗くする（両耳共通の一次LPF）
	TArray<float>         SpToneAlpha;  // 1=素通り / 小さいほど暗い（後方ほど小）
	TArray<float>         SpToneState;  // LPFの直前出力

	// --- HRTF（実測HRIR畳み込み）用 ---
	FString               HrtfProfileName;       // 選択中プロファイル名（空=簡易にフォールバック）
	bool                  bHrtfActive = false;   // HRTF畳み込みが有効か（setupで決定）
	int32                 HrtfTaps = 0;          // FIRタップ数
	TArray<const float*>  HrtfL;                 // chごとの左耳HRIR先頭ポインタ
	TArray<const float*>  HrtfR;                 // chごとの右耳HRIR先頭ポインタ
	TArray<TArray<float>> HrtfInBuf;             // chごとの入力リングバッファ（長さ=Taps）
	TArray<int32>         HrtfInWrite;           // リングバッファ書込み位置

	FSocket* Socket = nullptr;
	FUdpSocketReceiver* Receiver = nullptr;

	int32 Port = 58710;

	// ストリームのフォーマット（最初に受信した形式を採用）
	int32 StreamChannels = 0;     // 受信側のch数
	int32 StreamSampleRate = 0;
	int32 OutputChannels = 0;     // 実際に再生するch数（ダウンミックス時は2 / 立体音響時は受信ch数）

	// 出力方法（開始前に設定。変更時は再起動）。受信スレッドからも読むためアトミックに保持。
	TAtomic<EMonitorOutputMode> OutputMode { EMonitorOutputMode::Stereo };

	// 受信バッファ秒数（遅延上限）。既定1.0秒。受信スレッドから読むためアトミック。
	TAtomic<float> BufferSeconds { 1.0f };

	// 許可する送信元IPv4アドレス（uint32）。0=制限なし（全許可）。受信スレッドから読むためアトミック。
	TAtomic<uint32> AllowedSenderAddr { 0 };

	// 現在のストリームとして「ロック」している送信元（IP+ポート）。0=未ロック。
	// 同じポートへ複数の送信元（例：Reaperの別トラックのReaStream Send）が同時に送ってくると
	// パケットが混ざって音が割れるため、最初に音声を送ってきた送信元だけを受け付ける。
	TAtomic<uint32> LockedSenderAddr { 0 };
	TAtomic<int32>  LockedSenderPort { 0 };

	// 最後に有効な音声パケットを受信した時刻（秒, FPlatformTime基準）。0=未受信。
	TAtomic<double> LastAudioRecvTime { 0.0 };

	// ダウンミックス係数（受信ch・標準レイアウト L R C LFE Ls Rs Lb Rb ごと）。
	// 既定: フロントL/R=1.0、C/サラウンド/リア=-3dB(0.7071)、LFE=0（除外）。
	// 受信スレッドからも読むためアトミックに保持。既定値はコンストラクタで設定する。
	TAtomic<float> DmGains[MaxDownmixChannels];

	FThreadSafeBool bActive = false;
	FThreadSafeBool bReady = false;            // 再生準備完了
	FThreadSafeBool bSetupDispatched = false;  // セットアップ依頼済み
};
