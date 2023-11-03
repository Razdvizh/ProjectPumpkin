// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Templates/SubclassOf.h"
#include "ProjectPumpkinGameMode.generated.h"

class APlayerController;

/*
* Project-wide gamemode with type safe access to its classes.
*/
UCLASS(minimalapi)
class AProjectPumpkinGameMode final : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectPumpkinGameMode();

	virtual void PreInitializeComponents() override;

protected:
	virtual APlayerController* SpawnPlayerControllerCommon(ENetRole InRemoteRole, FVector const& SpawnLocation, FRotator const& SpawnRotation, TSubclassOf<APlayerController> InPlayerControllerClass) override;

};
