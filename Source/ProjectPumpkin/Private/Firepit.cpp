// Fill out your copyright notice in the Description page of Project Settings.

#include "Firepit.h"
#include "Components/StaticMeshComponent.h"
#include "ActivationVolumeComponent.h"
#include "ProjectPumpkin/ProjectPumpkinGameMode.h"
#include "ProjectPumpkin/ProjectPumpkinPlayerController.h"

// Sets default values
AFirepit::AFirepit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh->PrimaryComponentTick.bCanEverTick = false;
	ActivationVolume->PrimaryComponentTick.bCanEverTick = false;
}

void AFirepit::OnVolumeActivated(AActor* Activator)
{
	AProjectPumpkinGameMode::GetPumpkinPlayerController(GetWorld())->ConsoleCommand(TEXT("quit"));
}

void AFirepit::OnVolumeDeactivated(AActor* Activator)
{
	//No op
}
