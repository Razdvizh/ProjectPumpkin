// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"
#include "SlowingVine.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class ACharacter;
class AActor;
class UPrimitiveComponent;
class UCharacterMovementComponent;
struct FHitResult;

UCLASS()
class PROJECTPUMPKIN_API ASlowingVine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlowingVine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSlowingStarted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSlowingStopped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vine")
	UStaticMeshComponent* VineMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vine", meta = (ToolTip = "Field that slows down specified target."))
	UBoxComponent* SlowingVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vine", meta = (DisplayName = "Character to slow"))
	TSubclassOf<ACharacter> SlowingTargetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vine", meta = (ClampMin = 0, UIMin = 0, ForceUnits = "cm/s", ToolTip = "the amount by which the character will be slowed down. Be careful with small numbers as character might not be able to escape!"))
	float SpeedPenalty;

private:
	UCharacterMovementComponent* CachedMovementComponent;

	float CachedMaxSpeed;

};
