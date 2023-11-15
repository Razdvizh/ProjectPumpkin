// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeBoss.h"
#include "ProjectPumpkin/ProjectPumpkin.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DamageInfo.h"
#include "Engine/HitResult.h"

constexpr float APPROX_BOSS_PUMPKIN_MESH_RADIUS = 264.f / 2;

AHordeBoss::AHordeBoss()
	: JumpDelay(2.f),
	LandingGravity(-2.f),
	ExpandingSpeed(1.f),
	MaxExpansionRadius(APPROX_BOSS_PUMPKIN_MESH_RADIUS * 2),
	bNeedsToTick(false)
{
	LaunchDistance = 350.f;
	LaunchBoost = 1.f;
	Arc = 1.f;

	ExpandingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExpandingSphere"));
	ExpandingSphere->InitSphereRadius(APPROX_BOSS_PUMPKIN_MESH_RADIUS);
	ExpandingSphere->SetVisibility(false);
	ExpandingSphere->SetupAttachment(RootComponent);

	ExpandingSphere->SetGenerateOverlapEvents(true);
	ExpandingSphere->CanCharacterStepUpOn = ECB_No;
	ExpandingSphere->SetCollisionProfileName(TEXT("Trigger"));
	ExpandingSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Projectile, ECR_Ignore);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AHordeBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bNeedsToTick)
	{
		const float CurrentRadius = ExpandingSphere->GetUnscaledSphereRadius();
		const float NewRadius = FMath::FInterpTo(CurrentRadius, MaxExpansionRadius, DeltaTime, ExpandingSpeed);

		ExpandingSphere->SetSphereRadius(NewRadius);
	}
	else
	{
		ExpandingSphere->SetSphereRadius(APPROX_BOSS_PUMPKIN_MESH_RADIUS);
	}
}

void AHordeBoss::Interact_Implementation(AActor* Initiator)
{
	if (Initiator->IsA<AProjectile>())
	{
		Super::Interact_Implementation(Initiator);
	}
}

void AHordeBoss::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (PrevMovementMode == EMovementMode::MOVE_Falling)
	{
		bNeedsToTick = true;
	}
	else
	{
		bNeedsToTick = false;
	}
}

void AHordeBoss::SetJumpDelay(float Delay)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Delay, FString::Printf(TEXT("Delay value: %f is negative! Boss will use default jump delay value."), Delay));

	JumpDelay = Delay;
}

void AHordeBoss::SetLandingGravity(float Gravity)
{
	LandingGravity = Gravity;
}

void AHordeBoss::SetExpandingSpeed(float Speed)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Speed, FString::Printf(TEXT("Speed value: %f is negative! Boss will use default expanding speed value."), Speed));

	ExpandingSpeed = Speed;
}

void AHordeBoss::SetMaxExpansionRadius(float Radius)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Radius, FString::Printf(TEXT("Radius value: %f is negative! Boss will use default expansion radius value."), Radius));

	MaxExpansionRadius = Radius;
}

void AHordeBoss::BeginPlay()
{
	ExpandingSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AHordeBoss::OnExpandingSphereOverlap);

	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(JumpIntervalHandle, this, &AHordeBoss::Jump, JumpDelay, false, JumpDelay);
}

void AHordeBoss::Jump()
{
	Super::Jump();

	GetWorld()->GetTimerManager().SetTimer(JumpIntervalHandle, this, &AHordeBoss::Jump, JumpDelay, false);
}

void AHordeBoss::OnExpandingSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UInteractable>())
	{
		IInteractable::Execute_Interact(OtherActor, this);
	}
}
