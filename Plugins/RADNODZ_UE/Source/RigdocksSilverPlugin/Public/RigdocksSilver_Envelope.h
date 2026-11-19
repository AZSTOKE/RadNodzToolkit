// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FSection.h"
#include "RigdocksSilver_Envelope.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Envelope : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Envelope",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	 	void AZ_SetMediaItemEnvelopeAllOFF(URadnodzClient* client, UMediaItem* item);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Envelope",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	 	void AZ_SetMediaIdEnvelopeAllOFF(URadnodzClient* client, UReaProject* projID, int itemID);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Envelope",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static 	 	void AZ_SetSelectedMediaIdEnvelopeAllOFF(URadnodzClient* client, UReaProject* projID, int selectItemID);

};
