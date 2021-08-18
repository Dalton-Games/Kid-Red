// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KidRedGame : ModuleRules
{
	public KidRedGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
