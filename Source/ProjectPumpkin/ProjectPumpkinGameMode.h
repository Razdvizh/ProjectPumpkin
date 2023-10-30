// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectPumpkinGameMode.generated.h"

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
};
