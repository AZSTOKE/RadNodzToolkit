// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include <utility>

#define BUILD_FUNC(f) f##_Build
#define PARSE_FUNC(f) f##_Parse

namespace RADNODZ_client
{
    class TcpClient;

    class BatchValue
    {
    public:
        BatchValue(int v);
        BatchValue(int64_t v);
        BatchValue(double v);
        BatchValue(bool v);
        BatchValue(const char* v);
        BatchValue(const std::string& v);
        BatchValue(const std::vector<std::string>& v);
        BatchValue(const std::vector<int>& v);
        BatchValue(const std::vector<double>& v);

        static BatchValue Pointer(uint64_t handle);

        static BatchValue Ref(int callIndex, int retvalIndex = 0);

        const std::string& Json() const { return json_; }

    private:
        friend class BatchBuilder;
        BatchValue() = default;
        std::string json_;
    };

    struct BatchCallResult
    {
        bool        success = false;
        int         errorCode = 0;
        std::string errorMessage;
        std::string errorDetails;
        std::string retvalsJson;
    };

    class BatchBuilder
    {
    public:
        BatchBuilder();
        ~BatchBuilder();

        BatchBuilder(const BatchBuilder&) = delete;
        BatchBuilder& operator=(const BatchBuilder&) = delete;

        template <class... Args>
        int Add(const std::string& functionName, Args&&... args)
        {
            const int index = Begin(functionName);
            (Arg(BatchValue(std::forward<Args>(args))), ...);
            return index;
        }

        int Begin(const std::string& functionName); 
        void Arg(const BatchValue& value);

        static BatchValue Ref(int callIndex, int retvalIndex = 0);

        int AddJson(const std::string& functionName, const std::string& argsJson = "[]");

        int Count() const;

        void Clear();

        bool Execute(TcpClient& client);

        const std::vector<BatchCallResult>& Results() const;

        int FailedIndex() const;

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}
