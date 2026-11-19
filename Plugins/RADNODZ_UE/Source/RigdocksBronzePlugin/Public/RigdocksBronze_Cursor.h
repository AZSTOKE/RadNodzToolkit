// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "RigdocksBronze_Cursor.generated.h"

UCLASS()
class RIGDOCKSBRONZEPLUGIN_API URigdocksBronze_Cursor : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_GetPlayCursorPosition(URadnodzClient* client, UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_SetPlayCursorPosition(URadnodzClient* client, UReaProject* proj, int startEndType, double setPos);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_SetPlayCursorPositionFrame(URadnodzClient* client, UReaProject* proj, int startEndType, int setPosFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_StartPlaying(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_StartRecording(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_StopPlaying(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_PausePlaying(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_SetPlayCursorPositionAdd(URadnodzClient* client, UReaProject* projID, double addPos);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static int AZ_SetPlayCursorPositionAddFrame(URadnodzClient* client, UReaProject* projID, int addPosFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_SetPlayCursorStart(URadnodzClient* client);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static double AZ_SetPlayCursorEnd(URadnodzClient* client);

};
