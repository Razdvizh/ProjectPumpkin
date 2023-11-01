// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassSpawner.h"
#include "Templates/SubclassOf.h"
#include "HordeSpawner.generated.h"

class UBoxComponent;
class UPrimitiveComponent;
class AActor;
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

	UFUNCTION(BlueprintCallable, Category = "Horde|Spawner")
	void SetActivatorClass(const TSubclassOf<AActor> Class);

	UFUNCTION(BlueprintCallable, Category = "Horde|Spawner")
	void SetActivationVolume(UBoxComponent* Volume);

	UFUNCTION(BlueprintCallable, Category = "Horde|Spawner")
	void SetSpawnDelay(float Delay);

	UFUNCTION(BlueprintCallable, Category = "Horde|Spawner")
	FORCEINLINE TSubclassOf<AActor> GetActivatorClass() const { return ActivatorClass; }

	UFUNCTION(BlueprintCallable, Category = "Horde|Spawner")
	FORCEINLINE UBoxComponent* GetActivationVolume() const { return ActivationVolume; }

	UFUNCTION(BlueprintPure, Category = "Horde|Spawner")
	FORCEINLINE float GetSpawnDelay() const { return SpawnDelay; }

protected:
	UFUNCTION()
	virtual void OnSpawnerActivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSpawnerDeactivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Spawner", meta = (DisplayName = "Activator", ToolTip = "Class that will trigger the spawn when it overlaps with ActivationVolume."))
	TSubclassOf<AActor> ActivatorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Spawner", meta = (ToolTip = "Collision that will activate the spawner while instance of ActivatorClass stays in the collision."))
	UBoxComponent* ActivationVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetSpawnDelay, Category = "Horde|Spawner", meta = (Units = "s", ClampMin = 0.1f, UIMin = 0.1f, ToolTip = "Delay between spawn intervals of the entities."))
	float SpawnDelay;

private:
	FTimerHandle SpawnIntervalHandle;

};
