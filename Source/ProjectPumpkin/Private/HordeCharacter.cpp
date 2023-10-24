// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "MassAgentComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"

// Sets default values
AHordeCharacter::AHordeCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	CharacterMesh->SetupAttachment(GetCapsuleComponent());

	MassAgent = CreateDefaultSubobject<UMassAgentComponent>(TEXT("MassAgent"));

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Health->SetMaxHealth(4.f);
	Health->SetLethalHealth(0.f);
	Health->OnLethalHealthReached.AddDynamic(this, &AHordeCharacter::DestroySelf);

	PrimaryActorTick.bCanEverTick = false;
	CharacterMesh->PrimaryComponentTick.bCanEverTick = false;
	MassAgent->PrimaryComponentTick.bCanEverTick = false;
	Health->PrimaryComponentTick.bCanEverTick = false;
}

float AHordeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const EHealthAssignmentResult Result = Health->Damage(DamageAmount);

	if (Result == EHealthAssignmentResult::Ok)
	{
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	return 0.f;
}

void AHordeCharacter::DestroySelf()
{
	Destroy();
}

void AHordeCharacter::HittedPlayer()
{
	FDamageEvent DamageEvent;
	TakeDamage(1.f, DamageEvent, nullptr, nullptr);
}
