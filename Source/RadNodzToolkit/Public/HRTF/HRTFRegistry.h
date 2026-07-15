// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 1つのHRTFプロファイル（水平面の各方位のHRIR：左右FIR）。
 * データ実体は生成された .cpp 内の static 配列を指す（コンパイル同梱）。
 */
struct FReaperHRTFProfile
{
	FString      Name;
	FString      Attribution;       // ライセンス/著作権表記（無ければ空）。設定のライセンス欄に列挙する。
	int32        SampleRate = 44100;
	int32        Taps = 0;          // 1方位あたりのFIRタップ数
	int32        Count = 0;         // 方位数
	const float* AzDeg = nullptr;   // [Count] 方位角(度, -180..180)
	const float* L = nullptr;       // [Count*Taps] 左耳HRIR（方位i: L + i*Taps）
	const float* R = nullptr;       // [Count*Taps] 右耳HRIR

	bool IsValid() const { return Taps > 0 && Count > 0 && AzDeg && L && R; }

	/** 指定方位に最も近い方位indexを返す（見つからなければ-1）。 */
	int32 NearestIndex(float QueryAzDeg) const;
};

/**
 * 同梱HRTFプロファイルのグローバルレジストリ。
 * 生成された .cpp 内の static FReaperHRTFRegistrar が起動時に自動登録する。
 *
 * 現状は「内蔵（自前生成、Attributionが空＝ライセンス表記なし）」以外のプロファイル
 * （KEMAR等の実測データ、ライセンス表記あり）を GetNames/GetAttributions の一覧から除外している。
 * 登録自体（Register）は今まで通り行われるためデータは失われず、また列挙したくなったら
 * HRTFRegistry.cpp の bExposeExternalProfiles を true にするだけで復活する。
 */
class RADNODZTOOLKIT_API FReaperHRTFRegistry
{
public:
	static FReaperHRTFRegistry& Get();

	void Register(const FReaperHRTFProfile& Profile);

	int32 Num() const { return Profiles.Num(); }
	const FReaperHRTFProfile* FindByName(const FString& Name) const;
	const FReaperHRTFProfile* GetByIndex(int32 Index) const;

	/** 一覧に出すプロファイル名を返す（既定では内蔵のみ）。 */
	void GetNames(TArray<FString>& OutNames) const;

	/** 一覧に出すプロファイルのライセンス表記（重複・空を除く）を返す（既定では内蔵のみのため空）。 */
	void GetAttributions(TArray<FString>& OutAttributions) const;

private:
	TArray<FReaperHRTFProfile> Profiles;
};

/**
 * 生成された .cpp が使う自己登録ヘルパー。
 * 例: static const FReaperHRTFRegistrar Reg(TEXT("KEMAR"), 44100, 200, 25, Az, L, R);
 */
struct RADNODZTOOLKIT_API FReaperHRTFRegistrar
{
	FReaperHRTFRegistrar(const TCHAR* Name, int32 SampleRate, int32 Taps, int32 Count,
		const float* AzDeg, const float* L, const float* R, const TCHAR* Attribution = nullptr);
};
