// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksGold_Excel.h"
#include "RIGDOCKS_GOLD.h"

UXLDocument* URigdocksGold_Excel::AZ_Excel_OpenFile(URadnodzClient* client, FString filePath)
{
	XLDocument* document = GOLD::AZ_Excel_OpenFile(client->GetClient(), TCHAR_TO_UTF8(*filePath));

	UXLDocument* documentObj = NewObject<UXLDocument>();
	documentObj->SetPointer((void*)document);

	return documentObj;
}

void URigdocksGold_Excel::AZ_Excel_CloseFile(URadnodzClient* client, UXLDocument* excelDocumment)
{
	GOLD::AZ_Excel_CloseFile(client->GetClient(), UXLDocument::Get(excelDocumment));

	return;
}

UXLWorksheet* URigdocksGold_Excel::AZ_Excel_OpenWorksheet(URadnodzClient* client, UXLDocument* excelDocumment, FString sheetName)
{
	XLWorksheet* sheet = GOLD::AZ_Excel_OpenWorksheet(client->GetClient(), UXLDocument::Get(excelDocumment), TCHAR_TO_UTF8(*sheetName));

	UXLWorksheet* sheetObj = NewObject<UXLWorksheet>();
	sheetObj->SetPointer((void*)sheet);

	return sheetObj;
}

UXLWorksheet* URigdocksGold_Excel::AZ_Excel_OpenWorksheet_ByNumber(URadnodzClient* client, UXLDocument* excelDocumment, int sheetNumber)
{
	XLWorksheet* sheet = GOLD::AZ_Excel_OpenWorksheet_ByNumber(client->GetClient(), UXLDocument::Get(excelDocumment), sheetNumber);

	UXLWorksheet* sheetObj = NewObject<UXLWorksheet>();
	sheetObj->SetPointer((void*)sheet);

	return sheetObj;
}

UVarType* URigdocksGold_Excel::AZ_Excel_GetCellValue(URadnodzClient* client, UXLWorksheet* excelSheet, FString cell)
{
	VarType value = GOLD::AZ_Excel_GetCellValue(client->GetClient(), UXLWorksheet::Get(excelSheet), TCHAR_TO_UTF8(*cell));

	UVarType* valueObj = NewObject<UVarType>();
	valueObj->Initialze(value);

	return valueObj;
}

UVarType* URigdocksGold_Excel::AZ_Excel_GetCellValue_ByNumber(URadnodzClient* client, UXLWorksheet* excelSheet, int rowNumber, int columeNumber)
{
	VarType value = GOLD::AZ_Excel_GetCellValue_ByNumber(client->GetClient(), UXLWorksheet::Get(excelSheet), rowNumber, columeNumber);

	UVarType* valueObj = NewObject<UVarType>();
	valueObj->Initialze(value);

	return valueObj;
}

void URigdocksGold_Excel::AZ_Excel_SetCellValue(URadnodzClient* client, UXLWorksheet* excelSheet, FString cell, UVarType* value)
{
	GOLD::AZ_Excel_SetCellValue(client->GetClient(), UXLWorksheet::Get(excelSheet), TCHAR_TO_UTF8(*cell), value->GetCppObj());

}

void URigdocksGold_Excel::AZ_Excel_SetCellValue_ByNumber(URadnodzClient* client, UXLWorksheet* excelSheet, int rowNumber, int columeNumber, UVarType* value)
{
	GOLD::AZ_Excel_SetCellValue_ByNumber(client->GetClient(), UXLWorksheet::Get(excelSheet), rowNumber, columeNumber, value->GetCppObj());

}

void URigdocksGold_Excel::AZ_Excel_SaveDocument(URadnodzClient* client, UXLDocument* excelDocumment, FString newPath)
{
	GOLD::AZ_Excel_SaveDocument(client->GetClient(), UXLDocument::Get(excelDocumment), TCHAR_TO_UTF8(*newPath));
}

/// </summar/y>

UXLDocument* URigdocksGold_Excel::AZ_Excel_CreateDocument(URadnodzClient* client, FString newPath, bool isOverwrite)
{
	XLDocument* document = GOLD::AZ_Excel_CreateDocument(client->GetClient(), TCHAR_TO_UTF8(*newPath), isOverwrite);

	UXLDocument* documentObj = NewObject<UXLDocument>();
	documentObj->SetPointer((void*)document);

	return documentObj;
}

