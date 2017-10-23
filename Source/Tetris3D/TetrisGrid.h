// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisGrid.generated.h"

UENUM()
enum EGridState
{
	Default,
	CopyTetromino,
	ClearRowAnimation,
	ClearRowCleanup
};

UCLASS()
class TETRIS3D_API ATetrisGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetrisGrid();

/**
 *	PROPERTIES
 */
protected:
	class ATetromino * CurrentTetromino;
	class ATetromino * NextTetromino;

	float LapsedTime;
	float MoveLapseTime;
	float MoveLapseDelay;
	float SoftDropMultiplier;
	bool  IsMoveLeftActive;
	bool  IsMoveRightActive;

	bool IsClearing;
	int32 ColumnIndex;
	TArray<int32> RowIndeces;

	TArray<int32> BitMap;
	TArray<class UStaticMeshComponent *> Blocks;

	FVector2D Point;
	EGridState CurrentGridState;

public:
	UPROPERTY(EditDefaultsOnly)
	FVector2D Dimension;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh * BlockStaticMesh;

	UPROPERTY(EditDefaultsOnly)
	float BlockSize;// = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float BlockScale;// = 0.9f;

	UPROPERTY(EditDefaultsOnly)
	float Speed;


/**
 *	FUNCTIONS
 */
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	void TetrominoTickDrop(float DeltaTime);
	void TetrominoMove(float Deltatime);
	void ClearRowAnimation(float DeltaTime);
	void GridCleanup();
	void StartMergeTimer();
	void UpdateTetrominoPosition();

	bool DidHitABlock();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetTetrominoes(ATetromino * PCurrentTetromino, ATetromino * PNextTetromino);

	// Controls
	void HardDrop();
	void SoftDropStart();
	void TetrominoRotateCW();
	void TetrominoRotateCCW();
	void TetrominoMoveLeftStart();
	void TetrominoMoveRightStart();

	void SoftDropEnd();
	void TetrominoMoveLeftEnd();
	void TetrominoMoveRightEnd();

	

};
