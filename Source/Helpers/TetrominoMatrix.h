#pragma once

#include "CoreMinimal.h"

/**
 * A 1D array of integers representing current tetromino rotation / orientation
 */
struct FTetrominoMatrix
{

private:
	int32			Size;

	void TransposeBitMap()
	{

	}

	void ReverseBitMapRows()
	{

	}

public:

	TArray<int32>	BitMap;

	static const FTetrominoMatrix I;
	static const FTetrominoMatrix J;
	static const FTetrominoMatrix L;
	static const FTetrominoMatrix S;
	static const FTetrominoMatrix Z;
	static const FTetrominoMatrix T;
	static const FTetrominoMatrix O;

	void RotateClockwise()
	{

	}

	void RotateCounterClockwise()
	{

	}

	FTetrominoMatrix (){}

	FTetrominoMatrix (int32 PSize, ...)
	{
		Size = PSize;
		int32 Length = Size * Size;

		va_list MatrixValues;
		va_start(MatrixValues, PSize);

		for (int idx = 0; idx < Length; idx++)
		{
			BitMap.Add(va_arg(MatrixValues, int32));
		}

		va_end(MatrixValues);
	}
};

/**
 *	3 | 0 0 1 0
 *	2 | 0 0 1 0
 *	1 | 0 0 1 0
 *	0 | 0 0 1 0
 *	  ----------
 *		0 1 2 3
 */
const FTetrominoMatrix FTetrominoMatrix::I(4,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0 );

/**
 *	2 | 0 1 0
 *	1 | 0 1 0
 *	0 | 1 1 0
 *	  --------
 *		0 1 2 
 */
const FTetrominoMatrix FTetrominoMatrix::J(3,
	1, 1, 0,
	0, 1, 0,
	0, 1, 0);

/**
 *	2 | 0 1 0
 *	1 | 0 1 0
 *	0 | 0 1 1
 *	  --------
 *		0 1 2
 */
const FTetrominoMatrix FTetrominoMatrix::L(3,
	0, 1, 1,
	0, 1, 0,
	0, 1, 0);

/**
*	2 | 0 0 0
*	1 | 0 1 1
*	0 | 1 1 0
*	  --------
*		0 1 2
*/
const FTetrominoMatrix FTetrominoMatrix::S(3,
	1, 1, 0,
	0, 1, 1,
	0, 0, 0);

/**
*	2 | 0 0 0
*	1 | 1 1 0
*	0 | 0 1 1
*	  --------
*		0 1 2
*/
const FTetrominoMatrix FTetrominoMatrix::Z(3,
	0, 1, 1,
	1, 1, 0,
	0, 0, 0);

/**
*	2 | 0 0 0
*	1 | 1 1 1
*	0 | 0 1 0
*	  --------
*		0 1 2
*/
const FTetrominoMatrix FTetrominoMatrix::T(3,
	0, 1, 0,
	1, 1, 1,
	0, 0, 0);

/**
*	1 | 1 1
*	0 | 1 1
*	  ------
*		0 1
*/
const FTetrominoMatrix FTetrominoMatrix::O(2,
	1, 1,
	1, 1);

// I, J, L, S, Z, T, O