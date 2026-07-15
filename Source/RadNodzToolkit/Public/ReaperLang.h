// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"

/**
 * 簡易ローカライズ。9言語（日本語/英語/仏/伊/独/西/韓国語/中国語簡体字/繁体字）を切り替える。
 * UIラベル専用（トラック名・メディア名などのユーザーデータは対象外）。
 *
 * 使い方:  MakeText(..., ReaperLang::S(TEXT("接続"), TEXT("Connect")), ...)
 *
 * 日本語(JP)・英語(EN)は既存どおり呼び出し側でリテラルを直接渡す。
 * それ以外の言語は「英語文字列をキーにした翻訳辞書」（ReaperLang.cpp）を引き、
 * 辞書に未登録の場合は英語へフォールバックする。これにより呼び出し側の
 * S(JP, EN) / T(JP, EN) は一切変更不要。
 */
namespace ReaperLang
{
	enum class ELanguage : uint8
	{
		JP = 0,
		EN,
		FR,
		IT,
		DE,
		ES,
		KO,
		ZHS,   // 中国語（簡体字）
		ZHT,   // 中国語（繁体字）
		Count
	};

	// インライン関数のローカルstaticは全TUで単一インスタンス（C++保証）
	inline ELanguage& LanguageRef()
	{
		static ELanguage Lang = ELanguage::JP;
		return Lang;
	}

	inline ELanguage GetLanguage() { return LanguageRef(); }
	inline void SetLanguage(ELanguage InLang) { LanguageRef() = InLang; }
	inline bool IsJapanese() { return LanguageRef() == ELanguage::JP; }

	/** 言語ピッカー（プルダウン）に表示するその言語自身での言語名。 */
	const TCHAR* NativeName(ELanguage Lang);

	/** 表示名からELanguageを逆引きする（一致しなければJP）。 */
	ELanguage FromNativeName(const FString& Name);

	/** FIGS・韓国語・中国語(簡体/繁体) 用：英語文字列をキーに翻訳辞書を引く。未登録なら英語をそのまま返す。 */
	FString LookupTranslation(ELanguage Lang, const TCHAR* EN);

	/** 言語に応じた文字列を返す。 */
	inline FString S(const TCHAR* JP, const TCHAR* EN)
	{
		switch (LanguageRef())
		{
		case ELanguage::JP: return FString(JP);
		case ELanguage::EN: return FString(EN);
		default:            return LookupTranslation(LanguageRef(), EN);
		}
	}
	inline FText T(const TCHAR* JP, const TCHAR* EN)
	{
		return FText::FromString(S(JP, EN));
	}
}
