// Fill out your copyright notice in the Description page of Project Settings.
#define SPEED_FACTOR 2.0f
#define CLEAR_ROW_FPS (1.0f / 60.0f)
#define MOVE_SPEED (1.0f / 30.0f)

#include "TetrisGrid.h"
#include "Tetromino.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/World.h"

// Sets default values
ATetrisGrid::ATetrisGrid()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("[ATetrisGrid Constructor] Dimension: (%0.2f, %0.2f)"), Dimension.X, Dimension.Y));
	//}

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsClearing                    = false;
	IsMoveLeftActive              = false;
	IsMoveRightActive             = false;

	CurrentGridState              = Default;
	LapsedTime                    = 0.0f;
	MoveLapseTime                 = 0.0f;
	SoftDropMultiplier            = 1.0f;
}

void ATetrisGrid::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// add blocks
	UStaticMeshComponent * BlockStaticMeshComponent;
	int32 BlocksCount = Dimension.X * Dimension.Y * Sides;

	for (int32 Index = 0; Index < BlocksCount; ++Index)
	{
		BlockStaticMeshComponent = NewObject<UStaticMeshComponent>(this, FName(*FString::Printf(TEXT("Block_%d"), Index)));

		if (BlockStaticMeshComponent)
		{
			BlockStaticMeshComponent->SetupAttachment(RootComponent);
			BlockStaticMeshComponent->SetRelativeLocation(GetGridCoordinates(Index));
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

	// add scene capture 2D component
	NextTetrominoView = NewObject<USceneCaptureComponent2D>(this, FName(TEXT("NextTetrominoView")));
	if (NextTetrominoView && NextTetrominoRenderTexture2D)
	{
		NextTetrominoView->TextureTarget = NextTetrominoRenderTexture2D;
		NextTetrominoView->SetWorldLocation(NEXT_TETROMINO_LOCATION);
		NextTetrominoView->RegisterComponent();
	}
}

// Called when the game starts or when spawned
void ATetrisGrid::BeginPlay()
{
	Super::BeginPlay();

	CurrentTetromino->GenerateRandomTetromino();
	NextTetromino->GenerateRandomTetromino();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ATetrisGrid::BeginPlay"));
	}
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
		TetrominoTickDrop(DeltaTime);
		TetrominoMove(DeltaTime);
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

	//UpdateTetrominoPosition();
	CurrentTetromino->SetActorLocation(NEXT_TETROMINO_LOCATION + FVector(  0.0f, 0.0f, 500.0f));
	NextTetromino   ->SetActorLocation(NEXT_TETROMINO_LOCATION + FVector(300.0f, 0.0f,   0.0f));
	RefreshNextTetrominoView();
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GameState Set Tetrominoes..."));
	//}
}

void ATetrisGrid::UpdateTetrominoPosition()
{
	// update vertical position
	FVector NewLocation = CurrentTetromino->GetActorLocation();
	//NewLocation.Y = CurrentTetromino->BlockSize * Point.X;
	NewLocation.Z = CurrentTetromino->BlockSize * (Point.Y + 1);
	CurrentTetromino->SetActorLocation(NewLocation);

	// update horizontal position
	TArray<int32> GridIndeces = CurrentTetromino->GetGridIndeces(Dimension, Sides, Point);
	for (int32 Index = GridIndeces.Num() - 1; Index >= 0; --Index)
	{
		NewLocation = GetGridCoordinates(GridIndeces[Index]);
		CurrentTetromino->SetBlockRelativeLocationXY(Index, FVector2D(NewLocation.X, NewLocation.Y));
	}
}

void ATetrisGrid::RefreshNextTetrominoView()
{
	// center next tetromino in viewport
	FVector Center = NextTetromino->GetActorLocation();
	Center.Y = NEXT_TETROMINO_LOCATION.Y - (BlockSize * NextTetromino->GetSize() * 0.5f);
	Center.Z = NEXT_TETROMINO_LOCATION.Z - (BlockSize * NextTetromino->GetSize() * 0.5f);
	NextTetromino->SetActorLocation(Center);
}

