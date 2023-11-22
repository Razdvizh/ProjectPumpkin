// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectPumpkinGameInstance.h"
#include "Misc/OutputDevice.h"
#include "Engine/World.h"

UProjectPumpkinGameInstance::UProjectPumpkinGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	VolumeMultiplier(1.f)
{
}

PROJECTPUMPKIN_API bool UProjectPumpkinGameInstance::HandleOpenCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld)
{
	VolumeMultiplier = 1.f;

	return Super::HandleOpenCommand(Cmd, Ar, InWorld);
}

PROJECTPUMPKIN_API bool UProjectPumpkinGameInstance::HandleTravelCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld)
{
	VolumeMultiplier = 1.f;

	return Super::HandleTravelCommand(Cmd, Ar, InWorld);
}
