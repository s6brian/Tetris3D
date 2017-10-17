// Fill out your copyright notice in the Description page of Project Settings.
#define SIZE 4
#define BLOCKS_COUNT SIZE * SIZE

#include "Tetromino.h"
//#include "../Utilities/TetrominoMatrix.h"
//#include "TetrominoMatrix.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"


// TODO: this should be declared in header file
//FTetrominoMatrix CurrentBlockMap;

// Sets default values
ATetromino::ATetromino()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	UStaticMeshComponent * BlockStaticMeshComponent;
	for (int32 idx = 0; idx < BLOCKS_COUNT; ++idx)
	{
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, FString::Printf(TEXT("col: %d, row: %d"), (idx % SIZE), (idx / SIZE)));
		//	GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, FString::Printf(TEXT("y: %0.2f, z: %0.2f"), BlockSize * (idx % SIZE), BlockSize * (idx / SIZE)));
		//}

		BlockStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(*FString::Printf(TEXT("Block_%d"), idx)));
		BlockStaticMeshComponent->SetupAttachment(RootComponent);
		BlockStaticMeshComponent->SetRelativeLocation(FVector(0.0f, BlockSize * (idx % SIZE), BlockSize * (idx / SIZE)));
		BlockStaticMeshComponent->SetWorldScale3D(FVector(BlockScale));
		BlockStaticMeshComponent->SetVisibility(false);
		Blocks.Add(BlockStaticMeshComponent);
	}
}

void ATetromino::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	if (BlockStaticMesh)
	{
		for (int32 idx = 0; idx < BLOCKS_COUNT; ++idx)
		{
			Blocks[idx]->SetStaticMesh(BlockStaticMesh);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("BlockStaticMesh does not exist!"));
		}
	}
}

// Called when the game starts or when spawned
void ATetromino::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateRandomTetromino();
	RefreshDisplay();
}

// Called every frame
void ATetromino::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATetromino::InitiateTetrominoShapes(TArray<FTetrominoMatrix> TetrominoShapes)
{
	//for (int idx = 0; idx < TetrominoShapes.Num(); idx++)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("[%d] Init Tetromino Shapes!"), idx));
	//	}
	//}

	//for (int idx = 0; idx < TetrominoShapes[0].GetBitMap().Num(); ++idx)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("[%d] = %d"), idx, TetrominoShapes[0].GetBitMap()[idx]));
	//	}
	//}

	TetrominoShapesArray = TetrominoShapes;

	for (int idx = 0; idx < TetrominoShapesArray.Num(); ++idx)
	{
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("InitializeBitmap!"));
		//}

		TetrominoShapesArray[idx].InitializeBitmap();
	}
}

void ATetromino::GenerateRandomTetromino()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Generate Random Tetromino!"));
	}

	CurrentShape = TetrominoShapesArray[FMath::RandRange(0, TetrominoShapesArray.Num()-1)];

	//for (int idx = 0; idx < CurrentBlockMap.GetBitMap().Num(); ++idx)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d"), CurrentBlockMap.GetBitMap()[idx]));
	//	}
	//}

}

void ATetromino::SetBitmap(TArray<int32> NewBitmap)
{
	CurrentShape.SetBitmap(NewBitmap);
}

void ATetromino::ResetPosition()
{

}

void ATetromino::RefreshDisplay()
{
	TArray<int32> TetrominoMap = CurrentShape.GetBitMap();
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


