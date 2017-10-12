// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tetris3DGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS3D_API ATetris3DGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

//public:
//	ATetris3DGameModeBase();

protected:
	virtual void StartPlay() override;
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, FVector const & SpawnLocation, FRotator const & SpawnRotation) override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ATetromino> TetrominoBPClass;

	//UPROPERTY(EditAnywhere, Category = "Spawning")
	//TSubclassOf<class AS6PlayerController> S6PlayerControllerBPClass;

	class AS6PlayerController * S6PlayerController;
	class ATetromino * Tetromino;
	
};
