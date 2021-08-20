// Some copyright should be here...

using UnrealBuildTool;
using System.Reflection;

public class HelperFunctionLib : ModuleRules
{
	public HelperFunctionLib(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = true;
		MinFilesUsingPrecompiledHeaderOverride = 1;
		PrivatePCHHeaderFile = "Public/HelperFunctionLib.h";
		
		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"EngineSettings"
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"Slate",
				"SlateCore",
				"UMG"
			}
			);
	}
}
