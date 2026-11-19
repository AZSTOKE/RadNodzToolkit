// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdocksSilver_TRSC.h"
#include "RIGDOCKS_SILVER.h"
#include "DataClass/FWhisperSegment.h"

using namespace RADNODZ_client::Object;

UTranscriptionContext* URigdocksSilver_TRSC::AZ_TRSC_LoadModel(URadnodzClient* client, FString modelPath)
{
	TranscriptionContext* ctx = SILVER::AZ_TRSC_LoadModel(client->GetClient(), TCHAR_TO_UTF8(*modelPath));

	UTranscriptionContext* uCtx = NewObject<UTranscriptionContext>();
	uCtx->SetPointer(ctx);

	return uCtx;
}

FString URigdocksSilver_TRSC::AZ_TRSC_FullForMediaItem(URadnodzClient* client, UTranscriptionContext* ctx, UMediaItem* item, FString language)
{
	std::string fullText = SILVER::AZ_TRSC_FullForMediaItem(client->GetClient(), UTranscriptionContext::Get(ctx), UMediaItem::Get(item), TCHAR_TO_UTF8(*language));

	return UTF8_TO_TCHAR(fullText.c_str());
}

FString URigdocksSilver_TRSC::AZ_TRSC_FullForMediaID(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int mediaID, FString language)
{
	std::string fullText = SILVER::AZ_TRSC_FullForMediaID(client->GetClient(), UTranscriptionContext::Get(ctx), UReaProject::Get(proj), mediaID, TCHAR_TO_UTF8(*language));

	return UTF8_TO_TCHAR(fullText.c_str());
}

FString URigdocksSilver_TRSC::AZ_TRSC_FullForSelectedMediaID(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int selectedMediaID, FString language)
{
	std::string fullText = SILVER::AZ_TRSC_FullForSelectedMediaID(client->GetClient(), UTranscriptionContext::Get(ctx), UReaProject::Get(proj), selectedMediaID, TCHAR_TO_UTF8(*language));

	return UTF8_TO_TCHAR(fullText.c_str());
}

FString URigdocksSilver_TRSC::AZ_TRSC_FullForTrackMedia(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int trackID, int trackMediaID, FString language)
{
	std::string fullText = SILVER::AZ_TRSC_FullForTrackMedia(client->GetClient(), UTranscriptionContext::Get(ctx), UReaProject::Get(proj), trackID, trackMediaID, TCHAR_TO_UTF8(*language));

	return UTF8_TO_TCHAR(fullText.c_str());
}

FString URigdocksSilver_TRSC::AZ_TRSC_FullForSelectedTrackMedia(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int selectedTrackID, int trackMediaID, FString language)
{
	std::string fullText = SILVER::AZ_TRSC_FullForSelectedTrackMedia(client->GetClient(), UTranscriptionContext::Get(ctx), UReaProject::Get(proj), selectedTrackID, trackMediaID, TCHAR_TO_UTF8(*language));

	return UTF8_TO_TCHAR(fullText.c_str());
}

TArray<FWhisperSegment> URigdocksSilver_TRSC::AZ_TRSC_SegmentsForMediaItem(URadnodzClient* client, UTranscriptionContext* ctx, UMediaItem* item, FString language)
{
	std::vector<WhisperSegment> segmentList = SILVER::AZ_TRSC_SegmentsForMediaItem(client->GetClient(), UTranscriptionContext::Get(ctx), UMediaItem::Get(item), TCHAR_TO_UTF8(*language));

	TArray<FWhisperSegment> result;
	for (auto& segment : segmentList) {
		FWhisperSegment segmentObj;
		segmentObj.Initialize(segment);
		result.Add(segmentObj);
	}

	return result;
}

TArray<FWhisperSegment> URigdocksSilver_TRSC::AZ_TRSC_SegmentsForMediaID(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int mediaID, FString language)
{
	std::vector<WhisperSegment> segmentList = SILVER::AZ_TRSC_SegmentsForMediaID(client->GetClient(), UTranscriptionContext::Get(ctx), UReaProject::Get(proj), mediaID, TCHAR_TO_UTF8(*language));

	TArray<FWhisperSegment> result;
	for (auto& segment : segmentList)
	{
		FWhisperSegment segmentObj;
		segmentObj.Initialize(segment);
		result.Add(segmentObj);
	}

	return result;
}

TArray<FWhisperSegment> URigdocksSilver_TRSC::AZ_TRSC_SegmentsForSelectedMediaID(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int selectedMediaID, FString language)
{
	std::vector<WhisperSegment> segmentList = SILVER::AZ_TRSC_SegmentsForSelectedMediaID(client->GetClient(), UTranscriptionContext::Get(ctx), UReaProject::Get(proj), selectedMediaID, TCHAR_TO_UTF8(*language));

	TArray<FWhisperSegment> result;
	for (auto& segment : segmentList)
	{
		FWhisperSegment segmentObj;
		segmentObj.Initialize(segment);
		result.Add(segmentObj);
	}

	return result;
}

TArray<FWhisperSegment> URigdocksSilver_TRSC::AZ_TRSC_SegmentsForTrackMedia(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int trackID, int trackMediaID, FString language)
{
	std::vector<WhisperSegment> segmentList = SILVER::AZ_TRSC_SegmentsForTrackMedia(client->GetClient(), UTranscriptionContext::Get(ctx), UReaProject::Get(proj), trackID, trackMediaID, TCHAR_TO_UTF8(*language));

	TArray<FWhisperSegment> result;
	for (auto& segment : segmentList)
	{
		FWhisperSegment segmentObj;
		segmentObj.Initialize(segment);
		result.Add(segmentObj);
	}

	return result;
}

TArray<FWhisperSegment> URigdocksSilver_TRSC::AZ_TRSC_SegmentsForSelectedTrackMedia(URadnodzClient* client, UTranscriptionContext* ctx, UReaProject* proj, int selectedTrackID, int trackMediaID, FString language)
{
	std::vector<WhisperSegment> segmentList = SILVER::AZ_TRSC_SegmentsForSelectedTrackMedia(client->GetClient(), UTranscriptionContext::Get(ctx), UReaProject::Get(proj), selectedTrackID, trackMediaID, TCHAR_TO_UTF8(*language));

	TArray<FWhisperSegment> result;
	for (auto& segment : segmentList)
	{
		FWhisperSegment segmentObj;
		segmentObj.Initialize(segment);
		result.Add(segmentObj);
	}

	return result;
}

void URigdocksSilver_TRSC::AZ_TRSC_ReleaseModel(URadnodzClient* client, UTranscriptionContext* ctx)
{
	SILVER::AZ_TRSC_ReleaseModel(client->GetClient(), UTranscriptionContext::Get(ctx));
}