// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Engine/TimerHandle.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent() 
	  : bBurstProtection(false),
	  MaxHealth(100.f),
	  CurrentHealth(100.f),
	  LethalHealth(0.f),
	  DamageDelay(0.f),
	  bTookDamage(false)
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
			CurrentHealth = MaxHealth;
		}
		else
		{
			CurrentHealth += HealAmount;
		}
	}

	return Result;
}

EHealthAssignmentResult UHealthComponent::Damage(float DamageAmount)
{
	const EHealthAssignmentResult Result = DiagnosticCheck(DamageAmount);

	if (Result == EHealthAssignmentResult::Ok)
	{
		if (bBurstProtection && bTookDamage)
		{
			GetWorld()->GetTimerManager().SetTimer(DamageCooldownHandle, this, &UHealthComponent::ResetDamageCooldown, DamageDelay);
			return EHealthAssignmentResult::None;
		}

		CurrentHealth -= DamageAmount;
		bTookDamage = true;

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
		if (FMath::IsNearlyEqual(CurrentHealth, MaxHealth))
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
		if (NewLethalHealth >= MaxHealth)
		{
			return EHealthAssignmentResult::None;
		}

		LethalHealth = NewLethalHealth;
	}

	return Result;
}

#if WITH_EDITOR
void UHealthComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	const FName PropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UHealthComponent, MaxHealth))
	{
		CurrentHealth = MaxHealth;
	}
}
#endif

EHealthAssignmentResult UHealthComponent::DiagnosticCheck(float Health) const
{
	if (FMath::IsNaN(Health))
	{
		return EHealthAssignmentResult::NaN;
	}
	else if (FMath::IsNegativeOrNegativeZero(Health))
	{
		return EHealthAssignmentResult::Negative;
	}

	return EHealthAssignmentResult::Ok;
}

void UHealthComponent::ResetDamageCooldown()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(DamageCooldownHandle))
	{
		TimerManager.ClearTimer(DamageCooldownHandle);
		bTookDamage = false;
	}
}
