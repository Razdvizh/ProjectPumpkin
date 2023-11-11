// Fill out your copyright notice in the Description page of Project Settings.


#include "GrowingPumpkin.h"
#include "Components/StaticMeshComponent.h"
#include "ActivationVolumeComponent.h"
#include "HealthComponent.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"
#include "Curves/CurveFloat.h"
#include "Curves/RichCurve.h"
#include "TimerManager.h"

constexpr int32 NUM_STAGES = 3;
constexpr float MEDIUM_HEAL = 1.f;
constexpr float LARGE_HEAL = 2.f;
constexpr double KEY_TIME_TOLERANCE = 1.0e-3;

// Sets default values
AGrowingPumpkin::AGrowingPumpkin()
	: GrowingCurve(),
	GrowingTime(5.f),
	Stage(EGrowingStage::Small),
	CurrentGrowingTime(0.f)
{
	PumpkinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pumpkin"));
	RootComponent = PumpkinMesh;

	PumpkinMesh->SetGenerateOverlapEvents(false);

	InitialScale3D = FVector(0.25f, 0.25f, 0.25f);
	PumpkinMesh->SetRelativeScale3D(InitialScale3D);

	ActivationVolume = CreateDefaultSubobject<UActivationVolumeComponent>(TEXT("ActivationVolume"));
	ActivationVolume->SetupAttachment(RootComponent);

	ActivationVolume->GetActivatorClasses().Add(AProjectPumpkinCharacter::StaticClass());

	StageSizes.Add(EGrowingStage::Small, InitialScale3D);
	StageSizes.Add(EGrowingStage::Medium, FVector(0.75f, 0.75f, 0.75f));
	StageSizes.Add(EGrowingStage::Large, FVector(1.25f, 1.25f, 1.25f));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PumpkinMesh->PrimaryComponentTick.bCanEverTick = false;
}

void AGrowingPumpkin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentGrowingTime < GrowingTime)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(StagePauseHandle))
		{
			CurrentGrowingTime += DeltaTime;
			if (Stage != EGrowingStage::Large)
			{
				const float TimeRatio = FMath::Clamp(CurrentGrowingTime / GrowingTime, 0.f, 1.f);
				const float GrowAlpha = GrowingCurve->GetFloatValue(TimeRatio);

				const auto CheckKeyTime = [this](const FKeyHandle& Key, float Time) -> bool
				{
					return FMath::IsNearlyEqual(GrowingCurve->FloatCurve.GetKeyTime(Key), Time, KEY_TIME_TOLERANCE);
				};

				if (Stage == EGrowingStage::Small)
				{
					const FKeyHandle FirstKey = GrowingCurve->FloatCurve.GetFirstKeyHandle();
					const FKeyHandle SecondKey = GrowingCurve->FloatCurve.GetNextKey(FirstKey);

					const FVector* TargetScale = StageSizes.Find(EGrowingStage::Medium);
					checkf(TargetScale, TEXT("EGrowingState::None was used, it must never be used to inidicate growing state."));
					const FVector CurrentScale = FMath::Lerp(InitialScale3D, *TargetScale, GrowAlpha);

					SetActorScale3D(CurrentScale);

					if (CheckKeyTime(SecondKey, GrowAlpha))
					{
						Stage = EGrowingStage::Medium;
						OnGrowingStageReached.Broadcast(Stage);
					}
				}
				else
				{
					const FKeyHandle ThirdKey = GrowingCurve->FloatCurve.GetLastKeyHandle();

					const FVector* TargetScale = StageSizes.Find(EGrowingStage::Large);
					checkf(TargetScale, TEXT("EGrowingState::None was used, it must never be used to inidicate growing state."));
					const FVector CurrentScale = FMath::Lerp(InitialScale3D, *TargetScale, GrowAlpha);

					SetActorScale3D(CurrentScale);

					if (CheckKeyTime(ThirdKey, GrowAlpha))
					{
						Stage = EGrowingStage::Large;
						OnGrowingStageReached.Broadcast(Stage);
					}
				}
			}
		}
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void AGrowingPumpkin::SetGrowingCurve(UCurveFloat* Curve)
{
	if (Curve && ensureMsgf(Curve->FloatCurve.GetNumKeys() == NUM_STAGES, TEXT("Curve has more or less than %d keys!"), NUM_STAGES))
	{
		GrowingCurve = Curve;
	}
}

void AGrowingPumpkin::SetGrowingTime(float Time)
{
	if (ensureMsgf(!FMath::IsNegativeOrNegativeZero(Time), TEXT("Time value: %d is negative! Pumpkin will use default growing time."), Time))
	{
		GrowingTime = Time;
	}
}

void AGrowingPumpkin::SetPauseBetweenStages(float Time)
{
	if (ensureMsgf(!FMath::IsNegativeOrNegativeZero(Time), TEXT("Time value: %d is negative! Pumpkin will use default pause time."), Time))
	{
		PauseBetweenStages = Time;
	}
}

void AGrowingPumpkin::SetSizeForStage(EGrowingStage InStage, const FVector& InSize)
{
	*StageSizes.Find(InStage) = InSize;
}

#if WITH_EDITOR
void AGrowingPumpkin::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AGrowingPumpkin, StageSizes))
	{
		PumpkinMesh->SetRelativeScale3D(*StageSizes.Find(EGrowingStage::Small));
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AGrowingPumpkin, PumpkinMesh))
	{
		InitialScale3D = PumpkinMesh->GetRelativeScale3D();
	}
}
#endif

// Called when the game starts or when spawned
void AGrowingPumpkin::BeginPlay()
{
	OnGrowingStageReached.AddUniqueDynamic(this, &AGrowingPumpkin::OnStageReached);
	ActivationVolume->OnVolumeActivated.AddUniqueDynamic(this, &AGrowingPumpkin::OnPumpkinActivated);

	Super::BeginPlay();
	
	if (GrowingTime <= 0.f)
	{
		Stage = EGrowingStage::Large;
		SetActorScale3D(*StageSizes.Find(Stage));
	}
}

void AGrowingPumpkin::OnStageReached(EGrowingStage GrowingStage)
{
	GetWorld()->GetTimerManager().SetTimer(StagePauseHandle, PauseBetweenStages, false);
}

void AGrowingPumpkin::OnPumpkinActivated(AActor* Activator)
{
	if (AProjectPumpkinCharacter* Character = Cast<AProjectPumpkinCharacter>(Activator))
	{
		switch(Stage)
		{
		case EGrowingStage::Small:
			break;
		case EGrowingStage::Medium:
			Character->GetHealthComponent()->Heal(MEDIUM_HEAL);
			Destroy();
			break;
		case EGrowingStage::Large:
			Character->GetHealthComponent()->Heal(LARGE_HEAL);
			Destroy();
			break;
		default:
			check(false);
		}
	}
}
