// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RadnodzRpcBatch.h"
#include "RadnodzClient.h"
#include <string>
#include <vector>

// ---- URigdocksRpcCall ----------------------------------------------------

FString URigdocksRpcCall::GetFunctionName() const
{
	return UTF8_TO_TCHAR(Call.functionName.c_str());
}

FString URigdocksRpcCall::GetArgsJson() const
{
	return UTF8_TO_TCHAR(Call.argsJson.c_str());
}

FString URigdocksRpcCall::GetResultJson() const
{
	return UTF8_TO_TCHAR(Call.resultJson.c_str());
}

FString URigdocksRpcCall::GetRetvalsJson() const
{
	return UTF8_TO_TCHAR(Call.RetvalsJson().c_str());
}

bool URigdocksRpcCall::HasError() const
{
	return Call.HasError();
}

int32 URigdocksRpcCall::GetErrorCode() const
{
	return Call.ErrorCode();
}

void URigdocksRpcCall::BindRef(int32 ArgIndex, int32 FromCall, int32 RetvalIndex)
{
	Call.BindRef(ArgIndex, FromCall, RetvalIndex);
}

// ---- URadnodzRpcBatch ---------------------------------------------------

URadnodzRpcBatch* URadnodzRpcBatch::CreateRadnodzRpcBatch()
{
	return NewObject<URadnodzRpcBatch>();
}

int32 URadnodzRpcBatch::Add(URigdocksRpcCall* Call)
{
	return Calls.Add(Call);
}

int32 URadnodzRpcBatch::Num() const
{
	return Calls.Num();
}

void URadnodzRpcBatch::ClearBatch()
{
	Calls.Empty();
	FailedIndex = -1;
}

bool URadnodzRpcBatch::Execute(URadnodzClient* Client)
{
	FailedIndex = -1;

	if (Client == nullptr)
	{
		return false;
	}

	// Build the vector of RpcCall in queue order.
	std::vector<RADNODZ_client::RpcCall> vec;
	vec.reserve(Calls.Num());
	for (URigdocksRpcCall* c : Calls)
	{
		vec.push_back(c ? c->Get() : RADNODZ_client::RpcCall());
	}

	const bool bOk = RADNODZ_client::ExecuteBatch(Client->GetClient(), vec);

	for (int32 i = 0; i < Calls.Num() && i < static_cast<int32>(vec.size()); ++i)
	{
		if (Calls[i])
		{
			Calls[i]->Set(vec[i]);
		}
	}

	for (int32 i = 0; i < Calls.Num(); ++i)
	{
		if (Calls[i] && Calls[i]->GetErrorCode() > 0)
		{
			FailedIndex = i;
			break;
		}
	}

	return bOk;
}

int32 URadnodzRpcBatch::GetFailedIndex() const
{
	return FailedIndex;
}
