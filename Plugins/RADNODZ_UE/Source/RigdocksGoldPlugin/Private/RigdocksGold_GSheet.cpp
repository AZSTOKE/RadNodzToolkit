// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksGold_GSheet.h"
#include "RIGDOCKS_GOLD.h"

UGSheetConnection* URigdocksGold_GSheet::AZ_GSheet_Connect_UserAccount(URadnodzClient* client, FString spreadSheetId, FString clientId)
{
	GSheetConnection* connection = GOLD::AZ_GSheet_Connect_UserAccount(client->GetClient(), TCHAR_TO_UTF8(*spreadSheetId), TCHAR_TO_UTF8(*clientId));

	UGSheetConnection* connectionObj = NewObject<UGSheetConnection>();
	connectionObj->SetPointer((void*)connection);

	return connectionObj;
}

void URigdocksGold_GSheet::AZ_GSheet_Disconnect(URadnodzClient* client, UGSheetConnection* connection)
{
	GOLD::AZ_GSheet_Disconnect(client->GetClient(), UGSheetConnection::Get(connection));

	return;
}

FGSheetInfo URigdocksGold_GSheet::AZ_GSheet_AddWorksheet(URadnodzClient* client, UGSheetConnection* connection, FString sheetName)
{
	auto obj = GOLD::AZ_GSheet_AddWorksheet(client->GetClient(), UGSheetConnection::Get(connection), TCHAR_TO_UTF8(*sheetName));

	FGSheetInfo result;
	result.Initialize(obj);

	return result;
}

void URigdocksGold_GSheet::AZ_GSheet_DeleteWorksheet(URadnodzClient* client, UGSheetConnection* connection, int sheetId)
{
	GOLD::AZ_GSheet_DeleteWorksheet(client->GetClient(), UGSheetConnection::Get(connection), sheetId);

	return;
}

void URigdocksGold_GSheet::AZ_GSheet_RenameWorksheet(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString newSheetName)
{
	GOLD::AZ_GSheet_RenameWorksheet(client->GetClient(), UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*newSheetName));

	return;
}

FGSheetInfo URigdocksGold_GSheet::AZ_GSheet_CopyWorksheet(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString newSheetName, bool isNextSheet)
{
	auto obj = GOLD::AZ_GSheet_CopyWorksheet(client->GetClient(), UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*newSheetName), isNextSheet);

	FGSheetInfo result;
	result.Initialize(obj);

	return result;
}

TArray<FGSheetInfo> URigdocksGold_GSheet::AZ_GSheet_GetWorksheetList(URadnodzClient* client, UGSheetConnection* connection, FString searchStr, bool isRegex, TArray<FString> excludeList)
{
	std::vector<std::string> excludeListStr;
	for (auto str : excludeList)
	{
		excludeListStr.push_back(TCHAR_TO_UTF8(*str));
	}

	auto objList = GOLD::AZ_GSheet_GetWorksheetList(client->GetClient(), UGSheetConnection::Get(connection), TCHAR_TO_UTF8(*searchStr), isRegex, excludeListStr);

	TArray<FGSheetInfo> result;
	for (auto& obj : objList)
	{
		FGSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}

	return result;
}

UVarType* URigdocksGold_GSheet::AZ_GSheet_GetCellValue(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString cell)
{
	VarType value = GOLD::AZ_GSheet_GetCellValue(client->GetClient(), UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*cell));

	UVarType* valueObj = NewObject<UVarType>();
	valueObj->Initialze(value);

	return valueObj;
}

UVarType* URigdocksGold_GSheet::AZ_GSheet_GetCellValue_ByNumber(URadnodzClient* client, UGSheetConnection* connection, int sheetId, int rowNumber, int columnNumber)
{
	VarType value = GOLD::AZ_GSheet_GetCellValue_ByNumber(client->GetClient(), UGSheetConnection::Get(connection), sheetId, rowNumber, columnNumber);

	UVarType* valueObj = NewObject<UVarType>();
	valueObj->Initialze(value);

	return valueObj;
}

void URigdocksGold_GSheet::AZ_GSheet_SetCellValue(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString cell, UVarType* value)
{
	GOLD::AZ_GSheet_SetCellValue(client->GetClient(), UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*cell), value->GetCppObj());
}

void URigdocksGold_GSheet::AZ_GSheet_SetCellValue_ByNumber(URadnodzClient* client, UGSheetConnection* connection, int sheetId, int rowNumber, int columnNumber, UVarType* value)
{
	GOLD::AZ_GSheet_SetCellValue_ByNumber(client->GetClient(), UGSheetConnection::Get(connection), sheetId, rowNumber, columnNumber, value->GetCppObj());
}

FGSheetInfo URigdocksGold_GSheet::AZ_GSheet_SearchCellsInWorksheet(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString searchStr, FString range, bool isRegex)
{
	auto obj = GOLD::AZ_GSheet_SearchCellsInWorksheet(client->GetClient(), UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*range), isRegex);

	FGSheetInfo result;
	result.Initialize(obj);

	return result;
}

FGSheetInfo URigdocksGold_GSheet::AZ_GSheet_SearchRowsInWorksheet(URadnodzClient* client, UGSheetConnection* connection, int sheetId, FString searchStr, FString targetColumn, FString range, bool isRegex)
{
	auto obj = GOLD::AZ_GSheet_SearchRowsInWorksheet(client->GetClient(), UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*targetColumn), TCHAR_TO_UTF8(*range), isRegex);

	FGSheetInfo result;
	result.Initialize(obj);

	return result;
}

TArray<FGSheetInfo> URigdocksGold_GSheet::AZ_GSheet_SearchCellsInAllWorksheets(URadnodzClient* client, UGSheetConnection* connection, FString searchStr, FString range, bool isRegex)
{
	auto objList = GOLD::AZ_GSheet_SearchCellsInAllWorksheets(client->GetClient(), UGSheetConnection::Get(connection), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*range), isRegex);

	TArray<FGSheetInfo> result;
	for (auto& obj : objList)
	{
		FGSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}

	return result;
}

TArray<FGSheetInfo> URigdocksGold_GSheet::AZ_GSheet_SearchRowsInAllWorksheets(URadnodzClient* client, UGSheetConnection* connection, FString searchStr, FString targetColumn, FString range, bool isRegex)
{
	auto objList = GOLD::AZ_GSheet_SearchRowsInAllWorksheets(client->GetClient(), UGSheetConnection::Get(connection), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*targetColumn), TCHAR_TO_UTF8(*range), isRegex);

	TArray<FGSheetInfo> result;
	for (auto& obj : objList)
	{
		FGSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}

	return result;
}
