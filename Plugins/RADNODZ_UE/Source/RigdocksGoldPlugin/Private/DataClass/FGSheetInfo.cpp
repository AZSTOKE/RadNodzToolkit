// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FGSheetInfo.h"
#include "Object/GSheetInfo.h"

using namespace RADNODZ_client::Object;

void FGSheetInfo::Initialize(GSheetInfo& obj)
{
	Id = obj.Id;
	Name = UTF8_TO_TCHAR(obj.Name.c_str());
	Index = obj.Index;

	Cells = TArray<FCellRow>();
	for (const auto& row : obj.Cells)
	{
		FCellRow rowValue;
		rowValue.RowNumber = row.RowNumber;

		for (const auto& col : row.Cells)
		{
			FString colKey = UTF8_TO_TCHAR(col.first.c_str());

			FCellItem cellItem;
			cellItem.RowNumber = col.second.RowNumber;
			cellItem.ColNumber = col.second.ColNumber;
			cellItem.CellAddress = UTF8_TO_TCHAR(col.second.CellAddress.c_str());
			cellItem.Value = UTF8_TO_TCHAR(col.second.Value.c_str());

			rowValue.Cells.Add(colKey, cellItem);
		}

		Cells.Add(rowValue);
	}
}

GSheetInfo FGSheetInfo::GetCppObj() const
{
	GSheetInfo obj;

	obj.Id = Id;
	obj.Name = TCHAR_TO_UTF8(*Name);
	obj.Index = Index;

	obj.Cells = std::vector<CellRow>();
	for (const auto& row : Cells)
	{
		CellRow rowValue;
		rowValue.RowNumber = row.RowNumber;

		for (const auto& col : row.Cells)
		{
			std::string colKey = TCHAR_TO_UTF8(*col.Key);

			CellItem cellItem;
			cellItem.RowNumber = col.Value.RowNumber;
			cellItem.ColNumber = col.Value.ColNumber;
			cellItem.CellAddress = TCHAR_TO_UTF8(*col.Value.CellAddress);
			cellItem.Value = TCHAR_TO_UTF8(*col.Value.Value);

			rowValue.Cells[colKey] = cellItem;
		}

		obj.Cells.push_back(rowValue);
	}

	return obj;
}

bool UGSheetInfoFactory::FromJson(const FString& Json, FGSheetInfo& object)
{
	auto cppObjPtr = GSheetInfo::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UGSheetInfoFactory::FromJsonArray(const FString& Json, TArray<FGSheetInfo>& objectArray)
{
	auto cppObjArray = GSheetInfo::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FGSheetInfo obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UGSheetInfoFactory::ToJson(const FGSheetInfo& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UGSheetInfoFactory::ToJsonArray(const TArray<FGSheetInfo>& objectArray, int indent)
{
	std::vector<GSheetInfo> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = GSheetInfo::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
