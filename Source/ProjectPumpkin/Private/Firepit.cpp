// Fill out your copyright notice in the Description page of Project Settings.

#include "Firepit.h"
#include "Components/StaticMeshComponent.h"
#include "ActivationVolumeComponent.h"
#include "NiagaraComponent.h"
#include "ProjectPumpkin/ProjectPumpkinGameMode.h"
#include "ProjectPumpkin/ProjectPumpkinPlayerController.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"

// Sets default values
AFirepit::AFirepit() : bCanBeActivated(false)
{
	FireFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire"));
	FireFX->SetupAttachment(Mesh, TEXT("S_fire"));
	FireFX->bAutoActivate = false;

	ActivationVolume->GetActivatorClasses().Add(AProjectPumpkinCharacter::StaticClass());

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh->PrimaryComponentTick.bCanEverTick = false;
	ActivationVolume->PrimaryComponentTick.bCanEverTick = false;
	FireFX->PrimaryComponentTick.bCanEverTick = true;
}

void AFirepit::BeginPlay()
{
	Super::BeginPlay();
}

void AFirepit::OnActivated(AActor* Activator)
{
	if (bCanBeActivated)
	{
		AProjectPumpkinGameMode::GetPumpkinPlayerController(GetWorld())->ConsoleCommand(TEXT("open Menu"));
	}
}

void AFirepit::OnDeactivated(AActor* Activator)
{
	//No op
}

void AFirepit::MarkAsActivateable()
{
	FireFX->ActivateSystem();
	bCanBeActivated = true;
}
