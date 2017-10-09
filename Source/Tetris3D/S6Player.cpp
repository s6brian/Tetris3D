// Fill out your copyright notice in the Description page of Project Settings.

#include "S6Player.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


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
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(-1500.0f, 0.0f, 200.0f), FRotator(15.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 400.f;
	OurCameraSpringArm->bEnableCameraLag = false;//true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;

	// setup player camera
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->SetupAttachment(OurCameraSpringArm, USpringArmComponent::SocketName);

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AS6Player::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("S6Player Activated!"));
	}
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

}

// Called to bind functionality to input
void AS6Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MouseLeft", IE_Pressed, this, &AS6Player::OnMouseLeftDown);
	PlayerInputComponent->BindAction("MouseLeft", IE_Released, this, &AS6Player::OnMouseLeftUp);

	PlayerInputComponent->BindAxis("MouseX", this, &AS6Player::YawCamera);
}

void AS6Player::YawCamera(float AxisValue)
{
	// update camera rotation x only while holding down left mouse
	if (!bMouseLeftPressed) { return; }
	CameraInput.X = AxisValue;
}

void AS6Player::OnMouseLeftDown()
{
	bMouseLeftPressed = true;
}

void AS6Player::OnMouseLeftUp()
{
	bMouseLeftPressed = false;
}

