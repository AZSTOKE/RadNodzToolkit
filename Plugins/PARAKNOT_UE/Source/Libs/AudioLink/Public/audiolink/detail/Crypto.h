// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

//==============================================================================
/**
    暗号レイヤ (プロトコル v2)。Monocypher のみを使う。

    鍵は 3 段:
      1. **K** … RIGDOCKS がノード間の mTLS 越しに配る鍵 (32byte)。
                  待受ポートに 1 本で、購読者どうしで共有される
      2. **ハンドシェイク鍵** … K から導く。hello / challenge / confirm の MAC
                  (BLAKE2b, 16byte) に使う。鍵を持たない相手は hello を作れない
      3. **セッション鍵** … K とハンドシェイクの乱数 (saltC, saltL) から
                  **向きごとに**導く。以降のパケットは XChaCha20-Poly1305 で
                  これを使って暗号化・認証する

    セッション鍵がハンドシェイクごとに新しくなるので、nonce は 64bit の
    連番だけで足りる (同じ鍵で同じ番号が 2 度使われることは無い)。

    **フレームワーク非依存。**
*/
namespace audiolink
{

inline constexpr int kKeyBytes = 32;

//==============================================================================
/** RIGDOCKS から受け取った鍵 K と、そこから導いたハンドシェイク鍵。 */
class MasterKey
{
public:
    /** Base64 の 32byte で初期化する。復号できない / 長さが違う場合は
        鍵が入らない (IsValid() が false)。 */
    explicit MasterKey (const std::string& sessionKeyBase64);
    ~MasterKey();

    MasterKey (const MasterKey&) = delete;
    MasterKey& operator= (const MasterKey&) = delete;

    bool IsValid() const { return mValid; }

    /** ハンドシェイクの MAC。a ‖ b ‖ c に掛ける (使わない部分は nullptr / 0)。 */
    void Mac (const void* a, std::size_t aLen,
              const void* b, std::size_t bLen,
              const void* c, std::size_t cLen,
              std::uint8_t out[16]) const;

    /** MAC の検証。**定数時間で比べる。** */
    bool VerifyMac (const void* a, std::size_t aLen,
                    const void* b, std::size_t bLen,
                    const void* c, std::size_t cLen,
                    const std::uint8_t mac[16]) const;

    /** 向きごとのセッション鍵を導く。
        c2l = 接続側 → 待受側、l2c = 待受側 → 接続側。 */
    void DeriveSessionKeys (const std::uint8_t saltC[16], const std::uint8_t saltL[16],
                            std::uint8_t c2l[kKeyBytes], std::uint8_t l2c[kKeyBytes]) const;

private:
    std::uint8_t mKey[kKeyBytes] {};
    std::uint8_t mHandshakeKey[kKeyBytes] {};
    bool         mValid = false;
};

//==============================================================================
/** 1 方向ぶんのセッション鍵による AEAD (XChaCha20-Poly1305)。
    nonce は 64bit の連番を前詰めしてゼロ埋めする。 */
class SessionCipher
{
public:
    static constexpr int macBytes = 16;

    SessionCipher() = default;
    ~SessionCipher();

    SessionCipher (const SessionCipher& other);
    SessionCipher& operator= (const SessionCipher& other);

    void SetKey (const std::uint8_t key[kKeyBytes]);
    bool IsValid() const { return mValid; }

    /** in-place 暗号化。data(len) を暗号化し、macOut[16] に認証タグを出力。 */
    void Encrypt (void* data, std::size_t len,
                  const void* ad, std::size_t adLen,
                  std::uint64_t sequence, std::uint8_t macOut[macBytes]) const;

    /** in-place 復号。認証成功で true、改ざん検知で false。 */
    bool Decrypt (void* data, std::size_t len,
                  const void* ad, std::size_t adLen,
                  std::uint64_t sequence, const std::uint8_t mac[macBytes]) const;

private:
    std::uint8_t mKey[kKeyBytes] {};
    bool         mValid = false;
};

//==============================================================================
/** OS の暗号論的乱数で埋める。

    **std::random_device を使う。** 対象の処理系 (MSVC / Android NDK の libc++ /
    Apple の libc++) ではいずれも OS の CSPRNG (RtlGenRandom / getrandom /
    arc4random) に繋がっている。 */
void FillRandom (void* out, std::size_t len);

/** 定数時間の 16byte 比較。 */
bool Equal16 (const std::uint8_t a[16], const std::uint8_t b[16]);

/** 鍵の痕跡を消す (最適化で消されない書き方。crypto_wipe)。 */
void SecureWipe (void* data, std::size_t len);

//==============================================================================
/** Base64 デコード。成功したら out に中身を入れる。
    改行と空白は読み飛ばす。不正な文字が混ざっていたら false。 */
bool DecodeBase64 (const std::string& text, std::string& out);

} // namespace audiolink
