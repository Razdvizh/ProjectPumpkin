// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePumpkinPlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"

void AGamePumpkinPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void AGamePumpkinPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AGamePumpkinPlayerController::PauseGame);
	}
}

void AGamePumpkinPlayerController::PauseGame()
{
	if (GetWorld()->IsPaused())
	{
		if (SetPause(false))
		{
			OnGameUnPaused.Broadcast();
		}
		return;
	}

	if (SetPause(true))
	{
		OnGamePaused.Broadcast();
	}
}
