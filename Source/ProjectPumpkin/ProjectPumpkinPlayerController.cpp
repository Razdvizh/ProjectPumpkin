// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectPumpkinPlayerController.h"
#include "ProjectPumpkinHUD.h"
#include "FrameWidget.h"

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
	if (AProjectPumpkinHUD* HUD = GetHUD<AProjectPumpkinHUD>())
	{
		if (UFrameWidget* FrameWidget = HUD->GetMainframeWidget())
		{
			InputMode.SetWidgetToFocus(FrameWidget->GetCachedWidget());
		}
	}
	SetInputMode(InputMode);
}
