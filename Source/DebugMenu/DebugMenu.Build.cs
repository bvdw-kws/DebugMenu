// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.

using System;
using System.IO;
using UnrealBuildTool;

public class DebugMenu : ModuleRules
{
	public DebugMenu(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"DeveloperSettings",
		});

		if (Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PublicDefinitions.Add("WITH_DEBUG_MENU");
			PrivateDependencyModuleNames.Add("ImGui");
		}
	}
}