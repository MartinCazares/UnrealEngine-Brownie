// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BrowniePluginTesting : ModuleRules
{
	public BrowniePluginTesting(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "BrownieBridge" });
	}
}
