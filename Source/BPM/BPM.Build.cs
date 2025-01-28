// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BPM : ModuleRules
{
	public BPM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
