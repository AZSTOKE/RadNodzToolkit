// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_Project_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

FString URigdocksSilver_Project_Parse::AZ_SetProjectInfoRecPath_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string pathStr = SILVER::AZ_SetProjectInfoRecPath_Parse(call->Get());
	return UTF8_TO_TCHAR(pathStr.c_str());
}

void URigdocksSilver_Project_Parse::AZ_SetProjectInfoRecAddPath_Parse(URigdocksRpcCall* call, FString& brforePath, FString& afterPath)
{
	if (call == nullptr)
	{
		return;
	}

	auto[befPathStr, aftPathStr] = SILVER::AZ_SetProjectInfoRecAddPath_Parse(call->Get());

	brforePath = UTF8_TO_TCHAR(befPathStr.c_str());
	afterPath = UTF8_TO_TCHAR(aftPathStr.c_str());
}

void URigdocksSilver_Project_Parse::AZ_SetProjectPathFolder_Parse(URigdocksRpcCall* call, FString& brforePath, FString& afterPath)
{
	if (call == nullptr)
	{
		return;
	}

	auto[befPathStr, aftPathStr] = SILVER::AZ_SetProjectPathFolder_Parse(call->Get());

	brforePath = UTF8_TO_TCHAR(befPathStr.c_str());
	afterPath = UTF8_TO_TCHAR(aftPathStr.c_str());
}

void URigdocksSilver_Project_Parse::AZ_SetResoucePathFolder_Parse(URigdocksRpcCall* call, FString& brforePath, FString& afterPath)
{
	if (call == nullptr)
	{
		return;
	}

	auto[befPathStr, aftPathStr] = SILVER::AZ_SetResoucePathFolder_Parse(call->Get());

	brforePath = UTF8_TO_TCHAR(befPathStr.c_str());
	afterPath = UTF8_TO_TCHAR(aftPathStr.c_str());
}

bool URigdocksSilver_Project_Parse::AZ_GetRepeat_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetRepeat_Parse(call->Get());
}

void URigdocksSilver_Project_Parse::AZ_SetRepeate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SetRepeate_Parse(call->Get());
}

bool URigdocksSilver_Project_Parse::AZ_GetPlayState_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetPlayState_Parse(call->Get());
}

bool URigdocksSilver_Project_Parse::AZ_GetRecordState_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetRecordState_Parse(call->Get());
}

bool URigdocksSilver_Project_Parse::AZ_GetPauseState_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetPauseState_Parse(call->Get());
}

bool URigdocksSilver_Project_Parse::AZ_GetStopState_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_GetStopState_Parse(call->Get());
}

double URigdocksSilver_Project_Parse::AZ_GetRecordingSampleRate_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	return SILVER::AZ_GetRecordingSampleRate_Parse(call->Get());
}

int URigdocksSilver_Project_Parse::AZ_GetRecordingBitDepth_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	return SILVER::AZ_GetRecordingBitDepth_Parse(call->Get());
}

FString URigdocksSilver_Project_Parse::AZ_GetProjectName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string nameStr = SILVER::AZ_GetProjectName_Parse(call->Get());
	return UTF8_TO_TCHAR(nameStr.c_str());
}

FString URigdocksSilver_Project_Parse::AZ_GetProjectPath_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string pathStr = SILVER::AZ_GetProjectPath_Parse(call->Get());
	return UTF8_TO_TCHAR(pathStr.c_str());
}

UReaProject* URigdocksSilver_Project_Parse::AZ_OpenProject_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	ReaProject* proj = SILVER::AZ_OpenProject_Parse(call->Get());

	UReaProject* projObject = NewObject<UReaProject>();
	projObject->SetPointer((void*)proj);
	return projObject;
}

void URigdocksSilver_Project_Parse::AZ_SaveProject_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SaveProject_Parse(call->Get());
}

void URigdocksSilver_Project_Parse::AZ_CloseProject_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_CloseProject_Parse(call->Get());
}

void URigdocksSilver_Project_Parse::AZ_SwitchProject_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_SwitchProject_Parse(call->Get());
}

UReaProject* URigdocksSilver_Project_Parse::AZ_GetProjectFromName_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	ReaProject* proj = SILVER::AZ_GetProjectFromName_Parse(call->Get());

	UReaProject* projObject = NewObject<UReaProject>();
	projObject->SetPointer((void*)proj);
	return projObject;
}

void URigdocksSilver_Project_Parse::AZ_LaunchReaper_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_LaunchReaper_Parse(call->Get());
}

bool URigdocksSilver_Project_Parse::AZ_IsReaperRunning_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return false;
	}

	return SILVER::AZ_IsReaperRunning_Parse(call->Get());
}

void URigdocksSilver_Project_Parse::AZ_QuitReaper_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_QuitReaper_Parse(call->Get());
}
