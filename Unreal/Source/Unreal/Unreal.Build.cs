// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Unreal : ModuleRules
{
	public Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
