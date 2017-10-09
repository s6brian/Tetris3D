// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris3DGameModeBase.h"
#include "S6Player.h"

void ATetris3DGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is Tetris3D!"));
	}
}


