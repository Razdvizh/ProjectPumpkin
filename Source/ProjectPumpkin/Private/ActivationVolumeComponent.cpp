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
		OnVolumeActivated.AddUniqueDynamic(AsActivateable, &IActivateable::OnVolumeActivated);
		OnVolumeDeactivated.AddUniqueDynamic(AsActivateable, &IActivateable::OnVolumeDeactivated);
	}

	Super::BeginPlay();
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
	if (bDerivedAreActivators)
	{
		for (const auto& ActivatorClass : ActivatorClasses)
		{
			if (OtherActor->IsA<const TSubclassOf<AActor>>(ActivatorClass))
			{
				OnVolumeActivated.Broadcast(OtherActor);
				return;
			}
		}
	}
	else if (ActivatorClasses.Contains(OtherActor->GetClass()))
	{
		OnVolumeActivated.Broadcast(OtherActor);
	}
}

void UActivationVolumeComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bDerivedAreActivators)
	{
		for (const auto& ActivatorClass : ActivatorClasses)
		{
			if (OtherActor->IsA<const TSubclassOf<AActor>>(ActivatorClass))
			{
				OnVolumeDeactivated.Broadcast(OtherActor);
				return;
			}
		}
	}
	else if (ActivatorClasses.Contains(OtherActor->GetClass()))
	{
		OnVolumeDeactivated.Broadcast(OtherActor);
	}
}
