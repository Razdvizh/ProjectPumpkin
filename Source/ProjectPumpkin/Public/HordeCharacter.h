// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HordeCharacter.generated.h"

class UStaticMeshComponent;
class UMassAgentComponent;

UCLASS()
class PROJECTPUMPKIN_API AHordeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHordeCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character")
	UStaticMeshComponent* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character")
	UMassAgentComponent* MassAgent;
};
