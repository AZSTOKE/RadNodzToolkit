// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Parse/RigdocksSilver_TRSC_Parse.h"
#include "RIGDOCKS_SILVER_Parse.h"
#include "DataClass/FWhisperSegment.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

UTranscriptionContext* URigdocksSilver_TRSC_Parse::AZ_TRSC_LoadModel_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return nullptr;
	}

	TranscriptionContext* ctx = SILVER::AZ_TRSC_LoadModel_Parse(call->Get());

	UTranscriptionContext* uCtx = NewObject<UTranscriptionContext>();
	uCtx->SetPointer(ctx);
	return uCtx;
}

FString URigdocksSilver_TRSC_Parse::AZ_TRSC_FullForMediaItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string fullText = SILVER::AZ_TRSC_FullForMediaItem_Parse(call->Get());
	return UTF8_TO_TCHAR(fullText.c_str());
}

FString URigdocksSilver_TRSC_Parse::AZ_TRSC_FullForMediaID_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string fullText = SILVER::AZ_TRSC_FullForMediaID_Parse(call->Get());
	return UTF8_TO_TCHAR(fullText.c_str());
}

FString URigdocksSilver_TRSC_Parse::AZ_TRSC_FullForSelectedMediaID_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string fullText = SILVER::AZ_TRSC_FullForSelectedMediaID_Parse(call->Get());
	return UTF8_TO_TCHAR(fullText.c_str());
}

FString URigdocksSilver_TRSC_Parse::AZ_TRSC_FullForTrackMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string fullText = SILVER::AZ_TRSC_FullForTrackMedia_Parse(call->Get());
	return UTF8_TO_TCHAR(fullText.c_str());
}

FString URigdocksSilver_TRSC_Parse::AZ_TRSC_FullForSelectedTrackMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return FString();
	}

	std::string fullText = SILVER::AZ_TRSC_FullForSelectedTrackMedia_Parse(call->Get());
	return UTF8_TO_TCHAR(fullText.c_str());
}

TArray<FWhisperSegment> URigdocksSilver_TRSC_Parse::AZ_TRSC_SegmentsForMediaItem_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FWhisperSegment>();
	}

	std::vector<WhisperSegment> segmentList = SILVER::AZ_TRSC_SegmentsForMediaItem_Parse(call->Get());

	TArray<FWhisperSegment> result;
	for (auto& segment : segmentList)
	{
		FWhisperSegment segmentObj;
		segmentObj.Initialize(segment);
		result.Add(segmentObj);
	}

	return result;
}

TArray<FWhisperSegment> URigdocksSilver_TRSC_Parse::AZ_TRSC_SegmentsForMediaID_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FWhisperSegment>();
	}

	std::vector<WhisperSegment> segmentList = SILVER::AZ_TRSC_SegmentsForMediaID_Parse(call->Get());

	TArray<FWhisperSegment> result;
	for (auto& segment : segmentList)
	{
		FWhisperSegment segmentObj;
		segmentObj.Initialize(segment);
		result.Add(segmentObj);
	}

	return result;
}

TArray<FWhisperSegment> URigdocksSilver_TRSC_Parse::AZ_TRSC_SegmentsForSelectedMediaID_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FWhisperSegment>();
	}

	std::vector<WhisperSegment> segmentList = SILVER::AZ_TRSC_SegmentsForSelectedMediaID_Parse(call->Get());

	TArray<FWhisperSegment> result;
	for (auto& segment : segmentList)
	{
		FWhisperSegment segmentObj;
		segmentObj.Initialize(segment);
		result.Add(segmentObj);
	}

	return result;
}

TArray<FWhisperSegment> URigdocksSilver_TRSC_Parse::AZ_TRSC_SegmentsForTrackMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FWhisperSegment>();
	}

	std::vector<WhisperSegment> segmentList = SILVER::AZ_TRSC_SegmentsForTrackMedia_Parse(call->Get());

	TArray<FWhisperSegment> result;
	for (auto& segment : segmentList)
	{
		FWhisperSegment segmentObj;
		segmentObj.Initialize(segment);
		result.Add(segmentObj);
	}

	return result;
}

TArray<FWhisperSegment> URigdocksSilver_TRSC_Parse::AZ_TRSC_SegmentsForSelectedTrackMedia_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return TArray<FWhisperSegment>();
	}

	std::vector<WhisperSegment> segmentList = SILVER::AZ_TRSC_SegmentsForSelectedTrackMedia_Parse(call->Get());

	TArray<FWhisperSegment> result;
	for (auto& segment : segmentList)
	{
		FWhisperSegment segmentObj;
		segmentObj.Initialize(segment);
		result.Add(segmentObj);
	}

	return result;
}

void URigdocksSilver_TRSC_Parse::AZ_TRSC_ReleaseModel_Parse(URigdocksRpcCall* call)
{
	if (call == nullptr)
	{
		return;
	}

	SILVER::AZ_TRSC_ReleaseModel_Parse(call->Get());
}
