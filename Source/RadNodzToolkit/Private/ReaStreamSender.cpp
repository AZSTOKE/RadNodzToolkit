// Fill out your copyright notice in the Description page of Project Settings.

#include "ReaStreamSender.h"

#include "Common/UdpSocketBuilder.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"

namespace
{
	// android.media.AudioManager の定数
	constexpr int32 ANDROID_MODE_NORMAL          = 0;
	constexpr int32 ANDROID_MODE_IN_COMMUNICATION = 3;

	// アクティビティから AudioManager(jobject) を取得する。失敗時 nullptr。
	jobject GetAndroidAudioManager(JNIEnv* Env)
	{
		if (!Env || !FJavaWrapper::GameActivityThis) return nullptr;

		jclass ContextClass = Env->FindClass("android/content/Context");
		if (!ContextClass) { Env->ExceptionClear(); return nullptr; }

		jfieldID AudioServiceField = Env->GetStaticFieldID(ContextClass, "AUDIO_SERVICE", "Ljava/lang/String;");
		jstring AudioServiceName = AudioServiceField ? (jstring)Env->GetStaticObjectField(ContextClass, AudioServiceField) : nullptr;

		jclass ActivityClass = Env->GetObjectClass(FJavaWrapper::GameActivityThis);
		jmethodID GetSystemService = ActivityClass ? Env->GetMethodID(ActivityClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;") : nullptr;

		jobject AudioManager = nullptr;
		if (GetSystemService && AudioServiceName)
		{
			AudioManager = Env->CallObjectMethod(FJavaWrapper::GameActivityThis, GetSystemService, AudioServiceName);
		}

		if (AudioServiceName) Env->DeleteLocalRef(AudioServiceName);
		if (ContextClass)     Env->DeleteLocalRef(ContextClass);
		if (ActivityClass)    Env->DeleteLocalRef(ActivityClass);

		if (Env->ExceptionCheck()) { Env->ExceptionDescribe(); Env->ExceptionClear(); }
		return AudioManager;
	}

	// android.media.AudioDeviceInfo.TYPE_BLUETOOTH_SCO
	constexpr int32 ANDROID_TYPE_BLUETOOTH_SCO = 7;

	// 端末のAPIレベル(Build.VERSION.SDK_INT)を返す。
	int32 GetAndroidSdkInt(JNIEnv* Env)
	{
		if (!Env) return 0;
		jclass VC = Env->FindClass("android/os/Build$VERSION");
		if (!VC) { Env->ExceptionClear(); return 0; }
		jfieldID F = Env->GetStaticFieldID(VC, "SDK_INT", "I");
		const int32 V = F ? (int32)Env->GetStaticIntField(VC, F) : 0;
		Env->DeleteLocalRef(VC);
		return V;
	}

	// 通信用デバイス一覧から Bluetooth SCO の AudioDeviceInfo を探して返す（要 DeleteLocalRef）。
	// API31+ のみ。見つからなければ nullptr。
	jobject FindBluetoothScoCommDevice(JNIEnv* Env, jobject AudioManager)
	{
		if (!Env || !AudioManager) return nullptr;

		jclass AMC = Env->GetObjectClass(AudioManager);
		jmethodID GetList = AMC ? Env->GetMethodID(AMC, "getAvailableCommunicationDevices", "()Ljava/util/List;") : nullptr;
		if (AMC) Env->DeleteLocalRef(AMC);
		if (!GetList) { Env->ExceptionClear(); return nullptr; }

		jobject List = Env->CallObjectMethod(AudioManager, GetList);
		if (!List) { if (Env->ExceptionCheck()) Env->ExceptionClear(); return nullptr; }

		jclass ListC = Env->FindClass("java/util/List");
		jclass ADIC  = Env->FindClass("android/media/AudioDeviceInfo");
		jmethodID Size = ListC ? Env->GetMethodID(ListC, "size", "()I") : nullptr;
		jmethodID Get  = ListC ? Env->GetMethodID(ListC, "get", "(I)Ljava/lang/Object;") : nullptr;
		jmethodID GetType = ADIC ? Env->GetMethodID(ADIC, "getType", "()I") : nullptr;

		jobject Found = nullptr;
		if (Size && Get && GetType)
		{
			const jint N = Env->CallIntMethod(List, Size);
			for (jint i = 0; i < N; ++i)
			{
				jobject Dev = Env->CallObjectMethod(List, Get, i);
				if (!Dev) continue;
				const jint Type = Env->CallIntMethod(Dev, GetType);
				if (Type == ANDROID_TYPE_BLUETOOTH_SCO) { Found = Dev; break; }
				Env->DeleteLocalRef(Dev);
			}
		}

		if (ADIC)  Env->DeleteLocalRef(ADIC);
		if (ListC) Env->DeleteLocalRef(ListC);
		Env->DeleteLocalRef(List);
		if (Env->ExceptionCheck()) { Env->ExceptionClear(); }
		return Found;
	}

	// 入力経路を切り替える。API31+ は setCommunicationDevice、それ未満は従来の startBluetoothSco。
	void SetAndroidBluetoothRouting(bool bEnable)
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		if (!Env) return;

		jobject AudioManager = GetAndroidAudioManager(Env);
		if (!AudioManager) return;

		jclass AMClass = Env->GetObjectClass(AudioManager);
		if (AMClass)
		{
			jmethodID SetMode = Env->GetMethodID(AMClass, "setMode", "(I)V");
			const int32 Sdk = GetAndroidSdkInt(Env);

			if (Sdk >= 31)
			{
				jmethodID SetCommDevice   = Env->GetMethodID(AMClass, "setCommunicationDevice", "(Landroid/media/AudioDeviceInfo;)Z");
				jmethodID ClearCommDevice = Env->GetMethodID(AMClass, "clearCommunicationDevice", "()V");

				if (bEnable)
				{
					if (SetMode) Env->CallVoidMethod(AudioManager, SetMode, (jint)ANDROID_MODE_IN_COMMUNICATION);
					jobject Dev = FindBluetoothScoCommDevice(Env, AudioManager);
					if (Dev && SetCommDevice)
					{
						const jboolean Ok = Env->CallBooleanMethod(AudioManager, SetCommDevice, Dev);
						UE_LOG(LogTemp, Warning, TEXT("[RIGDRED Mic] setCommunicationDevice(BT_SCO)=%d"), (int32)Ok);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("[RIGDRED Mic] Bluetooth通信デバイスが見つかりません（未接続/権限?）"));
					}
					if (Dev) Env->DeleteLocalRef(Dev);
				}
				else
				{
					if (ClearCommDevice) Env->CallVoidMethod(AudioManager, ClearCommDevice);
					if (SetMode) Env->CallVoidMethod(AudioManager, SetMode, (jint)ANDROID_MODE_NORMAL);
				}
			}
			else
			{
				// 旧API（〜API30）: startBluetoothSco（非同期）
				jmethodID StartSco = Env->GetMethodID(AMClass, "startBluetoothSco", "()V");
				jmethodID StopSco  = Env->GetMethodID(AMClass, "stopBluetoothSco", "()V");
				jmethodID SetScoOn = Env->GetMethodID(AMClass, "setBluetoothScoOn", "(Z)V");
				if (bEnable)
				{
					if (SetMode)  Env->CallVoidMethod(AudioManager, SetMode, (jint)ANDROID_MODE_IN_COMMUNICATION);
					if (StartSco) Env->CallVoidMethod(AudioManager, StartSco);
					if (SetScoOn) Env->CallVoidMethod(AudioManager, SetScoOn, JNI_TRUE);
				}
				else
				{
					if (SetScoOn) Env->CallVoidMethod(AudioManager, SetScoOn, JNI_FALSE);
					if (StopSco)  Env->CallVoidMethod(AudioManager, StopSco);
					if (SetMode)  Env->CallVoidMethod(AudioManager, SetMode, (jint)ANDROID_MODE_NORMAL);
				}
			}

			Env->DeleteLocalRef(AMClass);
		}

