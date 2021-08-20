// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KidRedWeapons : ModuleRules
{
	public KidRedWeapons(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "Public/KidRedWeapons.h";

		if (Target.Configuration == UnrealTargetConfiguration.Debug ||
		    Target.Configuration == UnrealTargetConfiguration.DebugGame ||
		    Target.Configuration == UnrealTargetConfiguration.Development)
		{
			PrivateDependencyModuleNames.Add("Debug");
		}

		PublicDependencyModuleNames.Add("Core");
		PublicDependencyModuleNames.Add("CoreUObject");
		PublicDependencyModuleNames.Add("Engine");
		PublicDependencyModuleNames.Add("HelperFunctionLib");
	}
}
