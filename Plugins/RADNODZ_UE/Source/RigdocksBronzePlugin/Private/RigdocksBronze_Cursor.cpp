// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksBronze_Cursor.h"
#include "RIGDOCKS_BRONZE.h"

double URigdocksBronze_Cursor::AZ_GetPlayCursorPosition(URadnodzClient* client, UReaProject* proj)
{
	return BRONZE::AZ_GetPlayCursorPosition(client->GetClient(), UReaProject::Get(proj));
}

double URigdocksBronze_Cursor::AZ_SetPlayCursorPosition(URadnodzClient* client, UReaProject* proj, int startEndType, double setPos)
{
	return BRONZE::AZ_SetPlayCursorPosition(client->GetClient(), UReaProject::Get(proj), startEndType, setPos);
}

int URigdocksBronze_Cursor::AZ_SetPlayCursorPositionFrame(URadnodzClient* client, UReaProject* proj, int startEndType, int setPosFrame)
{
	return BRONZE::AZ_SetPlayCursorPositionFrame(client->GetClient(), UReaProject::Get(proj), startEndType, setPosFrame);
}

double URigdocksBronze_Cursor::AZ_StartPlaying(URadnodzClient* client)
{
	return BRONZE::AZ_StartPlaying(client->GetClient());
}

double URigdocksBronze_Cursor::AZ_StartRecording(URadnodzClient* client)
{
	return BRONZE::AZ_StartRecording(client->GetClient());
}

double URigdocksBronze_Cursor::AZ_StopPlaying(URadnodzClient* client)
{
	return BRONZE::AZ_StopPlaying(client->GetClient());
}

double URigdocksBronze_Cursor::AZ_PausePlaying(URadnodzClient* client)
{
	return BRONZE::AZ_PausePlaying(client->GetClient());
}

double URigdocksBronze_Cursor::AZ_SetPlayCursorPositionAdd(URadnodzClient* client, UReaProject* projID, double addPos)
{
	return BRONZE::AZ_SetPlayCursorPositionAdd(client->GetClient(), UReaProject::Get(projID), addPos);
}

int URigdocksBronze_Cursor::AZ_SetPlayCursorPositionAddFrame(URadnodzClient* client, UReaProject* projID, int addPosFrame)
{
	return BRONZE::AZ_SetPlayCursorPositionAddFrame(client->GetClient(), UReaProject::Get(projID), addPosFrame);
}

double URigdocksBronze_Cursor::AZ_SetPlayCursorStart(URadnodzClient* client)
{
	return BRONZE::AZ_SetPlayCursorStart(client->GetClient());
}

double URigdocksBronze_Cursor::AZ_SetPlayCursorEnd(URadnodzClient* client)
{
	return BRONZE::AZ_SetPlayCursorEnd(client->GetClient());
}

