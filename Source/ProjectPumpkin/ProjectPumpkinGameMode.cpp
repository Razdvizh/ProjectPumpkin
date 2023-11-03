// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectPumpkinGameMode.h"
#include "ProjectPumpkinPlayerController.h"
#include "ProjectPumpkinCharacter.h"
#include "ProjectPumpkinGameState.h"
#include "ProjectPumpkinPlayerState.h"
#include "ProjectPumpkinHUD.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/WorldSettings.h"
#include "GameFramework/PlayerController.h"
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

APlayerController* AProjectPumpkinGameMode::SpawnPlayerControllerCommon(ENetRole InRemoteRole, FVector const& SpawnLocation, FRotator const& SpawnRotation, TSubclassOf<APlayerController> InPlayerControllerClass)
{
	APlayerController* PlayerController = AGameModeBase::SpawnPlayerControllerCommon(InRemoteRole, SpawnLocation, SpawnRotation, InPlayerControllerClass);

	return PlayerController;
}