void ATetrisGrid::StartMergeTimer()
{
	TArray<int32> GridIndeces     = CurrentTetromino->GetGridIndeces(Dimension, Sides, Point);
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
	int32 Perimeter = (Dimension.X - 1) * Sides;

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
			ComputedIndex = (RowIndeces[Index] * Perimeter) + ColumnIndex;

			BitMap[ComputedIndex] = 0;
			Blocks[ComputedIndex]->SetVisibility(false);

			if (Index - 1 < 0)
			{
				continue;
			}

			ComputedIndex = (RowIndeces[Index - 1] * Perimeter) + (Perimeter - ColumnIndex - 1);
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

			for (int32 Col = 0; Col < Perimeter; ++Col)
			{
				RowValue += BitMap[(Row * Perimeter) + Col];
			}

			if (RowValue == Perimeter)
			{
				RowIndeces.Add(Row);
			}
		}

		IsClearing = RowIndeces.Num() > 0;
	}

	if (!IsClearing || ColumnIndex >= Perimeter)
	{
		LapsedTime       = 0.0f;
		ColumnIndex      = 0;
		IsClearing       = false;
		CurrentGridState = EGridState::ClearRowCleanup;
	}
}

void ATetrisGrid::GridCleanup()
{
	int32 Perimeter          = (Dimension.X - 1) * Sides;
	int32 RowIndecesCount    = RowIndeces.Num();
	int32 RowValue           = 0;
	int32 ComputedIndexA     = 0;
	int32 ComputedIndexB     = 0;

	// fill each cleared out row
	for (int32 CachedRowIndex = RowIndecesCount - 1; CachedRowIndex >= 0; --CachedRowIndex)
	{
		// drop all rows above the cleared row
		for (int32 GridRowIndex = RowIndeces[CachedRowIndex]; GridRowIndex < Dimension.Y - 1; ++GridRowIndex)
		{
			RowValue = 0;

			// drop each block in current row
			for (int32 GridColumnIndex = 0; GridColumnIndex < Perimeter; ++GridColumnIndex)
			{
				ComputedIndexA = (GridRowIndex       * Perimeter) + GridColumnIndex;
				ComputedIndexB = ((GridRowIndex + 1) * Perimeter) + GridColumnIndex;

				RowValue += BitMap[ComputedIndexB];

				BitMap[ComputedIndexA] = BitMap[ComputedIndexB];
				BitMap[ComputedIndexB] = 0;

				Blocks[ComputedIndexA]->SetVisibility(BitMap[ComputedIndexA] == 1);
				Blocks[ComputedIndexB]->SetVisibility(false);
			}

			// all rows with visible blocks dropped
			// proceed filling next cleared row
			if (RowValue <= 0)
			{
				break;
			}
		}
	}

	CurrentTetromino->Copy(NextTetromino);
	NextTetromino->GenerateRandomTetromino();
	RefreshNextTetrominoView();

	Point = FVector2D(4.0f, Dimension.Y);
	UpdateTetrominoPosition();

	CurrentGridState = EGridState::Default;
}

FVector ATetrisGrid::GetGridCoordinates(int32 PGridIndex)
{
	FVector Position = FVector::OneVector;
	int32 SideIndex;
	float Sine;
	float Cosine;

	SideIndex = (PGridIndex / (int32)(Dimension.X - 1)) % (int32)Sides;
	FMath::SinCos(&Sine, &Cosine, FMath::DegreesToRadians(90.0f * SideIndex));

	// forward | backward
	Position.X  = ((BlockSize * ( PGridIndex % (int32)(Dimension.X - 1   ))) - (BlockSize * (Dimension.X - 1) * (SideIndex / 2))) * Sine;
	Position.X += ((Dimension.X - 1) * BlockSize * -Cosine * 0.5f) + ((Dimension.X - 1) * BlockSize * FMath::Abs(Cosine) * 0.5f);

	// right | left
	Position.Y  = ((BlockSize * ( PGridIndex % (int32)(Dimension.X - 1   ))) - (BlockSize * (Dimension.X - 1) * (SideIndex / 2))) * Cosine; 
	Position.Y += ((Dimension.X - 1) * BlockSize * Sine * 0.5f) + ((Dimension.X - 1) * BlockSize * FMath::Abs(Sine) * 0.5f);

	// up | down
	Position.Z  = BlockSize * ((PGridIndex / (int32)((Dimension.X - 1) * Sides)) + 1);

	return Position;
}

