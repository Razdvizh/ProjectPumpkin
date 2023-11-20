// Fill out your copyright notice in the Description page of Project Settings.

#include "VineSubsystem.h"
#include "SlowingVine.h"
#include "ProjectPumpkin/ProjectPumpkin.h"
#include "Curves/CurveFloat.h"
#include "Curves/RichCurve.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Components/SceneComponent.h"
#include "Modules/ModuleManager.h"
#include "ProjectPumpkinSettings.h"

UVineSubsystem::UVineSubsystem()
	: ActivationTime(1.5f),
	CurrentActivationTime(0.f),
	bNeedsToTick(false),
	ActivationCurve()
{
}

void UVineSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	const UProjectPumpkinSettings* ProjectPumpkinSettings = GetDefault<UProjectPumpkinSettings>();

	const float ActiveVinesRatio = ProjectPumpkinSettings->GetActiveVinesRatio();
	const float NewZOffset = ProjectPumpkinSettings->GetNewZOffset();

	ActivationTime = ProjectPumpkinSettings->GetActivationTime();
	ActivationCurve = ProjectPumpkinSettings->GetActivationCurve().GetRichCurve();

	if (ActiveVinesRatio < 1.f)
	{
		TArray<ASlowingVine*> SlowingVines;

		for (TActorIterator<ASlowingVine> It(&InWorld); It; ++It)
		{
			ASlowingVine* const Vine = *It;
			if (Vine->GetRootComponent()->Mobility == EComponentMobility::Movable)
			{
				SlowingVines.Emplace(Vine);
			}
		}

		InactiveVines.Reserve(SlowingVines.Num());

		int32 i;
		const int32 Num = SlowingVines.Num();
		for (i = 0; i < Num; i++)
		{
			//Shuffle array
			const int32 i1 = FMath::RandRange(0, Num - 1);
			const int32 i2 = FMath::RandRange(0, Num - 1);

			SlowingVines.SwapMemory(i1, i2);
		}

		const float NewZLocation = (InWorld.GetWorldSettings()->KillZ) + NewZOffset;
		for (i = FMath::RoundToInt32(Num * ActiveVinesRatio); i < Num; i++)
		{
			ASlowingVine* SlowingVine = SlowingVines[i];
			const FVector OriginalLocation = SlowingVine->GetActorLocation();
			const FVector NewLocation = FVector(OriginalLocation.X, OriginalLocation.Y, NewZLocation);

			InactiveVines.Add(SlowingVine, TTuple<const FVector, const FVector>(OriginalLocation, NewLocation));

			SlowingVine->SetActorLocation(NewLocation);
		}
	}
}

void UVineSubsystem::Tick(float DeltaTime)
{
	UTickableWorldSubsystem::Tick(DeltaTime);

	if (CurrentActivationTime < ActivationTime)
	{
		CurrentActivationTime += DeltaTime;
		for (ASlowingVine* InactiveVine : VinesToActivate)
		{
			const float TimeRatio = FMath::Clamp(CurrentActivationTime / ActivationTime, 0.f, 1.f);
			const float ActivationAlpha = ActivationCurve->Eval(TimeRatio);

			const TTuple<const FVector, const FVector>* Locations = InactiveVines.Find(InactiveVine);
			const FVector TargetLocation = Locations->Key;
			const FVector InitialLocation = Locations->Value;
			const FVector CurrentLocation = FMath::Lerp(InitialLocation, TargetLocation, ActivationAlpha);

			InactiveVine->SetActorLocation(CurrentLocation);
		}
	}
	else
	{
		for (ASlowingVine* InactiveVine : VinesToActivate)
		{
			InactiveVines.Remove(InactiveVine);
		}

		VinesToActivate.Empty();
		bNeedsToTick = false;
	}
}

bool UVineSubsystem::IsTickable() const
{
	return bNeedsToTick;
}

TStatId UVineSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UVineSubsystem, STATGROUP_Tickables);
}

void UVineSubsystem::ActivateAllVines()
{
	if (!bNeedsToTick)
	{
		VinesToActivate.Reserve(InactiveVines.Num());
		for (const auto& InactiveVinePair : InactiveVines)
		{
			VinesToActivate.Emplace(InactiveVinePair.Key);
		}

		bNeedsToTick = true;
	}
}

bool UVineSubsystem::ActivateVine(ASlowingVine* Vine)
{
	const bool bIsInactive = InactiveVines.Contains(Vine);
	if (bIsInactive)
	{
		VinesToActivate.Emplace(Vine);
		bNeedsToTick = true;
	}
	else
	{
		UE_LOG(LogProjectPumpkin, Display, TEXT("%s is already active!"), *Vine->GetFName().ToString());
	}

	return bIsInactive;
}

void UVineSubsystem::ActivateVines(TArray<ASlowingVine*> Vines)
{
	for (const auto Vine : Vines)
	{
		ActivateVine(Vine);
	}
}

void UVineSubsystem::SetActivationTime(float Time)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Time, TEXT("Time value: %f is negative! Vine subsystem will use current time to activate vines value."))

	ActivationTime = Time;
}

void UVineSubsystem::GetInactiveVines(TArray<ASlowingVine*>& Vines) const
{
	for (const auto& InactiveVine : InactiveVines)
	{
		Vines.Emplace(InactiveVine.Key);
	}
}

bool UVineSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}
