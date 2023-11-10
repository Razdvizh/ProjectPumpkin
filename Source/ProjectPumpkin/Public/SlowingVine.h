// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlowingVine.generated.h"

class AActor;
class UStaticMeshComponent;
class UActivationVolumeComponent;
class UCharacterMovementComponent;

UCLASS()
class PROJECTPUMPKIN_API ASlowingVine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlowingVine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSlowingStarted(AActor* Activator);

	UFUNCTION()
	void OnSlowingStopped(AActor* Activator);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vine")
	UStaticMeshComponent* VineMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vine", meta = (ToolTip = "Field that slows down specified target."))
	UActivationVolumeComponent* SlowingVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vine", meta = (ClampMin = 0, UIMin = 0, ForceUnits = "cm/s", ToolTip = "the amount by which the character will be slowed down. Be careful with small numbers as character might not be able to escape!"))
	float SpeedPenalty;

private:
	UCharacterMovementComponent* CachedMovementComponent;

	float CachedMaxSpeed;

};
