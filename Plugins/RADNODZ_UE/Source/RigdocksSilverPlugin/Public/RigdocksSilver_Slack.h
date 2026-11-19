// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadnodzClient.h"
#include "ReaperClasses.h"
#include "DataClass/FSlackMessage.h"
#include "RigdocksSilver_Slack.generated.h"

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_Slack : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Slack",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FSlackMessage AZ_Slack_PostChannelMessage(URadnodzClient* client, FString token, FString channel, FString message, TArray<FString> mentionList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Slack",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FSlackMessage AZ_Slack_PostDirectMessage(URadnodzClient* client, FString token, FString toUser, FString message, bool useMension);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|Slack",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static FSlackMessage AZ_Slack_ReplayMessage(URadnodzClient* client, FString token, FString channel, FString threadTimeStamp, FString message, TArray<FString> mentionList);

};
