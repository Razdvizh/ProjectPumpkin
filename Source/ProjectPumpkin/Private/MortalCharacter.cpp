// Fill out your copyright notice in the Description page of Project Settings.

#include "MortalCharacter.h"
#include "HealthComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AMortalCharacter::AMortalCharacter(const FObjectInitializer& ObjectInitialier) : Super(ObjectInitialier)
{
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	
	PrimaryActorTick.bCanEverTick = false;
	Health->PrimaryComponentTick.bCanEverTick = false;
}

float AMortalCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const EHealthAssignmentResult Result = Health->Damage(DamageAmount);

	if (Result == EHealthAssignmentResult::Ok)
	{
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	return 0.f;
}
