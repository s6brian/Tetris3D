// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "S6PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS3D_API AS6PlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void SetupInputComponent() override;
	
protected:

	//Input functions
	void OnMouseX(float AxisValue);
	void OnMouseLeftDown();
	void OnMouseLeftUp();
	void OnPressA();
	void OnPressD();
	void OnPressE();
	void OnPressQ();

	
};
