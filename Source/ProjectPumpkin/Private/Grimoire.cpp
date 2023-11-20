// Fill out your copyright notice in the Description page of Project Settings.


#include "Grimoire.h"
#include "Components/StaticMeshComponent.h"
#include "ActivationVolumeComponent.h"
#include "HordeSpawner.h"
#include "MassHorde/Public/MassHordeHelpers.h"
#include "Firepit.h"
#include "VineSubsystem.h"
#include "SlowingVine.h"
#include "EngineUtils.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"

// Sets default values
AGrimoire::AGrimoire() 
	: RemainingVinesRatio(0.2f),
	VineActivationTime(1.5f)
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
			BossSpawner->UnloadConfig();
			BossSpawner->DoSpawning();
			Firepit->MarkAsActivateable();

			UVineSubsystem* VineSubsystem = GetWorld()->GetSubsystem<UVineSubsystem>();
			VineSubsystem->SetActivationTime(VineActivationTime);

			if (RemainingVinesRatio <= 0.f)
			{
				VineSubsystem->ActivateAllVines();
			}
			else
			{
				TArray<ASlowingVine*> InactiveVines;
				VineSubsystem->GetInactiveVines(InactiveVines);
				int32 Num = InactiveVines.Num();

				for (int32 i = FMath::RoundToInt32(Num * RemainingVinesRatio); i < Num; i++)
				{
					VineSubsystem->ActivateVine(InactiveVines[i]);
				}
			}
		}

		OnGrimoirePickedUp.Broadcast();
		Destroy();
	}
}

void AGrimoire::OnVolumeDeactivated(AActor* Activator)
{
	//No op
}
