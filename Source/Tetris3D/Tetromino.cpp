// Fill out your copyright notice in the Description page of Project Settings.
#define SIZE 4
#define BLOCKS_COUNT SIZE * SIZE

#include "Tetromino.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ConstructorHelpers.h"

// Sets default values
ATetromino::ATetromino()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//true;
}

void ATetromino::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	UStaticMeshComponent * BlockStaticMeshComponent;

	FLinearColor LColor = HSVToRGB((float)(rand() % 255) / 255.0f, 0.8f, 0.9f);
	DynamicBlockMat = UMaterialInstanceDynamic::Create(BlockMat, this);
	DynamicBlockMat->SetVectorParameterValue(TILE_COLOR_PARAM_NAME, LColor);
	//DynamicBlockMat->SetScalarParameterValue(GLOW_INTENSITY_PARAM_NAME, 10);

	for (int32 Index = 0; Index < BLOCKS_COUNT; ++Index)
	{
		BlockStaticMeshComponent = NewObject<UStaticMeshComponent>(this, FName(*FString::Printf(TEXT("Block_%d"), Index)));
		BlockStaticMeshComponent->SetupAttachment(RootComponent);
		BlockStaticMeshComponent->SetRelativeLocation(FVector(0.0f, BlockSize * (Index % SIZE), BlockSize * (Index / SIZE)));
		BlockStaticMeshComponent->SetWorldScale3D(FVector(BlockScale));
		BlockStaticMeshComponent->SetStaticMesh(BlockStaticMesh);
		BlockStaticMeshComponent->SetMaterial(0, DynamicBlockMat);
		BlockStaticMeshComponent->SetVisibility(false);
		BlockStaticMeshComponent->RegisterComponent();

		Blocks.Add(BlockStaticMeshComponent);
	}
}

// Called when the game starts or when spawned
void ATetromino::BeginPlay()
{
	Super::BeginPlay();
	//GenerateRandomTetromino();

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ATetromino::BeginPlay"));
	//}
}

void ATetromino::InitiateTetrominoShapes(TArray<FTetrominoMatrix> TetrominoShapes)
{
	TetrominoShapesArray = TetrominoShapes;

	for (int idx = 0; idx < TetrominoShapesArray.Num(); ++idx)
	{
		TetrominoShapesArray[idx].InitializeBitmap();
	}
}

void ATetromino::GenerateRandomTetromino()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Generate Random Tetromino!"));
	//}

	// reset block locations
	for (int32 Index = 0; Index < BLOCKS_COUNT; ++Index)
	{
		Blocks[Index]->SetRelativeLocation(FVector(0.0f, BlockSize * (Index % SIZE), BlockSize * (Index / SIZE)));
	}

	DynamicBlockMat->SetVectorParameterValue(TILE_COLOR_PARAM_NAME, HSVToRGB((float)(rand() % 255) / 255.0f, 0.8f, 0.9f));
	CurrentShape = TetrominoShapesArray[FMath::RandRange(0, TetrominoShapesArray.Num()-1)];
	RefreshDisplay();
}

void ATetromino::SetBitmap(TArray<int32> NewBitmap)
{
	CurrentShape.SetBitmap(NewBitmap);
}

void ATetromino::SetBlockRelativeLocationXY(int32 PIndex, FVector2D PLocation)
{
	int32 ComputedIndex  = PIndex % CurrentShape.GetSize();
	      ComputedIndex += SIZE * (PIndex / CurrentShape.GetSize());

	FVector NewLocation;
	NewLocation.X = PLocation.X;
	NewLocation.Y = PLocation.Y;
	NewLocation.Z = BlockSize * (ComputedIndex / SIZE);
	Blocks[ComputedIndex]->SetRelativeLocation(NewLocation);
}

int32 ATetromino::GetSize() const
{
	return CurrentShape.GetSize();
}

TArray<int32> ATetromino::GetBitmap() const
{
	return CurrentShape.GetBitmap();
}

TArray<int32> ATetromino::GetGridIndeces(FVector2D GridDimension, int32 PSidesNum, FVector2D GridPoint) const
{
	TArray<int32> Indeces;
	TArray<int32> TetrominoBitmap = CurrentShape.GetBitmap();
	int32 TetrominoSize = CurrentShape.GetSize();
	int32 Perimeter = (GridDimension.X - 1) * PSidesNum;
	int32 ComputedGridIndex;

	for (int32 Row = 0; Row < TetrominoSize; ++Row)
	{
		for (int32 Col = 0; Col < TetrominoSize; ++Col)
		{
			ComputedGridIndex  = (( Row + GridPoint.Y ) * Perimeter) + ((Col + (int32)GridPoint.X) % Perimeter);
			Indeces.Add(ComputedGridIndex);
		}
	}

	return Indeces;
}

