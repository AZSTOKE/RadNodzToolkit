// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include <cstdint>

//==============================================================================
/**
    リプレイ拒否のスライディングウィンドウ (1 方向ぶん)。

    受け取った最大の番号と、そこから 64 個前までの受信済みビットを持つ。
      - 最大より大きい番号      … 受け取ってよい
      - 64 個以上古い番号       … 捨てる (古すぎて確かめようがない)
      - 窓の中で受信済みの番号  … 捨てる (重複 = リプレイ)

    **検査と記録を分ける。** 番号を見ただけで記録すると、偽の(認証に通らない)
    パケットで窓を進められてしまい、正規のパケットが「古すぎる」で落ちる。
    復号(認証)に成功したものだけ Accept() する。

    UDP は順序が入れ替わることがあるので、少し古いだけの番号は受け入れる。
    **フレームワーク非依存。**
*/
namespace audiolink
{

class ReplayWindow
{
public:
    static constexpr std::uint64_t kWindow = 64;

    void Reset()
    {
        mStarted = false;
        mHighest = 0;
        mBits    = 0;
    }

    /** 受け取ってよい候補か。**状態は変えない。** */
    bool Check (std::uint64_t sequence) const
    {
        if (! mStarted || sequence > mHighest)
            return true;

        const std::uint64_t age = mHighest - sequence;

        if (age >= kWindow)
            return false;   // 古すぎる

        return (mBits & (std::uint64_t (1) << age)) == 0;   // 未受信なら可
    }

    /** 認証に通ったパケットの番号を記録する。 */
    void Accept (std::uint64_t sequence)
    {
        if (! mStarted)
        {
            mStarted = true;
            mHighest = sequence;
            mBits    = 1;   // bit0 = mHighest

            return;
        }

        if (sequence > mHighest)
        {
            const std::uint64_t shift = sequence - mHighest;

            mBits = (shift >= kWindow) ? 0 : (mBits << shift);
            mBits |= 1;

            mHighest = sequence;

            return;
        }

        const std::uint64_t age = mHighest - sequence;

        if (age < kWindow)
            mBits |= (std::uint64_t (1) << age);
    }

private:
    bool          mStarted = false;
    std::uint64_t mHighest = 0;
    std::uint64_t mBits    = 0;
};

} // namespace audiolink
