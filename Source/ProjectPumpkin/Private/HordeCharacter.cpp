// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "MassAgentComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AHordeCharacter::AHordeCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActualMesh"));
	CharacterMesh->SetupAttachment(GetCapsuleComponent());

	MassAgent = CreateDefaultSubobject<UMassAgentComponent>(TEXT("MassAgent"));
}

void AHordeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

// Called when the game starts or when spawned
void AHordeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
