// Fill out your copyright notice in the Description page of Project Settings.

#include "HRTF/HRTFRegistry.h"

namespace
{
	// 実測データ（Attributionありのプロファイル：KEMAR等）を一覧・ライセンス表記に出すかどうか。
	// false の間は内蔵（Attributionなし）のみが選択肢に出る。実測データ自体は登録済みのまま
	// 保持されるので、また列挙したくなったらここを true にするだけでよい。
	constexpr bool bExposeExternalProfiles = false;
}

int32 FReaperHRTFProfile::NearestIndex(float QueryAzDeg) const
{
	if (!IsValid()) return -1;

	// -180..180 に正規化
	auto Wrap = [](float D) -> float
	{
		while (D > 180.f)  D -= 360.f;
		while (D < -180.f) D += 360.f;
		return D;
	};
	const float Q = Wrap(QueryAzDeg);

	int32 Best = 0;
	float BestDiff = TNumericLimits<float>::Max();
	for (int32 i = 0; i < Count; ++i)
	{
		const float Diff = FMath::Abs(Wrap(AzDeg[i] - Q));
		if (Diff < BestDiff)
		{
			BestDiff = Diff;
			Best = i;
		}
	}
	return Best;
}

FReaperHRTFRegistry& FReaperHRTFRegistry::Get()
{
	static FReaperHRTFRegistry Instance;
	return Instance;
}

void FReaperHRTFRegistry::Register(const FReaperHRTFProfile& Profile)
{
	if (!Profile.IsValid()) return;
	// 同名があれば置き換え
	for (FReaperHRTFProfile& P : Profiles)
	{
		if (P.Name == Profile.Name) { P = Profile; return; }
	}
	Profiles.Add(Profile);
}

const FReaperHRTFProfile* FReaperHRTFRegistry::FindByName(const FString& Name) const
{
	for (const FReaperHRTFProfile& P : Profiles)
	{
		if (P.Name == Name) { return &P; }
	}
	return nullptr;
}

const FReaperHRTFProfile* FReaperHRTFRegistry::GetByIndex(int32 Index) const
{
	return Profiles.IsValidIndex(Index) ? &Profiles[Index] : nullptr;
}

void FReaperHRTFRegistry::GetNames(TArray<FString>& OutNames) const
{
	OutNames.Reset();
	for (const FReaperHRTFProfile& P : Profiles)
	{
		if (bExposeExternalProfiles || P.Attribution.IsEmpty())
		{
			OutNames.Add(P.Name);
		}
	}
}

void FReaperHRTFRegistry::GetAttributions(TArray<FString>& OutAttributions) const
{
	OutAttributions.Reset();
	if (!bExposeExternalProfiles) { return; }   // 内蔵のみの間はライセンス表記も出さない
	for (const FReaperHRTFProfile& P : Profiles)
	{
		if (!P.Attribution.IsEmpty())
		{
			OutAttributions.AddUnique(P.Attribution);
		}
	}
}

FReaperHRTFRegistrar::FReaperHRTFRegistrar(const TCHAR* Name, int32 SampleRate, int32 Taps, int32 Count,
	const float* AzDeg, const float* L, const float* R, const TCHAR* Attribution)
{
	FReaperHRTFProfile P;
	P.Name = Name;
	P.Attribution = Attribution ? FString(Attribution) : FString();
	P.SampleRate = SampleRate;
	P.Taps = Taps;
	P.Count = Count;
	P.AzDeg = AzDeg;
	P.L = L;
	P.R = R;
	FReaperHRTFRegistry::Get().Register(P);
}
