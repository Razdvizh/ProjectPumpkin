// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HordeCharacter.generated.h"

class UStaticMeshComponent;
class UMassAgentComponent;
class UHealthComponent;
class AController;
class AActor;
struct FDamageEvent;

UCLASS()
class PROJECTPUMPKIN_API AHordeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHordeCharacter(const FObjectInitializer& ObjectInitializer);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

protected:
	UFUNCTION()
	void DestroySelf();

	UFUNCTION(BlueprintCallable, Category = "Horde|Character", meta = (DevelopmentOnly))
	void HittedPlayer();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character")
	UStaticMeshComponent* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character")
	UMassAgentComponent* MassAgent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character")
	UHealthComponent* Health;
};
