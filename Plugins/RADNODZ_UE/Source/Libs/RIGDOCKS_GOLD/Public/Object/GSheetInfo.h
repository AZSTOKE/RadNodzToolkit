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
#include "CellRow.h"

class XLWorksheet;

namespace RADNODZ_client::Object
{
	class GSheetInfo
	{
	public:
		int Id;

		std::string Name;

		int Index;

		std::vector<CellRow> Cells;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<GSheetInfo>& objectArray, int indent = 4);

		static std::unique_ptr<GSheetInfo> FromJson(std::string jsonStr);

		static std::vector<GSheetInfo> FromJsonArray(std::string jsonStr);

	};
}
