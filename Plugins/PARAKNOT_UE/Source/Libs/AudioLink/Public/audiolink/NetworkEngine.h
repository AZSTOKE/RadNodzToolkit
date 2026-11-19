// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "Types.h"
#include "Wire.h"
#include "detail/Crypto.h"
#include "detail/JitterBuffer.h"
#include "detail/ReplayWindow.h"
#include "detail/SendFifo.h"

//==============================================================================
/**
    AudioLink の音声送受信エンジン。

    - 専用の背景スレッドで UDP ソケットを回す。
    - 双方向 KeepAlive による生存確認と自動再接続。
    - audio スレッドとは lock-free(SendFifo / JitterBuffer)で受け渡し。

    **役割(音声の向き)と接続の向きは独立**(audiolink::Role / Transport)。
    セッションの立ち上がり方は接続の向きで決まり、音声の向きは Role で決まる。

      待受(listen)  : cfg.port で bind。ハンドシェイク(用途 = subscribe:
                      相手が欲しがる / publish: 相手が押し込む)を経た相手だけを
                      登録する。
      接続(connect) : エフェメラルポートで bind。受信なら subscribe、
                      送信なら publish の用途で相手の待受ポートへハンドシェイクする。

    **鍵を持たない相手とは何も喋らない。** ハンドシェイクは鍵 K から導いた MAC で
    認証し、登録の前に相手の IP:port へ往復が届くことを確かめる。以降のパケットは
    向きごとのセッション鍵で暗号化・認証し、リプレイも拒む (詳細は Wire.h)。

    音声は常に「送信側 → 受信側」で、送信側が登録済みの相手へ Audio を出す。

    **フレームワーク非依存。** 標準ライブラリと OS のソケットだけで動く
    (std::thread / std::mutex / std::string / std::chrono / Winsock2 / BSD socket)。

    **接続先の解決はここでやらない。** どこへ繋ぐか (host / port / セッション鍵) を
    どう決めるかは利用側の都合なので、決まった結果を Config で渡してもらう。
    PARAKNOT は RIGDOCKS 越しに解決し、UE アプリは RADNODZ 経由で受け取る。

    **ランデブー(NAT 越え)は持たない。** 直接到達できる経路
    (同一 PC / 同一 LAN / 到達可能な IP) のみ。
*/
namespace audiolink
{

class NetworkEngine
{
public:
    NetworkEngine();
    ~NetworkEngine();

    NetworkEngine (const NetworkEngine&) = delete;
    NetworkEngine& operator= (const NetworkEngine&) = delete;

    struct Config
    {
        Role        role = Role::send;
        // 接続の向き。待受 = 自分の port で bind / 接続 = 相手の host:port へ繋ぐ。
        Transport   transport = Transport::listen;
        std::string host = "127.0.0.1";   // 接続側の接続先
        int         port = kDefaultPort;  // 待受 = 待受ポート / 接続 = 接続先ポート

        // セッション鍵 (Base64 の 32byte)。利用側が外から渡す。
        //
        // **音声は常に暗号化する。平文の経路は無い。** 鍵が空 / 不正な間は
        // 送りも受けもしない (送信は破棄、受信は捨てる)。
        std::string sessionKeyBase64;

        bool operator== (const Config& other) const
        {
            return role             == other.role
                && transport        == other.transport
                && host             == other.host
                && port             == other.port
                && sessionKeyBase64 == other.sessionKeyBase64;
        }

        bool operator!= (const Config& other) const { return ! (*this == other); }
    };

    /** audio 準備。FIFO / ジッターバッファを確保する。

        **走行中に呼ばないこと。** FIFO / ジッターバッファはネットスレッドと
        audio スレッドが読み書きしているので、その足元で確保し直すことになる。
        SR / ブロック長が変わったときだけ、Stop() してから呼ぶ。 */
    void Prepare (double sampleRate, int blockSize);
    void Release();

