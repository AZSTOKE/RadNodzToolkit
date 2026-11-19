// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

using UnrealBuildTool;

// PARAKNOT の配信を受け取るための UE 向けラッパー。
//
// **AudioLink をそのまま使わせない。** audiolink/*.h は STL をそのまま公開
// しており、UE のモジュールから直接触ると Windows.h との並びや警告設定で
// 面倒が起きる。このモジュールが pimpl で包み、公開ヘッダには UE の型しか
// 出さない。
//
// **接続先の解決はここでは行わない。** host / port / セッション鍵は
// 利用側が用意して Start() へ渡す (RadNodzToolkit は RADNODZ の業務 TCP で
// AZ_PARAKNOT_OpenOutputSession から受け取る)。AudioLink 本体と同じ切り分け。
public class ParaknotAudio : ModuleRules
{
	public ParaknotAudio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",

			// セッション鍵は RADNODZ の業務 TCP で取る。
			// **公開ヘッダが URadnodzClient* を受け取る**ので PUBLIC。
			"RadnodzClientPlugin"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// プリビルドの AudioLink (Source/Libs/AudioLink)。
			// **PRIVATE で取り込む** — audiolink/*.h を公開ヘッダに出さない。
			"AudioLink",

			// AZ_PARAKNOT_* の応答(JSON)の解析。
			"Json"
		});
	}
}
