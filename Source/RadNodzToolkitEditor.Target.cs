// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



using UnrealBuildTool;
using System.Collections.Generic;

public class RadNodzToolkitEditorTarget : TargetRules
{
	public RadNodzToolkitEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "RadNodzToolkit" } );
	}
}
