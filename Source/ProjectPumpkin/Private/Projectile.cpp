// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
	Collision->SetCollisionProfileName("Projectile");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
#endif

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Collision->PrimaryComponentTick.bCanEverTick = false;
	Mesh->PrimaryComponentTick.bCanEverTick = false;
	Arrow->PrimaryComponentTick.bCanEverTick = false;
}
