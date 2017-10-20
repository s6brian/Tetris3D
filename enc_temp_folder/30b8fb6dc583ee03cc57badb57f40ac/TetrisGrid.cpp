// Fill out your copyright notice in the Description page of Project Settings.
#define SPEED_FACTOR 2.0f

#include "TetrisGrid.h"
#include "Tetromino.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ATetrisGrid::ATetrisGrid()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("[ATetrisGrid Constructor] Dimension: (%0.2f, %0.2f)"), Dimension.X, Dimension.Y));
	//}

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ATetrisGrid::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UStaticMeshComponent * BlockStaticMeshComponent;
	int32 BlocksCount = Dimension.X * Dimension.Y;

	for (int32 Index = 0; Index < BlocksCount; ++Index)
	{
		BlockStaticMeshComponent = NewObject<UStaticMeshComponent>(this, FName(*FString::Printf(TEXT("Block_%d"), Index)));

		if (BlockStaticMeshComponent)
		{
			BlockStaticMeshComponent->SetupAttachment(RootComponent);
			BlockStaticMeshComponent->SetRelativeLocation(FVector(0.0f, BlockSize * (Index % (int32)Dimension.X), BlockSize * ((Index / (int32)Dimension.X) + 1)));
			BlockStaticMeshComponent->SetWorldScale3D(FVector(BlockScale));
			BlockStaticMeshComponent->SetStaticMesh(BlockStaticMesh);
			BlockStaticMeshComponent->SetVisibility(false);
			BlockStaticMeshComponent->RegisterComponent();

			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Add Block_%d at (%0.2f, %0.2f, %0.2f)"), Index, 0.0f, BlockSize * (Index % (int32)Dimension.X), BlockSize * (Index / Dimension.X)));
			//}

			Blocks.Add(BlockStaticMeshComponent);
			BitMap.Add(0);
		}
	}
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
		TryTetrominoDropOnce();
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

void ATetrisGrid::UpdateTetrominoPosition()
{
	FVector NewLocation = CurrentTetromino->GetActorLocation();
	NewLocation.Y = CurrentTetromino->BlockSize * Point.X;
	NewLocation.Z = CurrentTetromino->BlockSize * Point.Y;
	CurrentTetromino->SetActorLocation(NewLocation);
}

void ATetrisGrid::StartMergeTimer()
{
	TArray<int32> TetrominoBitmap = CurrentTetromino->GetBitmap();
	int32 TetrominoSize = CurrentTetromino->GetSize();
	int32 ComputedTetrominoIndex;
	int32 ComputedGridIndex;

	for (int32 Row = 0; Row < TetrominoSize; ++Row)
	{
		for (int32 Col = 0; Col < TetrominoSize; ++Col)
		{
			ComputedTetrominoIndex = ( Row * TetrominoSize ) + Col;
			ComputedGridIndex = (( Row + Point.Y ) * Dimension.X ) + Col + Point.X;

			if (   ComputedGridIndex < 0
				|| ComputedGridIndex >= (Dimension.X * Dimension.Y)
				|| TetrominoBitmap[ComputedTetrominoIndex] == 0)
			{
				continue;
			}

			BitMap[ComputedGridIndex] = TetrominoBitmap[ComputedTetrominoIndex];
			Blocks[ComputedGridIndex]->SetVisibility(true);
		}
	}

	CurrentTetromino->Copy(NextTetromino);
	NextTetromino->GenerateRandomTetromino();

	Point = FVector2D(4.0f, Dimension.Y);
	UpdateTetrominoPosition();
}

bool ATetrisGrid::DidHitABlock()
{
	//bool HasHit = false;
	//if (Point.Y <= 0.0f)
	//{
	//	return true;
	//}

	TArray<int32> TetrominoBitmap = CurrentTetromino->GetBitmap();
	int32 TetrominoSize = CurrentTetromino->GetSize();
	int32 ComputedTetrominoIndex;
	int32 ComputedGridIndex;

	for (int32 Row = 0; Row < TetrominoSize; ++Row)
	{
		for (int32 Col = 0; Col < TetrominoSize; ++Col)
		{
			ComputedTetrominoIndex = ( Row * TetrominoSize ) + Col;
			ComputedGridIndex = (( Row + Point.Y ) * Dimension.X ) + Col + Point.X;

			if (   ComputedGridIndex < 0
				|| ComputedGridIndex >= (Dimension.X * Dimension.Y)
				|| TetrominoBitmap[ComputedTetrominoIndex] == 0)
			{
				continue;
			}

			if ( BitMap[ComputedGridIndex] == 1 )
			{
				return true;
			}
		}
	}

	return false;
}

void ATetrisGrid::TryTetrominoDropOnce()
{
	Point.Y -= 1.0f;

	if (DidHitABlock())
	{
		Point.Y += 1.0f;
		StartMergeTimer();
	}
	else if(Point.Y >= 0)
	{
		UpdateTetrominoPosition();
	}
	else
	{
		Point.Y = 0;
		StartMergeTimer();
	}
}

void ATetrisGrid::TryTetrominoMoveLeft()
{
	Point.X -= 1.0f;

	if (DidHitABlock())
	{
		Point.X += 1.0f;
	}
	else
	{
		UpdateTetrominoPosition();
	}
}

void ATetrisGrid::TryTetrominoMoveRight()
{
	Point.X += 1.0f;

	if (DidHitABlock())
	{
		Point.X -= 1.0f;
	}
	else
	{
		UpdateTetrominoPosition();
	}
}

void ATetrisGrid::TryTetrominoRotateCW()
{
	CurrentTetromino->RotateCW();

	// TODO: check if kick is necessary
}

void ATetrisGrid::TryTetrominoRotateCCW()
{
	CurrentTetromino->RotateCCW();

	// TODO: check if kick is necessary
}


