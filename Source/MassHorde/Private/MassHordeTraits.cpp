// Fill out your copyright notice in the Description page of Project Settings.


#include "MassHordeTraits.h"
#include "MassEntityTemplateRegistry.h"
#include "MassHordeFragments.h"
#include "MassMovementFragments.h"
#include "MassEntityUtils.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

void UChasePlayerTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	FMassEntityManager& EntityManager = UE::Mass::Utils::GetEntityManagerChecked(World);

	const FConstSharedStruct PlayerFragment = EntityManager.GetOrCreateConstSharedFragment(Player);
	const FConstSharedStruct MovementParams = EntityManager.GetOrCreateConstSharedFragment(MovementParameters);
	BuildContext.AddConstSharedFragment(PlayerFragment);
	BuildContext.AddConstSharedFragment(MovementParams);
}

void UChasePlayerTrait::ValidateTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	ensureMsgf(Player.PlayerClass,
		TEXT("Player Class is not set for %s, there will be no operation for this trait"), *BuildContext.GetTemplateID().ToString());
}
