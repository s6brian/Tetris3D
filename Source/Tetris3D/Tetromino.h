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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bri")
	TArray<class UStaticMeshComponent *> Blocks;

	UPROPERTY(EditAnywhere)
	UStaticMesh * BlockStaticMesh;

	UPROPERTY(EditAnywhere)
	int32 BlockSize = 100;

	UPROPERTY(EditAnywhere)
	float BlockScale = 0.9f;

	FTetrominoMatrix CurrentBlockMap;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	void GenerateRandomTetromino();
	void ResetPosition();
	void RefreshDisplay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveLeft();
	void MoveRight();
	void RotateCW();
	void RotateCCW();

};
