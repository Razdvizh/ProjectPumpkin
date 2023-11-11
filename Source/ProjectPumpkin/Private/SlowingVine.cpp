// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowingVine.h"
#include "Components/StaticMeshComponent.h"
#include "ActivationVolumeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Templates/SubclassOf.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"
#include "ProjectPumpkin/ProjectPumpkin.h"

// Sets default values
ASlowingVine::ASlowingVine() 
	: SpeedPenalty(70.f),
	CachedMovementComponent(nullptr),
	CachedMaxSpeed(0.f)

{
	SlowingVolume = CreateDefaultSubobject<UActivationVolumeComponent>(TEXT("SlowingVolume"));
	RootComponent = SlowingVolume;

	SlowingVolume->GetActivatorClasses().Add(AProjectPumpkinCharacter::StaticClass());
	
	VineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vine"));

	VineMesh->SetupAttachment(SlowingVolume);

	VineMesh->CanCharacterStepUpOn = ECB_Yes;
	VineMesh->SetGenerateOverlapEvents(false);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SlowingVolume->PrimaryComponentTick.bCanEverTick = false;
	VineMesh->PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASlowingVine::BeginPlay()
{	
	SlowingVolume->OnVolumeActivated.AddUniqueDynamic(this, &ASlowingVine::OnSlowingStarted);
	SlowingVolume->OnVolumeDeactivated.AddUniqueDynamic(this, &ASlowingVine::OnSlowingStopped);

	Super::BeginPlay();
}

void ASlowingVine::OnSlowingStarted(AActor* Activator)
{
	ACharacter* Character = Cast<ACharacter>(Activator);
	if (ensureMsgf(Character, TEXT("Slowing Vine was activated by %s which is not a character. No slowing will be performed."), *Activator->GetFName().ToString()))
	{
		CachedMovementComponent = Character->GetCharacterMovement();
		CachedMaxSpeed = CachedMovementComponent->MaxWalkSpeed;

		if ((CachedMaxSpeed - SpeedPenalty) > KINDA_SMALL_NUMBER)
		{
			CachedMovementComponent->MaxWalkSpeed -= SpeedPenalty;
		}
		else
		{
			UE_LOG(LogProjectPumpkin, Warning, TEXT("Speed penalty is too high for %s! MaxSpeed received: %d, SpeedPenalty: %d"),
				*Character->GetFName().ToString(), CachedMaxSpeed, SpeedPenalty);
		}
	}
}

void ASlowingVine::OnSlowingStopped(AActor* Activator)
{
	if (CachedMovementComponent)
	{
		CachedMovementComponent->MaxWalkSpeed = CachedMaxSpeed;
		CachedMovementComponent = nullptr;
	}
}
