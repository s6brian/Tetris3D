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
			BlockStaticMeshComponent->SetRelativeLocation(FVector(0.0f, BlockSize * (Index % (int32)Dimension.X), BlockSize * ((Index + 1) / (int32)Dimension.X)));
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



