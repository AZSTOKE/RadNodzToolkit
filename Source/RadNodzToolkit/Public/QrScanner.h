// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#pragma once

#include "CoreMinimal.h"

/** QRスキャンの状態（Java側のカメラプレビューの状態をそのまま写す）。 */
enum class EQrScanState : uint8
{
	/** 未開始／停止済み。 */
	Idle,

	/** カメラを開いている途中（プレビューはまだ出ていない）。 */
	Starting,

	/** プレビュー中。フレームを取り出してデコードできる。 */
	Scanning,

	/** プレビュー上の「キャンセル」で利用者が閉じた。 */
	Cancelled,

	/** カメラを開けなかった（権限なし／カメラ無し／その他の失敗）。 */
	Failed,
};

/**
 * QRコードをカメラで読み取るための C++側ファサード。
 *
 * 実体は Android の GameActivity（UPL の gameActivityClassAdditions）にある
 * camera2 のプレビューオーバーレイで、ここでは Thunk メソッドを JNI で呼ぶだけ。
 * デコードは行わず、プレビューと同時に流れてくる輝度（Y平面）を取り出して返す。
 * デコードそのものは URadnodzClient::DecodeQrGrayscale（zxing-cpp）が行う。
 *
 * 使い方：
 *   1. IsSupported() が false のプラットフォームでは何もしない（URL手入力のみ）。
 *   2. android.permission.CAMERA を取得済みにしてから Start()。
 *      （権限は UAndroidPermissionFunctionLibrary で呼び出し側が要求する）
 *   3. 毎フレーム GetState() を見つつ、Scanning のあいだ PollDecode() を呼ぶ。
 *      読めたら true が返るので Stop() で閉じる。
 *   4. Cancelled / Failed でも Stop() を呼んでよい（多重呼び出し可）。
 *
 * かざしている間は連続でデコードを試すので、シャッター操作は要らない。
 */
class RADNODZTOOLKIT_API FQrScanner
{
public:
	/** このプラットフォームでカメラ読み取りができるか（現状 Android のみ）。 */
	static bool IsSupported();

	/** カメラプレビューを開く（結果は非同期。GetState で確認する）。 */
	static void Start();

	/** プレビューを閉じてカメラを解放する（多重呼び出し可）。 */
	static void Stop();

	/** 現在の状態。 */
	static EQrScanState GetState();

	/**
	 * 最新フレームを1枚取り出してQRデコードを試す。
	 *
	 * 新しいフレームが無ければ何もせず false を返す（毎フレーム呼んでよい）。
	 * @param OutText  読み取れた文字列（接続用URL）
	 * @return QRコードを読み取れたか
	 */
	static bool PollDecode(FString& OutText);
};
