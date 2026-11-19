// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "define.h"
#include <memory>
#include <string>
#include <vector>
#include <map>

class XLWorksheet;

namespace RADNODZ_client::Object
{
	class ExcelSheetInfo
	{
	public:
		XLWorksheet* Worksheet;

		std::string Name;

		std::map<std::string, std::map<std::string, std::string>> Cells;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<ExcelSheetInfo>& objectArray, int indent = 4);

		static std::unique_ptr<ExcelSheetInfo> FromJson(std::string jsonStr);

		static std::vector<ExcelSheetInfo> FromJsonArray(std::string jsonStr);

	};
}
