// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HordeCharacter.h"
#include "HordeBoss.generated.h"

class USphereComponent;
class UPrimitiveComponent;
class UNiagaraComponent;
class USoundBase;
struct FTimerHandle;
struct FHitResult;

UCLASS()
class PROJECTPUMPKIN_API AHordeBoss : public AHordeCharacter
{
	GENERATED_BODY()
	
public:
	AHordeBoss();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* Initiator) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Horde|Boss")
	FORCEINLINE UNiagaraComponent* GetExpandingSphereFX() const { return ExpandingSphereFX; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Horde|Boss")
	FORCEINLINE USoundBase* GetGroundImpactSound() const { return GroundImpactSound; }

	UFUNCTION(BlueprintPure, Category = "Horde|Boss")
	FORCEINLINE float GetJumpDelay() const { return JumpDelay; }

	UFUNCTION(BlueprintPure, Category = "Horde|Boss")
	FORCEINLINE float GetLandingGravity() const { return LandingGravityScale; }

	UFUNCTION(BlueprintPure, Category = "Horde|Boss")
	FORCEINLINE float GetExpandingSpeed() const { return ExpandingSpeed; }

	UFUNCTION(BlueprintPure, Category = "Horde|Boss")
	FORCEINLINE float GetMaxExpansionRadius() const { return MaxExpansionRadius; }

	UFUNCTION(BlueprintCallable, Category = "Horde|Boss")
	void SetGroundImpactSound(USoundBase* Sound);

	UFUNCTION(BlueprintCallable, Category = "Horde|Boss")
	void SetJumpDelay(float Delay);

	UFUNCTION(BlueprintCallable, Category = "Horde|Boss")
	void SetLandingGravity(float Gravity);

	UFUNCTION(BlueprintCallable, Category = "Horde|Boss")
	void SetExpandingSpeed(float Speed);

	UFUNCTION(BlueprintCallable, Category = "Horde|Boss")
	void SetMaxExpansionRadius(float Radius);

protected:
	virtual void BeginPlay() override;

	virtual void Jump() override;

	UFUNCTION()
	virtual void OnExpandingSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Boss")
	UNiagaraComponent* ExpandingSphereFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Boss")
	USoundBase* GroundImpactSound;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetJumpDelay, BlueprintSetter = SetJumpDelay, Category = "Horde|Boss", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "s", ToolTip = "Resting time between jumps. Zero means no jumping."))
	float JumpDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetLandingGravity, Category = "Horde|Boss", meta = (ClampMin = 0.f, UIMin = 0.f, DisplayName = "Gravity multiplier", ToolTip = "Gravity will be multiplied by this value when boss is falling."))
	float LandingGravityScale;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetExpandingSpeed, BlueprintSetter = SetExpandingSpeed, Category = "Horde|Boss", meta = (ClampMin = 0.f, UIMin = 0.f, ForceUnits = "cm/s", ToolTip = "Speed to use when interpolating afterjump sphere's expansion."))
	float ExpandingSpeed;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxExpansionRadius, BlueprintSetter = SetMaxExpansionRadius, Category = "Horde|Boss", meta = (Units = "cm", ToolTip = "Maximum radius that the expanding sphere can reach."))
	float MaxExpansionRadius;
	
private:
	bool bNeedsToTick;

	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* ExpandingSphere;

	float CachedGravityScale;

	FTimerHandle JumpIntervalHandle;

};
