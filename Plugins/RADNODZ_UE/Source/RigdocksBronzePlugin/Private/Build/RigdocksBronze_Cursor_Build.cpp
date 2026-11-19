// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksBronze_Cursor_Build.h"
#include "RIGDOCKS_BRONZE_Build.h"

using namespace RADNODZ_client;

URigdocksRpcCall* URigdocksBronze_Cursor_Build::AZ_GetPlayCursorPosition_Build(UReaProject* proj)
{
	RpcCall c = BRONZE::AZ_GetPlayCursorPosition_Build(UReaProject::Get(proj));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_Cursor_Build::AZ_SetPlayCursorPosition_Build(UReaProject* proj, int startEndType, double setPos)
{
	RpcCall c = BRONZE::AZ_SetPlayCursorPosition_Build(UReaProject::Get(proj), startEndType, setPos);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_Cursor_Build::AZ_SetPlayCursorPositionFrame_Build(UReaProject* proj, int startEndType, int setPosFrame)
{
	RpcCall c = BRONZE::AZ_SetPlayCursorPositionFrame_Build(UReaProject::Get(proj), startEndType, setPosFrame);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_Cursor_Build::AZ_StartPlaying_Build()
{
	RpcCall c = BRONZE::AZ_StartPlaying_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_Cursor_Build::AZ_StartRecording_Build()
{
	RpcCall c = BRONZE::AZ_StartRecording_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_Cursor_Build::AZ_StopPlaying_Build()
{
	RpcCall c = BRONZE::AZ_StopPlaying_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_Cursor_Build::AZ_PausePlaying_Build()
{
	RpcCall c = BRONZE::AZ_PausePlaying_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_Cursor_Build::AZ_SetPlayCursorPositionAdd_Build(UReaProject* projID, double addPos)
{
	RpcCall c = BRONZE::AZ_SetPlayCursorPositionAdd_Build(UReaProject::Get(projID), addPos);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_Cursor_Build::AZ_SetPlayCursorPositionAddFrame_Build(UReaProject* projID, int addPosFrame)
{
	RpcCall c = BRONZE::AZ_SetPlayCursorPositionAddFrame_Build(UReaProject::Get(projID), addPosFrame);

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_Cursor_Build::AZ_SetPlayCursorStart_Build()
{
	RpcCall c = BRONZE::AZ_SetPlayCursorStart_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksBronze_Cursor_Build::AZ_SetPlayCursorEnd_Build()
{
	RpcCall c = BRONZE::AZ_SetPlayCursorEnd_Build();

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
