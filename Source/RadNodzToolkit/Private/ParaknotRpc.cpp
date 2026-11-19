// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ParaknotRpc.h"

#include "RadnodzClient.h"
#include "RpcCall.h"
#include "TcpClient.h"

#include "Dom/JsonValue.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace
{
	// JSON 文字列リテラルへ入れる前のエスケープ（引用符とバックスラッシュだけ）。
	FString EscapeJson(const FString& In)
	{
		FString Out = In;
		Out.ReplaceInline(TEXT("\\"), TEXT("\\\\"));
		Out.ReplaceInline(TEXT("\""), TEXT("\\\""));
		return Out;
	}

	/** AZ_PARAKNOT_* を 1 本呼んで retvals を配列で返す。 */
	bool Call(URadnodzClient* Client, const char* FunctionName, const FString& ArgsJson,
		TArray<TSharedPtr<FJsonValue>>& OutValues, FString& OutError)
	{
		OutValues.Reset();
		OutError.Reset();

		if (!Client)
		{
			OutError = TEXT("client is null");
			return false;
		}

		RADNODZ_client::RpcCall RpcCall;
		RpcCall.functionName = FunctionName;
		RpcCall.argsJson = TCHAR_TO_UTF8(*ArgsJson);

		if (!RADNODZ_client::Execute(Client->GetClient(), RpcCall))
		{
			OutError = TEXT("送信に失敗しました（RIGDOCKS_Bridge への接続を確認してください）");
			return false;
		}

		if (RpcCall.HasError())
		{
			OutError = FString::Printf(TEXT("%hs が失敗しました（エラーコード %d）"),
				FunctionName, RpcCall.ErrorCode());
			return false;
		}

		const FString Retvals = UTF8_TO_TCHAR(RpcCall.RetvalsJson().c_str());

		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Retvals);

		if (!FJsonSerializer::Deserialize(Reader, OutValues))
		{
			OutError = TEXT("戻り値を解析できませんでした");
			return false;
		}

		return true;
	}

	/** セッション発行 2 本の共通処理（引数も戻りも同じで、関数名だけが違う）。 */
	bool CallSession(URadnodzClient* Client, const char* FunctionName, const FString& InstanceId,
		FParaknotSession& OutSession, FString& OutError)
	{
		OutSession = FParaknotSession();

		if (InstanceId.IsEmpty())
		{
			OutError = TEXT("instanceId is empty");
			return false;
		}

		const FString ArgsJson = FString::Printf(TEXT("[\"%s\"]"), *EscapeJson(InstanceId));

		TArray<TSharedPtr<FJsonValue>> Values;

		if (!Call(Client, FunctionName, ArgsJson, Values, OutError))
		{
			return false;
		}

		// retvals = [port, sessionId, key]
		if (Values.Num() < 3)
		{
			OutError = TEXT("戻り値が足りません");
			return false;
		}

		OutSession.Port = (int32)Values[0]->AsNumber();
		OutSession.SessionId = Values[1]->AsString();
		OutSession.Key = Values[2]->AsString();

		if (OutSession.Port <= 0 || OutSession.Key.IsEmpty())
		{
			OutError = TEXT("セッションが得られませんでした（相手のモードを確認してください）");
			return false;
		}

		return true;
	}
}

namespace ParaknotRpc
{

bool ListInstances(URadnodzClient* Client, TArray<FParaknotInstance>& OutInstances, FString& OutError)
{
	OutInstances.Reset();

	TArray<TSharedPtr<FJsonValue>> Values;

	if (!Call(Client, "AZ_PARAKNOT_GetInstanceList", TEXT("[]"), Values, OutError))
	{
		return false;
	}

	// retvals = [ [ {InstanceId, Label, Role, Transport, ...}, ... ] ]
	if (Values.Num() < 1)
	{
		return true;   // 動いていない（空の一覧）
	}

	const TArray<TSharedPtr<FJsonValue>>* Items = nullptr;

	if (!Values[0]->TryGetArray(Items) || Items == nullptr)
	{
		return true;
	}

	for (const TSharedPtr<FJsonValue>& Item : *Items)
	{
		const TSharedPtr<FJsonObject>* Object = nullptr;

		if (!Item.IsValid() || !Item->TryGetObject(Object) || Object == nullptr) continue;

		FParaknotInstance Instance;

		// 項目が欠けていても落ちないよう Try 系で読む（RIGDOCKS 側で
		// 有効化されている項目しか入ってこない）。
		(*Object)->TryGetStringField(TEXT("InstanceId"), Instance.InstanceId);
		(*Object)->TryGetStringField(TEXT("Label"), Instance.Label);
		(*Object)->TryGetNumberField(TEXT("Role"), Instance.Role);
		(*Object)->TryGetNumberField(TEXT("Transport"), Instance.Transport);
		(*Object)->TryGetNumberField(TEXT("ListenPort"), Instance.ListenPort);
		(*Object)->TryGetNumberField(TEXT("MainChannelCount"), Instance.MainChannelCount);

		if (!Instance.InstanceId.IsEmpty())
		{
			OutInstances.Add(MoveTemp(Instance));
		}
	}

	return true;
}

bool SetRole(URadnodzClient* Client, const FString& InstanceId, int32 Role, int32 OutputMode,
	FString& OutError)
{
	if (InstanceId.IsEmpty())
	{
		OutError = TEXT("instanceId is empty");
		return false;
	}

	const FString ArgsJson = FString::Printf(TEXT("[\"%s\",%d,%d]"),
		*EscapeJson(InstanceId), Role, OutputMode);

	TArray<TSharedPtr<FJsonValue>> Values;

	if (!Call(Client, "AZ_PARAKNOT_SetRole", ArgsJson, Values, OutError))
	{
		return false;
	}

	// retvals = [bool]。false は「適用できなかった」。
	if (Values.Num() >= 1 && !Values[0]->AsBool())
	{
		OutError = TEXT("役割を変更できませんでした");
		return false;
	}

	return true;
}

bool OpenInputSession(URadnodzClient* Client, const FString& InstanceId,
	FParaknotSession& OutSession, FString& OutError)
{
	return CallSession(Client, "AZ_PARAKNOT_OpenInputSession", InstanceId, OutSession, OutError);
}

bool OpenOutputSession(URadnodzClient* Client, const FString& InstanceId,
	FParaknotSession& OutSession, FString& OutError)
{
	return CallSession(Client, "AZ_PARAKNOT_OpenOutputSession", InstanceId, OutSession, OutError);
}

} // namespace ParaknotRpc
