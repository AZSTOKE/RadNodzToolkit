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
#include "RigdocksSilver_Envelope_Build.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Envelope_Build : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Envelope|Build")
	static URigdocksRpcCall* AZ_SetMediaItemEnvelopeAllOFF_Build(UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Envelope|Build")
	static URigdocksRpcCall* AZ_SetMediaIdEnvelopeAllOFF_Build(UReaProject* projID, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Envelope|Build")
	static URigdocksRpcCall* AZ_SetSelectedMediaIdEnvelopeAllOFF_Build(UReaProject* projID, int selectItemID);
};
