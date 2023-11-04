// Fill out your copyright notice in the Description page of Project Settings.

#include "VineSubsystem.h"
#include "SlowingVine.h"
#include "ProjectPumpkin/ProjectPumpkin.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Modules/ModuleManager.h"
#include "ProjectPumpkinSettings.h"

constexpr float NEW_Z_OFFSET = 50.f;

void UVineSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	FProjectPumpkinModule* PumpkinModule = &FModuleManager::LoadModuleChecked<FProjectPumpkinModule>(TEXT("ProjectPumpkin"));

	ActiveVinesRatio = PumpkinModule->GetProjectPumpkinSettings()->GetActiveVinesRatio();

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

		const float NewZLocation = (InWorld.GetWorldSettings()->KillZ) + NEW_Z_OFFSET;
		for (i = FMath::RoundToInt32(Num * ActiveVinesRatio); i < Num; i++)
		{
			ASlowingVine* SlowingVine = SlowingVines[i];
			FVector OriginalLocation = SlowingVine->GetActorLocation();

			InactiveVines.Add(SlowingVine, OriginalLocation);

			SlowingVine->SetActorLocation(FVector(OriginalLocation.X, OriginalLocation.Y, NewZLocation));
		}
	}
}

bool UVineSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || EWorldType::PIE;
}
