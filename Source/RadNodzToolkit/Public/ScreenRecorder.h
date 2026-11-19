// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"

/**
 * 画面録画（画面キャプチャ＋モニター再生音＋マイク送信音のステレオミックス→MP4→ギャラリー保存）の
 * C++側ファサード。実体は Android の前面サービス（RadNodzScreenRecordService）で、
 * ここでは GameActivity の Thunk メソッドを JNI で呼ぶだけ。
 *
 * 開始フロー：
 *   1. RequestPermission() … OSのMediaProjection許可ダイアログを表示。
 *   2. GetPermissionState() を毎フレーム確認（0:待機 / 1:許可 / -1:拒否）。
 *   3. 許可されたら（アプリ側で3・2・1カウントダウン後に）BeginCapture() で録画開始。
 *   4. Stop() で停止＝MP4を確定してギャラリーへ保存。
 *
 * 他プラットフォームでは各関数は何もしない（IsRecording は false、GetPermissionState は -1）。
 */
class RADNODZTOOLKIT_API FScreenRecorder
{
public:
	/** MediaProjection の許可ダイアログを表示する（結果は非同期。GetPermissionState で確認）。 */
	static void RequestPermission();

	/** 許可状態：0=待機/未取得, 1=許可, -1=拒否。 */
	static int32 GetPermissionState();

	/**
	 * 許可取得済みの状態で録画を開始する（前面サービスを起動）。
	 * @param bIncludeMic 録画にマイク音を含めるか（false=モニター再生音のみ）。
	 */
	static void BeginCapture(bool bIncludeMic);

	/** 録画を停止し、MP4を確定してギャラリーへ保存する。 */
	static void Stop();

	/** 現在録画中かどうか。 */
	static bool IsRecording();

	/**
	 * 録画中に「経過秒＋停止ボタン」を画面右上に表示する。
	 * これは FLAG_SECURE を付けた Android の別ウィンドウなので、
	 * MediaProjection の録画（非セキュアなVirtualDisplay）には映らない。
	 * 停止ボタンを押すと Stop() 相当が呼ばれる。
	 */
	static void ShowRecordingOverlay();

	/** 上記オーバーレイを消す（多重呼び出し可）。 */
	static void HideRecordingOverlay();
};
