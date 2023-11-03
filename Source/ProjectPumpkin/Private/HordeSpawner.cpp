// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeSpawner.h"
#include "ProjectPumpkin/ProjectPumpkin.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Engine/HitResult.h"

AHordeSpawner::AHordeSpawner() : SpawnDelay(2.f)
{
	ActivationVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	ActivationVolume->SetupAttachment(RootComponent);
	ActivationVolume->SetBoxExtent(FVector(384.f, 384.f, 128.f));
	
	ActivationVolume->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ActivationVolume->SetGenerateOverlapEvents(true);
	ActivationVolume->SetCollisionProfileName(TEXT("Trigger"));
	ActivationVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Projectile, ECollisionResponse::ECR_Overlap);

	ActivationVolume->OnComponentBeginOverlap.AddDynamic(this, &AHordeSpawner::OnSpawnerActivate);
	ActivationVolume->OnComponentEndOverlap.AddDynamic(this, &AHordeSpawner::OnSpawnerDeactivate);

	PrimaryActorTick.bCanEverTick = false;
	ActivationVolume->PrimaryComponentTick.bCanEverTick = false;
	bAutoSpawnOnBeginPlay = false;
}

void AHordeSpawner::SetActivatorClass(const TSubclassOf<AActor> Class)
{
	ActivatorClass = Class;
}

void AHordeSpawner::SetActivationVolume(UBoxComponent* Volume)
{
	ActivationVolume = Volume;
}

void AHordeSpawner::SetSpawnDelay(float Delay)
{
	checkf(FMath::IsNearlyZero(Delay), TEXT("Delay value: %d will cause unexpected spawner behavior! Expected delay that is greater than 0.1f"), Delay);
	SpawnDelay = Delay;
}

void AHordeSpawner::OnSpawnerActivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<TSubclassOf<AActor>>(ActivatorClass))
	{
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
