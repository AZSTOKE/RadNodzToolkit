// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ParaknotSessionClient.h"

#include "ParaknotAudio.h"
#include "RadnodzClient.h"

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Policies/CondensedJsonPrintPolicy.h"

namespace
{
	// 通信仕様の版。サーバーはこれを見て受け付けるか決める。
	// RADNODZ クライアントライブラリ（RADNODZ_client::AbiVersion()）と同じ値にする。
	//
	// 食い違っても黙って壊れない: サーバーは
	// 「server supports abiVersion 1..1, client sent N」という errorDetails を
	// 返すので、下でそれをそのまま表示へ回している。
	constexpr int32 kAbiVersion = 1;

	// AZ_* を 1 本呼んで retvals を JSON 配列として返す。
	//
	// **RADNODZ のランク別 (BRONZE/SILVER/GOLD) のラッパーは通さない。** PARAKNOT は
	// ランクではないので専用のラッパーが無い。
	//
	// **RADNODZ_client の静的ライブラリも直接は使わない。** こちらのモジュールへ
	// 重ねてリンクすると TcpClient の実装がこの DLL にも複製され、ファイル内
	// static を持つ経路で食い違う。URadnodzClient::Send() は同じ接続を
	// 持ち主の DLL 側で動かしてくれるので、生の JSON を往復させるだけにする。
	bool CallApi(URadnodzClient* Client, const FString& FunctionName, const FString& ArgsJson,
		TArray<TSharedPtr<FJsonValue>>& OutRetvals, FString& OutError)
	{
		OutRetvals.Reset();
		OutError.Reset();

		if (!Client || !Client->IsConnected())
		{
			OutError = TEXT("RADNODZ へ接続されていません");
			return false;
		}

		const FString Request = FString::Printf(
			TEXT("{\"abiVersion\":%d,\"functionName\":\"%s\",\"args\":%s}"),
			kAbiVersion, *FunctionName, *ArgsJson);

		const FString Response = Client->Send(Request);

		if (Response.IsEmpty())
		{
			OutError = FString::Printf(TEXT("%s の応答がありません"), *FunctionName);
			return false;
		}

		TSharedPtr<FJsonObject> Root;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response);

		if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
		{
			OutError = FString::Printf(TEXT("%s の応答を解析できませんでした"), *FunctionName);
			return false;
		}

		bool bSuccess = false;

		if (!Root->TryGetBoolField(TEXT("success"), bSuccess) || !bSuccess)
		{
			// **errorDetails を優先する。** 具体的な理由はこちらに入る。
			FString Details;
			Root->TryGetStringField(TEXT("errorDetails"), Details);

			if (Details.IsEmpty())
			{
				Root->TryGetStringField(TEXT("errorMessage"), Details);
			}

			int32 Code = 0;
			Root->TryGetNumberField(TEXT("errorCode"), Code);

			OutError = Details.IsEmpty()
				? FString::Printf(TEXT("%s が失敗しました (errorCode=%d)"), *FunctionName, Code)
				: FString::Printf(TEXT("%s: %s"), *FunctionName, *Details);

			return false;
		}

		const TArray<TSharedPtr<FJsonValue>>* Retvals = nullptr;

		if (Root->TryGetArrayField(TEXT("retvals"), Retvals) && Retvals != nullptr)
		{
			OutRetvals = *Retvals;
		}

		return true;
	}

	// 引数の JSON 配列を組み立てる。文字列は必ずエスケープさせるため、手で書式化しない。
	FString MakeArgsJson(const TArray<TSharedPtr<FJsonValue>>& Values)
	{
		FString Out;
		const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Out);
		FJsonSerializer::Serialize(Values, Writer);
		return Out;
	}
}

bool UParaknotSessionClient::ListInstances(URadnodzClient* Client,
	TArray<FParaknotInstanceInfo>& OutList, FString& OutError)
{
	OutList.Reset();

	TArray<TSharedPtr<FJsonValue>> Retvals;

	if (!CallApi(Client, TEXT("AZ_PARAKNOT_GetInstanceList"), TEXT("[]"), Retvals, OutError))
	{
		return false;
	}

	// retvals[0] が一覧の配列。
	if (Retvals.Num() < 1)
	{
		// PARAKNOT が 1 つも動いていないと空で返る。異常ではない。
		return true;
	}

	const TArray<TSharedPtr<FJsonValue>>* Items = nullptr;

	if (!Retvals[0]->TryGetArray(Items) || Items == nullptr)
	{
		OutError = TEXT("インスタンス一覧の形式が想定と違います");
		return false;
	}

	for (const TSharedPtr<FJsonValue>& Item : *Items)
	{
		const TSharedPtr<FJsonObject>* Object = nullptr;

		if (!Item.IsValid() || !Item->TryGetObject(Object) || Object == nullptr)
		{
			continue;
		}

		FParaknotInstanceInfo Info;

		(*Object)->TryGetStringField(TEXT("InstanceId"), Info.InstanceId);
		(*Object)->TryGetStringField(TEXT("Label"), Info.Label);
		(*Object)->TryGetNumberField(TEXT("Role"), Info.Role);
		(*Object)->TryGetNumberField(TEXT("ListenPort"), Info.ListenPort);
		(*Object)->TryGetNumberField(TEXT("MainChannelCount"), Info.MainChannelCount);

		if (!Info.InstanceId.IsEmpty())
		{
			OutList.Add(MoveTemp(Info));
		}
	}

	return true;
}

