// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include <string>

// Server connection info carried in a single URL (shown as a QR code).
//
//   rigdocks://server?v=1&id=<nodeId>&h=<host>&p=<port>&fp=<fingerprint>
//
// Reading it gives the client everything it needs to connect securely on the
// first attempt: the pin key (nodeId), the expected server fingerprint, and
// where to connect. Because the fingerprint is known up front there is no
// trust-on-first-use window.
//
// The host may be a name rather than an address literal: TcpClient resolves it
// with getaddrinfo. Naming the host keeps the url valid across a DHCP lease or
// a DDNS update, and the pin is keyed on the node id, so nothing is re-paired.
//
// The contents are public information (a public-key fingerprint and an
// address), so nothing here is secret. There is no tamper protection though,
// so a URL relayed by mail or chat should still have its fingerprint checked
// by eye against what the server shows.
namespace RADNODZ_client
{
    struct ServerPairingInfo
    {
        // Logical server id; used as the pin key.
        std::string nodeId;

        std::string host;

        int port = 0;

        // Expected server fingerprint, "SHA256:<base64>".
        std::string fingerprint;
    };

    // Parse a pairing URL. Returns false when the scheme, the version or any
    // required field is missing or malformed.
    bool ParseServerPairingUrl(const std::string& url, ServerPairingInfo& out);
}
