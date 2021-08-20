// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KidRedEditorTarget : TargetRules
{
	public KidRedEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		bUseIncrementalLinking = true;
		bForceUnityBuild = true;
		bUseUnityBuild = true;
		bUseAdaptiveUnityBuild = true;
		bUsePCHFiles = true;
		bEnforceIWYU = true;

		ExtraModuleNames.Add("KidRedCore");
	}
}
