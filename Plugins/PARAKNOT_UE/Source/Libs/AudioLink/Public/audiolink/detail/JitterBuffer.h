// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include <atomic>
#include <cstdint>
#include <vector>

#include "../Concealer.h"

//==============================================================================
/**
    受信側ジッターバッファ(単一生産者=ネットスレッド / 単一消費者=audioスレッド)。

    - プレーナ float のリングバッファ。書込位置は単調増加カウンタ。
    - 目標レイテンシ(target frames)まで貯めてから再生開始(プライミング)。
    - **ドリフト補正: 可変レートのリサンプリング**。滞留量(fill)と目標の差から
      読み出しレート(ratio)を ±0.5% 以内で滑らかに調整し、送受信のクロック差を
      追従する(線形補間)。急激な逸脱時のみ安全側のハード再同期を行う。
    - **送信側と SR が違うときは、公称レートの比を読み出し速度の基準にする**
      (SetSourceSampleRate)。ドリフト補正はその上の微調整。
    - 供給が尽きたら補間音を出す(アンダーラン)。直前の 1 周期を繰り返して 30ms で
      絞り、明けたら 5ms のクロスフェードで本物へ戻す(Concealer)。ハード再同期の
      飛び先へも同じくクロスフェードでつなぐ。

    **フレームワーク非依存。** 実体は 1 本の std::vector<float> で、
    ch ごとの領域を capacity 単位で並べてある。
*/
namespace audiolink
{

class JitterBuffer
{
public:
    JitterBuffer() = default;

    JitterBuffer (const JitterBuffer&) = delete;
    JitterBuffer& operator= (const JitterBuffer&) = delete;

    void Prepare (int numChannels, int capacityFrames, int targetFrames, double hostSampleRate);
    void Reset();

    /** 生産者(ネットスレッド): 送信側の公称サンプルレートを伝える。

        **ドリフト補正だけでは吸収できない。** 補正幅は ±0.5% しかないので、
        48k のホストへ 16k や 44.1k を流し込まれると常に追いつかず、再同期を
        繰り返して音にならない。公称レートの比を読み出し速度の基準に置き、
        ドリフト補正はその上の微調整として効かせる。

        端末(スマートフォン等)のマイクは 16k / 44.1k で上がってくることが
        普通なので、押し込み受信では必ずこの経路を通る。 */
    void SetSourceSampleRate (double sourceSampleRate);

    /** 目標滞留(自分の SR でのフレーム数)を変える。**走行中に呼んでよい。**

        容量の 1/4 までに丸める(ハード再同期が目標の 4 倍で掛かるため)。

        - **増やしたとき**は溜め直さない。途切れた間は読み出し位置が止まるので、
          その分だけ滞留が増えて新しい目標へ寄っていく。
        - **減らしたとき**は次の読み出しで滞留が目標の 4 倍を超えていれば
          ハード再同期で目標まで捨てる(そうでなければドリフト補正で縮む)。 */
    void SetTargetFrames (int targetFrames);

    /** 生産者(ネットスレッド): プレーナ float を書き込む。 */
    void Write (const float* const* src, int numChannels, int numFrames);

    /** 消費者(audioスレッド): numFrames を dst へ(可変レート補間)。不足分は無音。 */
    void Read (float* const* dst, int numChannels, int numFrames);

    int  Fill() const   { return (int) (mWriteCtr.load() - mReadFloor.load()); }
    int  Target() const { return mTarget.load(); }
    bool IsPrimed() const { return mPrimed.load(); }
    int  UnderrunCount() const { return mUnderruns.load(); }
    int  ResyncCount() const   { return mResyncs.load(); }

private:
    float*       ChannelData (int channel)       { return mRing.data() + (std::size_t) channel * (std::size_t) mCapacity; }
    const float* ChannelData (int channel) const { return mRing.data() + (std::size_t) channel * (std::size_t) mCapacity; }

    std::vector<float> mRing;
    int mCapacity = 0;
    int mChannels = 0;
    std::atomic<int> mTarget { 0 };   // 目標滞留(自分の SR でのフレーム数)。走行中に変わる

    // 自分(ホスト)の SR。送信側 SR との比を出すために持つ。
    double mHostRate = 48000.0;

    // 読み出し速度の基準 = 送信側 SR / 自分の SR。
    // 生産者が書き、消費者が読む。1.0 = 同じレート。
    std::atomic<double> mBaseRatio { 1.0 };

    std::atomic<std::int64_t> mWriteCtr  { 0 };   // 生産者が書いた総フレーム数
    std::atomic<std::int64_t> mReadFloor { 0 };   // 消費者の読み出し位置(整数, 生産者へ公開)

    double mReadPos       = 0.0;   // 消費者のみ: 小数の読み出し位置
    double mRatioSmoothed = 1.0;   // 消費者のみ: 平滑化した読み出しレート

    // 途切れの補間(消費者のみ)。mFrame は 1 フレーム分の作業領域。
    Concealer          mPlc;
    std::vector<float> mFrame;

    std::atomic<bool> mPrimed    { false };
    std::atomic<int>  mUnderruns { 0 };
    std::atomic<int>  mResyncs   { 0 };
};

} // namespace audiolink
