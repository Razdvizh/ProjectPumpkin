// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent() 
	  : MaxHealth(100.f),
	  CurrentHealth(100.f),
	  LethalHealth(0.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

EHealthAssignmentResult UHealthComponent::Heal(float HealAmount)
{
	const EHealthAssignmentResult Result = DiagnosticCheck(HealAmount);

	if (Result == EHealthAssignmentResult::Ok)
	{
		const bool bExceedMaxHealth = (HealAmount + CurrentHealth) > MaxHealth;
		if (bExceedMaxHealth)
		{
			return EHealthAssignmentResult::Overflow;
		}

		CurrentHealth += HealAmount;
	}


	return Result;
}

EHealthAssignmentResult UHealthComponent::Damage(float DamageAmount)
{
	const EHealthAssignmentResult Result = DiagnosticCheck(DamageAmount);

	if (Result == EHealthAssignmentResult::Ok)
	{
		CurrentHealth -= DamageAmount;

		if (CurrentHealth <= LethalHealth)
		{
			OnLethalHealthReached.Broadcast();
		}
	}

	return Result;
}

EHealthAssignmentResult UHealthComponent::SetMaxHealth(float NewMaxHealth)
{
	const EHealthAssignmentResult Result = DiagnosticCheck(NewMaxHealth);

	if (Result == EHealthAssignmentResult::Ok)
	{
		if (CurrentHealth == MaxHealth)
		{
			CurrentHealth = NewMaxHealth;
		}

		MaxHealth = NewMaxHealth;
	}

	return Result;
}

EHealthAssignmentResult UHealthComponent::SetLethalHealth(float NewLethalHealth)
{
	const EHealthAssignmentResult Result = DiagnosticCheck(NewLethalHealth);

	if (Result == EHealthAssignmentResult::Ok)
	{
		LethalHealth = NewLethalHealth;
	}

	return Result;
}

EHealthAssignmentResult UHealthComponent::DiagnosticCheck(float Health) const
{
	if (FMath::IsNaN(Health))
	{
		return EHealthAssignmentResult::NaN;
	}
	else if (FMath::IsNegative(Health))
	{
		return EHealthAssignmentResult::Negative;
	}

	return EHealthAssignmentResult::Ok;
}
