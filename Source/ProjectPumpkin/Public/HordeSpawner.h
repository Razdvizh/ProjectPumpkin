// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassSpawner.h"
#include "HordeSpawner.generated.h"

class AActor;
class UActivationVolumeComponent;
struct FHitResult;
struct FTimerHandle;

/*
* Spawns entities in intervals when the instance of specified actor class stays in the activation volume.
*/
UCLASS(Blueprintable)
class PROJECTPUMPKIN_API AHordeSpawner : public AMassSpawner
{
	GENERATED_BODY()
	
public:
	AHordeSpawner();

	UFUNCTION(BlueprintCallable, Category = "Mass")
	void SetSpawnDelay(float Delay);

	UFUNCTION(BlueprintPure, Category = "Mass")
	FORCEINLINE float GetSpawnDelay() const { return SpawnDelay; }

protected:
	virtual void BeginPlay() override;

	virtual void DoSpawningAsync();

	UFUNCTION()
	virtual void OnSpawnerActivate(AActor* Activator);

	UFUNCTION()
	virtual void OnSpawnerDeactivate(AActor* Activator);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mass", meta = (ToolTip = "Area that activates the spawner. Check Activators to specify who can trigger spawning."))
	UActivationVolumeComponent* ActivationVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetSpawnDelay, Category = "Mass", meta = (Units = "s", ClampMin = 0.1f, UIMin = 0.1f, ToolTip = "Delay between spawn intervals of the entities."))
	float SpawnDelay;

private:
	FTimerHandle SpawnIntervalHandle;

};
