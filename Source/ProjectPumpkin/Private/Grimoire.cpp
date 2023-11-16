// Fill out your copyright notice in the Description page of Project Settings.


#include "Grimoire.h"
#include "Components/StaticMeshComponent.h"
#include "ActivationVolumeComponent.h"
#include "MassSpawner.h"
#include "Firepit.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"

// Sets default values
AGrimoire::AGrimoire()
{
	ActivationVolume->GetActivatorClasses().Add(AProjectPumpkinCharacter::StaticClass());

	PrimaryActorTick.bCanEverTick = false;
	Mesh->PrimaryComponentTick.bCanEverTick = false;
	ActivationVolume->PrimaryComponentTick.bCanEverTick = false;
}

void AGrimoire::OnVolumeActivated(AActor* Activator)
{
	if (AProjectPumpkinCharacter* Character = Cast<AProjectPumpkinCharacter>(Activator))
	{
		if (Firepit && BossSpawner)
		{
			BossSpawner->DoSpawning();
			Firepit->MarkAsActivateable();
		}

		Destroy();
	}
}

void AGrimoire::OnVolumeDeactivated(AActor* Activator)
{
	//No op
}
