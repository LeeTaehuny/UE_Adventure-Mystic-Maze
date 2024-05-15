// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MysticMaze : ModuleRules
{
	public MysticMaze(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "MysticMaze" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "Slate", "SlateCore" });
	}
}
