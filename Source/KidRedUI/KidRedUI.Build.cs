// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KidRedUI : ModuleRules
{
	public KidRedUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "Public/KidRedUI.h";

		if (Target.Configuration == UnrealTargetConfiguration.Debug ||
		    Target.Configuration == UnrealTargetConfiguration.DebugGame ||
		    Target.Configuration == UnrealTargetConfiguration.Development)
		{
			PrivateDependencyModuleNames.Add("Debug");
		}

		PublicDependencyModuleNames.Add("Core");
		PublicDependencyModuleNames.Add("CoreUObject");
		PublicDependencyModuleNames.Add("Engine");
		PublicDependencyModuleNames.Add("InputCore");
		PublicDependencyModuleNames.Add("Slate");
		PublicDependencyModuleNames.Add("SlateCore");
		PublicDependencyModuleNames.Add("UMG");
		PublicDependencyModuleNames.Add("HelperFunctionLib");
		PublicDependencyModuleNames.Add("KidRedWeapons");
	}
}
