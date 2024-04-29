// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interactable.h"
#if WITH_EDITORONLY_DATA
#include "Components/ArrowComponent.h"
#endif

// Sets default values
AProjectile::AProjectile()
{
	InitialLifeSpan = 3.f;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	RootComponent = Collision;

	Collision->InitSphereRadius(20.5f);
	Collision->CanCharacterStepUpOn = ECB_No;
	Collision->SetGenerateOverlapEvents(true);
	Collision->SetCollisionProfileName(TEXT("Projectile"));

	ProjectileFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileSystem"));

	ProjectileFX->SetupAttachment(Collision);
	ProjectileFX->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileFX->CanCharacterStepUpOn = ECB_No;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Collision);
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->Velocity = FVector(750.f, 0.f, 0.f);

#if WITH_EDITORONLY_DATA
	Arrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Collision);
	Arrow->ArrowColor = FColor::Red;
	Arrow->ArrowSize = 0.7f;
	Arrow->ArrowLength = 70.f;
	Arrow->bTreatAsASprite = true;
	Arrow->SetSimulatePhysics(false);
#endif

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Collision->PrimaryComponentTick.bCanEverTick = false;
	ProjectileFX->PrimaryComponentTick.bCanEverTick = true;
#if WITH_EDITORONLY_DATA
	Arrow->PrimaryComponentTick.bCanEverTick = false;
#endif
}

void AProjectile::OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->Implements<UInteractable>())
	{
		IInteractable::Execute_Interact(OtherActor, this);

		Destroy();
	}
}

void AProjectile::BeginPlay()
{
	AActor::OnActorHit.AddUniqueDynamic(this, &AProjectile::OnActorHit);

	Super::BeginPlay();
}

void AProjectile::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	AActor::OnActorHit.RemoveDynamic(this, &AProjectile::OnActorHit);
}

void AProjectile::LifeSpanExpired()
{
	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		Super::LifeSpanExpired();
	});
}
