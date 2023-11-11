// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "MassHordeFragments.h"
#include "MassMovementFragments.h"
#include "MassHordeTraits.generated.h"

class ACharacter;

UCLASS(meta = (DisplayName = "ChasePlayer"))
class MASSHORDE_API UChasePlayerTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

protected:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;

	virtual void ValidateTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;

	UPROPERTY(EditAnywhere, Category = "Mass|Horde")
	FPlayerFragment Player;

	UPROPERTY(EditAnywhere, Category = "")
	FMassMovementParameters MovementParameters;
};
