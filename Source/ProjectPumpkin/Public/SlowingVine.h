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

protected:
	virtual void OnVolumeActivated(AActor* Activator) override;

	virtual void OnVolumeDeactivated(AActor* Activator) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetSpeedPenalty, Category = "Vine", meta = (ClampMin = 0, UIMin = 0, ForceUnits = "cm/s", ToolTip = "The amount by which the character will be slowed down. Be careful with small numbers as character might not be able to escape!"))
	float SpeedPenalty;

private:
	UCharacterMovementComponent* CachedMovementComponent;

	float CachedMaxSpeed;

};
