// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "define.h"
#include <map>
#include <memory>
#include <vector>

namespace RADNODZ_client::Object
{
	class HttpResponse
	{
	public:

		std::string Body;

		std::map<std::string, std::string> Headers;

		int StatusCode;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<HttpResponse>& objectArray, int indent = 4);

		static std::unique_ptr<HttpResponse> FromJson(std::string jsonStr);

		static std::vector<HttpResponse> FromJsonArray(std::string jsonStr);

	};
}
