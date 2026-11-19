// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "QrScanner.h"

#include "RadnodzClient.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#endif

#if PLATFORM_ANDROID
namespace
{
	// GameActivity の void Thunk を呼ぶ。
	void CallVoidThunk(const char* MethodName)
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID Method = FJavaWrapper::FindMethod(
				Env, FJavaWrapper::GameActivityClassID, MethodName, "()V", false);
			if (Method)
			{
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
			}
		}
	}
}
#endif

bool FQrScanner::IsSupported()
{
#if PLATFORM_ANDROID
	return true;
#else
	return false;
#endif
}

void FQrScanner::Start()
{
#if PLATFORM_ANDROID
	CallVoidThunk("AndroidThunkJava_StartQrScan");
#endif
}

void FQrScanner::Stop()
{
#if PLATFORM_ANDROID
	CallVoidThunk("AndroidThunkJava_StopQrScan");
#endif
}

EQrScanState FQrScanner::GetState()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jmethodID Method = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetQrScanState", "()I", false);
		if (Method)
		{
			// Java 側の数値：0=停止 / 1=起動中 / 2=プレビュー中 / -1=失敗 / -2=キャンセル
			switch ((int32)Env->CallIntMethod(FJavaWrapper::GameActivityThis, Method))
			{
			case 1:  return EQrScanState::Starting;
			case 2:  return EQrScanState::Scanning;
			case -1: return EQrScanState::Failed;
			case -2: return EQrScanState::Cancelled;
			default: return EQrScanState::Idle;
			}
		}
	}
#endif
	return EQrScanState::Idle;
}

bool FQrScanner::PollDecode(FString& OutText)
{
	OutText.Empty();

#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	if (!Env) { return false; }

	jmethodID Method = FJavaWrapper::FindMethod(
		Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_TakeQrFrame", "()[B", false);
	if (!Method) { return false; }

	jbyteArray FrameArray = (jbyteArray)Env->CallObjectMethod(FJavaWrapper::GameActivityThis, Method);
	if (!FrameArray) { return false; }   // 前回から新しいフレームが来ていない

	// 先頭8バイトが幅・高さ（ビッグエンディアンの int32）、以降が Y 平面（stride=幅で詰め直し済み）。
	// 幅/高さを別の Thunk で取ると、その間に次のフレームへ差し替わって食い違うため、
	// 1本の配列にまとめて受け取っている。
	const jsize Num = Env->GetArrayLength(FrameArray);

	TArray<uint8> Frame;
	Frame.SetNumUninitialized(Num);
	Env->GetByteArrayRegion(FrameArray, 0, Num, (jbyte*)Frame.GetData());
	Env->DeleteLocalRef(FrameArray);

	if (Num <= 8) { return false; }

	const int32 Width  = (Frame[0] << 24) | (Frame[1] << 16) | (Frame[2] << 8) | Frame[3];
	const int32 Height = (Frame[4] << 24) | (Frame[5] << 16) | (Frame[6] << 8) | Frame[7];

	if (Width <= 0 || Height <= 0 || (int64)Width * Height + 8 > (int64)Num)
	{
		return false;
	}

	FString Error;

	// デコードは RADNODZ クライアントライブラリ（zxing-cpp）。任意角度の回転・照明ムラにも耐える。
	// 読めなかった＝まだ画面内にコードが無いだけなので、エラーは表に出さない。
	return URadnodzClient::DecodeQrGrayscale(Frame.GetData() + 8, Width, Height, Width, OutText, Error);
#else
	return false;
#endif
}
