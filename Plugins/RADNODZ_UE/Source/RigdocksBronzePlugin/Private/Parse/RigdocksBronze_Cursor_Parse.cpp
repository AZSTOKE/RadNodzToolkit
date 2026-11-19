// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksBronze_Cursor_Parse.h"
#include "RIGDOCKS_BRONZE_Parse.h"

using namespace RADNODZ_client;

double URigdocksBronze_Cursor_Parse::AZ_GetPlayCursorPosition_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	auto r = BRONZE::AZ_GetPlayCursorPosition_Parse(call->Get());
	return r;
}

double URigdocksBronze_Cursor_Parse::AZ_SetPlayCursorPosition_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	auto r = BRONZE::AZ_SetPlayCursorPosition_Parse(call->Get());
	return r;
}

int URigdocksBronze_Cursor_Parse::AZ_SetPlayCursorPositionFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	auto r = BRONZE::AZ_SetPlayCursorPositionFrame_Parse(call->Get());
	return r;
}

double URigdocksBronze_Cursor_Parse::AZ_StartPlaying_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	auto r = BRONZE::AZ_StartPlaying_Parse(call->Get());
	return r;
}

double URigdocksBronze_Cursor_Parse::AZ_StartRecording_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	auto r = BRONZE::AZ_StartRecording_Parse(call->Get());
	return r;
}

double URigdocksBronze_Cursor_Parse::AZ_StopPlaying_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	auto r = BRONZE::AZ_StopPlaying_Parse(call->Get());
	return r;
}

double URigdocksBronze_Cursor_Parse::AZ_PausePlaying_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	auto r = BRONZE::AZ_PausePlaying_Parse(call->Get());
	return r;
}

double URigdocksBronze_Cursor_Parse::AZ_SetPlayCursorPositionAdd_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	auto r = BRONZE::AZ_SetPlayCursorPositionAdd_Parse(call->Get());
	return r;
}

int URigdocksBronze_Cursor_Parse::AZ_SetPlayCursorPositionAddFrame_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0;
	}

	auto r = BRONZE::AZ_SetPlayCursorPositionAddFrame_Parse(call->Get());
	return r;
}

double URigdocksBronze_Cursor_Parse::AZ_SetPlayCursorStart_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	auto r = BRONZE::AZ_SetPlayCursorStart_Parse(call->Get());
	return r;
}

double URigdocksBronze_Cursor_Parse::AZ_SetPlayCursorEnd_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return 0.0;
	}

	auto r = BRONZE::AZ_SetPlayCursorEnd_Parse(call->Get());
	return r;
}
