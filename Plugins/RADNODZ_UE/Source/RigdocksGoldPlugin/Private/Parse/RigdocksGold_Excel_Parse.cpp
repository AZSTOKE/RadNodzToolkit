// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksGold_Excel_Parse.h"
#include "RIGDOCKS_GOLD_Parse.h"
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

UXLDocument* URigdocksGold_Excel_Parse::AZ_Excel_OpenFile_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	XLDocument* document = GOLD::AZ_Excel_OpenFile_Parse(call->Get());

	UXLDocument* obj = NewObject<UXLDocument>();
	obj->SetPointer((void*)document);
	return obj;
}

TArray<FExcelSheetInfo> URigdocksGold_Excel_Parse::AZ_Excel_GetWorksheetList_Parse(URigdocksRpcCall* call)
{
	TArray<FExcelSheetInfo> result;
	if (call == nullptr)
	{
		return result;
	}

	std::vector<ExcelSheetInfo> sheetList = GOLD::AZ_Excel_GetWorksheetList_Parse(call->Get());
	for (ExcelSheetInfo& obj : sheetList)
	{
		FExcelSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}
	return result;
}

void URigdocksGold_Excel_Parse::AZ_Excel_CloseFile_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Excel_CloseFile_Parse(call->Get());
}

UXLWorksheet* URigdocksGold_Excel_Parse::AZ_Excel_OpenWorksheet_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	XLWorksheet* sheet = GOLD::AZ_Excel_OpenWorksheet_Parse(call->Get());

	UXLWorksheet* obj = NewObject<UXLWorksheet>();
	obj->SetPointer((void*)sheet);
	return obj;
}

UXLWorksheet* URigdocksGold_Excel_Parse::AZ_Excel_OpenWorksheet_ByNumber_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	XLWorksheet* sheet = GOLD::AZ_Excel_OpenWorksheet_ByNumber_Parse(call->Get());

	UXLWorksheet* obj = NewObject<UXLWorksheet>();
	obj->SetPointer((void*)sheet);
	return obj;
}

UVarType* URigdocksGold_Excel_Parse::AZ_Excel_GetCellValue_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	VarType value = GOLD::AZ_Excel_GetCellValue_Parse(call->Get());

	UVarType* valueObj = NewObject<UVarType>();
	valueObj->Initialze(value);
	return valueObj;
}

UVarType* URigdocksGold_Excel_Parse::AZ_Excel_GetCellValue_ByNumber_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	VarType value = GOLD::AZ_Excel_GetCellValue_ByNumber_Parse(call->Get());

	UVarType* valueObj = NewObject<UVarType>();
	valueObj->Initialze(value);
	return valueObj;
}

void URigdocksGold_Excel_Parse::AZ_Excel_SetCellValue_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Excel_SetCellValue_Parse(call->Get());
}

void URigdocksGold_Excel_Parse::AZ_Excel_SetCellValue_ByNumber_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Excel_SetCellValue_ByNumber_Parse(call->Get());
}

void URigdocksGold_Excel_Parse::AZ_Excel_SaveDocument_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Excel_SaveDocument_Parse(call->Get());
}

UXLDocument* URigdocksGold_Excel_Parse::AZ_Excel_CreateDocument_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	XLDocument* document = GOLD::AZ_Excel_CreateDocument_Parse(call->Get());

	UXLDocument* obj = NewObject<UXLDocument>();
	obj->SetPointer((void*)document);
	return obj;
}

UXLWorksheet* URigdocksGold_Excel_Parse::AZ_Excel_AddWorksheet_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	XLWorksheet* sheet = GOLD::AZ_Excel_AddWorksheet_Parse(call->Get());

	UXLWorksheet* obj = NewObject<UXLWorksheet>();
	obj->SetPointer((void*)sheet);
	return obj;
}

void URigdocksGold_Excel_Parse::AZ_Excel_RenameWorksheet_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Excel_RenameWorksheet_Parse(call->Get());
}

void URigdocksGold_Excel_Parse::AZ_Excel_DeleteWorksheet_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_Excel_DeleteWorksheet_Parse(call->Get());
}

FExcelSheetInfo URigdocksGold_Excel_Parse::AZ_Excel_SearchCellsInWorksheet_Parse(URigdocksRpcCall* call)
{
	FExcelSheetInfo result;
	if (call == nullptr)
	{
		return result;
	}

	ExcelSheetInfo obj = GOLD::AZ_Excel_SearchCellsInWorksheet_Parse(call->Get());
	result.Initialize(obj);
	return result;
}

FExcelSheetInfo URigdocksGold_Excel_Parse::AZ_Excel_SearchRowsInWorksheet_Parse(URigdocksRpcCall* call)
{
	FExcelSheetInfo result;
	if (call == nullptr)
	{
		return result;
	}

	ExcelSheetInfo obj = GOLD::AZ_Excel_SearchRowsInWorksheet_Parse(call->Get());
	result.Initialize(obj);
	return result;
}

TArray<FExcelSheetInfo> URigdocksGold_Excel_Parse::AZ_Excel_SearchCellsInAllWorksheets_Parse(URigdocksRpcCall* call)
{
	TArray<FExcelSheetInfo> result;
	if (call == nullptr)
	{
		return result;
	}

	std::vector<ExcelSheetInfo> objList = GOLD::AZ_Excel_SearchCellsInAllWorksheets_Parse(call->Get());
	for (ExcelSheetInfo& obj : objList)
	{
		FExcelSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}
	return result;
}

TArray<FExcelSheetInfo> URigdocksGold_Excel_Parse::AZ_Excel_SearchRowsInAllWorksheets_Parse(URigdocksRpcCall* call)
{
	TArray<FExcelSheetInfo> result;
	if (call == nullptr)
	{
		return result;
	}

	std::vector<ExcelSheetInfo> objList = GOLD::AZ_Excel_SearchRowsInAllWorksheets_Parse(call->Get());
	for (ExcelSheetInfo& obj : objList)
	{
		FExcelSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}
	return result;
}
