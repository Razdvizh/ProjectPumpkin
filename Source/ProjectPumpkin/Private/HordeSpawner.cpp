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
	checkf(FMath::IsNearlyZero(Delay), TEXT("Delay value: %d will cause unexpected spawner behavior! Expected delay that is greater than 0.1f"), Delay);
	SpawnDelay = Delay;
}

void AHordeSpawner::BeginPlay()
{
	ActivationVolume->OnVolumeActivated.AddUniqueDynamic(this, &AHordeSpawner::OnSpawnerActivate);
	ActivationVolume->OnVolumeDeactivated.AddUniqueDynamic(this, &AHordeSpawner::OnSpawnerDeactivate);

	Super::BeginPlay();
}

void AHordeSpawner::DoSpawningAsync()
{
	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		DoSpawning();
	});
}

void AHordeSpawner::OnSpawnerActivate(AActor* Activator)
{
	GetWorld()->GetTimerManager().SetTimer(SpawnIntervalHandle, this, &AHordeSpawner::DoSpawningAsync, SpawnDelay, true);
}

void AHordeSpawner::OnSpawnerDeactivate(AActor* Activator)
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnIntervalHandle);
}
