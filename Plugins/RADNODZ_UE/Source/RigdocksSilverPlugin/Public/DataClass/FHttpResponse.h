// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Object/HttpResponse.h"
#include "FHttpResponse.generated.h"

using namespace RADNODZ_client::Object;

USTRUCT(BlueprintType)
struct RIGDOCKSSILVERPLUGIN_API FHttpResponse
{
public:

	GENERATED_BODY()

	void Initialize(HttpResponse& obj);

	HttpResponse GetCppObj() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|HttpResponse")
	FString Body{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|HttpResponse")
	TMap<FString, FString> Headers{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RADNODZ|Object|HttpResponse")
	int StatusCode{};

};

UCLASS(BlueprintType)
class RIGDOCKSSILVERPLUGIN_API UHttpResponseFactory : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|HttpResponse")
	static bool FromJson(const FString& Json, FHttpResponse& object);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|HttpResponse")
	static bool FromJsonArray(const FString& Json, TArray<FHttpResponse>& objectArray);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|HttpResponse",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJson(const FHttpResponse& object, int indent = 4);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Object|HttpResponse",
		meta = (AdvancedDisplay = "indent")
	)
	static FString ToJsonArray(const TArray<FHttpResponse>& objectArray, int indent = 4);

};
