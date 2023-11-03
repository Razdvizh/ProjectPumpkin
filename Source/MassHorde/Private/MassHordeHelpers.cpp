// Fill out your copyright notice in the Description page of Project Settings.

#include "MassHordeHelpers.h"
#include "MassAgentComponent.h"
#include "MassEntityManager.h"
#include "GameFramework/Actor.h"

void UMassHordeHelpers::DestroyMassAgent(UMassAgentComponent* MassAgent)
{
	const AActor* const Owner = MassAgent->GetOwner();

	if (!UE::Mass::Utils::GetEntityManager(Owner->GetWorld())->IsProcessing())
	{
		MassAgent->KillEntity(true);
	}
}
