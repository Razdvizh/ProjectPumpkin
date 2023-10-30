// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageInfo.h"
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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UFUNCTION()
	void DestroySelf();

	UFUNCTION()
	void OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character", meta = (ToolTip = "How far and in what direction the character will be pushed. Zero is no pushback."))
	float PushbackDisplacement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character", meta = (ClampMin = 0.f, UIMin = 0.f, ToolTip = "Scale of the character pushback velocity."))
	float LaunchBoost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 1.f, UIMax = 1.f, ToolTip = "Arc that pushback should follow. Zero is up, one is straight forward or backward."))
	float Arc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character")
	UStaticMeshComponent* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character")
	UMassAgentComponent* MassAgent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character")
	UHealthComponent* Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character")
	FDamageInfo DamageInfo;
};
