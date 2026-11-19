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

namespace RADNODZ_client::Object
{
	class CellItem
	{
	public:
		int RowNumber = 0;
		int ColNumber = 0;
		std::string CellAddress = "";
		std::string Value = "";

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<CellItem>& objectArray, int indent = 4);

		static std::unique_ptr<CellItem> FromJson(std::string jsonStr);

		static std::vector<CellItem> FromJsonArray(std::string jsonStr);

	};

	class CellRow
	{
	public:
		int RowNumber = 0;

		std::map<std::string, CellItem>  Cells;

		std::string ToJson(int indent = 4);

		static std::string ToJsonArray(const std::vector<CellRow>& objectArray, int indent = 4);

		static std::unique_ptr<CellRow> FromJson(std::string jsonStr);

		static std::vector<CellRow> FromJsonArray(std::string jsonStr);

	};
}
