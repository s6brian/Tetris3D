// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "S6Player.generated.h"

UCLASS()
class TETRIS3D_API AS6Player : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AS6Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* OurCameraSpringArm;
	class UCameraComponent* OurCamera;
	
	//Input variables
	//FVector2D MovementInput;
	FVector2D CameraInput;
	bool bMouseLeftPressed;
	/*float ZoomFactor;
	bool bZoomingIn;*/

	//Input functions
	/*void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);*/
	void YawCamera(float AxisValue);
	void OnMouseLeftDown();
	void OnMouseLeftUp();
	/*void ZoomIn();
	void ZoomOut();*/
};