    /** 受信: ジッターバッファの目標遅延(秒)。既定 0.040。**走行中に呼んでよい。**

        揺らぎの大きい回線(画面オフ中のスマートフォンの Wi-Fi など、パケットが
        100〜300ms ごとにまとめて届く)では既定値だと途切れと再同期を繰り返すので、
        利用側が状況に応じて広げる。上限 0.25 秒(容量の 1/4)。

        Prepare より前に呼んでもよい(Prepare がその値で確保する)。
        増やしたときは溜め直さず、途切れた分だけ滞留が増えて追従する。
        減らしたときは次の読み出しで超過分を捨てる(JitterBuffer::SetTargetFrames)。 */
    void   SetReceiveLatency (double seconds);
    double ReceiveLatency() const { return mReceiveLatency.load(); }

    /** 設定変更。実行中なら安全に張り直す。

        **中身が同じなら何もしない。** 張り直すと登録済みの相手が消えるので、
        変化が無いのに呼ばれたときに繋ぎ直してはいけない (後述)。

        **セッション鍵だけが変わった場合は張り直さず、鍵だけを差し替える。**
        鍵は後から届くもの (利用側が RIGDOCKS 越しに受け取る) なので、
        そのたびにソケットを作り直すと登録済みの相手が消え、相手が気づくまで
        数秒音が止まる。接続側で**鍵が空から入った**ときだけは張り直す
        (鍵が無い間は接続を始めていないため)。 */
    void SetConfig (const Config& newConfig);
    Config GetConfig() const;

    void Start();
    void Stop();
    bool IsActive() const { return mRunning.load(); }

    //== audio スレッドから呼ぶ受け渡し ======================================
    //
    // **どちらも audio スレッドから呼ばれる。** 確保・ロック・ブロッキングは
    // しない (lock-free な FIFO / リングバッファで net スレッドへ渡す)。

    /** 送信: これから送る音声を渡す。
        @param channels     プレーナの入力 (numChannels 本)
        @param numChannels  合計チャンネル数
        @param mainCount    うちメイン ch 数 (受信側の表示構成用)
        @param subCount     うちサブ ch 数
        @param numFrames    サンプル数
        @param sampleRate   送信側のサンプルレート */
    void PushForSend (const float* const* channels, int numChannels,
                      int mainCount, int subCount,
                      int numFrames, double sampleRate);

    /** 受信: 出力バッファを受信音声で埋める。足りないぶんは無音にする。 */
    void PullForReceive (float* const* outChannels, int numOutChannels, int numFrames);

    //== 表示用チャンネル構成 ================================================
    /** 送信側: 受信側へ配る構成 (メイン/サブ数・名前)。 */
    void SetChannelLayout (int mainCount, int subCount, const std::vector<std::string>& names);

    /** 受信側: 送信側から受け取った構成。 */
    std::vector<std::string> ReceivedNames() const;

    //== 状態(GUI / 外部公開 用) ============================================
    ConnectionState State() const { return (ConnectionState) mState.load(); }
    ErrorCode       Error() const { return (ErrorCode) mErrorCode.load(); }

    /** 繋がっている相手の数。送信では配信先(購読者)、押し込み受信では
        押し込み元の数。**押し込み受信は 1 本しか受けない**ので 0 か 1。 */
    int SubscriberCount()  const { return mSubscriberCount.load(); }
    int ReceivedChannels() const { return mReceivedChannels.load(); }
    int ReceivedMainCount() const { return mReceivedMain.load(); }
    int ReceivedSubCount()  const { return mReceivedSub.load(); }

    /** 受信側: 復号に失敗して捨てたパケットの累計。

        **鍵の不一致を見分けるために使う。** 状態(ConnectionState)だけでは
        「相手が送っていない」のか「送っているが鍵が違って捨てている」のかが
        区別できない。セッションを張り直すかどうかの判断に使う。 */
    int DecryptFailureCount() const { return mDecryptFailures.load(); }

    /** 接続側: hello を送り直した回数 (= 前の hello に返事が無かった回数) の累計。

        **鍵の食い違いはここに出る。** v2 では鍵が違うとハンドシェイクの段階で
        相手に捨てられ、音声まで進まない (DecryptFailureCount は増えない)。
        相手が落ちている / まだ鍵を受け取っていない場合も増えるので、短時間の
        増加で判断しないこと (PARAKNOT は 3 秒続いたら鍵を取り直す)。 */
    int HandshakeFailureCount() const { return mHandshakeFailures.load(); }

    /** 受信側: 受信済み / 古すぎる番号として捨てたパケットの累計 (リプレイ拒否)。 */
    int ReplayRejectCount() const { return mReplayRejects.load(); }

