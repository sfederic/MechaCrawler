// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class MechaCrawlerEditorTarget : TargetRules
{
	public MechaCrawlerEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

        /*bUseAdaptiveUnityBuild = true;
        bAdaptiveUnityDisablesOptimizations = true;
        bAdaptiveUnityDisablesPCH = false;
        bAdaptiveUnityCreatesDedicatedPCH = true;
        bAdaptiveUnityEnablesEditAndContinue = true;*/

        ExtraModuleNames.AddRange( new string[] { "MechaCrawler" } );
	}
}
