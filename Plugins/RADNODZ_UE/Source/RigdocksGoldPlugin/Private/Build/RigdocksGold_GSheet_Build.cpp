// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksGold_GSheet_Build.h"
#include "RIGDOCKS_GOLD_Build.h"
#include <string>
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_Connect_UserAccount_Build(FString spreadSheetId, FString clientId)
{
	RpcCall c = GOLD::AZ_GSheet_Connect_UserAccount_Build(TCHAR_TO_UTF8(*spreadSheetId), TCHAR_TO_UTF8(*clientId));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_Disconnect_Build(UGSheetConnection* connection)
{
	RpcCall c = GOLD::AZ_GSheet_Disconnect_Build(UGSheetConnection::Get(connection));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_AddWorksheet_Build(UGSheetConnection* connection, FString sheetName)
{
	RpcCall c = GOLD::AZ_GSheet_AddWorksheet_Build(UGSheetConnection::Get(connection), TCHAR_TO_UTF8(*sheetName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_DeleteWorksheet_Build(UGSheetConnection* connection, int sheetId)
{
	RpcCall c = GOLD::AZ_GSheet_DeleteWorksheet_Build(UGSheetConnection::Get(connection), sheetId);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_RenameWorksheet_Build(UGSheetConnection* connection, int sheetId, FString newSheetName)
{
	RpcCall c = GOLD::AZ_GSheet_RenameWorksheet_Build(UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*newSheetName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_CopyWorksheet_Build(UGSheetConnection* connection, int sheetId, FString newSheetName, bool isNextSheet)
{
	RpcCall c = GOLD::AZ_GSheet_CopyWorksheet_Build(UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*newSheetName), isNextSheet);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_GetWorksheetList_Build(UGSheetConnection* connection, FString searchStr, bool isRegex, TArray<FString> excludeList)
{
	std::vector<std::string> excludeListStr;
	for (const FString& s : excludeList)
	{
		excludeListStr.push_back(TCHAR_TO_UTF8(*s));
	}

	RpcCall c = GOLD::AZ_GSheet_GetWorksheetList_Build(UGSheetConnection::Get(connection), TCHAR_TO_UTF8(*searchStr), isRegex, excludeListStr);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_GetCellValue_Build(UGSheetConnection* connection, int sheetId, FString cell)
{
	RpcCall c = GOLD::AZ_GSheet_GetCellValue_Build(UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*cell));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_GetCellValue_ByNumber_Build(UGSheetConnection* connection, int sheetId, int rowNumber, int columnNumber)
{
	RpcCall c = GOLD::AZ_GSheet_GetCellValue_ByNumber_Build(UGSheetConnection::Get(connection), sheetId, rowNumber, columnNumber);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_SetCellValue_Build(UGSheetConnection* connection, int sheetId, FString cell, UVarType* value)
{
	RpcCall c = GOLD::AZ_GSheet_SetCellValue_Build(UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*cell), value->GetCppObj());

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_SetCellValue_ByNumber_Build(UGSheetConnection* connection, int sheetId, int rowNumber, int columnNumber, UVarType* value)
{
	RpcCall c = GOLD::AZ_GSheet_SetCellValue_ByNumber_Build(UGSheetConnection::Get(connection), sheetId, rowNumber, columnNumber, value->GetCppObj());

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_SearchCellsInWorksheet_Build(UGSheetConnection* connection, int sheetId, FString searchStr, FString range, bool isRegex)
{
	RpcCall c = GOLD::AZ_GSheet_SearchCellsInWorksheet_Build(UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*range), isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_SearchRowsInWorksheet_Build(UGSheetConnection* connection, int sheetId, FString searchStr, FString targetColumn, FString range, bool isRegex)
{
	RpcCall c = GOLD::AZ_GSheet_SearchRowsInWorksheet_Build(UGSheetConnection::Get(connection), sheetId, TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*targetColumn), TCHAR_TO_UTF8(*range), isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_SearchCellsInAllWorksheets_Build(UGSheetConnection* connection, FString searchStr, FString range, bool isRegex)
{
	RpcCall c = GOLD::AZ_GSheet_SearchCellsInAllWorksheets_Build(UGSheetConnection::Get(connection), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*range), isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_GSheet_Build::AZ_GSheet_SearchRowsInAllWorksheets_Build(UGSheetConnection* connection, FString searchStr, FString targetColumn, FString range, bool isRegex)
{
	RpcCall c = GOLD::AZ_GSheet_SearchRowsInAllWorksheets_Build(UGSheetConnection::Get(connection), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*targetColumn), TCHAR_TO_UTF8(*range), isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
