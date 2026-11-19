// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_Project.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client::Object;

FString URigdocksSilver_Project::AZ_SetProjectInfoRecPath(URadnodzClient* client, UReaProject* projID, FString path)
{
    std::string pathStr = SILVER::AZ_SetProjectInfoRecPath(client->GetClient(), UReaProject::Get(projID), TCHAR_TO_UTF8(*path));

	return UTF8_TO_TCHAR(pathStr.c_str());
}

void URigdocksSilver_Project::AZ_SetProjectInfoRecAddPath(URadnodzClient* client, UReaProject* projID, FString addPath, FString& brforePath, FString& afterPath)
{
	auto[befPathStr, aftPathStr] = SILVER::AZ_SetProjectInfoRecAddPath(client->GetClient(), UReaProject::Get(projID), TCHAR_TO_UTF8(*addPath));

	brforePath = UTF8_TO_TCHAR(befPathStr.c_str());
	afterPath = UTF8_TO_TCHAR(aftPathStr.c_str());
}

void URigdocksSilver_Project::AZ_SetProjectPathFolder(URadnodzClient* client, FString folderName, int timeAddType, FString& brforePath, FString& afterPath)
{
	auto [befPathStr, aftPathStr] = SILVER::AZ_SetProjectPathFolder(client->GetClient(),TCHAR_TO_UTF8(*folderName), timeAddType);

	brforePath = UTF8_TO_TCHAR(befPathStr.c_str());
	afterPath = UTF8_TO_TCHAR(aftPathStr.c_str());
}

void URigdocksSilver_Project::AZ_SetResoucePathFolder(URadnodzClient* client, FString folderName, int timeAddType, FString& brforePath, FString& afterPath)
{
	auto [befPathStr, aftPathStr] = SILVER::AZ_SetResoucePathFolder(client->GetClient(), TCHAR_TO_UTF8(*folderName), timeAddType);

	brforePath = UTF8_TO_TCHAR(befPathStr.c_str());
	afterPath = UTF8_TO_TCHAR(aftPathStr.c_str());
}

bool URigdocksSilver_Project::AZ_GetRepeat(URadnodzClient* client, UReaProject* projID)
{
	return SILVER::AZ_GetRepeat(client->GetClient(), UReaProject::Get(projID));
}

void URigdocksSilver_Project::AZ_SetRepeate(URadnodzClient* client, UReaProject* projID, bool repeat)
{
	SILVER::AZ_SetRepeate(client->GetClient(), UReaProject::Get(projID), repeat);
}

bool URigdocksSilver_Project::AZ_GetPlayState(URadnodzClient* client, UReaProject* projID)
{
	return SILVER::AZ_GetPlayState(client->GetClient(), UReaProject::Get(projID));
}

bool URigdocksSilver_Project::AZ_GetRecordState(URadnodzClient* client, UReaProject* projID)
{
	return SILVER::AZ_GetRecordState(client->GetClient(), UReaProject::Get(projID));
}

bool URigdocksSilver_Project::AZ_GetPauseState(URadnodzClient* client, UReaProject* projID)
{
	return SILVER::AZ_GetPauseState(client->GetClient(), UReaProject::Get(projID));
}

bool URigdocksSilver_Project::AZ_GetStopState(URadnodzClient* client, UReaProject* projID)
{
	return SILVER::AZ_GetStopState(client->GetClient(), UReaProject::Get(projID));
}

double URigdocksSilver_Project::AZ_GetRecordingSampleRate(URadnodzClient* client, UReaProject* projID)
{
	return SILVER::AZ_GetRecordingSampleRate(client->GetClient(), UReaProject::Get(projID));
}

int URigdocksSilver_Project::AZ_GetRecordingBitDepth(URadnodzClient* client, UReaProject* projID)
{
	return SILVER::AZ_GetRecordingBitDepth(client->GetClient(), UReaProject::Get(projID));
}

FString URigdocksSilver_Project::AZ_GetProjectName(URadnodzClient* client, UReaProject* projID)
{
	std::string nameStr = SILVER::AZ_GetProjectName(client->GetClient(), UReaProject::Get(projID));

	return UTF8_TO_TCHAR(nameStr.c_str());
}

FString URigdocksSilver_Project::AZ_GetProjectPath(URadnodzClient* client, UReaProject* projID)
{
	std::string pathStr = SILVER::AZ_GetProjectPath(client->GetClient(), UReaProject::Get(projID));

	return UTF8_TO_TCHAR(pathStr.c_str());
}

UReaProject* URigdocksSilver_Project::AZ_OpenProject(URadnodzClient* client, FString projectPath)
{
	ReaProject* proj = SILVER::AZ_OpenProject(client->GetClient(), TCHAR_TO_UTF8(*projectPath));

	UReaProject* projObject = NewObject<UReaProject>();
	projObject->SetPointer((void*)proj);

	return projObject;
}

void URigdocksSilver_Project::AZ_SaveProject(URadnodzClient* client, UReaProject* projID, FString dstPath)
{
	SILVER::AZ_SaveProject(client->GetClient(), UReaProject::Get(projID), TCHAR_TO_UTF8(*dstPath));
}

void URigdocksSilver_Project::AZ_CloseProject(URadnodzClient* client, UReaProject* projID)
{
	SILVER::AZ_CloseProject(client->GetClient(), UReaProject::Get(projID));
}

void URigdocksSilver_Project::AZ_SwitchProject(URadnodzClient* client, UReaProject* projID)
{
	SILVER::AZ_SwitchProject(client->GetClient(), UReaProject::Get(projID));
}

UReaProject* URigdocksSilver_Project::AZ_GetProjectFromName(URadnodzClient* client, FString projectName)
{
	ReaProject* proj = SILVER::AZ_GetProjectFromName(client->GetClient(), TCHAR_TO_UTF8(*projectName));

	UReaProject* projObject = NewObject<UReaProject>();
	projObject->SetPointer((void*)proj);

	return projObject;
}

void URigdocksSilver_Project::AZ_LaunchReaper(URadnodzClient* client, FString projectPath)
{
	SILVER::AZ_LaunchReaper(client->GetClient(), TCHAR_TO_UTF8(*projectPath));
}

bool URigdocksSilver_Project::AZ_IsReaperRunning(URadnodzClient* client)
{
	return SILVER::AZ_IsReaperRunning(client->GetClient());
}

void URigdocksSilver_Project::AZ_QuitReaper(URadnodzClient* client, bool save)
{
	SILVER::AZ_QuitReaper(client->GetClient(), save);
}