		Env->DeleteLocalRef(AudioManager);
		if (Env->ExceptionCheck()) { Env->ExceptionDescribe(); Env->ExceptionClear(); }
	}

	// 接続中のBluetooth SCO入力デバイス名を返す（API31+）。無ければ空。
	FString GetAndroidBluetoothInputName()
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		if (!Env) return FString();

		jobject AudioManager = GetAndroidAudioManager(Env);
		if (!AudioManager) return FString();

		FString Result;
		if (GetAndroidSdkInt(Env) >= 31)
		{
			jobject Dev = FindBluetoothScoCommDevice(Env, AudioManager);
			if (Dev)
			{
				jclass ADIC = Env->GetObjectClass(Dev);
				jmethodID GetProductName = ADIC ? Env->GetMethodID(ADIC, "getProductName", "()Ljava/lang/CharSequence;") : nullptr;
				if (GetProductName)
				{
					jobject CharSeq = Env->CallObjectMethod(Dev, GetProductName);
					if (CharSeq)
					{
						jclass CSC = Env->GetObjectClass(CharSeq);
						jmethodID ToString = CSC ? Env->GetMethodID(CSC, "toString", "()Ljava/lang/String;") : nullptr;
						jstring JStr = ToString ? (jstring)Env->CallObjectMethod(CharSeq, ToString) : nullptr;
						if (JStr)
						{
							const char* Chars = Env->GetStringUTFChars(JStr, nullptr);
							if (Chars) { Result = FString(UTF8_TO_TCHAR(Chars)); Env->ReleaseStringUTFChars(JStr, Chars); }
							Env->DeleteLocalRef(JStr);
						}
						if (CSC) Env->DeleteLocalRef(CSC);
						Env->DeleteLocalRef(CharSeq);
					}
				}
				if (ADIC) Env->DeleteLocalRef(ADIC);
				Env->DeleteLocalRef(Dev);
			}
		}

		Env->DeleteLocalRef(AudioManager);
		if (Env->ExceptionCheck()) { Env->ExceptionClear(); }
		return Result;
	}
}
#endif // PLATFORM_ANDROID

