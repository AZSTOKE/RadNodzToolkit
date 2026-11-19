// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include <vector>

//==============================================================================
/**
    途切れの補間 (PLC: packet loss concealment)。

    受信バッファが空になったとき、無音を挟むと波形が急に 0 へ落ちて「ブツッ」と
    鳴る。ここでは代わりに**直前の波形を 1 周期ぶん繰り返しながら絞り**、
    音が戻ったら**補間音から本物へ短くクロスフェード**してつなぐ。

    - 周期は直前の履歴の自己相関から探す (2.5〜15ms = 声の基本周期の範囲)。
      全 ch を足したモノラルで探し、全 ch に同じ周期を使う。
    - 補間音は 30ms かけて 0 まで絞る。それより長い途切れは無音になる。
    - つなぎ目 (途切れの明け・古い音を捨てて飛んだ位置) は 5ms でクロスフェードする。

    **1 フレームずつ呼ぶ。** frame は Prepare の ch 数ぶん並んだ値
    (インターリーブ 1 フレーム分)。確保は Prepare だけで行い、Real / Gap / Splice は
    確保もロックもしないので audio スレッドから呼んでよい。

    **フレームワーク非依存。** AudioLink の JitterBuffer のほか、利用側の
    受信バッファ (UE のグループ通話など) からも使う。
*/
namespace audiolink
{

class Concealer
{
public:
    void Prepare (int numChannels, double sampleRate);
    void Reset();

    /** 本物の 1 フレーム。つなぎ目ならクロスフェードを掛けて書き戻す。 */
    void Real (float* frame);

    /** 届いていない 1 フレーム。補間音を書く。 */
    void Gap (float* frame);

    /** 次の Real の前に連続性が切れた (古い音を捨てて読み出し位置が飛んだ) ことを伝える。 */
    void Splice();

private:
    void Start();
    void Push (const float* frame);
    void NextConcealed (float* out);

    int mChannels = 0;

    // 直近の出力の履歴 (インターリーブのリング)。mHistWrite が最古。
    std::vector<float> mHist;
    int mHistFrames = 0;
    int mHistWrite  = 0;

    std::vector<float> mMono;   // 周期探索の作業領域 (古い順のモノラル)
    std::vector<float> mSeg;    // 繰り返す 1 周期 (インターリーブ)
    std::vector<float> mScratch;   // クロスフェード中の補間音 1 フレーム

    int mMinLag = 0, mMaxLag = 0, mMatch = 0;
    int mFadeFrames = 0, mXfadeFrames = 0;

    int  mLag = 1, mSegPos = 0;
    int  mFadeLeft  = 0;    // 補間音を絞り切るまでの残り
    int  mXfadeLeft = 0;    // 本物へ移るクロスフェードの残り
    bool mConcealing   = false;
    bool mPendingXfade = false;
};

} // namespace audiolink
