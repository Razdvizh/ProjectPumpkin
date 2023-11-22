// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectPumpkinGameInstance.generated.h"

class FOutputDevice;
class UWorld;

/**
 * Game instance that stores volume information.
 */
UCLASS()
class UProjectPumpkinGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()

public:
	float VolumeMultiplier;

protected:
	PROJECTPUMPKIN_API virtual bool HandleOpenCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld);

	PROJECTPUMPKIN_API virtual bool HandleTravelCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld);
};
