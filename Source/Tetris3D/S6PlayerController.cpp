// Fill out your copyright notice in the Description page of Project Settings.

#include "S6PlayerController.h"
#include "S6Player.h"
//#include "Tetromino.h"
#include "TetrisGrid.h"
#include "Engine/World.h"

void AS6PlayerController::BeginPlayingState()
{
	S6Player = Cast<AS6Player>(this->GetPawn());
}

//void AS6PlayerController::SetTetromino(ATetromino * PTetromino)
//{
//	Tetromino = PTetromino;
//}

void AS6PlayerController::SetTetrisGrid(ATetrisGrid * PTetrisGrid)
{
	TetrisGrid = PTetrisGrid;
}

void AS6PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MouseLeft", IE_Pressed , this, &AS6PlayerController::OnMouseLeftDown);
	InputComponent->BindAction("MoveLeft" , IE_Pressed , this, &AS6PlayerController::OnPressA       );
	InputComponent->BindAction("MoveRight", IE_Pressed , this, &AS6PlayerController::OnPressD       );
	InputComponent->BindAction("RotateCW" , IE_Pressed , this, &AS6PlayerController::OnPressE       );
	InputComponent->BindAction("RotateCCW", IE_Pressed , this, &AS6PlayerController::OnPressQ       );
	InputComponent->BindAction("SoftDrop" , IE_Pressed , this, &AS6PlayerController::OnPressS       );
	InputComponent->BindAction("HardDrop" , IE_Pressed , this, &AS6PlayerController::OnPressSpacebar);

	InputComponent->BindAction("MouseLeft", IE_Released, this, &AS6PlayerController::OnMouseLeftUp  );
	InputComponent->BindAction("MoveLeft" , IE_Released, this, &AS6PlayerController::OnReleaseA     );
	InputComponent->BindAction("MoveRight", IE_Released, this, &AS6PlayerController::OnReleaseD     );
	InputComponent->BindAction("SoftDrop" , IE_Released, this, &AS6PlayerController::OnReleaseS     );

	InputComponent->BindAxis  ("MouseX"   ,              this, &AS6PlayerController::OnMouseX       );

	
}

void AS6PlayerController::OnMouseX(float AxisValue)
{
	////DisplayLog("Move Left!");
	//if (GEngine && (AxisValue > 0.5f || AxisValue < -0.5f))
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("On Mouse X!"));
	//}

	if (S6Player)
	{
		S6Player->YawCamera(AxisValue);
	}
}

void AS6PlayerController::OnMouseLeftDown()
{
	//bMouseLeftPressed = true;
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("On Left Mouse Down!"));
	//}

	if (S6Player)
	{
		S6Player->OnMouseLeftDown();
	}
}

void AS6PlayerController::OnMouseLeftUp()
{
	//bMouseLeftPressed = false;
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("On Left Mouse Up!"));
	//}

	if (S6Player)
	{
		S6Player->OnMouseLeftUp();
	}
}

void AS6PlayerController::OnPressA()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Pressed A!"));
	//}

	//if (Tetromino)
	//{
	//	Tetromino->MoveLeft();
	//}

	if (TetrisGrid)
	{
		TetrisGrid->TetrominoMoveLeftStart();
	}
}

void AS6PlayerController::OnPressD()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Pressed D!"));
	//}

	//if (Tetromino)
	//{
	//	Tetromino->MoveRight();
	//}

	if (TetrisGrid)
	{
		TetrisGrid->TetrominoMoveRightStart();
	}
}

void AS6PlayerController::OnPressE()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Pressed E!"));
	//}

	//if (Tetromino)
	//{
	//	Tetromino->RotateCW();
	//}

	if (TetrisGrid)
	{
		TetrisGrid->TetrominoRotateCW();
	}
}

void AS6PlayerController::OnPressQ()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Pressed Q!"));
	//}

	//if (Tetromino)
	//{
	//	Tetromino->RotateCCW();
	//}

	if (TetrisGrid)
	{
		TetrisGrid->TetrominoRotateCCW();
	}
}

void AS6PlayerController::OnPressS()
{
	if (TetrisGrid)
	{
		TetrisGrid->SoftDropStart();
	}
}

void AS6PlayerController::OnPressSpacebar()
{
	if (TetrisGrid)
	{
		TetrisGrid->HardDrop();
	}
}

void AS6PlayerController::OnReleaseA()
{
	if (TetrisGrid)
	{
		TetrisGrid->TetrominoMoveLeftEnd();
	}
}

void AS6PlayerController::OnReleaseD()
{
	if (TetrisGrid)
	{
		TetrisGrid->TetrominoMoveRightEnd();
	}
}

void AS6PlayerController::OnReleaseS()
{
	if (TetrisGrid)
	{
		TetrisGrid->SoftDropEnd();
	}
}



