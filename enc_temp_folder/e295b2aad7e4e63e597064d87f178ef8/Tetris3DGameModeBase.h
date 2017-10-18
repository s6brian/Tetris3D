// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "Tetris3DGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS3D_API ATetris3DGameModeBase : public AGameMode //Base
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ATetromino> TetrominoBPClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ATetrisGrid> TetrisGridBPClass;

	//UPROPERTY(EditAnywhere, Category = "Spawning")
	//TSubclassOf<class AS6PlayerController> S6PlayerControllerBPClass;

	//class AS6PlayerController * S6PlayerController;
	class ATetromino * CurrentTetromino;
	class ATetromino * NextTetromino;

	//class ATetris3DGameStateBase * TetrisGameState;

protected:
	virtual void StartPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual void SetPlayerDefaults(APawn * PlayerPawn) override;
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, FVector const & SpawnLocation, FRotator const & SpawnRotation) override;

	
};
