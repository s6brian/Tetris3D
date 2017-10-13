// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris3DGameModeBase.h"
#include "S6PlayerController.h"
#include "Tetromino.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"


void ATetris3DGameModeBase::StartPlay()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("StartPlay..."));
	}

	Super::StartPlay();
}

APlayerController* ATetris3DGameModeBase::SpawnPlayerController(ENetRole InRemoteRole, FVector const & SpawnLocation, FRotator const & SpawnRotation)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("SpawnPlayerController..."));
	}

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

	FVector  Location = FVector(-450.0f, 50.0f, 100.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

	Tetromino = World->SpawnActor<ATetromino>(TetrominoBPClass, Location, Rotation);
	S6PlayerController = Cast<AS6PlayerController>(pc);

	if (S6PlayerController)
	{
		S6PlayerController->SetTetromino(Tetromino);
	}

	return pc;
}


