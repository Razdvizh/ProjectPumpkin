// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HordeCharacter.h"
#include "HordeBoss.generated.h"

class UShpereComponent;
struct FTimerHandle;

UCLASS()
class PROJECTPUMPKIN_API AHordeBoss : public AHordeCharacter
{
	GENERATED_BODY()
	
public:
	AHordeBoss();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* Initiator) override;

	UFUNCTION(BlueprintPure, Category = "Horde|Boss")
	FORCEINLINE float GetJumpDelay() const { return JumpDelay; }

	UFUNCTION(BlueprintPure, Category = "Horde|Boss")
	FORCEINLINE float GetLandingGravity() const { return LandingGravity; }

	UFUNCTION(BlueprintCallable, Category = "Horde|Boss")
	void SetJumpDelay(float Delay);

	UFUNCTION(BlueprintCallable, Category = "Horde|Boss")
	void SetLandingGravity(float Gravity);
	
protected:
	virtual void Jump() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintGetter = GetJumpDelay, BlueprintSetter = SetJumpDelay, Category = "Horde|Boss", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "s", ToolTip = "Resting time between jumps."))
	float JumpDelay;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetLandingGravity, BlueprintSetter = SetLandingGravity, Category = "Horde|Boss", meta = (ToolTip = "Gravity to use when boss is falling. Zero means world gravityZ will be used."))
	float LandingGravity;

private:
	bool bNeedsToTick;

	USphereComponent* ExpandingSphere;

	FTimerHandle JumpIntervalHandle;

};
