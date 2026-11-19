// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

using System.IO;
using UnrealBuildTool;

// AudioLink — PARAKNOT の音声送受信ライブラリ(プリビルドの静的ライブラリ)。
//
// ヘッダとビルド済みライブラリのみを同梱している(ソースは含まない)。
// Windows 用は Visual Studio 2022 のツールセットでビルドしている。
// どのコンパイラで作ったかは各 Release フォルダの toolchain.txt に記録してある。
public class AudioLink : ModuleRules
{
	public AudioLink(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		// audiolink/*.h は UE の規約に合わせず STL をそのまま使うので、
		// System 扱いにして UE の厳しめの警告を持ち込まない。
		PublicSystemIncludePaths.Add("$(ModuleDir)/Public");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(
				Path.Combine(ModuleDirectory, "x64", "VS2022", "Release", "AudioLink.lib"));

			// UdpSocket が Winsock2 を使う。
			PublicSystemLibraries.Add("ws2_32.lib");
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			// **両 ABI を並べる。** UE は ABI ごとにリンクするので、
			// 対象外のほうは単に使われない。
			PublicAdditionalLibraries.Add(
				Path.Combine(ModuleDirectory, "Android", "arm64", "Release", "libAudioLink.a"));
			PublicAdditionalLibraries.Add(
				Path.Combine(ModuleDirectory, "Android", "x86_64", "Release", "libAudioLink.a"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			PublicAdditionalLibraries.Add(
				Path.Combine(ModuleDirectory, "Mac", "Release", "libAudioLink.a"));
		}
	}
}
