// Fill out your copyright notice in the Description page of Project Settings.

#include "MortalCharacter.h"
#include "HealthComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AMortalCharacter::AMortalCharacter()
{
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
