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
	//UPROPERTY(EditAnywhere)
	class USpringArmComponent* OurCameraSpringArm;
	class UCameraComponent* OurCamera;
	//class ATetromino* NextTetromino;

	//Input variables
	FVector2D CameraInput;
	bool bMouseLeftPressed;

	int32 Score;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void AddScore(int32 PDeltaScore);

	UFUNCTION(BlueprintCallable)
	int32 GetScore();

	//Input functions
	void YawCamera(float AxisValue);
	void OnMouseLeftDown();
	void OnMouseLeftUp();



};
