// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectPumpkinGameMode.h"
#include "ProjectPumpkinPlayerController.h"
#include "ProjectPumpkinCharacter.h"
#include "ProjectPumpkinGameState.h"
#include "ProjectPumpkinPlayerState.h"
#include "ProjectPumpkinHUD.h"
#include "GameFramework/WorldSettings.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/GameNetworkManager.h"

AProjectPumpkinGameMode::AProjectPumpkinGameMode()
{
	PlayerControllerClass = AProjectPumpkinPlayerController::StaticClass();
	DefaultPawnClass = AProjectPumpkinCharacter::StaticClass();
	HUDClass = AProjectPumpkinPlayerController::StaticClass();
	GameStateClass = AProjectPumpkinGameState::StaticClass();
	PlayerStateClass = AProjectPumpkinPlayerState::StaticClass();
}

void AProjectPumpkinGameMode::PreInitializeComponents()
{
	GetWorldSettings()->GameNetworkManagerClass = nullptr;

	Super::PreInitializeComponents();
}
