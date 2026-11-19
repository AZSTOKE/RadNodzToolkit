// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "ReaperMonitorOutputMode.h"

/**
 * モニター出力の設定（ステレオ / 立体音響 / そのまま と、その調整値）。
 *
 * **ReaStream / PARAKNOT のどちらのモニターにも同じものを渡す。** 音をどこから
 * 貰うかは受信機ごとに違うが、受け取った後の変換はこの設定だけで決まる。
 */
struct RADNODZTOOLKIT_API FMonitorOutputSettings
{
	EMonitorOutputMode Mode = EMonitorOutputMode::Stereo;

	// 立体音響(バイノーラル)。既定 = 中。
	float                 BinItdMs = 0.65f;     // 最大ITD(ms, 0=ITDなし)
	float                 BinShadowAlpha = 0.35f; // 遠い耳の頭部減衰LPF(|pan|=1時, 1=減衰なし)
	EBinauralChannelOrder ChannelOrder = EBinauralChannelOrder::ReaperITU;
	TArray<float>         CustomAzimuths;       // Custom時の各ch方位角(度)
	TArray<float>         CustomDistances;      // Custom時の各ch距離(0.2〜2.0)

	// 立体音響で使うHRTFプロファイル名。空 or 未登録なら簡易バイノーラル。
	FString HrtfProfileName;

	// ステレオダウンミックス係数（標準レイアウト L R C LFE Ls Rs Lb Rb ごと）。
	// 足りない分は既定値（フロントL/R=1.0、C/サラウンド/リア=-3dB、LFE=0）。
	TArray<float> DownmixGains;
};

/**
 * 受信したプレーナ float を、モニター出力の設定に従ってインターリーブ float へ変換する。
 *
 * **UE のオーディオミキサーへ渡す前に済ませる。** UE のミキサーは 1 音源 8ch
 * （7.1）までしか扱えず、それを超える音源はゲイン 0 のチャンネルマップで
 * 鳴らされて無音になる（AudioMixerChannelMaps.cpp の Get2DChannelMap）。
 * そのため「そのまま」でも 9ch 以上はステレオへ落として渡す。
 *
 * 使う手順:
 *   1. SetSettings()            … 設定を渡す（次の Prepare から効く）
 *   2. Prepare(ch数, SR)        … 係数・状態を作る。戻り値が UE へ渡す ch 数
 *   3. Process(...)             … ブロックごとに変換する
 *
 * **スレッドの扱いは呼び出し側が決める。** Prepare と Process を同時に
 * 呼ばないこと（Process 中に係数を作り直すと壊れる）。
 */
class RADNODZTOOLKIT_API FMonitorOutputProcessor
{
public:

	/** ダウンミックス係数を持つ ch 数（標準レイアウト L R C LFE Ls Rs Lb Rb）。 */
	static constexpr int32 MaxDownmixChannels = 8;

	/** UE の再生へ渡せる ch 数の上限。 */
	static constexpr int32 MaxPlaybackChannels = 8;

	/** ダウンミックス係数の既定値。 */
	static const float DefaultDownmixGains[MaxDownmixChannels];

	/** 設定を渡す。**次の Prepare() から効く。** */
	void SetSettings(const FMonitorOutputSettings& InSettings) { Settings = InSettings; }

	const FMonitorOutputSettings& GetSettings() const { return Settings; }

	/**
	 * 受信 ch 数・サンプルレートに合わせて係数と状態を作る。
	 *
	 * @return UE の再生へ渡す ch 数（ステレオ / 立体音響 = 2、そのまま = 受信 ch 数。
	 *         ただし 9ch 以上は 2）
	 */
	int32 Prepare(int32 InChannels, int32 InSampleRate);

	/** 係数・状態を捨てる（Prepare 前の状態に戻す）。 */
	void Reset();

	/**
	 * 変換する。
	 *
	 * @param Plane          プレーナ float（ch c のサンプル s は Plane[c * Frames + s]）。
	 *                       ch 数は Prepare() に渡したもの
	 * @param Frames         1 ch あたりのサンプル数
	 * @param OutInterleaved 書き込み先（Frames × GetOutputChannels()）。-1..1 にクランプ済み
	 */
	void Process(const float* Plane, int32 Frames, float* OutInterleaved);

	bool  IsPrepared() const { return InputChannels > 0; }
	int32 GetInputChannels() const { return InputChannels; }
	int32 GetOutputChannels() const { return OutputChannels; }

	/** Prepare で実際に使うことにした出力方法（そのまま + 9ch 以上なら Stereo）。 */
	EMonitorOutputMode GetEffectiveMode() const { return EffectiveMode; }

	/** 立体音響で HRTF 畳み込みが有効になったか（false なら簡易バイノーラル）。 */
	bool IsHrtfActive() const { return bHrtfActive; }

	/** 受信ch数・ch順マッピングごとの各chの方位角（度・前方0°/右が正）を返す。 */
	static void GetChannelAzimuths(int32 InChannels, EBinauralChannelOrder Order, TArray<float>& OutAzimuthsDeg);

private:

	void BuildDownmixGains(int32 InChannels);
	void BuildBinauralCoeffs(int32 InChannels, int32 InSampleRate);
	void BuildHrtfCoeffs(int32 InChannels);

	void DownmixToStereo(const float* Plane, int32 Frames, float* Out) const;
	void BinauralMixToStereo(const float* Plane, int32 Frames, float* Out);
	void HrtfMixToStereo(const float* Plane, int32 Frames, float* Out);
	void CopyInterleaved(const float* Plane, int32 Frames, float* Out) const;

	FMonitorOutputSettings Settings;

	int32              InputChannels = 0;
	int32              OutputChannels = 0;
	EMonitorOutputMode EffectiveMode = EMonitorOutputMode::Stereo;

	// ステレオダウンミックス：受信chごとの左右ゲイン（Prepare で作る）。
	TArray<float> DmL;
	TArray<float> DmR;

	// 立体音響(バイノーラル)用の係数・状態（chごと）
	TArray<bool>          SpNearIsLeft;    // 近い耳が左か
	TArray<float>         SpNearGain;      // 近い耳のゲイン
	TArray<float>         SpFarGain;       // 遠い耳のゲイン
	TArray<int32>         SpDelaySamples;  // 遠い耳の遅延(サンプル, ITD)
	TArray<float>         SpLpfAlpha;      // 遠い耳の頭部減衰LPF係数(0〜1)
	TArray<TArray<float>> SpDelayBuf;      // 遠い耳の遅延リングバッファ
	TArray<int32>         SpDelayWrite;    // リングバッファ書込み位置
	TArray<float>         SpLpfPrev;       // LPFの直前出力
	TArray<float>         SpDistGain;      // 距離ゲイン（距離→音量）
	TArray<float>         SpToneAlpha;     // 前後キュー：1=素通り / 小さいほど暗い（後方ほど小）
	TArray<float>         SpToneState;     // 前後キューLPFの直前出力

	// HRTF（実測HRIR畳み込み）用
	bool                  bHrtfActive = false;
	int32                 HrtfTaps = 0;
	TArray<const float*>  HrtfL;           // chごとの左耳HRIR先頭ポインタ
	TArray<const float*>  HrtfR;           // chごとの右耳HRIR先頭ポインタ
	TArray<TArray<float>> HrtfInBuf;       // chごとの入力リングバッファ（長さ=Taps）
	TArray<int32>         HrtfInWrite;     // リングバッファ書込み位置
};