UMaterialInstanceDynamic * ATetromino::GetDynamicBlockMatInstance()
{
	return DynamicBlockMat;
}

void ATetromino::Copy(ATetromino * OtherTetromino)
{
	CurrentShape.SetBitmap(OtherTetromino->CurrentShape.GetBitmap());

	UMaterialInstanceDynamic * OtherTetrominoDynamicMat = OtherTetromino->GetDynamicBlockMatInstance();
	FLinearColor LColor;
	if (OtherTetrominoDynamicMat && OtherTetrominoDynamicMat->GetVectorParameterValue(TILE_COLOR_PARAM_NAME, LColor))
	{
		DynamicBlockMat->SetVectorParameterValue(TILE_COLOR_PARAM_NAME, LColor);
	}

	RefreshDisplay();
}

void ATetromino::RefreshDisplay()
{
	TArray<int32> TetrominoMap = CurrentShape.GetBitmap();
	int32 TetrominoSize        = CurrentShape.GetSize();
	int32 TetrominoBlockCount  = TetrominoSize * TetrominoSize;
	int32 ComputedIndex;

	// TODO: make this atmost O(n)
	//       currently this is O(2n)

	for (int32 idx = 0; idx < BLOCKS_COUNT; ++idx)
	{
		Blocks[idx]->SetVisibility(false);
	}

	for (int32 idx = 0; idx < TetrominoBlockCount; ++idx)
	{
		// row * col
		ComputedIndex  = idx % TetrominoSize;
		ComputedIndex += SIZE * (idx / TetrominoSize);
		Blocks[ComputedIndex]->SetVisibility(TetrominoMap[idx] == 1);
	}
}

FLinearColor ATetromino::HSVToRGB(float H, float S, float V)
{
	// sources:
	// (1) https://martin.ankerl.com/2009/12/09/how-to-create-random-colors-programmatically/
	// (2) https://www.programmingalgorithms.com/algorithm/hsv-to-rgb?lang=C%2B%2B

	int I;
	float F;
	float P;
	float Q;
	float T;

	if (S == 0.0f)
	{
		return FLinearColor(V, V, V);
	}
	else
	{
		I = roundf (H * 6);
		F = H * 6 - I;
		P = V * (1 - S);
		Q = V * (1 - F * S);
		T = V * (1 - (1 - F) * S);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("H: %.2f, S: %.2f, V: %.2f, LColor: "), H, S, V));

		switch (I)
		{
		case 0:
		{
			return FLinearColor(V, T, P);
		}
		case 1:
		{
			return FLinearColor(Q, V, P);
		}
		case 2:
		{
			return FLinearColor(P, V, T);
		}
		case 3:
		{
			return FLinearColor(P, Q, V);
		}
		case 4:
		{
			return FLinearColor(T, P, V);
		}
		default:
		{
			return FLinearColor(V, P, Q);
		}
		}
	}
}

void ATetromino::MoveLeft()
{
	//DisplayLog("Move Left!");
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Move Left!"));
	//}

	FVector NewLocation = GetActorLocation();
	NewLocation.Y -= 100.0f;
	SetActorLocation(NewLocation);
}

void ATetromino::MoveRight()
{
	//DisplayLog("Move Right!");
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Move Right!"));
	//}

	FVector NewLocation = GetActorLocation();
	NewLocation.Y += 100.0f;
	SetActorLocation(NewLocation);
}

void ATetromino::RotateCW()
{
	//FTetrominoMatrix CurrentTetromino = FTetrominoMatrix::L;
	//FTetrominoMatrix NextTetromino = FTetrominoMatrix::L;

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("current[0]: %d, next[0]: %d"), CurrentTetromino.GetBitMap()[0], NextTetromino.GetBitMap()[0]));
	//NextTetromino.GetBitMap()[0] = 20;
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("current[0]: %d, next[0]: %d"), CurrentTetromino.GetBitMap()[0], NextTetromino.GetBitMap()[0]));

	//DisplayLog("Clockwise Rotation!");
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Clockwise Rotation!"));
	//}

	CurrentShape.RotateClockwise();
	RefreshDisplay();
}

void ATetromino::RotateCCW()
{
	//DisplayLog("Counter-Clockwise Rotation!");
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Counter-Clockwise Rotation!"));
	//}

	CurrentShape.RotateCounterClockwise();
	RefreshDisplay();
}


