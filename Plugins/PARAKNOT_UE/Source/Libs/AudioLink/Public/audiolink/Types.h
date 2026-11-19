// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include <cstdint>

//==============================================================================
/**
    AudioLink の基本型。**フレームワークにも RIGDOCKS にも依存しない。**

    音声の向き(Role)と接続の向き(Transport)は**独立**している。組み合わせは
    4 通りあるが、セッションの型としては 2 つ(Pull / Push)で、同じセッションを
    両端から見ているだけ:

      send    + listen   : 待受 → 購読者へ配信        ┐ Pull 型
      receive + connect  : 相手へ購読を申し込んで受信  ┘

      receive + listen   : 待受 → 押し込まれた音声を受ける ┐ Push 型
      send    + connect  : 相手の待受へ音声を押し込む       ┘

    どちらの型でも**繋ぎに行くのは接続側**で、Audio を出すのは送信側。
    「誰が繋ぐか」と「音がどちら向きか」を分けてあるので、NAT の内側にいる
    端末からでもスタジオへ音声を送れる。
*/
namespace audiolink
{

//==============================================================================
/** ライブラリの ABI 版。

    **ヘッダと .lib の取り違えを見つけるためのもの。** 利用側は起動時に
    `kAbiVersion == AbiVersion()` を確かめること。食い違っていると、症状が
    「無言で音が来ない」になって原因究明が長引く。

    ワイヤ形式(kProtocolVersion)とは別物。あちらは通信相手との互換、
    こちらは配布物どうしの整合を見る。 */
inline constexpr int kAbiVersion = 3;

/** ライブラリ側がコンパイルされた時点の ABI 版を返す。 */
int AbiVersion();

//==============================================================================
/** 扱えるチャンネル数の上限。

    **固定配列の寸法に使っている。** 利用側がこれを超える構成を要求しても、
    超過分は落とされる(エラーにはしない)。 */
inline constexpr int kMaxChannels = 16;

//==============================================================================
/** 音声の向き。**どちらが繋ぎに行くかは Transport が決める。** */
enum class Role
{
    send = 0,
    receive = 1
};

/** 接続の向き。**音声の向き(Role)とは独立。** */
enum class Transport
{
    listen = 0,    // 固定ポートで待ち受ける(相手から繋いでもらう)
    connect = 1    // 相手の待受ポートへ自分から繋ぎに行く
};

//==============================================================================
/** 接続の状態。

    **文言は持たない。** 表示に使う文字列は利用側で作る(言語や言い回しは
    アプリの都合であって、ライブラリが決めることではない)。 */
enum class ConnectionState
{
    disconnected = 0,
    listening,     // 待受側: 待受中
    connecting,    // 接続側: 接続試行中
    connected,     // 送受信中
    error
};

/** エラーの内訳。state が error のときだけ意味を持つ。 */
enum class ErrorCode
{
    none = 0,
    bindFailed,    // ポートのバインド失敗(待受側の待受不可)
    socketError,
    timeout
};

} // namespace audiolink
