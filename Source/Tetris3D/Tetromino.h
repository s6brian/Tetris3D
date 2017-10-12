// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tetromino.generated.h"

USTRUCT()
struct FTetrominoMatrix
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Size;

	UPROPERTY()
	TArray<int32> VisibilityFlags;

	/*int32* I() const
	{
		int32 DefaultFlags[] = { 0, 0, 1, 0,
								 0, 0, 1, 0,
								 0, 0, 1, 0,
								 0, 0, 1, 0 };
		return DefaultFlags;
	}

	int32* J() const
	{
		int32 DefaultFlags[] = { 0, 1, 0,
								 0, 1, 0,
								 1, 1, 0 };

		return DefaultFlags;
	}

	int32* L() const
	{
		int32 DefaultFlags[] = { 0, 1, 0,
								 0, 1, 0,
								 0, 1, 1 };

		return DefaultFlags;
	}

	int32* O() const
	{
		int32 DefaultFlags[] = { 1, 1,
								 1, 1 };

		return DefaultFlags;
	}

	int32* S() const
	{
		int32 DefaultFlags[] = { 0, 0, 0,
								 0, 1, 1,
								 1, 1, 0 };

		return DefaultFlags;
	}*/

	

	FTetrominoMatrix(){}

	FTetrominoMatrix( const int32 & PSize, ... )
	{
		Size = PSize;

		va_list MatrixValues;
		va_start(MatrixValues, Size);

		for (int idx = 0; idx < Size * Size; idx++)
		{
			VisibilityFlags.Add(va_arg(MatrixValues, int32));
		}

		va_end(MatrixValues);

		//VisibilityFlags.Append(PMatrix, PSize);
	}
};

static const FTetrominoMatrix I = { 4,	0, 0, 1, 0,
										0, 0, 1, 0,
										0, 0, 1, 0,
										0, 0, 1, 0 };

UCLASS()
class TETRIS3D_API ATetromino : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATetromino();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
