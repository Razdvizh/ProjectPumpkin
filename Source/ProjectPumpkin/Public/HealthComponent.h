// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AController;
class AActor;
struct FPropertyChangedEvent;
struct FTimerHandle;

UENUM(BlueprintType)
enum class EHealthAssignmentResult : uint8
{
	None = 0, //No assignment
	Negative = 1, //Received negative amount of health, no assignment
	NaN = 2, //Received Not a Number as health amount, no assignment
	Ok = 3 //No issues, assignment was successful
};
ENUM_RANGE_BY_FIRST_AND_LAST(EHealthAssignmentResult, EHealthAssignmentResult::None, EHealthAssignmentResult::Ok);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLethalHealthReachedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedEvent, float, CurrentHealth);

/*
* Represents actor lifespan based on health points. Only unsigned health and damage input is considered valid. 
* Always begins play with full health (current health is always equal to the maximum health).
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPUMPKIN_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintCallable, Category = "Health Component", meta = (ToolTip = "Increase current health. Will yield successful result for non-negative numeric floating-point value. For values over MaxHealth, the CurrentHealth will be set equal to MaxHealth(regain all health points)."))
	EHealthAssignmentResult Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Health Component", meta = (ToolTip = "Decrease current health. Will yield successful result for non-negative numeric floating-point value."))
	EHealthAssignmentResult Damage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	EHealthAssignmentResult SetMaxHealth(float NewMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	EHealthAssignmentResult SetLethalHealth(float NewLethalHealth);

#pragma region Getters
	UFUNCTION(BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetLethalHealth() const { return LethalHealth; }

	UFUNCTION(BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetDamageDelay() const { return DamageDelay; }

	UFUNCTION(BlueprintPure, Category = "Health Component")
	FORCEINLINE bool IsBurstProtected() const { return bBurstProtection; }
#pragma endregion Getters

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	EHealthAssignmentResult DiagnosticCheck(float Health) const;

	void ResetDamageCooldown();

public:
	UPROPERTY(BlueprintAssignable, Category = "Health Component", meta = (ToolTip = "Delegate is broadcasted when CurrentHealth is equal or less than LethalHealth"))
	FOnLethalHealthReachedEvent OnLethalHealthReached;

	UPROPERTY(BlueprintAssignable, Category = "Health Component", meta = (ToolTip = "Delegate is broadcasted when CurrentHealth was changed. Returns the value after the change."))
	FOnHealthChangedEvent OnHealthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = IsBurstProtected, Category = "Health Component", meta = (DisplayAfter = LethalHealth, ToolTip = "Activate the delay between damage inputs"))
	bool bBurstProtection;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetMaxHealth, Category = "Health Component", meta = (AllowPrivateAccess = true, ClampMin = 0.f, UIMin = 0.f, ToolTip = "Indicates a maximum achievable amount of health. Can be changed during the game."))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter = GetCurrentHealth, Category = "Health Component", meta = (AllowPrivateAccess = true, ToolTip = "Amount of health at this moment. It is always below MaxHealth and can be above or less than LethalHealth."))
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetLethalHealth, Category = "Health Component", meta = (AllowPrivateAccess = true, ClampMin = 0.f, UIMin = 0.f, ToolTip = "Indicates an amount of health at which the actor should be killed or in a death state. Component does not force the actor to be destroyed when this value is reached."))
	float LethalHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetDamageDelay, Category = "Health Component", meta = (AllowPrivateAccess = true, EditCondition = bBurstProtection, EditConditionHides, Units = "s", ClampMin = 0.f, UIMin = 0.f, ToolTip = "Cooldown for damage input. New damage input during cooldown will refresh it. Has effect only when burst protection is active."))
	float DamageDelay;

	FTimerHandle DamageCooldownHandle;

	bool bTookDamage;
};
