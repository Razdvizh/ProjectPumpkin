// Fill out your copyright notice in the Description page of Project Settings.

#include "MassHordeHelpers.h"
#include "MassAgentComponent.h"
#include "MassEntityManager.h"
#include "GameFramework/Actor.h"

void UMassHordeHelpers::DestroyMassAgent(UMassAgentComponent* MassAgent)
{
	const AActor* const Owner = MassAgent->GetOwner();

	const bool bIsSafeToKill = !UE::Mass::Utils::GetEntityManager(Owner->GetWorld())->IsProcessing() && !MassAgent->IsPuppetPendingInitialization();
	if (bIsSafeToKill)
	{
		MassAgent->KillEntity(true);
	}
}
