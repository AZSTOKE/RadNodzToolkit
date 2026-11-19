// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

using UnrealBuildTool;

public class RadnodzClientEditerPlugin : ModuleRules
{
	public RadnodzClientEditerPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "UnrealEd",	
				"BlueprintGraph",
				"KismetCompiler",
				"GraphEditor",
				"Kismet",
                "RadnodzClientPlugin",
            }
			);
	}
}
