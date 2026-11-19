// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RadNodzAction.generated.h"

class UWG_RadNodzToolkit;
class URigdocksRpcCall;

/**
 * ホーム画面の「アクション」ドロップダウンから実行する処理の基底クラス。
 *
 * 新しいアクションはこのクラスを継承して Actions/ に置き、
 * UWG_RadNodzToolkit::RegisterActions() で一覧へ追加する。
 *
 * 実行の流れ:
 *   - 「実行」ボタン → Owner->BeginActionBusy() → Execute(Owner)
 *   - アクションは処理が終わったら（キャンセル・失敗を含め）必ず Owner->EndActionBusy() を呼ぶ。
 */
UCLASS(Abstract)
class RADNODZTOOLKIT_API URadNodzAction : public UObject
{
	GENERATED_BODY()

public:

	/** ドロップダウンに出す名前（現在の言語で返す）。 */
	virtual FString GetDisplayName() const PURE_VIRTUAL(URadNodzAction::GetDisplayName, return FString(););

	/** アクションを実行する（ゲームスレッドから呼ばれる）。 */
	virtual void Execute(UWG_RadNodzToolkit* Owner) PURE_VIRTUAL(URadNodzAction::Execute, );

protected:

	/**
	 * バッチ実行後の各呼び出しを先頭から調べ、最初に失敗したものを説明文にする。
	 * 失敗が無ければ true（OutError は空のまま）。
	 * 応答が無い呼び出し（通信失敗・前段の失敗で未実行）も失敗として扱う。
	 */
	static bool CheckBatchResult(const TArray<URigdocksRpcCall*>& Calls, FString& OutError);

	/** 失敗内容をダイアログで表示し、実行中状態を解除する。 */
	static void FinishWithError(UWG_RadNodzToolkit* Owner, const FString& Title, const FString& Message);
};
