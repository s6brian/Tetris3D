#pragma once

#include "CoreMinimal.h"
#include "TetrominoMatrix.generated.h"

/**
 * A 1D array of integers representing current tetromino rotation / orientation
 */
USTRUCT(BlueprintType)
struct FTetrominoMatrix
{
	GENERATED_BODY()

private:
	int32 Size;
	TArray<int32> BitMap;

	// TODO: consider bitwise operations for bitmap manipulation
	void TransposeBitMap()
	{
		int32 computedIndex0;
		int32 computedIndex1;
		int32 pointerIndex;

		for (int32 row = 0; row < Size - 1; ++row)
		{
			for (pointerIndex = 0; pointerIndex < Size - row - 1; ++pointerIndex)
			{
				// column index iterator (left to right)
				computedIndex0 = (Size * row) + pointerIndex;
				// row index iterator (top to bottom)
				computedIndex1 = (Size * (Size - pointerIndex - 1)) + (Size - row - 1);

				SwapBitmapAt(computedIndex0, computedIndex1);
			}
		}
	}

	void ReverseBitMapRows()
	{
		int32 computedIndex0;
		int32 computedIndex1;

		for (int32 row = 0; row < Size; ++row)
		{
			for (int32 colStart = 0, colEnd = Size - 1; colStart < colEnd; ++colStart, --colEnd)
			{
				computedIndex0 = (Size * row) + colStart;
				computedIndex1 = (Size * row) + colEnd;

				SwapBitmapAt(computedIndex0, computedIndex1);
			}
		}
	}

	void SwapBitmapAt(int32 IndexA, int32 IndexB)
	{
		BitMap[IndexA] += BitMap[IndexB];
		BitMap[IndexB]  = BitMap[IndexA] - BitMap[IndexB];
		BitMap[IndexA] -= BitMap[IndexB];
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<bool> BoolMap;

	int32 GetSize()
	{
		return Size;
	}

	// returns a BitMap copy
	TArray<int32> GetBitMap()
	{
		return BitMap;
	}

	void SetBitmap(TArray<int32> NewBitmap)
	{
		BitMap = NewBitmap;
	}

	TArray<bool> GetBoolMap()
	{
		return BoolMap;
	}

	void RotateClockwise()
	{
		TransposeBitMap();
		ReverseBitMapRows();
	}

	void RotateCounterClockwise()
	{
		ReverseBitMapRows();
		TransposeBitMap();
	}

	void InitializeBitmap()
	{
		int BoolMapLength = BoolMap.Num();
		// TODO: should check if length is perfect square
		Size = sqrt(BoolMapLength);

		for (int idx = 0; idx < BoolMapLength; ++idx)
		{
			BitMap.Add(BoolMap[idx] ? 1 : 0);
		}

		// we can clear out BoolMap as it is only needed by default shapes instances
		BoolMap.Empty();
	}

	FORCEINLINE FTetrominoMatrix ()
	{
		InitializeBitmap();
	}
};