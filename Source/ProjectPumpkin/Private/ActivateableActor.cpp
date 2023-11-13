// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivateableActor.h"
#include "Components/StaticMeshComponent.h"
#include "ActivationVolumeComponent.h"

FName AActivateableActor::MeshName(TEXT("Mesh"));

// Sets default values
AActivateableActor::AActivateableActor(const FObjectInitializer& ObjectInitializer)
{	
	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(MeshName);
	RootComponent = Mesh;

	ActivationVolume = CreateDefaultSubobject<UActivationVolumeComponent>(TEXT("Volume"));
	ActivationVolume->SetupAttachment(RootComponent);

	Mesh->CanCharacterStepUpOn = ECB_No;
	Mesh->SetGenerateOverlapEvents(false);
	
	//Allow in-editor opt-in for tick. Let derived classes override this behavior in their constructors.
	PrimaryActorTick.bStartWithTickEnabled = false;
	Mesh->PrimaryComponentTick.bStartWithTickEnabled = false;
	ActivationVolume->PrimaryComponentTick.bStartWithTickEnabled = false;
}

void AActivateableActor::OnVolumeActivated(AActor* Activator)
{
	IActivateable::OnVolumeActivated(Activator);
}

void AActivateableActor::OnVolumeDeactivated(AActor* Activator)
{
	IActivateable::OnVolumeDeactivated(Activator);
}
