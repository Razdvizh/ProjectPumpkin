// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MassHorde : ModuleRules
{
	public MassHorde(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "MassCommon", "MassEntity", 
            "MassMovement", "MassNavigation", "MassActors", "MassSpawner", "StructUtils"
        });
    }
}
