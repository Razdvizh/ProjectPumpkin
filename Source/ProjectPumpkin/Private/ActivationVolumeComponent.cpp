// Fill out your copyright notice in the Description page of Project Settings.

#include "ActivationVolumeComponent.h"
#include "GameFramework/Actor.h"
#include "Activateable.h"
#include "ProjectPumpkin/ProjectPumpkin.h"

UActivationVolumeComponent::UActivationVolumeComponent(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer),
	bDerivedAreActivators(true)
{
	BoxExtent = FVector(384.0f, 384.0f, 128.0f);
}

void UActivationVolumeComponent::BeginPlay()
{
	OnComponentBeginOverlap.AddUniqueDynamic(this, &UActivationVolumeComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddUniqueDynamic(this, &UActivationVolumeComponent::OnEndOverlap);

	AActor* Owner = GetOwner();
	if (Owner->Implements<UActivateable>())
	{
		IActivateable* AsActivateable = Cast<IActivateable>(Owner);
		OnActivated.AddUniqueDynamic(AsActivateable, &IActivateable::OnActivated);
		OnDeactivated.AddUniqueDynamic(AsActivateable, &IActivateable::OnDeactivated);
	}

	Super::BeginPlay();
}

void UActivationVolumeComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	OnComponentBeginOverlap.RemoveDynamic(this, &UActivationVolumeComponent::OnBeginOverlap);
	OnComponentEndOverlap.RemoveDynamic(this, &UActivationVolumeComponent::OnEndOverlap);

	AActor* Owner = GetOwner();
	if (Owner->Implements<UActivateable>())
	{
		OnActivated.RemoveAll(Owner);
		OnDeactivated.RemoveAll(Owner);
	}
}

void UActivationVolumeComponent::OnRegister()
{
	Super::OnRegister();

	CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	SetGenerateOverlapEvents(true);
	SetCollisionProfileName(TEXT("Trigger"));
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Projectile, ECollisionResponse::ECR_Overlap);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Mouse, ECollisionResponse::ECR_Overlap);

	PrimaryComponentTick.bCanEverTick = false;
}

void UActivationVolumeComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ForActivator(OtherActor, [this](AActor* Actor)
	{
		OnActivated.Broadcast(Actor);
	});
}

void UActivationVolumeComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ForActivator(OtherActor, [this](AActor* Actor) 
	{
		OnDeactivated.Broadcast(Actor);
	});
}

void UActivationVolumeComponent::ForActivator(AActor* OtherActor, TFunction<void(AActor* Actor)> Action)
{
	check(OtherActor);

	if (bDerivedAreActivators)
	{
		for (const auto& ActivatorClass : ActivatorClasses)
		{
			if (OtherActor->IsA<const TSubclassOf<AActor>>(ActivatorClass))
			{
				Action(OtherActor);
				return;
			}
		}
	}
	else if (ActivatorClasses.Contains(OtherActor->GetClass()))
	{
		Action(OtherActor);
	}
}
