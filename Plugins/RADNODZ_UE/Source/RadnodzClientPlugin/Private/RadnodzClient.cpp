// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RadnodzClient.h"
#include "RADNODZ_Core.h"
#include "PairingUrl.h"
#include "QrDecode.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"
#include "Modules/ModuleManager.h"

#include <chrono>

URadnodzClient* URadnodzClient::CreateRadnodzClient()
{
	return NewObject<URadnodzClient>();
}

bool URadnodzClient::Connect(const FString& ip, int port)
{
	return client.Connect(TCHAR_TO_UTF8(*ip), port);
}

FString URadnodzClient::Send(const FString& message)
{
	FString result = UTF8_TO_TCHAR(client.Send(TCHAR_TO_UTF8(*message)).c_str());

	return result;
}

void URadnodzClient::Disconnect()
{
	client.Disconnect();
}

bool URadnodzClient::IsConnected() const
{
	return client.IsConnected();
}

RADNODZ_client::TcpClient& URadnodzClient::GetClient()
{
	 return client; 
}

FRigdocksError URadnodzClient::GetError()
{
	FRigdocksError error;
	error.ErrorCode = client.errorCode;
	error.Message = UTF8_TO_TCHAR(client.errorMessage.c_str());
	error.Details = UTF8_TO_TCHAR(client.errorDetails.c_str());

	return error;
}

int64 URadnodzClient::GetLastSendTime() const
{
	return static_cast<int64>(client.GetLastSendTime());
}

int64 URadnodzClient::GetLastServerRecvTime() const
{
	return static_cast<int64>(client.GetLastServerRecvTime());
}

int64 URadnodzClient::GetLastServerReturnTime() const
{
	return static_cast<int64>(client.GetLastServerReturnTime());
}

int64 URadnodzClient::GetLastRecvTime() const
{
	return static_cast<int64>(client.GetLastRecvTime());
}

int64 URadnodzClient::GetNowUnixMicros()
{
	return static_cast<int64>(
		std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count());
}

bool URadnodzClient::SendTimingProbe()
{
	if (!client.IsConnected())
	{
		return false;
	}

	return RADNODZ_client::Core::AZ_CheckLicense(client) == 0;
}

// ===== セキュア通信: アイデンティティ(mTLS) =====

void URadnodzClient::SetStorageDirectory(const FString& Dir)
{
	client.SetStorageDirectory(TCHAR_TO_UTF8(*Dir));
}

bool URadnodzClient::GenerateIdentity()
{
	return client.GenerateIdentity();
}

bool URadnodzClient::RegenerateIdentity()
{
	return client.RegenerateIdentity();
}

FString URadnodzClient::GetOwnPublicKeyPem()
{
	return UTF8_TO_TCHAR(client.GetOwnPublicKeyPem().c_str());
}

FString URadnodzClient::GetOwnFingerprint()
{
	return UTF8_TO_TCHAR(client.GetOwnFingerprint().c_str());
}

// ===== セキュア通信: サーバ検証(ポリシー方式) =====

void URadnodzClient::SetServerId(const FString& Id)
{
	client.SetServerId(TCHAR_TO_UTF8(*Id));
}

void URadnodzClient::SetExpectedServerFingerprint(const FString& Fingerprint)
{
	ExpectedServerFingerprint = Fingerprint;
	EnsureVerifier();
}

void URadnodzClient::SetTrustOnFirstUse(bool bEnable)
{
	bTrustOnFirstUse = bEnable;
	EnsureVerifier();
}

void URadnodzClient::EnsureVerifier()
{
	// ポリシー方式のサーバ検証:
	//  - 期待 FP が設定されていれば、その値に一致した場合のみ許可(ピン留め)。
	//  - 未設定でも TOFU が有効なら、初回(未ピン)のみ許可。
	//  - それ以外は拒否(フェイルクローズ)。
	client.SetHostKeyVerifier(
		[this](const RADNODZ_client::HostKeyInfo& info) -> RADNODZ_client::HostKeyVerdict
		{
			if (!ExpectedServerFingerprint.IsEmpty())
			{
				const std::string expected = TCHAR_TO_UTF8(*ExpectedServerFingerprint);
				return (info.fingerprint == expected)
					? RADNODZ_client::HostKeyVerdict::AcceptAndPin
					: RADNODZ_client::HostKeyVerdict::Reject;
			}

			if (bTrustOnFirstUse && info.status == RADNODZ_client::HostKeyStatus::Unknown)
			{
				return RADNODZ_client::HostKeyVerdict::AcceptAndPin;
			}

			return RADNODZ_client::HostKeyVerdict::Reject;
		});
}

// ===== セキュア通信: ピン管理 =====

TArray<FRadnodzPinnedServer> URadnodzClient::ListPinnedServers()
{
	TArray<FRadnodzPinnedServer> Out;
	for (const auto& p : client.ListPinnedServers())
	{
		FRadnodzPinnedServer e;
		e.ServerId = UTF8_TO_TCHAR(p.serverId.c_str());
		e.Fingerprint = UTF8_TO_TCHAR(p.fingerprint.c_str());
		e.PublicKeyPem = UTF8_TO_TCHAR(p.publicKeyPem.c_str());
		Out.Add(e);
	}
	return Out;
}

bool URadnodzClient::RemovePinnedServer(const FString& ServerId)
{
	return client.RemovePinnedServer(TCHAR_TO_UTF8(*ServerId));
}

