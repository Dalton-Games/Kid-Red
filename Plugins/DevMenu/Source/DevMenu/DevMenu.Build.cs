// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DevMenu : ModuleRules
{
	public DevMenu(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = true;
		MinFilesUsingPrecompiledHeaderOverride = 1;
		PrivatePCHHeaderFile = "Public/DevMenu.h";
		
		PublicDependencyModuleNames.Add("Core");

		PrivateDependencyModuleNames.Add("CoreUObject");
		PrivateDependencyModuleNames.Add("Engine");
		PrivateDependencyModuleNames.Add("Settings");
		PrivateDependencyModuleNames.Add("Slate");
		PrivateDependencyModuleNames.Add("SlateCore");
		PrivateDependencyModuleNames.Add("UMG");
		PrivateDependencyModuleNames.Add("InputCore");
	}
}
