// Fill out your copyright notice in the Description page of Project Settings.

#include "VineSubsystem.h"
#include "SlowingVine.h"
#include "ProjectPumpkin/ProjectPumpkin.h"
#include "Curves/CurveFloat.h"
#include "Curves/RichCurve.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Modules/ModuleManager.h"
#include "ProjectPumpkinSettings.h"

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
static TAutoConsoleVariable<bool> CVarActivateAllVines(
	TEXT("Pumpkin.ActivateAllVines"),
	false,
	TEXT("Activates all incative vines in the world."),
	ECVF_Default
);
#endif

UVineSubsystem::UVineSubsystem()
	: TimeToActivate(1.5f),
	CurrentActivationTime(0.f),
	bNeedsToTick(false),
	ActivationCurve()
{
}

void UVineSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	FProjectPumpkinModule* PumpkinModule = &FModuleManager::LoadModuleChecked<FProjectPumpkinModule>(TEXT("ProjectPumpkin"));
	const UProjectPumpkinSettings* ProjectPumpkinSettings = PumpkinModule->GetProjectPumpkinSettings();

	const float ActiveVinesRatio = ProjectPumpkinSettings->GetActiveVinesRatio();
	const float NewZOffset = ProjectPumpkinSettings->GetNewZOffset();

	TimeToActivate = ProjectPumpkinSettings->GetTimeToActivate();
	ActivationCurve = ProjectPumpkinSettings->GetActivationCurve();

	if (ActiveVinesRatio < 1.f)
	{
		TArray<ASlowingVine*> SlowingVines;

		for (TActorIterator<ASlowingVine> It(&InWorld); It; ++It)
		{
			SlowingVines.Emplace(*It);
		}

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

	if (CurrentActivationTime < TimeToActivate)
	{
		CurrentActivationTime += DeltaTime;
		for (TPair<ASlowingVine*, TTuple<const FVector, const FVector>>& InactiveVine : InactiveVines)
		{
			const float TimeRatio = FMath::Clamp(CurrentActivationTime / TimeToActivate, 0.f, 1.f);
			const float ActivationAlpha = ActivationCurve.GetRichCurveConst()->Eval(TimeRatio);
			const FVector TargetLocation = InactiveVine.Value.Key;
			const FVector InitialLocation = InactiveVine.Value.Value;
			const FVector CurrentLocation = FMath::Lerp(InitialLocation, TargetLocation, ActivationAlpha);

			InactiveVine.Key->SetActorLocation(CurrentLocation);
		}
	}
	else
	{
		bNeedsToTick = false;
	}
}

bool UVineSubsystem::IsTickable() const
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	bNeedsToTick = bNeedsToTick || CVarActivateAllVines.GetValueOnGameThread();
#endif

	return bNeedsToTick;
}

TStatId UVineSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UVineSubsystem, STATGROUP_Tickables);
}

void UVineSubsystem::ActivateAllVines()
{
	bNeedsToTick = true;
}

bool UVineSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}
