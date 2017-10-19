// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris3DGameModeBase.h"
#include "S6PlayerController.h"
#include "S6Player.h"
#include "Tetris3DGameStateBase.h"
#include "Tetromino.h"
#include "TetrisGrid.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"

void ATetris3DGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("StartPlay..."));
	}
}

APlayerController* ATetris3DGameModeBase::SpawnPlayerController(ENetRole InRemoteRole, FVector const & SpawnLocation, FRotator const & SpawnRotation)
{
	APlayerController* pc = Super::SpawnPlayerController(InRemoteRole, SpawnLocation, SpawnRotation);
	UWorld * World = this->GetWorld();

	if (!World)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("World does not exist!"));
		}

		return pc;
	}

	if (!TetrominoBPClass)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("TetrominoBPClass does not exist!"));
		}

		return pc;
	}

	if (PlayerControllerClass && !PlayerControllerClass->GetDefaultObject()->IsA(AS6PlayerController::StaticClass()))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PlayerControllerClass is not an AS6PlayerController!"));
		}

		return pc;
	}

	FVector               Location           = FVector (0.0f, 0.0f, 0.0f);//FVector(-450.0f, 50.0f, 100.0f);
	FRotator              Rotation           = FRotator(0.0f, 0.0f, 0.0f);
	ATetromino          * CurrentTetromino   = World->SpawnActor<ATetromino >(TetrominoBPClass , Location, Rotation);
	ATetromino          * NextTetromino      = World->SpawnActor<ATetromino >(TetrominoBPClass , Location, Rotation);
	ATetrisGrid         * TetrisGrid         = World->SpawnActor<ATetrisGrid>(TetrisGridBPClass, Location, Rotation);
	AS6PlayerController * S6PlayerController = Cast<AS6PlayerController>(pc);

	if (S6PlayerController)
	{
		S6PlayerController->SetTetromino(CurrentTetromino);
	}

	if (TetrisGrid)
	{
		TetrisGrid->SetTetrominoes(CurrentTetromino, NextTetromino);
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("SpawnPlayerController..."));
	}

	return pc;
}


