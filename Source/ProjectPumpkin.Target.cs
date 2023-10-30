// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectPumpkinTarget : TargetRules
{
	public ProjectPumpkinTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("ProjectPumpkin");

		GlobalDefinitions.Add("ECC_Floor=ECC_GameTraceChannel1");
		GlobalDefinitions.Add("ECC_Projectile=ECC_GameTraceChannel2");
	}
}
