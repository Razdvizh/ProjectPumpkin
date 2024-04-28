// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivateableActor.h"
#include "SlowingVine.generated.h"

class UStaticMeshComponent;
class UActivationVolumeComponent;
class UCharacterMovementComponent;

UCLASS()
class PROJECTPUMPKIN_API ASlowingVine : public AActivateableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlowingVine();

	UFUNCTION(BlueprintPure, Category = "Vine")
	FORCEINLINE float GetSpeedPenalty() const { return SpeedPenalty; }

	UFUNCTION(BlueprintCallable, Category = "Vine")
	FORCEINLINE void SetSpeedPenalty(float Penalty);

protected:
	virtual void OnActivated(AActor* Activator) override;

	virtual void OnDeactivated(AActor* Activator) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintGetter = GetSpeedPenalty, BlueprintSetter = SetSpeedPenalty, Category = "Vine", meta = (ClampMin = 0.f, UIMin = 0.f, ForceUnits = "cm/s", ToolTip = "The amount by which the character will be slowed down. Be careful with small numbers as character might not be able to escape!"))
	float SpeedPenalty;

private:
	UCharacterMovementComponent* CachedMovementComponent;

	float CachedMaxSpeed;

};