bool UReaStreamSender::Start(const FString& TargetIP, int32 InPort, EMicInputMode InMode)
{
	if (bActive) return true;

	Port = InPort;
	InputMode = InMode;

	// マイクをキャプチャする前にOS側のルーティングを切り替える（Android: Bluetooth SCO）
	ApplyAudioRouting(InputMode);

#if PLATFORM_ANDROID
	// BluetoothはsetCommunicationDevice()が成功しても、実際のSCO音声リンク確立には
	// 物理的に数百ms〜1秒ほどかかる。ここで待たずに録音ストリームを開くと、リンクが
	// まだ繋がっていない状態で開始してしまい無音を拾い続けることがあるため、少し待つ。
	if (InMode == EMicInputMode::Bluetooth)
	{
		FPlatformProcess::Sleep(0.8f);
	}
#endif

	ISocketSubsystem* SS = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (!SS) return false;

	// 送信先アドレス
	TSharedPtr<FInternetAddr> Addr = SS->CreateInternetAddr();
	bool bValidIp = false;
	Addr->SetIp(*TargetIP, bValidIp);
	Addr->SetPort(Port);
	if (!bValidIp)
	{
		UE_LOG(LogTemp, Warning, TEXT("[RIGDRED Mic] 送信先IPが不正: %s"), *TargetIP);
		return false;
	}
	{
		FScopeLock Lock(&RemoteAddrsLock);
		RemoteAddrs.Reset();
		RemoteAddrs.Add(Addr);
	}

	// 送信用UDPソケット
	Socket = FUdpSocketBuilder(TEXT("ReaStreamSendSocket")).AsReusable().WithSendBufferSize(1 << 20);
	if (!Socket)
	{
		UE_LOG(LogTemp, Warning, TEXT("[RIGDRED Mic] 送信ソケット作成失敗"));
		return false;
	}

	// マイク入力をキャプチャ
	Audio::FAudioCaptureDeviceParams Params; // 既定デバイス
	Audio::FOnAudioCaptureFunction OnCap =
		[this](const void* InAudio, int32 NumFrames, int32 NumChannels, int32 SampleRate, double /*StreamTime*/, bool /*bOverflow*/)
	{
		OnAudioCapture(static_cast<const float*>(InAudio), NumFrames, NumChannels, SampleRate);
	};

	if (!AudioCapture.OpenAudioCaptureStream(Params, MoveTemp(OnCap), 1024))
	{
		UE_LOG(LogTemp, Warning, TEXT("[RIGDRED Mic] OpenAudioCaptureStream 失敗（マイク権限/デバイス確認）"));
		Stop();
		return false;
	}
	if (!AudioCapture.StartStream())
	{
		UE_LOG(LogTemp, Warning, TEXT("[RIGDRED Mic] StartStream 失敗"));
		Stop();
		return false;
	}

	bActive = true;
	UE_LOG(LogTemp, Warning, TEXT("[RIGDRED Mic] 送信開始 → %s:%d"), *TargetIP, Port);
	return true;
}

