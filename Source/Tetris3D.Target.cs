// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Tetris3DTarget : TargetRules
{
	public Tetris3DTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        ExtraModuleNames.AddRange(new string[] { "Tetris3D", "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore" });
        //ExtraModuleNames.AddRange( new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore" } );
	}
}
