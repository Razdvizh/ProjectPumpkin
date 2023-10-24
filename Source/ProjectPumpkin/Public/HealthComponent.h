// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AController;
class AActor;

UENUM(BlueprintType)
enum class EHealthAssignmentResult : uint8
{
	None = 0,
	Negative = 1, //Received negative amount of health
	Overflow = 2, //Received an amount of health that will overflow MaxHealth
	NaN = 3, //Received Not a Number as health amount
	Ok = 4 //No issues, assignment was successful
};
ENUM_RANGE_BY_FIRST_AND_LAST(EHealthAssignmentResult, EHealthAssignmentResult::None, EHealthAssignmentResult::Ok);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLethalHealthReachedEvent);

/*
* Represents actor lifespan based on health points.
*/
UCLASS( ClassGroup=(ProjectPumpkin), meta=(BlueprintSpawnableComponent) )
class PROJECTPUMPKIN_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintCallable, Category = "Health Component", meta = (ToolTip = "Increase current health."))
	EHealthAssignmentResult Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Health Component", meta = (ToolTip = "Decrease current health."))
	EHealthAssignmentResult Damage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	EHealthAssignmentResult SetMaxHealth(float NewMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	EHealthAssignmentResult SetLethalHealth(float NewLethalHealth);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetLethalHealth() const { return LethalHealth; }

protected:
	EHealthAssignmentResult DiagnosticCheck(float Health) const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Health Component", meta = (ToolTip = "Delegate is broadcasted when CurrentHealth is equal or less than LethalHealth"))
	FOnLethalHealthReachedEvent OnLethalHealthReached;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetMaxHealth, Category = "Health Component", meta = (AllowPrivateAccess = true, ToolTip = "Indicates a maximum achievable amount of health. Can be changed during the game."))
	float MaxHealth;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, BlueprintGetter = GetCurrentHealth, Category = "Health Component", meta = (AllowPrivateAccess = true, ToolTip = "Amount of health at this moment. It is always below MaxHealth and can be above or less than LethalHealth."))
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetLethalHealth, Category = "Health Component", meta = (AllowPrivateAccess = true, ToolTip = "Indicates an amount of health at which the actor should be killed or in a death state. Component does not force the actor to be destroyed when this value is reached."))
	float LethalHealth;
};
