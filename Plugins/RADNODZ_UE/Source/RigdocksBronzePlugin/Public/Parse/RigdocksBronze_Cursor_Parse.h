// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"

#include "RigdocksBronze_Cursor_Parse.generated.h"

UCLASS()
class RIGDOCKSBRONZEPLUGIN_API URigdocksBronze_Cursor_Parse : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Parse")
	static double AZ_GetPlayCursorPosition_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Parse")
	static double AZ_SetPlayCursorPosition_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Parse")
	static int AZ_SetPlayCursorPositionFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Parse")
	static double AZ_StartPlaying_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Parse")
	static double AZ_StartRecording_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Parse")
	static double AZ_StopPlaying_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Parse")
	static double AZ_PausePlaying_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Parse")
	static double AZ_SetPlayCursorPositionAdd_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Parse")
	static int AZ_SetPlayCursorPositionAddFrame_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Parse")
	static double AZ_SetPlayCursorStart_Parse(URigdocksRpcCall* call);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Parse")
	static double AZ_SetPlayCursorEnd_Parse(URigdocksRpcCall* call);
};
