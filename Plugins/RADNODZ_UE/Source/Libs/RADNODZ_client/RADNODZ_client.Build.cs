// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

using System.IO;
using UnrealBuildTool;

public class RADNODZ_client : ModuleRules
{
    public RADNODZ_client(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;
        PublicSystemIncludePaths.Add("$(ModuleDir)/Public");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "RADNODZ_client.lib"));
            // mbedTLS (referenced by RADNODZ_client for the mTLS channel)
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "mbedtls.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "mbedx509.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "mbedcrypto.lib"));
            // zxing-cpp (referenced by RADNODZ_client for QR decoding)
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "ZXing.lib"));
            // mbedTLS entropy on Windows uses BCryptGenRandom (bcrypt.lib).
            PublicSystemLibraries.Add("bcrypt.lib");
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            // Link order matters on ld: dependent (RADNODZ_client) first, then mbedtls -> mbedx509 -> mbedcrypto.
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "arm64", "Release", "libRADNODZ_client.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "arm64", "Release", "libmbedtls.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "arm64", "Release", "libmbedx509.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "arm64", "Release", "libmbedcrypto.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "arm64", "Release", "libZXing.a"));

            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "x86_64", "Release", "libRADNODZ_client.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "x86_64", "Release", "libmbedtls.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "x86_64", "Release", "libmbedx509.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "x86_64", "Release", "libmbedcrypto.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "x86_64", "Release", "libZXing.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libRADNODZ_client.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libmbedtls.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libmbedx509.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libmbedcrypto.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libZXing.a"));
        }
    }
}
