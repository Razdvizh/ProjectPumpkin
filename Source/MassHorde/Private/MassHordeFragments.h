// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonTypes.h"
#include "MassHordeFragments.generated.h"

class ACharacter;

USTRUCT()
struct MASSHORDE_API FPlayerFragment : public FMassSharedFragment
{
	GENERATED_BODY()

	FPlayerFragment() = default;
	FPlayerFragment(const TSubclassOf<ACharacter>& Class) : PlayerClass(Class) {}

	UPROPERTY(EditAnywhere, Category = "Player Fragment")
	TSubclassOf<ACharacter> PlayerClass;
};
