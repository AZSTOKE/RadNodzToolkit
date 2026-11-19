// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksGold_GSheet_Parse.h"
#include "RIGDOCKS_GOLD_Parse.h"
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

UGSheetConnection* URigdocksGold_GSheet_Parse::AZ_GSheet_Connect_UserAccount_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	GSheetConnection* connection = GOLD::AZ_GSheet_Connect_UserAccount_Parse(call->Get());

	UGSheetConnection* connectionObj = NewObject<UGSheetConnection>();
	connectionObj->SetPointer((void*)connection);
	return connectionObj;
}

void URigdocksGold_GSheet_Parse::AZ_GSheet_Disconnect_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_GSheet_Disconnect_Parse(call->Get());
}

FGSheetInfo URigdocksGold_GSheet_Parse::AZ_GSheet_AddWorksheet_Parse(URigdocksRpcCall* call)
{
	FGSheetInfo result;
	if (call == nullptr)
	{
		return result;
	}

	auto obj = GOLD::AZ_GSheet_AddWorksheet_Parse(call->Get());
	result.Initialize(obj);
	return result;
}

void URigdocksGold_GSheet_Parse::AZ_GSheet_DeleteWorksheet_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_GSheet_DeleteWorksheet_Parse(call->Get());
}

void URigdocksGold_GSheet_Parse::AZ_GSheet_RenameWorksheet_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_GSheet_RenameWorksheet_Parse(call->Get());
}

FGSheetInfo URigdocksGold_GSheet_Parse::AZ_GSheet_CopyWorksheet_Parse(URigdocksRpcCall* call)
{
	FGSheetInfo result;
	if (call == nullptr)
	{
		return result;
	}

	auto obj = GOLD::AZ_GSheet_CopyWorksheet_Parse(call->Get());
	result.Initialize(obj);
	return result;
}

TArray<FGSheetInfo> URigdocksGold_GSheet_Parse::AZ_GSheet_GetWorksheetList_Parse(URigdocksRpcCall* call)
{
	TArray<FGSheetInfo> result;
	if (call == nullptr)
	{
		return result;
	}

	auto objList = GOLD::AZ_GSheet_GetWorksheetList_Parse(call->Get());
	for (auto& obj : objList)
	{
		FGSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}
	return result;
}

UVarType* URigdocksGold_GSheet_Parse::AZ_GSheet_GetCellValue_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	VarType value = GOLD::AZ_GSheet_GetCellValue_Parse(call->Get());

	UVarType* valueObj = NewObject<UVarType>();
	valueObj->Initialze(value);
	return valueObj;
}

UVarType* URigdocksGold_GSheet_Parse::AZ_GSheet_GetCellValue_ByNumber_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	VarType value = GOLD::AZ_GSheet_GetCellValue_ByNumber_Parse(call->Get());

	UVarType* valueObj = NewObject<UVarType>();
	valueObj->Initialze(value);
	return valueObj;
}

void URigdocksGold_GSheet_Parse::AZ_GSheet_SetCellValue_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_GSheet_SetCellValue_Parse(call->Get());
}

void URigdocksGold_GSheet_Parse::AZ_GSheet_SetCellValue_ByNumber_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	GOLD::AZ_GSheet_SetCellValue_ByNumber_Parse(call->Get());
}

FGSheetInfo URigdocksGold_GSheet_Parse::AZ_GSheet_SearchCellsInWorksheet_Parse(URigdocksRpcCall* call)
{
	FGSheetInfo result;
	if (call == nullptr)
	{
		return result;
	}

	auto obj = GOLD::AZ_GSheet_SearchCellsInWorksheet_Parse(call->Get());
	result.Initialize(obj);
	return result;
}

FGSheetInfo URigdocksGold_GSheet_Parse::AZ_GSheet_SearchRowsInWorksheet_Parse(URigdocksRpcCall* call)
{
	FGSheetInfo result;
	if (call == nullptr)
	{
		return result;
	}

	auto obj = GOLD::AZ_GSheet_SearchRowsInWorksheet_Parse(call->Get());
	result.Initialize(obj);
	return result;
}

TArray<FGSheetInfo> URigdocksGold_GSheet_Parse::AZ_GSheet_SearchCellsInAllWorksheets_Parse(URigdocksRpcCall* call)
{
	TArray<FGSheetInfo> result;
	if (call == nullptr)
	{
		return result;
	}

	auto objList = GOLD::AZ_GSheet_SearchCellsInAllWorksheets_Parse(call->Get());
	for (auto& obj : objList)
	{
		FGSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}
	return result;
}

TArray<FGSheetInfo> URigdocksGold_GSheet_Parse::AZ_GSheet_SearchRowsInAllWorksheets_Parse(URigdocksRpcCall* call)
{
	TArray<FGSheetInfo> result;
	if (call == nullptr)
	{
		return result;
	}

	auto objList = GOLD::AZ_GSheet_SearchRowsInAllWorksheets_Parse(call->Get());
	for (auto& obj : objList)
	{
		FGSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}
	return result;
}
