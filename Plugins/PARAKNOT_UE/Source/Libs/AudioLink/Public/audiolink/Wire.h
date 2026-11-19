// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include <cstdint>

//==============================================================================
/**
    AudioLink の UDP プロトコルの**ワイヤ形式** (v2)。

    **フレームワーク非依存**(<cstdint> のみ)。PARAKNOT と、AudioLink を使う
    アプリ(UE プラグインなど)は、この同じ定義で通信する。

    **待受側 ⇄ 接続側** の構図で、音声の向きは 2 通りある。

    Server/Pull 型(待受側が送信 = 配信):
      - 送信側(スタジオ)が固定ポートで待受
      - 受信側(拠点)がハンドシェイク(用途 = subscribe)を経て購読開始
      - 送信側が Audio パケットを購読者へ連続送出

    Server/Push 型(待受側が受信):
      - 受信側が固定ポートで待受
      - 送信側がハンドシェイク(用途 = publish)を経て押し込み開始
      - 送信側が Audio パケットを待受側へ連続送出

    **どちらの型でも繋ぎに行くのは接続側**で、Audio を出すのは送信側。
    「誰が繋ぐか」と「音がどちら向きか」を分けてあるので、NAT の内側にいる
    端末(RadNodzToolkit など)からスタジオへ音声を送ることができる。

    ---------------------------------------------------------------------------
    接続確立時のハンドシェイク (v2)
    ---------------------------------------------------------------------------
    鍵を持たない相手の登録、送信元の詐称による反射・増幅、制御パケットの
    偽造、リプレイを、**接続確立時のハンドシェイク 1 回**でまとめて防ぐ。

      接続側 C                                       待受側 L
        │ hello     {用途, saltC, 詰め物, MAC}  ──▶   K で MAC を検証
        │                                             (鍵を知らない相手はここで落ちる)
        │ ◀── challenge {saltL, nonceL, MAC}          保留表に置く (登録はまだ)
        │ confirm   {nonceL, MAC}               ──▶   nonceL が返ってきた
        │                                             = その IP:port で実際に受け取れる
        │                                             ここで初めて相手として登録する
        │ ◀── keepAlive (以降はセッション鍵で暗号化)

      - MAC は RIGDOCKS が配る鍵 K から導いたハンドシェイク鍵による
        BLAKE2b(16byte)。鍵を持たない相手は hello を作れない
      - **登録の前に「その IP:port で受け取れる」ことを確かめる**(nonceL の
        往復)ので、詐称した宛先へは何も流れない
      - challenge(56byte) は hello(64byte) より小さい。**増幅率は 1 未満**
        (hello に詰め物を入れてあるのはこのため)
      - 以降のパケットは**向きごとのセッション鍵**で暗号化・認証する:
            K_c2l = BLAKE2b(K, "audiolink v2 c2l" ‖ saltC ‖ saltL)
            K_l2c = BLAKE2b(K, "audiolink v2 l2c" ‖ saltC ‖ saltL)
        鍵はハンドシェイクごとに新しくなるので、K を使い回しても nonce は
        衝突しない。nonce は 64bit の連番(一周は実質起きない)
      - 受信側は向きごとに 64 個分のスライディングウィンドウで、受信済みの
        番号と古すぎる番号を捨てる(リプレイ拒否)。過去のセッションの
        パケットは鍵が違うので復号の段階で落ちる

    **この定義を変えたら通信相手との互換が切れる。** 変えるときは
    kProtocolVersion を上げ、配布済みのライブラリを使っている側も入れ替える。
    **v1 と v2 は相互に通信できない**(版の違うパケットは読まずに捨てる)。
*/
namespace audiolink
{

inline constexpr std::uint16_t kMagic           = 0x5253; // 'R','S'
inline constexpr std::uint8_t  kProtocolVersion = 2;

// 待受ポートの既定値。
inline constexpr int kDefaultPort = 8420;

// 1 パケットの音声ペイロード上限(MTU 断片化回避のため控えめに設定)。
inline constexpr int kMaxPayloadBytes = 1024;

// 上限。**超えた相手にはハンドシェイクの応答を返さない。**
inline constexpr int kMaxSubscribers        = 16;    // 配信の購読者数
inline constexpr int kMaxPendingHandshakes  = 32;    // 待受側の保留表
inline constexpr int kPendingHandshakeTtlMs = 2000;  // 保留表の失効

enum class PacketType : std::uint8_t
{
    // --- ハンドシェイク (ハンドシェイク鍵の MAC で認証。暗号化はしない) ---
    hello       = 1,   // 接続 → 待受: 用途と saltC
    challenge   = 2,   // 待受 → 接続: saltL と nonceL
    confirm     = 3,   // 接続 → 待受: nonceL を返す

