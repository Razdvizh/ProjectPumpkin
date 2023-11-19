// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectPumpkinPlayerController.h"

AProjectPumpkinPlayerController::AProjectPumpkinPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AProjectPumpkinPlayerController::BeginPlay()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	SetInputMode(InputMode);
}
