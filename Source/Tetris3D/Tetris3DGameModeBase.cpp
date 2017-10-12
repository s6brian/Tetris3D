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
	Super::StartPlay();

	UWorld * World = this->GetWorld();
	if (!World) {return;}

	for (FConstPlayerControllerIterator PlayerControllerIterator = World->GetPlayerControllerIterator(); PlayerControllerIterator; PlayerControllerIterator++)
	{
		S6PlayerController = Cast<AS6PlayerController>(*PlayerControllerIterator);
		if (S6PlayerController)
		{
			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("S6 Controller valid!"));
			//}
			
			break;
		}
	}

	//static ConstructorHelpers::FClassFinder<AS6PlayerController> S6PlayerControllerBP(TEXT("Blueprint'/Game/Blueprints/BP_S6PlayerController.BP_S6PlayerController_C'"));
	//if (S6PlayerControllerBP.Class != NULL)
	//{
	//	S6PlayerController = Cast<AS6PlayerController>(S6PlayerControllerBP.Class);
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("S6 Controller valid!"));
	//	}
	//}

	for (FConstPawnIterator PawnIterator = World->GetPawnIterator(); PawnIterator; PawnIterator++)
	{
		Tetromino = Cast<ATetromino>(*PawnIterator);

		if (Tetromino)// && GEngine)
		{
			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Tetromino->GetName());
			//}
			
			if (S6PlayerController)
			{
				S6PlayerController->SetTetromino(Tetromino);
			}

			break;
		}
	}

	//static ConstructorHelpers::FClassFinder<ATetromino> TetrominoBP(TEXT("Blueprint'/Game/Blueprints/BP_Tetromino.BP_Tetromino_C'"));
	//UClass * TetrominoBPClass = TetrominoBP.Class;
	//UWorld * World = this->GetWorld();

	//if (World && TetrominoBPClass)//(TetrominoBP.Class != NULL)
	//{
	//	FVector  Location = FVector(0.0f, 0.0f, 100.0f);
	//	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

	//	Tetromino = World->SpawnActor<ATetromino>(TetrominoBPClass, Location, Rotation);
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Valid TetrominoBP!"));
	//}

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Tetris Game Mode created!"));
	//}
}