// ===== 宛先(サーバモジュール) =====

void URadnodzClient::SetTargetProductId(const FString& ProductId)
{
	client.SetTargetProductId(TCHAR_TO_UTF8(*ProductId));
}

FString URadnodzClient::GetTargetProductId() const
{
	return UTF8_TO_TCHAR(client.GetTargetProductId().c_str());
}

// ===== ペアリング(QR コード / URL) =====

bool URadnodzClient::ParsePairingUrl(const FString& Url, FRadnodzPairingInfo& OutInfo)
{
	OutInfo = FRadnodzPairingInfo();

	RADNODZ_client::ServerPairingInfo info;

	if (!RADNODZ_client::ParseServerPairingUrl(TCHAR_TO_UTF8(*Url), info))
	{
		return false;
	}

	OutInfo.NodeId = UTF8_TO_TCHAR(info.nodeId.c_str());
	OutInfo.Host = UTF8_TO_TCHAR(info.host.c_str());
	OutInfo.Port = info.port;
	OutInfo.Fingerprint = UTF8_TO_TCHAR(info.fingerprint.c_str());

	return true;
}

bool URadnodzClient::DecodeQrGrayscale(const uint8* Gray, int32 Width, int32 Height, int32 Stride,
	FString& OutText, FString& OutError)
{
	OutText.Empty();
	OutError.Empty();

	std::string Text;

	RADNODZ_client::QrDecodeError Error;

	if (!RADNODZ_client::DecodeQrCode(Gray, Width, Height, Stride, Text, Error))
	{
		// デコーダのメッセージは英語なので、利用者向けの文言はここで用意する。
		OutError = Error.notFound
			? TEXT("画像の中に QR コードが見つかりませんでした")
			: FString(UTF8_TO_TCHAR(Error.message.c_str()));

		return false;
	}

	OutText = FString(UTF8_TO_TCHAR(Text.c_str()));

	return true;
}

bool URadnodzClient::DecodeQrImageFile(const FString& FilePath, FString& OutText, FString& OutError)
{
	OutText.Empty();
	OutError.Empty();

	TArray<uint8> FileData;

	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		OutError = TEXT("ファイルを読めませんでした");

		return false;
	}

	IImageWrapperModule& ImageWrapperModule =
		FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));

	// 拡張子ではなく中身から形式を判定する(拡張子が実態と違っていても読める)。
	const EImageFormat Format = ImageWrapperModule.DetectImageFormat(FileData.GetData(), FileData.Num());

	if (Format == EImageFormat::Invalid)
	{
		OutError = TEXT("画像として認識できませんでした");

		return false;
	}

	const TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);

	if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
	{
		OutError = TEXT("画像を展開できませんでした");

		return false;
	}

	TArray<uint8> Raw;

	if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, Raw))
	{
		OutError = TEXT("画像を展開できませんでした");

		return false;
	}

	const int32 Width = ImageWrapper->GetWidth();
	const int32 Height = ImageWrapper->GetHeight();

	if (Width <= 0 || Height <= 0 || Raw.Num() < Width * Height * 4)
	{
		OutError = TEXT("画像の大きさが取れませんでした");

		return false;
	}

	// 輝度へ落とす。QR は白黒なので係数の精度は問わない。
	TArray<uint8> Gray;
	Gray.SetNumUninitialized(Width * Height);

	for (int32 i = 0; i < Width * Height; i++)
	{
		const uint8 Blue = Raw[i * 4 + 0];
		const uint8 Green = Raw[i * 4 + 1];
		const uint8 Red = Raw[i * 4 + 2];

		Gray[i] = static_cast<uint8>((Red * 77 + Green * 150 + Blue * 29) >> 8);
	}

	return DecodeQrGrayscale(Gray.GetData(), Width, Height, Width, OutText, OutError);
}

bool URadnodzClient::ApplyServerPairingUrl(const FString& Url)
{
	return client.ApplyServerPairingUrl(TCHAR_TO_UTF8(*Url));
}

bool URadnodzClient::PinServer(const FString& ServerId, const FString& Fingerprint,
	const FString& PublicKeyPem)
{
	return client.PinServer(TCHAR_TO_UTF8(*ServerId), TCHAR_TO_UTF8(*Fingerprint),
		TCHAR_TO_UTF8(*PublicKeyPem));
}

bool URadnodzClient::ConnectToPairedServer()
{
	// 接続先は ApplyServerPairingUrl(または Connect(ip, port))が保持している。
	return client.Connect();
}

ERadnodzEnrollStatus URadnodzClient::Enroll(const FString& ClientName)
{
	switch (client.Enroll(TCHAR_TO_UTF8(*ClientName)))
	{
	case RADNODZ_client::TcpClient::EnrollStatus::Pending:
		return ERadnodzEnrollStatus::Pending;

	case RADNODZ_client::TcpClient::EnrollStatus::Approved:
		return ERadnodzEnrollStatus::Approved;

	case RADNODZ_client::TcpClient::EnrollStatus::Closed:
		return ERadnodzEnrollStatus::Closed;

	case RADNODZ_client::TcpClient::EnrollStatus::Rejected:
		return ERadnodzEnrollStatus::Rejected;

	default:
		return ERadnodzEnrollStatus::Failed;
	}
}

