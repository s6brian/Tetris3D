// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TetrominoMatrix.h"
#include "Tetromino.generated.h"

UCLASS()
class TETRIS3D_API ATetromino : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATetromino();

/**
 *	VARIABLES | PROPERTIES
 */
private:
	TArray<class UStaticMeshComponent *> Blocks;
	TArray<FTetrominoMatrix> TetrominoShapesArray;
	FTetrominoMatrix CurrentShape;

public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh * BlockStaticMesh;

	UPROPERTY(EditDefaultsOnly)
	int32 BlockSize = 100;

	UPROPERTY(EditDefaultsOnly)
	float BlockScale = 0.9f;

/**
 *	FUNCTIONS
 */
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	void InitiateTetrominoShapes(TArray<FTetrominoMatrix> TetrominoShapes);

	void ResetPosition();
	void RefreshDisplay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateRandomTetromino();
	void SetBitmap(TArray<int32> NewBitmap);

	void MoveLeft();
	void MoveRight();
	void RotateCW();
	void RotateCCW();

};
