// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class MechaCrawlerTarget : TargetRules
{
	public MechaCrawlerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

        /*bUseAdaptiveUnityBuild = true;
        bAdaptiveUnityDisablesOptimizations = true;
        bAdaptiveUnityDisablesPCH = false;
        bAdaptiveUnityCreatesDedicatedPCH = true;
        bAdaptiveUnityEnablesEditAndContinue = true;*/

        ExtraModuleNames.AddRange( new string[] { "MechaCrawler" } );
	}
}