    //== 通信イベント (ログ用) ===============================================
    /** 相手との通信の出来事。**文言は持たない**(表示の文字列と同じく、
        どう書き残すかは利用側が決める)。

        | type         | いつ                                                             |
        |---|---|
        | connected    | 相手が成立した (待受: ハンドシェイク完了 / 接続: 確立)           |
        | disconnected | 成立していた相手が外れた (endReason / durationSeconds)            |
        | rejected     | 拒否・破棄したものの集計。**1 分ごと**と、セッション終了時に 1 回 |

        拒否をパケットごとに知らせないのは、鍵を知らない相手や詐称元から大量に
        届き得るため (ログが溢れる・通知のたびにネットスレッドが止まる)。 */
    struct PeerEvent
    {
        enum class Type { connected, disconnected, rejected };

        /** 外れた理由。replaced は接続側がハンドシェイクをやり直したとき。 */
        enum class EndReason { none, bye, timeout, stopped, replaced };

        Type        type     = Type::connected;
        bool        listener = false;   // こちらが待受側か

        // connected / disconnected: 相手
        std::string ip;
        int         port = 0;

        // disconnected
        EndReason endReason       = EndReason::none;
        double    durationSeconds = 0.0;   // 成立していた時間

        // rejected: 集計の窓 (秒) と理由ごとの件数
        double windowSeconds     = 0.0;
        int    handshakeRejected = 0;   // 鍵の合わない / 用途違い / 形の崩れた hello・confirm
        int    noSlot            = 0;   // 枠 (購読者の上限 / 押し込みの 1 本 / 保留表) が埋まっていた
        int    unknownSender     = 0;   // 成立していない送り元からのパケット
        int    decryptFailed     = 0;   // 復号 (認証) に失敗
        int    replayRejected    = 0;   // リプレイとして捨てた
        int    noReply           = 0;   // 接続側: hello / confirm に応答が無かった

        // rejected: 件数の多い送り元 (IP と件数。多い順に最大 5 件)
        std::vector<std::pair<std::string, int>> topSenders;
    };

    using PeerEventCallback = std::function<void (const PeerEvent&)>;

    /** 通信イベントの通知先。nullptr で解除。

        **ネットスレッドから呼ばれる。** ログを 1 行書く程度にとどめ、ブロックする
        処理をしないこと。例外は外へ出さないこと (出た場合は握りつぶす)。 */
    void SetPeerEventCallback (PeerEventCallback callback);

    int UnderrunCount() const { return mJitter.UnderrunCount(); }
    int ResyncCount()   const { return mJitter.ResyncCount(); }
    int JitterFill()    const { return mJitter.Fill(); }

    // **表示用の文言は持たない。** 状態は上の enum で返すので、文字列は
    // 利用側が作る (言語も言い回しもアプリの都合であって、ライブラリが
    // 決めることではない)。

private:
    void Run();
    void RunSocketSession();

    /** 鍵 K からハンドシェイク鍵を作り直す。**ネットスレッドからのみ。** */
    void ApplyMasterKey();

    struct RemotePeer;

    // --- 待受側 ---------------------------------------------------------------
    void HandleIncomingAsListener (const std::uint8_t* data, int len,
                                   const std::string& senderIp, int senderPort,
                                   class UdpSocket& socket);
    void HandleHello   (const std::uint8_t* data, int len, const std::string& ip, int port,
                        class UdpSocket& socket);
    void HandleConfirm (const std::uint8_t* data, int len, const std::string& ip, int port,
                        class UdpSocket& socket);
    void PrunePending (std::uint32_t nowMs);

    // --- 接続側 ---------------------------------------------------------------
    void HandleIncomingAsConnector (const std::uint8_t* data, int len,
                                    const std::string& senderIp, int senderPort,
                                    class UdpSocket& socket);
    void HandleChallenge (const std::uint8_t* data, int len, const std::string& ip, int port,
                          class UdpSocket& socket);
    void BeginHandshake (class UdpSocket& socket, std::uint32_t nowMs);
    void SendHello (class UdpSocket& socket);
    void SendConfirm (class UdpSocket& socket);
    bool IsFromTarget (const std::string& ip, int port) const;

