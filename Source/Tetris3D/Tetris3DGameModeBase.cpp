// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris3DGameModeBase.h"
#include "S6PlayerController.h"
#include "Tetromino.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"

//ATetris3DGameModeBase::ATetris3DGameModeBase()
//{
//	
//}

void ATetris3DGameModeBase::StartPlay()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("StartPlay..."));
	}

	Super::StartPlay();
	//UWorld * World = this->GetWorld();

	//if (!World)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("World does not exist!"));
	//	}

	//	return;
	//}

	//if (!TetrominoBPClass)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("TetrominoBPClass does not exist!"));
	//	}

	//	return;
	//}

	////if (!S6PlayerControllerBPClass)
	////{
	////	if (GEngine)
	////	{
	////		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("S6PlayerControllerBPClass does not exist!"));
	////	}

	////	return;
	////}

	//if (!PlayerControllerClass->GetDefaultObject()->IsA(AS6PlayerController::StaticClass()))
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PlayerControllerClass is not an AS6PlayerController!"));
	//	}

	//	return;
	//}

	//FVector  Location  = FVector (0.0f, 0.0f, 100.0f);
	//FRotator Rotation  = FRotator(0.0f, 0.0f,   0.0f);
	////PlayerControllerClass = S6PlayerControllerBPClass;
	//
	//Tetromino = World->SpawnActor<ATetromino>(TetrominoBPClass, Location, Rotation);
	////S6PlayerController = Cast<AS6PlayerController>(PlayerControllerClass);

	////if (S6PlayerController)
	////{
	////	S6PlayerController->SetTetromino(Tetromino);
	////}
}

APlayerController* ATetris3DGameModeBase::SpawnPlayerController(ENetRole InRemoteRole, FVector const & SpawnLocation, FRotator const & SpawnRotation)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("SpawnPlayerController..."));
	}

	APlayerController* pc = Super::SpawnPlayerController(InRemoteRole, SpawnLocation, SpawnRotation);
	S6PlayerController = Cast<AS6PlayerController>(pc);
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

	if (!PlayerControllerClass->GetDefaultObject()->IsA(AS6PlayerController::StaticClass()))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PlayerControllerClass is not an AS6PlayerController!"));
		}

		return pc;
	}

	FVector  Location = FVector(0.0f, 0.0f, 100.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

	Tetromino = World->SpawnActor<ATetromino>(TetrominoBPClass, Location, Rotation);

	if (S6PlayerController)
	{
		S6PlayerController->SetTetromino(Tetromino);
	}

	return pc;
}


