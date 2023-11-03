// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
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
	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return Mesh; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Projectile")
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly)
	UArrowComponent* Arrow;
#endif

};
