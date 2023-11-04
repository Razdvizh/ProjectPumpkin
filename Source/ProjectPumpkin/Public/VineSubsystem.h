// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VineSubsystem.generated.h"

class UWorld;
class ASlowingVine;

/**
 * Subsystem that randomizes activation of vines on the map.
 */
UCLASS()
class PROJECTPUMPKIN_API UVineSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	float ActiveVinesRatio;

	TMap<ASlowingVine*, FVector> InactiveVines;

};
