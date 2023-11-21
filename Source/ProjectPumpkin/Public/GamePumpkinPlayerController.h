// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPumpkin/ProjectPumpkinPlayerController.h"
#include "GamePumpkinPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnGamePausedSignature, AGamePumpkinPlayerController, OnGamePaused);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnGameUnPausedSignature, AGamePumpkinPlayerController, OnGameUnPaused);

/**
 * Gameplay player controller.
 */
UCLASS()
class PROJECTPUMPKIN_API AGamePumpkinPlayerController : public AProjectPumpkinPlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGamePausedSignature OnGamePaused;

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGameUnPausedSignature OnGameUnPaused;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void PauseGame();

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PauseAction;

};
