// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class RadNodzToolkit : ModuleRules
{
	public RadNodzToolkit(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// HRTFData_*.cpp は自動生成でファイルスコープに同名シンボル(Az/L/R/Reg)を定義するため、
		// ユニティビルドで結合されると再定義・名前衝突を起こす。非ユニティで個別にコンパイルする。
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore",
			"UMG", "Slate", "SlateCore",
			"ApplicationCore",
			"Sockets", "Networking",
			"AudioCaptureCore",
			"AndroidPermission",
			"RadnodzClientPlugin",
			"RigdocksSilverPlugin",
			"RigdocksBronzePlugin",
			"RigdocksGoldPlugin"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
