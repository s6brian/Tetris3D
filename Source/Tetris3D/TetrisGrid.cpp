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
	CurrentGridState = Default;
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

	switch (CurrentGridState) {
	case EGridState::CopyTetromino:
	{
		break;
	}
	
	case EGridState::ClearRowAnimation:
	{
		ClearRowAnimation();
		break;
	}

	case EGridState::ClearRowCleanup:
	{
		GridCleanup();
		break;
	}

	default:
	{
		if (LapsedTime >= SPEED_FACTOR / Speed)
		{
			LapsedTime = 0.0f;
			TryTetrominoDropOnce();
		}
		else
		{
			LapsedTime += DeltaTime;
		}

		break;
	}}
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
	NewLocation.Z = CurrentTetromino->BlockSize * (Point.Y + 1);
	CurrentTetromino->SetActorLocation(NewLocation);
}

void ATetrisGrid::StartMergeTimer()
{
	TArray<int32> GridIndeces     = CurrentTetromino->GetGridIndeces(Dimension, Point);
	TArray<int32> TetrominoBitmap = CurrentTetromino->GetBitmap();
	int32 TBitmapLength           = TetrominoBitmap.Num();
	int32 GBitmapLength           = BitMap.Num();

	for (int Index = 0; Index < TBitmapLength; ++Index)
	{
		if (   GridIndeces[Index]     <  0
			|| GridIndeces[Index]     >= GBitmapLength
			|| TetrominoBitmap[Index] == 0             )
		{
			continue;
		}

		BitMap[GridIndeces[Index]] = TetrominoBitmap[Index];
		Blocks[GridIndeces[Index]]->SetVisibility(true);
	}

	LapsedTime = 0.0f;
	CurrentGridState = EGridState::ClearRowAnimation;
}

void ATetrisGrid::ClearRowAnimation()
{


	CurrentGridState = EGridState::ClearRowCleanup;
}

void ATetrisGrid::GridCleanup()
{
	CurrentTetromino->Copy(NextTetromino);
	NextTetromino->GenerateRandomTetromino();

	Point = FVector2D(4.0f, Dimension.Y);
	UpdateTetrominoPosition();

	CurrentGridState = EGridState::Default;
}

bool ATetrisGrid::DidHitABlock()
{
	TArray<int32> GridIndeces     = CurrentTetromino->GetGridIndeces(Dimension, Point);
	TArray<int32> TetrominoBitmap = CurrentTetromino->GetBitmap();
	int32 TBitmapLength           = TetrominoBitmap.Num();
	int32 GBitmapLength           = BitMap.Num();

	for (int Index = 0; Index < TBitmapLength; ++Index)
	{
		if (GridIndeces[Index] >= GBitmapLength || TetrominoBitmap[Index] == 0)
		{
			continue;
		}

		if (GridIndeces[Index] < 0 || BitMap[GridIndeces[Index]] == 1)
		{
			return true;
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
}

void ATetrisGrid::InstantDrop()
{
	while (!DidHitABlock())
	{
		Point.Y -= 1.0f;
	}

	Point.Y += 1.0f;
	StartMergeTimer();
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


