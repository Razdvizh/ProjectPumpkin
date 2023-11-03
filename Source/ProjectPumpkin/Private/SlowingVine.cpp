// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowingVine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectPumpkin/ProjectPumpkin.h"
#include "Engine/HitResult.h"

// Sets default values
ASlowingVine::ASlowingVine() 
	: CachedMovementComponent(nullptr),
	SpeedPenalty(70.f)
{
	SlowingVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SlowingVolume"));
	RootComponent = SlowingVolume;
	
	SlowingVolume->SetBoxExtent(FVector(192.f, 192.f, 128.f));

	SlowingVolume->CanCharacterStepUpOn = ECB_No;
	SlowingVolume->SetGenerateOverlapEvents(true);
	SlowingVolume->SetCollisionProfileName(TEXT("Trigger"));
	SlowingVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Projectile, ECR_Overlap);
	
	VineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vine"));

	VineMesh->SetupAttachment(SlowingVolume);
	VineMesh->SetRelativeLocation(FVector(0.f, 0.f, -128.f));

	VineMesh->CanCharacterStepUpOn = ECB_Yes;
	VineMesh->SetGenerateOverlapEvents(false);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASlowingVine::BeginPlay()
{	
	SlowingVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASlowingVine::OnSlowingStarted);
	SlowingVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &ASlowingVine::OnSlowingStopped);

	Super::BeginPlay();
}

void ASlowingVine::OnSlowingStarted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<TSubclassOf<ACharacter>>(SlowingTargetClass))
	{
		ACharacter* Character = Cast<ACharacter>(OtherActor);
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

void ASlowingVine::OnSlowingStopped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CachedMovementComponent)
	{
		CachedMovementComponent->MaxWalkSpeed = CachedMaxSpeed;
		CachedMovementComponent = nullptr;
	}
}
