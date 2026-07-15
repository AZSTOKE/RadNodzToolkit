// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#pragma once

#include "CoreMinimal.h"
#include "Fonts/SlateFontInfo.h"
#include "Fonts/CompositeFont.h"
#include "Engine/FontFace.h"
#include "Styling/CoreStyle.h"
#include "UObject/StrongObjectPtr.h"
#include "Math/Range.h"

/**
 * アプリ共通フォント（M PLUS 1）を返すヘルパー。
 * Content/Fonts に取り込んだ FontFace アセットから合成フォントを組み立てて返す。
 * アセットがまだ無い場合は標準フォントにフォールバックする（クラッシュしない）。
 */
namespace ReaperFont
{
	inline FSlateFontInfo Get(int32 Size, bool bBold)
	{
		static TSharedPtr<FCompositeFont> Composite;
		static TStrongObjectPtr<UFontFace> RegFaceRef;
		static TStrongObjectPtr<UFontFace> BoldFaceRef;

		// まだ組み立てていなければ、FontFaceアセットを読み込んで合成フォントを作る
		if (!Composite.IsValid())
		{
			UFontFace* Reg = LoadObject<UFontFace>(nullptr, TEXT("/Game/Fonts/Mplus1-Regular.Mplus1-Regular"));
			if (Reg)
			{
				UFontFace* Bold = LoadObject<UFontFace>(nullptr, TEXT("/Game/Fonts/Mplus1-Bold.Mplus1-Bold"));
				if (!Bold) { Bold = Reg; }

				RegFaceRef.Reset(Reg);
				BoldFaceRef.Reset(Bold);

				TSharedRef<FCompositeFont> C = MakeShared<FCompositeFont>();

				FTypefaceEntry RegEntry;
				RegEntry.Name = FName(TEXT("Regular"));
				RegEntry.Font = FFontData(Reg);
				C->DefaultTypeface.Fonts.Add(RegEntry);

				FTypefaceEntry BoldEntry;
				BoldEntry.Name = FName(TEXT("Bold"));
				BoldEntry.Font = FFontData(Bold);
				C->DefaultTypeface.Fonts.Add(BoldEntry);

				// M PLUS 1 にはハングル（韓国語）や簡体字中国語特有の字形が含まれていないため、
				// 未同梱の場合は表示できない（文字が四角の欠字になる）。
				// ハングルは韓国語専用のUnicode範囲（M PLUS 1の日本語表示とは重複しない）なので、
				// SubTypefaceで範囲限定のフォールバックにし、M PLUS 1側の字形を一切上書きしない。
				if (UFontFace* KoreanFace = LoadObject<UFontFace>(nullptr, TEXT("/Game/Fonts/FallbackCJK-Regular.FallbackCJK-Regular")))
				{
					static TStrongObjectPtr<UFontFace> KoreanFaceRef;
					KoreanFaceRef.Reset(KoreanFace);

					FTypefaceEntry KoreanEntry;
					KoreanEntry.Name = FName(TEXT("Regular"));
					KoreanEntry.Font = FFontData(KoreanFace);

					FCompositeSubFont KoreanSubFont;
					KoreanSubFont.Typeface.Fonts.Add(KoreanEntry);
					KoreanSubFont.CharacterRanges.Add(FInt32Range(0x1100, 0x1200));   // Hangul Jamo
					KoreanSubFont.CharacterRanges.Add(FInt32Range(0x3130, 0x3190));   // Hangul Compatibility Jamo
					KoreanSubFont.CharacterRanges.Add(FInt32Range(0xA960, 0xA980));   // Hangul Jamo Extended-A
					KoreanSubFont.CharacterRanges.Add(FInt32Range(0xAC00, 0xD7A4));   // Hangul Syllables
					KoreanSubFont.CharacterRanges.Add(FInt32Range(0xD7B0, 0xD800));   // Hangul Jamo Extended-B
					C->SubTypefaces.Add(KoreanSubFont);
				}

				// 簡体字中国語特有の字形（M PLUS 1・ハングルSubTypeface双方に無い文字）は
				// 最終フォールバック（FallbackTypeface）で拾う。ここは「他のどのTypefaceにも
				// 一致しなかった最後の手段」としてのみ使われるため、M PLUS 1で表示できる文字を
				// 奪うことはない。
				// FontFaceアセットを "/Game/Fonts/NotoSansSC-Regular" として追加すれば、
				// アセットが無い間は何もせず、既存のJP/EN表示に影響しない。
				if (UFontFace* FallbackFace = LoadObject<UFontFace>(nullptr, TEXT("/Game/Fonts/NotoSansSC-Regular.NotoSansSC-Regular")))
				{
					static TStrongObjectPtr<UFontFace> FallbackFaceRef;
					FallbackFaceRef.Reset(FallbackFace);
					FTypefaceEntry FallbackEntry;
					FallbackEntry.Name = FName(TEXT("Regular"));
					FallbackEntry.Font = FFontData(FallbackFace);
					C->FallbackTypeface.Typeface.Fonts.Add(FallbackEntry);
				}

				Composite = C;
			}
		}

		if (Composite.IsValid())
		{
			return FSlateFontInfo(Composite, Size, bBold ? FName(TEXT("Bold")) : FName(TEXT("Regular")));
		}

		// フォールバック：エンジン標準フォント
		return FCoreStyle::GetDefaultFontStyle(bBold ? "Bold" : "Regular", Size);
	}
}
