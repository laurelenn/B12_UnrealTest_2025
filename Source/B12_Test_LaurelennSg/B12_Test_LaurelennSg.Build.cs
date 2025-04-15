// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class B12_Test_LaurelennSg : ModuleRules
{
	public B12_Test_LaurelennSg(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "NiagaraCore", "Niagara", "NavigationSystem" });
	}
}
