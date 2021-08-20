// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KidRedTarget : TargetRules
{
	public KidRedTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("KidRedCore");
	}
}
