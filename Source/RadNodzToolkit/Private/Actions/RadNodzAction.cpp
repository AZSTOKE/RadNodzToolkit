// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "Actions/RadNodzAction.h"
#include "WG_RadNodzToolkit.h"
#include "RadnodzRpcBatch.h"
#include "RigdocksError.h"   // AZ_ErrorCode（エラーコードの名前を表示に使う）
#include "ReaperLang.h"

namespace
{
	// 失敗したバッチの全呼び出しを出力する（原因調査用：$ref で渡った値がどうだったかを追えるよう、
	// 引数と戻り値をそのまま出す。実行されなかった呼び出しは結果が空になる）。
	void LogBatchCalls(const TArray<URigdocksRpcCall*>& Calls)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Action] batch failed (%d calls)"), Calls.Num());

		for (int32 i = 0; i < Calls.Num(); ++i)
		{
			const URigdocksRpcCall* Call = Calls[i];
			if (!Call)
			{
				UE_LOG(LogTemp, Warning, TEXT("[Action]   #%d (null)"), i);
				continue;
			}

			const bool bExecuted = !Call->GetResultJson().IsEmpty();
			UE_LOG(LogTemp, Warning, TEXT("[Action]   #%d %s %s err=%d args=%s retvals=%s"),
				i,
				*Call->GetFunctionName(),
				bExecuted ? TEXT("executed") : TEXT("NOT executed"),
				Call->GetErrorCode(),
				*Call->GetArgsJson(),
				bExecuted ? *Call->GetRetvalsJson() : TEXT("-"));

			// 失敗した呼び出しは応答全体（errorMessage / errorDetails を含む）も出す
			if (Call->HasError())
			{
				UE_LOG(LogTemp, Warning, TEXT("[Action]      result=%s"), *Call->GetResultJson());
			}
		}
	}
}

bool URadNodzAction::CheckBatchResult(const TArray<URigdocksRpcCall*>& Calls, FString& OutError)
{
	OutError.Reset();

	for (URigdocksRpcCall* Call : Calls)
	{
		if (!Call) { continue; }

		// 応答が無い＝送れなかった（通信失敗）
		if (Call->GetResultJson().IsEmpty())
		{
			OutError = ReaperLang::S(TEXT("REAPER との通信に失敗しました。"), TEXT("Failed to communicate with REAPER."))
				+ FString::Printf(TEXT("\n(%s)"), *Call->GetFunctionName());
			LogBatchCalls(Calls);
			return false;
		}

		if (Call->HasError())
		{
			const int32 Code = Call->GetErrorCode();

			FString CodeName;
			if (const UEnum* Enum = StaticEnum<AZ_ErrorCode>())
			{
				CodeName = Enum->GetNameStringByValue(Code);
			}

			OutError = FString::Printf(TEXT("%s\n%s (%s%d)"),
				*ReaperLang::S(TEXT("処理に失敗しました。"), TEXT("The operation failed.")),
				*Call->GetFunctionName(),
				CodeName.IsEmpty() ? TEXT("") : *(CodeName + TEXT(" / ")),
				Code);
			LogBatchCalls(Calls);
			return false;
		}
	}

	return true;
}

void URadNodzAction::FinishWithError(UWG_RadNodzToolkit* Owner, const FString& Title, const FString& Message)
{
	if (!Owner) { return; }

	Owner->ShowActionInfo(Title, Message);
	Owner->EndActionBusy();
}
