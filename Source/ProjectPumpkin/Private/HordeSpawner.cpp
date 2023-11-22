// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeSpawner.h"
#include "ProjectPumpkin/ProjectPumpkin.h"
#include "ActivationVolumeComponent.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"
#include "TimerManager.h"

AHordeSpawner::AHordeSpawner() : SpawnDelay(2.f)
{
	ActivationVolume = CreateDefaultSubobject<UActivationVolumeComponent>(TEXT("ActivationVolume"));
	ActivationVolume->SetupAttachment(RootComponent);

	ActivationVolume->GetActivatorClasses().Add(AProjectPumpkinCharacter::StaticClass());

	PrimaryActorTick.bCanEverTick = false;
	ActivationVolume->PrimaryComponentTick.bCanEverTick = false;
	bAutoSpawnOnBeginPlay = false;
}

void AHordeSpawner::SetSpawnDelay(float Delay)
{
	if (ensureMsgf(!FMath::IsNearlyZero(Delay), TEXT("Delay value: %d will cause unexpected behavior! Expected delay that is greater than 0.1f"), Delay))
	{
		SpawnDelay = Delay;
	}
}

void AHordeSpawner::DoSpawningAsync()
{
	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		DoSpawning();
	});
}

void AHordeSpawner::OnVolumeActivated(AActor* Activator)
{
	/* Need to unload config and clear templates before spawning so that the selected entity type can spawn.
	* Looks like there is a bug in the UMassSpawnerSubsystem that doesn't account for a different entity config if it's coming from a different spawner. 
	* See: UMassHordeHelpers::ResetMassSpawnerConfig
	*/
	UnloadConfig();

	GetWorld()->GetTimerManager().SetTimer(SpawnIntervalHandle, this, &AHordeSpawner::DoSpawningAsync, SpawnDelay, true);
}

void AHordeSpawner::OnVolumeDeactivated(AActor* Activator)
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnIntervalHandle);
}
