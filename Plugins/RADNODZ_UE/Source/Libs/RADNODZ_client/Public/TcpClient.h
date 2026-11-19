// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include <memory>
#include <string>
#include <cstdint>
#include <functional>
#include <vector>

namespace RADNODZ_client
{
    // State of the server host key seen at connect time.
    // A pinned key that matches does NOT invoke the verifier.
    enum class HostKeyStatus
    {
        Unknown,   // first seen (TOFU): no pin recorded for this serverId
        Changed,   // known but mismatched (server reinstall OR attack)
    };

    // Verifier decision.
    enum class HostKeyVerdict
    {
        Reject,        // refuse the connection
        AcceptAndPin,  // accept and pin for future automatic verification
    };

    // Information passed to the host key verifier callback.
    struct HostKeyInfo
    {
        std::string   serverId;      // app-supplied logical server id (= pin key)
        std::string   ip;
        int           port = 0;
        std::string   fingerprint;   // "SHA256:<base64>"
        std::string   publicKeyPem;
        HostKeyStatus status = HostKeyStatus::Unknown;
    };

    // A pinned server entry, keyed by the app-supplied logical server id.
    struct PinnedServer
    {
        std::string serverId;
        std::string fingerprint;   // "SHA256:<base64>"
        std::string publicKeyPem;
    };

    // Called when the server host key needs a decision (Unknown/Changed).
    // Registration is mandatory (fail-close): connecting without a verifier set,
    // when a decision is required, refuses the connection.
    using HostKeyVerifier = std::function<HostKeyVerdict(const HostKeyInfo&)>;

    class TcpClient
    {
    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;

        // Send the first frame of a connection, naming the target module.
        // The host replies with success/failure before anything else is sent.
        bool SendTargetFrame();

		std::string ip_;
		int port_;

		int64_t lastSendTime_ = 0; 
		int64_t lastServerRecvTime_ = 0; 
		int64_t lastServerReturnTime_ = 0;
		int64_t lastRecvTime_ = 0;

    public:
        TcpClient();
        ~TcpClient();

        TcpClient(const std::string& ip, const int port);

        bool Connect();

        // `ip` accepts a host name as well as an address literal; it is resolved
        // with getaddrinfo at connect time. Naming the host keeps a pairing url
        // valid across a DHCP lease or a DDNS update, since the pin is keyed on
        // the node id rather than the address.
        //
        // IPv4 candidates are tried first because the server listens on IPv4
        // only at the moment.
        bool Connect(const std::string& ip, const int port);

        // ===== Secure comms: identity (for mTLS) =====
        // The consuming app injects the storage directory for the key/cert
        // (handles UE/Android/Mac sandboxes). Call before GenerateIdentity/Connect.
        void SetStorageDirectory(const std::string& dir);

        // Prepare an ECDSA P-256 key pair + self-signed cert (idempotent:
        // load if present, generate if absent). Fails if the storage dir is unset.
        bool GenerateIdentity();

        // Explicit rotation (always generate anew). Invalidates all existing pairings.
        bool RegenerateIdentity();

        // For pairing: own public key PEM (register this in the server's authorized set).
        std::string GetOwnPublicKeyPem();

        // Own fingerprint, "SHA256:<base64>".
        std::string GetOwnFingerprint();

        // ===== Secure comms: server verification / pinning =====
        // Logical server id used as the pin key (IP may change on a LAN).
        // Call before Connect.
        void SetServerId(const std::string& id);

        // Register the host key verifier (mandatory for a first/changed key: fail-close).
        void SetHostKeyVerifier(HostKeyVerifier v);

        // Pin management (trust reset).
        std::vector<PinnedServer> ListPinnedServers();
        bool RemovePinnedServer(const std::string& serverId);

        // Pin a server *before* connecting.
        //
        // Used when the fingerprint was obtained out of band (a QR code or a
        // pasted URL). With the pin already in place the first connection needs
        // no verifier decision, so no trust-on-first-use window exists.
        // Requires the storage directory to be set.
        bool PinServer(const std::string& serverId, const std::string& fingerprint,
            const std::string& publicKeyPem = std::string());

        // Apply a pairing URL read from a QR code (or pasted).
        //
        // Sets the connection target and the pin in one step:
        //   - server id / address / port taken from the URL
        //   - the fingerprint is pinned, so the first connection needs no
        //     trust-on-first-use decision
        //
        // Requires the storage directory to be set. Returns false when the URL
        // is malformed or the pin could not be written.
        bool ApplyServerPairingUrl(const std::string& url);

        // ===== Routing =====
        // The host owns a single public TCP port and routes the connection to
        // the server module named here (a product id). Defaults to the product
        // id this client was built for, so callers normally leave it alone.
        void SetTargetProductId(const std::string& productId);

        std::string GetTargetProductId() const;

        // ===== Client key enrollment =====
        //
        // An unregistered client submits its own public key to the server; the
        // operator then approves it. Requires TLS, so the identity must exist
        // (GenerateIdentity) and the server must be pinned or verified.
        enum class EnrollStatus
        {
            // Failed to reach the server / TLS could not be established.
            Failed,

            // Submitted (or already submitted); waiting for the operator.
            Pending,

            // This client key is already authorized: a normal Connect will work.
            Approved,

            // The server is not accepting requests right now.
            Closed,

            // Refused (proof-of-possession mismatch, bad input, pool full, ...).
            Rejected,
        };

        // Submit this client's public key. `clientName` is a human-readable
        // label shown to the operator; it is not verified.
        EnrollStatus Enroll(const std::string& clientName);

        // Send a request and wait for its response.
        //
        // One call occupies the connection for the whole exchange: concurrent
        // calls from other threads are serialized rather than interleaved, so
        // the framing cannot be corrupted. A caller can therefore be blocked by
        // another thread's request that is still in flight.
        //
        // Returns an empty string on failure; `errorCode` / `errorMessage` then
        // say whether the response timed out (212), the request could not be
        // sent (213), or the connection dropped (214).
        std::string Send(const std::string& message);

        // Deadline for waiting on a response, in seconds. 0 (the default) means
        // no deadline: a dead peer is detected by TCP keepalive instead
        // (~30 s), which keeps legitimately slow calls - a loudness analysis
        // runs for tens of seconds - from being cut off.
        //
        // Set a value only where the caller knows its own upper bound.
        void SetResponseTimeoutSec(int seconds);

        int GetResponseTimeoutSec() const;

        // Close the connection.
        //
        // Safe to call while another thread is waiting on a response: the
        // socket is shut down first so that the waiting thread returns, and it
        // is closed only once that thread has left (closing a descriptor out
        // from under a blocked reader is not enough on Linux/Android, and the
        // number could be reused by another socket).
        void Disconnect();

        bool IsConnected() const;

        int64_t GetLastSendTime() const;
        int64_t GetLastServerRecvTime() const;
        int64_t GetLastServerReturnTime() const;
        int64_t GetLastRecvTime() const;

        int errorCode = 0;

        std::string errorMessage = "";

        std::string errorDetails = "";
    };
}
