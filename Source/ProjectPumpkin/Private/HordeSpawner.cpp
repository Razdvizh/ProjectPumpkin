// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeSpawner.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/HitResult.h"

AHordeSpawner::AHordeSpawner() : SpawnDelay(0.f)
{
	ActivationVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	ActivationVolume->SetupAttachment(RootComponent);
	ActivationVolume->SetBoxExtent(FVector(384.f, 384.f, 128.f));
	
	ActivationVolume->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ActivationVolume->SetGenerateOverlapEvents(true);
	ActivationVolume->SetCollisionProfileName(TEXT("Trigger"));

	ActivationVolume->OnComponentBeginOverlap.AddDynamic(this, &AHordeSpawner::OnSpawnerActivate);
	ActivationVolume->OnComponentEndOverlap.AddDynamic(this, &AHordeSpawner::OnSpawnerDeactivate);

	PrimaryActorTick.bCanEverTick = false;
	bAutoSpawnOnBeginPlay = false;
}

void AHordeSpawner::OnSpawnerActivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<TSubclassOf<AActor>>(ActivatorClass))
	{
		/// \todo: Change this to SpawnGeneratedEntities as there is no need to query new spawn points?
		GetWorld()->GetTimerManager().SetTimer(SpawnIntervalHandle, this, &AMassSpawner::DoSpawning, SpawnDelay, true);
	}
}

void AHordeSpawner::OnSpawnerDeactivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<TSubclassOf<AActor>>(ActivatorClass))
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnIntervalHandle);
	}
}
