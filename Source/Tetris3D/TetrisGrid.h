// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisGrid.generated.h"

UCLASS()
class TETRIS3D_API ATetrisGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetrisGrid();

protected:
	FVector2D Dimension;
	FVector2D Point;

	class ATetromino * CurrentTetromino;
	class ATetromino * NextTetromino;

	float Speed;
	float LapsedTime;

	TArray<int32> Cells;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetTetrominoes(ATetromino * PCurrentTetromino, ATetromino * PNextTetromino);

};
