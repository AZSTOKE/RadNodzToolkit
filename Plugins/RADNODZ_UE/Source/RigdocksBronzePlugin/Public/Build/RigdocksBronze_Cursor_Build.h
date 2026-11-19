// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "RadnodzRpcBatch.h"
#include "ReaperClasses.h"
#include "RigdocksBronze_Cursor_Build.generated.h"

UCLASS()
class RIGDOCKSBRONZEPLUGIN_API URigdocksBronze_Cursor_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Build")
	static URigdocksRpcCall* AZ_GetPlayCursorPosition_Build(UReaProject* proj);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Build")
	static URigdocksRpcCall* AZ_SetPlayCursorPosition_Build(UReaProject* proj, int startEndType, double setPos);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Build")
	static URigdocksRpcCall* AZ_SetPlayCursorPositionFrame_Build(UReaProject* proj, int startEndType, int setPosFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Build")
	static URigdocksRpcCall* AZ_StartPlaying_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Build")
	static URigdocksRpcCall* AZ_StartRecording_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Build")
	static URigdocksRpcCall* AZ_StopPlaying_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Build")
	static URigdocksRpcCall* AZ_PausePlaying_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Build")
	static URigdocksRpcCall* AZ_SetPlayCursorPositionAdd_Build(UReaProject* projID, double addPos);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Build")
	static URigdocksRpcCall* AZ_SetPlayCursorPositionAddFrame_Build(UReaProject* projID, int addPosFrame);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Build")
	static URigdocksRpcCall* AZ_SetPlayCursorStart_Build();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|BRONZE|Cursor|Build")
	static URigdocksRpcCall* AZ_SetPlayCursorEnd_Build();
};