bool ATetrisGrid::DidHitABlock()
{
	TArray<int32> GridIndeces     = CurrentTetromino->GetGridIndeces(Dimension, Sides, Point);
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

void ATetrisGrid::TetrominoTickDrop(float DeltaTime)
{
	if (LapsedTime <= SPEED_FACTOR / (Speed * SoftDropMultiplier))
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

void ATetrisGrid::TetrominoMove(float Deltatime)
{
	if (MoveLapseTime <= MOVE_SPEED + MoveLapseDelay)
	{
		MoveLapseTime += Deltatime;
		return;
	}

	MoveLapseDelay = 0.0f;
	MoveLapseTime = 0.0f;
	float Offset = 0.0f;

	if (IsMoveRightActive)
	{
		Offset = 1.0f;
	}
	else if (IsMoveLeftActive)
	{
		Offset = -1.0f;
	}

	if (Offset != 0.0f)
	{
		Point.X += Offset;

		if (DidHitABlock())
		{
			Point.X -= Offset;
		}
		else
		{
			UpdateTetrominoPosition();
		}
	}
}

void ATetrisGrid::SoftDropStart()
{
	SoftDropMultiplier = 8.0f;
}

void ATetrisGrid::HardDrop()
{
	while (!DidHitABlock())
	{
		Point.Y -= 1.0f;
	}

	Point.Y += 1.0f;
	StartMergeTimer();
}

void ATetrisGrid::TetrominoMoveLeftStart()
{
	TetrominoMoveRightEnd();
	IsMoveLeftActive = true;
	MoveLapseDelay = MOVE_SPEED * 10.0f;
	MoveLapseTime = 0.0f;
	Point.X -= 1.0f;

	if (Point.X < 0)
	{
		Point.X = ((Dimension.X - 1) * Sides) - 1;
	}

	if (DidHitABlock())
	{
		Point.X += 1.0f;
		Point.X  = (int32)Point.X % (int32)((Dimension.X - 1) * Sides);
	}
	else
	{
		UpdateTetrominoPosition();
	}
}

void ATetrisGrid::TetrominoMoveRightStart()
{
	TetrominoMoveLeftEnd();
	IsMoveRightActive = true;
	MoveLapseDelay = MOVE_SPEED * 10.0f;
	MoveLapseTime = 0.0f;
	Point.X += 1.0f;
	Point.X  = (int32)Point.X % (int32)((Dimension.X - 1) * Sides);

	if (DidHitABlock())
	{
		Point.X -= 1.0f;
		if (Point.X < 0)
		{
			Point.X = ((Dimension.X - 1) * Sides) - 1;
		}
	}
	else
	{
		UpdateTetrominoPosition();
	}
}

void ATetrisGrid::TetrominoRotateCW()
{
	CurrentTetromino->RotateCW();

	// TODO: check if kick is necessary
}

void ATetrisGrid::TetrominoRotateCCW()
{
	CurrentTetromino->RotateCCW();

	// TODO: check if kick is necessary
}

void ATetrisGrid::SoftDropEnd()
{
	SoftDropMultiplier = 1.0f;
}

void ATetrisGrid::TetrominoMoveLeftEnd()
{
	IsMoveLeftActive = false;
}

void ATetrisGrid::TetrominoMoveRightEnd()
{
	IsMoveRightActive = false;
}



