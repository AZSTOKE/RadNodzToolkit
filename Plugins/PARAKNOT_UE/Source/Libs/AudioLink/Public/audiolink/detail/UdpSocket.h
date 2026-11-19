// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include <string>

//==============================================================================
/**
    IPv4 の UDP ソケット。

    必要なぶんだけ:
      - 指定ポート (0 = 任意) へ bind
      - 到着待ち (タイムアウト付き)
      - 受信 (送信元の ip:port 付き)
      - 宛先を指定して送信

    実装は Windows = Winsock2 / それ以外 = BSD ソケット。**ヘッダには
    プラットフォームの型を出さない** (winsock2.h を引き込むと、後から
    windows.h を include した側とマクロが衝突する)。

    **1 スレッドから使う前提。** NetworkEngine のネットスレッド専有で、
    ロックは持たない。
*/
namespace audiolink
{

class UdpSocket
{
public:
    UdpSocket();
    ~UdpSocket();

    UdpSocket (const UdpSocket&) = delete;
    UdpSocket& operator= (const UdpSocket&) = delete;

    /** ポートを開く。port = 0 なら空いているポートが割り当てられる。 */
    bool Bind (int port);

    /** 実際に割り当てられたポート。未 bind なら 0。 */
    int BoundPort() const { return mBoundPort; }

    bool IsOpen() const;

    void Close();

    /** 受信できるデータがあるか。
        @returns 1 = あり / 0 = タイムアウト / -1 = エラー */
    int WaitUntilReady (int timeoutMs);

    /** 1 パケット受信する。
        @returns 受信バイト数。0 以下なら受信していない */
    int Read (void* dest, int maxBytes, std::string& senderIp, int& senderPort);

    /** 宛先へ送る。host は IPv4 のドット表記かホスト名。 */
    bool Write (const std::string& host, int port, const void* data, int numBytes);

private:
    // 実体はプラットフォーム依存 (SOCKET / int)。ヘッダに出さないため
    // 十分な幅の整数で持つ。-1 が「未オープン」。
    long long mHandle = -1;
    int       mBoundPort = 0;
};

} // namespace audiolink
