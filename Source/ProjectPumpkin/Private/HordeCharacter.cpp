// Fill out your copyright notice in the Description page of Project Settings.

#pragma region Gameplay
#include "HordeCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"
#include "MassAgentComponent.h"
#include "HealthComponent.h"
#include "MassHordeHelpers.h"
#include "Projectile.h"
#pragma endregion Gameplay
#pragma region Engine
#include "Engine/DamageEvents.h"
#include "Engine/HitResult.h"
#pragma endregion Engine
#pragma region Misc
#include "DamageInfo.h"
#include "Kismet/GameplayStatics.h"
#include "MassEntityManager.h"
#include "ProjectPumpkin/ProjectPumpkin.h"
#pragma endregion Misc

// Sets default values
AHordeCharacter::AHordeCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName)),
	LaunchDisplacement(-250.f),
	LaunchBoost(1.f),
	Arc(0.25f)
{
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	CharacterMesh->SetupAttachment(GetCapsuleComponent());

	MassAgent = CreateDefaultSubobject<UMassAgentComponent>(TEXT("MassAgent"));

	Health->SetMaxHealth(4.f);
	Health->SetLethalHealth(0.f);

	PrimaryActorTick.bCanEverTick = false;
	CharacterMesh->PrimaryComponentTick.bCanEverTick = false;
	MassAgent->PrimaryComponentTick.bCanEverTick = false;
}

void AHordeCharacter::Interact_Implementation(AActor* Initiator)
{
	if (Initiator->IsA<AProjectPumpkinCharacter>())
	{
		AProjectPumpkinCharacter* PumpkinCharacter = static_cast<AProjectPumpkinCharacter*>(Initiator);

		LaunchCharacter_CustomArc(LaunchDisplacement, LaunchBoost, /*OverrideGravityZ=*/0.f, Arc);

		TakeDamage(DamageInfo.DamageAmount, DamageInfo.DamageEvent, PumpkinCharacter->GetController(), PumpkinCharacter);
	}
	else if (Initiator->IsA<AProjectile>())
	{
		TakeDamage(DamageInfo.DamageAmount, DamageInfo.DamageEvent, Initiator->GetInstigatorController(), Initiator);
	}
}

void AHordeCharacter::LaunchCharacter_CustomArc(float InLaunchDisplacement, float InLaunchBoost, float InOverrideGravityZ, float InArc)
{
	FVector LaunchVelocity;
	const FVector StartPosition = GetActorLocation();
	const FVector EndPosition = (GetActorForwardVector() * InLaunchDisplacement) + StartPosition;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), LaunchVelocity, StartPosition, EndPosition, InOverrideGravityZ, InArc);

	LaunchVelocity = LaunchVelocity * InLaunchBoost;
	LaunchCharacter(LaunchVelocity, true, true);
}

void AHordeCharacter::SetLaunchDisplacement(float Displacement)
{
	LaunchDisplacement = Displacement;
}

void AHordeCharacter::SetLaunchBoost(float Boost)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Boost, FString::Printf(TEXT("Boost value: %f is negative! Character will use default boost value."), Boost));
		
	LaunchBoost = Boost;
}

void AHordeCharacter::SetArc(float InArc)
{
	RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(InArc, FString::Printf(TEXT("Arc value: %f is negative! Character will use default arc value."), InArc));
	
	Arc = FMath::Clamp(InArc, 0.f, 1.f);
}

void AHordeCharacter::OnDemise()
{
	UMassHordeHelpers::DestroyMassAgent(MassAgent);
}
