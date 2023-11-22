// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MortalCharacter.h"
#include "Interactable.h"
#include "HordeCharacter.generated.h"

class UStaticMeshComponent;
class UMassAgentComponent;
class UHealthComponent;
class AController;
struct FDamageEvent;

UCLASS()
class PROJECTPUMPKIN_API AHordeCharacter : public AMortalCharacter, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHordeCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Interact_Implementation(AActor* Initiator) override;

	UFUNCTION(BlueprintCallable, Category = "Horde|Character")
	FVector GetLaunchVelocity_CustomArc(float InLaunchDisplacement, float InLaunchBoost, float InOverrideGravityZ = 0.f, float InArc = 0.5f);

#pragma region Getters
	UFUNCTION(BlueprintPure, Category = "Horde|Character")
	FORCEINLINE float GetLaunchDistance() const { return LaunchDistance; }

	UFUNCTION(BlueprintPure, Category = "Horde|Character")
	FORCEINLINE float GetLaunchBoost() const { return LaunchBoost; }

	UFUNCTION(BlueprintPure, Category = "Horde|Character")
	FORCEINLINE float GetArc() const { return Arc; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Horde|Character")
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return CharacterMesh; } //ACharacter::GetMesh shadowing
#pragma endregion Getters

	UFUNCTION(BlueprintCallable, Category = "Horde|Character")
	void SetLaunchDistance(float Displacement);

	UFUNCTION(BlueprintCallable, Category = "Horde|Character")
	void SetLaunchBoost(float Boost);

	UFUNCTION(BlueprintCallable, Category = "Horde|Character")
	void SetArc(float InArc);

protected:
	virtual void OnDemise() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere, BlueprintGetter = GetLaunchDistance, BlueprintSetter = SetLaunchDistance, Category = "Horde|Character", meta = (ToolTip = "How far the character will be launched. Zero means no launch."))
	float LaunchDistance;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetLaunchBoost, BlueprintSetter = SetLaunchBoost, Category = "Horde|Character", meta = (ClampMin = 0.f, UIMin = 0.f, ToolTip = "Scale of the character launch velocity."))
	float LaunchBoost;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetArc, BlueprintSetter = SetArc, Category = "Horde|Character", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 1.f, UIMax = 1.f, ToolTip = "Arc that launch should follow. Zero is no launch."))
	float Arc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character")
	UStaticMeshComponent* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde|Character")
	UMassAgentComponent* MassAgent;

};
