// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
class UArrowComponent;

UCLASS(Blueprintable)
class PROJECTPUMPKIN_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Projectile")
	FORCEINLINE USphereComponent* GetProjectileCollision() const { return Collision; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Projectile")
	FORCEINLINE UNiagaraComponent* GetProjectileSystem() const { return ProjectileFX; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Projectile")
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	UFUNCTION()
	void OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	virtual void LifeSpanExpired() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (DisplayName = "ProjectileSystem"))
	UNiagaraComponent* ProjectileFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly)
	UArrowComponent* Arrow;
#endif

};
