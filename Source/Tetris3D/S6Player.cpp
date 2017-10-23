// Fill out your copyright notice in the Description page of Project Settings.

#include "S6Player.h"
#include "Tetromino.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AS6Player::AS6Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	
	// setup spring arm
	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(-2800.0f, 0.0f, 1000.0f), FRotator(0.0f, 0.0f, 0.0f));
	//OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(-100.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	OurCameraSpringArm->bEnableCameraLag = false;//true;
	OurCameraSpringArm->CameraLagSpeed   = 3.0f;
	OurCameraSpringArm->TargetArmLength  = 400.f;

	// setup player camera
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	//OurCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	OurCamera->SetupAttachment(OurCameraSpringArm, USpringArmComponent::SocketName);

	//// Set this pawn to be controlled by the lowest-numbered player
	//AutoPossessPlayer = EAutoReceiveInput::Player0;
	//AutoPossessAI = EAutoPossessAI::Disabled;
	//AIControllerClass = AController::None;
}

// Called when the game starts or when spawned
void AS6Player::BeginPlay()
{
	Super::BeginPlay();

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("S6Player Activated!"));
	//}
}

// Called every frame
void AS6Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Rotate our actor's yaw, which will turn our camera because we're attached to it
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	//if (NextTetromino)
	//{
	//	FVector NewLocation = OurCamera->GetSocketRotation(USpringArmComponent::SocketName).Vector();
	//	NewLocation.X += 100.f;
	//	NewLocation.Y += 100.f;
	//	NextTetromino->SetActorLocation(NewLocation);
	//}

}

//void AS6Player::SetNextTetrominoPreview(ATetromino * PNextTetromino)
//{
//	NextTetromino = PNextTetromino;
//}

// Called to bind functionality to input
//void AS6Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

void AS6Player::YawCamera(float AxisValue)
{
	// update camera rotation x only while holding down left mouse
	if (!bMouseLeftPressed) { return; }
	CameraInput.X = AxisValue * 0.3f;
}

void AS6Player::OnMouseLeftDown()
{
	bMouseLeftPressed = true;
}

void AS6Player::OnMouseLeftUp()
{
	bMouseLeftPressed = false;
}

