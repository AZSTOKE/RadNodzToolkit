// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksGold_Excel_Build.h"
#include "RIGDOCKS_GOLD_Build.h"
#include <string>
#include <vector>

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_OpenFile_Build(FString filePath)
{
	RpcCall c = GOLD::AZ_Excel_OpenFile_Build(TCHAR_TO_UTF8(*filePath));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_GetWorksheetList_Build(UXLDocument* document, FString searchStr, bool isRegex, TArray<FString> excludeList)
{
	std::vector<std::string> excludeListStr;
	for (const FString& s : excludeList)
	{
		excludeListStr.push_back(TCHAR_TO_UTF8(*s));
	}

	RpcCall c = GOLD::AZ_Excel_GetWorksheetList_Build(UXLDocument::Get(document), TCHAR_TO_UTF8(*searchStr), isRegex, excludeListStr);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_CloseFile_Build(UXLDocument* excelDocumment)
{
	RpcCall c = GOLD::AZ_Excel_CloseFile_Build(UXLDocument::Get(excelDocumment));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_OpenWorksheet_Build(UXLDocument* excelDocumment, FString sheetName)
{
	RpcCall c = GOLD::AZ_Excel_OpenWorksheet_Build(UXLDocument::Get(excelDocumment), TCHAR_TO_UTF8(*sheetName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_OpenWorksheet_ByNumber_Build(UXLDocument* excelDocumment, int sheetNumber)
{
	RpcCall c = GOLD::AZ_Excel_OpenWorksheet_ByNumber_Build(UXLDocument::Get(excelDocumment), sheetNumber);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_GetCellValue_Build(UXLWorksheet* excelSheet, FString cell)
{
	RpcCall c = GOLD::AZ_Excel_GetCellValue_Build(UXLWorksheet::Get(excelSheet), TCHAR_TO_UTF8(*cell));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_GetCellValue_ByNumber_Build(UXLWorksheet* excelSheet, int rowNumber, int columeNumber)
{
	RpcCall c = GOLD::AZ_Excel_GetCellValue_ByNumber_Build(UXLWorksheet::Get(excelSheet), rowNumber, columeNumber);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_SetCellValue_Build(UXLWorksheet* excelSheet, FString cell, UVarType* value)
{
	RpcCall c = GOLD::AZ_Excel_SetCellValue_Build(UXLWorksheet::Get(excelSheet), TCHAR_TO_UTF8(*cell), value->GetCppObj());

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_SetCellValue_ByNumber_Build(UXLWorksheet* excelSheet, int rowNumber, int columeNumber, UVarType* value)
{
	RpcCall c = GOLD::AZ_Excel_SetCellValue_ByNumber_Build(UXLWorksheet::Get(excelSheet), rowNumber, columeNumber, value->GetCppObj());

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_SaveDocument_Build(UXLDocument* excelDocumment, FString newPath)
{
	RpcCall c = GOLD::AZ_Excel_SaveDocument_Build(UXLDocument::Get(excelDocumment), TCHAR_TO_UTF8(*newPath));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_CreateDocument_Build(FString newPath, bool isOverwrite)
{
	RpcCall c = GOLD::AZ_Excel_CreateDocument_Build(TCHAR_TO_UTF8(*newPath), isOverwrite);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_AddWorksheet_Build(UXLDocument* excelDocumment, FString worksheetName)
{
	RpcCall c = GOLD::AZ_Excel_AddWorksheet_Build(UXLDocument::Get(excelDocumment), TCHAR_TO_UTF8(*worksheetName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_RenameWorksheet_Build(UXLWorksheet* excelWorksheet, FString worksheetName)
{
	RpcCall c = GOLD::AZ_Excel_RenameWorksheet_Build(UXLWorksheet::Get(excelWorksheet), TCHAR_TO_UTF8(*worksheetName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_DeleteWorksheet_Build(UXLWorksheet* excelWorksheet)
{
	RpcCall c = GOLD::AZ_Excel_DeleteWorksheet_Build(UXLWorksheet::Get(excelWorksheet));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_SearchCellsInWorksheet_Build(UXLWorksheet* excelSheet, FString searchStr, FString range, bool isRegex)
{
	RpcCall c = GOLD::AZ_Excel_SearchCellsInWorksheet_Build(UXLWorksheet::Get(excelSheet), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*range), isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_SearchRowsInWorksheet_Build(UXLWorksheet* excelSheet, FString searchStr, FString targetCulume, FString range, bool isRegex)
{
	RpcCall c = GOLD::AZ_Excel_SearchRowsInWorksheet_Build(UXLWorksheet::Get(excelSheet), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*targetCulume), TCHAR_TO_UTF8(*range), isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_SearchCellsInAllWorksheets_Build(UXLDocument* document, FString searchStr, FString range, bool isRegex)
{
	RpcCall c = GOLD::AZ_Excel_SearchCellsInAllWorksheets_Build(UXLDocument::Get(document), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*range), isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksGold_Excel_Build::AZ_Excel_SearchRowsInAllWorksheets_Build(UXLDocument* document, FString searchStr, FString targetCulume, FString range, bool isRegex)
{
	RpcCall c = GOLD::AZ_Excel_SearchRowsInAllWorksheets_Build(UXLDocument::Get(document), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*targetCulume), TCHAR_TO_UTF8(*range), isRegex);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
