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
#include "RigdocksSilver_UI.generated.h"

UENUM(BlueprintType)
enum class AZ_WindowType : uint8
{
	OK = 0,

	OK_CANCEL = 1,

	ABORT_RETRY_IGNORE = 2,

	YES_NO_CANCEL = 3,

	YES_NO = 4,

	RETRY_CANCEL = 5,

	CANCEL_TRY_CONTINUE = 6,
};

UENUM(BlueprintType)
enum class AZ_ButtonID : uint8
{
	None = 0 UMETA(Hidden),

	OK = 1,

	Cancel = 2,

	Abort = 3,

	Retry = 4,

	Ignore = 5,

	Yes = 6,

	No = 7,

	Try = 10,

	Continue = 11,
};

UCLASS()
class RIGDOCKSSILVERPLUGIN_API URigdocksSilver_UI : public URadnodzCommonClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_ShowMessageBox_Ex(URadnodzClient* client, FString messageStr, FString windowTitleStr, AZ_WindowType windowType, AZ_ButtonID& pushedButtonId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_GetUserInputList(URadnodzClient* client, FString titleStr, int inputNum, FString captionStr, FString resultStr, TArray<FString>& inputList);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_ShowMessageBox_Info(URadnodzClient* client, FString message, FString title, AZ_WindowType windowType, AZ_ButtonID& pushedButtonId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_ShowMessageBox_Warn(URadnodzClient* client, FString message, FString title, AZ_WindowType windowType, AZ_ButtonID& pushedButtonId);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|SILVER|UI",
		meta = (BlueprintInternalUseOnly = "true", RequiredPins = "client"))
	static bool AZ_ShowMessageBox_Error(URadnodzClient* client, FString message, FString title, AZ_WindowType windowType, AZ_ButtonID& pushedButtonId);

};
