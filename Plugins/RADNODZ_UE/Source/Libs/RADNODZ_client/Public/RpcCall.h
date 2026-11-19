// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include <string>
#include <vector>

namespace RADNODZ_client
{
    class TcpClient;

    struct RpcCall
    {
        std::string functionName;
        std::string argsJson = "[]";
        std::string resultJson;

        std::string BuildRequest() const;

        void SetResult(const std::string& res) { resultJson = res; }

        void BindRef(int argIndex, int fromCall, int retvalIndex = 0);

        bool HasError() const;
        int  ErrorCode() const;

        std::string RetvalsJson() const;
    };

    bool Execute(TcpClient& client, RpcCall& call);

    bool ExecuteBatch(TcpClient& client, std::vector<RpcCall>& calls);
}