UXLWorksheet* URigdocksGold_Excel::AZ_Excel_AddWorksheet(URadnodzClient* client, UXLDocument* excelDocumment, FString worksheetName)
{
	XLWorksheet* sheet = GOLD::AZ_Excel_AddWorksheet(client->GetClient(), UXLDocument::Get(excelDocumment), TCHAR_TO_UTF8(*worksheetName));

	UXLWorksheet* sheetObj = NewObject<UXLWorksheet>();
	sheetObj->SetPointer((void*)sheet);

	return sheetObj;
}

void URigdocksGold_Excel::AZ_Excel_RenameWorksheet(URadnodzClient* client, UXLWorksheet* excelWorksheet, FString worksheetName)
{
	GOLD::AZ_Excel_RenameWorksheet(client->GetClient(), UXLWorksheet::Get(excelWorksheet), TCHAR_TO_UTF8(*worksheetName));
}

void URigdocksGold_Excel::AZ_Excel_DeleteWorksheet(URadnodzClient* client, UXLWorksheet* excelWorksheet)
{
	GOLD::AZ_Excel_DeleteWorksheet(client->GetClient(), UXLWorksheet::Get(excelWorksheet));
}

TArray<FExcelSheetInfo> URigdocksGold_Excel::AZ_Excel_GetWorksheetList(URadnodzClient* client, UXLDocument* document, FString searchStr, bool isRegex, TArray<FString> excludeList)
{
	std::vector<std::string> excludeListStr;
	for (auto str : excludeList)
	{
		excludeListStr.push_back(TCHAR_TO_UTF8(*str));
	}

	std::vector<ExcelSheetInfo> sheetList =  GOLD::AZ_Excel_GetWorksheetList(client->GetClient(), UXLDocument::Get(document), TCHAR_TO_UTF8(*searchStr), isRegex, excludeListStr);

	TArray<FExcelSheetInfo> result;
	for (auto& obj : sheetList)
	{
		FExcelSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}

	return result;
}

FExcelSheetInfo URigdocksGold_Excel::AZ_Excel_SearchCellsInWorksheet(URadnodzClient* client, UXLWorksheet* excelSheet, FString searchStr, FString range, bool isRegex)
{
	auto obj = GOLD::AZ_Excel_SearchCellsInWorksheet(client->GetClient(), UXLWorksheet::Get(excelSheet), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*range), isRegex);

	FExcelSheetInfo result;
	result.Initialize(obj);

	return result;
}

	
FExcelSheetInfo URigdocksGold_Excel::AZ_Excel_SearchRowsInWorksheet(URadnodzClient* client, UXLWorksheet* excelSheet, FString searchStr, FString targetCulume, FString range, bool isRegex)
{
	auto obj = GOLD::AZ_Excel_SearchRowsInWorksheet(client->GetClient(), UXLWorksheet::Get(excelSheet), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*targetCulume), TCHAR_TO_UTF8(*range), isRegex);

	FExcelSheetInfo result;
	result.Initialize(obj);

	return result;
}

	
TArray<FExcelSheetInfo> URigdocksGold_Excel::AZ_Excel_SearchCellsInAllWorksheets(URadnodzClient* client, UXLDocument* document, FString searchStr, FString range, bool isRegex)
{
	auto objList = GOLD::AZ_Excel_SearchCellsInAllWorksheets(client->GetClient(), UXLDocument::Get(document), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*range), isRegex);

	TArray<FExcelSheetInfo> result;
	for (auto& obj : objList)
	{
		FExcelSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}

	return result;
}

	
TArray<FExcelSheetInfo> URigdocksGold_Excel::AZ_Excel_SearchRowsInAllWorksheets(URadnodzClient* client, UXLDocument* document, FString searchStr, FString targetCulume, FString range, bool isRegex)
{
	auto objList = GOLD::AZ_Excel_SearchRowsInAllWorksheets(client->GetClient(), UXLDocument::Get(document), TCHAR_TO_UTF8(*searchStr), TCHAR_TO_UTF8(*targetCulume), TCHAR_TO_UTF8(*range), isRegex);

	TArray<FExcelSheetInfo> result;
	for (auto& obj : objList)
	{
		FExcelSheetInfo info;
		info.Initialize(obj);
		result.Add(info);
	}

	return result;
}

