// Fill out your copyright notice in the Description page of Project Settings.

#include "S6PlayerController.h"
#include "S6Player.h"
#include "Tetromino.h"
#include "Engine/World.h"

void AS6PlayerController::BeginPlayingState()
{
	S6Player = Cast<AS6Player>(this->GetPawn());

	//for (FConstPawnIterator PawnIterator = GetWorld()->GetPawnIterator(); PawnIterator; PawnIterator++)
	//{
	//	Tetromino = Cast<ATetromino>(*PawnIterator);

	//	if (Tetromino)// && GEngine)
	//	{
	//		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Tetromino->GetName());
	//		break;
	//	}
	//}
}

void AS6PlayerController::SetTetromino(ATetromino * PTetromino)
{
	Tetromino = PTetromino;
}

void AS6PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MouseLeft", IE_Pressed , this, &AS6PlayerController::OnMouseLeftDown);
	InputComponent->BindAction("MouseLeft", IE_Released, this, &AS6PlayerController::OnMouseLeftUp);
	InputComponent->BindAction("MoveLeft" , IE_Pressed , this, &AS6PlayerController::OnPressA);
	InputComponent->BindAction("MoveRight", IE_Pressed , this, &AS6PlayerController::OnPressD);
	InputComponent->BindAction("RotateCW" , IE_Pressed , this, &AS6PlayerController::OnPressE);
	InputComponent->BindAction("RotateCCW", IE_Pressed , this, &AS6PlayerController::OnPressQ);

	InputComponent->BindAxis("MouseX", this, &AS6PlayerController::OnMouseX);

	
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

	if (Tetromino)
	{
		Tetromino->MoveLeft();
	}
}

void AS6PlayerController::OnPressD()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Pressed D!"));
	//}

	if (Tetromino)
	{
		Tetromino->MoveRight();
	}
}

void AS6PlayerController::OnPressE()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Pressed E!"));
	//}

	if (Tetromino)
	{
		Tetromino->RotateCW();
	}
}

void AS6PlayerController::OnPressQ()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Pressed Q!"));
	//}

	if (Tetromino)
	{
		Tetromino->RotateCCW();
	}
}


