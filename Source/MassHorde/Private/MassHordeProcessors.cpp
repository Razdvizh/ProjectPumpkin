// Fill out your copyright notice in the Description page of Project Settings.


#include "MassHordeProcessors.h"
#include "MassCommonUtils.h"
#include "MassHordeFragments.h"
#include "MassCommonFragments.h"
#include "MassMovementFragments.h"
#include "MassNavigationFragments.h"
#include "MassExecutionContext.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UChasePlayerProcessor::UChasePlayerProcessor() : EntityQuery(*this)
{
	bAutoRegisterWithProcessingPhases = true;
	bRequiresGameThreadExecution = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
	ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::Avoidance);
}

void UChasePlayerProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassMoveTargetFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddConstSharedRequirement<FPlayerFragment>(EMassFragmentPresence::All);
	EntityQuery.AddConstSharedRequirement<FMassMovementParameters>(EMassFragmentPresence::All);
}

void UChasePlayerProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		const int32 NumEntities = Context.GetNumEntities();

		const TConstArrayView<FTransformFragment> Transforms = Context.GetFragmentView<FTransformFragment>();
		const TArrayView<FMassMoveTargetFragment> MoveTargets = Context.GetMutableFragmentView<FMassMoveTargetFragment>();
		const FPlayerFragment& PlayerFragment = Context.GetConstSharedFragment<FPlayerFragment>();
		const FMassMovementParameters& MovementParams = Context.GetConstSharedFragment<FMassMovementParameters>();

		for (int32 EntityIndex = 0; EntityIndex < NumEntities; EntityIndex++)
		{
			const FTransform& Transform = Transforms[EntityIndex].GetTransform();
			FMassMoveTargetFragment& MoveTarget = MoveTargets[EntityIndex];

			if (AActor* Player = UGameplayStatics::GetActorOfClass(Context.GetWorld(), PlayerFragment.PlayerClass))
			{
				const FVector PlayerLocation = Player->GetActorLocation();
				MoveTarget.Center = PlayerLocation;
				MoveTarget.DistanceToGoal = (MoveTarget.Center - Transform.GetLocation()).Size();
				MoveTarget.Forward = (MoveTarget.Center - Transform.GetLocation()).GetSafeNormal();
				MoveTarget.DesiredSpeed = FMassInt16Real(MovementParams.DefaultDesiredSpeed);
				MoveTarget.IntentAtGoal = EMassMovementAction::Stand;
			}
		}
	});
}
