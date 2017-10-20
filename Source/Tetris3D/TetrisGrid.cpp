// Fill out your copyright notice in the Description page of Project Settings.
#define SPEED_FACTOR 2.0f
#define CLEAR_ROW_FPS (1.0f / 60.0f)

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
	IsClearing = false;
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
		ClearRowAnimation(DeltaTime);
		break;
	}

	case EGridState::ClearRowCleanup:
	{
		GridCleanup();
		break;
	}

	default:
	{
		TryTetrominoDropOnce(DeltaTime);
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
	//CurrentGridState = EGridState::ClearRowCleanup;
}

void ATetrisGrid::ClearRowAnimation(float DeltaTime)
{
	if (IsClearing)
	{
		// hide 1 column at a time
		if (LapsedTime <= CLEAR_ROW_FPS)
		{
			LapsedTime += DeltaTime;
			return;
		}

		int32 ComputedIndex = 0;
		
		for (int32 Index = RowIndeces.Num()-1; Index >= 0; Index -= 2)
		{
			ComputedIndex = (RowIndeces[Index] * Dimension.X) + ColumnIndex;

			BitMap[ComputedIndex] = 0;
			Blocks[ComputedIndex]->SetVisibility(false);

			if (Index - 1 < 0)
			{
				continue;
			}

			ComputedIndex = (RowIndeces[Index - 1] * Dimension.X) + (Dimension.X - ColumnIndex - 1);
			BitMap[ComputedIndex] = 0;
			Blocks[ComputedIndex]->SetVisibility(false);
		}

		ColumnIndex++;
		LapsedTime = 0.0f;
		
	}
	else // cache row indeces to be cleared
	{
		int32 RowValue = 0;
		ColumnIndex    = 0;
		RowIndeces.Empty();

		for (int32 Row = 0; Row < Dimension.Y; ++Row)
		{
			RowValue = 0;

			for (int32 Col = 0; Col < Dimension.X; ++Col)
			{
				RowValue += BitMap[(Row * Dimension.X) + Col];
			}

			if (RowValue == Dimension.X)
			{
				RowIndeces.Add(Row);
			}
		}

		IsClearing = RowIndeces.Num() > 0;
	}

	if (!IsClearing || ColumnIndex >= Dimension.X)
	{
		LapsedTime       = 0.0f;
		ColumnIndex      = 0;
		IsClearing       = false;
		CurrentGridState = EGridState::ClearRowCleanup;
	}
}

void ATetrisGrid::GridCleanup()
{
	int32 RowIndecesCount    = RowIndeces.Num();
	int32 NextCachedRowIndex = 0;
	int32 ComputedIndexA     = 0;
	int32 ComputedIndexB     = 0;

	for (int32 CachedRowIndex = RowIndecesCount - 1; CachedRowIndex >= 0; --CachedRowIndex)
	{
		NextCachedRowIndex = (CachedRowIndex < RowIndecesCount - 1) ? RowIndeces[CachedRowIndex + 1]: Dimension.Y - 1;

		for (int32 GridRowIndex = RowIndeces[CachedRowIndex]; GridRowIndex < NextCachedRowIndex; ++GridRowIndex)
		{
			for (int32 GridColumnIndex = 0; GridColumnIndex < Dimension.X; ++GridColumnIndex)
			{
				ComputedIndexA = (GridRowIndex       * Dimension.X) + GridColumnIndex;
				ComputedIndexB = ((GridRowIndex + 1) * Dimension.X) + GridColumnIndex;

				BitMap[ComputedIndexA] = BitMap[ComputedIndexB];
				BitMap[ComputedIndexB] = 0;

				Blocks[ComputedIndexA]->SetVisibility(BitMap[ComputedIndexA] == 1);
				Blocks[ComputedIndexB]->SetVisibility(false);
			}
		}
	}

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

void ATetrisGrid::TryTetrominoDropOnce(float DeltaTime)
{
	if (LapsedTime <= SPEED_FACTOR / Speed)
	{
		LapsedTime += DeltaTime;
		return;
	}

	LapsedTime = 0.0f;
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


