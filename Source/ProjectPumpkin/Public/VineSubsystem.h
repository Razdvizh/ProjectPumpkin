// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VineSubsystem.generated.h"

class UWorld;
class ASlowingVine;
struct FRuntimeFloatCurve;
struct TStatId;

/**
 * Subsystem that randomizes activation of vines on the map.
 */
UCLASS()
class PROJECTPUMPKIN_API UVineSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UVineSubsystem();

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable, Category = "Vine Subsystem")
	void ActivateAllVines();
	
protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	TMap<ASlowingVine*, TTuple<const FVector, const FVector>> InactiveVines;

private:
	float TimeToActivate;

	float CurrentActivationTime;

	mutable bool bNeedsToTick;

	FRuntimeFloatCurve ActivationCurve;

	TArray<ASlowingVine*> VinesToActivate;

};