    // --- セッション (待受/接続のどちらでも通る) -------------------------------
    /** 登録済みの相手からのセッションパケットを検証・復号する。
        通れば plainOut に平文、成功で true。**番号の記録もここで行う。** */
    bool OpenSessionPacket (RemotePeer& peer, const std::uint8_t* data, int len,
                            int headerEnd, std::uint64_t sequence,
                            std::vector<std::uint8_t>& plainOut);
    void HandleSessionPacket (RemotePeer& peer, const std::uint8_t* data, int len,
                              class UdpSocket& socket);
    void HandleAudio (const std::uint8_t* plain, int plainLen, const AudioHeader& audio);
    void HandleChannelInfo (const std::uint8_t* plain, int plainLen);

    /** セッションの制御パケット (channelInfo / keepAlive / bye) を 1 宛先へ送る。 */
    void SendSession (class UdpSocket& socket, RemotePeer& peer, PacketType type,
                      const std::uint8_t* payload, int payloadLen);
    void SendChannelInfo (class UdpSocket& socket, RemotePeer& peer);

    void DrainAndSend (class UdpSocket& socket, std::uint64_t& timestamp);
    void PrunePeers (std::uint32_t nowMs);

    // 登録済みの相手を探す / 外す(ネットスレッドのみ)。
    RemotePeer* FindPeer (const std::string& ip, int port);
    void RemovePeer (const std::string& ip, int port);
    void UpdateSubscriberCount();

    /** 停止要求を待つ。要求が来ていれば false。 */
    bool WaitForStop (int milliseconds);

    void SetState (ConnectionState s, ErrorCode e = ErrorCode::none);

    //== 通信イベント(ネットスレッドのみ) ===================================
    enum class RejectKind { handshake, noSlot, unknownSender, decrypt, replay, noReply };

    void NotifyPeerEvent (const PeerEvent& event);

    /** 相手の成立を知らせる (同じ相手で 2 回は知らせない)。 */
    void AnnouncePeer (RemotePeer& peer);

    /** 成立を知らせた相手が外れたことを知らせる。 */
    void NotifyPeerGone (const RemotePeer& peer, PeerEvent::EndReason reason);

    /** mPeers を空にする。成立を知らせた相手には外れたことを知らせる。 */
    void ClearPeers (PeerEvent::EndReason reason);

    void CountRejected (RejectKind kind, const std::string& ip);

    /** 拒否の集計を知らせる。force でなければ窓 (1 分) が過ぎたときだけ。 */
    void FlushRejected (bool force);

    static int SamplesPerPacket (int numChannels);

    //== 1 セッション中だけ使う状態(ネットスレッド専有) =====================
    // 登録済みの相手 1 件。**ハンドシェイクを終えた相手だけ**がここに入る。
    struct RemotePeer
    {
        std::string   ip;
        int           port = 0;
        std::uint32_t lastSeenMs = 0;   // 最後に認証に通ったパケットを受け取った時刻

        // 通信イベント: 成立を知らせたか / いつ成立したか
        bool          announced   = false;
        std::uint32_t connectedMs = 0;

        SessionCipher sendCipher;       // こちら → 相手
        SessionCipher recvCipher;       // 相手 → こちら
        std::uint64_t sendSequence = 0; // 向きごとの連番 (nonce)
        ReplayWindow  recvWindow;

        // 待受側: 成立したハンドシェイクの値。confirm の再送 (こちらの応答が
        // 落ちた場合) を見分けて応答し直すために持つ。
        std::uint8_t  saltC[kSaltBytes] {};
        std::uint8_t  saltL[kSaltBytes] {};
        std::uint8_t  nonceL[kNonceLBytes] {};
    };

    // 待受 + 送信 : 音声の配信先(購読者)。最大 kMaxSubscribers。
    // 待受 + 受信 : 音声を受け入れる押し込み元。**先着 1 本だけ**
    //               (ジッターバッファが単一ストリーム前提のため)。
    // 接続        : 相手 (待受側) 1 件。ハンドシェイクを終えてから入る。
    std::vector<RemotePeer> mPeers;

