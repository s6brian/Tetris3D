// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tetromino.generated.h"

UCLASS()
class TETRIS3D_API ATetromino : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATetromino();

private:
	TArray<class UStaticMeshComponent*> BitCubes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void RefreshDisplay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveLeft();
	void MoveRight();
	void RotateCW();
	void RotateCCW();
//
//protected:
//	void DisplayLog(FString LogMessage);

};
