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
* Spawner that spawns entities in intervals when the instance of specified actor class stays in the activation volume.
*/
UCLASS(Blueprintable)
class PROJECTPUMPKIN_API AHordeSpawner : public AMassSpawner
{
	GENERATED_BODY()
	
public:
	AHordeSpawner();

protected:
	UFUNCTION()
	virtual void OnSpawnerActivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSpawnerDeactivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mass|Spawn", meta = (ToolTip = "Class that will trigger the spawn when it overlaps with ActivationVolume"))
	TSubclassOf<AActor> ActivatorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mass|Spawn", meta = (ToolTip = "Collision that will activate the spawner while instance of ActivatorClass stays in the collision"))
	UBoxComponent* ActivationVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mass|Spawn", meta = (Units = "s", ToolTip = "Delay between spawn intervals of the entities"))
	float SpawnDelay;

private:
	FTimerHandle SpawnIntervalHandle;

};
