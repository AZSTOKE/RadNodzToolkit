// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "DataClass/FExcelSheetInfo.h"
#include "Object/ExcelSheetInfo.h"

using namespace RADNODZ_client::Object;

void FExcelSheetInfo::Initialize(ExcelSheetInfo& obj)
{
	Worksheet = NewObject<UXLWorksheet>();
	Worksheet->SetPointer((void*)obj.Worksheet);

	Name = UTF8_TO_TCHAR(obj.Name.c_str());

	Cells = TMap<FString, FExcelRow>();
	for (const auto& row : obj.Cells)
	{
		FString rowKey = UTF8_TO_TCHAR(row.first.c_str());

		FExcelRow rowValue;

		for (const auto& col : row.second)
		{
			FString colKey = UTF8_TO_TCHAR(col.first.c_str());
			FString value = UTF8_TO_TCHAR(col.second.c_str());
			rowValue.Cells.Add(colKey, value);
		}
		Cells.Add(rowKey, rowValue);
	}
}

ExcelSheetInfo FExcelSheetInfo::GetCppObj() const
{
	ExcelSheetInfo obj;

	obj.Worksheet = UXLWorksheet::Get(Worksheet);
	obj.Name = TCHAR_TO_UTF8(*Name);

	obj.Cells = std::map<std::string, std::map<std::string, std::string>>();
	for (const auto& row : Cells)
	{
		std::string rowKey = TCHAR_TO_UTF8(*row.Key);
		std::map<std::string, std::string> colMap;
		for (const auto& col : row.Value.Cells)
		{
			std::string colKey = TCHAR_TO_UTF8(*col.Key);
			std::string value = TCHAR_TO_UTF8(*col.Value);
			colMap[colKey] = value;
		}
		obj.Cells[rowKey] = colMap;
	}

	return obj;
}

bool UExcelSheetInfoFactory::FromJson(const FString& Json, FExcelSheetInfo& object)
{
	auto cppObjPtr = ExcelSheetInfo::FromJson(TCHAR_TO_UTF8(*Json));

	if (!cppObjPtr)
	{
		return false;
	}

	object.Initialize(*cppObjPtr);

	return true;
}

bool UExcelSheetInfoFactory::FromJsonArray(const FString& Json, TArray<FExcelSheetInfo>& objectArray)
{
	auto cppObjArray = ExcelSheetInfo::FromJsonArray(TCHAR_TO_UTF8(*Json));

	for (auto& cppObj : cppObjArray)
	{
		FExcelSheetInfo obj;
		obj.Initialize(cppObj);
		objectArray.Add(obj);
	}

	return true;
}

FString UExcelSheetInfoFactory::ToJson(const FExcelSheetInfo& object, int indent)
{
	auto cppObj = object.GetCppObj();

	std::string jStr = cppObj.ToJson(indent);

	return UTF8_TO_TCHAR(jStr.c_str());
}

FString UExcelSheetInfoFactory::ToJsonArray(const TArray<FExcelSheetInfo>& objectArray, int indent)
{
	std::vector<ExcelSheetInfo> cppObjArray;
	for (auto& obj : objectArray)
	{
		cppObjArray.push_back(obj.GetCppObj());
	}
	std::string jStr = ExcelSheetInfo::ToJsonArray(cppObjArray, indent);
	return UTF8_TO_TCHAR(jStr.c_str());
}
