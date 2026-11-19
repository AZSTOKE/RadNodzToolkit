// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "define.h"
#include <memory>
#include <vector>

namespace RADNODZ_client::Object
{
	class PathInfo
	{
	public:

		std::string ParentPath;

		std::string ParentName;

		std::string FilePath;

		std::string FileName;

		std::string FileNameWithoutExtension;

		std::string Extension;

		std::string Type;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<PathInfo>& objectArray, int indent = 4);

		static std::unique_ptr<PathInfo> FromJson(std::string jsonStr);

		static std::vector<PathInfo> FromJsonArray(std::string jsonStr);

	};
}
