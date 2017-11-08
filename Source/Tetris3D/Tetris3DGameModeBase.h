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

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ATetromino> TetrominoBPClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ATetrisGrid> TetrisGridBPClass;

protected:
	virtual void StartPlay() override;
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, FVector const & SpawnLocation, FRotator const & SpawnRotation) override;

	
};
