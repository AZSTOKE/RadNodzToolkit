// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

using System.IO;
using UnrealBuildTool;

public class RIGDOCKS_GOLD : ModuleRules
{
    public RIGDOCKS_GOLD(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;
        PublicSystemIncludePaths.Add("$(ModuleDir)/Public");


        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "RIGDOCKS_GOLD.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "arm64", "Release", "libRIGDOCKS_GOLD.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "x86_64", "Release", "libRIGDOCKS_GOLD.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libRIGDOCKS_GOLD.a"));
        }
    }
}
