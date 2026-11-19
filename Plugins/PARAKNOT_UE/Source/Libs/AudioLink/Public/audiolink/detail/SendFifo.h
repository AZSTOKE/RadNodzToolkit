// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <vector>

//==============================================================================
/**
    送信音声の受け渡し用 FIFO(単一生産者=audioスレッド / 単一消費者=ネットスレッド)。

    1 フレーム = 固定 stride 個の float をインターリーブで持つ。stride を
    固定にしてあるのは、ch 数が変わっても読み書きの位置計算が変わらない
    ようにするため。

    **audio スレッドから呼ぶ側はロックも確保もしない。** 容量は Prepare() で
    先に取る。空きが足りなければ入るぶんだけ書いて残りは捨てる(遅延を増やさない)。
*/
namespace audiolink
{

class SendFifo
{
public:
    SendFifo() = default;

    SendFifo (const SendFifo&) = delete;
    SendFifo& operator= (const SendFifo&) = delete;

    /** capacityFrames フレームぶん確保する。stride = 1 フレームの float 数。 */
    void Prepare (int capacityFrames, int stride)
    {
        mCapacity = std::max (1, capacityFrames);
        mStride   = std::max (1, stride);

        mData.assign ((std::size_t) mCapacity * (std::size_t) mStride, 0.0f);

        Reset();
    }

    void Release()
    {
        mData.clear();
        mData.shrink_to_fit();
        mCapacity = 0;
        Reset();
    }

    void Reset()
    {
        mWriteCtr.store (0);
        mReadCtr.store (0);
    }

    bool IsPrepared() const { return mCapacity > 0 && ! mData.empty(); }

    int NumReady() const
    {
        return (int) (mWriteCtr.load (std::memory_order_acquire)
                    - mReadCtr.load (std::memory_order_relaxed));
    }

    int FreeSpace() const
    {
        return mCapacity - (int) (mWriteCtr.load (std::memory_order_relaxed)
                                - mReadCtr.load (std::memory_order_acquire));
    }

    /** 生産者: numChannels 本のプレーナ入力を numFrames ぶん積む。
        stride を超えるぶんは捨て、足りないぶんは 0 で埋める。
        @returns 実際に積めたフレーム数 */
    int Write (const float* const* channels, int numChannels, int numFrames)
    {
        if (! IsPrepared() || numFrames <= 0)
            return 0;

        const int n   = std::min (numFrames, FreeSpace());
        const int nch = std::clamp (numChannels, 0, mStride);

        const std::int64_t w = mWriteCtr.load (std::memory_order_relaxed);

        for (int f = 0; f < n; ++f)
        {
            float* base = Frame ((int) ((w + f) % mCapacity));

            for (int c = 0; c < nch; ++c)
                base[c] = channels[c][f];

            for (int c = nch; c < mStride; ++c)
                base[c] = 0.0f;
        }

        mWriteCtr.store (w + n, std::memory_order_release);

        return n;
    }

    /** 消費者: index 番目(0 origin)の未読フレームを指す。
        **NumReady() の範囲内でのみ呼ぶこと。** */
    const float* PeekFrame (int index) const
    {
        const std::int64_t r = mReadCtr.load (std::memory_order_relaxed);

        return Frame ((int) ((r + index) % mCapacity));
    }

    /** 消費者: numFrames ぶん読んだことにする。 */
    void FinishRead (int numFrames)
    {
        const std::int64_t r = mReadCtr.load (std::memory_order_relaxed);

        mReadCtr.store (r + std::min (numFrames, NumReady()), std::memory_order_release);
    }

private:
    float*       Frame (int slot)       { return mData.data() + (std::size_t) slot * (std::size_t) mStride; }
    const float* Frame (int slot) const { return mData.data() + (std::size_t) slot * (std::size_t) mStride; }

    std::vector<float> mData;
    int mCapacity = 0;
    int mStride   = 1;

    std::atomic<std::int64_t> mWriteCtr { 0 };
    std::atomic<std::int64_t> mReadCtr  { 0 };
};

} // namespace audiolink
