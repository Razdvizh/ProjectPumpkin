// Fill out your copyright notice in the Description page of Project Settings.

#include "GrowingPumpkin.h"
#include "Components/StaticMeshComponent.h"
#include "ActivationVolumeComponent.h"
#include "HealthComponent.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"
#include "Curves/CurveVector.h"
#include "Curves/RichCurve.h"
#include "TimerManager.h"
#if WITH_EDITOR
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#endif

constexpr int32 NUM_STAGES = 3;
constexpr float MEDIUM_HEAL = 1.f;
constexpr float LARGE_HEAL = 2.f;

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

	ActivationVolume = CreateDefaultSubobject<UActivationVolumeComponent>(TEXT("ActivationVolume"));
	ActivationVolume->SetupAttachment(RootComponent);

	ActivationVolume->GetActivatorClasses().Add(AProjectPumpkinCharacter::StaticClass());

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
			const float TimeRatio = FMath::Clamp(CurrentGrowingTime / GrowingTime, 0.f, 1.f);
				
			const FVector Scale = GrowingCurve->GetVectorValue(TimeRatio);
			SetActorScale3D(Scale);

			if (GrowingCurve->FloatCurves[0].KeyExistsAtTime(TimeRatio))
			{
				switch (Stage)
				{
				case EGrowingStage::None:
					checkf(false, TEXT("EGrowingStage::None was used. It should never be used to indicate growing state."));
					break;
				case EGrowingStage::Small:
					break;
				case EGrowingStage::Medium:
					Stage = EGrowingStage::Medium;
					break;
				case EGrowingStage::Large:
					Stage = EGrowingStage::Large;
					break;
				default:
					check(false);
				}

				OnGrowingStageReached.Broadcast(Stage);
			}
		}
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void AGrowingPumpkin::SetGrowingCurve(UCurveVector* Curve)
{
	if (Curve)
	{
		for (int32 i = 0; i < 3; i++)
		{
			const FRichCurve FloatCurve = GrowingCurve->FloatCurves[i];
			if (ensureMsgf(FloatCurve.GetNumKeys() == NUM_STAGES, TEXT("Curve has less or more than %i keys!"), NUM_STAGES))
			{
				return;
			}
		}
	}

	GrowingCurve = Curve;
}

void AGrowingPumpkin::SetGrowingTime(float Time)
{
	if (ensureMsgf(!FMath::IsNegativeOrNegativeZero(Time), TEXT("Time value: %f is negative! Pumpkin will use default growing time."), Time))
	{
		GrowingTime = Time;
	}
}

void AGrowingPumpkin::SetPauseBetweenStages(float Time)
{
	if (ensureMsgf(!FMath::IsNegativeOrNegativeZero(Time), TEXT("Time value: %f is negative! Pumpkin will use default pause time."), Time))
	{
		PauseBetweenStages = Time;
	}
}

#if WITH_EDITOR
void AGrowingPumpkin::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AGrowingPumpkin, GrowingCurve))
	{
		for (int32 i = 0; i < 3; i++)
		{
			const FRichCurve FloatCurve = GrowingCurve->FloatCurves[i];
			if (FloatCurve.GetNumKeys() != NUM_STAGES)
			{
				FString Message = FString::Printf(TEXT("Curve has less or more than %i keys!"), NUM_STAGES);
				FNotificationInfo Info(FText::FromString(Message));
				Info.FadeInDuration = 0.1f;
				Info.FadeOutDuration = 0.5f;
				Info.ExpireDuration = 5.f;
				Info.bUseThrobber = false;
				Info.bUseSuccessFailIcons = true;
				Info.bUseLargeFont = true;
				Info.bFireAndForget = false;
				Info.bAllowThrottleWhenFrameRateIsLow = false;
				TSharedPtr<SNotificationItem> NotificationItem = FSlateNotificationManager::Get().AddNotification(Info);
				NotificationItem->SetCompletionState(SNotificationItem::CS_None);
				NotificationItem->ExpireAndFadeout();
				return;
			}
		}
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
		SetActorScale3D(GrowingCurve->GetVectorValue(1.f));
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
