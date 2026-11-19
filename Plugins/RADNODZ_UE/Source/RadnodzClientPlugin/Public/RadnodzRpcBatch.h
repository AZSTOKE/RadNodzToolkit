// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RpcCall.h"
#include "RadnodzRpcBatch.generated.h"

class URadnodzClient;

UCLASS(BlueprintType)
class RADNODZCLIENTPLUGIN_API URigdocksRpcCall : public UObject
{
	GENERATED_BODY()

public:
	RADNODZ_client::RpcCall& Get() { return Call; }
	void Set(const RADNODZ_client::RpcCall& InCall) { Call = InCall; }

	UFUNCTION(BlueprintPure, Category = "RADNODZ|Batch")
	FString GetFunctionName() const;

	UFUNCTION(BlueprintPure, Category = "RADNODZ|Batch")
	FString GetArgsJson() const;

	UFUNCTION(BlueprintPure, Category = "RADNODZ|Batch")
	FString GetResultJson() const;

	UFUNCTION(BlueprintPure, Category = "RADNODZ|Batch")
	FString GetRetvalsJson() const;

	UFUNCTION(BlueprintPure, Category = "RADNODZ|Batch")
	bool HasError() const;

	UFUNCTION(BlueprintPure, Category = "RADNODZ|Batch")
	int32 GetErrorCode() const;

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Batch")
	void BindRef(int32 ArgIndex, int32 FromCall, int32 RetvalIndex = 0);

private:
	RADNODZ_client::RpcCall Call;
};

UCLASS(BlueprintType)
class RADNODZCLIENTPLUGIN_API URadnodzRpcBatch : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Batch")
	static URadnodzRpcBatch* CreateRadnodzRpcBatch();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Batch")
	int32 Add(URigdocksRpcCall* Call);

	UFUNCTION(BlueprintPure, Category = "RADNODZ|Batch")
	int32 Num() const;

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Batch")
	void ClearBatch();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|Batch")
	bool Execute(URadnodzClient* Client);

	UFUNCTION(BlueprintPure, Category = "RADNODZ|Batch")
	int32 GetFailedIndex() const;

private:
	UPROPERTY()
	TArray<URigdocksRpcCall*> Calls;

	int32 FailedIndex = -1;
};
