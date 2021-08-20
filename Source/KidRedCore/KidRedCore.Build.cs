// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KidRedCore : ModuleRules
{
	public KidRedCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivatePCHHeaderFile = "KidRed.h";

		if (Target.Configuration == UnrealTargetConfiguration.Debug ||
		    Target.Configuration == UnrealTargetConfiguration.DebugGame ||
		    Target.Configuration == UnrealTargetConfiguration.Development)
		{
			PublicDependencyModuleNames.Add("Debug");
		}
		
		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.Add("KidRedEditor");
		}

		PublicDependencyModuleNames.Add("Core");
		PublicDependencyModuleNames.Add("CoreUObject");
		PublicDependencyModuleNames.Add("Engine");
		PublicDependencyModuleNames.Add("InputCore");
		PublicDependencyModuleNames.Add("GameplayCameras");
		PublicDependencyModuleNames.Add("HelperFunctionLib");
		PublicDependencyModuleNames.Add("KidRedWeapons");
		PublicDependencyModuleNames.Add("KidRedUI");
	}
}
