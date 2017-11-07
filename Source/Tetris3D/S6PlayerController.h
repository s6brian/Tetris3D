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

protected:
	UUserWidget * HUD;
	UUserWidget * PauseMenu;
	

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PauseMenuBP;

protected:
	virtual void SetupInputComponent() override;
	virtual void Possess(APawn* InPawn) override;
	virtual void BeginPlayingState() override;

	//class ATetromino  * Tetromino;
	class ATetrisGrid * TetrisGrid;
	class AS6Player   * S6Player;

	//Input functions
	void OnMouseX(float AxisValue);
	void OnMouseLeftDown();
	void OnMouseLeftUp();

	void OnPressA();
	void OnPressD();
	void OnPressE();
	void OnPressQ();
	void OnPressS();
	void OnPressSpacebar();

	void OnReleaseA();
	void OnReleaseD();
	void OnReleaseS();

	void OnPause();
	void OnDebugScore();

public:
	//void SetTetromino(class ATetromino * PTetromino);
	void SetTetrisGrid(class ATetrisGrid * PTetrisGrid);

	
};
