// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"

/**
 * バックグラウンド（画面OFF / 他アプリ表示）でもモニター再生・マイク送信・
 * グループトークの音声を継続させるためのキープアライブ。
 *
 * Androidでの動作：
 *  - SetActive(true) で前面サービス（通知常駐＋WakeLock/WifiLock）を開始し、
 *    OSによるプロセス凍結・マイク遮断・WiFiスリープを防ぐ。
 *  - 監視スレッドがバックグラウンド中に以下を行う：
 *      1. エンジンがバックグラウンド移行時に一時停止した音声デバイスを再開
 *         （OpenSLの再生再開。音声のミックス/再生はゲームスレッド非依存で続く）。
 *      2. 登録されたメンテナンス関数（グループトークの在席ハートビート等）を定期実行。
 *         ゲームスレッドが停止している間に監視スレッドから呼ばれる。
 *      3. ゲームスレッドを短時間だけ動かす「ポンプ」を定期実行し、
 *         新規参加者の再生セットアップ（AsyncTask/GameThread）等を処理させる。
 *
 * 他プラットフォームでは SetActive は何もしない（無害）。
 */
class RADNODZTOOLKIT_API FReaperKeepAlive
{
public:
	/**
	 * キープアライブの有効/無効を切り替える（重複呼び出しは内部で吸収する）。
	 * モニター受信・マイク送信・グループトークのいずれかが動作中なら true にする。
	 * ゲームスレッドから呼ぶこと。
	 */
	static void SetActive(bool bInActive);

	/**
	 * バックグラウンド中に定期実行するメンテナンス関数を登録する（1つのみ）。
	 * ★監視スレッドから呼ばれる★ため、UI(Slate/UMG)やUObject生成を行ってはならない。
	 * 解除は nullptr を渡す。
	 */
	static void SetBackgroundMaintenance(TFunction<void()> InFunc);

	/** アプリが現在バックグラウンド（サスペンド中）かどうか。 */
	static bool IsInBackground();
};
