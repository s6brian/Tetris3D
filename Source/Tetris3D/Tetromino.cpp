// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetromino.h"
#include "../Helpers/TetrominoMatrix.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"


// Sets default values
ATetromino::ATetromino()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	//RootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent")); //CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//RootComponent->SetWorldLocation(FVector(-450.0f, 50.0f, 100.0f));

	//// initialize bit cubes (4x4 matrix, max)
	//UStaticMeshComponent * BitCube;
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	////GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TRY ASSET!"));

	//if (CubeAsset.Succeeded())
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("SUCCESS!"));

	//	for (int idx = 0; idx < 1; ++idx)
	//	{
	//		BitCube = CreateDefaultSubobject<UStaticMeshComponent>(FName(*FString::Printf(TEXT("BitCube%d"), idx)));
	//		BitCube->SetupAttachment(RootComponent);
	//		BitCube->SetStaticMesh(CubeAsset.Object);
	//		BitCube->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//		BitCube->SetWorldScale3D(FVector(0.9f));
	//	}
	//}
}

// Called when the game starts or when spawned
void ATetromino::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATetromino::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//// Called to bind functionality to input
//void ATetromino::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

void ATetromino::RefreshDisplay()
{

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
	//NewLocation.X -= 50.0f;
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, NewLocation.ToString());
	//UE_LOG(LogTemp, Log, TEXT("prev loc: %s"), NewLocation.ToString());

	NewLocation.Y += 100.0f;
	//NewLocation.X += 50.0f;
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, NewLocation.ToString());
	//UE_LOG(LogTemp, Log, TEXT("new loc : %s"), NewLocation.ToString());

	SetActorLocation(NewLocation);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, GetActorLocation().ToString());
	//UE_LOG(LogTemp, Log, TEXT("curr loc: %s"), NewLocation.ToString());
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
}

void ATetromino::RotateCCW()
{
	//DisplayLog("Counter-Clockwise Rotation!");
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Counter-Clockwise Rotation!"));
	//}
}

//void ATetromino::DisplayLog(FString LogMessage)
//{
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT(LogMessage));
//	}
//}

