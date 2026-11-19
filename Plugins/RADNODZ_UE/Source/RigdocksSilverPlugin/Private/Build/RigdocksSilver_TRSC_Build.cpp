// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "Build/RigdocksSilver_TRSC_Build.h"
#include "RIGDOCKS_SILVER_Build.h"
#include "DataClass/FWhisperSegment.h"

using namespace RADNODZ_client;
using namespace RADNODZ_client::Object;

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_LoadModel_Build(FString modelPath)
{
	RpcCall c = SILVER::AZ_TRSC_LoadModel_Build(TCHAR_TO_UTF8(*modelPath));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_FullForMediaItem_Build(UTranscriptionContext* ctx, UMediaItem* item, FString language)
{
	RpcCall c = SILVER::AZ_TRSC_FullForMediaItem_Build(UTranscriptionContext::Get(ctx), UMediaItem::Get(item), TCHAR_TO_UTF8(*language));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_FullForMediaID_Build(UTranscriptionContext* ctx, UReaProject* proj, int mediaID, FString language)
{
	RpcCall c = SILVER::AZ_TRSC_FullForMediaID_Build(UTranscriptionContext::Get(ctx), UReaProject::Get(proj), mediaID, TCHAR_TO_UTF8(*language));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_FullForSelectedMediaID_Build(UTranscriptionContext* ctx, UReaProject* proj, int selectedMediaID, FString language)
{
	RpcCall c = SILVER::AZ_TRSC_FullForSelectedMediaID_Build(UTranscriptionContext::Get(ctx), UReaProject::Get(proj), selectedMediaID, TCHAR_TO_UTF8(*language));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_FullForTrackMedia_Build(UTranscriptionContext* ctx, UReaProject* proj, int trackID, int trackMediaID, FString language)
{
	RpcCall c = SILVER::AZ_TRSC_FullForTrackMedia_Build(UTranscriptionContext::Get(ctx), UReaProject::Get(proj), trackID, trackMediaID, TCHAR_TO_UTF8(*language));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_FullForSelectedTrackMedia_Build(UTranscriptionContext* ctx, UReaProject* proj, int selectedTrackID, int trackMediaID, FString language)
{
	RpcCall c = SILVER::AZ_TRSC_FullForSelectedTrackMedia_Build(UTranscriptionContext::Get(ctx), UReaProject::Get(proj), selectedTrackID, trackMediaID, TCHAR_TO_UTF8(*language));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_SegmentsForMediaItem_Build(UTranscriptionContext* ctx, UMediaItem* item, FString language)
{
	RpcCall c = SILVER::AZ_TRSC_SegmentsForMediaItem_Build(UTranscriptionContext::Get(ctx), UMediaItem::Get(item), TCHAR_TO_UTF8(*language));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_SegmentsForMediaID_Build(UTranscriptionContext* ctx, UReaProject* proj, int mediaID, FString language)
{
	RpcCall c = SILVER::AZ_TRSC_SegmentsForMediaID_Build(UTranscriptionContext::Get(ctx), UReaProject::Get(proj), mediaID, TCHAR_TO_UTF8(*language));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_SegmentsForSelectedMediaID_Build(UTranscriptionContext* ctx, UReaProject* proj, int selectedMediaID, FString language)
{
	RpcCall c = SILVER::AZ_TRSC_SegmentsForSelectedMediaID_Build(UTranscriptionContext::Get(ctx), UReaProject::Get(proj), selectedMediaID, TCHAR_TO_UTF8(*language));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_SegmentsForTrackMedia_Build(UTranscriptionContext* ctx, UReaProject* proj, int trackID, int trackMediaID, FString language)
{
	RpcCall c = SILVER::AZ_TRSC_SegmentsForTrackMedia_Build(UTranscriptionContext::Get(ctx), UReaProject::Get(proj), trackID, trackMediaID, TCHAR_TO_UTF8(*language));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_SegmentsForSelectedTrackMedia_Build(UTranscriptionContext* ctx, UReaProject* proj, int selectedTrackID, int trackMediaID, FString language)
{
	RpcCall c = SILVER::AZ_TRSC_SegmentsForSelectedTrackMedia_Build(UTranscriptionContext::Get(ctx), UReaProject::Get(proj), selectedTrackID, trackMediaID, TCHAR_TO_UTF8(*language));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}

URigdocksRpcCall* URigdocksSilver_TRSC_Build::AZ_TRSC_ReleaseModel_Build(UTranscriptionContext* ctx)
{
	RpcCall c = SILVER::AZ_TRSC_ReleaseModel_Build(UTranscriptionContext::Get(ctx));

	URigdocksRpcCall* obj = NewObject<URigdocksRpcCall>();
	obj->Set(c);
	return obj;
}
