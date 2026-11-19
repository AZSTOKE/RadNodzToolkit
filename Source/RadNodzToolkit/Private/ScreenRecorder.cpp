// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "ScreenRecorder.h"

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

void FScreenRecorder::RequestPermission()
{
#if PLATFORM_ANDROID
	CallVoidThunk("AndroidThunkJava_RequestScreenRecordPermission");
#endif
}

int32 FScreenRecorder::GetPermissionState()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jmethodID Method = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetScreenRecordPermissionState", "()I", false);
		if (Method)
		{
			return (int32)Env->CallIntMethod(FJavaWrapper::GameActivityThis, Method);
		}
	}
	return 0;
#else
	return -1;
#endif
}

void FScreenRecorder::BeginCapture(bool bIncludeMic)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jmethodID Method = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_BeginScreenCapture", "(Z)V", false);
		if (Method)
		{
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, (jboolean)(bIncludeMic ? JNI_TRUE : JNI_FALSE));
		}
	}
#endif
}

void FScreenRecorder::Stop()
{
#if PLATFORM_ANDROID
	CallVoidThunk("AndroidThunkJava_StopScreenRecording");
#endif
}

void FScreenRecorder::ShowRecordingOverlay()
{
#if PLATFORM_ANDROID
	CallVoidThunk("AndroidThunkJava_ShowRecordOverlay");
#endif
}

void FScreenRecorder::HideRecordingOverlay()
{
#if PLATFORM_ANDROID
	CallVoidThunk("AndroidThunkJava_HideRecordOverlay");
#endif
}

bool FScreenRecorder::IsRecording()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jmethodID Method = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IsScreenRecording", "()Z", false);
		if (Method)
		{
			return Env->CallBooleanMethod(FJavaWrapper::GameActivityThis, Method) != JNI_FALSE;
		}
	}
	return false;
#else
	return false;
#endif
}
