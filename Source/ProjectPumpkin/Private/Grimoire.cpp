// Fill out your copyright notice in the Description page of Project Settings.


#include "Grimoire.h"
#include "Components/StaticMeshComponent.h"
#include "ActivationVolumeComponent.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"

// Sets default values
AGrimoire::AGrimoire()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh->PrimaryComponentTick.bCanEverTick = false;
	ActivationVolume->PrimaryComponentTick.bCanEverTick = false;
}

void AGrimoire::OnVolumeActivated(AActor* Activator)
{
	if (AProjectPumpkinCharacter* Character = Cast<AProjectPumpkinCharacter>(Activator))
	{
		Destroy();
		//Bonfire->Lit(); Delegate to game mode?
		//SpawnPumpkinBoss(); Delegate to game mode?
		//DisplayInUI(); Delegate to HUD?
	}
}

void AGrimoire::OnVolumeDeactivated(AActor* Activator)
{
	//No op
}
