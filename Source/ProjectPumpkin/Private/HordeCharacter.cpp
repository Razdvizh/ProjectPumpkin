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
#pragma endregion Gameplay
#pragma region Engine
#include "Engine/DamageEvents.h"
#include "Engine/HitResult.h"
#pragma endregion Engine
#pragma region Misc
#include "DamageInfo.h"
#include "Kismet/GameplayStatics.h"
#include "MassEntityManager.h"
#pragma endregion Misc

// Sets default values
AHordeCharacter::AHordeCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName)),
	PushbackDisplacement(-250.f),
	LaunchBoost(1.f),
	Arc(0.25f)
{
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	CharacterMesh->SetupAttachment(GetCapsuleComponent());

	MassAgent = CreateDefaultSubobject<UMassAgentComponent>(TEXT("MassAgent"));

	Health->SetMaxHealth(4.f);
	Health->SetLethalHealth(0.f);

	AActor::OnActorHit.AddDynamic(this, &AHordeCharacter::OnActorHit);

	PrimaryActorTick.bCanEverTick = false;
	CharacterMesh->PrimaryComponentTick.bCanEverTick = false;
	MassAgent->PrimaryComponentTick.bCanEverTick = false;
}

void AHordeCharacter::OnDemise()
{
	UMassHordeHelpers::DestroyMassAgent(MassAgent);
}

void AHordeCharacter::OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA<AProjectPumpkinCharacter>() && !FMath::IsNearlyZero(PushbackDisplacement))
	{
		AProjectPumpkinCharacter* PumpkinCharacter = static_cast<AProjectPumpkinCharacter*>(OtherActor);
		FVector LaunchVelocity;
		const FVector StartPosition = GetActorLocation();
		const FVector EndPosition = (GetActorForwardVector() * PushbackDisplacement) + StartPosition;

		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), LaunchVelocity, StartPosition, EndPosition, /*OverrideGravityZ=*/0.f, Arc);

		LaunchVelocity = LaunchVelocity * LaunchBoost;
		LaunchCharacter(LaunchVelocity, true, true);

		TakeDamage(DamageInfo.DamageAmount, DamageInfo.DamageEvent, PumpkinCharacter->GetController(), OtherActor);
	}
}
