// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeBoss.h"
#include "ProjectPumpkin/ProjectPumpkin.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"
#include "Components/SphereComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DamageInfo.h"

AHordeBoss::AHordeBoss()
	: JumpDelay(2.f),
	LandingGravity(-2.f),
	bNeedsToTick(false)
{
	LaunchDistance = 350.f;
	LaunchBoost = 1.f;
	Arc = 1.f;

	ExpandingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExpandingSphere"));
	ExpandingSphere->SetupAttachment(RootComponent);

	FDamageInfo Info{ FDamageEvent(), 2.f };
	DamageInfoMap.Remove(AProjectPumpkinCharacter::StaticClass());
	DamageInfoMap.Add(AProjectPumpkinCharacter::StaticClass(), Info);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AHordeBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bNeedsToTick)
	{
		
	}
}

void AHordeBoss::Interact_Implementation(AActor* Initiator)
{
	if (Initiator->IsA<AProjectile>())
	{
		Super::Interact_Implementation(Initiator);
	}
}

void AHordeBoss::SetJumpDelay(float Delay)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Delay, FString::Printf(TEXT("Delay value: %f is negative! Boss will use default jump delay value."), Delay));

	JumpDelay = Delay;
}

void AHordeBoss::SetLandingGravity(float Gravity)
{
	LandingGravity = Gravity;
}

void AHordeBoss::Jump()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(JumpIntervalHandle))
	{
		LaunchCharacter_CustomArc(LaunchDistance, LaunchBoost, LandingGravity, Arc);

		TimerManager.SetTimer(JumpIntervalHandle, JumpDelay, false);
		bNeedsToTick = true;
	}
}