    // --- セッション (向きごとのセッション鍵で暗号化・認証) -----------------
    audio       = 4,   // 送信 → 受信: 音声データ
    channelInfo = 5,   // 送信 → 受信: チャンネル構成(メイン/サブ数・名前)
    keepAlive   = 6,   // 双方向: 生存確認
    bye         = 7,   // 双方向: 終了
};

// hello で申告する用途。**待ち受けている向きと逆の用途は受け付けない。**
enum class Intent : std::uint8_t
{
    subscribe = 1,   // 配信してほしい (待受側が送信)
    publish   = 2,   // 押し込みたい   (待受側が受信)
};

// チャンネル構成(channelInfo)の平文の形式(暗号化して送る):
//   [u8 mainCount][u8 subCount][u8 numNames] + ([u8 len][名前 UTF8 len個]) × numNames

// フラグ。
//
// **セッションのパケット(audio / channelInfo / keepAlive / bye)では常に立つ。**
// 受信側は立っていないものを中身を見ずに捨てる。フラグはヘッダごと AD として
// 認証されるので書き換えても通らないが、復号の前に足切りしておく。
inline constexpr std::uint8_t kFlagEncrypted = 0x01;

// 音声フォーマット。
inline constexpr std::uint8_t kFormatInt16 = 0;

inline constexpr int kSaltBytes  = 16;
inline constexpr int kNonceLBytes = 16;
inline constexpr int kMacBytes   = 16;

#pragma pack(push, 1)

// 全パケット共通の先頭ヘッダ(8 bytes)。
struct PacketHeader
{
    std::uint16_t magic;
    std::uint8_t  version;
    std::uint8_t  type;       // PacketType
    std::uint8_t  flags;      // kFlag*

    // 予備。**使う前に後方互換になるか確かめること。**
    std::uint8_t  reserved0;
    std::uint16_t reserved1;
};

// hello (64 bytes)。MAC は先頭 48byte (mac の手前まで) に掛かる。
//
// **詰め物は増幅を防ぐためのもの。** challenge(56byte) より小さい hello を
// 許すと、詐称した宛先へ受け取ったより大きいものを返すことになる。
struct HelloPacket
{
    PacketHeader header;
    std::uint8_t intent;                // Intent
    std::uint8_t reserved[7];
    std::uint8_t saltC[kSaltBytes];
    std::uint8_t padding[16];
    std::uint8_t mac[kMacBytes];
};

// challenge (56 bytes)。MAC は先頭 40byte ‖ saltC に掛かる (hello に縛る)。
struct ChallengePacket
{
    PacketHeader header;
    std::uint8_t saltL[kSaltBytes];
    std::uint8_t nonceL[kNonceLBytes];
    std::uint8_t mac[kMacBytes];
};

// confirm (40 bytes)。MAC は先頭 24byte ‖ saltC ‖ saltL に掛かる。
struct ConfirmPacket
{
    PacketHeader header;
    std::uint8_t nonceL[kNonceLBytes];
    std::uint8_t mac[kMacBytes];
};

// セッションの制御パケット(channelInfo / keepAlive / bye)の先頭(16 bytes)。
//   PacketHeader(8) + sequence(8) + MAC(16) + 暗号文
// AD は先頭 16byte。
struct SessionHeader
{
    PacketHeader  header;
    std::uint64_t sequence;   // 向きごとの連番 (nonce を兼ねる)
};

// Audio パケットの追加ヘッダ(PacketHeader の直後)。
//   PacketHeader(8) + AudioHeader(28) + MAC(16) + 暗号文(PCM)
// AD は先頭 36byte。
struct AudioHeader
{
    std::uint64_t sequence;          // 向きごとの連番 (nonce を兼ねる。制御パケットと共通)
    std::uint64_t timestampSamples;  // 送信側サンプルクロック
    std::uint32_t sampleRate;        // 送信側サンプルレート
    std::uint16_t numChannels;       // チャンネル数(合計)
    std::uint16_t numFrames;         // このパケットの 1ch あたりサンプル数
    std::uint8_t  format;            // kFormat*
    std::uint8_t  mainCount;         // メインch数(受信側の表示構成用)
    std::uint8_t  subCount;          // サブch数
    std::uint8_t  pad;
};

#pragma pack(pop)

static_assert (sizeof (PacketHeader)    == 8,  "PacketHeader must be 8 bytes");
static_assert (sizeof (HelloPacket)     == 64, "HelloPacket must be 64 bytes");
static_assert (sizeof (ChallengePacket) == 56, "ChallengePacket must be 56 bytes");
static_assert (sizeof (ConfirmPacket)   == 40, "ConfirmPacket must be 40 bytes");
static_assert (sizeof (SessionHeader)   == 16, "SessionHeader must be 16 bytes");
static_assert (sizeof (AudioHeader)     == 28, "AudioHeader must be 28 bytes");

// **増幅しないこと**をコンパイル時に縛る。
static_assert (sizeof (ChallengePacket) <= sizeof (HelloPacket),
               "challenge must not be larger than hello (reflection amplification)");

inline constexpr int kAudioHeaderEnd   = 36;   // PacketHeader + AudioHeader
inline constexpr int kSessionHeaderEnd = 16;   // SessionHeader

inline PacketHeader makeHeader (PacketType type, std::uint8_t flags = 0)
{
    PacketHeader h {};

    h.magic     = kMagic;
    h.version   = kProtocolVersion;
    h.type      = (std::uint8_t) type;
    h.flags     = flags;
    h.reserved0 = 0;
    h.reserved1 = 0;

    return h;
}

inline bool isValidHeader (const PacketHeader& h)
{
    return h.magic == kMagic && h.version == kProtocolVersion;
}

} // namespace audiolink
