// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetromino.h"
#include "Components/InputComponent.h"


// Sets default values
ATetromino::ATetromino()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
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

// Called to bind functionality to input
void ATetromino::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATetromino::MoveLeft()
{
	//DisplayLog("Move Left!");
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Move Left!"));
	}

	FVector NewLocation = GetActorLocation();
	NewLocation.Y -= 50.0f;
	//NewLocation.X -= 50.0f;
	SetActorLocation(NewLocation);
}

void ATetromino::MoveRight()
{
	//DisplayLog("Move Right!");
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Move Right!"));
	}

	FVector NewLocation = GetActorLocation();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, NewLocation.ToString());
	//UE_LOG(LogTemp, Log, TEXT("prev loc: %s"), NewLocation.ToString());

	NewLocation.Y += 50.0f;
	//NewLocation.X += 50.0f;
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, NewLocation.ToString());
	//UE_LOG(LogTemp, Log, TEXT("new loc : %s"), NewLocation.ToString());

	SetActorLocation(NewLocation);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, GetActorLocation().ToString());
	//UE_LOG(LogTemp, Log, TEXT("curr loc: %s"), NewLocation.ToString());
}

void ATetromino::RotateCW()
{
	//DisplayLog("Clockwise Rotation!");
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Clockwise Rotation!"));
	}
}

void ATetromino::RotateCCW()
{
	//DisplayLog("Counter-Clockwise Rotation!");
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Counter-Clockwise Rotation!"));
	}
}

//void ATetromino::DisplayLog(FString LogMessage)
//{
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT(LogMessage));
//	}
//}

