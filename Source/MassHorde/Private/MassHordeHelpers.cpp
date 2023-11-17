// Fill out your copyright notice in the Description page of Project Settings.

#include "MassHordeHelpers.h"
#include "MassAgentComponent.h"
#include "MassEntityManager.h"
#include "MassSpawner.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogHordeHelpers);

void UMassHordeHelpers::DestroyMassAgent(UMassAgentComponent* MassAgent)
{
	const AActor* const Owner = MassAgent->GetOwner();

	const bool bIsSafeToKill = !UE::Mass::Utils::GetEntityManager(Owner->GetWorld())->IsProcessing() && !MassAgent->IsPuppetPendingInitialization();
	if (bIsSafeToKill)
	{
		MassAgent->KillEntity(true);
	}
}

void UMassHordeHelpers::ResetMassSpawnerConfig(AMassSpawner* Spawner)
{
	if (Spawner)
	{
		Spawner->UnloadConfig();
	}
	else
	{
		UE_LOG(LogHordeHelpers, Display, TEXT("Received mass spawner is invalid, no resetting will be performed."));
	}
}
