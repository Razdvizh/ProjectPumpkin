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

	UFUNCTION(BlueprintCallable, Category = "Vine Subsystem", meta = (ToolTip = "Activates provided slowing vine. Returns result of the activation, false for already active vines. Note that the method will succeed for the vine that is in the process of activation!"))
	bool ActivateVine(ASlowingVine* Vine);

	UFUNCTION(BlueprintCallable, Category = "Vine Subsystem")
	void ActivateVines(TArray<ASlowingVine*> Vines);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Vine Subsystem", meta = (ToolTip = "Writes inactive vines to the Vines array. Note that vines are considered inactive even if they are in the process of being activated!"))
	void GetInactiveVines(TArray<ASlowingVine*>& Vines) const;

	UFUNCTION(BlueprintPure, Category = "Vine Subsystem")
	FORCEINLINE int32 GetInactiveVinesNum() const { return InactiveVines.Num(); }

	UFUNCTION(BlueprintPure, Category = "Vine Subsystem")
	FORCEINLINE bool IsActivationInProgress() const { return IsTickable(); }
	
protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	TMap<ASlowingVine*, TTuple<const FVector, const FVector>> InactiveVines;

private:
	float TimeToActivate;

	float CurrentActivationTime;

	bool bNeedsToTick;

	FRichCurve* ActivationCurve;

	TArray<ASlowingVine*> VinesToActivate;

};
