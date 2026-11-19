// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

using UnrealBuildTool;

public class RigdocksBronzePlugin : ModuleRules
{
	public RigdocksBronzePlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
				
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"RadnodzClientPlugin",
                "RIGDOCKS_BRONZE",
                "RADNODZ_client",
                "Json",
				"JsonUtilities",

            }
            );
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			}
			);
		
	}
}
