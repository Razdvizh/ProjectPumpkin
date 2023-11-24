// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeBoss.h"
#include "ProjectPumpkin/ProjectPumpkin.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DamageInfo.h"
#include "Engine/HitResult.h"

constexpr float APPROX_BOSS_PUMPKIN_MESH_RADIUS = 264.f / 2;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
static TAutoConsoleVariable<bool> CVarDebugBossExpandingSphere
(
	TEXT("Pumpkin.Boss.DebugExpandingSphere"),
	false,
	TEXT("Visualizes expanding sphere together with some useful information about it."),
	ECVF_Default
);
#endif

AHordeBoss::AHordeBoss()
	: JumpDelay(2.f),
	ExpandingSpeed(1.f),
	MaxExpansionRadius(APPROX_BOSS_PUMPKIN_MESH_RADIUS * 2),
	bNeedsToTick(false)
{
	CachedGravityScale = GetCharacterMovement()->GravityScale;
	LandingGravityScale = CachedGravityScale;

	ExpandingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExpandingSphere"));
	ExpandingSphere->InitSphereRadius(APPROX_BOSS_PUMPKIN_MESH_RADIUS);
	ExpandingSphere->SetVisibility(false);
	ExpandingSphere->SetupAttachment(RootComponent);

	ExpandingSphereFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	ExpandingSphereFX->SetupAttachment(CharacterMesh, TEXT("S_SlamEffect"));
	ExpandingSphereFX->bAutoActivate = false;

	ExpandingSphere->SetGenerateOverlapEvents(true);
	ExpandingSphere->CanCharacterStepUpOn = ECB_No;
	ExpandingSphere->SetCollisionProfileName(TEXT("Trigger"));
	ExpandingSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Projectile, ECR_Ignore);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	ExpandingSphere->PrimaryComponentTick.bCanEverTick = false;
	ExpandingSphereFX->PrimaryComponentTick.bCanEverTick = true;
}

void AHordeBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (CVarDebugBossExpandingSphere.GetValueOnGameThread())
	{
		ExpandingSphere->SetVisibility(true);
		ExpandingSphere->SetHiddenInGame(false);
		GEngine->AddOnScreenDebugMessage(997, 0.1f, FColor::Emerald, FString::Printf(TEXT("Sphere radius: %f"), ExpandingSphere->GetUnscaledSphereRadius()));
		GEngine->AddOnScreenDebugMessage(998, 0.1f, FColor::Emerald, FString::Printf(TEXT("Max Sphere radius: %f"), MaxExpansionRadius));
		GEngine->AddOnScreenDebugMessage(999, 0.1f, FColor::Emerald, FString::Printf(TEXT("Expanding speed: %f"), ExpandingSpeed));
	}
	#endif

	if (bNeedsToTick)
	{
		const float CurrentRadius = ExpandingSphere->GetUnscaledSphereRadius();
		const float NewRadius = FMath::FInterpTo(CurrentRadius, MaxExpansionRadius, DeltaTime, ExpandingSpeed);

		ExpandingSphere->SetSphereRadius(NewRadius);
	}
	else
	{
		ExpandingSphere->SetSphereRadius(APPROX_BOSS_PUMPKIN_MESH_RADIUS);
		ExpandingSphereFX->DeactivateImmediate();
		GetCharacterMovement()->GravityScale = CachedGravityScale;
	}
}

void AHordeBoss::Interact_Implementation(AActor* Initiator)
{
	if (Initiator->IsA<AProjectPumpkinCharacter>())
	{
		Super::Interact_Implementation(Initiator);
		Jump();
	}
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
		ExpandingSphereFX->ActivateSystem();
		if (GroundImpactSound)
		{
			const FVector SoundLocation = CharacterMesh->GetSocketLocation(TEXT("S_SlamEffect"));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GroundImpactSound, SoundLocation);
		}
		
		bNeedsToTick = true;
	}
	else
	{
		bNeedsToTick = false;
	}
}

void AHordeBoss::SetGroundImpactSound(USoundBase* Sound)
{
	GroundImpactSound = Sound;
}

void AHordeBoss::SetJumpDelay(float Delay)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Delay, FString::Printf(TEXT("Delay value: %f is negative! Boss will use current jump delay value."), Delay));

	JumpDelay = Delay;
}

void AHordeBoss::SetLandingGravity(float Gravity)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Gravity, FString::Printf(TEXT("Gravity value: %f is negative! Boss will use current gravity value."), Gravity));

	LandingGravityScale = Gravity;
}

void AHordeBoss::SetExpandingSpeed(float Speed)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Speed, FString::Printf(TEXT("Speed value: %f is negative! Boss will use current expanding speed value."), Speed));

	ExpandingSpeed = Speed;
}

void AHordeBoss::SetMaxExpansionRadius(float Radius)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Radius, FString::Printf(TEXT("Radius value: %f is negative! Boss will use current expansion radius value."), Radius));

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

	GetCharacterMovement()->GravityScale = LandingGravityScale;
	GetWorld()->GetTimerManager().SetTimer(JumpIntervalHandle, this, &AHordeBoss::Jump, JumpDelay, false);
}

void AHordeBoss::OnExpandingSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UInteractable>())
	{
		IInteractable::Execute_Interact(OtherActor, this);
	}
}
