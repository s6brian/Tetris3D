// Fill out your copyright notice in the Description page of Project Settings.
#define GLOW_INTENSITY_PARAM_NAME "Glow Intensity"
#define TILE_COLOR_PARAM_NAME "Tile Color"

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
	UMaterialInstanceDynamic * DynamicBlockMat;

public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh * BlockStaticMesh;

	UPROPERTY(EditDefaultsOnly)
	UMaterial * BlockMat;

	UPROPERTY(EditDefaultsOnly)
	float BlockSize = 100.0f;

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

	void RefreshDisplay();
	FLinearColor HSVToRGB(float H, float S, float V);

public:	
	int32 GetSize() const;
	TArray<int32> GetBitmap() const;
	TArray<int32> GetGridIndeces(FVector2D GridDimension, int32 PSidesNum, FVector2D GridPoint) const;
	UMaterialInstanceDynamic * GetDynamicBlockMatInstance();

	void GenerateRandomTetromino();
	void SetBitmap(TArray<int32> NewBitmap);
	void SetBlockRelativeLocationXY(int32 PIndex, FVector2D PLocation);
	void Copy(ATetromino * OtherTetromino);
	//FIntVector4 GetBoundIndex();

	void MoveLeft();
	void MoveRight();
	void RotateCW();
	void RotateCCW();

};
