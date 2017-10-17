// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris3DGameStateBase.h"

void ATetris3DGameStateBase::SetTetrominoes(ATetromino * PCurrentTetromino, ATetromino * PNextTetromino)
{
	CurrentTetromino = PCurrentTetromino;
	NextTetromino    = PNextTetromino;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GameState Set Tetrominoes..."));
	}
}


