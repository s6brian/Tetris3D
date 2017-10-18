// Fill out your copyright notice in the Description page of Project Settings.
#define SPEED_FACTOR 2.0f

#include "TetrisGrid.h"
#include "Tetromino.h"


// Sets default values
ATetrisGrid::ATetrisGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Speed = 5.0f;
	Dimension = FVector2D(10.0f, 20.0f);
}

// Called when the game starts or when spawned
void ATetrisGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATetrisGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentTetromino || !NextTetromino)
	{
		return;
	}

	if (LapsedTime >= SPEED_FACTOR / Speed)
	{
		LapsedTime = 0.0f;
		FVector NewLocation = CurrentTetromino->GetActorLocation();
		NewLocation.Z -= CurrentTetromino->BlockSize;
		CurrentTetromino->SetActorLocation(NewLocation);

		if (NewLocation.Z <= 0.0f)
		{
			// reset
			NewLocation = CurrentTetromino->GetActorLocation();
			NewLocation.Z = CurrentTetromino->BlockSize * Dimension.Y;
			CurrentTetromino->SetActorLocation(NewLocation);

			CurrentTetromino->Copy(NextTetromino);
			NextTetromino->GenerateRandomTetromino();
		}
	}
	else
	{
		LapsedTime += DeltaTime;
	}

}

void ATetrisGrid::SetTetrominoes(ATetromino * PCurrentTetromino, ATetromino * PNextTetromino)
{
	CurrentTetromino = PCurrentTetromino;
	NextTetromino = PNextTetromino;

	if (!CurrentTetromino || !NextTetromino)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No tetromino available!"));
		}

		return;
	}

	LapsedTime = 0.0f;
	Point = FVector2D(4.0f, Dimension.Y);

	CurrentTetromino->SetActorLocation(FVector(0.0f, CurrentTetromino->BlockSize * Point.X, CurrentTetromino->BlockSize * Point.Y));
	NextTetromino->SetActorLocation(FVector(0.0f, -450.0f, 200.0f));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GameState Set Tetrominoes..."));
	}
}