bool UParaknotSessionClient::SetBroadcastMode(URadnodzClient* Client, const FString& InstanceId,
	FString& OutError)
{
	if (InstanceId.IsEmpty())
	{
		OutError = TEXT("インスタンスが指定されていません");
		return false;
	}

	// 引数は (instanceId, role, outputMode)。role 0 = 送信、
	// 待受との組み合わせで「配信」になる。outputMode は 0 (そのまま) のまま。
	const FString Args = MakeArgsJson({
		MakeShared<FJsonValueString>(InstanceId),
		MakeShared<FJsonValueNumber>(0),
		MakeShared<FJsonValueNumber>(0) });

	TArray<TSharedPtr<FJsonValue>> Retvals;

	return CallApi(Client, TEXT("AZ_PARAKNOT_SetRole"), Args, Retvals, OutError);
}

bool UParaknotSessionClient::OpenBroadcastSession(URadnodzClient* Client,
	const FString& InstanceId, const FString& Host, FParaknotSessionInfo& OutSession,
	FString& OutError)
{
	OutSession = FParaknotSessionInfo();

	if (InstanceId.IsEmpty())
	{
		OutError = TEXT("インスタンスが指定されていません");
		return false;
	}

	if (Host.IsEmpty())
	{
		OutError = TEXT("接続先 IP が空です");
		return false;
	}

	const FString Args = MakeArgsJson({ MakeShared<FJsonValueString>(InstanceId) });

	TArray<TSharedPtr<FJsonValue>> Retvals;

	if (!CallApi(Client, TEXT("AZ_PARAKNOT_OpenOutputSession"), Args, Retvals, OutError))
	{
		return false;
	}

	// retvals = [port, sessionId, key]
	if (Retvals.Num() < 3)
	{
		OutError = TEXT("セッションの中身が足りません");
		return false;
	}

	OutSession.Host = Host;
	OutSession.Port = static_cast<int32>(Retvals[0]->AsNumber());
	OutSession.SessionId = Retvals[1]->AsString();
	OutSession.SessionKeyBase64 = Retvals[2]->AsString();

	if (!OutSession.IsValid())
	{
		OutError = TEXT("セッションの中身が足りません");
		return false;
	}

	return true;
}

bool UParaknotSessionClient::ConnectToBroadcast(URadnodzClient* Client, const FString& Host,
	const FString& PreferredInstanceId, FParaknotSessionInfo& OutSession, FString& OutError)
{
	TArray<FParaknotInstanceInfo> Instances;

	if (!ListInstances(Client, Instances, OutError))
	{
		return false;
	}

	if (Instances.Num() == 0)
	{
		OutError = TEXT("PARAKNOT が見つかりません。REAPER に挿してください。");
		return false;
	}

	const FParaknotInstanceInfo* Target = nullptr;

	// --- 名指しがあればそれを探す ---
	if (!PreferredInstanceId.IsEmpty())
	{
		for (const FParaknotInstanceInfo& Instance : Instances)
		{
			if (Instance.InstanceId == PreferredInstanceId)
			{
				Target = &Instance;
				break;
			}
		}

		if (Target == nullptr)
		{
			// 指定のインスタンスが消えている (REAPER を開き直した等)。
			// **黙って別のものを聞かせない**ための記録を残して自動選択へ落とす。
			UE_LOG(LogParaknotAudio, Warning,
				TEXT("指定の PARAKNOT (%s) が見つかりません。自動選択に切り替えます。"),
				*PreferredInstanceId);
		}
		else if (!Target->IsBroadcasting())
		{
			// 名指しされているので配信へ変えてよい。
			if (!SetBroadcastMode(Client, Target->InstanceId, OutError))
			{
				return false;
			}
		}
	}

	// --- 自動選択 ---
	if (Target == nullptr)
	{
		// **配信中のものを優先する。** 受信にしているインスタンスから鍵を取っても
		// 音は来ない (サーバー側が「送信で待ち受けていない」と弾く)。
		for (const FParaknotInstanceInfo& Instance : Instances)
		{
			if (Instance.IsBroadcasting())
			{
				Target = &Instance;
				break;
			}
		}
	}

	if (Target == nullptr)
	{
		// 配信中が 1 つも無い。
		//
		// **勝手に切り替えるのは 1 つしか無いときだけ。** 複数あるのに先頭を
		// 配信へ変えると、聞きたくないトラックのインスタンスの役割を
		// 黙って書き換えることになる。
		if (Instances.Num() != 1)
		{
			OutError = TEXT("配信中の PARAKNOT がありません。設定で配信元を選ぶか、REAPER 側で配信にしてください。");
			return false;
		}

		Target = &Instances[0];

		UE_LOG(LogParaknotAudio, Log,
			TEXT("配信中の PARAKNOT が無いので %s を配信へ切り替えます"), *Target->InstanceId);

		if (!SetBroadcastMode(Client, Target->InstanceId, OutError))
		{
			return false;
		}
	}

	return OpenBroadcastSession(Client, Target->InstanceId, Host, OutSession, OutError);
}
