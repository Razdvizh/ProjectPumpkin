// Fill out your copyright notice in the Description page of Project Settings.

#include "GrowingPumpkin.h"
#include "Components/StaticMeshComponent.h"
#include "ActivationVolumeComponent.h"
#include "HealthComponent.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"
#include "ProjectPumpkin/ProjectPumpkin.h"
#include "Curves/CurveVector.h"
#include "Curves/RichCurve.h"
#include "TimerManager.h"
#if WITH_EDITOR
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#endif

constexpr int32 NUM_STAGES = 3;
constexpr float KEY_TIME_TOLERANCE = 2e-3f;

// Sets default values
AGrowingPumpkin::AGrowingPumpkin()
	: GrowingCurve(),
	GrowingTime(5.f),
	PauseBetweenStages(0.f),
	Stage(EGrowingStage::Small),
	CurrentGrowingTime(0.f),
	bNeedsToTick(false),
	InitialCurveScale(FVector::ZeroVector),
	InitialLocationZ(0)
{
	ActivationVolume->GetActivatorClasses().Add(AProjectPumpkinCharacter::StaticClass());
	StageHeal.Add(EGrowingStage::Medium, 1.f);
	StageHeal.Add(EGrowingStage::Large, 2.f);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	Mesh->PrimaryComponentTick.bCanEverTick = false;
	ActivationVolume->PrimaryComponentTick.bCanEverTick = false;
}

void AGrowingPumpkin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentGrowingTime < GrowingTime && bNeedsToTick)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(StagePauseHandle))
		{
			CurrentGrowingTime += DeltaTime;
			const float TimeRatio = FMath::Clamp(CurrentGrowingTime / GrowingTime, 0.f, 1.f);
				
			const FVector Scale = GrowingCurve->GetVectorValue(TimeRatio);
			SetActorScale3D(Scale);

			//Offset Z position after scaling up.
			FVector ActorLocation = GetActorLocation();
			ActorLocation += FVector(0.f, 0.f, (InitialCurveScale.Z * Scale.Z));
			SetActorLocation(ActorLocation);

			const FRichCurve FirstCurve = GrowingCurve->FloatCurves[0];
			const FKeyHandle FirstKey = FirstCurve.GetFirstKeyHandle();
			const bool bKeyExists = FirstCurve.KeyExistsAtTime(TimeRatio, KEY_TIME_TOLERANCE);
			const bool bIsFirstKey = FirstKey == FirstCurve.FindKey(TimeRatio, KEY_TIME_TOLERANCE);
			if (bKeyExists && !bIsFirstKey)
			{
				switch (Stage)
				{
				case EGrowingStage::None:
					checkf(false, TEXT("EGrowingStage::None was used. It should never be used to indicate growing state."));
					break;
				case EGrowingStage::Small:
					Stage = EGrowingStage::Medium;
					break;
				case EGrowingStage::Medium:
					Stage = EGrowingStage::Large;
					break;
				case EGrowingStage::Large:
					break;
				default:
					check(false);
				}

				OnGrowingStageReached.Broadcast(Stage);
			}
			else if (bIsFirstKey)
			{
				OnGrowingStageReached.Broadcast(Stage);
			}
		}
	}
	else
	{
		bNeedsToTick = false;
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
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Time, FString::Printf(TEXT("Time value: %f is negative! Pumpkin will use current growing time."), Time));

	GrowingTime = Time;
}

void AGrowingPumpkin::SetPauseBetweenStages(float Time)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Time, FString::Printf(TEXT("Time value: %f is negative! Pumpkin will use current pause time."), Time));
		
	PauseBetweenStages = Time;
}

void AGrowingPumpkin::StartGrowth()
{
	const FVector InitialScale = GrowingCurve->GetVectorValue(0.f);
	const FVector LastScale = GrowingCurve->GetVectorValue(1.f);

	if (ensureMsgf(!InitialScale.IsZero() && !LastScale.IsZero(), TEXT("Curve contains negative scale!")))
	{
		if (GrowingTime <= 0.f)
		{
			Stage = EGrowingStage::Large;
			SetActorScale3D(LastScale);
			return;
		}

		if (!InitialScale.Equals(GetActorScale3D()))
		{
			UE_LOG(LogProjectPumpkin, Warning, TEXT("Initial curve scale and actor scale are different, actor will be scaled to match the curve."));
		}

		bNeedsToTick = true;
	}
}

void AGrowingPumpkin::ResetGrowth()
{
	bNeedsToTick = false;
	SetActorScale3D(InitialCurveScale);
	CurrentGrowingTime = 0.f;
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, InitialLocationZ));
	Stage = EGrowingStage::Small;

	OnGrowingStageReached.Broadcast(Stage);
}

#if WITH_EDITOR
void AGrowingPumpkin::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AGrowingPumpkin, GrowingCurve))
	{
		int32 i;
		for (i = 0; i < 3; i++)
		{
			const FRichCurve FloatCurve = GrowingCurve->FloatCurves[i];
			if (FloatCurve.GetNumKeys() != NUM_STAGES)
			{
				FString Message = FString::Printf(TEXT("Curve has less or more than %i keys!"), NUM_STAGES);
				FNotificationInfo Info(FText::FromString(Message));
				Info.FadeInDuration = 0.1f;
				Info.FadeOutDuration = 0.5f;
				Info.ExpireDuration = 3.f;
				Info.bUseThrobber = false;
				Info.bUseSuccessFailIcons = true;
				Info.bUseLargeFont = true;
				Info.bFireAndForget = false;
				Info.bAllowThrottleWhenFrameRateIsLow = true;
				TSharedPtr<SNotificationItem> NotificationItem = FSlateNotificationManager::Get().AddNotification(Info);
				NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
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

	Super::BeginPlay();

	InitialCurveScale = GrowingCurve->GetVectorValue(0.f);
	SetActorScale3D(InitialCurveScale);

	InitialLocationZ = GetActorLocation().Z;

	StartGrowth();
}

void AGrowingPumpkin::OnStageReached(EGrowingStage GrowingStage)
{
	GetWorld()->GetTimerManager().SetTimer(StagePauseHandle, PauseBetweenStages, false);
}

void AGrowingPumpkin::OnVolumeActivated(AActor* Activator)
{
	if (AProjectPumpkinCharacter* Character = Cast<AProjectPumpkinCharacter>(Activator))
	{
		if (float* Health = StageHeal.Find(Stage))
		{
			Character->GetHealthComponent()->Heal(*Health);
		}
		if (Stage == EGrowingStage::Medium || Stage == EGrowingStage::Large)
		{
			Destroy();
		}
	}
}

void AGrowingPumpkin::OnVolumeDeactivated(AActor* Activator)
{
	//No op
}
