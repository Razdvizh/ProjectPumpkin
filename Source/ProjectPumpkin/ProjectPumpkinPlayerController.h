// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectPumpkinPlayerController.generated.h"

/*
* Strongly typed player controller for project-wide gamemode
*/
UCLASS()
class AProjectPumpkinPlayerController final : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectPumpkinPlayerController();

private:
	virtual void BeginPlay() override;

};