void UReaStreamSender::SetGroupTargets(const TArray<FString>& InTargetIPs)
{
	SetGroupTargets(InTargetIPs, Port);
}

void UReaStreamSender::SetGroupTargets(const TArray<FString>& InTargetIPs, int32 InPort)
{
	// 宛先ポートを更新（以降に構築するアドレスへ反映される）。
	Port = InPort;

	ISocketSubsystem* SS = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (!SS) return;

	TArray<TSharedPtr<FInternetAddr>> NewAddrs;
	NewAddrs.Reserve(InTargetIPs.Num());
	for (const FString& IP : InTargetIPs)
	{
		if (IP.IsEmpty()) continue;
		TSharedPtr<FInternetAddr> Addr = SS->CreateInternetAddr();
		bool bValidIp = false;
		Addr->SetIp(*IP, bValidIp);
		Addr->SetPort(Port);
		if (bValidIp)
		{
			NewAddrs.Add(Addr);
		}
	}

	FScopeLock Lock(&RemoteAddrsLock);
	RemoteAddrs = MoveTemp(NewAddrs);
}

void UReaStreamSender::Stop()
{
	bActive = false;

	// 先にキャプチャを止めてコールバックを停止 → その後ソケットを閉じる
	AudioCapture.StopStream();
	AudioCapture.CloseStream();

	if (Socket)
	{
		Socket->Close();
		if (ISocketSubsystem* SS = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM))
		{
			SS->DestroySocket(Socket);
		}
		Socket = nullptr;
	}
	{
		FScopeLock Lock(&RemoteAddrsLock);
		RemoteAddrs.Reset();
	}

	// OS側のオーディオルーティングを既定（本体マイク側）へ戻す
	RestoreAudioRouting();

	// ハウリング検知の状態も次回開始時にはリセットしておく
	HowlingHighSince = 0.0;
	HowlingMuteUntil = 0.0;
}

void UReaStreamSender::ApplyAudioRouting(EMicInputMode Mode)
{
#if PLATFORM_ANDROID
	// Bluetoothマイク → 通信デバイスをBT SCOへ / 本体マイク → 解除
	SetAndroidBluetoothRouting(Mode == EMicInputMode::Bluetooth);
#endif
	UE_LOG(LogTemp, Warning, TEXT("[RIGDRED Mic] 入力経路=%s"),
		Mode == EMicInputMode::Bluetooth ? TEXT("Bluetooth") : TEXT("Builtin"));
}

void UReaStreamSender::RestoreAudioRouting()
{
#if PLATFORM_ANDROID
	// 起動時にBluetoothへ切り替えていた場合だけ戻す（本体マイク時は何もしない）
	if (InputMode == EMicInputMode::Bluetooth)
	{
		SetAndroidBluetoothRouting(false);
	}
#endif
}

FString UReaStreamSender::GetBluetoothInputDeviceName()
{
#if PLATFORM_ANDROID
	return GetAndroidBluetoothInputName();
#else
	return FString();
#endif
}

void UReaStreamSender::BeginDestroy()
{
	Stop();
	Super::BeginDestroy();
}

