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
	ActivationVolume->GetActivatorClasses().Add(AProjectPumpkinCharacter::StaticClass());

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh->PrimaryComponentTick.bCanEverTick = false;
	ActivationVolume->PrimaryComponentTick.bCanEverTick = false;
}

void ASlowingVine::OnVolumeActivated(AActor* Activator)
{
	ACharacter* Character = Cast<ACharacter>(Activator);
	if (ensureMsgf(Character, TEXT("Slowing Vine was activated by %s which is not a character. No slowing will be performed."), *Activator->GetFName().ToString()))
	{
		CachedMovementComponent = Character->GetCharacterMovement();
		CachedMaxSpeed = CachedMovementComponent->MaxWalkSpeed;

		const bool bIsSpeedPositive = (CachedMaxSpeed - SpeedPenalty) > KINDA_SMALL_NUMBER;
		if (bIsSpeedPositive)
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

void ASlowingVine::OnVolumeDeactivated(AActor* Activator)
{
	if (CachedMovementComponent)
	{
		CachedMovementComponent->MaxWalkSpeed = CachedMaxSpeed;
		CachedMovementComponent = nullptr;
	}
}
