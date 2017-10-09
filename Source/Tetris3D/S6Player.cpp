// Fill out your copyright notice in the Description page of Project Settings.

#include "S6Player.h"
#include "Camera/CameraComponent.h"


// Sets default values
AS6Player::AS6Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// Create a camera and a visible object
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-1500.0f, 0.0f, 200.0f));
	OurCamera->SetRelativeRotation(FRotator(15.0f, 0.0f, 0.0f));
	OurVisibleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AS6Player::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AS6Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*FVector NewLocation = GetActorLocation();
	NewLocation.X -= 1500.0f;
	NewLocation.Z += 300.0f;
	UE_LOG(LogTemp, Display, TEXT("new location: %s"), *NewLocation.ToString());
	SetActorLocation(NewLocation);*/

}

// Called to bind functionality to input
void AS6Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