void UReaStreamSender::OnAudioCapture(const float* InAudio, int32 NumFrames, int32 NumChannels, int32 SampleRate)
{
	if (!bActive || !Socket || !InAudio || NumChannels < 1 || NumFrames < 1) return;

	// 入力レベル(RMS)を毎回算出。声が入っていれば送信インジケータ用に時刻を記録する。
	const double Now = FPlatformTime::Seconds();
	const int32 N = NumFrames * NumChannels;
	double Sum = 0.0;
	for (int32 i = 0; i < N; ++i) { Sum += (double)InAudio[i] * InAudio[i]; }
	const double Rms = FMath::Sqrt(Sum / FMath::Max(1, N));

	// しきい値（約 -45dBFS）以上なら「声あり」とみなす
	const double VoiceThreshold = 0.0056;
	if (Rms > VoiceThreshold)
	{
		LastVoiceTime.Store(Now);
	}

	// ハウリング（フィードバックループ）簡易検知：普通の会話は音量が上下するが、
	// ハウリングは大音量が途切れず続くという特徴があるため、それで見分ける。
	// AECのような根本対策ではなく、鳴り続けを早めに断ち切るための安全網。
	{
		constexpr double HowlingRmsThreshold = 0.28;     // 目安-11dBFS。大声より高いが飽和よりは低い
		constexpr double HowlingSustainSeconds = 0.4;    // これだけ連続で超え続けたらハウリングとみなす
		constexpr double HowlingMuteSeconds = 1.5;       // 検知後、この秒数だけ送信を止めてループを断ち切る

		if (Now < HowlingMuteUntil)
		{
			return; // クールダウン中：送信しない
		}

		if (Rms > HowlingRmsThreshold)
		{
			if (HowlingHighSince <= 0.0)
			{
				HowlingHighSince = Now;
			}
			else if (Now - HowlingHighSince > HowlingSustainSeconds)
			{
				HowlingMuteUntil = Now + HowlingMuteSeconds;
				HowlingHighSince = 0.0;
				UE_LOG(LogTemp, Warning, TEXT("[RIGDRED Mic] ハウリングを検知、%.1f秒送信を停止します（rms=%.3f）"), HowlingMuteSeconds, Rms);
				return;
			}
		}
		else
		{
			HowlingHighSince = 0.0;
		}
	}

	// マイクが拾えているか確認用に約1秒おきにログ（送信先の数も一緒に出す）
	if (Now - LastLogTime > 1.0)
	{
		LastLogTime = Now;
		int32 NumTargets = 0;
		{
			FScopeLock Lock(&RemoteAddrsLock);
			NumTargets = RemoteAddrs.Num();
		}
		UE_LOG(LogTemp, Warning, TEXT("[RIGDRED Mic] ch=%d sr=%d frames=%d rms=%.5f targets=%d"),
			NumChannels, SampleRate, NumFrames, Rms, NumTargets);
	}

	// ReaStreamのサンプル総バイトは最大1200。チャンク分割して送信。
	const int32 MaxFrames = FMath::Max(1, 300 / NumChannels);
	for (int32 Off = 0; Off < NumFrames; Off += MaxFrames)
	{
		const int32 F = FMath::Min(MaxFrames, NumFrames - Off);
		SendChunk(InAudio, Off, F, NumChannels, SampleRate);
	}
}

void UReaStreamSender::SendChunk(const float* Interleaved, int32 FrameOffset, int32 Frames, int32 NumChannels, int32 SampleRate)
{
	if (!Socket) return;

	TArray<TSharedPtr<FInternetAddr>> Targets;
	{
		FScopeLock Lock(&RemoteAddrsLock);
		Targets = RemoteAddrs;   // コピーしてロック時間を最小化
	}
	if (Targets.Num() == 0) return;

	const int32 SampleBytes = NumChannels * Frames * (int32)sizeof(float); // = sblocklen
	const int32 PacketSize  = 47 + SampleBytes;

	TArray<uint8> Buf;
	Buf.SetNumUninitialized(PacketSize);
	uint8* P = Buf.GetData();

	// ヘッダ
	P[0] = 'M'; P[1] = 'R'; P[2] = 'S'; P[3] = 'R';
	FMemory::Memcpy(P + 4, &PacketSize, sizeof(int32));      // packetsize

	// identifier（32バイト, 末尾0埋め）。Reaper側Receiveの Identifier と一致させる。
	FMemory::Memzero(P + 8, 32);
	{
		const FTCHARToUTF8 IdConv(*Identifier);
		const int32 IdLen = FMath::Min(31, IdConv.Length());
		if (IdLen > 0) { FMemory::Memcpy(P + 8, IdConv.Get(), IdLen); }
	}

	P[40] = (uint8)NumChannels;                              // nch
	int32 SR = SampleRate;
	FMemory::Memcpy(P + 41, &SR, sizeof(int32));             // samplerate
	uint16 SBlock = (uint16)SampleBytes;
	FMemory::Memcpy(P + 45, &SBlock, sizeof(uint16));        // sblocklen

	// サンプル（インターリーブ → プレーナ）
	float* Out = reinterpret_cast<float*>(P + 47);
	for (int32 c = 0; c < NumChannels; ++c)
	{
		for (int32 f = 0; f < Frames; ++f)
		{
			Out[c * Frames + f] = Interleaved[(FrameOffset + f) * NumChannels + c];
		}
	}

	// 同じパケットを送信先全員へ（通常は1件、グループ通話では複数件）送る。
	for (const TSharedPtr<FInternetAddr>& Addr : Targets)
	{
		if (!Addr.IsValid()) continue;
		int32 BytesSent = 0;
		Socket->SendTo(Buf.GetData(), PacketSize, BytesSent, *Addr);
	}
}
