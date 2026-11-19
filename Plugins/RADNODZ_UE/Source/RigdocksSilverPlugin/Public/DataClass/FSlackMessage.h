// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/SlackMessage.h"
#include "FSlackMessage.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSSILVERPLUGIN_API FSlackMessage
{
public:

	GENERATED_BODY()
	
	void Initialize(SlackMessage& obj);

	SlackMessage GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|SlackMessage")
	FString Channel{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|SlackMessage")
	FString TimeStamp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|SlackMessage")
	FString ThreadTimeStamp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|SlackMessage")
	FString Message{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|SlackMessage")
	FString Error{};

};

UCLASS(BlueprintType)
class RIGDOCKSSILVERPLUGIN_API USlackMessageFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|SlackMessage")
	static bool FromJson(const FString& Json, FSlackMessage& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|SlackMessage")
	static bool FromJsonArray(const FString& Json, TArray<FSlackMessage>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|SlackMessage",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FSlackMessage& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|SlackMessage",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FSlackMessage>& objectArray, int indent = 4);

};
