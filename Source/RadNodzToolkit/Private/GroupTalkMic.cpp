// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

#include "GroupTalkMic.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#endif

void FGroupTalkMic::StartPreview(float Gain)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jmethodID Method = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_StartGtMicPreview", "(F)V", false);
		if (Method)
		{
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, (jfloat)Gain);
		}
	}
#endif
}

void FGroupTalkMic::Stop()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jmethodID Method = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_StopGtMic", "()V", false);
		if (Method)
		{
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
		}
	}
#endif
}

void FGroupTalkMic::SetGain(float Gain)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jmethodID Method = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_SetGtMicGain", "(F)V", false);
		if (Method)
		{
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, (jfloat)Gain);
		}
	}
#endif
}

bool FGroupTalkMic::IsPreviewRunning()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jmethodID Method = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IsGtMicPreview", "()Z", false);
		if (Method)
		{
			return Env->CallBooleanMethod(FJavaWrapper::GameActivityThis, Method) == JNI_TRUE;
		}
	}
	return false;
#else
	return false;
#endif
}
