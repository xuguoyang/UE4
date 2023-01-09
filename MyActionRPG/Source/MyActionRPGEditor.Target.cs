// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyActionRPGEditorTarget : TargetRules
{
	public MyActionRPGEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        //BuildEnvironment = TargetBuildEnvironment.Shared;
        DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange( new string[] { "MyActionRPG" } );
	}
}
