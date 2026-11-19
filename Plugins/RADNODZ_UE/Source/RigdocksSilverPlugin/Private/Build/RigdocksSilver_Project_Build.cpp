// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_Project_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FSection.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_SetProjectInfoRecPath_Build(UReaProject* projID, FString path)
{
	RpcCall c = SILVER::AZ_SetProjectInfoRecPath_Build(UReaProject::Get(projID), TCHAR_TO_UTF8(*path));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_SetProjectInfoRecAddPath_Build(UReaProject* projID, FString addPath)
{
	RpcCall c = SILVER::AZ_SetProjectInfoRecAddPath_Build(UReaProject::Get(projID), TCHAR_TO_UTF8(*addPath));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_SetProjectPathFolder_Build(FString folderName, int timeAddType)
{
	RpcCall c = SILVER::AZ_SetProjectPathFolder_Build(TCHAR_TO_UTF8(*folderName), timeAddType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_SetResoucePathFolder_Build(FString folderName, int timeAddType)
{
	RpcCall c = SILVER::AZ_SetResoucePathFolder_Build(TCHAR_TO_UTF8(*folderName), timeAddType);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_GetRepeat_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_GetRepeat_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_SetRepeate_Build(UReaProject* projID, bool repeat)
{
	RpcCall c = SILVER::AZ_SetRepeate_Build(UReaProject::Get(projID), repeat);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_GetPlayState_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_GetPlayState_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_GetRecordState_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_GetRecordState_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_GetPauseState_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_GetPauseState_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_GetStopState_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_GetStopState_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_GetRecordingSampleRate_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_GetRecordingSampleRate_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_GetRecordingBitDepth_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_GetRecordingBitDepth_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_GetProjectName_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_GetProjectName_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_GetProjectPath_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_GetProjectPath_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_OpenProject_Build(FString projectPath)
{
	RpcCall c = SILVER::AZ_OpenProject_Build(TCHAR_TO_UTF8(*projectPath));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_SaveProject_Build(UReaProject* projID, FString dstPath)
{
	RpcCall c = SILVER::AZ_SaveProject_Build(UReaProject::Get(projID), TCHAR_TO_UTF8(*dstPath));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_CloseProject_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_CloseProject_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_SwitchProject_Build(UReaProject* projID)
{
	RpcCall c = SILVER::AZ_SwitchProject_Build(UReaProject::Get(projID));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_GetProjectFromName_Build(FString projectName)
{
	RpcCall c = SILVER::AZ_GetProjectFromName_Build(TCHAR_TO_UTF8(*projectName));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_LaunchReaper_Build(FString projectPath)
{
	RpcCall c = SILVER::AZ_LaunchReaper_Build(TCHAR_TO_UTF8(*projectPath));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_IsReaperRunning_Build()
{
	RpcCall c = SILVER::AZ_IsReaperRunning_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_Project_Build::AZ_QuitReaper_Build(bool save)
{
	RpcCall c = SILVER::AZ_QuitReaper_Build(save);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