    // 待受側: ハンドシェイクの保留表。confirm が返るまでは登録しない。
    struct PendingHandshake
    {
        std::string   ip;
        int           port = 0;
        std::uint8_t  saltC[kSaltBytes] {};
        std::uint8_t  saltL[kSaltBytes] {};
        std::uint8_t  nonceL[kNonceLBytes] {};
        std::uint32_t createdMs = 0;
    };

    std::vector<PendingHandshake> mPending;

    // 接続側: ハンドシェイクの進み具合。
    enum class HandshakeStage { idle, hello, confirm, established };

    HandshakeStage mStage = HandshakeStage::idle;
    std::uint8_t   mSaltC[kSaltBytes] {};
    std::uint8_t   mSaltL[kSaltBytes] {};
    std::uint8_t   mNonceL[kNonceLBytes] {};
    std::uint32_t  mLastHandshakeSendMs = 0;
    std::uint32_t  mStageStartMs = 0;

    // 接続側の接続先。セッションの頭で config から写す。
    std::string mTargetHost;
    int         mTargetPort = 0;
    Intent      mIntent = Intent::subscribe;

    std::unique_ptr<MasterKey> mMaster;

    std::uint32_t mLastAudioMs       = 0;   // 受信側: 最後に音声を受け取った時刻
    std::uint32_t mLastChannelInfoMs = 0;   // 送信側: 構成の配信タイマ

    // 接続側: 最後に待受側から認証に通るパケットを受け取った時刻 (診断用)。
    // 途絶の判定は相手ごとの RemotePeer::lastSeenMs で行う。
    // **音声に応答は無いので、生存確認はこれだけが頼り。**
    std::uint32_t mLastPeerReplyMs = 0;

    // このセッションの向き。config の写しで、パケットごとに mConfigMutex を
    // 取らないために持つ。
    bool mSessionIsListener = true;
    bool mSessionSendsAudio = true;

    //== 設定 ================================================================
    mutable std::mutex mConfigMutex;
    Config mConfig;

    double mSampleRate = 48000.0;

    // 受信の目標遅延(秒)。SetReceiveLatency は利用側の任意のスレッドから来る。
    std::atomic<double> mReceiveLatency { 0.040 };

    //== 送信 FIFO(audio→net) ==============================================
    SendFifo mSendFifo;

    std::atomic<int>    mSendChannels { 2 };
    std::atomic<int>    mSendMain     { 2 };
    std::atomic<int>    mSendSub      { 0 };
    std::atomic<double> mSendSampleRate { 48000.0 };

    //== 受信ジッターバッファ(net→audio) ==================================
    JitterBuffer mJitter;

    //== 表示用チャンネル構成 ================================================
    mutable std::mutex mLayoutMutex;
    int mSendInfoMain = 0;
    int mSendInfoSub  = 0;
    std::vector<std::string> mSendNames;
    std::vector<std::string> mReceivedNames;

    //== スレッド ============================================================
    std::thread             mThread;
    std::mutex              mStopMutex;
    std::condition_variable mStopSignal;
    std::atomic<bool>       mShouldExit { false };
    std::atomic<bool>       mRunning    { false };

    // 鍵だけが差し替えられた。**ネットスレッドが次の周回で拾う。**
    std::atomic<bool>       mKeyChanged { false };

    //== 状態(atomic) =======================================================
    std::atomic<int> mState     { (int) ConnectionState::disconnected };
    std::atomic<int> mErrorCode { (int) ErrorCode::none };
    std::atomic<int> mSubscriberCount  { 0 };
    std::atomic<int> mReceivedChannels { 0 };
    std::atomic<int> mReceivedMain     { 0 };
    std::atomic<int> mReceivedSub      { 0 };
    std::atomic<int> mDecryptFailures  { 0 };
    std::atomic<int> mHandshakeFailures { 0 };
    std::atomic<int> mReplayRejects    { 0 };

    //== 通信イベント =========================================================
    // 通知先はどのスレッドからでも差し替えられるので、ロックの下で写してから呼ぶ。
    std::mutex        mPeerEventMutex;
    PeerEventCallback mPeerEventCallback;

    // 拒否の集計 (ネットスレッド専有)
    std::uint32_t              mRejectWindowStartMs = 0;
    PeerEvent                  mRejectCounts;
    std::map<std::string, int> mRejectBySender;
};

} // namespace audiolink
