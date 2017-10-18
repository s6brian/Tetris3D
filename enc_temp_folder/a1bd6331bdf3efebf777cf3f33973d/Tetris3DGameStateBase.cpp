// Fill out your copyright notice in the Description page of Project Settings.
#define SPEED_FACTOR 2.0f

#include "Tetris3DGameStateBase.h"
#include "Tetromino.h"

ATetris3DGameStateBase::ATetris3DGameStateBase()
{
	Speed		 = 5.0f;
	GridSize	 = FVector2D (10.0f, 20.0f);
	GridPosition = FVector2D ();
}

void ATetris3DGameStateBase::DoTick(float DeltaTime)
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ATetris3DGameStateBase::Tick!"));
	//}

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
			NewLocation.Z -= CurrentTetromino->BlockSize * GridSize.Y;
			CurrentTetromino->SetActorLocation(NewLocation);

			CurrentTetromino->Copy(NextTetromino);
			NextTetromino->GenerateRandomTetromino();
		}
	}
	else
	{
		LapsedTime += DeltaTime;

		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("lapse time: %0.2f"), LapsedTime));
		//}
	}
}

void ATetris3DGameStateBase::SetTetrominoes(ATetromino * PCurrentTetromino, ATetromino * PNextTetromino)
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
	GridPosition = FVector2D(4.0f, 10.0f);

	CurrentTetromino->SetActorLocation(FVector(0.0f, CurrentTetromino->BlockSize * GridPosition.X, CurrentTetromino->BlockSize * GridPosition.Y));
	NextTetromino->SetActorLocation(FVector(0.0f, -450.0f, 200.0f));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GameState Set Tetrominoes..."));
	}
}


